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
    eLight1 = 0x00,
    eLight2 = 0x02,
    eLight3 = 0x01,
    eLight4 = 0x03,
    eLight5 = 0x04,
    eLight6 = 0x05,
    eLight7 = 0x0A,
    eLight8 = 0x0B,
    eLight9 = 0x08,
    eLight10 = 0x09,
    eLight11 = 0x0E,
    eLight12 = 0x0F,
    eLightAllOFF = 0x0C,
};


// Motor Select
enum ePayloadMotor {
    eMotor1,
    eMotor2,
    eN_Motors
};

// Motor Select
enum ePayloadStepper {
    eStepper1,
    eStepper2,
    eN_Steppers
};

enum ePayloadStatus {
    ePayloadOK,
    // Board is still waiting for one or more initialization message
    ePayloadUnInit,
    // Initialization failed on an interface
    ePayloadIniFail,
    // The board was unable to connect to an I2C Device
    ePayloadI2CFail,
    ePayloadFail,
    ePayloadNoConnection,
};

#ifdef __cplusplus
}
#endif

#endif
