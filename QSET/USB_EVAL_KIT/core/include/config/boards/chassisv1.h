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
#define PIN_LED1      PIN('C', 8)
#define PIN_LED2      PIN('C', 9)
#define PIN_LED3      PIN('A', 8)

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

// USB Pins
#define PIN_USB_VBUS  PIN('A', 9)
#ifndef USBD_VBUS_DETECT
#define USBD_VBUS_DETECT
#endif
#define PIN_USB_DM    PIN('A', 11)
#define PIN_USB_DP    PIN('A', 12)


// Servos
#define PIN_SRVO_EN   PIN('C', 6)
// TIM3 CH 1
#define PIN_SERVO_1   PIN('A', 6)
// TIM3 CH 2
#define PIN_SERVO_2   PIN('A', 7)
// TIM3 CH 3
#define PIN_SERVO_3   PIN('B', 0)
// TIM3 CH 4
#define PIN_SERVO_4   PIN('B', 1)
#define PIN_SERVO_5   PIN('B', 1)
#define PIN_SERVO_6   PIN('B', 1)
#define PIN_SERVO_7   PIN('B', 1)
#define PIN_SERVO_8   PIN('B', 1)

#endif


