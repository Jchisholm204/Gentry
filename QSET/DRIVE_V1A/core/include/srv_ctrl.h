/**
 * @file srv_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Servo Control - Arm Board
 * @date 2025-01-24
 * @version 1.0
 * 
 */
#ifndef _SRV_CTRL_H_
#define _SRV_CTRL_H_

#include "hal/hal_tim_pwm.h"
#include "config/pin_cfg.h"
#include "usb_arm_defs.h"

#define SERVO_TIM TIM3

static void srvCtrl_init(uint16_t pre, uint16_t arr){
    // Arm Board uses Timer 3
    hal_tim_pwm_init(SERVO_TIM, pre, arr);
    hal_tim_pwm_configure_channel(SERVO_TIM, eTimCh1);
    hal_tim_pwm_configure_channel(SERVO_TIM, eTimCh2);
    hal_tim_pwm_configure_channel(SERVO_TIM, eTimCh3);
    hal_tim_pwm_configure_channel(SERVO_TIM, eTimCh4);
    // Setup the GPIO pins
    gpio_set_mode(PIN_SERVO_1, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_1, GPIO_AF_TIM3_5);
    gpio_set_mode(PIN_SERVO_2, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_2, GPIO_AF_TIM3_5);
    gpio_set_mode(PIN_SERVO_3, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_3, GPIO_AF_TIM3_5);
    gpio_set_mode(PIN_SERVO_4, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_4, GPIO_AF_TIM3_5);
    gpio_set_mode(PIN_SRVO_EN, GPIO_MODE_OUTPUT);
    gpio_write(PIN_SRVO_EN, true);
}   


static void srvCtrl_set(enum eArmServos srv, uint32_t val_us){
    switch(srv){
        case eServo1:
            hal_tim_pwm_set(SERVO_TIM, eTimCh1, val_us);
            break;
        case eServo2:
            hal_tim_pwm_set(SERVO_TIM, eTimCh2, val_us);
            break;
        case eServo3:
            hal_tim_pwm_set(SERVO_TIM, eTimCh3, val_us);
            break;
        case eServo4:
            hal_tim_pwm_set(SERVO_TIM, eTimCh4, val_us);
            break;
        default:
            break;
    };
}

#endif
