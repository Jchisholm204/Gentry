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
Purpose : BSP for STM32F100RB (STM32 Discovery evalboard)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT  (8)
#define LED1_BIT  (9)
#define LED_MASK_ALL ((1uL << LED0_BIT) | (1uL << LED1_BIT))

#define GPIOC_BASE_ADDR           (0x40011000)
#define LED_PORT_BASE_ADDR        (GPIOC_BASE_ADDR)
#define GPIOC_CRH                 (*(volatile unsigned int*)(LED_PORT_BASE_ADDR + 0x04))
#define GPIOC_ODR                 (*(volatile unsigned int*)(LED_PORT_BASE_ADDR + 0x0C))
#define GPIOC_BSRR                (*(volatile unsigned int*)(LED_PORT_BASE_ADDR + 0x10))
#define GPIOC_BRR                 (*(volatile unsigned int*)(LED_PORT_BASE_ADDR + 0x14))
#define SYSCTRL_RCC_APB2          (*(volatile unsigned int*)(0x40021018))
#define SYSCTRL_LEDPORT_BIT       (4)

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
  SYSCTRL_RCC_APB2 |= (1uL << SYSCTRL_LEDPORT_BIT);
  GPIOC_CRH  = (GPIOC_CRH & (0xFFFFF00uL)) | 0x00000033uL;
  GPIOC_BRR |= LED_MASK_ALL;   // Initially switch off all LEDS
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  GPIOC_BSRR |= (1uL << (LED0_BIT + Index));             /* Switch LED on  */
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  GPIOC_BRR  |= (1uL << (LED0_BIT + Index));             /* Switch LED off */
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if ((GPIOC_ODR & (1uL << (LED0_BIT + Index))) == 0) {  /* LED is switched off */
    BSP_SetLED(Index);                                   /* Switch LED on  */
  } else {
    BSP_ClrLED(Index);                                   /* Switch LED off */
  }
}

/****** End Of File *************************************************/
