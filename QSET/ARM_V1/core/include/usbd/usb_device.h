/**
 * @file usb_device.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief QSET USB Device
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _USB_DEVICE_H_
#define _USB_DEVICE_H_

#include "usbd/usb_packet.h"
#include "stdlib.h"

// USB Device Initializer (Only Call ONCE)
extern void udev_init(void);

// Update the UDEV motor info passed back to the host device
extern void udev_setMtr(int mtr, struct udev_mtr_info *pInfo);

// Get the latest control data from the USB Host
extern void udev_getMtr(int mtr, struct udev_mtr_ctrl *pCtrl);

// Get the latest servo data from the USB device
extern uint8_t udev_getSrv(int servo);

// Update the UDEV limit switch info passed back to the host device
extern void udev_setLmt(uint8_t lsw);

// Get the latest tool power output data from the USB Host
extern uint8_t udev_getTPR(void);

// Set the device status passed back to the host
extern void udev_setSts(uint8_t sts);

// Get the status given to the device from the host
extern uint8_t udev_getSts(void);

/**
 * @brief Write to the USB VCOM Port
 *
 * @param data Data to Write
 * @param len length of data
 * @return 
 */
extern size_t udev_write(char *data, size_t len);

/**
 * @brief Read from the USB VCOM Port
 *
 * @param data buffer to copy into
 * @param len length of data to copy
 * @return 
 */
extern size_t udev_read(char *data, size_t len);

#endif
