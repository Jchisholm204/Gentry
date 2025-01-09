/**
 * @file usb_core.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief USB Core Definitions
 * @date 2024-11-17
 * @version 0.1
 * 
 */

#ifndef _USB_CORE_H_
#define _USB_CORE_H_
#include "stm32f446xx.h"
#include <stm32f4xx.h>
#include <stdbool.h>
#include "os/hal/hal_usbfs.h"

enum usb_dev_class {
    // Class defined on interface level
    eUSB_CLASS_PER_INTERFACE = 0x00,
    // No subclass defined
    eUSB_SUBCLASS_NONE       = 0x00,
    // No protocol defined
    eUSB_PROTO_NONE          = 0x00,
    // Audio Device Class
    eUSB_CLASS_AUDIO         = 0x01,
    // Communications Device Class
    eUSB_CLASS_CDC           = 0x02,
    // Human Input Device
    eUSB_CLASS_HID           = 0x03,
    // Physical Device Class
    eUSB_CLASS_PHYSICAL      = 0x05,
    // Still Imaging Device
    eUSB_CLASS_STILL_IMAGE   = 0x06,
    // Printer Device
    eUSB_CLASS_PRINTER       = 0x07,
    // Mass Storage Device
    eUSB_CLASS_MASS_STORAGE  = 0x08,
    // USB Hub Class
    eUSB_CLASS_HUB           = 0x09,
    // USB Communications Device Data Class
    eUSB_CLASS_CDC_DATA      = 0x0A,
    // Smart Card Device Class
    eUSB_CLASS_CSCID         = 0x0B,
    // Content Security Device Class
    eUSB_CLASS_CONTENT_SEC   = 0x0D,
    // Video Device Class
    eUSB_CLASS_VIDEO         = 0x0E,
    // Personal Healthcare Device Class
    eUSB_CLASS_HEALTHCARE    = 0x0F,
    // Audio Video Device Class
    eUSB_CLASS_AV            = 0x10,
    // Billboard Device Class
    eUSB_CLASS_BILLBOARD     = 0x11,
    // USB A to C Bridge Class
    eUSB_CLASS_CBRIDGE       = 0x12,
    // Diagnostic Device Class
    eUSB_CLASS_DIAGNOSTIC    = 0xDC,
    // Wireless Controller Device Class
    eUSB_CLASS_WIRELESS      = 0xE0,
    // MISC Device Class
    eUSB_CLASS_MISC          = 0xEF,
    // Application Specific Device Class
    eUSB_CLASS_APP_SPEC      = 0xFE,
    // Vendor Specific Device Class
    eUSB_CLASS_VENDOR        = 0xFF,
    // Vendor Specific Device Subclass
    eUSB_SUBCLASS_VENDOR     = 0xFF,
    // Vendor Specific Protocol Device Subclass
    eUSB_PROTO_VENDOR        = 0xFF,
};

// USB Standard Descriptor Types
enum usb_descriptor_type {
    eUSB_DTYPE_DEVICE         = 0x01,
    eUSB_DTYPE_CONFIGURATION  = 0x02,
    eUSB_DTYPE_STRING         = 0x03,
    eUSB_DTYPE_INTERFACE      = 0x04,
    eUSB_DTYPE_ENDPOINT       = 0x05,
    eUSB_DTYPE_QUALIFIER      = 0x06,
    eUSB_DTYPE_OTHER          = 0x07,
    eUSB_DTYPE_INTERFACEPOWER = 0x08,
    eUSB_DTYPE_OTG            = 0x09,
    eUSB_DTYPE_DEBUG          = 0x0A,
    eUSB_DTYPE_INTERFACEASSOC = 0x0B,
    eUSB_DTYPE_CS_INTERFACE   = 0x24,
    eUSB_DTYPE_CS_ENDPOINT    = 0x25,
};

enum usb_request_typ {
    // Data Transfer Direction Mask
    eUSB_REQ_TYP_DIR_Msk        = 0x80,
    // Data Transfer Direction: Device to Host
    eUSB_REQ_TYP_DIR_DTH        = 0x80,
    // Data Transfer Direction: Host to Device
    eUSB_REQ_TYP_DIR_HTD        = 0x00,
    // Request Type Mask
    eUSB_REQ_TYP_TYPE_Msk       = 0x60,
    // Standard Request BMP
    eUSB_REQ_TYP_TYPE_Standard  = 0x00,
    // Class Specific Request
    eUSB_REQ_TYP_TYPE_Class     = 0x20,
    // Vendor Specific Request
    eUSB_REQ_TYP_TYPE_Vendor    = 0x40,
    // Recipient Type
    eUSB_REQ_TYP_RECP_Msk       = 0x0F,
    eUSB_REQ_TYP_RECP_Device    = 0x00,
    eUSB_REQ_TYP_RECP_Interface = 0x01,
    eUSB_REQ_TYP_RECP_Endpoint  = 0x02,
    eUSB_REQ_TYP_RECP_Other     = 0x03,
};

enum usb_std_request {
    eUSB_STDREQ_GET_STATUS        = 0x00,
    eUSB_STDREQ_CLEAR_FEATURE     = 0x01,
    eUSB_STDREQ_SET_FEATURE       = 0x03,
    eUSB_STDREQ_SET_ADDRESS       = 0x05,
    eUSB_STDREQ_GET_DESCRIPTOR    = 0x06,
    eUSB_STDREQ_SET_DESCRIPTOR    = 0x07,
    eUSB_STDREQ_GET_CONFIGURATION = 0x08,
    eUSB_STDREQ_SET_CONFIGURATION = 0x09,
    eUSB_STDREQ_GET_INTERFACE     = 0x0A,
    eUSB_STDREQ_SET_INTERFACE     = 0x0B,
    eUSB_STDREQ_SYNCH_FRAME       = 0x0C,

};

