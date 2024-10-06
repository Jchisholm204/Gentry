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

-------------------------- END-OF-HEADER -----------------------------
File    : BSP.c
Purpose : BSP for ST STM32U545RE-Q Nucleo
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT           (5) // LD2, Green, PA5, low active

#define RCC_BASE_ADDR      (0x46020C00u)
#define RCC_AHB2ENR1       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x8Cu))

#define GPIOA_BASE_ADDR    (0x42020000u)
#define GPIOA_MODER        (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))  // GPIOA port mode register
#define GPIOA_IDR          (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x10u))  // GPIOA input data register
#define GPIOA_BSRR         (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))  // GPIOA bit set/reset register

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
  RCC_AHB2ENR1 |=  (1u << 0);                 // Enable GPIOA clock
  GPIOA_MODER  &= ~(3u << (LED0_BIT * 2));    // Clear mode bits
  GPIOA_MODER  |=  (1u << (LED0_BIT * 2));    // Set mode to output
  GPIOA_BSRR   |=  (1u << (LED0_BIT + 16u));  // Initially clear LED
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOA_BSRR |= (1u << (LED0_BIT));
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOA_BSRR |= (1u << (LED0_BIT + 16u));
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (GPIOA_IDR & (1u << LED0_BIT)) {
      GPIOA_BSRR |= (1u << (LED0_BIT + 16u));
    } else {
      GPIOA_BSRR |= (1u << LED0_BIT);
    }
  }
}

/*************************** End of file ****************************/
