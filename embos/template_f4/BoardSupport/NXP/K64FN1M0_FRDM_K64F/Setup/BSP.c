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
Purpose : BSP for Freescale Kinetis K64F FRDM eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SIM_SCGC5                     (*(volatile int*)(0x40048038))              // System Clock Gating Control Register 5
#define SIM_SCGC5_PORTB_MASK          (1uL << 10)
#define SIM_SCGC5_PORTB_SHIFT         (10)
#define PORTB_BASE_ADDR               (0x400FF040)
#define PORTB_PCR21                   (*(volatile int*)(0x4004A054))              // Pin Control Register 21
#define PORTB_PCR22                   (*(volatile int*)(0x4004A058))              // Pin Control Register 22
#define GPIOB_PSOR                    (*(volatile int*)(PORTB_BASE_ADDR + 0x04))  // Port Set Output Register
#define GPIOB_PCOR                    (*(volatile int*)(PORTB_BASE_ADDR + 0x08))  // Port Clear Output Register
#define GPIOB_PTOR                    (*(volatile int*)(PORTB_BASE_ADDR + 0x0C))  // Port Toggle Output Register
#define GPIOB_PDDR                    (*(volatile int*)(PORTB_BASE_ADDR + 0x14))  // Port Data Direction Register

#define PORT_PCR_SRE_MASK             (0x4uL)
#define PORT_PCR_ODE_MASK             (0x20uL)
#define PORT_PCR_DSE_MASK             (0x40uL)

#define PORT_PCR_MUX(x)               (((int)(((int)(x))<<PORT_PCR_MUX_SHIFT))&PORT_PCR_MUX_MASK)
#define PORT_PCR_MUX_SHIFT            (8uL)
#define PORT_PCR_MUX_MASK             (0x700uL)

//
// Assignment of LEDs
//
#define LED0_BIT                      (21)
#define LED1_BIT                      (22)
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
  reg          &= ~SIM_SCGC5_PORTB_MASK;
  reg          |= (1uL << SIM_SCGC5_PORTB_SHIFT);
  SIM_SCGC5     = reg;                // Slow slew rate

  reg           = PORTB_PCR21;
  reg          |= PORT_PCR_SRE_MASK   // Slow slew rate
               |  PORT_PCR_ODE_MASK   // Open Drain Enable
               |  PORT_PCR_DSE_MASK;  // High drive strength
  PORTB_PCR21   = reg;
  PORTB_PCR21  |= PORT_PCR_MUX(1);    // Alternative 1 (GPIO)

  reg           = PORTB_PCR21;
  PORTB_PCR22  |= PORT_PCR_SRE_MASK   // Slow slew rate
               |  PORT_PCR_ODE_MASK   // Open Drain Enable
               |  PORT_PCR_DSE_MASK;  // High drive strength
  PORTB_PCR22   = reg;
  PORTB_PCR22  |= PORT_PCR_MUX(1);    // Alternative 1 (GPIO)

  GPIOB_PSOR    = LEDMASK_ALL;        // Set output high
  GPIOB_PDDR    = LEDMASK_ALL;        // Output Enable
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  GPIOB_PCOR = (1uL << (Index + LED0_BIT));  // Set output low
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  GPIOB_PSOR = (1uL << (Index + LED0_BIT));  // Set output high
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  GPIOB_PTOR = (1uL << (Index + LED0_BIT));
}

/****** End Of File *************************************************/
