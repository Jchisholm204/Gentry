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
    // hal_uart_init(USART2, 9600, PIN('A', 2), PIN('A', 3));
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // hal_uart_write_buf(USART2, (char*)str, strlen(str));
        Serial2.write((uint8_t*)str, strlen(str));
        // sleep for 1000 ms
        vTaskDelay(1000);
    }
}

extern "C" void vUART_FeedBack(void * pvParams){
    (void)pvParams;
    StreamBufferHandle_t xStreamBuffer = xStreamBufferCreate(100, 1);
    // Serial2.attach(xStreamBuffer);
    for(;;){
        char buffer[] = "Task2\n";
        // int bytes = xStreamBufferReceive(xStreamBuffer, (void*)buffer, 100, portMAX_DELAY);
        // printf("Serial 2 got: %s", buffer);
        Serial2.write((uint8_t*)buffer, 7);
        vTaskDelay(1000);
    }
}

// Initialize all system Interfaces
void Init(void){
    
    Serial2.setup(9600, PIN('A', 3), PIN('A', 2));
}


extern "C" int main(void){

    Init();
    
    xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUART_FeedBack, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
    }
    return 0;
}

