/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief QSET USB Arm Control Board
 * @date 2025-01-19
 * @version 2.2
 * 
 */


#include "main.h"
#include <stdio.h>
#include <string.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "config/FreeRTOSConfig.h"
#include "task.h"
#include "drivers/serial.h"
#include "drivers/canbus.h"
#include "config/pin_cfg.h"
#include "hal/hal_usb.h"

// USB Device Includes
#include "usb_arm_defs.h"
#include "drivers/stusb/usb.h"
#include "usb_desc.h"

#include "mtr_ctrl.h"
#include "test_tsks.h"

#define USB_STACK_SIZE configMINIMAL_STACK_SIZE << 1

// USB Device
usbd_device udev;
uint32_t usb0_buf[CDC_EP0_SIZE]; // EP0 Buffer
// USB Info Packet
volatile static struct udev_mtr_info udev_info;
// USBD Configuration Callback
static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg);

// Motor Controller Handles
static mtrCtrlHndl_t mtrControllers[ARM_N_MOTORS];
// USB Task Handle
static TaskHandle_t usbHndl;
static StackType_t puUsbStack[USB_STACK_SIZE];
static StaticTask_t pxUsbTsk;

// USB Task
void vTskUSB(void *pvParams);

// Initialize all system Interfaces
void Init(void){
    // Initialize the USB Device
    hal_usb_init_rcc();
    // libusb_stm32 init device
    usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, usb0_buf, sizeof(usb0_buf));
    // Apply the device registration function
    usbd_reg_config(&udev, udev_setconf);
    // Apply the USBD Descriptors
    usbd_reg_control(&udev, udev_control);
    usbd_reg_descr(&udev, udev_getdesc);
    
    // Enable USB OTG Interrupt
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN);
    NVIC_EnableIRQ(OTG_FS_IRQn);
    // Enable the USB Device
    usbd_enable(&udev, 1);
    usbd_connect(&udev, 1);

    // Initialize UART
    serial_init(&Serial3, /*baud*/ 250000, PIN_USART3_RX, PIN_USART3_TX);
    // Initialize CAN
    can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);
    // can_init(&CANBus2, CAN_1000KBPS, PIN_CAN2_RX, PIN_CAN2_TX);
    
    /**
     * Initialize System Tasks...
     * All tasks should be initialized as static
     * Tasks can be initialized dynamically, but may crash the system if they
     * overflow the system memory (128Kb for the STM32f446)
     */
    usbHndl = xTaskCreateStatic(
            vTskUSB, "USB", USB_STACK_SIZE, NULL,
            /*Priority*/1, puUsbStack, &pxUsbTsk);

    // Initialize the motor control Tasks
    mtrCtrl_init(&mtrControllers[eJoint1], eJoint1, eAK7010, 0x123);
    mtrCtrl_init(&mtrControllers[eJoint2], eJoint2, eAK7010, 0x124);
    mtrCtrl_init(&mtrControllers[eJoint3], eJoint3, eAK7010, 0x125);
    mtrCtrl_init(&mtrControllers[eJoint4], eJoint4, eAK7010, 0x126);
}

void vTskUSB(void *pvParams){

}

static void ctrl_rx(usbd_device *dev, uint8_t evt, uint8_t ep){

}

static void ctrl_tx(usbd_device *dev, uint8_t evt, uint8_t ep){

}

// USBD RX/TX Callbacks: Control
static void ctrl_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        ctrl_rx(dev, evt, ep);
    else
        ctrl_tx(dev, evt, ep);
}

// USBD RX/TX Callbacks: Virtual COM Port
static void vcom_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx){
        usbd_ep_read(dev, ep, NULL, 0);
    }
    else{
        usbd_ep_write(dev, ep, NULL, 0);
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

        // TODO: Add back these functions
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

// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}

int main(void){

    // Call the init function
    Init();

    // Start Scheduler: Runs tasks initialized above
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        // gpio_write(PIN_LED2, true);
    }
    return 0;
}

