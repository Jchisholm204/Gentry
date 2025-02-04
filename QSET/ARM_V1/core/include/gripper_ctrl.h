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

#define GRIPPER_TIM TIM2
#define GPOW_FACTOR 20

static void gripCtrl_init(uint16_t pre, uint16_t arr){
    // Arm Board uses Timer 3
    hal_tim_pwm_init(GRIPPER_TIM, pre, arr);
    hal_tim_pwm_configure_channel(GRIPPER_TIM, eTimCh1);
    hal_tim_pwm_configure_channel(GRIPPER_TIM, eTimCh2);
    // Setup the GPIO pins
    gpio_set_mode(PIN_MTR_PWM1, GPIO_MODE_AF);
    gpio_set_af(PIN_MTR_PWM1, GPIO_AF_TIM2);
    gpio_set_mode(PIN_MTR_PWM2, GPIO_MODE_AF);
    gpio_set_af(PIN_MTR_PWM2, GPIO_AF_TIM2);
    gpio_set_mode(PIN_MTR_EN, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_MTR_EN, true);
}


static void gripCtrl_set(int8_t pow){
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh1, GPOW_FACTOR);
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh2, GPOW_FACTOR);
    if(pow == 0){
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh1, GPOW_FACTOR);
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh2, GPOW_FACTOR);
    }
    else if(pow > 0){
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh1, 0);
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh2, ((uint32_t)pow)*GPOW_FACTOR);
    }
    else if(pow < 0){
        pow = -pow;
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh1, ((uint32_t)pow)*GPOW_FACTOR);
        hal_tim_pwm_set(GRIPPER_TIM, eTimCh2, 0);
    }
}

#endif
