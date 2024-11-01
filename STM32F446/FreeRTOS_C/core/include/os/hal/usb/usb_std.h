/**
 * @file usb_std.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief USB Standard Definitions
 * @date 2024-10-18
 * @version 0.1
 * 
 */

#ifndef _USB_STD_H_
#define _USB_STD_H_
#include <stm32f4xx.h>
#include <sys/stat.h>

enum USBD_Speed {
    USBD_SPEED_HIGH = 0U,
    USBD_SPEED_FULL = 1U, 
    USBD_SPEED_LOW = 2U
};

typedef struct usbd_ep {
    uint32_t status;
    uint32_t total_len;
    uint32_t rem_len;
    uint32_t maxpacket;
    uint16_t is_used;
    uint16_t bInterval;
} USBD_EP_t;

typedef struct usbd_setup_req {
    uint8_t bmRequest;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} USBD_SetupReq_t;

typedef struct usbd {
    uint8_t id;
    uint32_t dev_config;
    uint32_t dev_default_config;
    uint32_t dev_config_status;
    enum USBD_Speed dev_speed;
    USBD_EP_t ep_in[16];
    USBD_EP_t ep_out[16];
    __IO uint32_t ep0_state;
    uint32_t ep0_data_len;
    __IO uint8_t dev_state;
    __IO uint8_t dev_old_state;
    uint8_t dev_addr;
    uint8_t dev_connection_status;
    uint8_t dev_test_mode;
    uint32_t dev_remote_wake;
    uint8_t ConfIdx;
    USBD_SetupReq_t request;
} USBD_t;

#endif

