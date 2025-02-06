/**
 * @file arm_cfg.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief General ARM Configuration File
 * @version 0.1
 * @date 2025-1-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _MTR_CTRL_H_
#define _MTR_CTRL_H_

#include "FreeRTOS.h"
#include "task.h"

#include "AkMotor/AKMotor.h"
#include "usb_packet.h"

// Time interval to force a motor update in ms
#define MTR_UPDATE_TIME 20
// Maximum time the task will wait for an external event before trying later
#define MTR_POLL_TIME (MTR_UPDATE_TIME >> 2)

#define MTR_TSK_STACK_SIZE (configMINIMAL_STACK_SIZE<<1)

/**
 * @struct _mtrCtrlHndl
 * @brief Motor Control Task Data Structure
 *
 */
typedef struct _mtrCtrlHndl {
    // Motor Identifier
    enum eArmMotors mtr_id;
    // AK Motor
    AkMotor_t akMtr;
    // USB Device Packet Data
    volatile struct udev_mtr_ctrl  udev_ctrl;
    volatile struct udev_mtr_info  udev_info;
    // FreeRTOS Task Information
    TaskHandle_t pTskHndl;
    char pcName[10];
    StackType_t puxStack[MTR_TSK_STACK_SIZE];
    StaticTask_t pxTsk;
} mtrCtrlHndl_t;

/**
 * @brief Initialize a Motor Control Task
 *
 * @param pHndl Pointer to the memory storing the Task Handle
 * @param mtr_id Motor ID of the motor to control
 * @param mtr_typ Type of the motor to control
 * @param can_id CAN ID of the motor to control
 */
void mtrCtrl_init(mtrCtrlHndl_t *const pHndl, enum eArmMotors mtr_id, enum AKMotorType mtr_typ, uint32_t can_id);

/**
 * @brief Update the USB Control Packet
 *
 * @param pHndl Motor Control Handle to update
 * @param pCtrl Pointer to the UDEV Motor Control Data
 */
void mtrCtrl_update(mtrCtrlHndl_t *pHndl, struct udev_mtr_ctrl *pCtrl);

/**
 * @brief Get the latest data from the motor in UDEV format
 *
 * @param pHndl Handle to the motor control task
 * @param pInfo Pointer to the info struct to copy into
 */
void mtrCtrl_getInfo(mtrCtrlHndl_t *pHndl, struct udev_mtr_info *pInfo);

#endif
