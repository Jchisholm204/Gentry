#include "libusb_arm.h"
#include <unistd.h>
#include <stdbool.h>

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
        .enable = true,
    };
    printf("udev_connected\n");
    for(;;){
        // armDev_setMtr(&dev, eJoint1, &mtr);
        // armDev_setServo(&dev, eServo2, 2200);
        // struct udev_pkt_status *sts = armDev_getStatusPkt(&dev);
        uint8_t pkt_status[52] = {0};
        struct udev_pkt_status *sts = (void*)&pkt_status;
        int transferred = 0;
    int res = libusb_bulk_transfer(dev.lusb_devHndl, CTRL_TXD_EP, (unsigned char *)&pkt_status, 52, &transferred, 0);
        printf("Transfered %d bytes\n", transferred);
        printf("bufsts: %d %d\n", pkt_status[2], pkt_status[3]);
        printf("Joint1: %0.2f LS: %d\n", sts->mtr[0].velocity, sts->limit_sw);
        sleep(1);
    }
    armDev_free(&dev);
    return 0;
}
