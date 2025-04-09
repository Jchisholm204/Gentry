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
#include "light_ctrl.h"
#include "motor_ctrl.h"

#include "adc_tsk.h"

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

    // Initialize ADC
    adc_init(&pyld_info.adc_info);

    // Initialize Motor Control
    motorCtrl_init();

    motorCtrl_set(eMotor1, 0);
    motorCtrl_set(eMotor2, 0);

    // Initialize the PWM Timer for the servos
    // DO NOT CHANGE THESE VALUES - They work for us control
    servoCtrl_init((PLL_N/PLL_P)-1, SERVO_MAX_PWM);

    // Set Servos to default Values - in us
    servoCtrl_set(eServo1, 1500);
    servoCtrl_set(eServo2, 1500);
    servoCtrl_set(eServo3, 1500);
    servoCtrl_set(eServo4, 1500);
    servoCtrl_set(eServo5, 1500);
    servoCtrl_set(eServo6, 1500);
    servoCtrl_set(eServo7, 1500);
    servoCtrl_set(eServo8, 1500);

    lightCtrl_init();
    lightCtrl_setState(eLightAllOFF);

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

// I2C Reference Material:
// https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53

void hal_i2c_init(I2C_TypeDef *I2C){
    if(I2C == I2C1) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
    if(I2C == I2C2) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN);
    if(I2C == I2C3) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C3EN);
    gpio_set_mode(PIN_I2C1_SCL, GPIO_MODE_AF);
    gpio_set_af(PIN_I2C1_SCL, GPIO_AF_I2C);
    gpio_set_mode(PIN_I2C1_SDA, GPIO_MODE_AF);
    gpio_set_af(PIN_I2C1_SDA, GPIO_AF_I2C);

    // Reset the I2C interface
    SET_BIT(I2C->CR1, I2C_CR1_SWRST);
    CLEAR_BIT(I2C->CR1, I2C_CR1_SWRST);
    // Setup I2C Clock Divider
    // APB clock should be 42 MHz?
    SET_BIT(I2C->CR2, 42);
    // SCL line frequency (set to 100kHz)
    SET_BIT(I2C->CCR, 210);
    SET_BIT(I2C->TRISE, 43);
    // Enable I2C
    SET_BIT(I2C->CR1, I2C_CR1_PE);
}

