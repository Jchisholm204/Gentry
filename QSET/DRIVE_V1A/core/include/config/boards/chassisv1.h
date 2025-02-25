/**
 * @file drivev1.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Pin Mappings for the QSET Chassis Control Board
 * @version 0.1
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PINS_CHASSISV1_H_
#define _PINS_CHASSISV1_H_
#include "hal/pin.h"
#define _BOARD_CONFIG_


// LED Pins
#define PIN_LED1      PIN('C', 6)
#define PIN_LED2      PIN('C', 7)
#define PIN_LED3      PIN('C', 8)

#define PIN_LEDR      PIN('C', 13)
#define PIN_LEDG      PIN('C', 14)
#define PIN_LEDB      PIN('C', 15)

// USART/UART Pins
#define PIN_USART3_TX PIN('C', 10)
#define PIN_USART3_RX PIN('C', 11)

#define PIN_UART5_TX  PIN('C', 12)
#define PIN_UART5_RX  PIN('D', 2)

// CAN Pins
#define PIN_CAN1_RX   PIN('B', 8)
#define PIN_CAN1_TX   PIN('B', 9)

#define PIN_CAN2_RX   PIN('B', 5)
#define PIN_CAN2_TX   PIN('B', 6)

// I2C Pins
#define PIN_I2C3_SDA  PIN('C', 9)
#define PIN_I2C3_SCL  PIN('A', 8)

// SPI Pins
#define PIN_SPI2_CS2  PIN('B', 10)
#define PIN_SPI2_CS1  PIN('B', 12)
#define PIN_SPI2_SCK  PIN('B', 13)
#define PIN_SPI2_MISO PIN('B', 14)
#define PIN_SPI2_MOSI PIN('B', 15)

// USB Pins
#define PIN_USB_VBUS  PIN('A', 9)
#ifndef USBD_VBUS_DETECT
#define USBD_VBUS_DETECT
#endif
#define PIN_USB_DM    PIN('A', 11)
#define PIN_USB_DP    PIN('A', 12)

// ADC Inputs
#define PIN_ADC_1     PIN('C', 0)
#define PIN_ADC_2     PIN('C', 1)
#define PIN_ADC_3     PIN('C', 2)
#define PIN_ADC_4     PIN('C', 3)

// Servos
#define PIN_SRVO_EN   PIN('C', 5)
#define PIN_SERVO_1   PIN('B', 2)
#define PIN_SERVO_2   PIN('B', 1)
#define PIN_SERVO_3   PIN('B', 0)
#define PIN_SERVO_4   PIN('A', 7)
#define PIN_SERVO_5   PIN('A', 6)
#define PIN_SERVO_6   PIN('A', 5)
#define PIN_SERVO_7   PIN('A', 2)
#define PIN_SERVO_8   PIN('A', 1)

#endif


