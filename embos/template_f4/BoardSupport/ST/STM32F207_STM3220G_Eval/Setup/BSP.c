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
Purpose : BSP for STM32F207 (ST MB786)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** LED port assignement on MB786 eval board ********************/
#define GPIOG_BASE_ADDR           ((unsigned int)0x40021800)

#define GPIOG_MODER               (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x00))
#define GPIOG_ODR                 (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x14))
#define GPIOG_BSRR                (*(volatile unsigned int*)(GPIOG_BASE_ADDR + 0x18))

/****** SFRs used for LED-Port **************************************/

#define RCC_BASE_ADDR             ((unsigned int)(0x40023800))
#define RCC_AHB1ENR               (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x30))

#define RCC_LEDPORT_ENR           RCC_AHB1ENR
#define RCC_LEDPORT_BIT           (6)

/****** Assign LEDs to Ports ****************************************/

#define LED0_BIT        (6)
#define LED1_BIT        (8)

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

  GPIOG_MODER &= (3uL << (LED0_BIT * 2)) | (3uL << (LED1_BIT * 2)); // Reset mode, set to input
  GPIOG_MODER |= (1uL << (LED0_BIT * 2)) | (1uL << (LED1_BIT * 2)); // Set to output mode
  GPIOG_BSRR   = (0x10000uL << LED0_BIT) | (0x10000uL << LED1_BIT); // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    GPIOG_BSRR = (1uL << LED0_BIT);   // Switch on LED0
  } else if (Index == 1) {
    GPIOG_BSRR = (1uL << LED1_BIT);   // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    GPIOG_BSRR = (0x10000uL << LED0_BIT); // Switch off LED0
  } else if (Index == 1) {
    GPIOG_BSRR = (0x10000uL << LED1_BIT); // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if ((GPIOG_ODR & (1uL << LED0_BIT)) == 0) {  // LED is switched off
      GPIOG_BSRR = (1uL << LED0_BIT);            // Switch on LED0
    } else {
      GPIOG_BSRR = (0x10000uL << LED0_BIT);      // Switch off LED0
    }
  } else if (Index == 1) {
    if ((GPIOG_ODR & (1uL << LED1_BIT)) == 0) {  // LED is switched off
      GPIOG_BSRR = (1uL << LED1_BIT);            // Switch on LED1
    } else {
      GPIOG_BSRR = (0x10000uL << LED1_BIT);      // Switch off LED1
    }
  }
}

/****** End Of File *************************************************/
