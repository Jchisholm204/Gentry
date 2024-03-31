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

void jump_to_main(void){
    uint32_t *reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDR + 4UL);
    uint32_t *reset_vector = (uint32_t*)(*reset_vector_entry);
    void (*jmpFunction)(void) = (void (*)(void))reset_vector;
    jmpFunction();
}

int main(void){
    jump_to_main();

    // never return
    return 0;
}
