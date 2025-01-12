/**
 * @file usb_tsk.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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
#include "os/hal/hal_gpio.h"

#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/usb.h"
#include "os/hal/stusb/usb_cdc.h"
#include "os/hal/stusb/usb_hid.h"
#include "os/hal/stusb/hid_usage_desktop.h"
#include "os/hal/stusb/hid_usage_button.h"

struct ctime {
    int hrs, mins, secs, msecs;
};

static inline void cTimeGet(TickType_t ticks, struct ctime *t){
    if (!t)
        return;
    float tms = ((float)ticks) / ((float)configTICK_RATE_HZ) * 1000.0;
    t->msecs = ((int)tms) % 1000;
    int secs = ((int)(tms + 500) / 1000);
    t->secs = secs % 60;
    int mins = (secs / 60);
    t->mins = mins % 60;
    t->hrs = (mins / 60);
}

#define PRINT_CTIME(ct) "%02d:%02d:%02d.%03d\n", ct.hrs, ct.mins, ct.secs, ct.msecs

#define CDC_EP0_SIZE    0x08
#define CDC_RXD_EP      0x01
#define CDC_TXD_EP      0x81
#define CDC_DATA_SZ     0x40
#define CDC_NTF_EP      0x82
#define CDC_NTF_SZ      0x08

#define CDC_U5_TXEP  0x83
#define CDC_U5_RXEP  0x03
#define CDC_U5_SZ    0x40

// #define CDC_LOOPBACK
// #define ENABLE_HID_COMBO

#define CDC_PROTOCOL USB_PROTO_NONE

/* Declaration of the report descriptor */
struct cdc_config {
    // Configuration Descriptor
    struct usb_config_descriptor        config;
    // Virtual COM port
    // struct usb_interface_descriptor     vcom;
    // struct usb_cdc_header_desc          vcom_hdr;
    // struct usb_cdc_call_mgmt_desc       vcom_mgmt;
    struct usb_interface_descriptor     vcom_data;
    struct usb_endpoint_descriptor      vcom_eprx;
    struct usb_endpoint_descriptor      vcom_eptx;

    // UART 5 COM Port
    // struct usb_interface_descriptor     com;
    // struct usb_cdc_header_desc          com_hdr;
    // struct usb_cdc_call_mgmt_desc       com_mgmt;
    struct usb_interface_descriptor     com_data;
    struct usb_endpoint_descriptor      com_eprx;
    struct usb_endpoint_descriptor      com_eptx;
} __attribute__((packed));


/* Device descriptor */
static const struct usb_device_descriptor device_desc = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DTYPE_DEVICE,
    .bcdUSB             = VERSION_BCD(2,0,0),
    .bDeviceClass       = USB_CLASS_CDC_DATA,
    .bDeviceSubClass    = USB_SUBCLASS_NONE,
    .bDeviceProtocol    = USB_PROTO_NONE,
    .bMaxPacketSize0    = CDC_EP0_SIZE,
    .idVendor           = 0x0483,
    .idProduct          = 0x5740,
    .bcdDevice          = VERSION_BCD(1,0,0),
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

