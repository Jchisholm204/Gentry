/**
 * @file uart.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <stm32f4xx.h>
#include "hal_gpio.h"
#include "ringBuffer.h"
#include <stdbool.h>

#define UART_BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[UART_BUFFER_SIZE];
    ring_buffer_t rb;
    USART_TypeDef *UART;
    uint16_t pin_tx, pin_rx;
} Serial_t;

static Serial_t Serial2;

void     uart_init(      Serial_t *Serial, USART_TypeDef * UART, uint32_t baud, uint16_t pin_tx, uint16_t pin_rx);
void     uart_write(     Serial_t *Serial, uint8_t *data, const size_t len);
uint32_t uart_read(      Serial_t *Serial, char *data, const size_t len);
void     uart_write_byte(Serial_t *Serial, uint8_t data);
void     uart_deinit(    Serial_t *Serial);
uint8_t  uart_read_byte( Serial_t *Serial);
bool     uart_read_ready(Serial_t *Serial);

void USART2_IRQHandler(void);

