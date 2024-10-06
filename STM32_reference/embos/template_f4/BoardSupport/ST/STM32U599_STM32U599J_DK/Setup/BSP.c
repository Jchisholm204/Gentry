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
Purpose : BSP for ST STM32U599J-DK
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define LED0_BIT           (0)     // LD3, Green, PE0
#define LED1_BIT           (1)     // LD4, Red,  PE1

#define RCC_BASE_ADDR      (0x46020C00u)
#define RCC_AHB2ENR1       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x8Cu))

#define GPIOE_BASE_ADDR    (0x42021000u)
#define GPIOE_MODER        (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x00u))  // GPIOB port mode register
#define GPIOE_IDR          (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x10u))  // GPIOB input data register
#define GPIOE_BSRR         (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x18u))  // GPIOB bit set/reset register

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
  RCC_AHB2ENR1 |=  (1u << 4);               // Enable GPIOE clock
  GPIOE_MODER  &= ~(3u << (LED0_BIT * 2))   // Clear mode bits
               &  ~(3u << (LED1_BIT * 2));
  GPIOE_MODER  |=  (1u << (LED0_BIT * 2))   // Set mode to output
               |   (1u << (LED1_BIT * 2));
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  switch (Index) {
  case 0:
    GPIOE_BSRR |= (1u << (LED0_BIT));
    break;
  case 1:
    GPIOE_BSRR |= (1u << (LED1_BIT));
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  switch (Index) {
  case 0:
    GPIOE_BSRR |= (1u << (LED0_BIT + 16u));
    break;
  case 1:
    GPIOE_BSRR |= (1u << (LED1_BIT + 16u));
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  switch (Index) {
  case 0:
    if (GPIOE_IDR & (1u << LED0_BIT)) {
      GPIOE_BSRR |= (1u << (LED0_BIT + 16u));
    } else {
      GPIOE_BSRR |= (1u << LED0_BIT);
    }
    break;
  case 1:
    if (GPIOE_IDR & (1u << LED1_BIT)) {
      GPIOE_BSRR |= (1u << (LED1_BIT + 16u));
    } else {
      GPIOE_BSRR |= (1u << LED1_BIT);
    }
    break;
  default:
    break;
  }
}

/*************************** End of file ****************************/
