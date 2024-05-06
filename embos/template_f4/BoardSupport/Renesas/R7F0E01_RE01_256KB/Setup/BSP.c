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
Purpose : BSP for Renesas RE01 256KB evalboard
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0          (1u << 10)  // Orange, P210 Pin32, low active
#define LED1          (1u << 10)  // Red,    P410 Pin17, low active
#define PORT2_PCNTR1  (*(volatile unsigned int*)(0x40040040u))
#define PORT4_PCNTR1  (*(volatile unsigned int*)(0x40040080u))

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
  PORT2_PCNTR1 &= ~(LED0 << 16);  // Initially turn LED0 off
  PORT2_PCNTR1 |= LED0;           // Set pin to output
  PORT4_PCNTR1 &= ~(LED1 << 16);  // Initially turn LED1 off
  PORT4_PCNTR1 |= LED1;           // Set pin to output
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    PORT2_PCNTR1 |= (LED0 << 16);
  } else {
    if (Index == 1) {
      PORT4_PCNTR1 |= (LED1 << 16);
    }
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    PORT2_PCNTR1 &= ~(LED0 << 16);
  } else {
    if (Index == 1) {
       PORT4_PCNTR1 &= ~(LED1 << 16);
    }
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if ((PORT2_PCNTR1 & (LED0 << 16)) == 0) {
      PORT2_PCNTR1 |= (LED0 << 16);
    } else {
      PORT2_PCNTR1 &= ~(LED0 << 16);
    }
  } else {
    if (Index == 1) {
      if ((PORT4_PCNTR1 & (LED1 << 16)) == 0) {
        PORT4_PCNTR1 |= (LED1 << 16);
      } else {
        PORT4_PCNTR1 &= ~(LED1 << 16);
      }
    }
  }
}

/*************************** End of file ****************************/
