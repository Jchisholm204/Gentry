/**
 * @file payloadv1.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Pin Mappings for the QSET Payload Control Board
 * @version 0.1
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PINS_PAYLOADV1_H_
#define _PINS_PAYLOADV1_H_
#include "hal/pin.h"
#define _BOARD_CONFIG_

#define PIN_LED1      PIN('C', 13)  // Red
#define PIN_LED2      PIN('C', 14)  // Green


// USART/UART Pins
#define PIN_USART3_TX PIN('C', 10)  
#define PIN_USART3_RX PIN('C', 11)

#define PIN_USART5_TX PIN('C', 12) 
#define PIN_USART5_RX PIN('D', 2)


// USB Pins
#define PIN_USB_VBUS  PIN('A', 9)
#ifndef USBD_VBUS_DETECT
#define USBD_VBUS_DETECT
#endif
#define PIN_USB_DM    PIN('A', 11)
#define PIN_USB_DP    PIN('A', 12)


// Stepper Motors
#define PIN_USART4_TX PIN('A', 0)   // Stepper Motor 1
#define PIN_USART4_RX PIN('A', 1)

#define PIN_USART2_TX PIN('A', 2)   // Stepper Motor 2
#define PIN_USART2_RX PIN('A', 3)

// The following are tied to both Stepper Motors
#define PIN_STEP      PIN('A', 3)   
#define PIN_DIR       PIN('A', 3)
#define PIN_SM_EN     PIN('C', 8)   // Active High
#define PIN_MS1       PIN('B', 12)  // Used for setting step resolution
#define PIN_MS2       PIN('C', 15)


// DC Motor Control
// H-Bridge PWM Pairs, (##) Represents number and direction i.e 12 is Motor 1 backwards
#define PIN_M11P      PIN('C', 6)   // TIM8_CH1
#define PIN_M11N      PIN('A', 5)   // TIM8_CH1N

#define PIN_M12P      PIN('A', 8)   // TIM1_CH1
#define PIN_M12N      PIN('B', 13)  // TIM1_CH1N

#define PIN_M21P      PIN('A', 10)  // TIM1_CH3
#define PIN_M21N      PIN('B', 15)  // TIM1_CH3N

#define PIN_M22P      PIN('C', 7)   // TIM8_CH2
#define PIN_M22N      PIN('B', 14)  // TIM8_CH2N


// Servos
#define PIN_SERVO_EN  PIN('C', 9)   // Active High, for level shifter
#define PIN_SERVO_1   PIN('A', 15)  // TIM2_CH1
#define PIN_SERVO_2   PIN('B', 4)   // TIM3_CH1
#define PIN_SERVO_3   PIN('B', 5)   // TIM3_CH2
#define PIN_SERVO_4   PIN('B', 6)   // TIM4_CH1
#define PIN_SERVO_5   PIN('B', 9)   // TIM2_CH2
#define PIN_SERVO_6   PIN('B', 2)   // TIM2_CH4
#define PIN_SERVO_7   PIN('B', 0)   // TIM3_CH3
#define PIN_SERVO_8   PIN('B', 1)   // TIM3_CH4


// Lights Control (Digital Pot)
#define PIN_I2C2_SCL  PIN('B', 10) 
#define PIN_I2C2_SDA  PIN('B', 3)

// Light Selection (Binary Selection i.e 0011 is light 3)
#define PIN_LS0       PIN('C', 0)
#define PIN_LS1       PIN('C', 1)
#define PIN_LS2       PIN('C', 2)
#define PIN_LS3       PIN('A', 4)

// Sensors I2C
#define PIN_I2C1_SCL  PIN('B', 8) 
#define PIN_I2C1_SDA  PIN('B', 7)

// Fan
#define PIN_FAN_PWM   PIN('A', 7)   // TIM14_CH1

// Thermistor ADC
#define PIN_THERM_ADC PIN('C', 3)   // ADC3_IN13


#endif