/* Device configuration descriptor */
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
    // Virtual COM PORT
    // .vcom = {
    //     .bLength                = sizeof(struct usb_interface_descriptor),
    //     .bDescriptorType        = USB_DTYPE_INTERFACE,
    //     .bInterfaceNumber       = 0,
    //     .bAlternateSetting      = 0,
    //     .bNumEndpoints          = 2,
    //     .bInterfaceClass        = USB_CLASS_CDC,
    //     .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
    //     .bInterfaceProtocol     = CDC_PROTOCOL,
    //     .iInterface             = NO_DESCRIPTOR,
    // },
    // .vcom_hdr = {
    //     .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
    //     .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
    //     .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
    //     .bcdCDC                 = VERSION_BCD(1,1,0),
    // },
    // .vcom_mgmt = {
    //     .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
    //     .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
    //     .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
    //     .bmCapabilities         = 0,
    //     .bDataInterface         = 1,

    // },
    .vcom_data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = 0,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .vcom_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_RXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },
    .vcom_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_TXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_DATA_SZ,
        .bInterval              = 0x01,
    },

    // UART COM PORT
    // .com= {
    //     .bLength                = sizeof(struct usb_interface_descriptor),
    //     .bDescriptorType        = USB_DTYPE_INTERFACE,
    //     .bInterfaceNumber       = 2,
    //     .bAlternateSetting      = 0,
    //     .bNumEndpoints          = 2,
    //     .bInterfaceClass        = USB_CLASS_CDC_DATA,
    //     .bInterfaceSubClass     = USB_SUBCLASS_NONE,
    //     .bInterfaceProtocol     = USB_PROTO_NONE,
    //     .iInterface             = NO_DESCRIPTOR,
    // },
    // .com_hdr = {
    //     .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
    //     .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
    //     .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
    //     .bcdCDC                 = VERSION_BCD(1,1,0),

    // },
    // .com_mgmt = {
    //     .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
    //     .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
    //     .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
    //     .bmCapabilities         = 0,
    //     .bDataInterface         = 1,
    // },

    .vcom_data = {
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
    .com_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_U5_TXEP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_U5_SZ,
        .bInterval              = 0x01,
    },
    .com_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CDC_U5_RXEP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CDC_U5_SZ,
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

usbd_device udev;
uint32_t	ubuf[0x20];
uint8_t     fifo[0x200];
uint32_t    fpos = 0;

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate          = 38400,
    .bCharFormat        = USB_CDC_1_STOP_BITS,
    .bParityType        = USB_CDC_NO_PARITY,
    .bDataBits          = 8,
};

static usbd_respond cdc_getdesc (usbd_ctlreq *req, void **address, uint16_t *length) {
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


static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback) {
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


static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
   usbd_ep_read(dev, ep, fifo, CDC_DATA_SZ);
}

volatile struct ctime t_last;
static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
    // static uint8_t psize = 0x00U;
    // static uint8_t remained = 0x00U;
    // static uint8_t lastsym = 0x00U;

    // uint8_t _t = (remained < CDC_DATA_SZ) ? remained : CDC_DATA_SZ;
    // // fill buffer by sequental data
    // for (size_t i = 0; i < _t; ++i) {
    //     fifo[i] = lastsym++;
    // }
    // usbd_ep_write(dev, ep, fifo, _t);

    // if (remained < CDC_DATA_SZ) {
    //     // bulk xfer completed. increase bulk size
    //     remained = ++psize;
    // } else {
    //     // continue to send remained data or ZLP
    //     remained -= _t;
    // }
    char msg[0xFF];
    // Send the time out every 1 s
    if(ep == CDC_TXD_EP){
        struct ctime time;
        cTimeGet(xTaskGetTickCount(), &time);
        // If a new second has elapsed, send the time
        if(t_last.secs != time.secs){
            int strsize = sprintf(msg, PRINT_CTIME(time));
            // usbd_ep_unstall(&udev, CDC_TXD_EP);
            usbd_ep_write(&udev, ep, msg, strsize);
        }
        // NAK if a second has not elapsed
        else{
            usbd_ep_write(&udev, ep, NULL, 0);
        }
        t_last.secs = time.secs;
    }
    else{
        int strsize = sprintf(msg, "EP: %d", ep & 0x0F);
        usbd_ep_write(&udev, ep, msg, strsize);
    }
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep) {
    if (event == usbd_evt_eptx) {
        cdc_txonly(dev, event, ep);
    } else {
        cdc_rxonly(dev, event, ep);
    }
}