void hal_i2c_write(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr, uint8_t value) {
    // Generate START condition
    SET_BIT(I2C->CR1, I2C_CR1_START);
    while (!(I2C->SR1 & I2C_SR1_SB));  // Wait for START condition generated

    // Send device address (write)
    I2C->DR = dev_addr & ~0x01;  // Ensure LSB = 0 for write
    while (!(I2C->SR1 & I2C_SR1_ADDR));  // Wait for address to be sent
    (void)I2C->SR2;  // Clear ADDR by reading SR1 and SR2

    // Send register address
    while (!(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    while (!(I2C->SR1 & I2C_SR1_TXE));

    // Send data
    I2C->DR = value;
    while (!(I2C->SR1 & I2C_SR1_BTF));  // Wait for byte transfer finished

    // Generate STOP condition
    SET_BIT(I2C->CR1, I2C_CR1_STOP);
}

// int hal_i2c_read(I2C_TypeDef * I2C, uint8_t dev, uint8_t addr, uint8_t *data, uint8_t size){
//     SET_BIT(I2C->CR1, I2C_CR1_START | I2C_CR1_ACK);
//     while(!(I2C->SR1 & I2C_SR1_SB));
//     I2C->DR = dev;
//     while(!(I2C->SR1 & I2C_SR1_ADDR));
//     while(!(I2C->SR1 & I2C_SR1_TXE));
//     I2C->DR = addr;
//     while(!(I2C->SR1 & I2C_SR1_BTF));
//     SET_BIT(I2C->CR1, I2C_CR1_START);
//     while(!(I2C->SR1 & I2C_SR1_SB));
//     I2C->DR = dev;
//     while(!(I2C->SR1 & I2C_SR1_ADDR));
//     for(int i = 0; i < size; i++, data++){
//         if(i + 1 == size){
//             CLEAR_BIT(I2C->CR1, I2C_CR1_ACK);
//             SET_BIT(I2C->CR1, I2C_CR1_STOP);
//         }
//         while(!(I2C->SR1 & I2C_SR1_RXNE));
//         *data = (uint8_t)I2C->DR;
//     }
//     return 0;
// }

uint8_t hal_i2c_read(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr) {
    uint32_t reg;
    // Send START and device address (write)
    SET_BIT(I2C->CR1, I2C_CR1_START | I2C_CR1_ACK);
    while (!(I2C->SR1 & I2C_SR1_SB));
    I2C->DR = dev_addr & ~0x01;  // Write mode
    while (!(I2C->SR1 & I2C_SR1_ADDR));
    reg = I2C->SR1 | I2C->SR2;

    // Send register address
    while (!(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    while (!(I2C->SR1 & I2C_SR1_BTF));

    // Repeated START and device address (read)
    SET_BIT(I2C->CR1, I2C_CR1_START);
    while (!(I2C->SR1 & I2C_SR1_SB));
    I2C->DR = dev_addr | 0x01;  // Read mode
    while (!(I2C->SR1 & I2C_SR1_ADDR));
    reg = I2C->SR1 | I2C->SR2;
    CLEAR_BIT(I2C->CR1, I2C_CR1_ACK);  // Only reading 1 byte

    // Stop condition
    SET_BIT(I2C->CR1, I2C_CR1_STOP);

    // Wait for data
    while (!(I2C->SR1 & I2C_SR1_RXNE));
    return I2C->DR;
}

#define TCS34725_ADDRESS      0x39 << 1  // 7-bit address shifted left
#define TCS34725_COMMAND_BIT  0x80
#define TCS34725_ENABLE       0x00
#define TCS34725_ENABLE_PON   0x01
#define TCS34725_ENABLE_AEN   0x02
#define TCS34725_ATIME        0x01  // Integration time
#define TCS34725_CONTROL      0x0F  // Gain

void tcs34725_init(I2C_TypeDef *I2C) {
    // Power ON
    hal_i2c_write(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | TCS34725_ENABLE, TCS34725_ENABLE_PON);
    vTaskDelay(3); // Wait for power-on
    // Enable ADC
    hal_i2c_write(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
    // Set integration time (e.g. 0xD5 = 101 ms)
    hal_i2c_write(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | TCS34725_ATIME, 0xD5);
    // Set gain (1x gain)
    hal_i2c_write(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | TCS34725_CONTROL, 0x00);
}

uint16_t read_word(I2C_TypeDef *I2C, uint8_t reg) {
    uint8_t low = hal_i2c_read(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | reg);
    uint8_t high = hal_i2c_read(I2C, TCS34725_ADDRESS, TCS34725_COMMAND_BIT | (reg + 1));
    return ((uint16_t)high << 8) | low;
}

void tcs34725_read_rgb(I2C_TypeDef *I2C, uint16_t *r, uint16_t *g, uint16_t *b) {
    *r = read_word(I2C, 0x16);  // Red
    *g = read_word(I2C, 0x18);  // Green
    *b = read_word(I2C, 0x1A);  // Blue
}


void vTskUSB(void *pvParams){
    (void)(pvParams);
    // char msg[] = "USB Task Online";
    struct systime time;
    vTaskDelay(100);
    gpio_set_mode(PIN_LED1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    gpio_write(PIN_LED1, true);
    gpio_write(PIN_LED2, false);
    hal_i2c_init(I2C1);
    tcs34725_init(I2C1);
    for(;;){
        // Enable sensor: power on and enable RGBC
        // hal_i2c_write(I2C1, 0x52, 0x80 | 0x00, 0x03);  // 0x00 = ENABLE register, 0x03 = PON | AEN
        // systime_fromTicks(xTaskGetTickCount(), &time);
        // int stlen = strlen(time.str);
        // memcpy((void*)vcom_txBuf, time.str, SYSTIME_STR_LEN);
        gpio_toggle_pin(PIN_LED1);
        gpio_toggle_pin(PIN_LED2);
        // pyld_info.status.code = ePayloadOK;
        // size_t code_len = UDEV_ERROR_LEN;
        // if(SYSTIME_STR_LEN < UDEV_ERROR_LEN) code_len = SYSTIME_STR_LEN;
        // memcpy((void*)pyld_info.status.msg, time.str, code_len);
        uint16_t temp[3];
        tcs34725_read_rgb(I2C1, temp, temp + 1, temp + 2);
        // hal_i2c_read(I2C1, 0x52, 0x94 | 0x80, (void*)&temp, 2);
        sprintf((void*)vcom_txBuf, "%d, %d, %d\n", temp[0], temp[1], temp[2]);
        vTaskDelay(100);
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
        case ePktLightCtrl:
            lightCtrl_setState(pyld_ctrl.lightCtrl.eLightChannel);
            break;
        case ePktServoCtrl:
            servoCtrl_set(pyld_ctrl.servoCtrl.ePWMChannel, pyld_ctrl.servoCtrl.value);
            break;
        case ePktMotorCtrl:
            motorCtrl_set(pyld_ctrl.mtrCtrl.eMtrChannel, pyld_ctrl.mtrCtrl.value);
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

