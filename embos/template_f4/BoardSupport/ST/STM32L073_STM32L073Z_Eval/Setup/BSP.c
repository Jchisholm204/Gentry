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
Purpose : BSP for ST STM32L073Z_Eval
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LED0_BIT         (4)  // (Green)  user LD1 is connected to PE4, active low
#define LED1_BIT         (5)  // (Orange) user LD2 is connected to PE5, active low
#define LED2_BIT         (1)  // (Red)    user LD3 is connected to PD1, active low
#define LED3_BIT         (7)  // (Blue)   user LD4 is connected to PE7, active low

#define GPIOD_BASE_ADDR  (0x50000C00u)
#define GPIOD_MODER      (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x00u))
#define GPIOD_ODR        (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x14u))

#define GPIOE_BASE_ADDR  (0x50001000u)
#define GPIOE_MODER      (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x00u))
#define GPIOE_ODR        (*(volatile unsigned int*)(GPIOE_BASE_ADDR + 0x14u))

#define RCC_IOPENR       (*(volatile unsigned int*)(0x4002102Cu))
#define IOPDEN_BIT       (3)
#define IOPEEN_BIT       (4)

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
  RCC_IOPENR  |= (1u << IOPDEN_BIT)              // Enable clock to Port D
              |  (1u << IOPEEN_BIT)              // Enable clock to Port E
              ;
  GPIOE_MODER &= ~(1u << (LED0_BIT << 1) << 1);  // Alternate function PE4
  GPIOE_MODER &= ~(1u << (LED1_BIT << 1) << 1);  // Alternate function PE5
  GPIOD_MODER &= ~(1u << (LED2_BIT << 1) << 1);  // Alternate function PD1
  GPIOE_MODER &= ~(1u << (LED3_BIT << 1) << 1);  // Alternate function PE7

  GPIOE_ODR   |= (1u << LED0_BIT);               // Initially clear LD0
  GPIOE_ODR   |= (1u << LED1_BIT);               // Initially clear LD1
  GPIOD_ODR   |= (1u << LED2_BIT);               // Initially clear LD2
  GPIOE_ODR   |= (1u << LED3_BIT);               // Initially clear LD3
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  switch (Index) {
  case 0:
    GPIOE_ODR &= ~(1u << LED0_BIT);
    break;
  case 1:
    GPIOE_ODR &= ~(1u << LED1_BIT);
    break;
  case 2:
    GPIOD_ODR &= ~(1u << LED2_BIT);
    break;
  case 3:
    GPIOE_ODR &= ~(1u << LED3_BIT);
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  switch (Index) {
  case 0:
    GPIOE_ODR |= (1u << LED0_BIT);
    break;
  case 1:
    GPIOE_ODR |= (1u << LED1_BIT);
    break;
  case 2:
    GPIOD_ODR |= (1u << LED2_BIT);
    break;
  case 3:
    GPIOE_ODR |= (1u << LED3_BIT);
    break;
  default:
    break;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  switch (Index) {
  case 0:
    GPIOE_ODR ^= (1u << LED0_BIT);
    break;
  case 1:
    GPIOE_ODR ^= (1u << LED1_BIT);
    break;
  case 2:
    GPIOD_ODR ^= (1u << LED2_BIT);
    break;
  case 3:
    GPIOE_ODR ^= (1u << LED3_BIT);
    break;
  default:
    break;
  }
}

/*************************** End of file ****************************/
