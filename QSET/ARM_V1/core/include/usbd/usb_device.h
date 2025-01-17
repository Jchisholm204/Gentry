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

extern void udev_init(void);

extern void udev_setMtr(int mtr, struct udev_mtr_info *pInfo);

extern void udev_getMtr(int mtr, struct udev_mtr_ctrl *pCtrl);

extern uint8_t udev_getSrv(int servo);

extern void udev_setLmt(uint8_t lsw);

extern uint8_t udev_getTPR(void);

extern void udev_setSts(uint8_t sts);

extern uint8_t udev_getSts(void);

extern size_t udev_write(char *data, size_t len);

extern size_t udev_read(char *data, size_t len);

#endif
