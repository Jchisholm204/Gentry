/**
 * @file light_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Light Control - Payload Control
 * @date 2025-01-24
 * @version 1.0
 * 
 */
#ifndef _LIGHT_CTRL_H_
#define _LIGHT_CTRL_H_

#include "pin_cfg.h"
#include "usb_payload_defs.h"
#include "hal/hal_gpio.h"

static void lightCtrl_init(void){
    gpio_set_mode(PIN_LS0, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LS1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LS2, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LS3, GPIO_MODE_OUTPUT);
}

static void lightCtrl_setState(enum ePayloadLight elight){
    uint8_t value = (uint8_t)elight;
    gpio_write(PIN_LS0, value & 0x01U);
    gpio_write(PIN_LS1, value & 0x02U);
    gpio_write(PIN_LS2, value & 0x04U);
    gpio_write(PIN_LS3, value & 0x08U);
}

#endif

