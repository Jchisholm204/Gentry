/**
 * @file usb_arm_defs.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief QSET ARM Interface Definitions
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2023
 *
 *
 * This file is used by the driver and the embedded device
 * Both must be recompiled/flashed when changed
 *
 */
#ifndef _USB_ARM_DEFS_H_
#define _USB_ARM_DEFS_H_

enum eArmMotors {
    eJoint1,
    eJoint2,
    eJoint3,
    eJoint4,
    ARM_N_MOTORS
};

enum eArmServos {
    eServo1,
    eServo2,
    eServo3,
    eServo4,
    ARM_N_SERVOS
};

// Bit mappings for the limit switches
enum eArmLimitSW {
    LIMITSW_1 = 0x01,
    LIMITSW_2 = 0x02,
    LIMITSW_3 = 0x04,
    LIMITSW_4 = 0x08,
    LIMITSW_5 = 0x10,
    LIMITSW_6 = 0x20,
};

enum eArmStatus {
    eArmOK,
    // Arm is still waiting for one or more initialization message
    eArmUnInit,
    // Initialization failed on an interface
    eArmIniFail,
    // A bus or task has stalled triggering the watchdog (will self resolve in time)
    eArmStall,
    // A motor has failed to respond in a timely manner
    eArmMtrFail,
};

#endif
