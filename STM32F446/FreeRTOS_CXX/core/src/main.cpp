/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */


#include "main.hpp"
#include <stdio.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "os/config/FreeRTOSConfig.h"
#include "task.h"
#include "drivers/uart.hpp"
#include "string.h"
#include "hal/hal_clock.h"
#include "hal/hal_gpio.h"

#define debug_led PIN('D', 12)

USE_Serial2()

                                    

extern "C" void vTestTask(void * pvParams){
    (void)pvParams;
    const static char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // Serial2.write((uint8_t*)str, strlen(str));
        // sleep for 1000 ms
        vTaskDelay(1000);
    }
}

// Initialize all system Interfaces
void Init(void){
    
    Serial2.setup(9600, PIN('D', 6), PIN('D', 5));
}


extern "C" int main(void){

    Init();
    
    xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
    }
    return 0;
}

