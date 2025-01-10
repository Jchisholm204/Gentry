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
#include "os/config/FreeRTOSConfig.h"
#include "task.h"
#include "os/drivers/serial.h"
#include "string.h"
#include "os/hal/hal_clock.h"
#include "os/hal/hal_gpio.h"
#include "os/config/pin_cfg.h"
#include "os/drivers/canbus.h"
#include "os/hal/usb/hal_usbd_otgfs.h"


// Initialize all system Interfaces
void Init(void){
    // Initialize UART
    serial_init(&Serial3, 9600, PIN_USART3_RX, PIN_USART3_TX);
    // Required for MockECU Board
    // gpio_set_mode(PIN('A', 10), GPIO_MODE_OUTPUT);
    // gpio_write(PIN('A', 10), false);
    // gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    // Initialize CAN
    // can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);

}

void vTestTask(void * pvParams);
void vUART_FeedBack(void * pvParams);
void vUSB_tsk(void * pvParams);

int main(void){

    Init();
    
    // THESE SHOULD BE STATIC
    xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUART_FeedBack, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    // xTaskCreate(vUSB_tsk, "USB tst", configMINIMAL_STACK_SIZE<<1, NULL, 1, NULL);
    // xTaskCreate(vCanTask, "Can1RX", configMINIMAL_STACK_SIZE, NULL, 0, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
}

