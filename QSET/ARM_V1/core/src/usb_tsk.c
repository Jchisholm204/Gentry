/**
 * @file usb_tsk.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"
#include <stdio.h>
#include <memory.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "os/stusb/stm32_compat.h"
#include "os/stusb/stm32_compat.h"
#include "os/stusb/usb.h"
#include "os/stusb/usb_cdc.h"
#include "usb_tsk.h"
#include "systime.h"
#include "usbd/usb_desc.h"

usbd_device udev;

uint8_t read_fifo[VCOM_DATA_SZ];

static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
        usbd_ep_read(dev, ep, read_fifo, VCOM_DATA_SZ);
}

struct systime t_last;

static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
    char msg[0xFF];
    // Send the time out every 1 s
    // if(ep == VCOM_TXD_EP){
        struct systime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        systime_fromTicks(xTaskGetTickCountFromISR(), &time);
        // If a new second has elapsed, send the time
        // if(t_last.secs != time.secs){
            int strsize = sprintf(msg, "EP %d: %s", ep &0x0F, time.str);
            // usbd_ep_unstall(&udev, CDC_TXD_EP);
            usbd_ep_write(&udev, ep, msg, strsize);
        // }
        // NAK if a second has not elapsed
        // else{
        //     usbd_ep_write(&udev, ep, NULL, 0);
        // }
        // t_last.secs = time.secs;
    // }
    // else{
    //     usbd_ep_write(&udev, ep, read_fifo, VCOM_DATA_SZ);
    // }
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep) {
    if (event == usbd_evt_eptx) {
        cdc_txonly(dev, event, ep);
    } else {
        cdc_rxonly(dev, event, ep);
    }
}

struct arm_control ctrl_rx = {0}, ctrl_tx = {
    .control_type = 47U,
    .status = 0,
    .ctrl_tool = 0,
    .limit_sw = 55U,
    {0},
    {0},
};
static void ctrl_rxtx(usbd_device *dev, uint8_t event, uint8_t ep){
    if(event == usbd_evt_eprx){
        usbd_ep_read(dev, ep, &ctrl_rx, sizeof(struct arm_control));
    }
    else{
        usbd_ep_write(dev, ep, &ctrl_tx, sizeof(struct arm_control));
    }
}

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg) {
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

        usbd_reg_endpoint(dev, VCOM_RXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, ctrl_rxtx);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, ctrl_rxtx);

        usbd_ep_write(dev, VCOM_TXD_EP, 0, 0);
        usbd_ep_write(dev, CTRL_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

uint32_t ubuf[CDC_EP0_SIZE];

static void cdc_init_usbd(void) {
    usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
    usbd_reg_config(&udev, cdc_setconf);
    udev_applydesc(&udev);
}

void usb_cdc_init_rcc(void){
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




void vUSB_tsk(void * pvParams){
    (void)pvParams;
    usb_cdc_init_rcc();
    cdc_init_usbd();
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN);
    NVIC_EnableIRQ(OTG_FS_IRQn);
    usbd_enable(&udev, 1);
    usbd_connect(&udev, 1);
    // const char msg[] = "Hello, World!\n";
    const char msg[256] ={0};
    char rx_buf[64];
    vTaskDelete(NULL);
    for(;;){
        // printf("Writing to USB\n");
        // struct ctime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        // int strsize = sprintf(msg, PRINT_CTIME(time));
        // // usbd_ep_unstall(&udev, CDC_TXD_EP);
        // usbd_ep_write(&udev, CDC_TXD_EP, msg, strsize);
        // int32_t rx_len = usbd_ep_read(&udev, CDC_RXD_EP, rx_buf, sizeof(rx_buf));
        // if(rx_len > 0){
        //     usbd_ep_write(&udev, CDC_TXD_EP, rx_buf, rx_len);
        //     printf("USBRX %d bytes: %s\n", rx_len, rx_buf);
        // }
        // vTaskDelay(1000);
    }
}


// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}

