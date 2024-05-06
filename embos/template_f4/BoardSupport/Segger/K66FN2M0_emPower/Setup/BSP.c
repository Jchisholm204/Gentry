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
Purpose : BSP for Segger emPower board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "MK66F18.h"    // Device specific header file, contains CMSIS
#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NUM_LEDS 8u

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/
typedef struct _LED_INFO {
  int PortPin;
  volatile unsigned int * pSetReg;
  volatile unsigned int * pClrReg;
  volatile unsigned int * pToggleReg;
  volatile unsigned int * pDirReg;
  volatile unsigned int * pControlReg;
} LED_INFO;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static LED_INFO _aLEDInfo[] = {
  {12, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR12},  // LED0 GREEN
  {10, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR10},  // LED1 GREEN
  { 8, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR8 },  // LED2 GREEN
  { 0, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR0 },  // LED3 GREEN
  {13, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR13},  // LED0 RED
  {11, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR11},  // LED1 RED
  { 9, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR9 },  // LED2 RED
  { 1, (volatile unsigned int *)&GPIOC_PCOR, (volatile unsigned int *)&GPIOC_PSOR, (volatile unsigned int *)&GPIOC_PTOR, (volatile unsigned int *)&GPIOC_PDDR, (volatile unsigned int *)&PORTC_PCR1 },  // LED3 RED
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
  unsigned int reg;
  unsigned int i;
  reg           = SIM_SCGC5;
  reg          &= ~SIM_SCGC5_PORTB_MASK;
  reg          |= (1uL << SIM_SCGC5_PORTC_SHIFT);
  SIM_SCGC5     = reg;  // Slow slew rate

  for (i = 0; i < NUM_LEDS; i++) {
    reg           = *(_aLEDInfo[i].pControlReg);
    reg          |= PORT_PCR_SRE_MASK   // Slow slew rate
                 |  PORT_PCR_ODE_MASK   // Open Drain Enable
                 |  PORT_PCR_DSE_MASK;  // High drive strength
    *(_aLEDInfo[i].pControlReg)  = reg;
    *(_aLEDInfo[i].pControlReg) |= PORT_PCR_MUX(1);          // Alternative 1 (GPIO)
    *(_aLEDInfo[i].pClrReg)  = (1 << _aLEDInfo[i].PortPin);  // Alternative 1 (GPIO)
    *(_aLEDInfo[i].pDirReg) |= (1 << _aLEDInfo[i].PortPin);  // Alternative 1 (GPIO)
  }
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < (int)NUM_LEDS) {
   *(_aLEDInfo[Index].pSetReg) = (1u << _aLEDInfo[Index].PortPin);
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < (int)NUM_LEDS) {
   *(_aLEDInfo[Index].pClrReg) = (1u << _aLEDInfo[Index].PortPin);
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < (int)NUM_LEDS) {
    *(_aLEDInfo[Index].pToggleReg) = (1u << _aLEDInfo[Index].PortPin);
  }
}

/****** End Of File *************************************************/
