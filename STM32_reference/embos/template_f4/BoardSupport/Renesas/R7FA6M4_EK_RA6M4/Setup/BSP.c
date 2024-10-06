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
Purpose : BSP for the Renesas EK-RA6M4 eval board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED1  (15)  // User LED1, Blue, P415
#define LED2  ( 4)  // User LED2, Green, P404
#define LED3  ( 0)  // User LED3, Red, P400

#define PORT4_BASE_ADDR    (0x40080000u)
#define PORT4_PCNTR1_PODR  (*(volatile unsigned short*)(PORT4_BASE_ADDR + (0x20u * 4) + 0x00u))  // Control Register 1 Output Data
#define PORT4_PCNTR1_PDR   (*(volatile unsigned short*)(PORT4_BASE_ADDR + (0x20u * 4) + 0x02u))  // Control Register 1 Direction
#define PORT4_PCNTR3_PORR  (*(volatile unsigned short*)(PORT4_BASE_ADDR + (0x20u * 4) + 0x08u))  // Control Register 3 Output Reset
#define PORT4_PCNTR3_POSR  (*(volatile unsigned short*)(PORT4_BASE_ADDR + (0x20u * 4) + 0x0Au))  // Control Register 3 Output Set

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
  // Set pins to output
  //
  PORT4_PCNTR1_PDR |= (1u << LED1)
                   |  (1u << LED2)
                   |  (1u << LED3);
  //
  // Clear Leds
  //
  PORT4_PCNTR3_PORR = ((1u << LED1) 
                     | (1u << LED2) 
                     | (1u << LED3));
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  switch (Index) {
  case 0:
    PORT4_PCNTR3_POSR = (1u << LED1);
    break;
  case 1:
    PORT4_PCNTR3_POSR = (1u << LED2);
    break;
  case 2:
    PORT4_PCNTR3_POSR = (1u << LED3);
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  switch (Index) {
  case 0:
    PORT4_PCNTR3_PORR = (1u << LED1);
    break;
  case 1:
    PORT4_PCNTR3_PORR = (1u << LED2);
    break;
  case 2:
    PORT4_PCNTR3_PORR = (1u << LED3);
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  switch (Index) {
  case 0:
    if ((PORT4_PCNTR1_PODR & (1u << LED1)) != 0) {
      PORT4_PCNTR3_PORR = (1u << LED1);
    } else {
      PORT4_PCNTR3_POSR = (1u << LED1);
    }
    break;
  case 1:
    if ((PORT4_PCNTR1_PODR & (1u << LED2)) != 0) {
      PORT4_PCNTR3_PORR = (1u << LED2);
    } else {
      PORT4_PCNTR3_POSR = (1u << LED2);
    }
    break;
  case 2:
    if ((PORT4_PCNTR1_PODR & (1u << LED3)) != 0) {
      PORT4_PCNTR3_PORR = (1u << LED3);
    } else {
      PORT4_PCNTR3_POSR = (1u << LED3);
    }
    break;
  default:
    break;
  }
}

/*************************** End of file ****************************/
