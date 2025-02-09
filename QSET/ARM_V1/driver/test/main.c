#include "libusb_arm.h"
#include <unistd.h>
#include <stdbool.h>
#include <memory.h>
#include <libusb-1.0/libusb.h>


armDev_t dev;

int main(void){
    armDev_init(&dev);
    struct udev_mtr_ctrl mtr = {
        .velocity = 1,
        .position = 0,
        .kP = 1,
        .kI = 0,
        .kD = 0,
        .kF = 0,
        .enable = AK_MTR_EN | AK_MTR_ZERO,
    };
    printf("udev_connected\n");
    bool dis = false;
    for(;;){
        if (dev.err != 0) {
            printf("Disconnection Event Detected\n");
            // Device is disconnected, attempt to reconnect
            armDev_reconnect(&dev);
        }
        else{
            armDev_setMtr(&dev, eJoint1, &mtr);
            // armDev_setServo(&dev, eServo2, 2200);
            // armDev_setGripper(&dev, 120);
            struct udev_pkt_status *sts = armDev_getStatusPkt(&dev);
            printf("Joint1: %0.2f rad/s LS: %d\n", sts->mtr[eJoint1].velocity, sts->limit_sw & 0x7F);
        }
        // sleep(1);
    }
    armDev_free(&dev);
    return 0;
}
