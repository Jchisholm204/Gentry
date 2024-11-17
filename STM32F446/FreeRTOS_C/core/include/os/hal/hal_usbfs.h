/**
 * @file hal_usbfs.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief STM32 USB FS HAL
 * @date 2024-11-14
 * @version 0.1
 * 
 */

#ifndef _HAL_USBFS_H_
#define _HAL_USBFS_H_
#include "stm32f446xx.h"
#include <stm32f4xx.h>
#include <stdbool.h>

#define MAX_EP          6
#define MAX_RX_PACKET   128
#define MAX_CONTROL_EP  1
#define MAX_FIFO_SZ     320  /*in 32-bit chunks */

#define RX_FIFO_SZ      ((4 * MAX_CONTROL_EP + 6) + ((MAX_RX_PACKET / 4) + 1) + (MAX_EP * 2) + 1)

// Create a reference to the USB OTG Periphrial
#ifndef USB
#define USB USB_OTG_FS
#endif

// Create a reference to the USB Device Specific Registers
#ifndef USBD
#define USBD ((__IO USB_OTG_DeviceTypeDef*)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#endif

// Create a pointer to the USB Power Clock and Gating Control Regiser
#ifndef USB_PCGCCTL
#define USB_PCGCCTL ((__IO uint32_t*)(USB_OTG_FS_PERIPH_BASE+ USB_OTG_PCGCCTL_BASE))
#endif

// Pointer to the USB endpoint FIFOs
#ifndef USB_EP_FIFO
#define USB_EP_FIFO(ep) ((__IO uint32_t*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_FIFO_BASE + (ep*USB_OTG_FIFO_SIZE)))
#endif

#ifndef USB_EP_IN
#define USB_EP_IN(ep) ((__IO USB_OTG_INEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_IN_ENDPOINT_BASE+(ep*sizeof(USB_OTG_INEndpointTypeDef))))
#endif
#ifndef USB_EP_OUT
#define USB_EP_OUT(ep) ((__IO USB_OTG_OUTEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_OUT_ENDPOINT_BASE+(ep*sizeof(USB_OTG_OUTEndpointTypeDef))))
#endif

enum hal_usb_ep {
    eHUSB_EP_IN  = 0x80,
    eHUSB_EP_OUT = 0x00,
};

enum hal_usb_eptype {
    eHUSB_EPTYPE_CONTROL    = 0x0,
    eHUSB_EPTYPE_ISOCHRONUS = 0x1,
    eHUSB_EPTYPE_BULK       = 0x2,
    eHUSB_EPTYPE_INTERRUPT  = 0x3,
};

// HAL USB Status
enum hal_usb_sts {
    eHUSB_OK = 0,
    eHUSB_NULL,
    eHUSB_TIMEOUT,
    eHUSB_NOCLK,
    eHUSB_DISCONNECT,
    eHUSB_EP_STALL,
    eHUSB_EP_FULL,
    eHUSB_EP_NENA,
};

enum hal_usb_phy {eHUSB_PHY_ULPI, eHUSB_PHY_EMBEDDED};

enum hal_usb_DCFG_frame_interval {
    USBD_DCFG_FRAME_INTERVAL_80 = 0U,
    USBD_DCFG_FRAME_INTERVAL_85 = 1U,
    USBD_DCFG_FRAME_INTERVAL_90 = 2U,
    USBD_DCFG_FRAME_INTERVAL_95 = 3U
};

// USB Standard Descriptor Types
enum hal_usb_dtype {
    eHUSB_DTYPE_DEVICE         = 0x01,
    eHUSB_DTYPE_CONFIGURATION  = 0x02,
    eHUSB_DTYPE_STRING         = 0x03,
    eHUSB_DTYPE_INTERFACE      = 0x04,
    eHUSB_DTYPE_ENDPOINT       = 0x05,
    eHUSB_DTYPE_QUALIFIER      = 0x06,
    eHUSB_DTYPE_OTHER          = 0x07,
    eHUSB_DTYPE_INTERFACEPOWER = 0x08,
    eHUSB_DTYPE_OTG            = 0x09,
    eHUSB_DTYPE_DEBUG          = 0x0A,
    eHUSB_DTYPE_INTERFACEASSOC = 0x0B,
    eHUSB_DTYPE_CS_INTERFACE   = 0x24,
    eHUSB_DTYPE_CS_ENDPOINT    = 0x25,
};

