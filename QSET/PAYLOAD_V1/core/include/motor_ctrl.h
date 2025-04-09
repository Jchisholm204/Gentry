/**
 * @file motor_ctrl.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET Motor Control - Payload Control
 * @date 2025-04-09
 * @version 1.0
 * 
 */
#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_
#include "hal/hal_timac_pwm.h"
#include "usb_payload_defs.h"
#include "pin_cfg.h"

// int8_t maximum value (unsigned)
#define MTR_MAX (127)

static void motorCtrl_init(void){
    // Timer 1 Init
    hal_timac_pwm_init(TIM1, (PLL_N/PLL_P)-1, MTR_MAX-1);
    hal_timac_pwm_configure_channel(TIM1, eTimCh1);
    hal_timac_pwm_configure_nChannel(TIM1, eTimCh1);
    gpio_set_mode(PIN_M12P, GPIO_MODE_AF);
    gpio_set_af(PIN_M12P, GPIO_AF_TIM1);
    gpio_set_mode(PIN_M12N, GPIO_MODE_AF);
    gpio_set_af(PIN_M12N, GPIO_AF_TIM1);
    hal_timac_pwm_configure_channel(TIM1, eTimCh3);
    hal_timac_pwm_configure_nChannel(TIM1, eTimCh3);
    gpio_set_mode(PIN_M21P, GPIO_MODE_AF);
    gpio_set_af(PIN_M21P, GPIO_AF_TIM1);
    gpio_set_mode(PIN_M21N, GPIO_MODE_AF);
    gpio_set_af(PIN_M21N, GPIO_AF_TIM1);

    // Timer 8 Init
    hal_timac_pwm_init(TIM8, (PLL_N/PLL_P)-1, MTR_MAX-1);
    hal_timac_pwm_configure_channel(TIM8, eTimCh1);
    hal_timac_pwm_configure_nChannel(TIM8, eTimCh1);
    gpio_set_mode(PIN_M11P, GPIO_MODE_AF);
    gpio_set_af(PIN_M11P, GPIO_AF_TIM8_11);
    gpio_set_mode(PIN_M11N, GPIO_MODE_AF);
    gpio_set_af(PIN_M11N, GPIO_AF_TIM8_11);
    hal_timac_pwm_configure_channel(TIM8, eTimCh2);
    hal_timac_pwm_configure_nChannel(TIM8, eTimCh2);
    gpio_set_mode(PIN_M22P, GPIO_MODE_AF);
    gpio_set_af(PIN_M22P, GPIO_AF_TIM8_11);
    gpio_set_mode(PIN_M22N, GPIO_MODE_AF);
    gpio_set_af(PIN_M22N, GPIO_AF_TIM8_11);

    // M11
    hal_timac_pwm_set(TIM8, eTimCh1, (uint32_t)(0));
    // M12
    hal_timac_pwm_set(TIM1, eTimCh1, (uint32_t)(0));
    // M21
    hal_timac_pwm_set(TIM1, eTimCh3, (uint32_t)(0));
    // M22
    hal_timac_pwm_set(TIM8, eTimCh2, (uint32_t)(0));
}


static inline void motorCtrl_set(enum ePayloadMotor motor, int8_t speed){
    // H-Bridge Speed Logic - PWM high side, Ground low side
    // High and low variables refer to the +/- labeling on the board
    // If the code labels are swapped, the motor will run in reverse
    uint32_t high = 0, low = 0;
    if(speed >= 0){
        // Pulse the high side between Voltage/Ground
        high = (uint32_t)((uint8_t)(speed) & 0xFF);
        // Pin the low side to ground
        low  = 127;
    }
    else{
        // Pin the high side to ground
        high = 0;
        // Pulse the low side between Voltage/Ground
        low  = (uint32_t)((uint8_t)(127 + speed) & 0xFF);
    }

    // Switch for the different motors
    switch(motor){
        case eMotor1:
            // M11
            hal_timac_pwm_set(TIM8, eTimCh1, high);
            // M12
            hal_timac_pwm_set(TIM1, eTimCh1, low);
            break;
        case eMotor2:
            // M21
            hal_timac_pwm_set(TIM1, eTimCh3, high);
            // M22
            hal_timac_pwm_set(TIM8, eTimCh2, low);
            break;
        default:
            break;
    }

}

#endif
