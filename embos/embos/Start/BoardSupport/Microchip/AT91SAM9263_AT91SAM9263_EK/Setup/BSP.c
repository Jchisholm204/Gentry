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
File    : BSP.c
Purpose : BSP for AT91SAM9263-EK
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PIOB_ID             (3u)
#define PIOCDE_ID           (4u)         // Parallel I/O Controller C, D and E

#define PIOC_BASE_ADDR      (0xFFFFF600u)

#define PIO_PER_OFFS        (0x00u)
#define PIO_OER_OFFS        (0x10u)
#define PIO_SODR_OFFS       (0x30u)      // Set output data
#define PIO_CODR_OFFS       (0x34u)      // Clear output data register
#define PIO_ODSR_OFFS       (0x38u)      // output data status register
#define PIO_PDSR_OFFS       (0x3cu)      // pin data status register

#define PIOB_BASE_ADDR      (0xFFFFF400u)
#define PMC_BASE_ADDR       (0xFFFFFC00u)
#define PMC_PCER            (*(volatile unsigned int*) (PMC_BASE_ADDR + 0x10u))

#define LEDPORT0_BASE_ADDR  (PIOB_BASE_ADDR)
#define LEDPORT1_BASE_ADDR  (PIOC_BASE_ADDR)

#define LEDPORT0_ID         (PIOB_ID)
#define LEDPORT1_ID         (PIOCDE_ID)

#define LED0_BIT            ( 8u)        // LED1 on AT91SAM9263-EK board, active low
#define LED1_BIT            (29u)        // LED2 on AT91SAM9263-EK board, active low

#define LEDPORT0_PER        (*(volatile unsigned int*) (LEDPORT0_BASE_ADDR + PIO_PER_OFFS))
#define LEDPORT0_OER        (*(volatile unsigned int*) (LEDPORT0_BASE_ADDR + PIO_OER_OFFS))
#define LEDPORT0_SODR       (*(volatile unsigned int*) (LEDPORT0_BASE_ADDR + PIO_SODR_OFFS))
#define LEDPORT0_CODR       (*(volatile unsigned int*) (LEDPORT0_BASE_ADDR + PIO_CODR_OFFS))
#define LEDPORT0_STATE      (*(volatile unsigned int*) (LEDPORT0_BASE_ADDR + PIO_ODSR_OFFS))

#define LEDPORT1_PER        (*(volatile unsigned int*) (LEDPORT1_BASE_ADDR + PIO_PER_OFFS))
#define LEDPORT1_OER        (*(volatile unsigned int*) (LEDPORT1_BASE_ADDR + PIO_OER_OFFS))
#define LEDPORT1_SODR       (*(volatile unsigned int*) (LEDPORT1_BASE_ADDR + PIO_SODR_OFFS))
#define LEDPORT1_CODR       (*(volatile unsigned int*) (LEDPORT1_BASE_ADDR + PIO_CODR_OFFS))
#define LEDPORT1_STATE      (*(volatile unsigned int*) (LEDPORT1_BASE_ADDR + PIO_ODSR_OFFS))

#define LEDMASK0_ALL        (1u << LED0_BIT)
#define LEDMASK1_ALL        (1u << LED1_BIT)

/*********************************************************************
*
*       Static const
*
**********************************************************************
*/
typedef struct {
  volatile unsigned int* pSODR;
  volatile unsigned int* pCODR;
  volatile unsigned int* pSTATE;
  char                   Id;
} LED_TYPE;

static const LED_TYPE _aLED[] = {
  {&LEDPORT0_SODR, &LEDPORT0_CODR, &LEDPORT0_STATE, LED0_BIT},
  {&LEDPORT1_SODR, &LEDPORT1_CODR, &LEDPORT1_STATE, LED1_BIT}
};

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
  PMC_PCER     |= (1 << LEDPORT0_ID)   // Enable peripheral clock for LED-Ports
               |  (1 << LEDPORT1_ID);

  LEDPORT0_PER   = LEDMASK0_ALL;       // enable Port
  LEDPORT0_OER   = LEDMASK0_ALL;       // switch to output mode
  LEDPORT0_SODR |= LEDMASK0_ALL;       // Initially clear LEDs
  LEDPORT1_PER   = LEDMASK1_ALL;       // enable Port
  LEDPORT1_OER   = LEDMASK1_ALL;       // switch to output mode
  LEDPORT1_SODR |= LEDMASK1_ALL;       // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if(Index < 2) {
    *_aLED[Index].pCODR = (1u << _aLED[Index].Id);         // Switch on LED 0
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < 2) {
    *_aLED[Index].pSODR = (1 << _aLED[Index].Id);          // Switch on LED 0
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if(Index < 2) {
    if (*_aLED[Index].pSTATE & (1 << _aLED[Index].Id)) {   // LED is switched off
      BSP_SetLED(Index);
    } else {
      BSP_ClrLED(Index);
    }
  }
}

/*************************** End of file ****************************/
