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
#include "config/FreeRTOSConfig.h"
#include "task.h"
#include "drivers/serial.h"
#include "string.h"
#include "hal/hal_clock.h"
#include "hal/hal_gpio.h"
#include "config/pin_cfg.h"
#include "drivers/canbus.h"
#include "systime.h"

can_msg_t msg = {
    .id = 12,
    .len = 1,
    .data = {0},
    .timestamp = 0,
    .format = STANDARD_FORMAT,
    .type = DATA_FRAME,
};

void vTsk_testOnline(void * pvParams){
    (void)pvParams;
    char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN_LED1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN_LED3, GPIO_MODE_OUTPUT);
    uint8_t leds = 1U;
    // hal_can_init(CAN1, CAN_1000KBPS, true, PIN_CAN1_TX, PIN_CAN1_RX);
    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    vTaskDelay(100);
    for(;;){
        // leds = leds << 1U;
        // if(leds == 8) leds = 1;
        leds ^= 0x3U;
        gpio_write(PIN_LED1, leds & 0x1U);
        gpio_write(PIN_LED2, leds & 0x2U);
        // gpio_write(PIN_LED3, leds & 0x4U);
        // fprintf(Serial3.fp, "Hello ");
        // struct ctime time;
        // cTimeGet(xTaskGetTickCount(), &time);
        // fprintf(Serial3.fp, PRINT_CTIME(time));
        struct systime t;
        systime_fromTicks(xTaskGetTickCount(), &t);
        fprintf(Serial3.fp, "%s\n", t.str);
        // sleep for 1000 ms
        msg.data[0] = (uint8_t) t.secs;
        can_write(&CANBus1, &msg, 100);
        // while(hal_can_send_ready(CAN1, 0) == 0);
        // hal_can_send(CAN1, &msg, 0);
        // if(hal_can_read_ready(CAN1)){
        //     can_msg_t rx;
        //     hal_can_read(CAN1, &rx);
        //     gpio_toggle_pin(PIN_LED3);
        //     fprintf(Serial3.fp, "CAN MSG ID: %d\n", rx.id);
        // }
        vTaskDelay(1000);
    }
}

void vTsk_testUART(void * pvParams){
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

