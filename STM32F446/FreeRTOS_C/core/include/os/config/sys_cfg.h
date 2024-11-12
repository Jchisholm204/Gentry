/**
 * @file sys_cfg.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief System Configuration File
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 * Set interface to 1 to enable
 * 
 */

#ifndef _SYS_CFG_H
#define _SYS_CFG_H

// STM32 Chip Family
#ifndef STM32F4xx
#define STM32F4xx
#endif
#ifndef STM32F446xx
#define STM32F446xx
#endif

// Interfaces

// Serial (UART/USART)
#define configUSE_SERIAL1 0
#define configUSE_SERIAL2 0
#define configUSE_SERIAL3 1
#define configUSE_SERIAL4 0
#define configUSE_SERIAL5 0
#define configUSE_SERIAL6 0

// CAN Bus
#define configUSE_CAN1 0
#define configUSE_CAN2 0

// Analog to Digital Converters
#define configUSE_ADC1 0
#define configUSE_ADC2 0
#define configUSE_ADC3 0


#endif
