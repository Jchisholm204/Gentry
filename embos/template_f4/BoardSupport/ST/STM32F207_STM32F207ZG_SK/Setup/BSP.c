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
Purpose : BSP for STM32F207 (IAR SK)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** LED port assignement on IAR SK eval board ********************/
#define GPIOF_BASE_ADDR           ((unsigned int)0x40021400)

#define GPIOF_MODER               (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x00))
#define GPIOF_ODR                 (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x14))
#define GPIOF_BSRR                (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x18))

/****** SFRs used for LED-Port **************************************/

#define RCC_BASE_ADDR             ((unsigned int)(0x40023800))
#define RCC_AHB1ENR               (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30))

#define RCC_LEDPORT_ENR           RCC_AHB1ENR
#define RCC_LEDPORT_BIT           (5)

/****** Assign LEDs to Ports ****************************************/

#define LED0_BIT        (6)
#define LED1_BIT        (7)

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
  //
  // Initialize port for LEDs (sample application)
  //
  RCC_LEDPORT_ENR &= ~(1uL << RCC_LEDPORT_BIT);
  RCC_LEDPORT_ENR |=  (1uL << RCC_LEDPORT_BIT);

  GPIOF_MODER &= (3uL << (LED0_BIT * 2)) | (3uL << (LED1_BIT * 2)); // Reset mode, set to input
  GPIOF_MODER |= (1uL << (LED0_BIT * 2)) | (1uL << (LED1_BIT * 2)); // Set to output mode
  GPIOF_BSRR   = (0x10000uL << LED0_BIT) | (0x10000uL << LED1_BIT); // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOF_BSRR = (1uL << LED0_BIT);   // Switch on LED0
  } else if (Index == 1) {
    GPIOF_BSRR = (1uL << LED1_BIT);   // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOF_BSRR = (0x10000uL << LED0_BIT); // Switch off LED0
  } else if (Index == 1) {
    GPIOF_BSRR = (0x10000uL << LED1_BIT); // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if ((GPIOF_ODR & (1uL << LED0_BIT)) == 0) {  // LED is switched off
      GPIOF_BSRR = (1uL << LED0_BIT);            // Switch on LED0
    } else {
      GPIOF_BSRR = (0x10000uL << LED0_BIT);      // Switch off LED0
    }
  } else if (Index == 1) {
    if ((GPIOF_ODR & (1uL << LED1_BIT)) == 0) {  // LED is switched off
      GPIOF_BSRR = (1uL << LED1_BIT);            // Switch on LED1
    } else {
      GPIOF_BSRR = (0x10000uL << LED1_BIT);      // Switch off LED1
    }
  }
}

/****** End Of File *************************************************/
