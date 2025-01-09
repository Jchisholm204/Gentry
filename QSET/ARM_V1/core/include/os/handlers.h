/**
 * @file handlers.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Hard Fault Handlers
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _FAULT_HANDLERS_H_
#define _FAULT_HANDLERS_H_

#include <stm32f4xx.h>

extern void HardFault_Handler(void);

extern void MemManage_Handler(void);

#endif