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

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb_packet.h"
#include "stdlib.h"


// USB Device Initializer (Only Call ONCE)
extern void udev_init(void);

extern void vTsk_usb(void *pvParams);

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
