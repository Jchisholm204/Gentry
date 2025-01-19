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
 *  This task is designed to be able to control a single motor,
 *  The parameter passed to the task determines the "joint" to control
 *  *Designed for use with AK70-10 CAN based motors*
 *
 * @param pvParams Motor ID this task should control
 */
void vTsk_mtr_ctrl(void *pvParams){
    // Typecast the task parameters to get the internal motor ID
    uint32_t mtr_id = (uint32_t)pvParams;
    // Grab the base motor configuration (from the tsk_mtr_ctrl header file)
    AK7010_t *mtr_cfg = &joint[mtr_id];
    // Create a copy of the motor configuration for internal modification
    AK7010_t mtr = *mtr_cfg;
    // Local CAN message to use as in intermediate
    can_msg_t msg;

    // CAN Mailbox Setup
    CanMailbox_t mailbox;
    // CAN Mailboxes are used to transfer messages from the bus controller
    //  into tasks. This also allows notifications.
    can_mailbox_init(&CANBus1, &mailbox, mtr_cfg->can_id);
    
    // Attempt to send initialization message to the motor
    do{
        // Get init message from the ak7010 CANAL (CAN Abstraction Layer)
        ak7010_enable(&mtr, &msg);
        // Send the message to the bus
        can_write(&CANBus1, &msg, /*send timeout*/MTR_UPDATE_TIME);
        // Repeat send while motor is inactive
    } while(can_mailbox_wait(&mailbox, &msg, MTR_UPDATE_TIME) != eCanOK);

    // Wake time save value:
    //  Ensures the task will wake every MTR_UPDATE_TIME ms
    TickType_t wakeTime = xTaskGetTickCount();

    // Motor Control Loop
    for(;;){
        // Attempt to get a message from the can bus within the time frame
        //  Blocks for MTR_UPDATE_TIME ms then continues
        if(can_mailbox_wait(&mailbox, &msg, MTR_UPDATE_TIME) == eCanOK){
            // Forward data to USB device on recv
            ak7010_unpack(&mtr, &msg);
            // Pack the data into a USB compliant structure
            struct udev_mtr_info mtr_info = {
                .temp = (uint8_t)mtr.temp,
                .velocity = mtr.velocity,
                .position = mtr.position,
                .current = (uint8_t)mtr.current,
            };
            // Call the USB Device function to update the current transmit header
            udev_setMtr((int)mtr_id, &mtr_info);
        }

        // Relay data from USB device to the CAN bus
        // Assume that the data has been updated
        struct udev_mtr_ctrl mtr_ctrl;
        udev_getMtr((int)mtr_id, &mtr_ctrl);
        // Transfer data from udev control structure into AK70-10 structure
        mtr.position = mtr_ctrl.position;
        mtr.velocity = mtr_ctrl.velocity;
        // use AK70-10 CANAL to pack the message
        ak7010_pack(&mtr, &msg);
        // Transmit the message onto the CAN Bus
        can_write(&CANBus1, &msg, MTR_UPDATE_TIME);

        // Ensure a minimum delay so that other tasks can run
        vTaskDelayUntil(&wakeTime, MTR_UPDATE_TIME);
    }
}

