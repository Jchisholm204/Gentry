/**
 * @file usb_tsk.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2023
 * 
 * QSET USB CDC Device
 * Contains CDC interfaces for:
 *  - ctrl
 *  - vcom
 *
 */

#ifndef _USB_TSK_H_
#define _USB_TSK_H_

#include "os/stusb/stm32_compat.h"
#include "os/stusb/stm32_compat.h"
#include "os/stusb/usb.h"
#include "os/stusb/usb_cdc.h"


struct arm_control {
    uint8_t control_type;
    uint8_t status;
    uint8_t ctrl_tool;
    uint8_t limit_sw;
    uint8_t ctrl_servo[4];
    uint16_t motor_pos[6];
} __attribute__((packed));


#endif
