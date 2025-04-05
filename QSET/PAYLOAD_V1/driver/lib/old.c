#include <libusb-1.0/libusb.h>
int is_device_connected(libusb_context *ctx) {
    libusb_device **devices;
    ssize_t count;
    int found = 0;

    // Get the list of USB devices
    count = libusb_get_device_list(ctx, &devices);
    if (count < 0) {
        fprintf(stderr, "Error getting device list: %s\n", libusb_error_name(count));
        return -1; // Error
    }

    // Iterate through the list of devices
    for (ssize_t i = 0; i < count; i++) {
        libusb_device *device = devices[i];
        struct libusb_device_descriptor desc;

        // Get the device descriptor
        int ret = libusb_get_device_descriptor(device, &desc);
        if (ret < 0) {
            fprintf(stderr, "Error getting device descriptor: %s\n", libusb_error_name(ret));
            continue;
        }

        // Check if the device matches the vendor and product IDs
        if (desc.idVendor == VENDOR_ID && desc.idProduct == DEVICE_ID) {
            found = 1;
            break; // Device found
        }
    }

    // Free the device list
    libusb_free_device_list(devices, 1);

    return found; // 1 if device is connected, 0 otherwise
}

int dev_init(chassisDev_t *pDev, int inum){
    int res = 0;
    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(pDev->lusb_devHndl, inum) == 1) {
        res = libusb_detach_kernel_driver(pDev->lusb_devHndl, inum);
        if (res != 0) {
            pDev->err = res;
            libusb_close(pDev->lusb_devHndl);
            libusb_exit(pDev->lusb_ctx);
            return 1;
        }
    }

    // Claim the correct interface
    res = libusb_claim_interface(pDev->lusb_devHndl, inum);
    if (res != 0) {
        pDev->err = res;
        libusb_close(pDev->lusb_devHndl);
        libusb_exit(pDev->lusb_ctx);
        return 1;
    }
    return res;
}


int chassisDev_init(chassisDev_t *pDev){
    pDev->lusb_ctx = NULL;
    pDev->lusb_devHndl = NULL;
    memset(&pDev->drvm_sts, 0, sizeof(struct udev_pkt_drvm_sts));
    memset(&pDev->srvo_sts, 0, sizeof(struct udev_pkt_drvm_sts));
    memset(&pDev->sens_sts, 0, sizeof(struct udev_pkt_drvm_sts));
    int res = 0;
    res = libusb_init(&pDev->lusb_ctx);
    if(res != 0){
        pDev->err = res;
        return -1;
    }
    while(!is_device_connected(pDev->lusb_ctx)){
        printf("Device Not Connected\n");
        sleep(1);
    }
    pDev->lusb_devHndl = libusb_open_device_with_vid_pid(pDev->lusb_ctx, VENDOR_ID, DEVICE_ID);
    if(!pDev->lusb_devHndl){
        libusb_exit(pDev->lusb_ctx);
        return -2;
    }

    // Open the device interfaces
    dev_init(pDev, DRVM_DATA_INUM);
    dev_init(pDev, SRVO_DATA_INUM);
    dev_init(pDev, SENS_DATA_INUM);

    return 0;
}

int chassisDev_reconnect(chassisDev_t *pDev) {
    while (1) {
        // Attempt to reconnect
        pDev->lusb_devHndl = libusb_open_device_with_vid_pid(pDev->lusb_ctx, VENDOR_ID, DEVICE_ID);
        if (pDev->lusb_devHndl) {
            // Re-initialize the device
            dev_init(pDev, DRVM_DATA_INUM);
            dev_init(pDev, SRVO_DATA_INUM);
            dev_init(pDev, SENS_DATA_INUM);
            break; // Successfully reconnected
        }
    }
}

int chassisDev_get(chassisDev_t *pDev){
    // Receive data
    int transferred;
    int res = libusb_bulk_transfer(pDev->lusb_devHndl, CTRL_TXD_EP, (unsigned char *)&pDev->pkt_status, sizeof(struct udev_pkt_status), &transferred, 0);
    if (res != 0) {
        pDev->err = res;
        return -1;
    }
    return transferred;
}

int chassisdev_send(chassisDev_t *pDev, struct udev_pkt_ctrl *pPkt){
    int res = 0, transferred;
    // Send data
    res = libusb_bulk_transfer(pDev->lusb_devHndl, CTRL_RXD_EP, (unsigned char *)pPkt, sizeof(struct udev_pkt_ctrl), &transferred, 0);
    if(res != 0) pDev->err = res;
    return res;
}

uint8_t chassisDev_getLS(chassisDev_t *pDev){
    (void)chassisDev_get(pDev);
    return pDev->pkt_status.limit_sw;
}

struct udev_mtr_info *chassisDev_getMtrInfo(chassisDev_t *pDev, enum eArmMotors mtr){
    (void)chassisDev_get(pDev);
    return &pDev->pkt_status.mtr[mtr];
}

struct udev_pkt_status *chassisDev_getStatusPkt(chassisDev_t *pDev){
    (void)chassisDev_get(pDev);
    return &pDev->pkt_status;
}

int chassisDev_setServo(chassisDev_t *pDev, enum eArmServos servo, uint32_t val_us){
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeSrvo;
    ctrl.hdr.ctrl_typ = (uint8_t)servo;
    ctrl.servo_ctrl = val_us;
    return chassisdev_send(pDev, &ctrl);
}

int chassisDev_setMtr(chassisDev_t *pDev, enum eArmMotors mtr, struct udev_mtr_ctrl *pVals){
    if(pVals == NULL) return -1;
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeMtr;
    ctrl.hdr.ctrl_typ = (uint8_t)mtr;
    memcpy(&ctrl.mtr_ctrl, pVals, sizeof(struct udev_mtr_ctrl));
    memcpy(&pDev->pkt_mtr[mtr], pVals, sizeof(struct udev_mtr_ctrl));
    return chassisdev_send(pDev, &ctrl);

}

int chassisDev_setGripper(chassisDev_t *pDev, int8_t val){
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeGrip;
    ctrl.hdr.ctrl_typ = 0;
    ctrl.grip_ctrl = val;
    return chassisdev_send(pDev, &ctrl);
}

int chassisDev_updateMtr(chassisDev_t *pDev, enum eArmMotors mtr, float pos, float vel){
    pDev->pkt_mtr[mtr].position = pos;
    pDev->pkt_mtr[mtr].velocity = vel;

    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeMtr;
    ctrl.hdr.ctrl_typ = (uint8_t)mtr;
    memcpy(&ctrl.mtr_ctrl, &pDev->pkt_mtr[mtr], sizeof(struct udev_mtr_ctrl));
    return chassisdev_send(pDev, &ctrl);
}

const char *chassisDev_getErr(chassisDev_t *pDev){
    return libusb_error_name(pDev->err);
}

int chassisDev_free(chassisDev_t *pDev){
    // Release interface and close device
    libusb_release_interface(pDev->lusb_devHndl, CTRL_DATA_INUM);
    libusb_close(pDev->lusb_devHndl);
    libusb_exit(pDev->lusb_ctx);
    return 0;
}

