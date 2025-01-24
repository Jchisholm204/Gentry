/**
 * @file limit_switches.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief QSET Arm Limit Switches
 * @date 2025-01-19
 * @version 2.2
 * 
 */
#ifndef _LIMIT_SWITCHES_H_
#define _LIMIT_SWITCHES_H_
#include "hal/hal_gpio.h"
#include "config/pin_cfg.h"

// Initialize the limit switches
static inline void lmtSW_init(void){
    gpio_set_mode(PIN_LSW_1, GPIO_MODE_INPUT);
    gpio_set_mode(PIN_LSW_2, GPIO_MODE_INPUT);
    gpio_set_mode(PIN_LSW_3, GPIO_MODE_INPUT);
    gpio_set_mode(PIN_LSW_4, GPIO_MODE_INPUT);
    gpio_set_mode(PIN_LSW_5, GPIO_MODE_INPUT);
    gpio_set_mode(PIN_LSW_6, GPIO_MODE_INPUT);
}

/**
 * @brief Get the limit switches state
 *
 * @returns one hot encoding / highest bit always hot
 */
static inline uint8_t lmtSW_get(void){
    uint8_t lsw = 0x00U;
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_1) << 0);
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_2) << 1);
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_3) << 2);
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_4) << 3);
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_5) << 4);
    lsw |= (uint8_t)((uint8_t)gpio_read_idr(PIN_LSW_6) << 5);
    lsw |= 0x80;
    return lsw;
}

#endif
