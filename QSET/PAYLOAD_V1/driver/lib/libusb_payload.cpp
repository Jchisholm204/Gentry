/**
 * @file libusb_chassis.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Chassis Control Board
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#include "libusb_payload.hpp"
#include <unistd.h>
#include <memory.h>


PayloadControlBoard::PayloadControlBoard(void){
    this->connection_active = false;
    this->lusb_ctx = NULL;
    this->lusb_devHndl = NULL;
    this->status = ePayloadNoConnection;
}

PayloadControlBoard::~PayloadControlBoard(void){
    if(lusb_ctx != 0){
        if(lusb_devHndl != 0){
            libusb_release_interface(this->lusb_devHndl, PYLD_DATA_INUM);
            libusb_close(this->lusb_devHndl);
        }
        libusb_exit(this->lusb_ctx);
    }
    this->connection_active = false;
}

int PayloadControlBoard::connect(bool blocking){
    if(connection_active) return 0;
    int res = 0;
    // Attempt to open context for the first time if it is NULL
    if(!lusb_ctx)
        res = libusb_init(&this->lusb_ctx);
    // Error catch on LUSB Init
    if(res != 0){
        this->lusb_err = res;
        this->status = ePayloadIniFail;
        lusb_ctx = NULL;
        return res;
    }
    // Check if blocking open has been set
    bool dev_connected = false;
    do{
        dev_connected = this->is_dev_connected();
    } while(!dev_connected && blocking);
    // if(blocking){
    //     // Blocking Option - Wait until the device is connected
    //     while(!this->is_dev_connected()){
    //         sleep(1);
    //     }
    // }
    // else{
    //     // Non-Blocking Option - Fail and return error if device is not connected
    //     if(!this->is_dev_connected()){
    //         this->status = ePayloadNoConnection;
    //         return -1;
    //     }
    // }
    if(!lusb_devHndl)
        this->lusb_devHndl = libusb_open_device_with_vid_pid(lusb_ctx, VENDOR_ID, DEVICE_ID);
    if(!lusb_devHndl){
        this->status = ePayloadIniFail;
        // Confirm NULL
        lusb_devHndl = NULL;
        return -2;
    }
    res = dev_init(PYLD_DATA_INUM);
    if(res != 0){
        this->status = ePayloadIniFail;
        return res;
    }
    this->connection_active = true;
    return 0;
}

bool PayloadControlBoard::is_active(bool reconnect, bool blocking){
    // Attempt a reconnect?
    if(!connection_active && lusb_ctx != 0 && lusb_devHndl != 0 && reconnect){
        do{
            this->reconnect();
        } while(blocking && !connection_active);
    }
    return connection_active;
}

ePayloadStatus PayloadControlBoard::get_status(void){
    this->pkt_poll();
    return this->status;
}

ePayloadStatus PayloadControlBoard::get_status(std::string &msg){
    this->get_status();
    msg = std::string(this->pkt_info.status.msg);
    return this->status;
}

struct udev_stepper_info PayloadControlBoard::get_stepperInfo(enum ePayloadStepper n){
    this->pkt_poll();
    if(n >= eN_Steppers) n = eStepper1;
    return this->pkt_info.stepper_info[n];
}

float PayloadControlBoard::get_ADC(void){
    this->pkt_poll();
    return this->pkt_info.adc_info.ADC_13;
}

float PayloadControlBoard::get_intTemp(void){
    this->pkt_poll();
    return this->pkt_info.adc_info.temp_internal;
}

float PayloadControlBoard::get_bmeTemp(void){
    this->pkt_poll();
    return this->pkt_info.bme_info.temp;
}

float PayloadControlBoard::get_bmePressure(void){
    this->pkt_poll();
    return this->pkt_info.bme_info.pressure;
}

float PayloadControlBoard::get_bmeHumidity(void){
    this->pkt_poll();
    return this->pkt_info.bme_info.humidity;
}

float PayloadControlBoard::get_bmeGasRest(void){
    this->pkt_poll();
    return this->pkt_info.bme_info.gas_resistance;
}

int PayloadControlBoard::transmit(struct udev_pkt_ctrl *pkt){
    if(!pkt) return -1;
    if(!connection_active) return -2;
    if(!lusb_devHndl){
        connection_active = false;
        return -3;
    }
    int res = 0, transfered;
    res = libusb_bulk_transfer(lusb_devHndl, PYLD_RXD_EP, (unsigned char*)pkt, sizeof(struct udev_pkt_ctrl), &transfered, 0);
    if(res != 0){
        lusb_err = res;
        return res;
    }
    if(transfered != sizeof(struct udev_pkt_ctrl)){
        return transfered;
    }
    return res;
}

int PayloadControlBoard::pkt_poll(void){
    if(!connection_active) return -1;
    if(!lusb_devHndl){
        connection_active = false;
        return -2;
    }
    int res = 0, transfered;
    res = libusb_bulk_transfer(lusb_devHndl, PYLD_TXD_EP, (unsigned char*)&pkt_info, sizeof(struct udev_pkt_info), &transfered, 0);
    if(res != 0){
        lusb_err = res;
        connection_active = false;
        return res;
    }
    if(transfered != sizeof(struct udev_pkt_ctrl)){
        return transfered;
    }
    return res;

}

bool PayloadControlBoard::is_dev_connected(void){
    if(!lusb_ctx) return false;
    libusb_device **devices;
    ssize_t count;
    bool found = 0;
    count = libusb_get_device_list(lusb_ctx, &devices);
    if(count < 0) return false;
    // Iterate through the list of devices
    for (ssize_t i = 0; i < count; i++) {
        libusb_device *device = devices[i];
        struct libusb_device_descriptor desc;

        // Get the device descriptor
        int ret = libusb_get_device_descriptor(device, &desc);
        if (ret < 0) {
            // fprintf(stderr, "Error getting device descriptor: %s\n", libusb_error_name(ret));
            continue;
        }

        // Check if the device matches the vendor and product IDs
        if (desc.idVendor == VENDOR_ID && desc.idProduct == DEVICE_ID) {
            found = true;
            break; // Device found
        }
    }

    // Free the device list
    libusb_free_device_list(devices, 1);

    return found; // 1 if device is connected, 0 otherwise
}

int PayloadControlBoard::reconnect(void){
    if(!this->is_dev_connected()) return -1;
    this->lusb_devHndl = libusb_open_device_with_vid_pid(lusb_ctx, VENDOR_ID, DEVICE_ID);
    int error = 0;
    if(lusb_devHndl != 0){
        // Re-initialize the device
        if (libusb_kernel_driver_active(lusb_devHndl, PYLD_DATA_INUM) == 1) {
            libusb_detach_kernel_driver(lusb_devHndl, PYLD_DATA_INUM);
        }
        error = libusb_claim_interface(lusb_devHndl, PYLD_DATA_INUM);
        if(error != 0)
            lusb_err = error;
        // Successfully reconnected
        else
            connection_active = true;
    }
    else{
        lusb_err = error;
    }
    return error;
}

int PayloadControlBoard::dev_init(int inum){
    if(!lusb_devHndl) return -1;
    int res;
    if (libusb_kernel_driver_active(this->lusb_devHndl, inum) == 1) {
        res = libusb_detach_kernel_driver(lusb_devHndl, inum);
        if (res != 0) {
            lusb_err = res;
            return 1;
        }
    }

    // Claim the correct interface
    res = libusb_claim_interface(lusb_devHndl, inum);
    if (res != 0) {
        lusb_err = res;
        return 1;
    }
    return res;
}
