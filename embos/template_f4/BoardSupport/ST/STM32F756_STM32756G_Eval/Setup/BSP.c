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
Purpose : BSP for STM32756G eval board Rev.B
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCC_BASE_ADDR             (0x40023800u)
#define RCC_AHB1RSTR              (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x10u))
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))
#define RCC_LEDPORT_MASK          ((1u << 5u) | (1u << 1))

#define GPIOB_BASE_ADDR           (0x40020400u)
#define GPIOB_MODER               (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_ODR                 (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x14u))

#define GPIOF_BASE_ADDR           (0x40021400u)
#define GPIOF_MODER               (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x00u))
#define GPIOF_ODR                 (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x14u))

#define LED1_BIT                  (10)  // LED1 - PF10
#define LED3_BIT                  ( 7)  // LED3 - PB7

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

  GPIOF_MODER  &= ~(3u << (LED1_BIT * 2));   // Reset mode; sets port to input
  GPIOF_MODER  |=  (1u << (LED1_BIT * 2));   // Set to output mode
  GPIOF_ODR    |=  (1u << LED1_BIT);         // Initially clear LEDs, low active

  GPIOB_MODER  &= ~(3u << (LED3_BIT * 2));   // Reset mode; sets port to input
  GPIOB_MODER  |=  (1u << (LED3_BIT * 2));   // Set to output mode
  GPIOB_ODR    |=  (1u << LED3_BIT);         // Initially clear LEDs, low active
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOF_ODR &= ~(1u << LED1_BIT);          // Switch on LED1
  } else if (Index == 1) {
    GPIOB_ODR &= ~(1u << LED3_BIT);          // Switch on LED3
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOF_ODR |= (1u << LED1_BIT);           // Switch off LED1
  } else if (Index == 1) {
    GPIOB_ODR |= (1u << LED3_BIT);           // Switch off LED3
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOF_ODR ^= (1u << LED1_BIT);           // Toggle LED1
  } else if (Index == 1) {
    GPIOB_ODR ^= (1u << LED3_BIT);           // Toggle LED3
  }
}

/****** End Of File *************************************************/
