/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */
#include "hal_gpio.h"
#include "stm32f446xx.h"
#include "pins.h"
#include "uart.h"

void SystemInit(void){
    return;
}

#define BOOTLOADER_SIZE (0x8000)
#define MAIN_APP_START_ADDR  (FLASH_BASE + BOOTLOADER_SIZE)

typedef void (*pFunction)(void);

void jump_to_main(void){
    pFunction jmpFunction = (pFunction)(*(uint32_t*)(MAIN_APP_START_ADDR + 4UL));
    jmpFunction();
}

int main(void){

    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    
    // Initialize UART
    uart_init();

    // Bootloader Loop
    for(;;){
        uart_write((uint8_t*)"hello\n", 7);
        gpio_write(PIN('B', 0), 1);
        gpio_write(PIN('B', 1), 0);
        spin(99999);
        gpio_write(PIN('B', 0), 0);
        gpio_write(PIN('B', 1), 1);
        spin(99999);
    }

    // Deinit Everything
    uart_deinit();
    // Offset the Interrupt vector table
    SCB->VTOR = BOOTLOADER_SIZE;
    // Boot into the main program
    jump_to_main();

    // never return
    return 0;
}
