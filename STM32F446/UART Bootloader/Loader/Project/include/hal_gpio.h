/**
 * @file hal_gpio.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 1.2
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <stm32f4xx.h>
#include <stdbool.h>
typedef __SIZE_TYPE__ size_t;

// default system clock frequency for stm32f4xx
#define FREQ 16000000

// Package a pin bank (U8) and pin number (U8) into single package (U16)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
// Retrieve pin number (U8) from pin package (U16)
#define PINNO(pin) (pin & 255)
// Retrieve pin bank (U8) from pin package (U16)
#define PINBANK(pin) (pin >> 8)


#define GPIO(bank) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400U * (bank)))

// GPIO Mode Enum
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_AF_UART = 7};

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

/**
 * @brief Set the PinMode of a GPIO Pin
 * 
 * @param pin PIN(bank, number)
 * @param mode GPIO_MODE::<INPUT, OUTPUT, AF, ANALOG>
 */
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    GPIO_TypeDef *gpio = (GPIO_TypeDef*)GPIO(PINBANK(pin));
    int n = PINNO(pin);
    SET_BIT(RCC->AHB1ENR, (1UL << PINBANK(pin)));
    gpio->MODER &= ~(3U << (n*2));
    gpio->MODER |= (mode & 3U) << (n * 2);
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
