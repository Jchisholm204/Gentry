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

// USB Device
usbd_device udev;
uint32_t usb0_buf[CDC_EP0_SIZE];
static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg);


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
}

// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}

// Static Task Buffers
// Increase N_TSKS to add a new task
#define N_TSKS 7
StackType_t puxTskStack[N_TSKS][configMINIMAL_STACK_SIZE<<1];
StaticTask_t pxTsks[N_TSKS];

int main(void){

    Init();

    /**
     * Initialize System Tasks...
     * All tasks should be initialized as static
     * Tasks can be initialized dynamically, but may crash the system if they
     * overflow the system memory (128Kb for the STM32f446)
     */
    xTaskCreateStatic(vTsk_testOnline, "TestTask", configMINIMAL_STACK_SIZE, NULL,
            /*Priority*/1, puxTskStack[0], &pxTsks[0]);

    // xTaskCreateStatic(vTsk_testUART, "S2 Echo", configMINIMAL_STACK_SIZE, NULL,
    //         1, puxTskStack[1], &pxTsks[1]);

    // xTaskCreateStatic(vTsk_usb, "USB", configMINIMAL_STACK_SIZE<<1, NULL, 
    //         1, puxTskStack[2], &pxTsks[2]);

    // Initialize Motor Control Tasks
    // Pass the motor they control into the task as a (void*)
    // Task will typecast the passed parameter to uint32_t
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL1", configMINIMAL_STACK_SIZE,
    //         (void*)eJoint1, 1, puxTskStack[3], &pxTsks[3]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL2", configMINIMAL_STACK_SIZE,
    //         (void*)J2, 1, puxTskStack[4], &pxTsks[4]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL3", configMINIMAL_STACK_SIZE,
    //         (void*)J3, 1, puxTskStack[5], &pxTsks[5]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL4", configMINIMAL_STACK_SIZE,
    //         (void*)J4, 1, puxTskStack[6], &pxTsks[6]);

    // Start Scheduler: Runs tasks initialized above
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
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
        usbd_reg_endpoint(dev, VCOM_RXD_EP, NULL);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, NULL);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, NULL);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, NULL);

        usbd_ep_write(dev, VCOM_TXD_EP, 0, 0);
        usbd_ep_write(dev, CTRL_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

