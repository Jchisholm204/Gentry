/**
 * @file usb_device.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief QSET USB Device
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <memory.h>
#include "drivers/stusb/stm32_compat.h"
#include "drivers/stusb/usb_cdc.h"
#include "drivers/stusb/usb.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "usbd/usb_device.h"
#include "usbd/usb_desc.h"

#include "config/nvic.h"

// libusb_stm32 device handle
usbd_device udev;
uint32_t ubuf[CDC_EP0_SIZE];
// Incoming and outgoing usb packets
struct udev_pkt_ctrl pkt_ctrl;
struct udev_pkt_status pkt_status;

// VCOM buffers
uint8_t vcom_tx_buf[VCOM_DATA_SZ] = {0};
size_t  vcom_tx_n = {0};
SemaphoreHandle_t vcom_tx_hndl;
StaticSemaphore_t vcom_tx_static_sem;

uint8_t vcom_rx_buf[VCOM_DATA_SZ] = {0};
size_t  vcom_rx_n = {0};
SemaphoreHandle_t vcom_rx_hndl;
StaticSemaphore_t vcom_rx_static_sem;


void udev_setMtr(int mtr, struct udev_mtr_info *pInfo){
    if(mtr >= UDEV_N_MOTORS) return;
    pkt_status.mtr[mtr] = *pInfo;
}

void udev_getMtr(int mtr, struct udev_mtr_ctrl *pCtrl){
    if(mtr >= UDEV_N_MOTORS) return;
    *pCtrl = pkt_ctrl.mtr[mtr];
    // pCtrl->position = pkt_ctrl.mtr[mtr].position;
    // pCtrl->velocity = pkt_ctrl.mtr[mtr].velocity;
}

uint8_t udev_getSrv(int servo){
    if(servo >= UDEV_N_SERVOS) return 0;
    return pkt_ctrl.ctrl_servo[servo];
}

void udev_setLmt(uint8_t lsw){
    pkt_status.limit_sw = lsw;
}

uint8_t udev_getTPR(void){
    return pkt_ctrl.tool_pwr;
}

void udev_setSts(uint8_t sts){
    pkt_status.status = sts;
}

uint8_t udev_getSts(void){
    return pkt_ctrl.status;
}

size_t udev_write(char *data, size_t len){
    size_t remain = VCOM_DATA_SZ - vcom_tx_n;
    size_t i = 0;
    if(xSemaphoreTake(vcom_tx_hndl, 10)){
        for(; i < len && i < remain; i++)
            vcom_tx_buf[vcom_tx_n + i] = data[i];
        xSemaphoreGive(vcom_tx_hndl);
        vcom_tx_n += i;
        return i;
    }
    return 0;
}

size_t udev_read(char *data, size_t len){
    size_t i = 0;
    if(xSemaphoreTake(vcom_rx_hndl, 10)){
        for(; i < len && i < vcom_rx_n; i++)
            data[i] = vcom_rx_buf[i];
        xSemaphoreGive(vcom_rx_hndl);
        return i;
    }
    return 0;

}

static void ctrl_rxtx(usbd_device *dev, uint8_t event, uint8_t ep){
    if(event == usbd_evt_eprx){
        usbd_ep_read(dev, ep, &pkt_ctrl, sizeof(struct udev_pkt_ctrl));
    }
    else{
        usbd_ep_write(dev, ep, &pkt_status, sizeof(struct udev_pkt_status));
    }
}

static void vcom_rxtx(usbd_device *dev, uint8_t event, uint8_t ep){
    if(event == usbd_evt_eprx){
        int byts = usbd_ep_read(dev, ep, &vcom_rx_buf[vcom_rx_n], (uint16_t)(VCOM_DATA_SZ-vcom_rx_n));
        if(byts > 0) vcom_rx_n += (size_t)byts;
    }
    else{
        usbd_ep_write(dev, ep, &vcom_tx_buf, (uint16_t)vcom_tx_n);
        vcom_tx_n = 0;
    }
}

static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        usbd_ep_deconfig(dev, VCOM_NTF_EP);
        usbd_ep_deconfig(dev, VCOM_TXD_EP);
        usbd_ep_deconfig(dev, VCOM_RXD_EP);
        usbd_ep_deconfig(dev, CTRL_NTF_EP);
        usbd_ep_deconfig(dev, CTRL_TXD_EP);
        usbd_ep_deconfig(dev, CTRL_RXD_EP);
        usbd_reg_endpoint(dev, VCOM_RXD_EP, 0);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, 0);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, 0);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, VCOM_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_NTF_EP, USB_EPTYPE_INTERRUPT, VCOM_NTF_SZ);
        usbd_ep_config(dev, CTRL_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CTRL_DATA_SZ);
        usbd_ep_config(dev, CTRL_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CTRL_DATA_SZ);
        usbd_ep_config(dev, CTRL_NTF_EP, USB_EPTYPE_INTERRUPT, CTRL_NTF_SZ);

        usbd_reg_endpoint(dev, VCOM_RXD_EP, vcom_rxtx);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, vcom_rxtx);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, ctrl_rxtx);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, ctrl_rxtx);

        usbd_ep_write(dev, VCOM_TXD_EP, 0, 0);
        usbd_ep_write(dev, CTRL_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}



void udev_init_rcc(void){
    /* enabling GPIOA and setting PA11 and PA12 to AF10 (USB_FS) */
    #if defined(USBD_PRIMARY_OTGHS)
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    _BST(GPIOB->AFR[1], (0x0C << 24) | (0x0C << 28));
    _BMD(GPIOB->MODER, (0x03 << 28) | (0x03 << 30), (0x02 << 28) | (0x02 << 30));
    #else
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    _BST(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    _BMD(GPIOA->MODER, (0x03 << 22) | (0x03 << 24), (0x02 << 22) | (0x02 << 24));
    #endif
}


void udev_init(void){
    // Initialize the VCOM Semaphores
    vcom_tx_hndl = xSemaphoreCreateMutexStatic(&vcom_tx_static_sem);
    vcom_rx_hndl = xSemaphoreCreateMutexStatic(&vcom_rx_static_sem);
    // Enable the USBD RCC
    udev_init_rcc();
    // libusb_stm32 init device
    usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
    // Apply the device registration function
    usbd_reg_config(&udev, udev_setconf);
    // Apply the USBD Descriptors
    udev_applydesc(&udev);
    // Enable USB OTG Interrupt
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN);
    NVIC_EnableIRQ(OTG_FS_IRQn);
    // Enable the USB Device
    usbd_enable(&udev, 1);
    usbd_connect(&udev, 1);
}

// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}