/* CDC loop callback. Both for the Data IN and Data OUT endpoint */
static void cdc_loopback(usbd_device *dev, uint8_t event, uint8_t ep) {
    int _t;
    if (fpos <= (sizeof(fifo) - CDC_DATA_SZ)) {
        _t = usbd_ep_read(dev, CDC_RXD_EP, &fifo[fpos], CDC_DATA_SZ);
        if (_t > 0) {
            fpos += _t;
        }
    }
    if (fpos > 0) {
        _t = usbd_ep_write(dev, CDC_TXD_EP, &fifo[0], (fpos < CDC_DATA_SZ) ? fpos : CDC_DATA_SZ);
        if (_t > 0) {
            memmove(&fifo[0], &fifo[_t], fpos - _t);
            fpos -= _t;
        }
    }
}

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        // usbd_ep_deconfig(dev, CDC_NTF_EP);
        usbd_ep_deconfig(dev, CDC_TXD_EP);
        usbd_ep_deconfig(dev, CDC_RXD_EP);
        usbd_ep_deconfig(dev, CDC_U5_TXEP);
        usbd_ep_deconfig(dev, CDC_U5_RXEP);
        usbd_reg_endpoint(dev, CDC_RXD_EP, 0);
        usbd_reg_endpoint(dev, CDC_TXD_EP, 0);
        usbd_reg_endpoint(dev, CDC_U5_TXEP, 0);
        usbd_reg_endpoint(dev, CDC_U5_RXEP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
        usbd_ep_config(dev, CDC_U5_RXEP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_U5_SZ);
        usbd_ep_config(dev, CDC_U5_TXEP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_U5_SZ);
        // usbd_ep_config(dev, CDC_NTF_EP, USB_EPTYPE_INTERRUPT, CDC_NTF_SZ);
#if defined(CDC_LOOPBACK)
        // usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_loopback);
        // usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_loopback);
#elif ((CDC_TXD_EP & 0x7F) == (CDC_RXD_EP & 0x7F))
        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, CDC_U5_RXEP, cdc_rxtx);
        usbd_reg_endpoint(dev, CDC_U5_TXEP, cdc_rxtx);
#else
        usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_rxonly);
        usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_txonly);
#endif
        usbd_ep_write(dev, CDC_TXD_EP, 0, 0);
        usbd_ep_write(dev, CDC_U5_TXEP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

static void cdc_init_usbd(void) {
    usbd_init(&udev, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));
    usbd_reg_config(&udev, cdc_setconf);
    usbd_reg_control(&udev, cdc_control);
    usbd_reg_descr(&udev, cdc_getdesc);
}

void usb_cdc_init_rcc(void){
    /* enabling GPIOA and setting PA11 and PA12 to AF10 (USB_FS) */
    #if defined(USBD_PRIMARY_OTGHS)
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    _BST(GPIOB->AFR[1], (0x0C << 24) | (0x0C << 28));
    _BMD(GPIOB->MODER, (0x03 << 28) | (0x03 << 30), (0x02 << 28) | (0x02 << 30));
    #else
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    _BST(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    _BMD(GPIOA->MODER, (0x03 << 22) | (0x03 << 24), (0x02 << 22) | (0x02 << 24));
    #endif
}



void vUSB_tsk(void * pvParams){
    (void)pvParams;
    usb_cdc_init_rcc();
    cdc_init_usbd();
    NVIC_EnableIRQ(OTG_FS_IRQn);
    usbd_enable(&udev, 1);
    usbd_connect(&udev, 1);
    // const char msg[] = "Hello, World!\n";
    const char msg[256] ={0};
    char rx_buf[64];
    for(;;){
        // printf("Writing to USB\n");
        // struct ctime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        // int strsize = sprintf(msg, PRINT_CTIME(time));
        // // usbd_ep_unstall(&udev, CDC_TXD_EP);
        // usbd_ep_write(&udev, CDC_TXD_EP, msg, strsize);
        // int32_t rx_len = usbd_ep_read(&udev, CDC_RXD_EP, rx_buf, sizeof(rx_buf));
        // if(rx_len > 0){
        //     if(usbd_ep_write(&udev, CDC_TXD_EP, rx_buf, rx_len) != 0){
        //         printf("USB ERR\n");
        //         usbd_connect(&udev, 0);
        //         vTaskDelay(10);
        //         usbd_connect(&udev, 1);
        //     }
        //     printf("USBRX %d bytes: %s\n", rx_len, rx_buf);
        // }
        vTaskDelay(1000);
    }
}


// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}
