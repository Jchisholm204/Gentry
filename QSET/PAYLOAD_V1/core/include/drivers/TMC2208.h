/**
 * @file TMC2208.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief TMC2208 Stepper Motor Drivers
 * @version 0.1
 * @date 2025-04-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _TMC2208_H_
#define _TMC2208_H_

#include "drivers/serial.h"

typedef enum _eTMC2208Error_ {
    eTMC2208_OK,
    eTMC2208_Fail,
    eTMC2208_NullParam,
    eTMC2208_NoSerial,
} eTMC2208Error;

typedef struct _TMC2208_ {
    Serial_t *pSerial;
    StreamBufferHandle_t buffer_hndl;
    StaticStreamBuffer_t buffer_stic;
    struct _TMC2208_ *pMaster;
    struct _TMC2208_ *pSlave;
} TMC2208_t;

/**
 * @brief Initialize a TMC2208 Stepper Driver
 *
 * @param pDev Pointer to the TMC2208 Data Structure to Initialize
 * @param pSerial Serial interface to communicate on or NULL if none
 * @param pin_dir TMC2208 Direction Pin
 * @param pin_step TMC2208 Step pin (Must be on timer interface)
 * @return 
 */
extern eTMC2208Error tmc2208_init(TMC2208_t *pDev, Serial_t *pSerial, pin_t pin_dir, pin_t pin_step);


/**
 * @brief Initialize a TMC2208 Chain Device (For when devices share the same dir/step pins)
 *
 * @param pMaster Master TMC2208 (The one commands will be issued to)
 * @param pSlave Pointer to the slave's data structure
 * @param pSerial Serial Interface for the slave device or NULL if none
 * @return 
 */
extern eTMC2208Error tmc2208_initChain(TMC2208_t *pMaster, TMC2208_t *pSlave, Serial_t *pSerial);

/**
 * @brief Move the Stepper motor
 *
 * @param pDev Motor to move
 * @param speed Speed to move at
 * @param steps Steps to move
 * @return 
 */
extern eTMC2208Error tmc2208_move(TMC2208_t *pDev, int32_t speed, uint32_t steps);

/**
 * @brief Set the current limits for the device
 *
 * @param pDev Device Handle Pointer
 * @param hold (0-31) Hold Current Max
 * @param run (0-31) Running Current Max
 * @return 
 */
extern eTMC2208Error tmc2208_setCurrent(TMC2208_t *pDev, uint8_t hold, uint8_t run);

/**
 * @brief Set the Hold Delay (time before switching to hold current)
 *
 * @param pDev Device Handle Pointer
 * @param hold_delay The Hold Delay to set
 * @return 
 */
extern eTMC2208Error tmc2208_setHoldDelay(TMC2208_t *pDev, uint8_t hold_delay);

/**
 * @brief Get the current velocity from the TMC2208
 *
 * @param pDev Pointer to Device Handle
 * @param speed Return by reference the current speed
 * @return 
 */
extern eTMC2208Error tmc2208_getVelocity(TMC2208_t *pDev, uint32_t *speed);

/**
 * @brief Get the current motor current
 *
 * @param pDev Pointer to Device Handle
 * @param current Return by reference the motor current
 * @return 
 */
extern eTMC2208Error tmc2208_getCurrent(TMC2208_t *pDev, int16_t *current);

#endif

