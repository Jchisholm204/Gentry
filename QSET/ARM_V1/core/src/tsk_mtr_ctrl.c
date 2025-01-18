/**
 * @file tsk_mtr_ctrl.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Motor Control Task
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"
#include "drivers/AK7010/ak7010.h"
#include "tsk_mtr_ctrl.h"
#include "usbd/usb_device.h"

/**
 * @brief Motor Control Task
 *
 * @param pvParams Motor ID this task should control
 */
void vTsk_mtr_ctrl(void *pvParams){
    uint32_t mtr_id = (uint32_t)pvParams;
    AK7010_t *mtr_cfg = &joint[mtr_id];
    AK7010_t mtr = *mtr_cfg;
    CanMailbox_t mailbox;
    can_mailbox_init(&CANBus1, &mailbox, mtr_cfg->can_id);
    can_msg_t msg;
    
    // Attempt to send initialization message to the motor
    ak7010_enable(&mtr, &msg);
    can_write(&CANBus1, &msg, MTR_UPDATE_TIME);

    // Motor Control Loop
    for(;;){
        // Attempt to get a message from the can bus within the time frame
        if(can_mailbox_wait(&mailbox, &msg, MTR_UPDATE_TIME) == eCanOK){
            // Forward data to USB device on recv
            ak7010_unpack(&mtr, &msg);
            struct udev_mtr_info mtr_info = {
                .temp = (uint8_t)mtr.temp,
                .velocity = mtr.velocity,
                .position = mtr.position,
                .current = (uint8_t)mtr.current,
            };
            udev_setMtr((int)mtr_id, &mtr_info);
        }

        // Relay data from USB device to the CAN bus
        struct udev_mtr_ctrl mtr_ctrl;
        udev_getMtr((int)mtr_id, &mtr_ctrl);
        mtr.position = mtr_ctrl.position;
        mtr.velocity = mtr_ctrl.velocity;
        ak7010_pack(&mtr, &msg);
        can_write(&CANBus1, &msg, MTR_UPDATE_TIME);
    }
}

