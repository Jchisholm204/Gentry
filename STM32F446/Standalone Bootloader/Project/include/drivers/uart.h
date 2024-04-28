
#pragma once

#include "structs/ring_buffer.h"
#include <stm32f4xx.h>

#ifdef BUFFER_SIZE
    #define UART_BUFFER_SIZE BUFFER_SIZE
#else
    #define UART_BUFFER_SIZE 64 
#endif

typedef struct {
    USART_TypeDef *USART;
    ring_buffer_t rx_rb;
    uint8_t rx_buffer[UART_BUFFER_SIZE];
} UART_t;

extern uint8_t rXbyte;
extern UART_t Serial2;

void uart_init(uint32_t baud);

bool uart_write(UART_t *self, uint8_t *buf, size_t len);

bool uart_write_byte(UART_t *self, uint8_t byte);

bool uart_read_ready(UART_t *self);

size_t uart_read(UART_t *self, uint8_t *buf, size_t len);

uint8_t uart_read_byte(UART_t *self);

void USART2_IRQHandler(void);

