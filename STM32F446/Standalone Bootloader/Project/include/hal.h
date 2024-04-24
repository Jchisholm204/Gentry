#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stm32f446xx.h>

// CPU Frequency
#define FREQ 16000000

// Set Bit x 1
#define BIT(x) (1UL << (x))
// Package a pin bank (U8) and pin number (U8) into single package (U16)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
// Retrieve pin number (U8) from pin package (U16)
#define PINNO(pin) (pin & 255)
// Retrieve pin bank (U8) from pin package (U16)
#define PINBANK(pin) (pin >> 8)

#define GPIO(bank) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400U * (bank)))
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

/**
 * @brief Set the PinMode of a GPIO Pin
 * 
 * @param pin PIN(bank, number)
 * @param mode GPIO_MODE::<INPUT, OUTPUT, AF, ANALOG>
 */
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    RCC->AHB1ENR |= BIT(PINBANK(pin));       // Enable GPIO clock
    gpio->MODER &= ~(3U << (n*2));
    gpio->MODER |= (mode & 3U) << (n * 2);   // Set new mode

}

/**
 * @brief Set GPIO Alternate Function
 * 
 * @param pin 
 * @param af_num 
 */
static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}

/**
 * @brief Write a Digital Value to a pin
 * 
 * @param pin PIN(bank, number)
 * @param value <true, false>
 */
static inline void gpio_write(uint16_t pin, bool value) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
}


///////////////////////////////////////////////////////////////////////////////////////////////////



static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
    // figure 19. selecting an alternate function (7=spi2/3, usart1..3, uart5, spdif-in)
    uint8_t af = 7;           // Alternate function
    uint16_t rx = 0, tx = 0;  // pins

    if (uart == USART1) RCC->APB2ENR |= BIT(4);
    if (uart == USART2) RCC->APB1ENR |= BIT(17);
    if (uart == USART3) RCC->APB1ENR |= BIT(18);

    if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
    if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
    if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, af);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    uart->CR1 = 0;                           // Disable this UART
    uart->BRR = FREQ / baud;                 // FREQ is a UART bus frequency
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3);  // Set UE, RE, TE
}

static inline int uart_read_ready(USART_TypeDef *uart){
    return uart->SR & BIT(5);
}

static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
    return ((uint8_t) (uart->DR & 255));
}

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

static inline void uart_write_byte(USART_TypeDef * uart, uint8_t byte) {
    uart->DR = byte;
    while ((uart->SR & BIT(7)) == 0) spin(1);
    
}

static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len){
    while(len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// t: expiration time, prd: period, now: current time. Return true if expired
static inline bool timer_expired(volatile uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // First poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}