/**
 * @file libusb_payload.hpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Chassis
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#ifndef _LIBUSB_PAYLOAD_H_
#define _LIBUSB_PAYLOAD_H_

extern "C"{
#include "usb_payload_defs.h"
#include "usb_dev.h"
#include "usb_packet.h"
#include <libusb-1.0/libusb.h>
}


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class PayloadControlBoard {
    public:
        PayloadControlBoard(void);
        ~PayloadControlBoard(void);
        int connect(bool blocking = false);
        bool is_active(bool reconnect = true, bool blocking = true);
        ePayloadStatus get_status(void);
        ePayloadStatus get_status(std::string &msg);
        struct udev_stepper_info get_stepperInfo(enum ePayloadStepper n = eStepper1);
        float get_ADC(void);
        float get_intTemp(void);
        float get_bmeTemp(void);
        float get_bmePressure(void);
        float get_bmeHumidity(void);
        float get_bmeGasRest(void);
        int lusb_err;
    private:
        int transmit(struct udev_pkt_ctrl *pkt);
        int pkt_poll(void);
        bool is_dev_connected(void);
        int reconnect(void);
        bool connection_active;
        int dev_init(int inum);
        struct udev_pkt_info pkt_info;
        libusb_context *lusb_ctx;
        libusb_device_handle *lusb_devHndl; 
        enum ePayloadStatus status;
};


#endif
