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

#ifndef _MTR_CTRL_H_
#define _MTR_CTRL_H_

#include "FreeRTOS.h"
#include "task.h"

#include "AkMotor/AKMotor.h"


// Time interval to force a motor update in ms
#define MTR_UPDATE_TIME 100


/**
 * @struct _mtr_controller
 * @brief Motor Control Task Data Structure
 *
 */
struct _mtr_controller {
    AkMotor_t ak_motor;
    TaskHandle_t pRxHndl;
};

/**
 * @brief Motor Control Task
 *
 * @param pvParams Motor ID this task should control
 */
void vTsk_mtr_ctrl(void *pvParams);

#endif
