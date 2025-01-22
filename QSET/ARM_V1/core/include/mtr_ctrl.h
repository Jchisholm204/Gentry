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

#define MTR_TSK_STACK_SIZE configMINIMAL_STACK_SIZE

/**
 * @struct _mtrCtrlHndl
 * @brief Motor Control Task Data Structure
 *
 */
typedef struct _mtrCtrlHndl {
    enum eArmMotors mtr_id;
    AkMotor_t akMtr;
    TaskHandle_t pTskHndl;
    char pcName[10];
    struct udev_mtr_setup udev_setup;
    struct udev_mtr_ctrl  udev_ctrl;
    struct udev_mtr_info  udev_info;
    StackType_t puxStack[MTR_TSK_STACK_SIZE];
    StaticTask_t pxTsk;
} mtrCtrlHndl_t;

void mtrCtrl_init(mtrCtrlHndl_t *pHndl, enum eArmMotors mtr_id, enum AKMotorType mtr_typ, uint32_t can_id);

void mtrCtrl_setup(mtrCtrlHndl_t *pHndl, struct udev_mtr_setup *pSetup);

void mtrCtrl_update(mtrCtrlHndl_t *pHndl, struct udev_mtr_ctrl *pCtrl);


#endif
