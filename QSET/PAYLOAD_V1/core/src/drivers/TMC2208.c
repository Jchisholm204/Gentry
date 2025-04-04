/**
 * @file TMC2208.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief TMC2208 Stepper Motor Drivers
 * @version 0.1
 * @date 2025-04-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/TMC2208.h"

/**
 * @brief Initialize a TMC2208 Stepper Driver
 *
 * @param pDev Pointer to the TMC2208 Data Structure to Initialize
 * @param pSerial Serial interface to communicate on or NULL if none
 * @param pin_dir TMC2208 Direction Pin
 * @param pin_step TMC2208 Step pin (Must be on timer interface)
 * @return 
 */
eTMC2208Error tmc2208_init(TMC2208_t *pDev, Serial_t *pSerial, pin_t pin_dir, pin_t pin_step){
    (void)pDev;
    (void)pSerial;
    (void)pin_dir;
    (void)pin_step;
    return eTMC2208_OK;
}


/**
 * @brief Initialize a TMC2208 Chain Device (For when devices share the same dir/step pins)
 *
 * @param pMaster Master TMC2208 (The one commands will be issued to)
 * @param pSlave Pointer to the slave's data structure
 * @param pSerial Serial Interface for the slave device or NULL if none
 * @return 
 */
eTMC2208Error tmc2208_initChain(TMC2208_t *pMaster, TMC2208_t *pSlave, Serial_t *pSerial){
    (void)pMaster;
    (void)pSlave;
    (void)pSerial;
    return eTMC2208_OK;
}

/**
 * @brief Move the Stepper motor
 *
 * @param pDev Motor to move
 * @param speed Speed to move at
 * @param steps Steps to move
 * @return 
 */
eTMC2208Error tmc2208_move(TMC2208_t *pDev, int32_t speed, uint32_t steps){
    (void)pDev;
    (void)speed;
    (void)steps;
    return eTMC2208_OK;
}

/**
 * @brief Set the current limits for the device
 *
 * @param pDev Device Handle Pointer
 * @param hold (0-31) Hold Current Max
 * @param run (0-31) Running Current Max
 * @return 
 */
eTMC2208Error tmc2208_setCurrent(TMC2208_t *pDev, uint8_t hold, uint8_t run){
    (void)pDev;
    (void)hold;
    (void)run;
    return eTMC2208_OK;
}

/**
 * @brief Set the Hold Delay (time before switching to hold current)
 *
 * @param pDev Device Handle Pointer
 * @param hold_delay The Hold Delay to set
 * @return 
 */
eTMC2208Error tmc2208_setHoldDelay(TMC2208_t *pDev, uint8_t hold_delay){
    (void)pDev;
    (void)hold_delay;
    return eTMC2208_OK;
}

/**
 * @brief Get the current velocity from the TMC2208
 *
 * @param pDev Pointer to Device Handle
 * @param speed Return by reference the current speed
 * @return 
 */
eTMC2208Error tmc2208_getVelocity(TMC2208_t *pDev, uint32_t *speed){
    (void)pDev;
    (void)speed;
    return eTMC2208_OK;
}

/**
 * @brief Get the current motor current
 *
 * @param pDev Pointer to Device Handle
 * @param current Return by reference the motor current
 * @return 
 */
eTMC2208Error tmc2208_getCurrent(TMC2208_t *pDev, int16_t *current){
    (void)pDev;
    (void)current;
    return eTMC2208_OK;
}

