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

Purpose : BSP for NXP LPCXpresso55S69 eval board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0 (4u)  // Red   P1_4
#define LED1 (6u)  // Blue  P1_6
#define LED2 (7u)  // Green P1_7

#define AHB_CLKCTRL0  (*(volatile unsigned int *)(0x40000000u + 0x200u))

#define IOCON_BASE    (0x40001000u)
#define IOCON1        ((IOCON_BASE + 0x80u))
#define IOCON1_4      (*(volatile unsigned int*)(IOCON1 + (4u * LED0)))
#define IOCON1_6      (*(volatile unsigned int*)(IOCON1 + (4u * LED1)))
#define IOCON1_7      (*(volatile unsigned int*)(IOCON1 + (4u * LED2)))

#define GPIO1_BASE    (0x4008C000u)
#define GPIO1_DIR1    (*(volatile unsigned int*)(GPIO1_BASE + 0x2004u))
#define GPIO1_SET1    (*(volatile unsigned int*)(GPIO1_BASE + 0x2204u))
#define GPIO1_CLR1    (*(volatile unsigned int*)(GPIO1_BASE + 0x2284u))
#define GPIO1_NOT1    (*(volatile unsigned int*)(GPIO1_BASE + 0x2304u))

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
  AHB_CLKCTRL0 |=  (1u << 13u);  // enables clock for the IOCON block
  IOCON1_4      =   0xA0u;       // reset IO configuration
  IOCON1_6      =   0xA0u;       // reset IO configuration
  IOCON1_7      =   0xA0u;       // reset IO configuration
  AHB_CLKCTRL0 |= (1u << 15u);   // enables clock for the GPIO1 port registers
  GPIO1_SET1    = (1u << LED0)   // Clear LED0
                | (1u << LED1)   // Clear LED1
                | (1u << LED2);  // Clear LED1
  GPIO1_DIR1   |= (1u << LED0)   // Set LED0 to output
                | (1u << LED1)   // Set LED1 to output
                | (1u << LED2);  // Set LED1 to output
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIO1_CLR1 = 1u << LED0;
  } else if (Index == 1) {
    GPIO1_CLR1 = 1u << LED1;
  } else if (Index == 2) {
    GPIO1_CLR1 = 1u << LED2;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIO1_SET1 = 1u << LED0;
  } else if (Index == 1) {
    GPIO1_SET1 = 1u << LED1;
  } else if (Index == 2) {
    GPIO1_SET1 = 1u << LED2;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    GPIO1_NOT1 = 1u << LED0;
  } else if (Index == 1) {
    GPIO1_NOT1 = 1u << LED1;
  } else if (Index == 2) {
    GPIO1_NOT1 = 1u << LED2;
  }
}

/*************************** End of file ****************************/
