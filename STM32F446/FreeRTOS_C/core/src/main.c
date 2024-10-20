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
#include "usb_cdc_tsk.h"

#define debug_led PIN('D', 12)


void vTestTask(void * pvParams){
    (void)pvParams;
    char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    vTaskDelay(100);
    // hal_uart_init(USART2, 9600, PIN('A', 2), PIN('A', 3));
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, str, strlen(str), 100);
        // printf("Hello from Serial 2\n");
        // hal_uart_write_buf(USART2, (char*)str, strlen(str));
        // Serial2.write((uint8_t*)str, strlen(str));
        // sleep for 1000 ms
        vTaskDelay(1000);
    }
}

void vUART_FeedBack(void * pvParams){
    (void)pvParams;
    // StreamBufferHandle_t hndl = xStreamBufferCreateStatic(100, 1, NULL, NULL);
    StreamBufferHandle_t rx_buf = xStreamBufferCreate(100, 5);
    // StreamBufferHandle_t xStreamBuffer = xStreamBufferCreate(100, 1);
    // Serial2.attach(xStreamBuffer);
    serial_attach(&Serial3, &rx_buf);
    // char * msg = "Serial 2 got: ";
    vTaskDelay(1000);
    printf("Uart Feedback online\n");
    for(;;){
        char buffer[100] = {0};
        xStreamBufferReceive(rx_buf, (void*)buffer, 100, portMAX_DELAY);
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, msg, strlen(msg), 100);
        // serial_write(&Serial2, buffer, bytes, 100);
        printf("Serial 2 RX: %s\n", buffer);
        // printf("Serial 2 got: %s", buffer);
        // vTaskDelay(10);
    }
}



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


int main(void){

    Init();
    vUSB_tsk(NULL);
    
    // THESE SHOULD BE STATIC
    xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUART_FeedBack, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUSB_tsk, "USB tst", configMINIMAL_STACK_SIZE<<1, NULL, 1, NULL);
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

