/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */
#define STM32F446xx
#include "stm32f4xx.h"
#include "hal/hal_gpio.h"
#include "hal/hal_clock.h"
#include "serialCAN.h"
#include "drivers/uart.h"

void SystemInit(void){
    return;
}

#define BOOTLOADER_SIZE (0x4000)
#define MAIN_APP_START_ADDR  (FLASH_BASE + BOOTLOADER_SIZE)

static char loader_msg[] = "BOOTLOADER";

void jump_to_main(void){
    uint32_t *reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDR + 4UL);
    uint32_t *reset_vector = (uint32_t*)(*reset_vector_entry);
    void (*jmpFunction)(void) = (void (*)(void))reset_vector;
    jmpFunction();
}

int main(void){
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    gpio_set_mode(PIN('B', 1), GPIO_MODE_OUTPUT);
    clock_init();
    // uart_init(USART2, 9600);
    // uart_write_buf(USART2, msg, sizeof(msg));
    uart_init(9600);
    SerialCAN_t scan;
    serialCAN_init(&scan, &Serial2);
    uint32_t tick = 0;
    // for(;;){
    //     while(uart_read_ready(scan.pSerial)){
    //         uint8_t byte = uart_read_byte(scan.pSerial);
    //         uart_write_byte(scan.pSerial, byte);
    //     }
    // }
    for(;;){
        bool ok = serialCAN_tick(&scan);
        gpio_write(debug_led2, ok);
        // gpio_write(debug_led1, true);
        SerialCAN_msg_t msg;
        // if(serialCAN_read_ready(&scan)){
        //     gpio_toggle_pin(debug_led1);
        //     // uint8_t byte;
        //     // uart_read(&Serial2, &byte, 1);
        //     // uart_write_byte(&Serial2, byte);
        //     serialCAN_read(&scan, &msg);
        //     serialCAN_write(&scan, &msg);
        // }
        // else{
        //     gpio_toggle_pin(debug_led2);
        // }
        if(tick > 999){
            msg.id = 99;
            msg.len = 8;
            for(uint8_t i = 0; i < 8; i++) msg.data[i] = i;
            tick = 0;
            serialCAN_write(&scan, &msg);
        }
        else{
            tick++;
        }
        spin(1);
    }

    for(uint32_t i = 0; i < 20; i++){
        gpio_write(PIN('B', 0), 1);
        gpio_write(PIN('B', 1), 0);
        spin(99999);
        gpio_write(PIN('B', 0), 0);
        gpio_write(PIN('B', 1), 1);
        spin(99999);
    }
    SCB->VTOR = BOOTLOADER_SIZE;
    jump_to_main();

    // never return
    return 0;
}
