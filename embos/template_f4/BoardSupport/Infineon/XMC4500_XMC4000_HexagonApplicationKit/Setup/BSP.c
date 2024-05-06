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
Purpose : BSP for Hitex XMC4500 eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define P3_BASE_ADDR  0x48028300
#define P3_OUT        (*(volatile unsigned int*)(P3_BASE_ADDR + 0x00))
#define P3_OMR        (*(volatile unsigned int*)(P3_BASE_ADDR + 0x04))
#define P3_IOCR8      (*(volatile unsigned int*)(P3_BASE_ADDR + 0x18))

#define LED0          9

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
  P3_IOCR8 &= (unsigned int)~0x0000FF00;
  P3_IOCR8 |= 0x00008000;
  P3_OUT   &= ~(1ul << LED0);
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    P3_OUT |= (1uL << LED0);
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    P3_OUT &= ~(1uL << LED0);
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    P3_OMR |= (1uL << LED0) | (1uL << (LED0 + 16));
  }
}

/****** End Of File *************************************************/