enum hal_usb_dclass {
    // Class defined on interface level
    eHUSB_LASS_PER_INTERFACE = 0x00,
    // No subclass defined
    eHUSB_SUBCLASS_NONE       = 0x00,
    // No protocol defined
    eHUSB_PROTO_NONE          = 0x00,
    // Audio Device Class
    eHUSB_LASS_AUDIO         = 0x01,
    // Communications Device Class
    eHUSB_LASS_CDC           = 0x02,
    // Human Input Device
    eHUSB_LASS_HID           = 0x03,
    // Physical Device Class
    eHUSB_LASS_PHYSICAL      = 0x05,
    // Still Imaging Device
    eHUSB_LASS_STILL_IMAGE   = 0x06,
    // Printer Device
    eHUSB_LASS_PRINTER       = 0x07,
    // Mass Storage Device
    eHUSB_LASS_MASS_STORAGE  = 0x08,
    // USB Hub Class
    eHUSB_LASS_HUB           = 0x09,
    // Smart Card Device Class
    eHUSB_LASS_CSCID         = 0x0B,
    // Content Security Device Class
    eHUSB_LASS_CONTENT_SEC   = 0x0D,
    // Video Device Class
    eHUSB_LASS_VIDEO         = 0x0E,
    // Personal Healthcare Device Class
    eHUSB_LASS_HEALTHCARE    = 0x0F,
    // Audio Video Device Class
    eHUSB_LASS_AV            = 0x10,
    // Billboard Device Class
    eHUSB_LASS_BILLBOARD     = 0x11,
    // USB A to C Bridge Class
    eHUSB_LASS_CBRIDGE       = 0x12,
    // Diagnostic Device Class
    eHUSB_LASS_DIAGNOSTIC    = 0xDC,
    // Wireless Controller Device Class
    eHUSB_LASS_WIRELESS      = 0xE0,
    // MISC Device Class
    eHUSB_LASS_MISC          = 0xEF,
    // Application Specific Device Class
    eHUSB_LASS_APP_SPEC      = 0xFE,
    // Vendor Specific Device Class
    eHUSB_LASS_VENDOR        = 0xFF,
    // Vendor Specific Device Subclass
    eHUSB_SUBCLASS_VENDOR     = 0xFF,
    // Vendor Specific Protocol Device Subclass
    eHUSB_PROTO_VENDOR        = 0xFF,
};

/**\brief common USB descriptor header */
struct usb_header_descriptor {
    uint8_t bLength;                /**<\brief Size of the descriptor, in bytes. */
    uint8_t bDescriptorType;        /**<\brief Type of the descriptor. */
} __attribute__((packed));

/**\brief Represents a USB device descriptor
 * \details A device descriptor describes general information about a USB device. It includes
 * information that applies globally to the device and all of the device’s configurations. A USB
 * device has only one device descriptor. A high-speed capable device that has different device
 * information for full-speed and high-speed must also  have a \ref usb_qualifier_descriptor.*/
struct usb_device_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes.*/
    uint8_t  bDescriptorType;       /**<\brief \ref USB_DTYPE_DEVICE Device descriptor.*/
    uint16_t bcdUSB;                /**<\brief BCD of the supported USB specification.*/
    uint8_t  bDeviceClass;          /**<\brief USB device class.*/
    uint8_t  bDeviceSubClass;       /**<\brief USB device subclass.*/
    uint8_t  bDeviceProtocol;       /**<\brief USB device protocol.*/
    uint8_t  bMaxPacketSize0;       /**<\brief Size of the control endpoint's bank in bytes.*/
    uint16_t idVendor;              /**<\brief Vendor ID for the USB product.*/
    uint16_t idProduct;             /**<\brief Unique product ID for the USB product.*/
    uint16_t bcdDevice;             /**<\brief Product release (version) number.*/
    uint8_t  iManufacturer;         /**<\brief String index for the manufacturer's name.*/
    uint8_t  iProduct;              /**<\brief String index for the product name/details.*/
    uint8_t  iSerialNumber;         /**<\brief String index for the product serial number.*/
    uint8_t  bNumConfigurations;    /**<\brief Total number of configurations supported by the device.*/
} __attribute__((packed));

/**\brief USB device qualifier descriptor
 * \details The device_qualifier descriptor describes information about a high-speed capable device
 * that would change if the device were operating at the other speed. For example, if the device is
 * currently operating at full-speed, the device qualifier returns information about how it would
 * operate at high-speed and vice-versa.*/
