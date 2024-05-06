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
Purpose : BSP for STM32F303C-Eval eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define GPIOE_BASE_ADDR           (0x48001000u)
#define GPIOE_MODER               (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x00u))
#define GPIOE_ODR                 (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x14u))

#define RCC_BASE_ADDR             (0x40021000u)
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x14u))

#define NUM_LEDS                  (4)
#define LED0_BIT                  (8u)
#define LED1_BIT                  (9u)
#define LED2_BIT                  (10u)
#define LED3_BIT                  (11u)
#define LED_MASK_ALL              ((1uL << LED0_BIT) | (1uL << LED1_BIT)  | (1uL << LED2_BIT)  | (1uL << LED3_BIT))

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
  RCC_AHBENR |= (1u << 21u);  // Enable clock for PortE

  GPIOE_MODER &= ~((3u << (LED0_BIT * 2u)) | (3u << (LED1_BIT * 2u)) | (3u << (LED2_BIT * 2u)) | (3u << (LED3_BIT * 2u))); // Reset mode, sets port to input
  GPIOE_MODER |=  ((1u << (LED0_BIT * 2u)) | (1u << (LED1_BIT * 2u)) | (1u << (LED2_BIT * 2u)) | (1u << (LED3_BIT * 2u))); // Set to output mode
  GPIOE_ODR   |=  LED_MASK_ALL;    // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIOE_ODR &= ~(1uL << (LED0_BIT + Index));            // Switch Led on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIOE_ODR |= (1uL << (LED0_BIT + Index));            // Switch Led off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIOE_ODR ^= (1uL << (LED0_BIT + Index));            // Toggle Led
  }
}

/****** End Of File *************************************************/
