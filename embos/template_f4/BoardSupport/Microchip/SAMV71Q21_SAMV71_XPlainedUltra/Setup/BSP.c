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
Purpose : BSP for Atmel SAMV71 XPlained Ultra eval board
*/

#include "BSP.h"
#include "samv71.h"

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
  PMC->PMC_WPMR  = 0x504D4300u;      // Disable write protect
  PMC->PMC_PCER0 = (1u << ID_PIOA)   // enable PIOA clock
                 | (1u << ID_PIOC);  // enable PIOC clock
  PMC->PMC_WPMR  = 0x504D4301u;      // Enable write protect
  PIOA->PIO_PER  = PIO_PA23;         // Enable pin
  PIOA->PIO_SODR = PIO_PA23;         // Turn LED off
  PIOA->PIO_OER  = PIO_PA23;         // Enable output
  PIOA->PIO_PUDR = PIO_PA23;         // Disable pullup resistor
  PIOC->PIO_PER  = PIO_PC9;          // Enable pin
  PIOC->PIO_SODR = PIO_PC9;          // Turn LED off
  PIOC->PIO_OER  = PIO_PC9;          // Enable output
  PIOC->PIO_PUDR = PIO_PC9;          // Disable pullup resistor

}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    PIOA->PIO_CODR = PIO_PA23;
  } else if (Index == 1) {
    PIOC->PIO_CODR = PIO_PC9;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    PIOA->PIO_SODR = PIO_PA23;
  } else if (Index == 1) {
    PIOC->PIO_SODR = PIO_PC9;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (PIOA->PIO_ODSR & PIO_PA23) {
      PIOA->PIO_CODR = PIO_PA23;
    } else {
      PIOA->PIO_SODR = PIO_PA23;
    }
  } else if (Index == 1) {
    if (PIOC->PIO_ODSR & PIO_PC9) {
      PIOC->PIO_CODR = PIO_PC9;
    } else {
      PIOC->PIO_SODR = PIO_PC9;
    }
  }
}

/*************************** End of file ****************************/