struct usb_qualifier_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes.*/
    uint8_t  bDescriptorType;       /**<\brief Qualifier descriptor.*/
    uint16_t bcdUSB;                /**<\brief BCD of the supported USB specification.*/
    uint8_t  bDeviceClass;          /**<\brief USB device class.*/
    uint8_t  bDeviceSubClass;       /**<\brief USB device subclass.*/
    uint8_t  bDeviceProtocol;       /**<\brief USB device protocol.*/
    uint8_t  bMaxPacketSize0;       /**<\brief Size of the control endpoint's bank in bytes.*/
    uint8_t  bNumConfigurations;    /**<\brief Total number of configurations supported by the device.*/
    uint8_t  bReserved;             /**<\brief Reserved for future use, must be 0.*/
} __attribute__((packed));

/**\brief USB device configuration descriptor
 * \details The configuration descriptor describes information about a specific device configuration.
 * The descriptor contains a bConfigurationValue field with a value that, when used as a parameter
 * to the SetConfiguration() request, causes the device to assume the described configuration.*/
struct usb_config_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes.*/
    uint8_t  bDescriptorType;       /**<\brief Configuration descriptor.*/
    uint16_t wTotalLength;          /**<\brief Size of the configuration descriptor header, and all
                                     * sub descriptors inside the configuration. */
    uint8_t  bNumInterfaces;        /**<\brief Total number of interfaces in the configuration.*/
    uint8_t  bConfigurationValue;   /**<\brief Configuration index of the current configuration.*/
    uint8_t  iConfiguration;        /**<\brief Index of a string descriptor describing the configuration.*/
    uint8_t  bmAttributes;          /**<\brief Configuration attributes.
                                     * \details Comprised of a mask of \c eHUSB_ONFIG_ATTR_ masks. On
                                     * all devices, this should include eHUSB_ONFIG_ATTR_RESERVED at
                                     * a minimum. */
    uint8_t  bMaxPower;             /**<\brief Maximum power consumption of the device.
                                     * \ref eHUSB_FG_POWER_MA() macro.*/
} __attribute__((packed));

/**\brief USB interface descriptor
 * \details The interface descriptor describes a specific interface within a configuration. A
 *configuration provides one or more interfaces, each with zero or more endpoint descriptors
 * describing a unique set of endpoints within the configuration.*/
struct usb_interface_descriptor {
    uint8_t bLength;                /**<\brief Size of the descriptor, in bytes.*/
    uint8_t bDescriptorType;        /**<\brief Interface descriptor.*/
    uint8_t bInterfaceNumber;       /**<\brief Index of the interface in the current configuration.*/
    uint8_t bAlternateSetting;      /**<\brief Alternate setting for the interface number.*/
    uint8_t bNumEndpoints;          /**<\brief Total number of endpoints in the interface.*/
    uint8_t bInterfaceClass;        /**<\brief Interface class ID.*/
    uint8_t bInterfaceSubClass;     /**<\brief Interface subclass ID.*/
    uint8_t bInterfaceProtocol;     /**<\brief Interface protocol ID. */
    uint8_t iInterface;             /**<\brief Index of the string descriptor describing the interface. */
} __attribute__((packed));

/**\brief USB interface association descriptor
 * \details USB interface association descriptor (IAD) allows the device to group interfaces that
 * belong to a function.*/
struct usb_iad_descriptor {
    uint8_t bLength;                /**<\brief Size of the descriptor, in bytes.*/
    uint8_t bDescriptorType;        /**<\brief IAD descriptor */
    uint8_t bFirstInterface;        /**<\brief Index of the first associated interface. */
    uint8_t bInterfaceCount;        /**<\brief Total number of associated interfaces. */
    uint8_t bFunctionClass;         /**<\brief Function class ID. */
    uint8_t bFunctionSubClass;      /**<\brief Function subclass ID. */
    uint8_t bFunctionProtocol;      /**<\brief Function protocol ID. */
    uint8_t iFunction;              /**<\brief Index of the string descriptor describing the
                                     * interface association. */
} __attribute__((packed));

/**\brief USB endpoint descriptor
 * \details This descriptor contains the information required by the host to determine the bandwidth
 * requirements of each endpoint.*/
