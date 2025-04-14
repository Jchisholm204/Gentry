/**
 * @file i2c.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief STM32 I2C Master Driver
 * @version 0.1
 * @date 2025-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_

#include "hal/hal_i2c.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "config/sys_cfg.h"
#include <stdio.h>

typedef enum {
    eI2C_OK,
    eI2C_HWFail,
    eI2C_Semphr,
    eI2C_NoInit,
    eI2C_IniFail,
    eI2C_Null,
} eI2CError;

struct _I2C {
    I2C_TypeDef *Interface;
    SemaphoreHandle_t semphr_hndl;
    StaticSemaphore_t static_semphr;
    eI2CError state;
};

typedef struct _I2C *I2C_t;

typedef struct _I2CDev {
    I2C_t pI2C;
    uint8_t addr;
} I2CDev_t;

/**
 * @brief Initialize an I2C Interface - Called in Init
 *
 * @param pHndl Handle of the I2C Interface to init
 * @param pin_sda 
 * @param pin_scl 
 * @return 
 */
extern eI2CError i2c_init(I2C_t pHndl, pin_t pin_sda, pin_t pin_scl);

/**
 * @brief Write to an I2C Device
 *
 * @param pDev Device to Write to
 * @param reg_addr Register Address
 * @param buf Pointer to the data buffer
 * @param size Size of the data buffer (in bytes)
 * @param timeout Semaphore timeout
 * @return 
 */
extern eI2CError i2c_write(I2CDev_t *pDev, uint8_t reg_addr, void *buf, size_t size, TickType_t timeout);

/**
 * @brief Read from an I2C Device
 *
 * @param pDev Device to read from
 * @param reg_addr Register Address
 * @param buf Pointer to the data buffer
 * @param size number of bytes to read
 * @param timeout Semaphore timeout
 * @return 
 */
extern eI2CError i2c_read(I2CDev_t *pDev, uint8_t reg_addr, void *buf, size_t size, TickType_t timeout);

// Require _SYS_CFG_VER to be defined and greater than minor version 2
// Version minor 1 of _SYS_CFG_H_ does not contain I2C USE directives
// This file can still be used, but preconfigured interfaces will
//  not be able to be used.
#ifdef _SYS_CFG_VER
#if (_SYS_CFG_VER >= 2)

#if (configUSE_I2C1 == 1)
extern I2C_t I2CBus1;
#endif
#if (configUSE_I2C2 == 1)
extern I2C_t I2CBus2;
#endif
#if (configUSE_I2C3 == 1)
extern I2C_t I2CBus3;
#endif

#endif // #ifdef _SYS_CFG_VER
#endif // #if _SYS_CFG_VER >= 2

#endif

