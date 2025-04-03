/**
 * @file usb_payload_defs.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief QSET Payload Interface Definitions
 * @version 0.1
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2023
 *
 *
 * This file is used by the driver and the embedded device
 * Both must be recompiled/flashed when changed
 *
 */
#ifndef _USB_PYLD_DEFS_H_
#define _USB_PYLD_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

// Servo Channel Select
enum ePayloadServo {
    eServo1,
    eServo2,
    eServo3,
    eServo4,
    eServo5,
    eServo6,
    eServo7,
    eServo8,
    eN_Servo
};

// Light Select
enum ePayloadLight {
    eLight1,
    eLight2,
    eLight3,
    eLight4,
    eLight5,
    eLight6,
    eLight7,
    eLight8,
    eLight9,
    eLight10,
    eLight11,
    eLight12,
    eLightAllOFF,
    eN_Lights
};


// Motor Select
enum ePayloadMotor {
    eMotor1,
    eMotor2
};

#define SERVO_MAX_VEL 10000

enum ePayloadStatus {
    ePayloadOK,
    // Board is still waiting for one or more initialization message
    ePayloadUnInit,
    // Initialization failed on an interface
    ePayloadIniFail,
    // The board was unable to connect to an I2C Device
    ePayloadI2CFail,
};

#ifdef __cplusplus
}
#endif

#endif
