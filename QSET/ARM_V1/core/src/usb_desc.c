/**
 * @file usb_desc.c
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief QSET Arm Control USB Device Descriptor
 * @version 0.1
 * @date 2025-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 *
 */
#include <memory.h>
#include "usb_desc.h"


static usbd_respond udev_getdesc (usbd_ctlreq *req, void **address, uint16_t *length) {
    const uint8_t dtype = req->wValue >> 8;
    const uint8_t dnumber = req->wValue & 0xFF;
    const void* desc;
    uint16_t len = 0;
    switch (dtype) {
    case USB_DTYPE_DEVICE:
        desc = &device_desc;
        break;
    case USB_DTYPE_CONFIGURATION:
        desc = &config_desc;
        len = sizeof(config_desc);
        break;
    case USB_DTYPE_STRING:
        if (dnumber < 3) {
            desc = dtable[dnumber];
        } else {
            return usbd_fail;
        }
        break;
    default:
        return usbd_fail;
    }
    if (len == 0) {
        len = ((struct usb_header_descriptor*)desc)->bLength;
    }
    *address = (void*)desc;
    *length = len;
    return usbd_ack;
}

static usbd_respond udev_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback) {
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) == (USB_REQ_INTERFACE | USB_REQ_CLASS)
        && req->wIndex == 0 ) {
        switch (req->bRequest) {
        case USB_CDC_SET_CONTROL_LINE_STATE:
            return usbd_ack;
        case USB_CDC_SET_LINE_CODING:
            memcpy(&cdc_line, req->data, sizeof(cdc_line));
            return usbd_ack;
        case USB_CDC_GET_LINE_CODING:
            dev->status.data_ptr = &cdc_line;
            dev->status.data_count = sizeof(cdc_line);
            return usbd_ack;
        default:
            return usbd_fail;
        }
    }
    return usbd_fail;
}

void udev_applydesc(usbd_device *dev){
    usbd_reg_control(dev, udev_control);
    usbd_reg_descr(dev, udev_getdesc);
}

