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
Purpose : BSP for IAR STM32F051R8-SK eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT                  (6)
#define LED1_BIT                  (7)
#define LED_MASK_ALL              ((1uL << LED0_BIT) | (1uL << LED1_BIT))

#define GPIOC_BASE_ADDR           (0x48000800)
#define GPIOC_MODER               (*(volatile unsigned int*)(GPIOC_BASE_ADDR + 0x00))
#define GPIOC_IDR                 (*(volatile unsigned int*)(GPIOC_BASE_ADDR + 0x10))
#define GPIOC_ODR                 (*(volatile unsigned int*)(GPIOC_BASE_ADDR + 0x14))
#define GPIOC_BSRR                (*(volatile unsigned int*)(GPIOC_BASE_ADDR + 0x18))

#define RCC_AHBENR                (*(volatile unsigned int*)(0x40021014))
#define IOPCEN_BIT                (19)

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
  RCC_AHBENR  |= (1uL << IOPCEN_BIT);
  GPIOC_MODER |= (1uL << (LED0_BIT << 1));
  GPIOC_MODER |= (1uL << (LED1_BIT << 1));
  GPIOC_ODR   |= LED_MASK_ALL;
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOC_BSRR |= (1uL << (LED0_BIT + 16));   /* Switch LED on */
  } else if (Index == 1) {
    GPIOC_BSRR |= (1uL << (LED1_BIT + 16));   /* Switch LED on */
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOC_BSRR |= (1uL << LED0_BIT);          /* Switch LED off */
  } else if (Index == 1) {
    GPIOC_BSRR |= (1uL << LED1_BIT);          /* Switch LED off */
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (GPIOC_IDR & (1uL << LED0_BIT)) {
      BSP_SetLED(Index);
    } else {
      BSP_ClrLED(Index);
    }
  } else if (Index == 1) {
    if (GPIOC_IDR & (1uL << LED1_BIT)) {
      BSP_SetLED(Index);
    } else {
      BSP_ClrLED(Index);
    }
  }
}

/****** End Of File *************************************************/
