/**
 * @file usb_packet.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Control Packet Declaration
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _USB_PACKET_H_
#define _USB_PACKET_H_

#include <stdint.h>
#include <assert.h>

#define UDEV_N_MOTORS 6
#define UDEV_N_SERVOS 4

// USB Status
// Enable the USB Device
#define UDEV_STS_EN    0x01
// Home the Arm
#define UDEV_STS_HOME  0x02
// Set when Arm has reached a software limit or limit switch is closed
#define UDEV_STS_LIMIT 0x04
// Motor Over Temp Detected
#define UDEV_STS_OTEMP 0x08
// Hardware Failure Detected
#define UDEV_STS_HFAIL 0x10
// AK Motor Failure
#define UDEV_STS_HFAIL 0x20

// Bit mappings for the tool power
#define UDEV_TOOL_A1   0x01
#define UDEV_TOOL_A2   0x02
#define UDEV_TOOL_LP1  0x04
#define UDEV_TOOL_LP2  0x08

// Bit mappings for the limit switches
#define UDEV_LSW_1    0x01
#define UDEV_LSW_2    0x02
#define UDEV_LSW_3    0x04
#define UDEV_LSW_4    0x08
#define UDEV_LSW_5    0x10
#define UDEV_LSW_6    0x20

#define BIT_GET(reg, bit) ((reg & (1 << bit)) > 0)
#define BIT_SET(reg, bit) (reg |= (1 << bit))
#define BIT_CLR(reg, bit) (reg &= ~(1 << bit))

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

// Motor Control Structure
struct udev_mtr_ctrl {
    // Motor Position Command
    float position;
    // Motor Velocity Command
    float velocity;
} __attribute__((packed));

// Control Packet:
//  From Host to Device
//  Must be less than 0x40 in length
struct udev_pkt_ctrl {
    // Device Status (Set bit to reset trigger)
    uint8_t status;
    // Enable the tool Power (bitwise controlled)
    uint8_t tool_pwr;
    // Servo PWM Outputs
    uint8_t ctrl_servo[UDEV_N_SERVOS];
    // CAN Bus Motor Control
    struct udev_mtr_ctrl mtr[UDEV_N_MOTORS];
} __attribute__((packed));

// Status Packet:
//  From Device to Host
//  Must be less than 0x40 in length
struct udev_pkt_status {
    // Device Status
    uint8_t status;
    // Each bit represents a limit switch that is open (0) or closed (1)
    uint8_t limit_sw;
    // Motor Control Response Information
    struct udev_mtr_info mtr[UDEV_N_MOTORS];
} __attribute__((packed));

// USB Packets must be less than 0x40/64 bytes in length
static_assert(sizeof(struct udev_pkt_ctrl) <= 0x40, "USBD Control Packet Oversize");
static_assert(sizeof(struct udev_pkt_status) <= 0x40, "USBD Status Packet Oversize");


#endif

