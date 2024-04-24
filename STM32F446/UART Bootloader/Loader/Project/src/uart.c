/**
 * @file uart.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "uart.h"
#include "hal_uart.h"
#include "nvicConfig.h"
#include <stddef.h>
#include "pins.h"


void uart_init(Serial_t *Serial, USART_TypeDef * UART, uint32_t baud, uint16_t pin_tx, uint16_t pin_rx){
    // Null check Pointer Inputs
    if(Serial == NULL || UART == NULL) return;

    // Assign UART
    Serial->UART = UART;

    // Uart Initialization
    gpio_set_mode(pin_rx, GPIO_MODE_AF);
    gpio_set_mode(pin_tx, GPIO_MODE_AF);
    gpio_set_af(pin_rx, GPIO_AF_UART);
    gpio_set_af(pin_tx, GPIO_AF_UART);
    hal_uart_init(UART, baud);
    hal_uart_enable_rxne(UART, true);

    switch ((uint32_t)UART){
        case USART1_BASE: {
            NVIC_SetPriority(USART1_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART1_IRQn);
            break;
            }
        case USART2_BASE: {
            NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART2_IRQn);
            break;
            }
        case USART3_BASE: {
            NVIC_SetPriority(USART3_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART3_IRQn);
            break;
            }
        case UART4_BASE: {
            NVIC_SetPriority(UART4_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(UART4_IRQn);
            break;
            }
        case UART5_BASE: {
            NVIC_SetPriority(UART5_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(UART5_IRQn);
            break;
            }
        case USART6_BASE: {
            NVIC_SetPriority(USART6_IRQn, NVIC_Priority_MIN);
            NVIC_EnableIRQ(USART6_IRQn);
            break;
            }
    }
    // Init UART Ring Buffer
    ring_buffer_init(&Serial->rb, Serial->buffer, UART_BUFFER_SIZE);
}

void uart_deinit(Serial_t *Serial){
    if(Serial == NULL) return;
    // Deinit the GPIO pins
    gpio_set_mode(Serial->pin_tx, 0);
    gpio_set_mode(Serial->pin_rx, 0);
    gpio_set_af(Serial->pin_tx, 0);
    gpio_set_af(Serial->pin_rx, 0);
    if(Serial->UART == USART2){
        hal_uart_enable_rxne(USART2, false);
        NVIC_DisableIRQ(USART2_IRQn);
    }
}

void uart_write(Serial_t *Serial, uint8_t *data, const size_t len){
    hal_uart_write_buf(Serial->UART, (char*)data, len);
}

void uart_write_byte(Serial_t *Serial, uint8_t data){
    hal_uart_write_byte(Serial->UART, data);
}

uint32_t uart_read(Serial_t *Serial, char *data, const size_t len){
    if(len == 0) return 0;
    for(size_t i = 0; i < len; i++){
        if(!ring_buffer_read(&Serial->rb, (uint8_t*)&(data[i]))){
            return i;
        }
    }
    return len;
}

uint8_t uart_read_byte(Serial_t *Serial){
    uint8_t data;
    (void)ring_buffer_read(&Serial->rb, &data);
    return data;
}

bool uart_read_ready(Serial_t *Serial){
    return !ring_buffer_empty(&Serial->rb);
}

void USART2_IRQHandler(void){
    if(hal_uart_read_ready(USART2)){
        uint8_t rx = hal_uart_read_byte(USART2);
        (void)ring_buffer_write(&Serial2.rb, rx);
    }
}


