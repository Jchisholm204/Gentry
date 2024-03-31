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
#include "hal/hal_gpio.h"
#include "drivers/uart.h"
#include "stm32f446xx.h"

TaskHandle_t xControllerTaskHandle;

StaticTask_t xMainTaskBuffer;
#define MainTaskStackSize 0x200
StackType_t xMainTaskStack[MainTaskStackSize];

void vTask_main(void *param){
    (void)param;
    // Initialize the GPIO
    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    for(;;){
        gpio_write(debug_led1, true);
        uart_write(&Serial2, "Hello World!\n", 13, 10);
        gpio_write(debug_led1, false);
        vTaskDelay(1000);
    }
}

int main(void){
    // Initialize the Serial Interface (Baud = 9600 default)
    uart_init(9600);

    xControllerTaskHandle = xTaskCreateStatic(
        vTask_main,
        "main",
        MainTaskStackSize,
        NULL,
        tskIDLE_PRIORITY+10,
        xMainTaskStack,
        &xMainTaskBuffer
    );

    printf("Controller Task Initialized\n");

    printf("Starting Scheduler...\n");
    printf("\n");

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        // gpio_write(debug_led2, false);
        __asm__("nop");
    }
    return 0;
}

