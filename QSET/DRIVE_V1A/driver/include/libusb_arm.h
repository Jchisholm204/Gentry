/**
 * @file libusb_arm.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Arm
 * @version 0.1
 * @date 2025-01-25
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#ifndef _LIBUSB_ARM_H_
#define _LIBUSB_ARM_H_
#include "usb_arm_defs.h"
#include "usb_dev.h"
#include "usb_packet.h"

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

#define AK_MTR_EN   0x01
#define AK_MTR_ZERO 0x02

typedef struct _armDev{
    libusb_context *lusb_ctx;
    libusb_device_handle *lusb_devHndl;
    struct udev_mtr_ctrl pkt_mtr[ARM_N_MOTORS];
    struct udev_pkt_status pkt_status;
    int err;
} armDev_t;

int armDev_init(armDev_t *pDev);

// BLOCKING FUNCTION
int armDev_reconnect(armDev_t *pDev);

int armDev_get(armDev_t *pDev);

uint8_t armDev_getLS(armDev_t *pDev);

struct udev_mtr_info *armDev_getMtrInfo(armDev_t *pDev, enum eArmMotors mtr);

struct udev_pkt_status *armDev_getStatusPkt(armDev_t *pDev);

int armDev_setServo(armDev_t *pDev, enum eArmServos servo, uint32_t val_us);

int armDev_setMtr(armDev_t *pDev, enum eArmMotors mtr, struct udev_mtr_ctrl *pVals);

int armDev_setGripper(armDev_t *pDev, int8_t val);

int armDev_updateMtr(armDev_t *pDev, enum eArmMotors mtr, float pos, float vel);

const char *armDev_getErr(armDev_t *pDev);

int armDev_free(armDev_t *pDev);


#endif
