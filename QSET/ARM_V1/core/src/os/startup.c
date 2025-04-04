/**
 * @file startup.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief System Startup
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "hal/hal_clock.h"

/**
 * @brief Initializes the CPU
 * 
 * Enable System Configuration Register
 * Enable Floating Point Unit
 * Configure System Tick for 1000hz (1ms)
 * 
 */
void SystemInit(void){
    // Enable system configuration register
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    SET_BIT(PWR->CR, PWR_CR_VOS_Msk);
    // enable FPU
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
    // Initialize Clock
    hal_clock_init();
    // configure system tick (ARM)
    SysTick_Config(SYS_FREQUENCY/1000); // DO NOT USE "SystemClock"
}

