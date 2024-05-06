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

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for AT91SAM9260-EK and AT91SAM9260-EK
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** Addresses of sfrs and peripheral identifiers ****************/

#define AT91C_PIOA_BASE_ADDR  (0xFFFFF400)
#define AT91C_PIOB_BASE_ADDR  (0xFFFFF600)

#define AT91C_PMC_BASE_ADDR   (0xFFFFFC00)

#define PIOA_ID          (2)
#define PIO_PER_OFFS     (0x00)
#define PIO_OER_OFFS     (0x10)
#define PIO_SODR_OFFS    (0x30)    // Set output data
#define PIO_CODR_OFFS    (0x34)    // Clear output data register
#define PIO_ODSR_OFFS    (0x38)    // output data status register

//
// SFRs and identifiers
//
#define AT91C_PMC_PCER (*(volatile unsigned int*) (AT91C_PMC_BASE_ADDR + 0x10))

//
// Assign LEDs to Ports
//
#define LEDPORT_BASE_ADDR  (AT91C_PIOA_BASE_ADDR)
#define LEDPORT_ID    (PIOA_ID)
#define LED0_BIT      (6)  /* User LED on AT91SAM9260-EK board, active low  */
#define LED1_BIT      (9)  /* POWERLED on AT91SAM9260-EK board, active high */

#define LEDPORT_PER   (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_PER_OFFS))
#define LEDPORT_OER   (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_OER_OFFS))
#define LEDPORT_SODR  (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_SODR_OFFS))
#define LEDPORT_CODR  (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_CODR_OFFS))
#define LEDPORT_STATE (*(volatile unsigned int*) (LEDPORT_BASE_ADDR + PIO_ODSR_OFFS))

#define LED0_MASK     (1uL << LED0_BIT)
#define LED1_MASK     (1uL << LED1_BIT)
#define LEDMASK_ALL   (LED0_MASK | LED1_MASK)

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
  AT91C_PMC_PCER     |= (1 << LEDPORT_ID); /* Enable peripheral clock for LED-Port    */
  LEDPORT_PER   = LEDMASK_ALL;       /* enable Port           */
  LEDPORT_OER   = LEDMASK_ALL;       /* switch to output mode */
  LEDPORT_SODR |= LED0_MASK;         /* Initially clear LEDs  */
  LEDPORT_CODR |= LED1_MASK;         /* Initially clear LEDs  */
}

/*********************************************************************
*
*       LED switching routines
*       LEDs are switched on by low level on port lines
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    LEDPORT_CODR = (1uL << LED0_BIT);         /* Switch on LED 0  */
  } else if (Index == 1) {
    LEDPORT_SODR = (1uL << LED1_BIT);         /* Switch on LED1  */
  }
}

void BSP_ClrLED(int Index) {
  if (Index == 0) {
    LEDPORT_SODR = (1uL << LED0_BIT);         /* Switch off LED 0 */
  } else if (Index == 1) {
    LEDPORT_CODR = (1uL << LED1_BIT);         /* Switch off LED1 */
  }
}

void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (LEDPORT_STATE & (1uL << LED0_BIT)) {  /* LED is switched off */
      BSP_SetLED(0);
    } else {
      BSP_ClrLED(0);
    }
  } else if (Index == 1) {
    if (LEDPORT_STATE & (1uL << LED1_BIT)) {  /* LED is switched on */
      BSP_ClrLED(1);
    } else {
      BSP_SetLED(1);
    }
  }
}

/****** End of file *************************************************/
