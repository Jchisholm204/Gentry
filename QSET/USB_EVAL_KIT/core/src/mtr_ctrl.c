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

// Motor Task
void mtrCtrl_task(void *pvParams);


void mtrCtrl_init(mtrCtrlHndl_t * const pHndl, enum eArmMotors mtr_id, enum AKMotorType mtr_typ, uint32_t can_id){
    // Setup the Motor Information
    pHndl->mtr_id = mtr_id;
    // Forcefully modify the mtr_type parameter
    *(enum AKMotorType*)(&pHndl->akMtr.type) = mtr_typ;
    // Forcefully modify the can_id parameter
    *((uint32_t*)&pHndl->akMtr.can_id) = (uint32_t)can_id;
    pHndl->akMtr.position = 0;
    pHndl->akMtr.velocity = 0;
    pHndl->akMtr.enable = false;

    // Zero out Control Data
    pHndl->udev_ctrl = (struct udev_mtr_ctrl){0};
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

void mtrCtrl_update(mtrCtrlHndl_t *pHndl, struct udev_mtr_ctrl *pCtrl){
    BaseType_t xHigherWoken;
    volatile struct udev_mtr_ctrl *pHndlCtrl = &pHndl->udev_ctrl;
    // memcpy(&pHndl->udev_ctrl, pCtrl, sizeof(struct udev_mtr_ctrl));
    pHndlCtrl->enable = pCtrl->enable;
    pHndlCtrl->kP = pCtrl->kP;
    pHndlCtrl->kI = pCtrl->kI;
    pHndlCtrl->kD = pCtrl->kD;
    pHndlCtrl->kF = pCtrl->kF;
    pHndlCtrl->velocity = pCtrl->velocity;
    pHndlCtrl->position = pCtrl->position;
    xTaskNotifyFromISR(pHndl->pTskHndl, 0, eSetValueWithOverwrite, &xHigherWoken);
    portYIELD_FROM_ISR(xHigherWoken);
}

void mtrCtrl_getInfo(mtrCtrlHndl_t *pHndl, struct udev_mtr_info *pInfo){
    volatile struct udev_mtr_info *pI = &pHndl->udev_info;
    pInfo->position = pI->position;
    pInfo->velocity = pI->velocity;
    pInfo->temp = pI->temp;
    pInfo->current = pI->current;
}


/**
 * @brief Motor Control Task
 *  This task is designed to be able to control a single motor,
 *  The parameter passed to the task determines the "joint" to control
 *  *Designed for use with AK70-10 CAN based motors*
 *
 * @param pvParams Motor ID this task should control
 */
void mtrCtrl_task(void *pvParams){
    // Typecast the task parameters to get the internal motor ID
    mtrCtrlHndl_t *pHndl = (mtrCtrlHndl_t*)pvParams;
    AkMotor_t *pMtr = &pHndl->akMtr;
    volatile struct udev_mtr_ctrl *pCtrl = &pHndl->udev_ctrl;
    volatile struct udev_mtr_info *pInfo = &pHndl->udev_info;
    can_msg_t msg;

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
        if(can_mailbox_wait(&mailbox, &msg, MTR_POLL_TIME) == eCanOK){
            // Forward data to USB device on recv
            akMotor_unpack(pMtr, &msg);
            // Pack the data into a USB compliant structure
            *pInfo =  (struct udev_mtr_info){
                .temp = (uint8_t)pMtr->temp,
                .velocity = pMtr->velocity,
                .position = pMtr->position,
                .current = (uint8_t)(pMtr->current/10),
            };
        }

        // Handle incoming messages from the USB Bus
        uint32_t ulNotify;
        // Check to see if a new message has been deposited
        if(xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotify, MTR_POLL_TIME) == pdPASS){
            // Only write to the bus if there is a USB message pending

            // Get the data from the control message
            pMtr->velocity = pCtrl->velocity;
            pMtr->position = pCtrl->position;
            pMtr->kP = pCtrl->kP;
            pMtr->kI = pCtrl->kI;
            pMtr->kD = pCtrl->kD;
            pMtr->enable = pCtrl->enable & 0x01;

            // Check if the motor needs to be enabled
            if(pCtrl->enable & 0x01){
                akMotor_enable(pMtr, &msg);
                // Write enable or disable message to the bus
                can_write(&CANBus1, &msg, MTR_POLL_TIME);
                CLEAR_BIT(pCtrl->enable, 0x01);
            }
            // Check if the motor needs to be zeroed
            if(pCtrl->enable & 0x02){
                akMotor_zero(pMtr, &msg);
                // Write enable or disable message to the bus
                can_write(&CANBus1, &msg, MTR_POLL_TIME);
                CLEAR_BIT(pCtrl->enable, 0x02);
            }

            // Transmit the latest motor message to the CAN BUS
            akMotor_pack(pMtr, &msg);
            can_write(&CANBus1, &msg, MTR_POLL_TIME);
        }

        // Ensure a minimum delay so that other tasks can run
        vTaskDelayUntil(&wakeTime, MTR_UPDATE_TIME);
    }
}

