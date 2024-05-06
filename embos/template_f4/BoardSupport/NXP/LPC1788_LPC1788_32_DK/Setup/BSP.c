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
Purpose : BSP for Embedded Artists LPC1788_EA eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"
#include "LPC177x_8x.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define NUM_LEDS       2
#define LED0_BIT      26
#define LED1_BIT      27
#define LED_MASK_ALL  ((1 << LED0_BIT) | (1 << LED1_BIT))

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
  LPC_IOCON->P2_26  = 0;             // GPIO (LED0)
  LPC_IOCON->P2_27  = 0;             // GPIO (LED1)
  LPC_GPIO2->DIR   |= LED_MASK_ALL;  // Set to output
  //
  // All LEDs off
  //
  LPC_GPIO2->SET    = LED_MASK_ALL;  // Initially clear leds (active low)
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  LPC_GPIO2->CLR = (1 << (LED0_BIT + Index));  // Active low
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  LPC_GPIO2->SET = (1 << (LED0_BIT + Index));  // Active low
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index >= NUM_LEDS) {
    return;
  }
  if (LPC_GPIO2->PIN & (1 << (LED0_BIT + Index))) {
    BSP_SetLED(Index);
  } else {
    BSP_ClrLED(Index);
  }
}

/****** End Of File *************************************************/
