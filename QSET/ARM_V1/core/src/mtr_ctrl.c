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
#include "mtr_ctrl.h"
#include <memory.h>
#include "drivers/canbus.h"
#include <stdio.h>
#include <limits.h>

enum eTskUpdateType{
    eTskReset,
    eTskSetup,
    eTskUpdate
};

// Motor Task
void mtrCtrl_task(void *pvParams);


void mtrCtrl_init(mtrCtrlHndl_t *pHndl, enum eArmMotors mtr_id, enum AKMotorType mtr_typ, uint32_t can_id){
    // Setup the Motor Information
    pHndl->mtr_id = mtr_id;
    pHndl->akMtr.type = mtr_typ;
    pHndl->akMtr.can_id = can_id;
    pHndl->akMtr.position = 0;
    pHndl->akMtr.velocity = 0;
    pHndl->akMtr.enable = false;

    // Zero out Control Data
    pHndl->udev_ctrl = (struct udev_mtr_ctrl){0};
    pHndl->udev_setup = (struct udev_mtr_setup){0};
    pHndl->udev_info = (struct udev_mtr_info){0};
    
    // Setup Task Information
    memcpy(pHndl->pcName, "MTR_TSK0", sizeof("MTR_TSK0"));
    pHndl->pcName[9] = '0' + (uint8_t)mtr_id;

    // Setup the Task
    pHndl->pTskHndl = xTaskCreateStatic(
            mtrCtrl_task,
            pHndl->pcName,
            MTR_TSK_STACK_SIZE,
            pHndl,
            1,
            pHndl->puxStack,
            &pHndl->pxTsk
        );
}

void mtrCtrl_setup(mtrCtrlHndl_t *pHndl, struct udev_mtr_setup *pSetup){
    memcpy(&pHndl->udev_setup, pSetup, sizeof(struct udev_mtr_setup));
    xTaskNotify(pHndl->pTskHndl, eTskSetup, eSetValueWithOverwrite);
}

void mtrCtrl_update(mtrCtrlHndl_t *pHndl, struct udev_mtr_ctrl *pCtrl){
    memcpy(&pHndl->udev_ctrl, pCtrl, sizeof(struct udev_mtr_ctrl));
    xTaskNotify(pHndl->pTskHndl, eTskUpdate, eSetValueWithOverwrite);
}

void hndl_setup(AkMotor_t *pMtr, struct udev_mtr_setup *pSetup){
    pMtr->kP = pSetup->kP;
    pMtr->kI = pSetup->kI;
    pMtr->kD = pSetup->kD;
    pMtr->enable = pSetup->enable;
}

void hndl_update(AkMotor_t *pMtr, struct udev_mtr_ctrl *pCtrl){
    pMtr->velocity = pCtrl->velocity;
    pMtr->position = pCtrl->position;
}

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
    mtrCtrlHndl_t *pHndl = (mtrCtrlHndl_t*)pvParams;
    AkMotor_t *pMtr = &pHndl->akMtr;
    can_msg_t msg;

    // Wait for an intialization message from USB
    uint32_t ulNotify = eTskReset;
    while(ulNotify != eTskSetup){
        xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotify, MTR_UPDATE_TIME);
    }
    // Setup the motor using the setup data
    struct udev_mtr_setup *pSetup = &pHndl->udev_setup;
    hndl_setup(pMtr, pSetup);

    // CAN Mailbox Setup
    CanMailbox_t mailbox;
    // CAN Mailboxes are used to transfer messages from the bus controller
    //  into tasks. This also allows notifications.
    can_mailbox_init(&CANBus1, &mailbox, pHndl->akMtr.can_id);

    // Wake time save value:
    //  Ensures the task will wake every MTR_UPDATE_TIME ms
    TickType_t wakeTime = xTaskGetTickCount();

    // Motor Control Loop
    for(;;){
        // Attempt to get a message from the can bus within the time frame
        //  Blocks for MTR_UPDATE_TIME ms then continues
        if(can_mailbox_wait(&mailbox, &msg, MTR_UPDATE_TIME) == eCanOK){
            // Forward data to USB device on recv
            akMotor_unpack(pMtr, &msg);
            // Pack the data into a USB compliant structure
            pHndl->udev_info =  (struct udev_mtr_info){
                .temp = (uint8_t)pMtr->temp,
                .velocity = pMtr->velocity,
                .position = pMtr->position,
                .current = (uint8_t)(pMtr->current/10),
            };
        }

        // Handle incoming messages from the USB Bus
        if(xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotify, MTR_UPDATE_TIME) == pdPASS){
            switch((enum eTskUpdateType)ulNotify){
                case eTskSetup:
                    hndl_setup(pMtr, &pHndl->udev_setup);
                    if(pMtr->enable){
                        akMotor_enable(pMtr, &msg);
                        // Transmit the message onto the CAN Bus
                        can_write(&CANBus1, &msg, MTR_UPDATE_TIME);
                    }
                    else{
                        akMotor_disable(pMtr, &msg);
                        // Transmit the message onto the CAN Bus
                        can_write(&CANBus1, &msg, MTR_UPDATE_TIME);
                    }
                    break;
                case eTskUpdate:
                    hndl_update(pMtr, &pHndl->udev_ctrl);
                    break;
                case eTskReset:
                    pMtr->enable = false;
                    akMotor_disable(pMtr, &msg);
                    // Transmit the message onto the CAN Bus
                    can_write(&CANBus1, &msg, MTR_UPDATE_TIME);
                    break;
            }
            // Transmit the latest motor message to the CAN BUS
            akMotor_pack(pMtr, &msg);
            can_write(&CANBus1, &msg, MTR_UPDATE_TIME);
        }

        // Ensure a minimum delay so that other tasks can run
        vTaskDelayUntil(&wakeTime, MTR_UPDATE_TIME);
    }
}

