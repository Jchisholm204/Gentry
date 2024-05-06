/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
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
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : BSP for NXP iMX6U5 and Variscite eval board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define GPIO5_DR    (*((volatile unsigned long *)0x020AC000))
#define GPIO5_GDIR  (*((volatile unsigned long *)0x020AC004))
#define NUM_LEDS    (2)

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
  GPIO5_DR   &= ~(3u << 15);
  GPIO5_GDIR |=  (3u << 15);
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIO5_DR |= (1u << (15 + Index));
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIO5_DR &= ~(1u << (15 + Index));
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < NUM_LEDS) {
    GPIO5_DR ^= (1u << (15 + Index));
  }
}

/*************************** End of file ****************************/
