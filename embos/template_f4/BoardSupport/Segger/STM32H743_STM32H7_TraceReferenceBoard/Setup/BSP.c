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
Purpose : BSP for the STM32H7 Trace Reference Board

Additional information:
  The STM32H7 Trace Reference Board has three LEDs 
  on GPIO Port A, Pins 8 - 10.
--------  END-OF-HEADER  ---------------------------------------------
*/
#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** SFRs used **************************************/

#define GPIOA_BASE_ADDR           ((unsigned int)0x58020000)

#define GPIOx_MODER_OFFSET        (0x00)
#define GPIOx_ODR_OFFSET          (0x14)
#define GPIOx_BSRR_OFFSET         (0x18)

#define RCC_BASE_ADDR             ((unsigned int)(0x58024400))
#define RCC_AHB4RSTR              (*(volatile unsigned int*)(RCC_BASE_ADDR + 0x88))
#define RCC_AHB4ENR               (*(volatile unsigned int*)(RCC_BASE_ADDR + 0xE0))

/****** LED port assignement on Cortex-M Trace Reference Board ******/

#define NUM_LEDS                  3u

#define LEDPORT_BASE_ADDR         GPIOA_BASE_ADDR

#define LEDPORT_RCC_RSTR          RCC_AHB4RSTR
#define LEDPORT_RCC_ENR           RCC_AHB4ENR
#define LEDPORT_RCC_BIT           (0)


#define LED0_BIT                  (8)
#define LED1_BIT                  (9)
#define LED2_BIT                  (10)

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/
typedef struct _LED_INFO {
           int            PortPin;
  volatile unsigned int*  pModeReg;
  volatile unsigned int*  pReadReg;
  volatile unsigned int*  pSetReg;
} LED_INFO;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static LED_INFO _aLEDInfo[NUM_LEDS] = {
// Pin       Mode Register (GPIOx_MODER)                                        Read register (GPIOx_ODR)                                       Set register (GPIOx_BSRR)
  {LED0_BIT, (volatile unsigned int *)(LEDPORT_BASE_ADDR + GPIOx_MODER_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_ODR_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_BSRR_OFFSET)},  // LED0 GREEN
  {LED1_BIT, (volatile unsigned int *)(LEDPORT_BASE_ADDR + GPIOx_MODER_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_ODR_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_BSRR_OFFSET)},  // LED1 GREEN
  {LED2_BIT, (volatile unsigned int *)(LEDPORT_BASE_ADDR + GPIOx_MODER_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_ODR_OFFSET), (volatile unsigned int*)(LEDPORT_BASE_ADDR + GPIOx_BSRR_OFFSET)},  // LED2 GREEN
};

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
  unsigned int i;
  //
  // Initialize port for LEDs (sample application)
  //
  LEDPORT_RCC_ENR  &= ~(1uL << LEDPORT_RCC_BIT);
  LEDPORT_RCC_RSTR |=  (1uL << LEDPORT_RCC_BIT);
  LEDPORT_RCC_RSTR &= ~(1uL << LEDPORT_RCC_BIT);
  LEDPORT_RCC_ENR  |=  (1uL << LEDPORT_RCC_BIT);

  for (i = 0; i < NUM_LEDS; i++) {
    *(_aLEDInfo[i].pModeReg)  &= ~(3uL << (_aLEDInfo[i].PortPin * 2));   // Reset mode; sets port to input
    *(_aLEDInfo[i].pModeReg)  |=  (1uL << (_aLEDInfo[i].PortPin * 2));   // Set to output mode
    *(_aLEDInfo[i].pSetReg)    =  (1u << _aLEDInfo[i].PortPin);   // Initially clear LED
  }
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < (int)NUM_LEDS) {
    *(_aLEDInfo[Index].pSetReg) = (0x10000uL << _aLEDInfo[Index].PortPin);
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {  
  if (Index < (int)NUM_LEDS) {
    *(_aLEDInfo[Index].pSetReg) = (1u << _aLEDInfo[Index].PortPin);
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < (int)NUM_LEDS) {
    if ((*(_aLEDInfo[Index].pReadReg) & (1uL << _aLEDInfo[Index].PortPin)) == 0) {  // LED is switched off
      *(_aLEDInfo[Index].pSetReg) = (1uL << _aLEDInfo[Index].PortPin);              // Switch on LED
    } else {
      *(_aLEDInfo[Index].pSetReg) = (0x10000uL << _aLEDInfo[Index].PortPin);        // Switch off LED
    }
  }
}

/****** End Of File *************************************************/
