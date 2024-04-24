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

// Wait for flash to not be busy
static inline bool hal_flash_busy_wait(uint32_t timeout){
    // Wait for flash to not be busy
    for(uint32_t i = 0; (FLASH->SR & FLASH_SR_BSY) > 0; i++){
        if(i > timeout) return false;
        for(uint32_t j = 0; j < 99; j++) __asm__("nop");
    }
    return true;
}

static inline bool hal_flash_unlock(uint32_t timeout){
    // Wait for flash to not be busy
    if(hal_flash_busy_wait(timeout) == false) return false;
    // Flash Keys from datasheet (used to unlock the control register)
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    return READ_BIT(FLASH->CR, FLASH_CR_LOCK);
}

static inline bool hal_flash_lock(uint32_t timeout){
    // Wait for flash to not be busy
    if(hal_flash_busy_wait(timeout) == false) return false;
    SET_BIT(FLASH->CR, FLASH_CR_LOCK);
    return true;
}

static inline bool hal_flash_sector_erase(uint8_t sector, uint32_t timeout){
    // Wait for flash to not be busy
    if(hal_flash_busy_wait(timeout) == false) return false;
    SET_BIT(FLASH->CR, FLASH_CR_SER);
    CLEAR_BIT(FLASH->CR, FLASH_CR_SNB_Msk);
    SET_BIT(FLASH->CR, (uint32_t)(sector << FLASH_CR_SNB_Pos));
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
    if(hal_flash_busy_wait(timeout) == false) return false;
    return true;
}

static inline bool hal_flash_write(uint32_t address, uint32_t data){
    if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) == true) return false;
    FLASH->CR &= ~(FLASH_CR_PSIZE);
    FLASH->CR |= FLASH_CR_PSIZE_1;
    SET_BIT(FLASH->CR, FLASH_CR_PG);
    *((volatile uint32_t*)address) = data;
    return true;
}
