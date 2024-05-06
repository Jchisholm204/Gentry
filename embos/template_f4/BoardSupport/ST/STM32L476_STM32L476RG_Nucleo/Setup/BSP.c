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
Purpose : BSP for ST STM32L476 and L452 Nucleo boards
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT                  (5)

#define GPIOA_BASE_ADDR           (0x48000000u)
#define GPIOA_MODER               (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))
#define GPIOA_IDR                 (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x10u))
#define GPIOA_BSRR                (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR             (0x40021000u)
#define RCC_AHB2ENR               (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x4Cu))
#define RCC_GPIOAEN_BIT           (0)

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
  RCC_AHB2ENR |=  (1u << RCC_GPIOAEN_BIT);    // Enable Clock for port A
  GPIOA_MODER &= ~(3u << (LED0_BIT << 1));    // Reset mode; sets port to input
  GPIOA_MODER |=  (1u << (LED0_BIT << 1));    // Set to output mode
  GPIOA_BSRR   =  (16u << LED0_BIT);          // Switch LED off
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOA_BSRR |= (1u << (LED0_BIT));         // Switch LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOA_BSRR |= (1u << (LED0_BIT + 16));    // Switch LED off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (GPIOA_IDR & (1u << LED0_BIT)) {
      GPIOA_BSRR |= (1u << (LED0_BIT + 16));  // Switch LED off
    } else {
      GPIOA_BSRR |= (1u << LED0_BIT);         // Switch LED on
    }
  }
}

/****** End Of File *************************************************/
