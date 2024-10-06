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
Purpose : BSP for LPC1768 (KEIL MCB1700)
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define FIO_BASE_ADDR       (0x2009C000)

#define FIO0DIR             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x00))
#define FIO1DIR             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x20))

#define FIO0SET             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x18))
#define FIO1SET             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x38))

#define FIO0CLR             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x1C))
#define FIO1CLR             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x3C))

#define FIO0PIN             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x14))
#define FIO1PIN             (*(volatile unsigned int  *)(FIO_BASE_ADDR + 0x34))

/****** Assign LEDs to ports ****************************************/

#define LED0_BIT            (25)        // User-LED1, port1.25
#define LED1_BIT             (4)        // User-LED2, port0.4

#define LED0_DIR            (FIO1DIR)
#define LED1_DIR            (FIO0DIR)
#define LED0_SET            (FIO1SET)
#define LED1_SET            (FIO0SET)
#define LED0_CLR            (FIO1CLR)
#define LED1_CLR            (FIO0CLR)
#define LED0_PIN            (FIO1PIN)
#define LED1_PIN            (FIO0PIN)

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
  // Init LED port, switch port to output mode
  //
  LED0_DIR |= (1uL << LED0_BIT);
  LED1_DIR |= (1uL << LED1_BIT);
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    LED0_SET = (1uL << LED0_BIT);   // Switch on LED1
  } else {
    LED1_SET = (1uL << LED1_BIT);   // Switch on LED2
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index == 0) {
    LED0_CLR = (1uL << LED0_BIT);   // Switch off LED1
  } else {
    LED1_CLR = (1uL << LED1_BIT);   // Switch off LED2
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    LED0_PIN ^= (1uL << LED0_BIT);  // Toggle LED1
  } else {
    LED1_PIN ^= (1uL << LED1_BIT);  // Toggle LED2
  }
}

/****** End Of File *************************************************/
