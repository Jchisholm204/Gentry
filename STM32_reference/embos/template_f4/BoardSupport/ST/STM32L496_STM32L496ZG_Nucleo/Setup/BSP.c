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
Purpose : BSP for ST STM32L496ZG Nucleo and STM32L4A6ZG Nucleo boards.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

// LD2 PB07
// LD3 PB14

#define LED0_BIT                  ( 7)
#define LED1_BIT                  (14)

#define GPIOB_BASE_ADDR           (0x48000400u)
#define GPIOB_MODER               (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_IDR                 (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x10u))
#define GPIOB_BSRR                (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR             (0x40021000u)
#define RCC_AHB2ENR               (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x4Cu))
#define RCC_GPIOBEN_BIT           (1)

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
  RCC_AHB2ENR |=  (1u << RCC_GPIOBEN_BIT);    // Enable Clock for port B
  GPIOB_MODER &= ~(3u << (LED0_BIT << 1));    // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED0_BIT << 1));    // Set to output mode
  GPIOB_BSRR   =  (16u << LED0_BIT);          // Switch LED off
  GPIOB_MODER &= ~(3u << (LED1_BIT << 1));    // Reset mode; sets port to input
  GPIOB_MODER |=  (1u << (LED1_BIT << 1));    // Set to output mode
  GPIOB_BSRR   =  (16u << LED1_BIT);          // Switch LED off
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= (1u << (LED0_BIT));         // Switch LED on
  } else if (Index == 1) {
    GPIOB_BSRR |= (1u << (LED1_BIT));         // Switch LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= (1u << (LED0_BIT + 16));    // Switch LED off
  } else if (Index == 1) {
    GPIOB_BSRR |= (1u << (LED1_BIT + 16));    // Switch LED off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (GPIOB_IDR & (1u << LED0_BIT)) {
      GPIOB_BSRR |= (1u << (LED0_BIT + 16));  // Switch LED off
    } else {
      GPIOB_BSRR |= (1u << LED0_BIT);         // Switch LED on
    }
  } else if (Index == 1) {
    if (GPIOB_IDR & (1u << LED1_BIT)) {
      GPIOB_BSRR |= (1u << (LED1_BIT + 16));  // Switch LED off
    } else {
      GPIOB_BSRR |= (1u << LED1_BIT);         // Switch LED on
    }
  }
}

/****** End Of File *************************************************/