struct usb_endpoint_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes. */
    uint8_t  bDescriptorType;       /**<\brief Endpoint descriptor.*/
    uint8_t  bEndpointAddress;      /**<\brief Logical address of the endpoint within the device for
                                     * the current configuration, including direction mask. */
    uint8_t  bmAttributes;          /**<\brief Endpoint attributes, \ref USB_ENDPOINT_DEF. */
    uint16_t wMaxPacketSize;        /**<\brief Size of the endpoint bank, in bytes. This indicates the
                                     * maximum packet size that the endpoint can receive at a time. */
    uint8_t  bInterval;             /**<\brief Polling interval in milliseconds for the endpoint if
                                     * it is an INTERRUPT or ISOCHRONOUS type.*/
} __attribute__((packed));

/**\brief USB string descriptor
 * \details String descriptors are referenced by their one-based index number. A string descriptor
 * contains one or more not NULL-terminated Unicode strings.
 * \note String descriptors are optional. if a device does not support string descriptors, all
 * references to string descriptors within device, configuration, and interface descriptors must be
 * reset to zero.*/
struct usb_string_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes.*/
    uint8_t  bDescriptorType;       /**<\brief String descriptor type.*/
    uint16_t wString[126];             /**<\brief String data, as unicode characters or array of
                                     * \ref USB_STD_LANGID codes. */
} __attribute__((packed, aligned(2)));

/**\brief USB debug descriptor
 * \details This descriptor is used to describe certain characteristics of the device that the host
 * debug port driver needs to know to communicate with the device. Specifically, the debug descriptor
 * lists the addresses of the endpoints that comprise the Debug Pipe. The endpoints are identified by
 * endpoint number.*/
struct usb_debug_descriptor {
    uint8_t  bLength;               /**<\brief Size of the descriptor, in bytes.*/
    uint8_t  bDescriptorType;       /**<\brief Debug descriptor type.*/
    uint8_t  bDebugInEndpoint;      /**<\brief Endpoint number of the Debug Data IN endpoint.*/
    uint8_t  bDebugOutEndpoint;     /**<\brief Endpoint number of the Debug Data OUTendpoint.*/
} __attribute__((packed));

static inline enum hal_usb_sts hal_usb_ahbIdl(void){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_flushTxFifo(uint32_t num){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    hal_usb_ahbIdl();
    // Flush the TX Fifo corrisponding to the right EP
    SET_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH | (num << USB_OTG_GRSTCTL_TXFNUM_Pos));

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_flushRxFifo(void){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    hal_usb_ahbIdl();
    // Flush the RX Fifo corrisponding to the right EP
    USB->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_getInfo(void){
    if(!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN)) return eHUSB_NOCLK;
    if(READ_BIT(USBD->DCFG, USB_OTG_DCTL_SDIS)) return eHUSB_DISCONNECT;
    return eHUSB_OK;
}


static inline enum hal_usb_sts hal_usb_EP_setStall(uint8_t ep, bool stall){
    if((ep & eHUSB_EP_IN) == eHUSB_EP_IN){
        ep &= (eHUSB_EP_IN-1);
        uint32_t ep_ctl = USB_EP_IN(ep)->DIEPCTL;
        if((ep_ctl & USB_OTG_DIEPCTL_USBAEP) == USB_OTG_DIEPCTL_USBAEP){
            if(stall)
                // Set the Stall Bit
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
            else {
                // Clear the Stall Bit and the NAK bit
                CLEAR_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_SD0PID_SEVNFRM | USB_OTG_DIEPCTL_CNAK);
            }
            USB_EP_IN(ep)->DIEPCTL = ep_ctl;
        }
    }
    else {
        uint32_t ep_ctl = USB_EP_OUT(ep)->DOEPCTL;
        if((ep_ctl & USB_OTG_DIEPCTL_USBAEP) == USB_OTG_DIEPCTL_USBAEP){
            if(stall)
                // Set the Stall Bit
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
            else {
                // Clear the Stall Bit and the NAK bit
                CLEAR_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_SD0PID_SEVNFRM | USB_OTG_DIEPCTL_CNAK);
            }
            USB_EP_OUT(ep)->DOEPCTL = ep_ctl;
        }

    }
}

static inline enum hal_usb_sts hal_usb_EP_isStalled(uint8_t ep){
    if((ep & eHUSB_EP_IN) == eHUSB_EP_IN){
        ep &= (eHUSB_EP_IN-1);
        return (READ_BIT(USB_EP_IN(ep)->DIEPCTL, USB_OTG_DIEPCTL_STALL) ? eHUSB_EP_STALL : eHUSB_OK);
    }
    return (READ_BIT(USB_EP_OUT(ep)->DOEPCTL, USB_OTG_DIEPCTL_STALL) ? eHUSB_EP_STALL : eHUSB_OK);
}

