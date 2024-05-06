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
File    : BSP.c
Purpose : BSP for STM32H573I-DK board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_PIN         (8)  // PI8, LD2, orange, low active
#define LED1_PIN         (9)  // PI9, LD1, green, low active

#define GPIOI_BASE_ADDR  (0x42022000u)
#define GPIOI_MODER      (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x00u))
#define GPIOI_OTYPER     (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x04u))
#define GPIOI_OSPEEDR    (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x08u))
#define GPIOI_PUPDR      (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x0Cu))
#define GPIOI_ODR        (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x14u))
#define GPIOI_BSRR       (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x18u))

#define RCC_BASE_ADDR    (0x44020C00u)
#define RCC_AHB2ENR      (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x8Cu))

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
  // Enable GPIO clocks
  //
  RCC_AHB2ENR   |= (0x1u << 8);                // Enable the GPIOI clock
  //
  // Initialize LED0
  //
  GPIOI_MODER   &= ~(0x3u << (LED0_PIN * 2));  // Clear mode register
  GPIOI_MODER   |=  (0x1u << (LED0_PIN * 2));  // Set IO direction to output mode
  GPIOI_OSPEEDR |=  (0x3u << (LED0_PIN * 2));  // Set speed to high frequency
  GPIOI_OTYPER  &= ~(0x1u << (LED0_PIN * 1));  // Set output to push-pull
  GPIOI_PUPDR   &= ~(0x3u << (LED0_PIN * 2));  // Clear the pull-up/pull-down register
  GPIOI_PUPDR   |=  (0x1u << (LED0_PIN * 2));  // Set push-pull to pull-up
  GPIOI_BSRR     =  (0x1u << LED0_PIN);        // Turn LED off
  //
  // Initialize LED1
  //
  GPIOI_MODER   &= ~(0x3u << (LED1_PIN * 2));  // Clear mode register
  GPIOI_MODER   |=  (0x1u << (LED1_PIN * 2));  // Set IO direction to output mode
  GPIOI_OSPEEDR |=  (0x3u << (LED1_PIN * 2));  // Set speed to high frequency
  GPIOI_OTYPER  &= ~(0x1u << (LED1_PIN * 1));  // Set output to push-pull
  GPIOI_PUPDR   &= ~(0x3u << (LED1_PIN * 2));  // Clear the pull-up/pull-down register
  GPIOI_PUPDR   |=  (0x1u << (LED1_PIN * 2));  // Set push-pull to pull-up
  GPIOI_BSRR     =  (0x1u << LED1_PIN);        // Turn LED off
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < 2) {
    GPIOI_BSRR |= ((0x1u << 16) << (LED0_PIN + Index));  // Turn LED on
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < 2) {
    GPIOI_BSRR |= (0x1u << (LED0_PIN + Index));          // Turn LED off
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < 2) {
    GPIOI_ODR ^= (0x1u << (LED0_PIN + Index));           // Toggle LED
  }
}

/*************************** End of file ****************************/
