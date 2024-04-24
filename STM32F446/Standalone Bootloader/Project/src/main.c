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
    return;
}

#define BOOTLOADER_SIZE (0x4000)
#define MAIN_APP_START_ADDR  (FLASH_BASE + BOOTLOADER_SIZE)

static char msg[] = "BOOTLOADER";

void jump_to_main(void){
    uint32_t *reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDR + 4UL);
    uint32_t *reset_vector = (uint32_t*)(*reset_vector_entry);
    void (*jmpFunction)(void) = (void (*)(void))reset_vector;
    jmpFunction();
}

int main(void){
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    uart_init(USART2, 9600);
    uart_write_buf(USART2, msg, sizeof(msg));
    for(uint32_t i = 0; i < 20; i++){
        gpio_write(PIN('B', 0), 1);
        gpio_write(PIN('B', 1), 0);
        spin(99999);
        gpio_write(PIN('B', 0), 0);
        gpio_write(PIN('B', 1), 1);
        spin(99999);
    }
    SCB->VTOR = BOOTLOADER_SIZE;
    jump_to_main();

    // never return
    return 0;
}
