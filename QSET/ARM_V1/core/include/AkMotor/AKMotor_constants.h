/**
 * @file AKMotor_Constants.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief AK Motor Constants
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _AKMOTOR_CONSTANTS_H_
#define _AKMOTOR_CONSTANTS_H_

enum AKMotorType{
    eAK7010,
    eAK_N_MOTORS
};

struct AKMotorConfig {
    float pos_min, pos_max;
    float vel_min, vel_max;
    float trq_min, trq_max;
    float kp_min,  kp_max;
    float kd_min,  kd_max;
    float cur_min, cur_max;
    float tmp_min, tmp_max;
};

// AK Motor Configurations - Must be in order of AKMotorType enum
static const struct AKMotorConfig AKConfigs[eAK_N_MOTORS] = {
    // AK70-10 Configuration
    (const struct AKMotorConfig){
        .pos_min = -12.5,
        .pos_max = 12.5,
        .vel_min = -50,
        .vel_max = 50,
        .trq_min = -25,
        .trq_max = 25,
        .kp_min = 0,
        .kp_max = 500,
        .kd_min = 0,
        .kd_max = 5,
        .cur_min = -60,
        .cur_max = 60,
        .tmp_min = -20,
        .tmp_max = 127
    },
};


#endif
