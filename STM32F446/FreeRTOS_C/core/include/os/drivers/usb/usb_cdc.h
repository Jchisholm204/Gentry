/**
 * @file usb_cdc.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief USB CDC Definitions
 * @date 2024-11-17
 * @version 0.1
 * 
 */

#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include "os/hal/hal_usbfs.h"
#include "os/drivers/usb/usb_core.h"

/**
 * @class usb_cdc_header_desc
 * @brief CDC Header Functional Description
 * @param bFunctionLength      Size of this descriptor
 * @param bDescriptorType      CS_INTERFACE descriptor type
 * @param bDescriptorSubType   Header Functional Description Subtype
 * @param Device Specification Release Number
 */
struct usb_cdc_header_desc {
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubType;
} __attribute__((packed));

/**
 * @class usb_dev_request
 * @brief USB Device Request Packet (USB SPEC pg 277)
 * @param bmRequestType Bitmap Field for Request Direction
 * @param bRequest Specific Request Number
 * @param wValue parameter passed to device
 * @param wIndex  parameter passed to device
 * @param wLength Length of request
 * @param Data Data Packed with Request
 */
struct usb_cdc_dev_request {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
    uint8_t  Data[];
} __attribute__((packed));



/**
 * @class usb_union_desc
 * @brief Union Functional Descriptor
 * Describes the relation between a group of interfaces that can be considered as
 *  a single functional unit. Can only occur within the class-specific portion of
 *  an interface descriptor.
 *
 * @param bFunctionLength    Size of this descriptor
 * @param bDescriptorType    CS_INTERFACE
 * @param bDescriptorSubType Union Functional Descriptor Subtype
 * @param bControlInterface  The interface number for the controlling interface
 * @param bSubordinateInterface0 Interface number of the first subordinate interface
 * @param bSubordinateInterface1 OPTIONAL Interface number of the 2nd subordinate interface
 */
struct usb_union_desc {
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    uint8_t bControlInterface;
    uint8_t bSubordinateInterface0;
    uint8_t bSubordinateInterface1;
} __attribute__((packed));




#endif

