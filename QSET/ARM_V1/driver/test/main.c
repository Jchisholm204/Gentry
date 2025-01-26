#include "libusb_arm.h"
#include <unistd.h>
#include <stdbool.h>

armDev_t dev;

int main(void){
    int set = armDev_init(&dev);
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
        armDev_setMtr(&dev, eJoint1, &mtr);
        armDev_setServo(&dev, eServo2, 2200);
        struct udev_pkt_status sts = {0};// = armDev_getStatusPkt(&dev);
        // Receive data
        int transferred;
        int res = libusb_bulk_transfer(dev.lusb_devHndl, CTRL_RXD_EP, (unsigned char *)&sts, sizeof(struct udev_pkt_status), &transferred, 100);
        printf("transferred %d/%d bytes\n", transferred, sizeof(struct udev_pkt_status));
        printf("Joint1: %0.2f rad/s LS: %d\n", sts.mtr[eJoint1].velocity, (int)sts.limit_sw);
        uint8_t lsw = sts.limit_sw & 0x7F;
        printf("%d, %d, %d\n", lsw & 0x02, lsw & 0x04, lsw & 0x08);
        sleep(1);
    }
    armDev_free(&dev);
    return 0;
}
