/**
 * @file pin.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief PINS declaration file
 * @version 0.1
 * @date 2025-01-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _PIN_H_
#define _PIN_H_

#include <stdint.h>
typedef uint16_t pin_t;

// Package a pin bank (U8) and pin number (U8) into single package (U16)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
// Retrieve pin number (U8) from pin package (U16)
#define PINNO(pin) (pin & 255)
// Retrieve pin bank (U8) from pin package (U16)
#define PINBANK(pin) (pin >> 8)

#endif
