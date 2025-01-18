/**
 * @file tsk_mtr_ctrl.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Motor Control Task
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"
#include "drivers/ak7010.h"
#include "usbd/usb_device.h"

#define MAILBOX_SIZE 1

void MotorTask(void *pvParams){
    AK7010_t *motor = pvParams;
    (void)motor;
    for(;;){
    }
}
