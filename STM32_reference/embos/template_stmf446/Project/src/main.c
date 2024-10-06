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
#include "stm32f446xx.h"
#include "RTOS.h"


void vTask_main(void *param){
    (void)param;
    // Initialize the GPIO
    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    for(;;){
        gpio_write(debug_led1, true);
        spin(99999);
        gpio_write(debug_led1, false);
        spin(99999);
    }
}

static OS_STACKPTR int Stack[128];
static OS_TASK TBC;

int main(void){
    OS_Init();
    OS_InitHW();
    OS_TASK_CREATE(&TBC, "Task", 100, vTask_main, Stack);
    OS_Start();
    // Initialize the Serial Interface (Baud = 9600 default)
    vTask_main(NULL);
    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        // gpio_write(debug_led2, false);
        __asm__("nop");
    }
    return 0;
}

