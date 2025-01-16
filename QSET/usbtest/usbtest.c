#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VENDOR_ID 0xFFFF
#define PRODUCT_ID 0x1234

// Interface and Endpoint Definitions
#define CTRL_DATA_INUM  0x03  // Interface number for data
#define CTRL_RXD_EP     0x03  // Bulk OUT endpoint for sending data
#define CTRL_TXD_EP     0x83  // Bulk IN endpoint for receiving data

struct arm_control {
    uint8_t control_type;
    uint8_t status;
    uint8_t ctrl_tool;
    uint8_t limit_sw;
    uint8_t ctrl_servo[4];
    uint16_t motor_pos[6];
} __attribute__((packed));

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    int transferred;
    int res;

    struct arm_control data_out = {
        .control_type = 1,
        .status = 0,
        .ctrl_tool = 2,
        .limit_sw = 3,
        .ctrl_servo = {10, 20, 30, 40},
        .motor_pos = {100, 200, 300, 400, 500, 600}
    };

    struct arm_control data_in;

    // Initialize libusb
    res = libusb_init(&ctx);
    if (res != 0) {
        fprintf(stderr, "Failed to initialize libusb: %s\n", libusb_error_name(res));
        return 1;
    }

    // Open the USB device
    dev_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
    if (!dev_handle) {
        fprintf(stderr, "Failed to open USB device\n");
        libusb_exit(ctx);
        return 1;
    }

    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(dev_handle, CTRL_DATA_INUM) == 1) {
        res = libusb_detach_kernel_driver(dev_handle, CTRL_DATA_INUM);
        if (res != 0) {
            fprintf(stderr, "Failed to detach kernel driver: %s\n", libusb_error_name(res));
            libusb_close(dev_handle);
            libusb_exit(ctx);
            return 1;
        }
    }

    // Claim the correct interface
    res = libusb_claim_interface(dev_handle, CTRL_DATA_INUM);
    if (res != 0) {
        fprintf(stderr, "Failed to claim interface: %s\n", libusb_error_name(res));
        libusb_close(dev_handle);
        libusb_exit(ctx);
        return 1;
    }

    while(1){

        // Send data
        res = libusb_bulk_transfer(dev_handle, CTRL_RXD_EP, (unsigned char *)&data_out, sizeof(data_out), &transferred, 0);
        if (res != 0) {
            fprintf(stderr, "Failed to send data: %s\n", libusb_error_name(res));
        } else {
            printf("Sent %d bytes\n", transferred);
        }

        // Receive data
        res = libusb_bulk_transfer(dev_handle, CTRL_TXD_EP, (unsigned char *)&data_in, sizeof(data_in), &transferred, 0);
        if (res != 0) {
            fprintf(stderr, "Failed to receive data: %s\n", libusb_error_name(res));
        } else {
            printf("Received %d bytes\n", transferred);
            printf("Data received: control_type=%d, status=%d, ctrl_tool=%d\n",
                    data_in.control_type, data_in.status, data_in.ctrl_tool);
        }
        
        sleep(1);
    }

    // Release interface and close device
    libusb_release_interface(dev_handle, CTRL_DATA_INUM);
    libusb_close(dev_handle);
    libusb_exit(ctx);

    return 0;
}

