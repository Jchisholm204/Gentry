/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */

#include "hal.h"

#define FLASH_APP_ADDR 0x8004000
#define STATIC __attribute__((section(".staticPrograms")))

typedef void (*pFunction)(void);

uint32_t SystemCoreClock = FREQ;
void SystemInit(void){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

int main(void){
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    gpio_write(PIN('B', 1), false);
    for(int i = 0; i < 999999; i++) __asm__("nop");
    gpio_write(PIN('B', 0), false);
    for(;;){
        gpio_write(PIN('B', 1), true);
        gpio_write(PIN('B', 0), false);
        spin(999999999);
        gpio_write(PIN('B', 1), false);
        gpio_write(PIN('B', 0), true);
        spin(99999999);
    }
    return 0;
}
