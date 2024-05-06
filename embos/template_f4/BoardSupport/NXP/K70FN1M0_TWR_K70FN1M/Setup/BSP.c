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
Purpose : BSP for Freescale Kinetis K70 Tower eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SIM_SCGC5                     (*(volatile unsigned int *)(0x40048038))                 // System Clock Gating Control Register 5
#define SIM_SCGC5_PORTA_MASK          (1uL << 9)
#define SIM_SCGC5_PORTA_SHIFT         9
#define PORTA_BASE_ADDR               (0x40049000)
#define PORTA_PCR10                   (*(volatile unsigned int *)(PORTA_BASE_ADDR + 0x0028))   // Pin Control Register 10
#define PORTA_PCR11                   (*(volatile unsigned int *)(PORTA_BASE_ADDR + 0x002C))   // Pin Control Register 11
#define GPIOA_PSOR                    (*(volatile unsigned int *)(0x400FF004))                 // Port Set Output Register
#define GPIOA_PCOR                    (*(volatile unsigned int *)(0x400FF008))                 // Port Clear Output Register
#define GPIOA_PTOR                    (*(volatile unsigned int *)(0x400FF00C))                 // Port Toggle Output Register
#define GPIOA_PDDR                    (*(volatile unsigned int *)(0x400FF014))                 // Port Data Direction Register

#define PORT_PCR_SRE_MASK             0x4uL
#define PORT_PCR_ODE_MASK             0x20uL
#define PORT_PCR_DSE_MASK             0x40uL

#define PORT_PCR_MUX(x)               (((unsigned int)(((unsigned int)(x))<<PORT_PCR_MUX_SHIFT))&PORT_PCR_MUX_MASK)
#define PORT_PCR_MUX_SHIFT            8uL
#define PORT_PCR_MUX_MASK             0x700uL

//
// Assignment of LEDs
//
#define LED0_BIT                      (10)
#define LED1_BIT                      (11)
#define LEDMASK_ALL                   ((1uL << LED0_BIT) | (1uL << LED1_BIT))


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
  reg           = SIM_SCGC5;
  reg          &= ~SIM_SCGC5_PORTA_MASK;
  reg          |= (1uL << SIM_SCGC5_PORTA_SHIFT);
  SIM_SCGC5     = reg;                // Slow slew rate

  reg           = PORTA_PCR11;
  reg          |= PORT_PCR_SRE_MASK   // Slow slew rate
               |  PORT_PCR_ODE_MASK   // Open Drain Enable
               |  PORT_PCR_DSE_MASK;  // High drive strength
  PORTA_PCR11   = reg;
  PORTA_PCR11   = PORT_PCR_MUX(1);    // Alternative 1 (GPIO)

  reg           = PORTA_PCR10;
  PORTA_PCR10  |= PORT_PCR_SRE_MASK   // Slow slew rate
               |  PORT_PCR_ODE_MASK   // Open Drain Enable
               |  PORT_PCR_DSE_MASK;  // High drive strength
  PORTA_PCR10   = reg;

  PORTA_PCR10   = PORT_PCR_MUX(1);    // Alternative 1 (GPIO)

  GPIOA_PSOR    = LEDMASK_ALL;        // Set output high
  GPIOA_PDDR    = LEDMASK_ALL;        // Output Enable
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  GPIOA_PCOR = (1uL << (Index + LED0_BIT));   // Set output low
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  GPIOA_PSOR = (1uL << (Index + LED0_BIT));   // Set output high
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  GPIOA_PTOR = (1uL << (Index + LED0_BIT));   // Toggle output
}

/****** End Of File *************************************************/
