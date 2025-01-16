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
#include <memory.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/stm32_compat.h"
#include "os/hal/stusb/usb.h"
#include "os/hal/stusb/usb_cdc.h"
#include "usb_tsk.h"
#include "systime.h"

/* Device descriptor */
static const struct usb_device_descriptor device_desc = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DTYPE_DEVICE,
    .bcdUSB             = VERSION_BCD(2,0,0),
    .bDeviceClass       = USB_CLASS_IAD,
    .bDeviceSubClass    = USB_SUBCLASS_IAD,
    .bDeviceProtocol    = USB_PROTO_IAD,
    .bMaxPacketSize0    = CDC_EP0_SIZE,
    .idVendor           = 0xFFFF,
    .idProduct          = 0x1234,
    .bcdDevice          = VERSION_BCD(1,0,0),
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

/* Device configuration descriptor */
static const struct udev_config config_desc = {
    .config = {
        .bLength                = sizeof(struct usb_config_descriptor),
        .bDescriptorType        = USB_DTYPE_CONFIGURATION,
        .wTotalLength           = sizeof(struct udev_config),
        .bNumInterfaces         = UDEV_INTERFACES,
        .bConfigurationValue    = 1,
        .iConfiguration         = NO_DESCRIPTOR,
        .bmAttributes           = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
        .bMaxPower              = USB_CFG_POWER_MA(100),
    },

    .vcom_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface        = 0,
        .bInterfaceCount        = 2,
        .bFunctionClass         = USB_CLASS_CDC,
        .bFunctionSubClass      = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol      = CDC_PROTOCOL,
        .iFunction              = NO_DESCRIPTOR,
    },
    .vcom = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = VCOM_NTF_INUM,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_CDC,
        .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol     = CDC_PROTOCOL,
        .iInterface             = NO_DESCRIPTOR,
    },
    .vcom_hdr = {
        .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
        .bcdCDC                 = VERSION_BCD(1,1,0),
    },
    .vcom_mgmt = {
        .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities         = 0,
        .bDataInterface         = 1,

    },
    .vcom_acm = {
        .bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_ACM,
        .bmCapabilities         = 0,
    },
    .vcom_union = {
        .bFunctionLength        = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_UNION,
        .bMasterInterface0      = 0,
        .bSlaveInterface0       = 1,
    },
    .vcom_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = VCOM_NTF_EP,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = VCOM_NTF_SZ,
        .bInterval              = 0x01,
    },
    .vcom_data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = VCOM_DATA_INUM,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .vcom_eptx= {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = VCOM_TXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = VCOM_DATA_SZ,
        .bInterval              = 0x01,
    },
    .vcom_eprx= {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = VCOM_RXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = VCOM_DATA_SZ,
        .bInterval              = 0x01,
    },

    .ctrl_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface        = CTRL_NTF_INUM,
        .bInterfaceCount        = 2,
        .bFunctionClass         = USB_CLASS_CDC,
        .bFunctionSubClass      = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol      = CDC_PROTOCOL,
        .iFunction              = NO_DESCRIPTOR,
    },
    .ctrl = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = CTRL_NTF_INUM,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 1,
        .bInterfaceClass        = USB_CLASS_CDC,
        .bInterfaceSubClass     = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol     = CDC_PROTOCOL,
        .iInterface             = NO_DESCRIPTOR,
    },
    .ctrl_hdr = {
        .bFunctionLength        = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_HEADER,
        .bcdCDC                 = VERSION_BCD(1,1,0),
    },
    .ctrl_mgmt = {
        .bFunctionLength        = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities         = 0,
        .bDataInterface         = CTRL_DATA_INUM,

    },
    .ctrl_acm = {
        .bFunctionLength        = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_ACM,
        .bmCapabilities         = 0,
    },
    .ctrl_union = {
        .bFunctionLength        = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType        = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType     = USB_DTYPE_CDC_UNION,
        .bMasterInterface0      = CTRL_NTF_INUM,
        .bSlaveInterface0       = CTRL_DATA_INUM,
    },
    .ctrl_ep = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CTRL_NTF_EP,
        .bmAttributes           = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize         = CTRL_NTF_SZ,
        .bInterval              = 0x01,
    },
    .ctrl_data = {
        .bLength                = sizeof(struct usb_interface_descriptor),
        .bDescriptorType        = USB_DTYPE_INTERFACE,
        .bInterfaceNumber       = CTRL_DATA_INUM,
        .bAlternateSetting      = 0,
        .bNumEndpoints          = 2,
        .bInterfaceClass        = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
        .bInterfaceProtocol     = USB_PROTO_NONE,
        .iInterface             = NO_DESCRIPTOR,
    },
    .ctrl_eptx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CTRL_TXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CTRL_DATA_SZ,
        .bInterval              = 0x01,
    },
    .ctrl_eprx = {
        .bLength                = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType        = USB_DTYPE_ENDPOINT,
        .bEndpointAddress       = CTRL_RXD_EP,
        .bmAttributes           = USB_EPTYPE_BULK,
        .wMaxPacketSize         = CTRL_DATA_SZ,
        .bInterval              = 0x01,
    },
};

