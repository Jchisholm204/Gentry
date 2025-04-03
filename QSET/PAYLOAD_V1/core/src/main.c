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
#include "usb_chassis_defs.h"
#include "drivers/stusb/usb.h"
#include "usb_desc.h"

#include "mtr_ctrl.h"
#include "test_tsks.h"
#include "srv_ctrl.h"
#include "light_ctrl.h"

#define USB_STACK_SIZE (configMINIMAL_STACK_SIZE << 2)

// USB Device
usbd_device udev;
uint32_t usb0_buf[CDC_EP0_SIZE]; // EP0 Buffer
// USB Drive Control Buffers
static volatile struct udev_pkt_drvm_ctrl drvm_ctrl;
static volatile struct udev_pkt_drvm_sts drvm_sts;
// USB Servo Control Buffers
static volatile struct udev_pkt_srvo_ctrl srvo_ctrl;
static volatile struct udev_pkt_srvo_sts srvo_sts;
// USB Sensor Control Buffers
static volatile struct udev_pkt_sens_ctrl sens_ctrl;
static volatile struct udev_pkt_sens_sts sens_sts;

// USBD Configuration Callback
static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg);

// Motor Controller Handles
mtrCtrlHndl_t mtrControllers[eN_DrvMotor];

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

    // Initialize the Auto Light Control
    lightCtrl_init();

    // Initialize the PWM Timer for the servos
    // DO NOT CHANGE THESE VALUES - They work for us control
    srvCtrl_init((PLL_N/PLL_P)-1, 9999);

    // Set Servos to default Values - in us
    srvCtrl_set(eServo1, 1500);
    srvCtrl_set(eServo2, 1500);
    srvCtrl_set(eServo3, 1500);
    srvCtrl_set(eServo4, 1500);
    srvCtrl_set(eServo5, 1500);
    srvCtrl_set(eServo6, 1500);
    srvCtrl_set(eServo7, 1500);
    srvCtrl_set(eServo8, 1500);

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
    //           Motor Control Handle,  Joint ID, AK Mtr Type, CAN ID
    mtrCtrl_init(&mtrControllers[eDrvMtrFL], eDrvMtrFL, eAK7010, 10);
    mtrCtrl_init(&mtrControllers[eDrvMtrFR], eDrvMtrFR, eAK7010, 11);
    mtrCtrl_init(&mtrControllers[eDrvMtrBL], eDrvMtrBL, eAK7010, 12);
    mtrCtrl_init(&mtrControllers[eDrvMtrBR], eDrvMtrBR, eAK7010, 13);
}

void vTskUSB(void *pvParams){
    (void)(pvParams);
    char msg[] = "USB Task Online";
    struct systime time;
    gpio_set_mode(PIN_LED1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    gpio_write(PIN_LED1, true);
    for(;;){
        systime_fromTicks(xTaskGetTickCount(), &time);
        // int stlen = strlen(time.str);
        memcpy(srvo_sts.buf, time.str, SYSTIME_STR_LEN);
        srvo_sts.len = SYSTIME_STR_LEN;
        srvo_sts.core_temp = 0.0;
        gpio_toggle_pin(PIN_LED1);
        gpio_toggle_pin(PIN_LED2);
        vTaskDelay(1000);
    }
}


// Triggered when the USB Host provides data to the DRVM interface
static void drvm_rx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    usbd_ep_read(dev, ep, (void*)&drvm_ctrl, sizeof(struct udev_pkt_drvm_ctrl));
    enum eDrvMotors mtr_id = (enum eDrvMotors)drvm_ctrl.mtr_id;
    if(mtr_id >= eN_DrvMotor) return;
    mtrCtrl_update(&mtrControllers[mtr_id], (struct udev_mtr_ctrl*)&drvm_ctrl.mtr_ctrl);
    lightCtrl_pkt(drvm_ctrl.light_ctrl);
}

// Triggered when the USB Host requests data from the DRVM interface
static void drvm_tx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    // Get the latest data from the motor
    for(enum eDrvMotors m = 0; m < eN_DrvMotor; m++){
        mtrCtrl_getInfo(&mtrControllers[m], (struct udev_mtr_info*)&drvm_sts.mtr_info[m]);
    }

    // Write back to the USB Memory
    usbd_ep_write(dev, ep, (void*)&drvm_sts, sizeof(struct udev_pkt_drvm_sts));
}

// USBD RX/TX Callbacks: Drive Control
// Triggered during Control Interface events
static void drvm_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        drvm_rx(dev, evt, ep);
    else
        drvm_tx(dev, evt, ep);
}

// Triggered when the USB Host provides data to the SRVO interface
static void srvo_rx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    usbd_ep_read(dev, ep, (void*)&srvo_ctrl, sizeof(struct udev_pkt_srvo_ctrl));
    enum eChassisServo srvo_id = (enum eChassisServo)srvo_ctrl.srvo_id;
    if(srvo_id >= eN_Servo) return;
    srvCtrl_set(srvo_id, srvo_ctrl.srvo_ctrl);
}

