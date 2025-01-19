/**
 * @file usb_mtr.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief QSET USB Device - Motor Task Interface
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _USB_MOTOR_H_
#define _USB_MOTOR_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb_packet.h"
#include "stdlib.h"

// USB Device Motor Mailbox
struct udev_mtr_mailbox {
    enum eArmMotors mtr_id;
    TaskHandle_t pRxHndl;
    struct udev_mtr_setup setup;
    struct udev_mtr_ctrl  cntrl;
    uint8_t error;
};

extern void udev_mtr_mailboxInit(struct udev_mtr_mailbox *pMailbox);

extern int udev_mtr_mailboxPoll(struct udev_mtr_mailbox *pMailbox, struct udev_mtr_info *pInfo);

// Set the device status passed back to the host
extern void udev_reportError(enum eArmStatus code, uint8_t value);

#endif
