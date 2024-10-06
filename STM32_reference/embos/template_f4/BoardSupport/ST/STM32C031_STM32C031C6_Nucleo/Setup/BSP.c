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
Purpose : BSP for ST STM32C031C6 Nucleo board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT         (5)

#define GPIOA_BASE_ADDR  (0x50000000u)
#define GPIOA_MODER      (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))
#define GPIOA_IDR        (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x10u))
#define GPIOA_BSRR       (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR    (0x40021000u)
#define RCC_IOPENR       (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x34u))
#define GPIOAEN_BIT      (0)

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
  RCC_IOPENR  |=  (1u << GPIOAEN_BIT);           // Enable clock to Port A
  GPIOA_MODER &= ~(1u << (LED0_BIT << 1) << 1);  // Alternate function PA5
  GPIOA_BSRR   =  (1u << (LED0_BIT + 16));       // Initially clear LD3
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOA_BSRR |= (1u << LED0_BIT);           // Switch LED on
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

/*************************** End of file ****************************/
