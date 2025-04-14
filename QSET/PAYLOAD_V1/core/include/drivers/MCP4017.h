/**
 * @file MCP4017.h
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
#ifndef _MCP4017_DRV_H_
#define _MCP4017_DRV_H_
#include "drivers/i2c.h"

#define MCP4017_I2C_ADDR 0x2F
#define MCP4017_RESET 0x03
#define MCP4017_WRITE 0x02

static inline void mcp4017_write(I2C_t I2CBus, uint8_t value){
    I2CDev_t dev = {0};
    i2cDev_init(&dev, I2CBus, MCP4017_I2C_ADDR);
    i2c_write(&dev, value, &value, 1, 100);
}

#endif

