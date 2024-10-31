/**
 * @file test_tsks.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "main.h"
// #include "/usr/arm-none-eabi/include/stdio.h"
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
#include "stddef.h"

struct ctime {
    int hrs, mins, secs, msecs;
};

static inline void cTimeGet(TickType_t ticks, struct ctime *t){
    if (!t)
        return;
    float tms = ((float)ticks) / ((float)configTICK_RATE_HZ) * 1000.0;
    t->msecs = ((int)tms) % 1000;
    int secs = ((int)(tms + 500) / 1000);
    t->secs = secs % 60;
    int mins = (secs / 60);
    t->mins = mins % 60;
    t->hrs = (mins / 60);
}

#define PRINT_CTIME(ct) "%02d:%02d:%02d.%03d\n", ct.hrs, ct.mins, ct.secs, ct.msecs

void vTestTask(void * pvParams){
    (void)pvParams;
    char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    vTaskDelay(100);
    FILE * pSer3 = fdopen(3, "w");
    // hal_uart_init(USART2, 9600, PIN('A', 2), PIN('A', 3));
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        struct ctime time;
        cTimeGet(xTaskGetTickCount(), &time);
        fprintf(pSer3, PRINT_CTIME(time));
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

