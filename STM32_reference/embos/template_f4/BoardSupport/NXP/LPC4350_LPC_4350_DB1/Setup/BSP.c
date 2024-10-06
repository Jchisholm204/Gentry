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
Purpose : BSP for LPC-4350-DB1 Development (Evaluation) Board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_MASK     (1uL << 28)
#define LED1_MASK     (1uL << 26)
#define LED_MASK_ALL  (LED0_MASK | LED1_MASK)

#define LPC_SCU_BASE  0x40086000
#define SCU_SFSPD_12  (*(volatile unsigned int *)(LPC_SCU_BASE + 0x06B0))  //  Pin configuration register for pin PD_12
#define SCU_SFSPD_14  (*(volatile unsigned int *)(LPC_SCU_BASE + 0x06B8))  //  Pin configuration register for pin PD_14

#define LPC_GPIO_BASE 0x400F4000
#define GPIO_DIR6     (*(volatile unsigned int *)(LPC_GPIO_BASE + 0x2018))  //  Pin output register
#define GPIO_SET6     (*(volatile unsigned int *)(LPC_GPIO_BASE + 0x2218))  //  Pin set register
#define GPIO_CLR6     (*(volatile unsigned int *)(LPC_GPIO_BASE + 0x2298))  //  Pin clear register
#define GPIO_NOT6     (*(volatile unsigned int *)(LPC_GPIO_BASE + 0x2318))  //  Pin toggle register

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
  // Init LEDs
  //
  SCU_SFSPD_12 = 0x0C;
  SCU_SFSPD_14 = 0x0C;
  GPIO_DIR6 |= LED_MASK_ALL;
  //
  // All LEDs off
  //
  GPIO_CLR6 |= LED_MASK_ALL;
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIO_SET6 |= LED0_MASK;
  } else if (Index == 1) {
    GPIO_SET6 |= LED1_MASK;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIO_CLR6 |= LED0_MASK;
  } else if (Index == 1) {
    GPIO_CLR6 |= LED1_MASK;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIO_NOT6 |= LED0_MASK;
  } else if (Index == 1) {
    GPIO_NOT6 |= LED1_MASK;
  }
}

/****** End Of File *************************************************/
