/**
 * @file usb_dev.h
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief QSET Arm Control USB Device Definitions
 * @version 0.1
 * @date 2025-01-19
 * 
 * @copyright Copyright (c) 2023
 * 
 * USB Device Definitions:
 *  - Endpoint Definitions
 *  - USB Device IDs
 *
 * This file is used by the driver and the embedded device
 * Both must be recompiled/flashed when changed
 *
 */

#ifndef _USB_DEV_H_
#define _USB_DEV_H_

#ifndef UDEV_VERSION
#define UDEV_VERSION "1.0.0"
#endif

#define UDEV_INTERFACES  0x04

// Virtual Com Port Interface
#define VCOM_RXD_EP      0x01
#define VCOM_TXD_EP      0x81
#define VCOM_DATA_SZ     0x40
#define VCOM_NTF_EP      0x82
#define VCOM_NTF_SZ      0x08
#define VCOM_NTF_INUM    0x00
#define VCOM_DATA_INUM   0x01

// Device Control Interface
#define CTRL_RXD_EP      0x03
#define CTRL_TXD_EP      0x83
#define CTRL_DATA_SZ     0x40
#define CTRL_NTF_EP      0x84
#define CTRL_NTF_SZ      0x08
#define CTRL_NTF_INUM    0x02
#define CTRL_DATA_INUM   0x03

// USB Device Vendor ID:
//  Use 0xFFFF or 0xFFFE as designated by the USBIF,
//  These vendor ID's are reserved for test or hobby devices and
//  will not conflict with registered vendor drivers
#define VENDOR_ID 0xFFFE
// USB Device Product ID:
//  Used to seperate usb devices from the same vendor
//  Must be different for each type of device
#define DEVICE_ID 0x0A4D

#endif  // INCLUDE_INCLUDE_USB_DEV_H_

