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
Purpose : BSP for LPC1788 (FDI uEZGUI 1788 70WVE)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define FIO_BASE_ADDR              (0x20098000)

#define FIO1DIR1                   (*(volatile unsigned char*)(FIO_BASE_ADDR + 0x21))
#define FIO1SET1                   (*(volatile unsigned char*)(FIO_BASE_ADDR + 0x39))
#define FIO1CLR1                   (*(volatile unsigned char*)(FIO_BASE_ADDR + 0x3D))
#define FIO1PIN1                   (*(volatile unsigned char*)(FIO_BASE_ADDR + 0x35))

#define LED0_BIT                   5  // P1.13

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
  FIO1DIR1 |= (1 << LED0_BIT);  // Set to output for heartbeat LED
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    FIO1SET1 = (1 << LED0_BIT);   // Switch on LED
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    FIO1CLR1 = (1 << LED0_BIT);   // Switch off LED
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    FIO1PIN1 ^= (1 << LED0_BIT);  // Toggle LED
  }
}

/****** End Of File *************************************************/
