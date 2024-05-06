/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2023 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system                           *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.18.2.0                                        *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for the ST STM32F429_Nucleo board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCC_BASE_ADDR    (0x40023800u)
#define RCC_AHBENR       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))
#define RCC_LEDPORT_BIT  (1)

#define GPIOB_BASE_ADDR  (0x40020400u)
#define GPIOB_MODER      (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_ODR        (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x14u))

#define LED0_BIT         ( 0)  // LED1 (green) - PB0
#define LED1_BIT         ( 7)  // LED2 (blue)  - PB7
#define LED2_BIT         (14)  // LED4 (red)   - PB14

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  //
  // Initialize port for LEDs (sample application)
  //
  RCC_AHBENR  |=  (1u << RCC_LEDPORT_BIT);

  GPIOB_MODER &= ~(3u << (LED0_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED0_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED0_BIT);       // Initially clear LEDs

  GPIOB_MODER &= ~(3u << (LED1_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED1_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED1_BIT);       // Initially clear LEDs

  GPIOB_MODER &= ~(3u << (LED2_BIT * 2));  // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED2_BIT * 2));  // Set to output mode
  GPIOB_ODR   &= ~(1u <<  LED2_BIT);       // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR |= (1u << LED0_BIT);         // Switch on LED0
  } else if (Index == 1) {
    GPIOB_ODR |= (1u << LED1_BIT);         // Switch on LED1
  } else if (Index == 2) {
    GPIOB_ODR |= (1u << LED2_BIT);         // Switch on LED2
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR &= ~(1u << LED0_BIT);        // Switch off LED0
  } else if (Index == 1) {
    GPIOB_ODR &= ~(1u << LED1_BIT);        // Switch off LED1
  } else if (Index == 2) {
    GPIOB_ODR &= ~(1u << LED2_BIT);        // Switch off LED2
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOB_ODR ^= (1u << LED0_BIT);         // Toggle LED0
  } else if (Index == 1) {
    GPIOB_ODR ^= (1u << LED1_BIT);         // Toggle LED1
  } else if (Index == 2) {
    GPIOB_ODR ^= (1u << LED2_BIT);         // Toggle LED2
  }
}

/****** End Of File *************************************************/
