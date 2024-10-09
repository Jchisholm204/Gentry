/**
 * @file mockecu.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief MockECU Pin Declarations
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PINS_MOCKECU_H_
#define _PINS_MOCKECU_H_
#include "os/hal/hal_gpio.h"

#define PIN_LED1 PIN('B', 0)
#define PIN_LED2 PIN('B', 1)

#define PIN_USART2_RX PIN('A', 3)
#define PIN_USART2_TX PIN('A', 2)

#define PIN_CAN1_RX PIN('A', 11)
#define PIN_CAN1_TX PIN('A', 12)

#endif