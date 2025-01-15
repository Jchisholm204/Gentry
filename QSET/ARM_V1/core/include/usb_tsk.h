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

#ifndef _USB_TSK_H_
#define _USB_TSK_H_

#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/usb.h"
#include "os/hal/stusb/usb_cdc.h"

// USB CDC Protocol to use
#define CDC_PROTOCOL USB_PROTO_IAD

// Do not change (Control EP0 Size)
#define CDC_EP0_SIZE    0x08

// Virtual Com Port
#define VCOM_RXD_EP      0x01
#define VCOM_TXD_EP      0x81
#define VCOM_DATA_SZ     0x40
#define VCOM_NTF_EP      0x82
#define VCOM_NTF_SZ      0x08

// Device Control
#define CTRL_RXD_EP      0x03
#define CTRL_TXD_EP      0x83
#define CTRL_DATA_SZ     0x40
#define CTRL_NTF_EP      0x84
#define CTRL_NTF_SZ      0x08

/* Declaration of the report descriptor */
struct udev_config {

    struct usb_config_descriptor        config;

    struct usb_iad_descriptor           vcom_iad;
    struct usb_interface_descriptor     vcom;
    struct usb_cdc_header_desc          vcom_hdr;
    struct usb_cdc_call_mgmt_desc       vcom_mgmt;
    struct usb_cdc_acm_desc             vcom_acm;
    struct usb_cdc_union_desc           vcom_union;
    struct usb_endpoint_descriptor      vcom_ep;
    struct usb_interface_descriptor     vcom_data;
    struct usb_endpoint_descriptor      vcom_eprx;
    struct usb_endpoint_descriptor      vcom_eptx;

    struct usb_iad_descriptor           ctrl_iad;
    struct usb_interface_descriptor     ctrl;
    struct usb_cdc_header_desc          ctrl_hdr;
    struct usb_cdc_call_mgmt_desc       ctrl_mgmt;
    struct usb_cdc_acm_desc             ctrl_acm;
    struct usb_cdc_union_desc           ctrl_union;
    struct usb_endpoint_descriptor      ctrl_ep;
    struct usb_interface_descriptor     ctrl_data;
    struct usb_endpoint_descriptor      ctrl_eprx;
    struct usb_endpoint_descriptor      ctrl_eptx;
} __attribute__((packed));




#endif
