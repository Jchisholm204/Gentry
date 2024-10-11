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

// Interfaces

// Serial (UART/USART)
#define configUSE_SERIAL1 1
#define configUSE_SERIAL2 1
#define configUSE_SERIAL3 1
#define configUSE_SERIAL4 1
#define configUSE_SERIAL5 1
#define configUSE_SERIAL6 1

// CAN Bus
#define configUSE_CAN1 1
#define configUSE_CAN2 1

// Analog to Digital Converters
#define configUSE_ADC1 1
#define configUSE_ADC2 1
#define configUSE_ADC3 1


#endif