static const struct usb_string_descriptor lang_desc     = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC("JC Custom USB Device");
static const struct usb_string_descriptor prod_desc_en  = USB_STRING_DESC("QSET ARM Control");
static const struct usb_string_descriptor *const dtable[] = {
    &lang_desc,
    &manuf_desc_en,
    &prod_desc_en,
};

usbd_device udev;

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


uint8_t read_fifo[VCOM_DATA_SZ];

static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
        usbd_ep_read(dev, ep, read_fifo, VCOM_DATA_SZ);
}

struct systime t_last;

static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
    char msg[0xFF];
    // Send the time out every 1 s
    // if(ep == VCOM_TXD_EP){
        struct systime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        systime_fromTicks(xTaskGetTickCountFromISR(), &time);
        // If a new second has elapsed, send the time
        // if(t_last.secs != time.secs){
            int strsize = sprintf(msg, "EP %d: %s", ep &0x0F, time.str);
            // usbd_ep_unstall(&udev, CDC_TXD_EP);
            usbd_ep_write(&udev, ep, msg, strsize);
        // }
        // NAK if a second has not elapsed
        // else{
        //     usbd_ep_write(&udev, ep, NULL, 0);
        // }
        // t_last.secs = time.secs;
    // }
    // else{
    //     usbd_ep_write(&udev, ep, read_fifo, VCOM_DATA_SZ);
    // }
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep) {
    if (event == usbd_evt_eptx) {
        cdc_txonly(dev, event, ep);
    } else {
        cdc_rxonly(dev, event, ep);
    }
}

struct arm_control ctrl_rx = {0}, ctrl_tx = {
    .control_type = 47U,
    .status = 0,
    .ctrl_tool = 0,
    .limit_sw = 55U,
    {0},
    {0},
};
static void ctrl_rxtx(usbd_device *dev, uint8_t event, uint8_t ep){
    if(event == usbd_evt_eprx){
        usbd_ep_read(dev, ep, &ctrl_rx, sizeof(struct arm_control));
    }
    else{
        usbd_ep_write(dev, ep, &ctrl_tx, sizeof(struct arm_control));
    }
}

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
    case 0:
        /* deconfiguring device */
        usbd_ep_deconfig(dev, VCOM_NTF_EP);
        usbd_ep_deconfig(dev, VCOM_TXD_EP);
        usbd_ep_deconfig(dev, VCOM_RXD_EP);
        usbd_ep_deconfig(dev, CTRL_NTF_EP);
        usbd_ep_deconfig(dev, CTRL_TXD_EP);
        usbd_ep_deconfig(dev, CTRL_RXD_EP);
        usbd_reg_endpoint(dev, VCOM_RXD_EP, 0);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, 0);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, 0);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, 0);
        return usbd_ack;
    case 1:
        /* configuring device */
        usbd_ep_config(dev, VCOM_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, VCOM_DATA_SZ);
        usbd_ep_config(dev, VCOM_NTF_EP, USB_EPTYPE_INTERRUPT, VCOM_NTF_SZ);
        usbd_ep_config(dev, CTRL_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CTRL_DATA_SZ);
        usbd_ep_config(dev, CTRL_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CTRL_DATA_SZ);
        usbd_ep_config(dev, CTRL_NTF_EP, USB_EPTYPE_INTERRUPT, CTRL_NTF_SZ);

        usbd_reg_endpoint(dev, VCOM_RXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, VCOM_TXD_EP, cdc_rxtx);
        usbd_reg_endpoint(dev, CTRL_RXD_EP, ctrl_rxtx);
        usbd_reg_endpoint(dev, CTRL_TXD_EP, ctrl_rxtx);

        usbd_ep_write(dev, VCOM_TXD_EP, 0, 0);
        usbd_ep_write(dev, CTRL_TXD_EP, 0, 0);
        return usbd_ack;
    default:
        return usbd_fail;
    }
}

uint32_t ubuf[CDC_EP0_SIZE];

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
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN);
    NVIC_EnableIRQ(OTG_FS_IRQn);
    usbd_enable(&udev, 1);
    usbd_connect(&udev, 1);
    // const char msg[] = "Hello, World!\n";
    const char msg[256] ={0};
    char rx_buf[64];
    vTaskDelete(NULL);
    for(;;){
        // printf("Writing to USB\n");
        // struct ctime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        // int strsize = sprintf(msg, PRINT_CTIME(time));
        // // usbd_ep_unstall(&udev, CDC_TXD_EP);
        // usbd_ep_write(&udev, CDC_TXD_EP, msg, strsize);
        // int32_t rx_len = usbd_ep_read(&udev, CDC_RXD_EP, rx_buf, sizeof(rx_buf));
        // if(rx_len > 0){
        //     usbd_ep_write(&udev, CDC_TXD_EP, rx_buf, rx_len);
        //     printf("USBRX %d bytes: %s\n", rx_len, rx_buf);
        // }
        // vTaskDelay(1000);
    }
}


// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    usbd_poll(&udev);
}

