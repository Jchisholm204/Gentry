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

struct ctime {
    int hrs, mins, secs, msecs;
};

static inline void cTimeGet(TickType_t ticks, struct ctime *t){
    if (!t)
        return;
    float tms = (((float)ticks) / ((float)configTICK_RATE_HZ) * 1000);
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
    gpio_set_mode(PIN_LED1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED3, GPIO_MODE_OUTPUT);
    uint8_t leds = 1U;
    vTaskDelay(100);
    for(;;){
        // leds = leds << 1U;
        // if(leds == 8) leds = 1;
        leds += 1;
        leds &= 0x7U;
        gpio_write(PIN_LED1, leds & 0x1U);
        gpio_write(PIN_LED2, leds & 0x2U);
        gpio_write(PIN_LED3, leds & 0x4U);
        fprintf(Serial3.fp, "Hello ");
        struct ctime time;
        cTimeGet(xTaskGetTickCount(), &time);
        fprintf(Serial3.fp, PRINT_CTIME(time));
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
    vTaskDelay(portMAX_DELAY);
    printf("Uart Feedback online\n");
    for(;;){
        char buffer[100] = {0};
        xStreamBufferReceive(rx_buf, (void*)buffer, 100, portMAX_DELAY);
        // gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, msg, strlen(msg), 100);
        // serial_write(&Serial2, buffer, bytes, 100);
        // printf("Serial 2 RX: %s\n", buffer);
        // printf("Serial 2 got: %s", buffer);
        vTaskDelay(10000);
    }
}

