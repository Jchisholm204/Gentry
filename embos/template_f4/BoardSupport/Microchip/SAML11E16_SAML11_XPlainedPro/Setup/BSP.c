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
Purpose : BSP for SAML11 XPlainedPro eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define LED0 (7)

#define MCLK_BASE_ADDR  (0x40000800u)
#define MCLK_APBAMASK   (*(volatile unsigned int*)(MCLK_BASE_ADDR + 0x14u))

#define GPIOA_BASE_ADDR  (0x40003200u)
#define GPIOA_DIRSET     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x08u))
#define GPIOA_OUTSET     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x14u))
#define GPIOA_OUTCLR     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))
#define GPIOA_OUTTGL     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x1Cu))

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
  MCLK_APBAMASK |= (1u <<   12);  // Enable APB clock for PORT
  GPIOA_OUTCLR   = (1u << LED0);  // Disable LED0
  GPIOA_DIRSET   = (1u << LED0);  // Set pin A07 to output
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOA_OUTSET = (1u << LED0);
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOA_OUTCLR = (1u << LED0);
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOA_OUTTGL = (1u << LED0);
  }
}

/****** End Of File *************************************************/
