/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */


#include <stdio.h>
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"
#include "libopencm3/stm32/f4/nvic.h"
#include "libopencm3/cm3/systick.h"
// #include "libopencm3/stm32/"

#define CLOCK rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ].ahb_frequency

extern void blocking_handler(void);
__attribute__((weak)) void blocking_handler(void) {
    while (1);
}


// Initialize all system Interfaces
void SystemInit(void){
    // Setup Clock
    // APB1 -> 45Mhz 
    // APB2 -> 90Mhz 
    // AHB  -> 180Mhz 
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
    // systick_set_reload(1000/CLOCK); // 1 ms tick at 16 MHz
    // systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // systick_interrupt_enable();
    // systick_counter_enable();
    

}


int main(void){
    

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        // gpio_write(PIN_LED2, true);
    }
    return 0;
}

