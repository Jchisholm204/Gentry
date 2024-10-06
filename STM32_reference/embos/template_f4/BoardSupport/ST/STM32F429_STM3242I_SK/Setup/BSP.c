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
Purpose : BSP for the IAR STM32F429 SK eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCC_BASE_ADDR     (0x40023800u)
#define RCC_AHB1RSTR      (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x10u))
#define RCC_AHBENR        (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))
#define RCC_LEDPORT_MASK  ((1u << 0) | (1u << 6))

#define GPIOA_BASE_ADDR   (0x40020000u)
#define GPIOA_MODER       (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))
#define GPIOA_ODR         (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x14u))

#define GPIOG_BASE_ADDR   (0x40021800u)
#define GPIOG_MODER       (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x00u))
#define GPIOG_ODR         (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x14u))

#define LED0_BIT          (4)  // LED1 - PA4
#define LED1_BIT          (3)  // LED2 - PG3

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
  RCC_AHBENR   &= ~RCC_LEDPORT_MASK;
  RCC_AHB1RSTR &= ~RCC_LEDPORT_MASK;
  RCC_AHBENR   |=  RCC_LEDPORT_MASK;

  GPIOA_MODER &= ~(3u << (LED0_BIT * 2));  // Reset mode; sets port to input
  GPIOA_MODER |=  (1u << (LED0_BIT * 2));  // Set to output mode
  GPIOA_ODR   &= ~(1u << LED0_BIT);        // Initially clear LEDs, low active

  GPIOG_MODER &= ~(3u << (LED1_BIT * 2));  // Reset mode; sets port to input
  GPIOG_MODER |=  (1u << (LED1_BIT * 2));  // Set to output mode
  GPIOG_ODR   &= ~(1u << LED1_BIT);        // Initially clear LEDs, low active
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOA_ODR &= ~(1u << LED0_BIT);        // Switch on LED0
  } else if (Index == 1) {
    GPIOG_ODR &= ~(1u << LED1_BIT);        // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOA_ODR |= (1u << LED0_BIT);         // Switch off LED0
  } else if (Index == 1) {
    GPIOG_ODR |= (1u << LED1_BIT);         // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOA_ODR ^= (1u << LED0_BIT);         // Toggle LED0
  } else if (Index == 1) {
    GPIOG_ODR ^= (1u << LED1_BIT);         // Toggle LED1
  }
}

/****** End Of File *************************************************/
