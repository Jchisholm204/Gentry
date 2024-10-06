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
#define FIO_BASE_ADDR              (0x2009C000)

#define FIO1DIR3                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x23))
#define FIO2DIR0                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x40))

#define FIO1SET3                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3B))
#define FIO2SET0                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x58))

#define FIO1CLR3                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3F))
#define FIO2CLR0                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5C))

#define FIO1PIN3                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x37))
#define FIO2PIN0                   (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x54))

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const unsigned char _aLed[] = {4, 5, 7, 2, 3, 4, 5, 6};  // LED bit positions for P1 and P2

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
  FIO1DIR3 |= 0
              | (1 << 4)
              | (1 << 5)
              | (1 << 7)
              ;
  FIO2DIR0 |= 0
              | (1 << 2)
              | (1 << 3)
              | (1 << 4)
              | (1 << 5)
              | (1 << 6)
              ;
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index > 2) {
    FIO2SET0 = (1 << _aLed[Index]);   // Switch on LED
  } else {
    FIO1SET3 = (1 << _aLed[Index]);   // Switch on LED
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index > 2) {
    FIO2CLR0 = (1 << _aLed[Index]);   // Switch off LED
  } else {
    FIO1CLR3 = (1 << _aLed[Index]);   // Switch off LED
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index > 2) {
    FIO2PIN0 ^= (1 << _aLed[Index]);  // Toggle LED
  } else {
    FIO1PIN3 ^= (1 << _aLed[Index]);  // Toggle LED
  }
}

/****** End Of File *************************************************/