// Triggered when the USB Host requests data from the SRVO interface
static void srvo_tx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    // Servo Status Updates are handled in main usb task
    // Write back to the USB Memory
    usbd_ep_write(dev, ep, (void*)&srvo_sts, sizeof(struct udev_pkt_srvo_sts));
}

// USBD RX/TX Callbacks: Servo Control
// Triggered during Control Interface events
static void srvo_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        srvo_rx(dev, evt, ep);
    else
        srvo_tx(dev, evt, ep);
}

// Triggered when the USB Host provides data to the SRVO interface
static void sens_rx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    // Sensors have no controllable values
    usbd_ep_read(dev, ep, (void*)&sens_ctrl, sizeof(struct udev_pkt_sens_ctrl));
}

// Triggered when the USB Host requests data from the SRVO interface
static void sens_tx(usbd_device *dev, uint8_t evt, uint8_t ep){
    (void)evt;
    // Servo Status Updates are handled in main usb task
    // Write back to the USB Memory
    usbd_ep_write(dev, ep, (void*)&srvo_sts, sizeof(struct udev_pkt_srvo_sts));
}

// USBD RX/TX Callbacks: Servo Control
// Triggered during Control Interface events
static void sens_rxtx(usbd_device *dev, uint8_t evt, uint8_t ep){
    if(evt == usbd_evt_eprx)
        sens_rx(dev, evt, ep);
    else
        sens_tx(dev, evt, ep);
}


static usbd_respond udev_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        usbd_ep_deconfig(dev, DRVM_NTF_EP);
        usbd_ep_deconfig(dev, DRVM_TXD_EP);
        usbd_ep_deconfig(dev, DRVM_RXD_EP);
        usbd_ep_deconfig(dev, SRVO_NTF_EP);
        usbd_ep_deconfig(dev, SRVO_TXD_EP);
        usbd_ep_deconfig(dev, SRVO_RXD_EP);
        usbd_ep_deconfig(dev, SENS_NTF_EP);
        usbd_ep_deconfig(dev, SENS_TXD_EP);
        usbd_ep_deconfig(dev, SENS_RXD_EP);

        usbd_reg_endpoint(dev, DRVM_RXD_EP, 0);
        usbd_reg_endpoint(dev, DRVM_TXD_EP, 0);
        usbd_reg_endpoint(dev, SRVO_RXD_EP, 0);
        usbd_reg_endpoint(dev, SRVO_TXD_EP, 0);
        usbd_reg_endpoint(dev, SENS_RXD_EP, 0);
        usbd_reg_endpoint(dev, SENS_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, DRVM_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, DRVM_DATA_SZ);
        usbd_ep_config(dev, DRVM_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, DRVM_DATA_SZ);
        usbd_ep_config(dev, DRVM_NTF_EP, USB_EPTYPE_INTERRUPT, DRVM_NTF_SZ);
        usbd_ep_config(dev, SRVO_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, SRVO_DATA_SZ);
        usbd_ep_config(dev, SRVO_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, SRVO_DATA_SZ);
        usbd_ep_config(dev, SRVO_NTF_EP, USB_EPTYPE_INTERRUPT, SRVO_NTF_SZ);
        usbd_ep_config(dev, SENS_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, SENS_DATA_SZ);
        usbd_ep_config(dev, SENS_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, SENS_DATA_SZ);
        usbd_ep_config(dev, SENS_NTF_EP, USB_EPTYPE_INTERRUPT, SENS_NTF_SZ);

        // TODO: Add back these functions
        usbd_reg_endpoint(dev, DRVM_RXD_EP, drvm_rxtx);
        usbd_reg_endpoint(dev, DRVM_TXD_EP, drvm_rxtx);
        usbd_reg_endpoint(dev, SRVO_RXD_EP, srvo_rxtx);
        usbd_reg_endpoint(dev, SRVO_TXD_EP, srvo_rxtx);
        // usbd_reg_endpoint(dev, SENS_RXD_EP, sens_rxtx);
        // usbd_reg_endpoint(dev, SENS_TXD_EP, sens_rxtx);

        // usbd_reg_endpoint(dev, DRVM_RXD_EP, srvo_rxtx);
        // usbd_reg_endpoint(dev, DRVM_TXD_EP, srvo_rxtx);
        usbd_reg_endpoint(dev, SENS_RXD_EP, srvo_rxtx);
        usbd_reg_endpoint(dev, SENS_TXD_EP, srvo_rxtx);

        usbd_ep_write(dev, DRVM_TXD_EP, 0, 0);
        usbd_ep_write(dev, SRVO_TXD_EP, 0, 0);
        usbd_ep_write(dev, SENS_TXD_EP, 0, 0);
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

