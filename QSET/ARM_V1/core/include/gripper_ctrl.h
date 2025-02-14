/**
 * @file gripper_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Gripper Control - Arm Board
 * @date 2025-01-24
 * @version 1.0
 * 
 */
#ifndef _GRIPPER_CTRL_H_
#define _GRIPPER_CTRL_H_

#include "hal/hal_tim_pwm.h"
#include "config/pin_cfg.h"
#include "usb_arm_defs.h"

#define GRIP_IDL_PWR 1000

static void gripCtrl_init(uint16_t pre, uint16_t arr){
    // Arm Board uses Timer 2
    hal_tim_pwm_init(TIM2, pre, arr); // Must call twice
    hal_tim_pwm_init(TIM2, pre, arr);
    hal_tim_pwm_configure_channel(TIM2, eTimCh1);
    hal_tim_pwm_configure_channel(TIM2, eTimCh2);
    hal_tim_pwm_configure_channel(TIM2, eTimCh3);
    hal_tim_pwm_configure_channel(TIM2, eTimCh4);
    gpio_set_mode(PIN_MTR_EN, GPIO_MODE_OUTPUT);
    gpio_write(PIN_MTR_EN, true);

    gpio_set_mode(PIN_MTR_PWM1, GPIO_MODE_AF);
    gpio_set_af(PIN_MTR_PWM1, GPIO_AF_TIM2);
    gpio_set_mode(PIN_MTR_PWM2, GPIO_MODE_AF);
    gpio_set_af(PIN_MTR_PWM2, GPIO_AF_TIM2);
}


static void gripCtrl_set(int8_t pow){
    // hal_tim_pwm_set(TIM2, eTimCh1, GPOW_FACTOR);
    // hal_tim_pwm_set(TIM2, eTimCh2, GPOW_FACTOR);
    if(pow == 0){
        hal_tim_pwm_set(TIM2, eTimCh1, GRIP_IDL_PWR);
        hal_tim_pwm_set(TIM2, eTimCh3, GRIP_IDL_PWR);
    }
    else if(pow > 0){
        hal_tim_pwm_set(TIM2, eTimCh1, 0);
        hal_tim_pwm_set(TIM2, eTimCh3, ((uint32_t)pow*10000)/127);
    }
    else if(pow < 0){
        pow = -pow;
        hal_tim_pwm_set(TIM2, eTimCh1, ((uint32_t)pow*10000)/127);
        hal_tim_pwm_set(TIM2, eTimCh3, 0);
    }
}

#endif
