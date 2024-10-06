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
Purpose : BSP for STM32F103 (ST MB672)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// GPIOF
//
#define GPIOF_BASE_ADDR  ((unsigned int)(0x40011C00u))
#define GPIOF_CRL        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x00u))
#define GPIOF_CRH        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x04u))
#define GPIOF_ODR        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x0Cu))
#define GPIOF_BSR        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x10u))
#define GPIOF_BRR        (*(volatile unsigned int*)(GPIOF_BASE_ADDR + 0x14u))

//
// RCC
//
#define RCC_BASE_ADDR    ((unsigned int)(0x40021000u))
#define RCC_APB2RSTR     (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x0Cu))
#define RCC_APB2ENR      (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x18u))
#define IOPFEN_BIT       (7)

//
// LEDs
//
#define NUM_LEDS         4u
#define LED0_BIT         (6)
#define LED1_BIT         (7)
#define LED2_BIT         (8)
#define LED3_BIT         (9)

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
  // Enable clock to I/O Port F
  //
  RCC_APB2ENR |= (1u << IOPFEN_BIT);
  //
  // Set CNF6, CNF7 to 0b00 --> Analog mode
  //
  GPIOF_CRL   &= ~( (0xCu <<  (LED0_BIT * 4))
                  | (0xCu <<  (LED1_BIT * 4)));
  //
  // Set CNF8, CNF9 to 0b00 --> Analog mode
  //
  GPIOF_CRH   &= ~( (0xCu << ((LED2_BIT - 8) * 4))
                  | (0xCu << ((LED3_BIT - 8) * 4)));
  //
  // Set MODE6, MODE7 to 0b11 --> Output mode (max 50 MHz)
  //
  GPIOF_CRL   |= (0x3u <<  (LED0_BIT * 4))
              |  (0x3u <<  (LED1_BIT * 4));
  //
  // Set MODE8, MODE9 to 0b11 --> Output mode (max 50 MHz)
  //
  GPIOF_CRH   |= (0x3u << ((LED2_BIT - 8) * 4))
              |  (0x3u << ((LED3_BIT - 8) * 4));
  //
  // Initially clear LEDs
  //
  GPIOF_BRR   |= (1u << LED0_BIT)
              |  (1u << LED1_BIT)
              |  (1u << LED2_BIT)
              |  (1u << LED3_BIT);
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if ((unsigned int)Index < NUM_LEDS) {
    GPIOF_BSR |= (1u << (LED0_BIT + Index));    // Switch on LED
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if ((unsigned int)Index < NUM_LEDS) {
    GPIOF_BRR |= (1u << (LED0_BIT + Index));    // Switch off LED
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if ((unsigned int)Index < NUM_LEDS) {
    if ((GPIOF_ODR & (1u << (LED0_BIT + Index))) == 0) {
      GPIOF_BSR |= (1u << (LED0_BIT + Index));  // Switch on LED
    } else {
      GPIOF_BRR |= (1u << (LED0_BIT + Index));  // Switch off LED
    }
  }
}

/****** End Of File *************************************************/
