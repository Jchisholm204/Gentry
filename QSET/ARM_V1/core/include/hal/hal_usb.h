/**
 * @file hal_usb.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief USB HAL
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _HAL_USB_H_
#define _HAL_USB_H_
#include "hal_gpio.h"

// USB Initialize RCC
static inline void hal_usb_init_rcc(void){
    /* enabling GPIOA and setting PA11 and PA12 to AF10 (USB_FS) */
    #if defined(USBD_PRIMARY_OTGHS)
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    _BST(GPIOB->AFR[1], (0x0C << 24) | (0x0C << 28));
    _BMD(GPIOB->MODER, (0x03 << 28) | (0x03 << 30), (0x02 << 28) | (0x02 << 30));
    #else
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    SET_BIT(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    CLEAR_BIT(GPIOA->MODER, (uint32_t)((0x03 << 22) | (0x03 << 24)));
    SET_BIT(GPIOA->MODER, (uint32_t)(0x02 << 22) | (uint32_t)(0x02 << 24));
    #endif
}



#endif

