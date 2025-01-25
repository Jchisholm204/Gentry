/**
 * @file AKMotor_errors.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2025-01-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _AKMOTOR_ERRORS_H_
#define _AKMOTOR_ERRORS_H_

enum eAKErrors {
    eAKERR_NONE,
    eAKerr_OVERVOLTAGE,
    eAKerr_UNDERVOLTAGE,
    eAKerr_DRV,
    eAKerr_OVERCURRENT,
    eAKerr_TEMP_FET,
    eAKerr_TEMP_MOTOR,
    eAKerr_GATE_OVERVOLTAGE,
    eAKerr_GATE_UNDERVOLTAGE,
    eAKerr_MCU_UNDERVOLTAGE,
    eAKerr_WATCHDOG,
    eAKerr_ENCODER_SPI,
    eAKerr_ENCODER_OUTRNG_HI,
    eAKerr_ENCODER_OUTRNG_LO,
    eAKerr_flashCorrupt,
    eAKerr_currentSense1,
    eAKerr_currentSense2,
    eAKerr_currentSense3,
    eAKerr_currentUnbalanced,
};

#ifdef AKMOTOR_USE_STR
const char *mc_fault_code[19]{
    "AK_MOTOR_OK",
    "AK_MOTOR_OVER_VOLTAGE",                        // OVER VOLTAGE
    "AK_MOTOR_UNDER_VOLTAGE",                       // UNDER_VOLTAGE
    "AK_MOTOR_DRV",                                 // DRIVE FAULT
    "AK_MOTOR_ABS_OVER_CURRENT",                    // OVER_CURRENT
    "AK_MOTOR_OVER_TEMP_FET",                       // MOS OVER TEMPERATURE
    "AK_MOTOR_OVER_TEMP_MOTOR",                     // MOS OVER TEMPERATURE
    "AK_MOTOR_GATE_DRIVER_OVER_VOLTAGE",            // DRIVER_OVER_VOLTAGE
    "AK_MOTOR_GATE_DRIVER_UNDER_VOLTAGE",           // DRIVER UNDER VOLTAGE
    "AK_MOTOR_MCU_UNDER_VOLTAGE",                   // MCU UNDRE VOLTAGE
    "AK_MOTOR_BOOTING_FROM_WATCHDOG_RESET",         // UNDREVOLTAGE
    "AK_MOTOR_ENCODER_SPI",                         // SPI ENCODER FAULT
    "AK_MOTOR_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE",  // Encoder overrun
    "AK_MOTOR_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE",  // Encoder overrun
    "AK_MOTOR_FLASH_CORRUPTION",                    // FLASH FAULT
    "AK_MOTOR_HIGH_OFFSET_CURRENT_SENSOR_1",        // Current sampling channel 1 fault
    "AK_MOTOR_HIGH_OFFSET_CURRENT_SENSOR_2",        // Current sampling channel 2 fault
    "AK_MOTOR_HIGH_OFFSET_CURRENT_SENSOR_3",        // Current sampling channel 1 fault
    "AK_MOTOR_UNBALANCED_CURRENTS"                  // current unbalance
};

#endif

#endif

