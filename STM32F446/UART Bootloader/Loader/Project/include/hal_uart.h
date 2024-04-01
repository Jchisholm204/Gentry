/**
 * @file hal_uart.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 1.2
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "hal_gpio.h"
#include <stdbool.h>

static inline void hal_uart_init(USART_TypeDef *uart, unsigned long baud) {
    // figure 19. selecting an alternate function (7=spi2/3, usart1..3, uart5, spdif-in)

    if (uart == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    if (uart == USART2) RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    if (uart == USART3) RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    if (uart == UART4) RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
    if (uart == UART5) RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    if (uart == USART6) RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

    // Reset and disable the interface
    uart->CR1 = 0;
    // Set Baud rate (bits per second)
    uart->BRR = FREQ / baud;
    // Set => UART Enable | Receiver Enable | Transmit Enable
    uart->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
}

static inline void hal_uart_enable_rxne(USART_TypeDef *uart, bool enable){
    if(enable){
        SET_BIT(uart->CR1, USART_CR1_RXNEIE);
    }
    else{
        CLEAR_BIT(uart->CR1, USART_CR1_RXNEIE);
    };
}

static inline void hal_uart_enable_txne(USART_TypeDef *uart, bool enable){
    if(enable){
        SET_BIT(uart->CR1, USART_CR1_TXEIE);
    }
    else{
        CLEAR_BIT(uart->CR1, USART_CR1_TXEIE);
    };
}

static inline int hal_uart_read_ready(const USART_TypeDef *uart){
    return uart->SR & USART_SR_RXNE;
}

static inline uint8_t hal_uart_read_byte(const USART_TypeDef *uart) {
    return ((uint8_t) (uart->DR & 255));
}


static inline void hal_uart_write_byte(USART_TypeDef * uart, uint8_t byte) {
    uart->DR = byte;
    while ((uart->SR & USART_SR_TXE) == 0) __asm__("nop");
    
}

static inline void hal_uart_write_buf(USART_TypeDef *uart, char *buf, size_t len){
    while(len-- > 0) hal_uart_write_byte(uart, *(uint8_t *) buf++);
}

