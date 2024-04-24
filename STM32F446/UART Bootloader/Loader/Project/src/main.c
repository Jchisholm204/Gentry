/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */
#include "hal_gpio.h"
#include "stm32f446xx.h"
#include "pins.h"
#include "uart.h"
#include "serialCAN.h"
#include "hal_flash.h"
#include <stddef.h>
#include <stdio.h>

void SystemInit(void){
    return;
}

#define BOOTLOADER_SIZE (0x8000)
#define MAIN_APP_START_ADDR  (FLASH_BASE + BOOTLOADER_SIZE)

typedef void (*pFunction)(void);

void jump_to_main(void){
    pFunction jmpFunction = (pFunction)(*(uint32_t*)(MAIN_APP_START_ADDR + 4UL));
    jmpFunction();
}

uint32_t cnt = 0;

void u2_write(uint8_t *buf, size_t len){
    uart_write(&Serial2, buf, len);
}

uint8_t u2_read(void){
    return uart_read_byte(&Serial2);
}

bool u2_read_ready(void){
    return uart_read_ready(&Serial2);
}

void u2_clear_buffer(void){

}

int main(void){

    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    
    // Initialize UART
    uart_init(&Serial2, USART2, 9600, PIN_USART2_TX, PIN_USART2_RX);

    SerialCAN_t SerialCAN;
    serialCAN_init(&SerialCAN, u2_write, u2_read, u2_clear_buffer, u2_read_ready);
    while(!serialCAN_connect(&SerialCAN, 999));
    hal_flash_unlock(999);
    
    // Bootloader Loop
    for(;;){
        // uint8_t data_buffer[1];
        // if(uart_read((char*)data_buffer, 1) > 0){
        //     uart_write(data_buffer, 1);
        // }
        serialCAN_tick(&SerialCAN);
        scan_msg_t msg;
        if(cnt > 99 && false) {
            cnt = 0;
            msg.id = 2;
            msg.len = 1;
            for(uint8_t i = 0; i < 8; i++){
                msg.data[i] = i*4;
            }
            msg.data[0] = 9;
            serialCAN_write(&SerialCAN, &msg);
        }
        else{
            cnt++;
        }

        if(serialCAN_read_ready(&SerialCAN)){
            // gpio_toggle_pin(debug_led1);
            serialCAN_read(&SerialCAN, &msg);
            // serialCAN_write(&SerialCAN, &msg);
        }
        spin(9999);
        // gpio_write(debug_led1, false);
        // uart_write((uint8_t*)"hello\n", 7);
        // gpio_write(PIN('B', 0), 1);
        // gpio_write(PIN('B', 1), 0);
        // spin(99999);
        // gpio_write(PIN('B', 0), 0);
        // gpio_write(PIN('B', 1), 1);
        // spin(99999);
    }

    // Deinit Everything
    uart_deinit(&Serial2);
    // Offset the Interrupt vector table
    SCB->VTOR = BOOTLOADER_SIZE;
    // Boot into the main program
    jump_to_main();

    // never return
    return 0;
}
