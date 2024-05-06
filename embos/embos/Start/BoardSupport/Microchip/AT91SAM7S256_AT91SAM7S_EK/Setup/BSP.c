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
Purpose : BSP for AT91SAM7S-EK
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** Addresses of sfrs and peripheral identifiers ****************/

#define _AT91C_PIOA_BASE_ADDR  (0xFFFFF400)
#define _AT91C_PIOB_BASE_ADDR  (0xFFFFF600)

#define _AT91C_PMC_BASE_ADDR   (0xFFFFFC00)

#define _PIOA_ID          (2)
#define _PIOB_ID          (3)
#define _PIO_PER_OFFS     (0x00uL)
#define _PIO_OER_OFFS     (0x10uL)
#define _PIO_SODR_OFFS    (0x30uL)    /* Set output data              */
#define _PIO_CODR_OFFS    (0x34uL)    /* Clear output data register   */
#define _PIO_ODSR_OFFS    (0x38uL)    /* output data status register  */
#define _PIO_PDSR_OFFS    (0x3cuL)    /* pin data status register     */

/****** SFRs and identifiers used for LED demo **********************/

#define _PMC           (*(volatile unsigned int*) _AT91C_PMC_BASE_ADDR)

/****** Assign LEDs to Ports ****************************************/

/****** Target specific configuration *******************************/

#define _LEDPORT_BASE_ADDR  (_AT91C_PIOA_BASE_ADDR)
#define _LEDPORT_ID    (_PIOA_ID)
#define _LED0_BIT      (0)  /* LED1 on SAM7S-EK board, active low */
#define _LED1_BIT      (1)  /* LED2 on SAM7S-EK board, active low */

#define _LEDPORT_PER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_PER_OFFS))
#define _LEDPORT_OER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_OER_OFFS))
#define _LEDPORT_SODR  (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_SODR_OFFS))
#define _LEDPORT_CODR  (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_CODR_OFFS))
#define _LEDPORT_STATE (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_ODSR_OFFS))

#define _LEDMASK_ALL   ((1uL << _LED0_BIT) | (1uL << _LED1_BIT))

#define _USB_PER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_PER_OFFS))
#define _USB_OER   (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_OER_OFFS))
#define _USB_CODR  (*(volatile unsigned int*) (_LEDPORT_BASE_ADDR + _PIO_CODR_OFFS))
#define _USB_DP_PUP_BIT (16)

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
  _PMC |= (1uL << _LEDPORT_ID);   /* Enable peripheral clock for LED-Port    */
  _LEDPORT_PER   = _LEDMASK_ALL;  /* enable Port           */
  _LEDPORT_OER   = _LEDMASK_ALL;  /* switch to output mode */
  _LEDPORT_SODR |= _LEDMASK_ALL;  /* Initially clear LEDs  */
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  _LEDPORT_CODR = (1uL << Index);         /* Switch on LED 0  */
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  _LEDPORT_SODR = (1uL << Index);         /* Switch off LED 0 */
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (_LEDPORT_STATE & (1uL << Index)) {  /* LED is switched off */
    BSP_SetLED(Index);
  } else {
    BSP_ClrLED(Index);
  }
}

/*************************** End of file ****************************/
