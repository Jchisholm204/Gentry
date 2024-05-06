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
Purpose : BSP for NXP FRDM-KE02Z40M eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define GPIOB_BASE_ADDR  (0x400FF040u)
#define GPIOB_PSOR       (*(volatile unsigned int *)(GPIOB_BASE_ADDR + 0x04u))   // Port set output register
#define GPIOB_PCOR       (*(volatile unsigned int *)(GPIOB_BASE_ADDR + 0x08u))   // Port clear output register
#define GPIOB_PTOR       (*(volatile unsigned int *)(GPIOB_BASE_ADDR + 0x0Cu))   // Port toggle output register
#define GPIOB_PDDR       (*(volatile unsigned int *)(GPIOB_BASE_ADDR + 0x14u))   // Port data direction register

#define LED0             (1u << 25)  // PTH1
#define LED1             (1u << 26)  // PTH2
#define LED2             (1u <<  7)  // PTE7

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
  GPIOB_PSOR  = LED0 | LED1 | LED2;  // Disable LEDs
  GPIOB_PDDR |= LED0 | LED1 | LED2;  // Set GPIO pins to output
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOB_PCOR = LED0;
  } else if (Index == 1) {
    GPIOB_PCOR = LED1;
  } else {
    GPIOB_PCOR = LED2;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOB_PSOR = LED0;
  } else if (Index == 1) {
    GPIOB_PSOR = LED1;
  } else {
    GPIOB_PSOR = LED2;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIOB_PTOR = LED0;
  } else if (Index == 1) {
    GPIOB_PTOR = LED1;
  } else {
    GPIOB_PTOR = LED2;
  }
}

/****** End Of File *************************************************/
