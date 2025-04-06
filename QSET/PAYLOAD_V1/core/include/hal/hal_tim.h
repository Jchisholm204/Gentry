/**
 * @file hal_tim.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief HAL Definitions for STM32 Timers 
 * @version 0.1
 * @date 2025-04-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _HAL_TIM_H_
#define _HAL_TIM_H_
#include <stm32f4xx.h>
#include "hal_gpio.h"

// ARR = Frequency 
// CCRx = Duty Cycle 
// PWM can be ran on a per channel basis (one PWM per OCx)
//  with PWM mode 1 (OCxM = 110 or OCxM = 111 for PWM mode 2)
//  The preload register must be enabled with the OCxPE bit in the CCMRx Register
//  and the autoreload bit (ARPE) must be set in the CR1 register
// All timers must be initialized by setting the UG bit in the EGR
//

enum eTimCh {
    eTimCh1,
    eTimCh2,
    eTimCh3,
    eTimCh4
};

#endif
