/**
 * @file hal_flash.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 1.2
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "stm32f4xx.h"
#include <stdbool.h>

static inline bool hal_flash_unlock(uint32_t timeout){
    // Wait for flash to not be busy
    for(uint32_t i = 0; (FLASH->SR & FLASH_SR_BSY) > 0; i++){
        if(i > timeout) return false;
        for(uint32_t j = 0; j < 99; j++) __asm__("nop");
    }
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    return true;
}

static inline bool hal_flash_lock(uint32_t timeout){
    // Wait for flash to not be busy
    for(uint32_t i = 0; (FLASH->SR & FLASH_SR_BSY) > 0; i++){
        if(i > timeout) return false;
        for(uint32_t j = 0; j < 99; j++) __asm__("nop");
    }
    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
    return true;
}
