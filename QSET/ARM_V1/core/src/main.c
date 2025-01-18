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

#include "tsk_mtr_ctrl.h"
#include "tsk_testing.h"
#include "tsk_usbTest.h"


// Initialize all system Interfaces
void Init(void){
    // Initialize UART
    serial_init(&Serial3, 250000, PIN_USART3_RX, PIN_USART3_TX);
    // Initialize CAN
    can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);
    // can_init(&CANBus2, CAN_1000KBPS, PIN_CAN2_RX, PIN_CAN2_TX);

}

#define N_TSKS 7
StackType_t puxTskStack[N_TSKS][configMINIMAL_STACK_SIZE<<1];
StaticTask_t pxTsks[N_TSKS];

int main(void){

    Init();

    // THESE SHOULD BE STATIC
    xTaskCreateStatic(vTsk_testOnline, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, puxTskStack[0], &pxTsks[0]);
    // xTaskCreateStatic(vTsk_testUART, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, puxTskStack[1], &pxTsks[1]);
    xTaskCreateStatic(vTsk_usbTest, "USB tst", configMINIMAL_STACK_SIZE<<1, NULL, 1, puxTskStack[2], &pxTsks[2]);

    // Initialize Motor Control Tasks
    xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL1", configMINIMAL_STACK_SIZE, J1, 1, puxTskStack[3], &pxTsks[3]);
    xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL2", configMINIMAL_STACK_SIZE, J2, 1, puxTskStack[4], &pxTsks[4]);
    xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL3", configMINIMAL_STACK_SIZE, J3, 1, puxTskStack[5], &pxTsks[5]);
    xTaskCreateStatic(vTsk_mtr_ctrl, "MTRCTRL4", configMINIMAL_STACK_SIZE, J4, 1, puxTskStack[6], &pxTsks[6]);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
}

