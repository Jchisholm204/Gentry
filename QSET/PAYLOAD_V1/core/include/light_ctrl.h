/**
 * @file light_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Auto Lights Control - Chassis Control
 * @date 2025-02-24
 * @version 1.0
 * 
 */

#ifndef _LIGHT_CTRL_H_
#define _LIGHT_CTRL_H_

#include "hal/hal_gpio.h"
#include "config/pin_cfg.h"
#include "usb_chassis_defs.h"

static void lightCtrl_init(void){
    gpio_set_mode(PIN_LEDR, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LEDG, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LEDB, GPIO_MODE_OUTPUT);
    gpio_write(PIN_LEDR, 1);
    gpio_write(PIN_LEDG, 0);
    gpio_write(PIN_LEDB, 1);
}


static void lightCtrl_pkt(uint8_t pkt){
    // Red Light Control
    bool R1 = (pkt & eDrvLight_R1);
    bool R0 = (pkt & eDrvLight_R0);
    if(R1)
        gpio_write(PIN_LEDR, 1);
    else if(R0)
        gpio_write(PIN_LEDR, 0);

    // Green Light Control
    bool G1 = (pkt & eDrvLight_G1);
    bool G0 = (pkt & eDrvLight_G0);
    if(G1)
        gpio_write(PIN_LEDG, 1);
    else if(G0)
        gpio_write(PIN_LEDG, 0);

    // Blue Light Control
    bool B1 = (pkt & eDrvLight_B1);
    bool B0 = (pkt & eDrvLight_B0);
    if(B1)
        gpio_write(PIN_LEDB, 1);
    else if(B0)
        gpio_write(PIN_LEDB, 0);
}


#endif
