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
#include "ringBuffer.h"

#define BUFFNR_SIZE 64
static uint8_t data_buffer[BUFFNR_SIZE];

static ring_buffer_t rb = {0U};

void uart_init(void){
    ring_buffer_init(&rb, data_buffer, BUFFNR_SIZE);
    hal_uart_init(USART2, 9600);
    hal_uart_enable_rxne(USART2, true);
}

void uart_write(uint8_t *data, const size_t len){
    hal_uart_write_buf(USART2, (char*)data, len);
}

void uart_write_byte(uint8_t data){
    hal_uart_write_byte(USART2, data);
}

uint32_t uart_read(char *data, const size_t len){
    if(len == 0) return 0;
    for(size_t i = 0; i < len; i++){
        if(!ring_buffer_read(&rb, (uint8_t*)&(data[i]))){
            return i;
        }
    }
    return len;
}

uint8_t uart_read_byte(void){
    uint8_t data;
    (void)ring_buffer_read(&rb, &data);
    return data;
}

bool uart_read_ready(void){
    return !ring_buffer_empty(&rb);
}

void USART2_IRQHandler(void){
    if(hal_uart_read_ready(USART2)){
        uint8_t rx = hal_uart_read_byte(USART2);
        (void)ring_buffer_write(&rb, rx);
    }
}

