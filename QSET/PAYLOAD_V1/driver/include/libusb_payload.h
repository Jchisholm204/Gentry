/**
 * @file libusb_chassis.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Chassis
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#ifndef _LIBUSB_CHASSIS_H_
#define _LIBUSB_CHASSIS_H_
#include "usb_payload_defs.h"
#include "usb_dev.h"
#include "usb_packet.h"

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

#define AK_MTR_EN   0x01
#define AK_MTR_ZERO 0x02

typedef struct _chassisDev{
    libusb_context *lusb_ctx;
    libusb_device_handle *lusb_devHndl;
    int err;
} chassisDev_t;

// int chassisDev_init(chassisDev_t *pDev);
//
// // BLOCKING FUNCTION
// int chassisDev_reconnect(chassisDev_t *pDev);
//
// struct udev_mtr_info *chassisDev_getMtrInfo(chassisDev_t *pDev, enum eDrvMotors mtr);
//
// int chassisDev_setLights(chassisDev_t *pDev, uint8_t vals);
//
// int chassisDev_setServo(chassisDev_t *pDev, enum eChassisServo servo, uint32_t val_us);
//
// int chassisDev_setMtr(chassisDev_t *pDev, enum eDrvMotors mtr, struct udev_mtr_ctrl *pVals);
//
// int chassisDev_free(chassisDev_t *pDev);


#endif
