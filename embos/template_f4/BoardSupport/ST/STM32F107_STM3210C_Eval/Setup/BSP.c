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
Purpose : BSP for STM32F107 (ST MB784)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define GPIO_PD_BASE_ADDR         ((unsigned int)0x40011400)
#define GPIOD_CRL                 (*(volatile unsigned int*)(GPIO_PD_BASE_ADDR))
#define GPIOD_CRH                 (*(volatile unsigned int*)(GPIO_PD_BASE_ADDR + 0x04))
#define GPIOD_ODR                 (*(volatile unsigned int*)(GPIO_PD_BASE_ADDR + 0x0C))
#define GPIOD_BSRR                (*(volatile unsigned int*)(GPIO_PD_BASE_ADDR + 0x10))
#define GPIOD_BRR                 (*(volatile unsigned int*)(GPIO_PD_BASE_ADDR + 0x14))

/****** SFRs used for LED-Port **************************************/
#define SYSCTRL_RCC_APB2          (*(volatile unsigned int*)(0x40021018))
#define SYSCTRL_LEDPORT_BIT       (5)

/****** Assign LEDs to Ports ****************************************/
#define LED0_BIT     (3)
#define LED1_BIT     (4)
#define LED2_BIT     (13)
#define LED_MASK_ALL ((1uL << LED0_BIT) | (1uL << LED1_BIT) | (1uL << LED2_BIT))

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
  SYSCTRL_RCC_APB2 |= (1 << SYSCTRL_LEDPORT_BIT);
  GPIOD_CRL = (GPIOD_CRL & 0xFFF00FFFuL) | 0x00033000uL;
  GPIOD_CRH = (GPIOD_CRH & 0xFF0FFFFFuL) | 0x00300000uL;
  GPIOD_BRR |= LED_MASK_ALL;   // Initially switch off LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  GPIOD_BSRR |= (1uL << (LED0_BIT + Index));             // Switch on LED
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  GPIOD_BRR  |= (1uL << (LED0_BIT + Index));             // Switch off LED
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if ((GPIOD_ODR & (1uL << (LED0_BIT + Index))) == 0) {  // LED is switched off
    BSP_SetLED(Index);                                   // Switch on LED
  } else {
    BSP_ClrLED(Index);                                   // Switch off LED
  }
}

/****** End Of File *************************************************/