enum usb_language_id{
    eUSB_LANGID_EN_CA = 0x1009,
    eUSB_LANGID_EN_US = 0x0409
};

/**
 * @class usb_std_dev_desc
 * @brief Standard Device Descriptor
 * Contains general information about a USB device
 * @param bLength            Size of this Descriptor
 * @param bDescriptorType    DEVICE Descriptor Type
 * @param bcdUSB             USB Specification Release Number
 * @param bDeviceClass       Class Code. If set to 0, each interface specifies its own configuration
 * @param bDeviceSubClass    Subclass Code. If bDeviceClass == 0, this must also be 0
 * @param bDeviceProtocol    Protocol Code
 * @param bMaxPacketSize0    Maximum Packet Size for EP0
 * @param idVendor           Product Vendor ID
 * @param idProduct          Product Specific ID
 * @param bcdDevice          Device Release number in BCD format
 * @param iManufacturer      Index of String Descriptor describing product
 * @param iSerialNumber      Index of String Descriptor describing devices serial number
 * @param bNumConfigurations Number of possible device configurations
 */
struct usb_std_dev_desc {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
};

/**
 * @class usb_device_qualifier
 * @brief Describes alternate configurations for different USB speeds
 * 
 * @param bLength            Size of this Descriptor
 * @param bDescriptorType    DEVICE Descriptor Type
 * @param bcdUSB             USB Specification Release Number
 * @param bDeviceClass       Class Code. If set to 0, each interface specifies its own configuration
 * @param bDeviceSubClass    Subclass Code. If bDeviceClass == 0, this must also be 0
 * @param bDeviceProtocol    Protocol Code
 * @param bMaxPacketSize0    Maximum Packet Size for EP0 for other speeds
 * @param bNumConfigurations Number of possible device configurations
 * @param bReserved          Reserved for future use, must be 0
 */
struct usb_device_qualifier {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint8_t  bNumConfigurations;
    uint8_t  bReserved;
};

/**
 * @class usb_config_descriptor
 * @brief Describes information about a specific device configuration. A device can have one or more.
 *  Each configuration has zero or more endpoints.
 *
 * @param bLength             Size of this Descriptor
 * @param bDescriptorType     DEVICE Descriptor Type
 * @param wTotalLength        Toltal Length of data returned for this configuration.
 *                              Includes combined length of all descriptors (configuration,
 *                              interface, endpoint and class/vendor specific)
 * @param bNumInterfaces      Number of interfaces supported by this configuration
 * @param bConfigurationValue Value to use as an argument to request this configuration
 * @param iConfiguration      Index of string descriptor describing this configuration
 * @param bmAttributes        Configuration characteristics
 * @param bMaxPower           Maximum power consumption of device (expressed in 2mA)
 */
struct usb_config_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  bMaxPower;
};

/**
 * @class usb_interface_descriptor
 * @brief Describes a specific interface within a configuration.
 *
 * @param bLength             Size of this Descriptor
 * @param bDescriptorType     INTERFACE Descriptor Type
 * @param bInterfaceNumber    Number of this interface
 * @param bAlternateSetting   Alternate setting number if alternate
 * @param bNumEndpoints       Number of endpoints used by this interface (excluding EP0)
 * @param bInterfaceClass     Class Code
 * @param bInterfaceSubClass  Subclass Code
 * @param bInterfaceProtocol  Protocol Code (Class Specific)
 * @param iInterface          Intex of the string descriptor describing this interface
 */
struct usb_interface_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bInterfaceNumber;
    uint8_t  bAlternateSetting;
    uint8_t  bNumEndpoints;
    uint8_t  bInterfaceClass;
    uint8_t  bInterfaceSubClass;
    uint8_t  bInterfaceProtocol;
    uint8_t  iInterface;
};

/**
 * @class usb_endpoint_descriptor
 * @brief Descriptor for an endpoint in an interface
 * 
 * @param bLength           Size of this Descriptor
 * @param bDescriptorType   ENDPOINT Descriptor Type
 * @param bEndpointAddress  Address of this endpoint on the USB device
 * @param bmAttributes      Transfer Type, Synchronization Type, Usage Type
 * @param wMaxPacketSize    Maximum packet size the EP is capable of
 * @param bInterval         Polling interval in MS
 */
struct usb_endpoint_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
};

/**
 * @class usb_string_descriptor
 * @brief UNICODE String Descriptor
 *
 * @param bLength Size of this Descriptor
 * @param bDescriptorType STRING descriptor type
 * @param wString Either UNICODE string or USB_LANGID codes
 */
struct usb_string_descriptor {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wString[];
};


static uint32_t fnv1a32_turn (uint32_t fnv, uint32_t data ) {
    for (int i = 0; i < 4 ; i++) {
        fnv ^= (data & 0xFF);
        fnv *= 16777619;
        data >>= 8;
    }
    return fnv;
}

static inline void hal_usb_serialNo(struct usb_string_descriptor *dsc){
    uint16_t *str = dsc->wString;
    uint32_t fnv = 2166136261;
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x00));
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x04));
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x08));
    for (int i = 28; i >= 0; i -= 4 ) {
        uint16_t c = (fnv >> i) & 0x0F;
        c += (c < 10) ? '0' : ('A' - 10);
        *str++ = c;
    }
    dsc->bDescriptorType = eUSB_DTYPE_STRING;
    dsc->bLength = 18;
}

#endif

