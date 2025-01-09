/**
 * @file usbd.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief USB Device FreeRTOS Driver
 * @version 0.1
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _USBD_H_
#define _USBD_H_
#include <stm32f4xx.h>
#include "os/hal/hal_usbdef.h"
#include "os/hal/hal_usbfs.h"

struct usb_cdc_header_desc {
    uint8_t     bFunctionLength;    /**<\brief Size of this descriptor in bytes.*/
    uint8_t     bDescriptorType;    /**<\brief CS_INTERFACE descriptor type.*/
    uint8_t     bDescriptorSubType; /**<\brief Header functional descriptor subtype.*/
    uint16_t    bcdCDC;             /**<\brief USB CDC Specification release number in BCD.*/
} __attribute__ ((packed));

struct usbd_def {
    struct usb_config_descriptor     config;
    struct usb_iad_descriptor        iad;
    struct usb_interface_descriptor  interface;
};

typedef struct USBD {
    
} USBD_t;

#endif
