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
Purpose : BSP for SAMD20-Xplained Pro evalboard
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PIOA_BASE_ADDR (0x41004400)

#define PIO_DIR_OFFS    (0x00) /* Data Direction register     */
#define PIO_OUTCLR_OFFS (0x14) /* Clear output data register  */
#define PIO_OUTSET_OFFS (0x18) /* Set output data             */
#define PIO_OUTTGL_OFFS (0x1C) /* Toggle output data register */

/****** Assignment of LEDs ******************************************/

#define LEDPORT_BASE_ADDR  (PIOA_BASE_ADDR)  
#define LED0_BIT           (14)     /* Yellow LED on SAMD20-Xplained Pro board */

#define LEDPORT_DIR        (*(volatile unsigned int*) (PIOA_BASE_ADDR + PIO_DIR_OFFS))
#define LEDPORT_OUTCLR     (*(volatile unsigned int*) (PIOA_BASE_ADDR + PIO_OUTCLR_OFFS))
#define LEDPORT_OUTSET     (*(volatile unsigned int*) (PIOA_BASE_ADDR + PIO_OUTSET_OFFS))
#define LEDPORT_OUTTGL     (*(volatile unsigned int*) (PIOA_BASE_ADDR + PIO_OUTTGL_OFFS))

#define LEDMASK_ALL        (1uL << LED0_BIT)

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
  LEDPORT_DIR     = LEDMASK_ALL;  /* Enable Port           */
  LEDPORT_OUTSET |= LEDMASK_ALL;  /* Initially clear LEDs  */
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    LEDPORT_OUTCLR = (1uL << LED0_BIT);         /* Switch on LED  */
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    LEDPORT_OUTSET = (1uL << LED0_BIT);         /* Switch off LED */
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    LEDPORT_OUTTGL = (1uL << LED0_BIT);         /* Switch off LED */
  }
}

/****** End Of File *************************************************/
