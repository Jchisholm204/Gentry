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
Purpose : BSP for Atmel SAM3X-EK evalboard
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define AT91C_PIOB_BASE_ADDR  (0x400E1000)

#define PIO_PER_OFFS          (0x00)
#define PIO_OER_OFFS          (0x10)
#define PIO_SODR_OFFS         (0x30) /* Set output data                     */
#define PIO_CODR_OFFS         (0x34) /* Clear output data register          */
#define PIO_ODSR_OFFS         (0x38) /* Output data status register         */

#define LEDPORT_BASE_ADDR     (AT91C_PIOB_BASE_ADDR)
#define LED0_BIT              (12u)  /* LED D3 Amber on SAM3X-EK board, active low  */
#define LED1_BIT              (13u)  /* LED D4 Green on SAM3X-EK board, active low  */

#define LEDPORT_PER           (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_PER_OFFS))
#define LEDPORT_OER           (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_OER_OFFS))
#define LEDPORT_SODR          (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_SODR_OFFS))
#define LEDPORT_CODR          (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_CODR_OFFS))
#define LEDPORT_STATE         (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_ODSR_OFFS))

#define LEDMASK_ALL           ((1uL << LED0_BIT) | (1uL << LED1_BIT))

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
  LEDPORT_PER   = LEDMASK_ALL;  /* Enable Port           */
  LEDPORT_OER   = LEDMASK_ALL;  /* Switch to output mode */
  LEDPORT_SODR |= LEDMASK_ALL;  /* Initially clear LEDs  */
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  LEDPORT_CODR = (1uL << (Index + LED0_BIT));         /* Switch on LED  */
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  LEDPORT_SODR = (1uL << (Index + LED0_BIT));         /* Switch off LED */
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (LEDPORT_STATE & (1uL << (Index + LED0_BIT))) {
    LEDPORT_CODR = (1uL << (Index + LED0_BIT));
  } else {
    LEDPORT_SODR = (1uL << (Index + LED0_BIT));
  }
}

/****** End Of File *************************************************/
