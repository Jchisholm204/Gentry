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

#define debug_led PIN('D', 12)


void vTestTask(void * pvParams){
    (void)pvParams;
    char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    
    // hal_uart_init(USART2, 9600, PIN('A', 2), PIN('A', 3));
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, str, strlen(str), 100);
        printf("Hello from Serial 2\n");
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
    serial_attach(&Serial2, &rx_buf);
    // char * msg = "Serial 2 got: ";
    for(;;){
        char buffer[100] = {0};
        int bytes = xStreamBufferReceive(rx_buf, (void*)buffer, 100, portMAX_DELAY);
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, msg, strlen(msg), 100);
        // serial_write(&Serial2, buffer, bytes, 100);
        printf("Serial 2 RX: %s\n", buffer);
        // printf("Serial 2 got: %s", buffer);
        // vTaskDelay(10);
    }
}

void vCanTask(void *pvParams){
    CanMailbox_t mailbox;
    static can_msg_t buf[100];
    vPortEnterCritical();
    printf("Starting CAN1 rx task\n");
    eCanError e = can_mailbox_init(&mailbox, buf, 100);
    // mailbox.buf_hndl = xStreamBufferCreateStatic(sizeof(can_msg_t)*100, sizeof(can_msg_t), buf, &mailbox.static_buf);
    // mailbox.buf_hndl = xStreamBufferCreate(sizeof(can_msg_t)*100, sizeof(can_msg_t));
    printf("Mailbox Initialized %d\n", e);
    // Allow all can messages
    can_mailbox_addMask(&mailbox, UINT32_MAX);
    can_attach(&CANBus1, &mailbox);
    printf("mailbox attached with id %d\n", mailbox.id);
    vPortExitCritical();
    vTaskDelay(100);
    printf("Entering CAN1 rx task\n");
    for(int i = 0; i < 100; i++){
        can_msg_t msg;
        can_mailbox_read(&mailbox, &msg, portMAX_DELAY);
        printf("CAN1 got msg with id: %d\n", msg.id);
        // vTaskDelay(100);
    }
    vPortEnterCritical();
    can_detach(&CANBus1, &mailbox);
    // CANBus1.mailbox = NULL;
    // CANBus1.n_mailboxes = 0;
    vPortExitCritical();
    printf("Mailbox detatched\n");
    // EXIT Gracefully (Scheduler will halt if this is not present)
    vTaskSuspend(NULL);
}

// Initialize all system Interfaces
void Init(void){
    serial_init(&Serial2, 9600, PIN_USART2_RX, PIN_USART2_TX);
    gpio_set_mode(PIN('A', 10), GPIO_MODE_OUTPUT);
    gpio_write(PIN('A', 10), false);
    gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MIN);
    NVIC_EnableIRQ(CAN1_RX0_IRQn);
    NVIC_SetPriority(CAN1_RX0_IRQn, NVIC_Priority_MIN);
    // Serial2.setup(9600, PIN('A', 3), PIN('A', 2));
}


int main(void){

    Init();
    
    // xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    // xTaskCreate(vUART_FeedBack, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vCanTask, "Can1RX", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
}