static inline enum hal_usb_sts hal_usb_init(bool vbus_detect){
    // Enable the USB CLock
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    if(hal_usb_ahbIdl() != eHUSB_OK) return eHUSB_TIMEOUT;
    // Force device mode, Ensure use of internal PHY
    SET_BIT(USB->GRSTCTL, USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL);
    // Set Turnaround time to reflect higest frequency used on AHB bus
    MODIFY_REG(USB->GRSTCTL, USB_OTG_GUSBCFG_TRDT, 0x6 << USB_OTG_GUSBCFG_TRDT_Pos);
    if(vbus_detect){
        // Enable VBUS detection and enable the USB FS PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_VBDEN | USB_OTG_GCCFG_PWRDWN);
    }
    else {
        // Enable VBUS Override and Set the BValid to 1
        SET_BIT(USB->GOTGCTL, USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL);
        // Enable the USB FS PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN);
    }
    // Restart the USB PHY
    *USB_PCGCCTL = 0;
    // Soft Disconnect Device
    SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    // Setup Periodic Schedule Interval (25%)
    CLEAR_BIT(USBD->DCFG, USB_OTG_DCFG_PERSCHIVL);
    // Set USBD to use internal FS PHY 
    MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DSPD, USB_OTG_DCFG_DSPD);

    // Setup Interrupts
    // Unmask EP Interrupts
    SET_BIT(USBD->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
    // Unmask Core Interrupts
    SET_BIT(USB->GINTMSK,
            // Reset Events
            USB_OTG_GINTMSK_USBRST   |
            // USB Enumeration Done Event
            USB_OTG_GINTMSK_ENUMDNEM |
            // Start of frame
            USB_OTG_GINTMSK_SOFM     |
            // Bus suspended
            USB_OTG_GINTMSK_USBSUSPM |
            // Remote wakeup events
            USB_OTG_GINTMSK_WUIM     |
            // Enable IN endpoint interrupts
            USB_OTG_GINTMSK_IEPINT   |
            // RX Fifo non empty
            USB_OTG_GINTMSK_RXFLVLM
            );
    // Clear all pending interrupts
    USB->GINTSTS = 0xFFFFFFFF;
    // Unmask global interrupt bit
    USB->GAHBCFG = USB_OTG_GAHBCFG_GINT;
    // Set max RX FIFO size
    USB->GRXFSIZ = RX_FIFO_SZ;
    // setting up EP0 TX FIFO SZ as 64 byte
    USB->DIEPTXF0_HNPTXFSIZ = RX_FIFO_SZ | (0x10 << USB_OTG_DIEPTXF_INEPTXFD_Pos);
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_connect(bool connect){
    if(connect)
        CLEAR_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    else
        SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_setAddress(uint8_t addr){
    // Set Device Address
    MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DAD, (uint32_t)(addr << USB_OTG_DCFG_DAD_Pos));
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_setTXFIFO(uint8_t ep, uint16_t ep_size){
    uint32_t _fsa = USB->DIEPTXF0_HNPTXFSIZ;
    /* calculating initial TX FIFO address. next from EP0 TX fifo */
    _fsa = 0xFFFF & (_fsa + (_fsa >> 16));
    /* looking for next free TX fifo address */
    for (int i = 0; i < (MAX_EP - 1); i++) {
        uint32_t _t = USB->DIEPTXF[i];
        if ((_t & 0xFFFF) < 0x200) {
            _t = 0xFFFF & (_t + (_t >> 16));
            if (_t > _fsa) {
                _fsa = _t;
            }
        }
    }
    /* calculating requited TX fifo size */
    /* getting in 32 bit terms */
    ep_size = (ep_size + 0x03) >> 2;
    /* it must be 16 32-bit words minimum */
    if (ep_size < 0x10) ep_size = 0x10;
    /* checking for the available fifo */
    if ((_fsa + ep_size) > MAX_FIFO_SZ) return false;
    /* programming fifo register */
    _fsa |= (uint32_t)(ep_size << 16U);
    USB->DIEPTXF[ep - 1] = _fsa;
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_EP0_config(uint16_t ep_size){
    /* configureing control endpoint EP0 */
    uint32_t mp_size;
    if (ep_size <= 0x08) {
        ep_size = 0x08;
        mp_size = 0x03;
    } else if (ep_size <= 0x10) {
        ep_size = 0x10;
        mp_size = 0x02;
    } else if (ep_size <= 0x20) {
        ep_size = 0x20;
        mp_size = 0x01;
    } else {
        ep_size = 0x40;
        mp_size = 0x00;
    }
    /* EP0 TX FIFO size is setted on init level */
    /* enabling RX and TX interrupts from EP0 */
    USBD->DAINTMSK |= 0x00010001;
    /* setting up EP0 TX and RX registers */
    /*EPIN(ep)->DIEPTSIZ  = epsize;*/
    USB_EP_IN(0)->DIEPCTL = mp_size | USB_OTG_DIEPCTL_SNAK;
    /* 1 setup packet, 1 packets total */
    USB_EP_OUT(0)->DOEPTSIZ = ep_size | USB_OTG_DOEPTSIZ_STUPCNT_0 | (1 << USB_OTG_DOEPTSIZ_PKTCNT_Pos);
    USB_EP_OUT(0)->DOEPCTL = mp_size | USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK;
    return eHUSB_OK;

}

static inline enum hal_usb_sts hal_usb_EPIN_config(uint8_t epn, enum hal_usb_eptype ep_type, uint16_t ep_size){
    enum hal_usb_sts status = eHUSB_OK;
    epn &= (eHUSB_EP_IN-1);
    __IO USB_OTG_INEndpointTypeDef *ep = USB_EP_IN(epn);

    // Set up TX fifo and size register
    if(ep_type == eHUSB_EPTYPE_ISOCHRONUS || ep_type == eHUSB_EPTYPE_BULK)
        status = hal_usb_setTXFIFO(epn, (uint16_t)(ep_size << 1));
    else
        status = hal_usb_setTXFIFO(epn, (uint16_t)(ep_size));
    if(status != eHUSB_OK) return status;

    // Enable the EP TX Interrupt
    SET_BIT(USBD->DAINTMSK, (0x1UL << epn));

    // Clear any previous Configuration settings
    CLEAR_REG(ep->DIEPCTL);
    // Set up the endpoint types
    switch(ep_type){
        case eHUSB_EPTYPE_ISOCHRONUS:
            SET_BIT(ep->DIEPCTL,
                    // Endpoint Enable
                    USB_OTG_DIEPCTL_EPENA                         |
                    // Clear endoint NAK
                    USB_OTG_DIEPCTL_CNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
        case eHUSB_EPTYPE_BULK:
            SET_BIT(ep->DIEPCTL,
                    // Set endoint NAK (No data ready)
                    USB_OTG_DIEPCTL_SNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
        case eHUSB_EPTYPE_INTERRUPT:
        case eHUSB_EPTYPE_CONTROL:
        default:
            SET_BIT(ep->DIEPCTL,
                    // Set endoint NAK (No data ready)
                    USB_OTG_DIEPCTL_SNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
    }
    return status;
}

static inline enum hal_usb_sts hal_usb_EPOUT_config(uint8_t epn, enum hal_usb_eptype ep_type, uint16_t ep_size){
    __IO USB_OTG_OUTEndpointTypeDef *ep = USB_EP_OUT(epn);
    // Clear any previous Configuration settings
    CLEAR_REG(ep->DOEPCTL);
    // Set up the endpoint type
    SET_BIT(ep->DOEPCTL,
            // Endpoint Enable
            USB_OTG_DIEPCTL_EPENA                         |
            // Clear endoint NAK
            USB_OTG_DIEPCTL_CNAK                          |
            // Set endpoint transfer mode
            (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
            // Set the endpoint as active
            USB_OTG_DIEPCTL_USBAEP                        |
            // Set Even frame
            USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
            (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
           );
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_EP_config(uint8_t epn, enum hal_usb_eptype eptype, uint16_t epsize) {
    if (epn == 0) return hal_usb_EP0_config(epsize);
    if (epn & 0x80) return hal_usb_EPIN_config(epn, eptype, epsize);
    return hal_usb_EPOUT_config(epn, eptype, epsize);
}

static inline enum hal_usb_sts hal_usb_EP_deconfig(uint8_t epn){
    epn &= (eHUSB_EP_IN-1);
    __IO USB_OTG_INEndpointTypeDef *epi = USB_EP_IN(epn);
    __IO USB_OTG_OUTEndpointTypeDef *epo = USB_EP_OUT(epn);

    // Clear the EP IN interrupt
    CLEAR_BIT(USBD->DAINTMSK, 0x1UL << epn);
    // Deactivate the IN endpoint
    CLEAR_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_USBAEP);
    // Flush the FIFO
    hal_usb_flushTxFifo(epn);
    // Do not disable EP0 or previously disabled EPs
    if(READ_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA) && (epn!=0)){
        // Set the endpoint to disabled
        epi->DIEPCTL = USB_OTG_DIEPCTL_EPDIS;
    }
    // Clear any pending interrupts
    epi->DIEPINT = 0xFF;
    if(epn > 0){
        // Reset the FIFO size register
        USB->DIEPTXF[epn-1] = 0x02000200 + 0x200 * epn;
    }
    
    // Deactivate the OUT endpoint
    CLEAR_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_USBAEP);
    if(READ_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_EPENA) && (epn!=0)){
        // Set the endpoint to disabled
        epo->DOEPCTL = USB_OTG_DOEPCTL_EPDIS;
    }
    // Clear any pending interrupts
    epo->DOEPINT = 0xFF;
    return eHUSB_OK;
}

static inline int32_t hal_usb_EP_read(uint8_t epn, void *buf, uint16_t blen){
    if(!buf) return -1;
    epn &= (eHUSB_EP_IN-1);
    __IO uint32_t *FIFO = USB_EP_FIFO(0);
    __IO USB_OTG_OUTEndpointTypeDef *epo = USB_EP_OUT(epn);
    // Check that there is data waiting in one of the TX FIFOs
    if(!READ_BIT(USB->GINTSTS, USB_OTG_GINTSTS_RXFLVL)) return -1;
    // Check that the packet belongs to the current EP
    if(READ_BIT(USB->GRXSTSR, USB_OTG_GRXSTSP_EPNUM) != epn) return -1;
    uint16_t pktlen = (USB->GRXSTSP & USB_OTG_GRXSTSP_BCNT_Msk) >> USB_OTG_GRXSTSP_BCNT_Pos;
    uint32_t tmp = 0;
    // Read from FIFO in words, write to buffer in bytes
    for(uint16_t i = 0; i < pktlen; i++){
        if((i & 0x03U) == 0x00)
            tmp = *FIFO;
        if(i < blen){
            ((uint8_t*)buf)[i] = tmp &0xFF;
            tmp = tmp >> 8;
        }
    }
    // ACK the packet and reset the EP enable bit (core will clear it after RX)
    SET_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
    return (pktlen < blen) ? pktlen : blen;
}

static inline enum hal_usb_sts hal_usb_EP_write(uint8_t epn, const void *buf, uint16_t blen){
    if(!buf) return eHUSB_NULL;
    epn &= (eHUSB_EP_IN-1);
    __IO uint32_t *FIFO = USB_EP_FIFO(epn);
    __IO USB_OTG_INEndpointTypeDef *epi = USB_EP_IN(epn);
    // Transfer data in 32 bit words
    uint16_t len = (blen + 3) >> 2;
    // Check for space in the TX FIFO
    if(len > epi->DTXFSTS) return eHUSB_EP_FULL;
    if(epn != 0 && READ_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA)) return eHUSB_EP_NENA;
    // Clear the IN EP transfer register
    CLEAR_REG(epi->DIEPTSIZ);
    // Set the IN EP transfer reg to 1 packet | size
    epi->DIEPTSIZ = (1 << USB_OTG_DIEPTSIZ_PKTCNT_Pos) | blen;
    CLEAR_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_STALL);
    SET_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK);
    uint32_t tmp = 0;
    for(uint16_t i = 0; i < blen; i++){
        tmp |= (uint32_t)((const uint8_t*)buf)[i] << ((i & 0x03U) << 3);
        if((i & 0x03U) == 0x03U || (i+1)==blen){
            *FIFO = tmp;
            tmp = 0;
        }
    }
    return eHUSB_OK;
}

static inline uint16_t hal_usb_getFrame(void){
    return (USBD->DSTS & USB_OTG_DSTS_FNSOF) >> USB_OTG_DSTS_FNSOF_Pos;
}

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
    dsc->bDescriptorType = eHUSB_DTYPE_STRING;
    dsc->bLength = 18;
}

#endif

