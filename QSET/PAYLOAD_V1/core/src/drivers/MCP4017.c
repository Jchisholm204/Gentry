/**
 * @file MCP4017.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date Created: 2025-04-13
 * @modified Last Modified: 2025-04-13
 *
 * @copyright Copyright (c) 2025
 *
 * Header file for the MCP4017 Driver
 *
 * The MCP4017 is an I2C Digital Potentiometer
 * This driver relies on I2C Driver functions and I2C Hal
 */

#include "drivers/MCP4017.h"

#define I2C_ADDR 0x2F
#define RESET 0x03
#define WRITE 0x02

