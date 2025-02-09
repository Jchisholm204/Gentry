/**
 * @file libusb_arm.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Interface Library for QSET Arm
 * @version 0.1
 * @date 2025-01-25
 * 
 * @copyright Copyright (c) 2023
 *
 *
 */
#include "libusb_arm.h"
#include <memory.h>

int armDev_init(armDev_t *pDev){
    pDev->lusb_ctx = NULL;
    pDev->lusb_devHndl = NULL;
    memset(&pDev->pkt_status, 0, sizeof(struct udev_pkt_status));
    memset(&pDev->pkt_mtr, 0, sizeof(struct udev_mtr_ctrl)*ARM_N_MOTORS);
    int res = 0;
    res = libusb_init(&pDev->lusb_ctx);
    if(res != 0){
        pDev->err = res;
        return -1;
    }
    pDev->lusb_devHndl = libusb_open_device_with_vid_pid(pDev->lusb_ctx, VENDOR_ID, DEVICE_ID);
    if(!pDev->lusb_devHndl){
        libusb_exit(pDev->lusb_ctx);
        return -2;
    }

    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(pDev->lusb_devHndl, CTRL_DATA_INUM) == 1) {
        res = libusb_detach_kernel_driver(pDev->lusb_devHndl, CTRL_DATA_INUM);
        if (res != 0) {
            pDev->err = res;
            libusb_close(pDev->lusb_devHndl);
            libusb_exit(pDev->lusb_ctx);
            return 1;
        }
    }

    // Claim the correct interface
    res = libusb_claim_interface(pDev->lusb_devHndl, CTRL_DATA_INUM);
    if (res != 0) {
        pDev->err = res;
        libusb_close(pDev->lusb_devHndl);
        libusb_exit(pDev->lusb_ctx);
        return 1;
    }
    return 0;
}

int armDev_reconnect(armDev_t *pDev) {
    while (1) {
        // Attempt to reconnect
        pDev->lusb_devHndl = libusb_open_device_with_vid_pid(pDev->lusb_ctx, VENDOR_ID, DEVICE_ID);
        if (pDev->lusb_devHndl) {
            // Re-initialize the device
            if (libusb_kernel_driver_active(pDev->lusb_devHndl, CTRL_DATA_INUM) == 1) {
                libusb_detach_kernel_driver(pDev->lusb_devHndl, CTRL_DATA_INUM);
            }
            pDev->err = libusb_claim_interface(pDev->lusb_devHndl, CTRL_DATA_INUM);
            break; // Successfully reconnected
        }
    }
}

int armDev_get(armDev_t *pDev){
    // Receive data
    int transferred;
    int res = libusb_bulk_transfer(pDev->lusb_devHndl, CTRL_TXD_EP, (unsigned char *)&pDev->pkt_status, sizeof(struct udev_pkt_status), &transferred, 0);
    if (res != 0) {
        pDev->err = res;
        return -1;
    }
    return transferred;
}

int armdev_send(armDev_t *pDev, struct udev_pkt_ctrl *pPkt){
    int res = 0, transferred;
    // Send data
    res = libusb_bulk_transfer(pDev->lusb_devHndl, CTRL_RXD_EP, (unsigned char *)pPkt, sizeof(struct udev_pkt_ctrl), &transferred, 0);
    if(res != 0) pDev->err = res;
    return res;
}

uint8_t armDev_getLS(armDev_t *pDev){
    (void)armDev_get(pDev);
    return pDev->pkt_status.limit_sw;
}

struct udev_mtr_info *armDev_getMtrInfo(armDev_t *pDev, enum eArmMotors mtr){
    (void)armDev_get(pDev);
    return &pDev->pkt_status.mtr[mtr];
}

struct udev_pkt_status *armDev_getStatusPkt(armDev_t *pDev){
    (void)armDev_get(pDev);
    return &pDev->pkt_status;
}

int armDev_setServo(armDev_t *pDev, enum eArmServos servo, uint32_t val_us){
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeSrvo;
    ctrl.hdr.ctrl_typ = (uint8_t)servo;
    ctrl.servo_ctrl = val_us;
    return armdev_send(pDev, &ctrl);
}

int armDev_setMtr(armDev_t *pDev, enum eArmMotors mtr, struct udev_mtr_ctrl *pVals){
    if(pVals == NULL) return -1;
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeMtr;
    ctrl.hdr.ctrl_typ = (uint8_t)mtr;
    memcpy(&ctrl.mtr_ctrl, pVals, sizeof(struct udev_mtr_ctrl));
    memcpy(&pDev->pkt_mtr[mtr], pVals, sizeof(struct udev_mtr_ctrl));
    return armdev_send(pDev, &ctrl);

}

int armDev_setGripper(armDev_t *pDev, int8_t val){
    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeGrip;
    ctrl.hdr.ctrl_typ = 0;
    ctrl.grip_ctrl = val;
    return armdev_send(pDev, &ctrl);
}

int armDev_updateMtr(armDev_t *pDev, enum eArmMotors mtr, float pos, float vel){
    pDev->pkt_mtr[mtr].position = pos;
    pDev->pkt_mtr[mtr].velocity = vel;

    struct udev_pkt_ctrl ctrl;
    ctrl.hdr.pkt_typ = ePktTypeMtr;
    ctrl.hdr.ctrl_typ = (uint8_t)mtr;
    memcpy(&ctrl.mtr_ctrl, &pDev->pkt_mtr[mtr], sizeof(struct udev_mtr_ctrl));
    return armdev_send(pDev, &ctrl);
}

const char *armDev_getErr(armDev_t *pDev){
    return libusb_error_name(pDev->err);
}

int armDev_free(armDev_t *pDev){
    // Release interface and close device
    libusb_release_interface(pDev->lusb_devHndl, CTRL_DATA_INUM);
    libusb_close(pDev->lusb_devHndl);
    libusb_exit(pDev->lusb_ctx);
    return 0;
}
