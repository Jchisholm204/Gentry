/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */
#include "hal.h"
#include "stm32f446xx.h"

void SystemInit(void){
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(FREQ/1000);
    return;
}

static volatile uint32_t counter = 0;
void SysTick_Handler(void){
    counter++;
}

int main(void){
    // SCB->VTOR = 0x4000U;
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    volatile uint32_t timer = 0, period = 500;
    bool led_on = false;
    for(;;) {
        if(timer_expired(&timer, period, counter)){
            gpio_write(PIN('B', 0), led_on);
            led_on = !led_on;
            gpio_write(PIN('B', 1), led_on);
        }
    }
    return 0;
}
