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
Purpose : BSP for the ST STM32F767 EVAL board
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
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30u))
#define RCC_LEDPORT_BIT           (9)

#define GPIOJ_BASE_ADDR           (0x40022400u)
#define GPIOJ_MODER               (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x00u))
#define GPIOJ_ODR                 (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x14u))

#define LED0_BIT                  (0)  // LED1 (orange) - PJ0
#define LED1_BIT                  (1)  // LED2 (red) - PJ1

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
  // Initialize clock and port pins for LEDs
  //
  RCC_AHBENR   |= (1u << RCC_LEDPORT_BIT);

  GPIOJ_MODER &= ~(3u << (LED0_BIT * 2));   // Reset mode; sets port to input
  GPIOJ_MODER |=  (1u << (LED0_BIT * 2));   // Set to output mode
  GPIOJ_ODR   |=  (1u << LED0_BIT);         // Initially clear LEDs, low active

  GPIOJ_MODER &= ~(3u << (LED1_BIT * 2));   // Reset mode; sets port to input
  GPIOJ_MODER |=  (1u << (LED1_BIT * 2));   // Set to output mode
  GPIOJ_ODR   |=  (1u << LED1_BIT);         // Initially clear LEDs, low active
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOJ_ODR &= ~(1u << LED0_BIT);         // Switch on LED0
  } else if (Index == 1) {
    GPIOJ_ODR &= ~(1u << LED1_BIT);         // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOJ_ODR |= (1u << LED0_BIT);          // Switch off LED0
  } else if (Index == 1) {
    GPIOJ_ODR |= (1u << LED1_BIT);          // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOJ_ODR ^= (1u << LED0_BIT);          // Toggle LED0
  } else if (Index == 1) {
    GPIOJ_ODR ^= (1u << LED1_BIT);          // Toggle LED1
  }
}

/****** End Of File *************************************************/
