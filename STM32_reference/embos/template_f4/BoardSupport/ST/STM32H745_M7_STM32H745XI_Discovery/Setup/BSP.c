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
Purpose : BSP for STM32H745I Discovery eval board
*/

#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#ifdef CORE_CM7
  #define LED_BASE_INDEX     ( 1)  // Start with the green LED on pseudo index 0.
  #define NUM_LEDS           ( 2)  // Use the green and red LED for a nice blinky on the CM7.
#else
  #define LED_BASE_INDEX     ( 0)  // Start with the green LED on real index 0.
  #define NUM_LEDS           ( 1)  // Use the single green LED for blinky on the CM4.
#endif

#define LED0                 ( 3)  // Green, LD8, PD3
#define LED1                 ( 2)  // Green, LD7, PJ2,  active low
#define LED2                 (13)  // Red,   LD6, PI13, active low

//
// General Purpose Input/Output
//
#define GPIOD_BASE_ADDR      (0x58020C00u)
#define GPIOD_MODER          (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x00u))  // Mode register
#define GPIOD_OTYPER         (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x04u))  // Output type register
#define GPIOD_OSPEEDR        (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x08u))  // Output speed register
#define GPIOD_PUPDR          (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x0Cu))  // Pull-up/pull-down register
#define GPIOD_ODR            (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x14u))  // Output data register
#define GPIOD_BSRR           (*(volatile unsigned int*)(GPIOD_BASE_ADDR + 0x18u))  // Bit set/reset register

#define GPIOI_BASE_ADDR      (0x58022000u)
#define GPIOI_MODER          (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x00u))  // Mode register
#define GPIOI_OTYPER         (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x04u))  // Output type register
#define GPIOI_OSPEEDR        (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x08u))  // Output speed register
#define GPIOI_PUPDR          (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x0Cu))  // Pull-up/pull-down register
#define GPIOI_ODR            (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x14u))  // Output data register
#define GPIOI_BSRR           (*(volatile unsigned int*)(GPIOI_BASE_ADDR + 0x18u))  // Bit set/reset register

#define GPIOJ_BASE_ADDR      (0x58022400u)
#define GPIOJ_MODER          (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x00u))  // Mode register
#define GPIOJ_OTYPER         (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x04u))  // Output type register
#define GPIOJ_OSPEEDR        (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x08u))  // Output speed register
#define GPIOJ_PUPDR          (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x0Cu))  // Pull-up/pull-down register
#define GPIOJ_ODR            (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x14u))  // Output data register
#define GPIOJ_BSRR           (*(volatile unsigned int*)(GPIOJ_BASE_ADDR + 0x18u))  // Bit set/reset register

//
// Reset and Clock Control
//
#define RCC_BASE_ADDR        (0x58024400u)
#define RCC_AHB4ENR          (*(volatile unsigned int*)(RCC_BASE_ADDR + 0xE0u))    // AHB4 clock register

#define RCC_AHB4ENR_GPIODEN  (1u << 3)
#define RCC_AHB4ENR_GPIOIEN  (1u << 8)
#define RCC_AHB4ENR_GPIOJEN  (1u << 9)

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
  unsigned long v;

  v = RCC_AHB4ENR_GPIODEN
    | RCC_AHB4ENR_GPIOIEN
    | RCC_AHB4ENR_GPIOJEN;
  //
  // Try to detect if the other core is running and has already
  // executed the same setup already.
  //
  if ((RCC_AHB4ENR & v) != v) {
    //
    // Enable GPIO clocks
    //
    RCC_AHB4ENR   |= RCC_AHB4ENR_GPIODEN
                  |  RCC_AHB4ENR_GPIOIEN
                  |  RCC_AHB4ENR_GPIOJEN;
    //
    // Initialize LD8
    //
    GPIOD_MODER   &= ~(0x3u << (LED0 * 2));    // Clear mode register
    GPIOD_MODER   |=  (0x1u << (LED0 * 2));    // Set IO direction to output mode
    GPIOD_OSPEEDR |=  (0x3u << (LED0 * 2));    // Set speed to high frequency
    GPIOD_OTYPER  &= ~(0x1u << (LED0 * 1));    // Set output to push-pull
    GPIOD_PUPDR   &= ~(0x3u << (LED0 * 2));    // Clear the pull-up/pull-down register
    GPIOD_PUPDR   |=  (0x1u << (LED0 * 2));    // Set push-pull to pull-up
    GPIOD_BSRR     =  ((0x1u << 16) << LED0);  // Turn LED off
    //
    // Initialize LD7
    //
    GPIOJ_MODER   &= ~(0x3u << (LED1 * 2));    // Clear mode register
    GPIOJ_MODER   |=  (0x1u << (LED1 * 2));    // Set IO direction to output mode
    GPIOJ_OSPEEDR |=  (0x3u << (LED1 * 2));    // Set speed to high frequency
    GPIOJ_OTYPER  &= ~(0x1u << (LED1 * 1));    // Set output to push-pull
    GPIOJ_PUPDR   &= ~(0x3u << (LED1 * 2));    // Clear the pull-up/pull-down register
    GPIOJ_PUPDR   |=  (0x1u << (LED1 * 2));    // Set push-pull to pull-up
    GPIOJ_BSRR     =  (0x1u << LED1);          // Turn LED off
    //
    // Initialize LD6
    //
    GPIOI_MODER   &= ~(0x3u << (LED2 * 2));    // Clear mode register
    GPIOI_MODER   |=  (0x1u << (LED2 * 2));    // Set IO direction to output mode
    GPIOI_OSPEEDR |=  (0x3u << (LED2 * 2));    // Set speed to high frequency
    GPIOI_OTYPER  &= ~(0x1u << (LED2 * 1));    // Set output to push-pull
    GPIOI_PUPDR   &= ~(0x3u << (LED2 * 2));    // Clear the pull-up/pull-down register
    GPIOI_PUPDR   |=  (0x1u << (LED2 * 2));    // Set push-pull to pull-up
    GPIOI_BSRR     =  (0x1u << LED2);          // Turn LED off
  }
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  Index += LED_BASE_INDEX;
  if (Index < (LED_BASE_INDEX + NUM_LEDS)) {
    if (Index == 0) {
      GPIOD_BSRR = (0x1u << LED0);             // Turn LED on
    } else if (Index == 1) {
      GPIOJ_BSRR = ((0x1u << 16) << LED1);     // Turn LED off
    } else if (Index == 2) {
      GPIOI_BSRR = ((0x1u << 16) << LED2);     // Turn LED off
    }
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  Index += LED_BASE_INDEX;
  if (Index < (LED_BASE_INDEX + NUM_LEDS)) {
    if (Index == 0) {
      GPIOD_BSRR = ((0x1u << 16) << LED0);     // Turn LED off
    }else if (Index == 1) {
      GPIOJ_BSRR = (0x1u << LED1);             // Turn LED on
    } else if (Index == 2) {
      GPIOI_BSRR = (0x1u << LED2);             // Turn LED on
    }
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  Index += LED_BASE_INDEX;
  if (Index < (LED_BASE_INDEX + NUM_LEDS)) {
    if (Index == 0) {
      GPIOD_ODR ^= (0x1u << LED0);             // Toggle LED
    } else if (Index == 1) {
      GPIOJ_ODR ^= (0x1u << LED1);             // Toggle LED
    } else if (Index == 2) {
      GPIOI_ODR ^= (0x1u << LED2);             // Toggle LED
    }
  }
}

/*************************** End of file ****************************/
