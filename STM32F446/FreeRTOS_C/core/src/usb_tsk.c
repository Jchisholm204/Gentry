#include "main.h"
#include <stdio.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "os/config/FreeRTOSConfig.h"
#include "task.h"
#include "os/drivers/serial.h"
#include "string.h"
#include "os/hal/hal_clock.h"
#include "os/hal/hal_gpio.h"
#include "os/config/pin_cfg.h"
#include "os/drivers/canbus.h"
#include "os/hal/usb/hal_usbd_otgfs.h"
#include "os/hal/usb/usb_std.h"
#include "os/hal/usb/usb_cdc.h"

#define CDC_EP0_SIZE 0x08

static const struct usb_device_descriptor usb_descriptor = {
    .bLength = sizeof(struct usb_device_descriptor),
    .bDescriptorType = USB_DTYPE_DEVICE,
    .bcdUSB = VERSION_BCD(2,0,0),
    .bDeviceClass = USB_CLASS_IAD,
    .bDeviceSubClass = USB_SUBCLASS_IAD,
    .bDeviceProtocol = USB_PROTO_IAD,
    .bMaxPacketSize0 = CDC_EP0_SIZE,
    .idVendor = 0x0483,
    .idProduct = 0x5740,
    .bcdDevice = VERSION_BCD(1,0,0),
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber =INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

/* Declaration of the report descriptor */
struct cdc_config {
    struct usb_config_descriptor        config;
    struct usb_iad_descriptor           comm_iad;
    struct usb_interface_descriptor     comm;
    struct usb_cdc_header_desc          cdc_hdr;
    struct usb_cdc_call_mgmt_desc       cdc_mgmt;
    struct usb_cdc_acm_desc             cdc_acm;
    struct usb_cdc_union_desc           cdc_union;
    struct usb_endpoint_descriptor      comm_ep;
    struct usb_interface_descriptor     data;
    struct usb_endpoint_descriptor      data_eprx;
    struct usb_endpoint_descriptor      data_eptx;
} __attribute__((packed));

static const struct cdc_config config_desc = {
    .config = {
        .bLength                = sizeof(struct usb_config_descriptor),
        .bDescriptorType        = USB_DTYPE_CONFIGURATION,
        .wTotalLength           = sizeof(struct cdc_config),
        .bNumInterfaces         = 2,
        .bConfigurationValue    = 1,
        .iConfiguration         = NO_DESCRIPTOR,
        .bmAttributes           = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
        .bMaxPower              = USB_CFG_POWER_MA(100),
    },
    .comm_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface        = 0,
        .bInterfaceCount        = 2,
        .bFunctionClass         = USB_CLASS_CDC,
        .bFunctionSubClass      = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol      = USB_PROTO_NONE,
        .iFunction              = NO_DESCRIPTOR,
    },
    .comm = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 0,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_CDC,
        .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .cdc_hdr = {
        .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
        .bcdCDC                 = VERSION_BCD(1,1,0),
    },
    .cdc_mgmt = {
        .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities         = 0,
        .bDataInterface         = 1,

    },
    .cdc_acm = {
        .bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_ACM,
        .bmCapabilities         = 0,
    },
    .cdc_union = {
        .bFunctionLength        = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_UNION,
        .bMasterInterface0      = 0,
        .bSlaveInterface0       = 1,
    },
    .comm_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = 0x82,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = 0x08,
        .bInterval              = 0xFF,
    },
    .data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 1,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .data_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = 0x01,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = 0x40,
        .bInterval              = 0x01,
    },
    .data_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = 0x81,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = 0x40,
        .bInterval              = 0x01,
    },
};

static const struct usb_string_descriptor lang_desc     = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("Open source USB stack for STM32");
static const struct usb_string_descriptor prod_desc_en  = USB_STRING_DESC("CDC Loopback demo");
static const struct usb_string_descriptor *const dtable[] = {
    &lang_desc,
    &manuf_desc_en,
    &prod_desc_en,
};

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate          = 38400,
    .bCharFormat        = USB_CDC_1_STOP_BITS,
    .bParityType        = USB_CDC_NO_PARITY,
    .bDataBits          = 8,
};


void vUSB_tsk(void * pvParams){
    (void)pvParams;

    // Setup USB GPIO
    GPIOA->MODER &= ~(0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DM)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->MODER &= (uint32_t)~(uint32_t)((uint32_t)0x3UL << (uint32_t)(PINNO(PIN_USB_DP)*2UL));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DP)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DP)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DP) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DP) & 7)*4));

    hal_usb_init(false);
    hal_usb_connect(true);


    // for(;;){
    //     // if(reset_detected)
    //     //     printf("USB reset detected\n");
    //     // else
    //     //     USB_RemoteWake();
    //     // if(reset_ended)
    //     //     printf("USB reset finished\n");
    //     vTaskDelay(10);
    // }
    // Main USB task delay to allow for USB processing
    vTaskDelay(100);
    printf("Deleting USB task\n");
    vTaskDelete(NULL);
}


// Interrupt handler for USB OTG FS
// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    uint32_t gintsts = USB_OTG_FS->GINTSTS;

    // Check for USB reset interrupt (used when the USB host resets the device)
    if (gintsts & USB_OTG_GINTSTS_USBRST) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_USBRST;  // Clear interrupt flag
    }

    // Handle enumeration done (when device enumeration is complete)
    if (gintsts & USB_OTG_GINTSTS_ENUMDNE) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;  // Clear interrupt flag
    }

    // Handle RX FIFO level (data received on control endpoint)
    if (gintsts & USB_OTG_GINTSTS_RXFLVL) {
        // Add RX FIFO processing code here...
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_RXFLVL;   // Clear interrupt flag
    }

    if (gintsts & USB_OTG_GINTSTS_NPTXFE) {
        // Add RX FIFO processing code here...
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_NPTXFE;   // Clear interrupt flag
    }

    // Start of Frame interrupt
    if (gintsts & USB_OTG_GINTSTS_SOF) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_SOF;      // Clear interrupt flag
    }

    // Handle Global IN NAK Effective interrupt
    if (gintsts & USB_OTG_GINTSTS_GINAKEFF) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_GINAKEFF; // Clear interrupt flag
    }

    // Handle other interrupt flag (unknown, 0x10)
    if (gintsts & 0x10) {
        USB_OTG_FS->GINTSTS = 0x10;                    // Clear interrupt flag
    }

    // Handle OTG interrupt
    if (gintsts & USB_OTG_GINTSTS_OTGINT) {
        uint32_t goint = USB_OTG_FS->GOTGINT;
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_OTGINT;   // Clear interrupt flag
    }

    // Clear other necessary interrupt flags if needed
}
