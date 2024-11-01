#ifndef _HAL_CLOCK_H_
#define _HAL_CLOCK_H_

#include <stm32f4xx.h>
#define HSE_VALUE ((uint32_t)(8000000))

/**
 * @file clock.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Set up system clock for STM32F446RE (180Mhz)
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023
 * 
 * Note about clocks:
 *  The CMSIS "SystemCoreClock" does not work. This is because the STM32 eval boards contain a 25Mhz external
 *  crystal oscillator. Our FSAE boards use an 8Mhz crystal. Therefore make sure to use the correct clock
 *  in your design.
 * 
 *      SYS_FREQUENCY  = 180Mhz
 *      APB2_FREQUENCY = 45Mhz
 *      APB1_FREQUENCY = 36Mhz
 * 
 */
#define BIT(x) (1UL << (x))


// 6.3.3: APB1 clock <= 45MHz; APB2 clock <= 90MHz 
// 3.5.1, Table 11: configure flash latency (WS) in accordance to clock freq 
// 33.4: The AHB clock must be at least 25 MHz when Ethernet is used 

// enum { APB1_PRE = 5 /* AHB clock / 4(MAX) */, APB2_PRE = 4 /* AHB clock / 2(MAX) */ }; 
// enum { PLL_HSE = 8, PLL_M = 4, PLL_N = 180, PLL_P = 2 };  // Run at 180 Mhz

enum { APB1_PRE = 4 /* AHB clock / 4(MAX) */, APB2_PRE = 2 /* AHB clock / 2(MAX) */ }; 
enum { PLL_HSE = 8, PLL_M = 4, PLL_N = 168, PLL_P = 2, PLL_Q = 7, PLL_R = 2 };  // Run at 180 Mhz

// #define FLASH_LATENCY 5 
// #define PLL_M 4
// #define PLL_N 168
// #define PLL_P 2
// #define PLL_Q 7
// #define PLL_R 2

// System Frequency
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
// Interface Frequencies
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3))) 
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3))) 

// Initialize the system clock
// static inline void clock_init(void){
//     FLASH->ACR |= FLASH_ACR_LATENCY | BIT(8) | BIT(9); // flash latency / prefetch
//     
//     RCC->CR &= ~(BIT(0) | BIT(16) | BIT(18));                               // clear hsi on, hse on, hse byp / not needed
//     RCC->CR |= BIT(16);                                                     // set HSE ON
//     while(!(RCC->CR & RCC_CR_HSERDY)) __asm__("nop");                // wait for HSE ready
//     RCC->PLLCFGR &= ~(BIT(17)-1);                                           // clear PLL multipliers
//     RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);
//     //RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;                          // Set PLL_P
//     RCC->PLLCFGR |= PLL_M | (PLL_N << 6);                                     // Set PLL_M and PLL_N
//     RCC->CR |= BIT(24);                                                 // Enable PLL
//     RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);                       // Set prescalers 
//     while (!(RCC->CR & RCC_CR_PLLRDY)) __asm__("nop");                              // Wait until done
//     RCC->CFGR |= 2;                                                       // Set clock source to PLL 
//     while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) __asm__("nop"); // Wait until done
// }

static inline void hal_clock_flash_init(void){
    // Enable Flash latency, INS prefetch and caching
    SET_BIT(FLASH->ACR, (FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN));
}


static inline void hal_clock_osc_init(void){
    // Disable the HSI clock and the HSE Bypass (Crystal occilator used)
    CLEAR_BIT(RCC->CR, RCC_CR_HSION | RCC_CR_HSEBYP);
    // Enable the HSE Clock
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    // Wait for the external clock to be ready
    while(!(READ_BIT(RCC->CR, RCC_CR_HSERDY))) __asm__("nop");
    // Configure the PLL
    WRITE_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE |
            PLL_M |
            PLL_N << RCC_PLLCFGR_PLLN_Pos |
            ((PLL_P >> 1U) - 1U) << RCC_PLLCFGR_PLLP_Pos |
            PLL_Q << RCC_PLLCFGR_PLLQ_Pos
            );
    // Enable the PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY)) __asm__("nop");
}

static inline void hal_clock_init(void){
    hal_clock_flash_init();
    hal_clock_osc_init();
    // Clear the APBx Dividers
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_HPRE_DIV16);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_CFGR_HPRE_DIV16 << 3U));
    // Setup the HCLK
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    // Set the system clock to the PLL
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    // Setup PLL 1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
    // Setup PLL 2
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_CFGR_PPRE2_DIV2 << 3U));
}

static inline void spin(volatile uint32_t count) {
  while (count--) __asm__("nop");
}

#endif

