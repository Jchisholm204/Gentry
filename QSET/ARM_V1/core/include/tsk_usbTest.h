/**
 * @file usb_tsk.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2023
 * 
 * QSET USB CDC Device
 * Contains CDC interfaces for:
 *  - ctrl
 *  - vcom
 *
 */

#ifndef _TSK_TEST_USB_H_
#define _TSK_TEST_USB_H_

#include "os/stusb/stm32_compat.h"
#include "os/stusb/stm32_compat.h"
#include "os/stusb/usb.h"
#include "os/stusb/usb_cdc.h"

void vTsk_usbTest(void* pvParams);

#endif
