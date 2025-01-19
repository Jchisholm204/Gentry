/**
 * @file usb_packet.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Packet Declarations
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

#ifndef _USB_PACKET_H_
#define _USB_PACKET_H_

#include <stdint.h>
#include <assert.h>
#include "usb_arm_defs.h"
#include "usb_dev.h"

enum ePktType{
    ePktTypeReset,
    ePktTypeMtr,
    ePktTypeSrvo,
    ePktTypeSetup,
};

struct udev_pkt_hdr {
    enum ePktType typ;
} __attribute__((packed));

// Motor Control Structure
struct udev_mtr_ctrl {
    // Motor Position Command
    float position;
    // Motor Velocity Command
    float velocity;
} __attribute__((packed));

// Motor Control Structure
struct udev_mtr_setup {
    // Motor Configuration Data
    float kP, kI, kD, kF;
    uint32_t can_id;
} __attribute__((packed));

// Control Packet:
//  From Host to Device
//  Must be less than 0x40 in length
struct udev_pkt_ctrl {
    struct udev_pkt_hdr hdr;
    union {
        enum eArmMotors mtr;
        enum eArmServos srv;
    } id;
    union{
        uint32_t servo_ctrl;
        // CAN Bus Motor Control
        struct udev_mtr_ctrl mtr_ctrl;
        struct udev_mtr_setup mtr_setup;
    };
} __attribute__((packed));

struct udev_status {
    // Status code given by the enum
    enum eArmStatus code;
    // Extra information could be:
    //  - Error code from the motor
    //  - number of the motor that failed initialization
    uint8_t value;
};

// Motor Control Structure
struct udev_mtr_info {
    // Motor Temperature (in C)
    uint8_t temp;
    // Motor Current in A/10
    uint8_t current;
    // Motor Position
    float position;
    // Motor Velocity
    float velocity;
} __attribute__((packed));

// Status Packet:
//  From Device to Host
//  Must be less than 0x40 in length
struct udev_pkt_status {
    // Device Status
    enum eArmStatus status;
    // Additional status information
    uint8_t status_val;
    // Each bit represents a limit switch that is open (0) or closed (1)
    uint8_t limit_sw;
    // Motor Control Response Information
    struct udev_mtr_info mtr[ARM_N_MOTORS];
} __attribute__((packed));

// USB Packets must be less than 0x40/64 bytes in length
static_assert(sizeof(struct udev_pkt_ctrl) <= CTRL_DATA_SZ, "USBD Control Packet Oversize");
static_assert(sizeof(struct udev_pkt_status) <= CTRL_DATA_SZ, "USBD Status Packet Oversize");

#endif


