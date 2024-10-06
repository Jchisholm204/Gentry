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
Purpose : BSP for the IAR STM32L152VB-SK eval board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define GPIOA_BASE_ADDR           (0x40020000u)
#define GPIOA_MODER               (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x00u))
#define GPIOA_ODR                 (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x14u))
#define GPIOA_BSRR                (*(volatile unsigned int*)(GPIOA_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR             (0x40023800u)
#define RCC_AHB1RSTR              (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x10u))
#define RCC_AHBENR                (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x1Cu))

#define RCC_LEDPORT_RSTR          RCC_AHB1RSTR
#define RCC_LEDPORT_ENR           RCC_AHBENR
#define RCC_LEDPORT_BIT           (0)

/****** Assign LEDs to Ports ****************************************/

#define LED_PORT_MODER            GPIOA_MODER
#define LED_PORT_ODR              GPIOA_ODR
#define LED_PORT_BSRR             GPIOA_BSRR

#define LED0_BIT                  (4)
#define LED1_BIT                  (5)

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
  RCC_LEDPORT_ENR  &= ~(1u << RCC_LEDPORT_BIT);
  RCC_LEDPORT_RSTR &= ~(1u << RCC_LEDPORT_BIT);
  RCC_LEDPORT_ENR  |=  (1u << RCC_LEDPORT_BIT);

  LED_PORT_MODER &= ~(3u << (LED0_BIT * 2)) | (3u << (LED1_BIT * 2));  // Reset mode; sets port to input
  LED_PORT_MODER |=  (1u << (LED0_BIT * 2)) | (1u << (LED1_BIT * 2));  // Set to output mode
  LED_PORT_BSRR   =  (0x10000u << LED0_BIT) | (0x10000u << LED1_BIT);  // Initially clear LEDs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    LED_PORT_BSRR = (1u << LED0_BIT);              // Switch on LED0
  } else if (Index == 1) {
    LED_PORT_BSRR = (1u << LED1_BIT);              // Switch on LED1
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    LED_PORT_BSRR = (0x10000u << LED0_BIT);        // Switch off LED0
  } else if (Index == 1) {
    LED_PORT_BSRR = (0x10000u << LED1_BIT);        // Switch off LED1
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    if ((LED_PORT_ODR & (1u << LED0_BIT)) == 0) {  // LED is switched off
      LED_PORT_BSRR = (1u << LED0_BIT);            // Switch on LED0
    } else {
      LED_PORT_BSRR = (0x10000u << LED0_BIT);      // Switch off LED0
    }
  } else if (Index == 1) {
    if ((LED_PORT_ODR & (1u << LED1_BIT)) == 0) {  // LED is switched off
      LED_PORT_BSRR = (1u << LED1_BIT);            // Switch on LED1
    } else {
      LED_PORT_BSRR = (0x10000u << LED1_BIT);      // Switch off LED1
    }
  }
}

/****** End Of File *************************************************/
