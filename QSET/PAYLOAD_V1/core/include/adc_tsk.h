/**
 * @file adc_tsk.h
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET ADC Task - Payload Control
 * @date 2025-04-09
 * @version 1.0
 * 
 */

#ifndef _ADC_TSK_H_
#define _ADC_TSK_H_

#include "hal/hal_adc.h"
#include "usb_payload_defs.h"
#include "usb_packet.h"

// Initialize the ADC and its task
extern void adc_init(volatile struct udev_adc_info * const pADC_info);

#endif

