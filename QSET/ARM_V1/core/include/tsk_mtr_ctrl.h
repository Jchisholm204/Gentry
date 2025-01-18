/**
 * @file arm_cfg.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief General ARM Configuration File
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _TSK_MTR_CTRL_H_
#define _TSK_MTR_CTRL_H_

#include "drivers/AK7010/ak7010.h"
#include "usbd/usb_device.h"

// Motor Joint References
#define J1 0
#define J2 1
#define J3 2
#define J4 3

// Time interval to force a motor update in ms
#define MTR_UPDATE_TIME 100

// Motor Joint initializers
static AK7010_t joint[UDEV_N_MOTORS] = {
    // Joint 1
    (AK7010_t){
        .can_id = 0,
        .kP = 0,
        .kI = 0,
        .kD = 0,
        .kF = 0,
        .position = 0,
        .velocity = 0,
        .temp = 0,
        .enable = false,
        .error = 0,
    },
    // Joint 2
    (AK7010_t){
        .can_id = 0,
        .kP = 0,
        .kI = 0,
        .kD = 0,
        .kF = 0,
        .position = 0,
        .velocity = 0,
        .temp = 0,
        .enable = false,
        .error = 0,
    },
    // Joint 3
    (AK7010_t){
        .can_id = 0,
        .kP = 0,
        .kI = 0,
        .kD = 0,
        .kF = 0,
        .position = 0,
        .velocity = 0,
        .temp = 0,
        .enable = false,
        .error = 0,
    },
    // Joint 4
    (AK7010_t){
        .can_id = 0,
        .kP = 0,
        .kI = 0,
        .kD = 0,
        .kF = 0,
        .position = 0,
        .velocity = 0,
        .temp = 0,
        .enable = false,
        .error = 0,
    },
};

/**
 * @brief Motor Control Task
 *
 * @param pvParams Motor ID this task should control
 */
void vTsk_mtr_ctrl(void *pvParams);

#endif
