/**
 * @file srv_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Servo Control - Payload Control
 * @date 2025-01-24
 * @version 1.0
 * 
 */
#ifndef _SRV_CTRL_H_
#define _SRV_CTRL_H_

#include "hal/hal_tim_pwm.h"
#include "config/pin_cfg.h"
#include "usb_payload_defs.h"

#define SERVO_MAX_PWM 10000

static void servoCtrl_init(uint16_t pre, uint16_t arr){
    arr -= 1;
    // Uses several Timer Interfaces
    hal_tim_pwm_init(TIM2, pre, arr);
    hal_tim_pwm_init(TIM3, pre, arr);
    hal_tim_pwm_init(TIM4, pre, arr);

    // Initialize Timer 2 Channels
    hal_tim_pwm_configure_channel(TIM2, eTimCh1);
    gpio_set_mode(PIN_SERVO_1, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_1, GPIO_AF_TIM2);
    hal_tim_pwm_configure_channel(TIM2, eTimCh2);
    gpio_set_mode(PIN_SERVO_5, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_5, GPIO_AF_TIM2);
    hal_tim_pwm_configure_channel(TIM2, eTimCh4);
    gpio_set_mode(PIN_SERVO_6, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_6, GPIO_AF_TIM2);

    // Initialize Timer 3 Channels
    hal_tim_pwm_configure_channel(TIM3, eTimCh1);
    gpio_set_mode(PIN_SERVO_2, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_2, GPIO_AF_TIM3_5);
    hal_tim_pwm_configure_channel(TIM3, eTimCh2);
    gpio_set_mode(PIN_SERVO_3, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_3, GPIO_AF_TIM3_5);
    hal_tim_pwm_configure_channel(TIM3, eTimCh3);
    gpio_set_mode(PIN_SERVO_7, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_7, GPIO_AF_TIM3_5);
    hal_tim_pwm_configure_channel(TIM3, eTimCh4);
    gpio_set_mode(PIN_SERVO_8, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_8, GPIO_AF_TIM3_5);

    // Initialize Timer 4 Channels
    hal_tim_pwm_configure_channel(TIM4, eTimCh1);
    gpio_set_mode(PIN_SERVO_4, GPIO_MODE_AF);
    gpio_set_af(PIN_SERVO_4, GPIO_AF_TIM3_5);

    gpio_set_mode(PIN_SERVO_EN, GPIO_MODE_OUTPUT);
    gpio_write(PIN_SERVO_EN, true);
}   


static void servoCtrl_set(enum ePayloadServo srv, uint32_t val_us){
    if(val_us > SERVO_MAX_PWM) val_us = SERVO_MAX_PWM;
    switch(srv){
        case eServo1:
            hal_tim_pwm_set(TIM2, eTimCh1, val_us);
            break;
        case eServo2:
            hal_tim_pwm_set(TIM3, eTimCh1, val_us);
            break;
        case eServo3:
            hal_tim_pwm_set(TIM3, eTimCh2, val_us);
            break;
        case eServo4:
            hal_tim_pwm_set(TIM4, eTimCh1, val_us);
            break;
        case eServo5:
            hal_tim_pwm_set(TIM2, eTimCh2, val_us);
            break;
        case eServo6:
            hal_tim_pwm_set(TIM2, eTimCh4, val_us);
            break;
        case eServo7:
            hal_tim_pwm_set(TIM3, eTimCh3, val_us);
            break;
        case eServo8:
            hal_tim_pwm_set(TIM3, eTimCh4, val_us);
            break;
        default:
            break;
    };
}

#endif
