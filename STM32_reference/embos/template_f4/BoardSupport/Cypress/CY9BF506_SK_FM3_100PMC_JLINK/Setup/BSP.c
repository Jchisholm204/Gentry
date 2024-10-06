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
Purpose : BSP for Fujitsu MB9BF506 and Fujitsu MB9BF506 SK FM3 100PMC evalboard
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define PFR1        (*(volatile unsigned int*)(0x40033004))
#define DDR1        (*(volatile unsigned int*)(0x40033204))
#define PDOR1       (*(volatile unsigned int*)(0x40033404))
#define ADE         (*(volatile unsigned int*)(0x40033500))


#define LED1        (1uL <<  8)
#define LED2        (1uL <<  9)
#define LED3        (1uL << 10)
#define LED4        (1uL << 11)
#define LED5        (1uL << 12)
#define LED6        (1uL << 13)
#define LED7        (1uL << 14)
#define LED8        (1uL << 15)

#define LED_MASK    (LED1 | LED2 | LED3 | LED4 | \
                     LED5 | LED6 | LED7 | LED8 )

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
  PFR1  &= ~LED_MASK;    // Select CPIO function
  PDOR1 |= LED_MASK;     // Set Pin to turn off leds
  DDR1  |= LED_MASK;     // Make led pins outputs
  ADE   &= ~LED_MASK;    // Disable analog inputs
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < 8) {
    PDOR1 |= (1uL << (Index + 8));
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < 8) {
    PDOR1 &= ~(1uL << (Index + 8));
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < 8) {
    PDOR1 ^= (1uL << (Index + 8));
  }
}
/****** End Of File *************************************************/
