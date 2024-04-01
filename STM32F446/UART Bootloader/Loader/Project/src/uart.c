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

static volatile uint8_t data_buffer = 0U;
static volatile bool data_avaliable = false;

void uart_init(void){
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
    if(len > 0){
        *data = data_buffer;
        data_avaliable = false;
        return 1;
    }
    return 0;
}

uint8_t uart_read_byte(void){
    data_avaliable = false;
    return data_buffer;
}

bool uart_read_ready(void){
    return data_avaliable;
}

void USART2_IRQHandler(void){
    if(hal_uart_read_ready(USART2)){
        data_buffer = hal_uart_read_byte(USART2);
        data_avaliable = true;
    }
}

