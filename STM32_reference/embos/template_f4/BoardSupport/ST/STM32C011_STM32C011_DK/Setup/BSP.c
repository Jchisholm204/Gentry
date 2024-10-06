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
Purpose : BSP for ST STM32C011 Discovery Kit
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT         (6)

#define GPIOB_BASE_ADDR  (0x50000400u)
#define GPIOB_MODER      (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x00u))
#define GPIOB_IDR        (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x10u))
#define GPIOB_BSRR       (*(volatile unsigned int*)(GPIOB_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR    (0x40021000u)
#define RCC_IOPENR       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x34u))
#define GPIOBEN_BIT      (1)

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
  RCC_IOPENR  |=  (1u << GPIOBEN_BIT);           // Enable clock to Port B
  GPIOB_MODER &= ~(1u << (LED0_BIT << 1) << 1);  // Alternate function PB6
  GPIOB_BSRR   =  (1u << (LED0_BIT + 16));       // Initially clear LD3
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= (1u << LED0_BIT);           // Switch LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_BSRR |= (1u << (LED0_BIT + 16));    // Switch LED off
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
  }
}

/*************************** End of file ****************************/
