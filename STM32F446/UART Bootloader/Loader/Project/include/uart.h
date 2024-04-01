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
#include <stdbool.h>

void uart_init(void);
void uart_write(uint8_t *data, const size_t len);
void uart_write_byte(uint8_t data);
uint32_t uart_read(char *data, const size_t len);
uint8_t uart_read_byte(void);
bool uart_read_ready(void);

void USART2_IRQHandler(void);

