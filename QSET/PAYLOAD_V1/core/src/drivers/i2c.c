/**
 * @file i2c.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief STM32 I2C Master Driver
 * @version 0.1
 * @date 2025-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/i2c.h"

eI2CError i2c_init(I2C_t pHndl, pin_t pin_sda, pin_t pin_scl){
    if(!pHndl) return eI2C_Null;
    if(pHndl->state == eI2C_OK) return pHndl->state;

    // Initialize the lower level hardware
    hal_i2c_init(pHndl->Interface, pin_scl, pin_sda);
    pHndl->semphr_hndl = xSemaphoreCreateMutexStatic(&pHndl->static_semphr);
    if(!pHndl->semphr_hndl)
        return eI2C_IniFail;
    xSemaphoreGive(pHndl->semphr_hndl);
    pHndl->state = eI2C_OK;
    return pHndl->state;
}

eI2CError i2c_write(I2CDev_t *pDev, uint8_t reg_addr, void *buf, size_t size, TickType_t timeout){
    if(!pDev || !buf) return eI2C_Null;
    if(!pDev->pI2C)   return eI2C_Null;
    I2C_t pHndl = pDev->pI2C;

    if(pHndl->state != eI2C_OK)
        return pHndl->state;
    if(xSemaphoreTake(pHndl->semphr_hndl, timeout) == pdTRUE){
        // Check for single byte write
        if(size <= 1)
            hal_i2c_write_byte(pHndl->Interface, pDev->addr, reg_addr, *((uint8_t*)buf));
        else 
            hal_i2c_write(pHndl->Interface, pDev->addr, reg_addr, (uint8_t*)buf, size);
        xSemaphoreGive(pHndl->semphr_hndl);
        return eI2C_OK;
    }
    return eI2C_Semphr;
}

eI2CError i2c_read(I2CDev_t *pDev, uint8_t reg_addr, void *buf, size_t size, TickType_t timeout){
    if(!pDev || !buf) return eI2C_Null;
    if(!pDev->pI2C)   return eI2C_Null;
    I2C_t pHndl = pDev->pI2C;

    if(pHndl->state != eI2C_OK)
        return pHndl->state;
    if(xSemaphoreTake(pHndl->semphr_hndl, timeout) == pdTRUE){
        // Check for single byte write
        if(size <= 1)
            *((uint8_t*)buf) = hal_i2c_read_byte(pHndl->Interface, pDev->addr, reg_addr);
        else 
            hal_i2c_read(pHndl->Interface, pDev->addr, reg_addr, (uint8_t*)buf, size);
        xSemaphoreGive(pHndl->semphr_hndl);
        return eI2C_OK;
    }
    return eI2C_Semphr;
}


#ifdef _SYS_CFG_VER
#if (_SYS_CFG_VER >= 2)

#if (configUSE_I2C1 == 1)
struct _I2C _I2CBus1 = {
    .Interface = I2C1,
    .semphr_hndl = NULL,
    .static_semphr = {0},
    .state = eI2C_NoInit
};
I2C_t I2CBus1 = &_I2CBus1;
#endif
#if (configUSE_I2C2 == 1)
struct _I2C _I2CBus2 = {
    .Interface = I2C2,
    .semphr_hndl = NULL,
    .static_semphr = {0},
    .state = eI2C_NoInit
};
I2C_t I2CBus2 = &_I2CBus2;
#endif
#if (configUSE_I2C3 == 1)
struct _I2C _I2CBus3 = {
    .Interface = I2C3,
    .semphr_hndl = NULL,
    .static_semphr = {0},
    .state = eI2C_NoInit
};
I2C_t I2CBus3 = &_I2CBus3;
#endif

#endif // #ifdef _SYS_CFG_VER
#endif // #if _SYS_CFG_VER >= 2
