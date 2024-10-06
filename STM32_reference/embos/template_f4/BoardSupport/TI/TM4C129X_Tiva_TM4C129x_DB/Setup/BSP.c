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
Purpose : BSP for TI TM4C129 Development Kit
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RCGCGPIO            (*(volatile unsigned int*)(0x400FE608u))

#define GPION_BASE_ADDR     (0x40064000u)
#define GPION_GPIODEN       (*(volatile unsigned int*)(GPION_BASE_ADDR + 0x51Cu))
#define GPION_GPIODIR       (*(volatile unsigned int*)(GPION_BASE_ADDR + 0x400u))
#define GPION5_GPIODATA     (*(volatile unsigned int*)(GPION_BASE_ADDR + 0x080u))
#define RCGCGPIO_GPION_BIT  (12u)

#define GPIOQ_BASE_ADDR     (0x40066000u)
#define GPIOQ_GPIODEN       (*(volatile unsigned int*)(GPIOQ_BASE_ADDR + 0x51Cu))
#define GPIOQ_GPIODIR       (*(volatile unsigned int*)(GPIOQ_BASE_ADDR + 0x400u))
#define GPIOQ4_GPIODATA     (*(volatile unsigned int*)(GPIOQ_BASE_ADDR + 0x040u))
#define GPIOQ7_GPIODATA     (*(volatile unsigned int*)(GPIOQ_BASE_ADDR + 0x200u))
#define RCGCGPIO_GPIOQ_BIT  (14u)

#define LED0                (5)  // D12, PN5/LED_R
#define LED1                (4)  // D12, PQ4/LED_B
#define LED2                (7)  // D12, PQ7/LED_G

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
  RCGCGPIO |= (1u << RCGCGPIO_GPION_BIT); // Enable power for port N
  RCGCGPIO |= (1u << RCGCGPIO_GPIOQ_BIT); // Enable power for port Q

  GPION_GPIODEN   |=  (1u << LED0);       // Enable pad
  GPION_GPIODIR   |=  (1u << LED0);       // Set to output
  GPION5_GPIODATA &= ~(1u << LED0);       // Initially clear LED

  GPIOQ_GPIODEN   |=  (1u << LED1);       // Enable pad
  GPIOQ_GPIODIR   |=  (1u << LED1);       // Set to output
  GPIOQ4_GPIODATA &= ~(1u << LED1);       // Initially clear LED

  GPIOQ_GPIODEN   |=  (1u << LED2);       // Enable pad
  GPIOQ_GPIODIR   |=  (1u << LED2);       // Set to output
  GPIOQ7_GPIODATA &= ~(1u << LED2);       // Initially clear LED
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPION5_GPIODATA |= (1u << LED0);
  } else if (Index == 1) {
    GPIOQ4_GPIODATA |= (1u << LED1);
  } else if (Index == 2) {
    GPIOQ7_GPIODATA |= (1u << LED2);
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPION5_GPIODATA &= ~(1u << LED0);
  } else if (Index == 1) {
    GPIOQ4_GPIODATA &= ~(1u << LED1);
  } else if (Index == 2) {
    GPIOQ7_GPIODATA &= ~(1u << LED2);
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if (GPION5_GPIODATA != 0) {
      GPION5_GPIODATA &= ~(1u << LED0);
    } else {
      GPION5_GPIODATA |=  (1u << LED0);
    }
  } else if (Index == 1) {
    if (GPIOQ4_GPIODATA != 0) {
      GPIOQ4_GPIODATA &= ~(1u << LED1);
    } else {
      GPIOQ4_GPIODATA |=  (1u << LED1);
  }
  } else if (Index == 2) {
    if (GPIOQ7_GPIODATA != 0) {
      GPIOQ7_GPIODATA &= ~(1u << LED2);
    } else {
      GPIOQ7_GPIODATA |=  (1u << LED2);
    }
  }
}

/****** End Of File *************************************************/
