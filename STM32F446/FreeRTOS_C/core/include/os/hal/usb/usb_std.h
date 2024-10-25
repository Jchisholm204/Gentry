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

struct usb_header_descriptor{
    uint8_t len, type;
};

struct usb_device_descriptor{

}

#endif

