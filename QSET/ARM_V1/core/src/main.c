/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */


#include "main.h"
#include <stdio.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "config/FreeRTOSConfig.h"
#include "task.h"
#include "drivers/serial.h"
#include "string.h"
#include "config/pin_cfg.h"

#include "usb_device.h"
#include "usb_arm_defs.h"

#include "mtr_ctrl.h"
#include "test_tsks.h"


// Initialize all system Interfaces
void Init(void){
    // Initialize UART
    serial_init(&Serial3, 250000, PIN_USART3_RX, PIN_USART3_TX);
    // Initialize CAN
    can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);
    // can_init(&CANBus2, CAN_1000KBPS, PIN_CAN2_RX, PIN_CAN2_TX);

    udev_init();
}

// Static Task Buffers
// Increase N_TSKS to add a new task
#define N_TSKS 7
StackType_t puxTskStack[N_TSKS][configMINIMAL_STACK_SIZE<<1];
StaticTask_t pxTsks[N_TSKS];

int main(void){

    Init();

    /**
     * Initialize System Tasks...
     * All tasks should be initialized as static
     * Tasks can be initialized dynamically, but may crash the system if they
     * overflow the system memory (128Kb for the STM32f446)
     */
    xTaskCreateStatic(vTsk_testOnline, "TestTask", configMINIMAL_STACK_SIZE, NULL,
            /*Priority*/1, puxTskStack[0], &pxTsks[0]);

    // xTaskCreateStatic(vTsk_testUART, "S2 Echo", configMINIMAL_STACK_SIZE, NULL,
    //         1, puxTskStack[1], &pxTsks[1]);

    xTaskCreateStatic(vTsk_usb, "USB", configMINIMAL_STACK_SIZE<<1, NULL, 
            1, puxTskStack[2], &pxTsks[2]);

    // Initialize Motor Control Tasks
    // Pass the motor they control into the task as a (void*)
    // Task will typecast the passed parameter to uint32_t
    xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL1", configMINIMAL_STACK_SIZE,
            (void*)eJoint1, 1, puxTskStack[3], &pxTsks[3]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL2", configMINIMAL_STACK_SIZE,
    //         (void*)J2, 1, puxTskStack[4], &pxTsks[4]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL3", configMINIMAL_STACK_SIZE,
    //         (void*)J3, 1, puxTskStack[5], &pxTsks[5]);
    // xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL4", configMINIMAL_STACK_SIZE,
    //         (void*)J4, 1, puxTskStack[6], &pxTsks[6]);

    // Start Scheduler: Runs tasks initialized above
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
}

