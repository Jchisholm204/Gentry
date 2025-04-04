/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief QSET Payload Control Board
 * @date 2025-02-24
 * @version 2.2
 * 
 */


#include "main.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "config/FreeRTOSConfig.h"
#include "task.h"
#include "drivers/serial.h"
#include "drivers/canbus.h"
#include "config/pin_cfg.h"
#include "hal/hal_usb.h"
#include "systime.h"

// USB Device Includes
#include "usb_payload_defs.h"
#include "usb_packet.h"
#include "drivers/stusb/usb.h"
#include "usb_desc.h"

#include "test_tsks.h"
#include "servo_ctrl.h"

#define USB_STACK_SIZE (configMINIMAL_STACK_SIZE << 2)

// USB Device
usbd_device udev;
uint32_t usb0_buf[CDC_EP0_SIZE]; // EP0 Buffer
// USB Payload Interface Buffers
static volatile struct udev_pkt_ctrl pyld_ctrl;
static volatile struct udev_pkt_info  pyld_info;
// USB VCOM Interfaces
static volatile char vcom_txBuf[VCOM_DATA_SZ];
static volatile char vcom_rxBuf[VCOM_DATA_SZ];

// USBD Configuration Callback
static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg);

// USB Task Handle
static TaskHandle_t usbHndl;
static StackType_t puUsbStack[USB_STACK_SIZE];
static StaticTask_t pxUsbTsk;

// USB Task
void vTskUSB(void *pvParams);

// Initialize all system Interfaces
void Init(void){
    hal_clock_init();
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
    // can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);
    // can_init(&CANBus2, CAN_1000KBPS, PIN_CAN2_RX, PIN_CAN2_TX);

    // Initialize the PWM Timer for the servos
    // DO NOT CHANGE THESE VALUES - They work for us control
    servoCtrl_init((PLL_N/PLL_P)-1, 9999);

    // Set Servos to default Values - in us
    servoCtrl_set(eServo1, 1500);
    servoCtrl_set(eServo2, 1500);
    servoCtrl_set(eServo3, 1500);
    servoCtrl_set(eServo4, 1500);
    servoCtrl_set(eServo5, 1500);
    servoCtrl_set(eServo6, 1500);
    servoCtrl_set(eServo7, 1500);
    servoCtrl_set(eServo8, 1500);

    /**
     * Initialize System Tasks...
     * All tasks should be initialized as static
     * Tasks can be initialized dynamically, but may crash the system if they
     * overflow the system memory (128Kb for the STM32f446)
     */
    usbHndl = xTaskCreateStatic(
            vTskUSB, "USB", USB_STACK_SIZE, NULL,
            /*Priority*/1, puUsbStack, &pxUsbTsk);

}

void vTskUSB(void *pvParams){
    (void)(pvParams);
    // char msg[] = "USB Task Online";
    struct systime time;
    gpio_set_mode(PIN_LED1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    gpio_write(PIN_LED1, true);
    gpio_write(PIN_LED2, false);
    for(;;){
        systime_fromTicks(xTaskGetTickCount(), &time);
        // int stlen = strlen(time.str);
        memcpy((void*)vcom_txBuf, time.str, SYSTIME_STR_LEN);
        gpio_toggle_pin(PIN_LED1);
        gpio_toggle_pin(PIN_LED2);
        vTaskDelay(1000);
    }
}


// Triggered when the USB Host provides data to the PYLD interface
static void pyld_rx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    usbd_ep_read(dev, ep, (void*)&pyld_ctrl, sizeof(struct udev_pkt_ctrl));
    enum ePktType type = (enum ePktType)pyld_ctrl.hdr.ePktType;
    switch(type){
        case ePktStepSetup:
            break;
        case ePktStepCtrl:
            break;
        case ePktStepInfo:
            break;
        case ePktLightCtrl:
            break;
        case ePktServoCtrl:
            servoCtrl_set(pyld_ctrl.servoCtrl.ePWMChannel, pyld_ctrl.servoCtrl.value);
            break;
        default:
            break;
    }
}

// Triggered when the USB Host requests data from the PYLD interface
static void pyld_tx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;

    // Write back to the USB Memory
    usbd_ep_write(dev, ep, (void*)&pyld_info, sizeof(struct udev_pkt_info));
}

// USBD RX/TX Callbacks: Payload Interfaces
// Triggered during Control Interface events
static void pyld_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        pyld_rx(dev, evt, ep);
    else
        pyld_tx(dev, evt, ep);
}

// Triggered when the USB Host provides data to the VCOM interface
static void vcom_rx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    usbd_ep_read(dev, ep, (void*)&vcom_rxBuf, VCOM_DATA_SZ);
}

// Triggered when the USB Host requests data from the VCOM interface
static void vcom_tx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    // Write back to the USB Memory
    usbd_ep_write(dev, ep, (void*)&vcom_txBuf, VCOM_DATA_SZ);
}

// USBD RX/TX Callbacks: Virtial COM Port
// Triggered during Control Interface events
static void vcom_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        vcom_rx(dev, evt, ep);
    else
        vcom_tx(dev, evt, ep);
}


static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        usbd_ep_deconfig(dev, PYLD_NTF_EP);
        usbd_ep_deconfig(dev, PYLD_TXD_EP);
        usbd_ep_deconfig(dev, PYLD_RXD_EP);
        usbd_ep_deconfig(dev, VCOM_NTF_EP);
        usbd_ep_deconfig(dev, VCOM_TXD_EP);
        usbd_ep_deconfig(dev, VCOM_RXD_EP);

        usbd_reg_endpoint(dev, PYLD_RXD_EP, 0);
        usbd_reg_endpoint(dev, PYLD_TXD_EP, 0);
        usbd_reg_endpoint(dev, VCOM_RXD_EP, 0);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, PYLD_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, PYLD_DATA_SZ);
        usbd_ep_config(dev, PYLD_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, PYLD_DATA_SZ);
        usbd_ep_config(dev, PYLD_NTF_EP, USB_EPTYPE_INTERRUPT, PYLD_NTF_SZ);
        usbd_ep_config(dev, VCOM_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_NTF_EP, USB_EPTYPE_INTERRUPT, VCOM_NTF_SZ);

        usbd_reg_endpoint(dev, PYLD_RXD_EP, pyld_rxtx);
        usbd_reg_endpoint(dev, PYLD_TXD_EP, pyld_rxtx);
        usbd_reg_endpoint(dev, VCOM_RXD_EP, vcom_rxtx);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, vcom_rxtx);

        usbd_ep_write(dev, PYLD_TXD_EP, 0, 0);
        usbd_ep_write(dev, VCOM_TXD_EP, 0, 0);
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

