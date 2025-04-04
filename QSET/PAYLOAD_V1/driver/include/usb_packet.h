/**
 * @file usb_packet.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Packet Declarations
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

#ifndef _USB_PACKET_H_
#define _USB_PACKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <assert.h>
#include "usb_payload_defs.h"
#include "usb_dev.h"

enum ePktType {
    ePktStepSetup,
    ePktStepCtrl,
    ePktStepInfo,
    ePktLightCtrl,
    ePktServoCtrl,
};

struct udev_pkt_hdr {
    uint8_t ePktType;
} __attribute__((packed));

struct udev_status {
    // Status code given by the enum
    // enum eDrvStatus code;
    uint8_t code;
    // Error Message Associated with the Code
    char msg[16];
};

// Setup Packet Structure for Stepper Motors
struct udev_stepper_setup {
    // 4 bits
    uint8_t holdCurrent_mA;
    // 4 bits
    uint8_t runCurrent_mA;
    // 4 bits
    uint8_t hold_delay;
} __attribute__((packed));

// Control Packet Structure for Stepper Motors
struct udev_stepper_ctrl {
    // Speed to step at (signed, +/- SERVO_MAX_VEL)
    int32_t speed;
    // Position to step to
    uint32_t position;
} __attribute__((packed));

// Info Structure from TMC2208
struct udev_stepper_info {
    // 24 bits signed
    int32_t velocity;
    // Coil Currents (16 bit unsigned)
    uint16_t curA_mA;
    uint16_t curB_mA;
} __attribute__((packed));

// Servo Control Structure 
struct udev_servo_ctrl {
    // PWM Channel to write to
    uint8_t ePWMChannel;
    // Value to write to the channel
    uint32_t value;
} __attribute__((packed));

// Light Control Structure
struct udev_light_ctrl {
    // ePayloadLight to turn on
    uint8_t eLightChannel;
} __attribute__((packed));

// PWM Motor Control Structure
struct udev_mtr_ctrl {
    // Motor Channel to write to (ePayloadMotor)
    uint8_t eMtrChannel;
    // Power Value for Motor (signed, -127, 127)
    int8_t  value;
} __attribute__((packed));

// ADC Sensor Readings Structure
struct udev_adc_info {
    // ADC_13 Reading (V)
    float ADC_13;
    // Internal Temperature Reading (deg C)
    float temp_internal;
} __attribute__((packed));

// BME680 Sensor Data Structure
struct udev_bme_info {
    // BME680 Reported Temperature Reading (deg C)
    float temp;
    // BME680 Reported Pressure (hPa)
    float pressure;
    // BME680 Reported Humidity (%RH)
    float humidity;
    // BME680 Reported Gas Resistance (Ohms)
    float gas_resistance;
} __attribute__((packed));

// USB Device Control Packet
//  From Host to Device
//  Must be less than 0x40 in length
struct udev_pkt_ctrl {
    struct udev_pkt_hdr hdr;
    union {
        struct udev_stepper_setup stepSetp;
        struct udev_stepper_ctrl  stepCtrl;
        struct udev_servo_ctrl    servoCtrl;
        struct udev_light_ctrl    lightCtrl;
        struct udev_mtr_ctrl      mtrCtrl;
    };
} __attribute__((packed));

// USB Device Info Packet:
//  From Device to Host
//  Must be less than 0x40 in length
struct udev_pkt_info {
    struct udev_status status;
    struct udev_stepper_info stepper1;
    struct udev_stepper_info stepper2;
    struct udev_adc_info     adc_info;
    struct udev_bme_info     bme_info;
} __attribute__((packed));


// USB Packets must be less than 0x40/64 bytes in length
static_assert(sizeof(struct udev_pkt_ctrl) <= PYLD_DATA_SZ, "USBD Packet Oversize");
static_assert(sizeof(struct udev_pkt_info) <= PYLD_DATA_SZ, "USBD Packet Oversize");

#ifdef __cplusplus
}
#endif

#endif


