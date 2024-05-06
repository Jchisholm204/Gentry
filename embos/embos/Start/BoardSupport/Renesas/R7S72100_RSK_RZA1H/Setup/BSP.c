/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
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
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : BSP for Renesas RSK+RZA1 evalboard
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PORT7_BASE_ADDR  (0xFCFE3000)
#define PORT7_P          (*(volatile unsigned short*) (PORT7_BASE_ADDR + 0x001Cu))
#define PORT7_PM         (*(volatile unsigned short*) (PORT7_BASE_ADDR + 0x031Cu))
#define PORT7_PMC        (*(volatile unsigned short*) (PORT7_BASE_ADDR + 0x041Cu))
#define PORT7_PIPC       (*(volatile unsigned short*) (PORT7_BASE_ADDR + 0x421Cu))

#define LED0_BIT         (1)  // User LED0, green, active low 
#define LED0_MASK        (1u << LED0_BIT)
#define LEDMASK_ALL      (LED0_MASK)
#define LED_NUM_LEDS     (1)

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
  PORT7_PMC  &= ~(LEDMASK_ALL);     /* Enable Port           */
  PORT7_P    |= LEDMASK_ALL;        /* Initially all LEDs off  */
  PORT7_PM   &= ~(LEDMASK_ALL);     /* Initially clear LEDs    */
  PORT7_PIPC &= ~(LEDMASK_ALL);     /* Initially clear LEDs    */
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < LED_NUM_LEDS) {
    PORT7_P &= ~(1u << (LED0_BIT + Index));  /* Switch on LED, active low */
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < LED_NUM_LEDS) {
    PORT7_P |= (1u << (LED0_BIT + Index));   /* Switch off LED */
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < LED_NUM_LEDS) {
    PORT7_P ^= (1L << (LED0_BIT + Index));
  }
}

/*************************** End of file ****************************/
