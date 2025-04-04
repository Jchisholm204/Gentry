/**
 * @file main.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <stm32f4xx.h>
#include "os/config/nvic.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SystemInit(void);

extern int main(void);

#ifdef __cplusplus
}
#endif
#endif
