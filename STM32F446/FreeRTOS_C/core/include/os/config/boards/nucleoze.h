/**
 * @file nucleoze.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Pin Mappings for STM Nucleo F446ZE
 * @version 0.1
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PINS_NUCLEOZE_H_
#define _PINS_NUCLEOZE_H_
#include "os/hal/hal_gpio.h"

#define cfgSerial Serial3

#define PIN_LED1 PIN('B', 0)
#define PIN_LED2 PIN('B', 7)
#define PIN_LED3 PIN('B', 14)

// #define PIN_USART2_RX PIN('A', 0)
// #define PIN_USART2_TX PIN('A', 0)

#define PIN_USART3_RX PIN('D', 9)
#define PIN_USART3_TX PIN('D', 8)

// #define PIN_CAN1_RX PIN('A', 0)
// #define PIN_CAN1_TX PIN('A', 0)

#define PIN_USB_SOF      PIN('A', 8)
#define PIN_USB_VBUS     PIN('A', 9)
#define PIN_USB_ID       PIN('A', 10)
#define PIN_USB_DM       PIN('A', 11)
#define PIN_USB_DP       PIN('A', 12)
#define PIN_USB_GPIO_OUT PIN('G', 6)
#define PIN_USB_GPIO_IN  PIN('G', 7)

#endif

