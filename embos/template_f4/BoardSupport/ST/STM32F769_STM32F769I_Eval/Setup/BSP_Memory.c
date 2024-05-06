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
File    : BSP_Memory.c
Purpose : This file initializes the SDRAM on the STM32f769I Eval.
*/

#include "BSP.h"
#include <stdint.h>

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       RCC
*/
#define RCC_BASEADDR    ((unsigned int)0x40023800)
#define RCC_AHB1ENR     (*(volatile unsigned int*)(RCC_BASEADDR + 0x30))
#define RCC_AHB3ENR     (*(volatile unsigned int*)(RCC_BASEADDR + 0x38))

/*********************************************************************
*
*       FMC
*/
#define FMC_BASEADDR    ((unsigned int)0xA0000000)
#define FMC_SDCR1       (*(volatile unsigned int*)(FMC_BASEADDR + 0x140))
#define FMC_SDCR2       (*(volatile unsigned int*)(FMC_BASEADDR + 0x144))
#define FMC_SDTR1       (*(volatile unsigned int*)(FMC_BASEADDR + 0x148))
#define FMC_SDTR2       (*(volatile unsigned int*)(FMC_BASEADDR + 0x14C))
#define FMC_SDCMR       (*(volatile unsigned int*)(FMC_BASEADDR + 0x150))
#define FMC_SDRTR       (*(volatile unsigned int*)(FMC_BASEADDR + 0x154))
#define FMC_SDSR        (*(volatile unsigned int*)(FMC_BASEADDR + 0x158))

/*********************************************************************
*
*       DMA
*/
#define DMA2_BASEADDR   ((unsigned int)0x40026400)
#define DMA2_S0CR        (*(volatile unsigned int*)(DMA2_BASEADDR + 0x10))
#define DMA2_S0FCR       (*(volatile unsigned int*)(DMA2_BASEADDR + 0x24))

/*********************************************************************
*
*       GPIOs
*/
#define GPIOB_BASEADDR  ((unsigned int)0x40020400)
#define GPIOC_BASEADDR  ((unsigned int)0x40020800)
#define GPIOD_BASEADDR  ((unsigned int)0x40020C00)
#define GPIOE_BASEADDR  ((unsigned int)0x40021000)
#define GPIOF_BASEADDR  ((unsigned int)0x40021400)
#define GPIOG_BASEADDR  ((unsigned int)0x40021800)
#define GPIOH_BASEADDR  ((unsigned int)0x40021C00)
#define GPIOI_BASEADDR  ((unsigned int)0x40022000)

#define GPIOx_MODER(x)     (*(volatile unsigned int*)(x + 0x00))
#define GPIOx_OTYPER(x)    (*(volatile unsigned int*)(x + 0x04))
#define GPIOx_OSPEEDER(x)  (*(volatile unsigned int*)(x + 0x08))
#define GPIOx_PUPDR(x)     (*(volatile unsigned int*)(x + 0x0C))
#define GPIOx_IDR(x)       (*(volatile unsigned int*)(x + 0x10))
#define GPIOx_ODR(x)       (*(volatile unsigned int*)(x + 0x14))
#define GPIOx_BSRR(x)      (*(volatile unsigned int*)(x + 0x18))
#define GPIOx_LCKR (x)     (*(volatile unsigned int*)(x + 0x1C))
#define GPIOx_AFRL(x)      (*(volatile unsigned int*)(x + 0x20))
#define GPIOx_AFRH(x)      (*(volatile unsigned int*)(x + 0x24))

#define GPIO_PIN_0         (0)
#define GPIO_PIN_1         (1)
#define GPIO_PIN_2         (2)
#define GPIO_PIN_3         (3)
#define GPIO_PIN_4         (4)
#define GPIO_PIN_5         (5)
#define GPIO_PIN_6         (6)
#define GPIO_PIN_7         (7)
#define GPIO_PIN_8         (8)
#define GPIO_PIN_9         (9)
#define GPIO_PIN_10        (10)
#define GPIO_PIN_11        (11)
#define GPIO_PIN_12        (12)
#define GPIO_PIN_13        (13)
#define GPIO_PIN_14        (14)
#define GPIO_PIN_15        (15)

#define GPIOA_BitPos        (0)
#define GPIOB_BitPos        (1)
#define GPIOC_BitPos        (2)
#define GPIOD_BitPos        (3)
#define GPIOE_BitPos        (4)
#define GPIOF_BitPos        (5)
#define GPIOG_BitPos        (6)
#define GPIOH_BitPos        (7)
#define GPIOI_BitPos        (8)

#define CMD_CLK_ENABLE       (1)
#define CMD_PALL             (2)
#define CMD_AUTOREFRESH_MODE (3)
#define CMD_LOAD_MODE        (4)
#define CMD_TARGET_BANK1     (16)
#define CMD_TARGET_BANK2     (8)
#define CMD_AUTOREFRESH_1    (1)
#define CMD_AUTOREFRESH_4    (4)
#define CMD_AUTOREFRESH_8    (8)

#define REFRESH_COUNT        (1539)   // SDRAM refresh counter

/*********************************************************************
*
*       Types, local
*
**********************************************************************
*/
typedef struct{
  uint8_t   Enable;
  uint8_t   Number;
  uint32_t  BaseAddress;
  uint8_t   Size;
  uint8_t   SubRegionDisable;
  uint8_t   TypeExtField;
  uint8_t   AccessPermission;
  uint8_t   DisableExec;
  uint8_t   IsShareable;
  uint8_t   IsCacheable;
  uint8_t   IsBufferable;
} MPU_Region_InitTypeDef;

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _DMAInit()
*/
static void _DMAInit(void) {
  //
  // Init DMA2 clock
  //
  RCC_AHB1ENR |= (1 << 22);
  //
  // Disable DMA2
  //
  DMA2_S0CR  &= ~0x1;
  //
  //
  //
  DMA2_S0CR   = 0x00025680;
  DMA2_S0FCR  = 0x00000020;
  //
  // Enable DMA2
  //
  DMA2_S0CR  |= 0x1;
}

/*********************************************************************
*
*       _PinInit()
*/
static void _PinInit(void) {
  //
  // Enable FMC clock
  //
  RCC_AHB3ENR |= 1;
  //
  // Enable GPIOs clock
  //
  RCC_AHB1ENR |= (0x1ul << GPIOD_BitPos)
              |  (0x1ul << GPIOE_BitPos)
              |  (0x1ul << GPIOF_BitPos)
              |  (0x1ul << GPIOG_BitPos)
              |  (0x1ul << GPIOH_BitPos)
              |  (0x1ul << GPIOI_BitPos)
              ;
  //
  // GPIOD Init
  // Pin 0,1,8,9,10,14,15
  //
  GPIOx_AFRL(GPIOD_BASEADDR)     |= (0xCul << 0)  // PD0, alternate function 12
                                 |  (0xCul << 4)  // PD1, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOD_BASEADDR)     |= (0xCul << 0)  // PD8, alternate function 12
                                 |  (0xCul << 4)  // PD9, alternate function 12
                                 |  (0xCul << 8)  // PD10, alternate function 12
                                 |  (0xCul << 24) // PD14, alternate function 12
                                 |  (0xCul << 28) // PD15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOD_BASEADDR)    |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_8  * 2))  // PB8, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_9  * 2))  // PB9, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_10 * 2))  // PB10, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOD_BASEADDR) |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2ul << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x2ul << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x2ul << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  // set Pullup with GPIOx_PUPDR
  GPIOx_PUPDR(GPIOD_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))  // PB0, pull-up
                                 |  (0x1ul << (GPIO_PIN_1  * 2))  // PB1, pull-up
                                 |  (0x1ul << (GPIO_PIN_8  * 2))  // PB8, pull-up
                                 |  (0x1ul << (GPIO_PIN_9  * 2))  // PB9, pull-up
                                 |  (0x1ul << (GPIO_PIN_10 * 2))  // PB10, pull-up
                                 |  (0x1ul << (GPIO_PIN_14 * 2))  // PB14, pull-up
                                 |  (0x1ul << (GPIO_PIN_15 * 2))  // PB15, pull-up
                                 ;
  //
  // GPIOE Init
  // Pin 0,1,7,8,9,10,11,12,13,14,15
  //
  GPIOx_AFRL(GPIOE_BASEADDR)     |= (0xCul << 0)   // PB0, alternate function 12
                                 |  (0xCul << 4)   // PB1, alternate function 12
                                 |  (0xCul << 28)  // PB7, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOE_BASEADDR)     |= (0xCul << 0)   // PB8, alternate function 12
                                 |  (0xCul << 4)   // PB9, alternate function 12
                                 |  (0xCul << 8)   // PB10, alternate function 12
                                 |  (0xCul << 12)  // PB11, alternate function 12
                                 |  (0xCul << 16)  // PB12, alternate function 12
                                 |  (0xCul << 20)  // PB13, alternate function 12
                                 |  (0xCul << 24)  // PB14, alternate function 12
                                 |  (0xCul << 28)  // PB15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOE_BASEADDR)    |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_7  * 2))  // PB7, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_8  * 2))  // PB8, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_9  * 2))  // PB9, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_10 * 2))  // PB10, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_11 * 2))  // PB11, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_12 * 2))  // PB12, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_13 * 2))  // PB13, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOE_BASEADDR) |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2ul << (GPIO_PIN_7  * 2))   // PB7, fast speed
                                 |  (0x2ul << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x2ul << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x2ul << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x2ul << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x2ul << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x2ul << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  // set Pullup with GPIOx_PUPDR
  GPIOx_PUPDR(GPIOE_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x1ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x1ul << (GPIO_PIN_7  * 2))  // PB7, fast speed
                                 |  (0x1ul << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x1ul << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x1ul << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x1ul << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x1ul << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x1ul << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x1ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x1ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  //
  // GPIOF Init
  // Pin 0,1,2,3,4,5,11,12,13,14,15
  //
  GPIOx_AFRL(GPIOF_BASEADDR)     |= (0xCul << 0)   // PB0, alternate function 12
                                 |  (0xCul << 4)   // PB1, alternate function 12
                                 |  (0xCul << 8)   // PB2, alternate function 12
                                 |  (0xCul << 12)  // PB3, alternate function 12
                                 |  (0xCul << 16)  // PB4, alternate function 12
                                 |  (0xCul << 20)  // PB5, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOF_BASEADDR)     |= (0xCul << 12)  // PB11, alternate function 12
                                 |  (0xCul << 16)  // PB12, alternate function 12
                                 |  (0xCul << 20)  // PB13, alternate function 12
                                 |  (0xCul << 24)  // PB14, alternate function 12
                                 |  (0xCul << 28)  // PB15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOF_BASEADDR)    |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_2  * 2))  // PB2, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_3  * 2))  // PB3, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_4  * 2))  // PB4, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_5  * 2))  // PB5, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_11 * 2))  // PB11, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_12 * 2))  // PB12, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_13 * 2))  // PB13, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOF_BASEADDR) |= (0x2ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2ul << (GPIO_PIN_2  * 2))  // PB2, fast speed
                                 |  (0x2ul << (GPIO_PIN_3  * 2))  // PB3, fast speed
                                 |  (0x2ul << (GPIO_PIN_4  * 2))  // PB4, fast speed
                                 |  (0x2ul << (GPIO_PIN_5  * 2))  // PB5,  fast speed
                                 |  (0x2ul << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x2ul << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x2ul << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x2ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  // set GPIO_PULLUP with GPIOx_PUPDR
  GPIOx_PUPDR(GPIOF_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))
                                 |  (0x1ul << (GPIO_PIN_1  * 2))
                                 |  (0x1ul << (GPIO_PIN_2  * 2))
                                 |  (0x1ul << (GPIO_PIN_3  * 2))
                                 |  (0x1ul << (GPIO_PIN_4  * 2))
                                 |  (0x1ul << (GPIO_PIN_5  * 2))
                                 |  (0x1ul << (GPIO_PIN_11 * 2))
                                 |  (0x1ul << (GPIO_PIN_12 * 2))
                                 |  (0x1ul << (GPIO_PIN_13 * 2))
                                 |  (0x1ul << (GPIO_PIN_14 * 2))
                                 |  (0x1ul << (GPIO_PIN_15 * 2))
                                 ;
  //
  // GPIOG Init
  // Pin 0,1,2,4,5,8,15
  // Alternate function 12
  //
  GPIOx_AFRL(GPIOG_BASEADDR)     |= (0xCul << (GPIO_PIN_0  * 4))
                                 |  (0xCul << (GPIO_PIN_1  * 4))
                                 |  (0xCul << (GPIO_PIN_2  * 4))
                                 |  (0xCul << (GPIO_PIN_4  * 4))
                                 |  (0xCul << (GPIO_PIN_5  * 4))
                                 ;
  GPIOx_AFRH(GPIOG_BASEADDR)     |= (0xCul << ((GPIO_PIN_8  - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_15 - GPIO_PIN_8) * 4))
                                 ;
  // Set Alternate Function Push Pull Mode (0x02)
  GPIOx_MODER(GPIOG_BASEADDR)    |= (0x2ul << (GPIO_PIN_0  * 2))
                                 |  (0x2ul << (GPIO_PIN_1  * 2))
                                 |  (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_4  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_8  * 2))
                                 |  (0x2ul << (GPIO_PIN_15 * 2))
                                 ;
  // set GPIO_SPEED to "fast speed" = 0x02
  GPIOx_OSPEEDER(GPIOG_BASEADDR) |= (0x2ul << (GPIO_PIN_0  * 2))
                                 |  (0x2ul << (GPIO_PIN_1  * 2))
                                 |  (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_4  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_8  * 2))
                                 |  (0x2ul << (GPIO_PIN_15 * 2))
                                 ;
  // set GPIO_PULLUP("Pull-up activation") (0x01)
  GPIOx_PUPDR(GPIOG_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))
                                 |  (0x1ul << (GPIO_PIN_1  * 2))
                                 |  (0x1ul << (GPIO_PIN_2  * 2))
                                 |  (0x1ul << (GPIO_PIN_4  * 2))
                                 |  (0x1ul << (GPIO_PIN_5  * 2))
                                 |  (0x1ul << (GPIO_PIN_8  * 2))
                                 |  (0x1ul << (GPIO_PIN_15 * 2))
                                 ;
  //
  // GPIOH Init
  // Pin 2,3,5,8,9,10,11,12,13,14,15
  // Alternate function 12
  //
  GPIOx_AFRL(GPIOH_BASEADDR)     |= (0xCul << (GPIO_PIN_2  * 4))
                                 |  (0xCul << (GPIO_PIN_3  * 4))
                                 |  (0xCul << (GPIO_PIN_5  * 4))
                                 ;
  GPIOx_AFRH(GPIOH_BASEADDR)     |= (0xCul << ((GPIO_PIN_8  - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_9  - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_10 - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_11 - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_12 - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_13 - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_14 - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_15 - GPIO_PIN_8) * 4))
                                 ;
  // Set Alternate Function Push Pull Mode (0x02)
  GPIOx_MODER(GPIOH_BASEADDR)    |= (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_3  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_8  * 2))
                                 |  (0x2ul << (GPIO_PIN_9  * 2))
                                 |  (0x2ul << (GPIO_PIN_10 * 2))
                                 |  (0x2ul << (GPIO_PIN_11 * 2))
                                 |  (0x2ul << (GPIO_PIN_12 * 2))
                                 |  (0x2ul << (GPIO_PIN_13 * 2))
                                 |  (0x2ul << (GPIO_PIN_14 * 2))
                                 |  (0x2ul << (GPIO_PIN_15 * 2))
                                 ;
  // set GPIO_SPEED to "fast speed" = 0x02
  GPIOx_OSPEEDER(GPIOH_BASEADDR) |= (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_3  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_8  * 2))
                                 |  (0x2ul << (GPIO_PIN_9  * 2))
                                 |  (0x2ul << (GPIO_PIN_10 * 2))
                                 |  (0x2ul << (GPIO_PIN_11 * 2))
                                 |  (0x2ul << (GPIO_PIN_12 * 2))
                                 |  (0x2ul << (GPIO_PIN_13 * 2))
                                 |  (0x2ul << (GPIO_PIN_14 * 2))
                                 |  (0x2ul << (GPIO_PIN_15 * 2))
                                 ;
  // set GPIO_PULLUP("Pull-up activation") (0x01)
  GPIOx_PUPDR(GPIOH_BASEADDR)    |= (0x1ul << (GPIO_PIN_2  * 2))
                                 |  (0x1ul << (GPIO_PIN_3  * 2))
                                 |  (0x1ul << (GPIO_PIN_5  * 2))
                                 |  (0x1ul << (GPIO_PIN_8  * 2))
                                 |  (0x1ul << (GPIO_PIN_9  * 2))
                                 |  (0x1ul << (GPIO_PIN_10 * 2))
                                 |  (0x1ul << (GPIO_PIN_11 * 2))
                                 |  (0x1ul << (GPIO_PIN_12 * 2))
                                 |  (0x1ul << (GPIO_PIN_13 * 2))
                                 |  (0x1ul << (GPIO_PIN_14 * 2))
                                 |  (0x1ul << (GPIO_PIN_15 * 2))
                                 ;

  //
  // GPIOI Init
  // Pin 0,1,2,3,4,5,6,7,9,10
  // Alternate function 12
  //
  GPIOx_AFRL(GPIOI_BASEADDR)     |= (0xCul << (GPIO_PIN_0  * 4))
                                 |  (0xCul << (GPIO_PIN_1  * 4))
                                 |  (0xCul << (GPIO_PIN_2  * 4))
                                 |  (0xCul << (GPIO_PIN_3  * 4))
                                 |  (0xCul << (GPIO_PIN_4  * 4))
                                 |  (0xCul << (GPIO_PIN_5  * 4))
                                 |  (0xCul << (GPIO_PIN_6  * 4))
                                 |  (0xCul << (GPIO_PIN_7  * 4))
                                 ;
  GPIOx_AFRH(GPIOI_BASEADDR)     |= (0xCul << ((GPIO_PIN_9  - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_10 - GPIO_PIN_8) * 4))
                                 ;
  // Set Alternate Function Push Pull Mode (0x02)
  GPIOx_MODER(GPIOI_BASEADDR)    |= (0x2ul << (GPIO_PIN_0  * 2))
                                 |  (0x2ul << (GPIO_PIN_1  * 2))
                                 |  (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_3  * 2))
                                 |  (0x2ul << (GPIO_PIN_4  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_6  * 2))
                                 |  (0x2ul << (GPIO_PIN_7  * 2))
                                 |  (0x2ul << (GPIO_PIN_9  * 2))
                                 |  (0x2ul << (GPIO_PIN_10 * 2))
                                 ;
  // set GPIO_SPEED to "fast speed" = 0x02
  GPIOx_OSPEEDER(GPIOI_BASEADDR) |= (0x2ul << (GPIO_PIN_0  * 2))
                                 |  (0x2ul << (GPIO_PIN_1  * 2))
                                 |  (0x2ul << (GPIO_PIN_2  * 2))
                                 |  (0x2ul << (GPIO_PIN_3  * 2))
                                 |  (0x2ul << (GPIO_PIN_4  * 2))
                                 |  (0x2ul << (GPIO_PIN_5  * 2))
                                 |  (0x2ul << (GPIO_PIN_6  * 2))
                                 |  (0x2ul << (GPIO_PIN_7  * 2))
                                 |  (0x2ul << (GPIO_PIN_9  * 2))
                                 |  (0x2ul << (GPIO_PIN_10 * 2))
                                 ;
  // set GPIO_PULLUP("Pull-up activation") (0x01)
  GPIOx_PUPDR(GPIOI_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))
                                 |  (0x1ul << (GPIO_PIN_1  * 2))
                                 |  (0x1ul << (GPIO_PIN_2  * 2))
                                 |  (0x1ul << (GPIO_PIN_3  * 2))
                                 |  (0x1ul << (GPIO_PIN_4  * 2))
                                 |  (0x1ul << (GPIO_PIN_5  * 2))
                                 |  (0x1ul << (GPIO_PIN_6  * 2))
                                 |  (0x1ul << (GPIO_PIN_7  * 2))
                                 |  (0x1ul << (GPIO_PIN_9  * 2))
                                 |  (0x1ul << (GPIO_PIN_10 * 2))
                                 ;
}

/*********************************************************************
*
*       _SDRAM_InitSequence()
*/
static void _SDRAM_InitSequence(void) {
  unsigned int Cmd;

  //
  // Configure a clock configuration enable command
  //
  Cmd = 0
      | CMD_CLK_ENABLE                  // Mode
      | CMD_TARGET_BANK1                // Target bank
      | ((CMD_AUTOREFRESH_1 - 1) << 5)  // Auto refresh number
      | (0 << 9)                        // Mode register definition
      ;
  //
  // Send command
  //
  FMC_SDCMR = Cmd;
  while (FMC_SDSR & 0x20);
  //
  // Configure a PALL (precharge all) command
  //
  Cmd = 0
      | CMD_PALL                        // Mode
      | CMD_TARGET_BANK1                // Target bank
      | ((CMD_AUTOREFRESH_1 - 1) << 5)  // Auto refresh number
      | (0 << 9)                        // Mode register definition
      ;
  //
  // Send command
  //
  FMC_SDCMR = Cmd;
  while (FMC_SDSR & 0x20);
  //
  // Configure an Auto Refresh command
  //
  Cmd = 0
      | CMD_AUTOREFRESH_MODE            // Mode
      | CMD_TARGET_BANK1                // Target bank
      | ((CMD_AUTOREFRESH_8 - 1) << 5)  // Auto refresh number
      | (0 << 9)                        // Mode register definition
      ;
  //
  // Send command
  //
  FMC_SDCMR = Cmd;
  while (FMC_SDSR & 0x20);
  //
  // Program the external memory mode register
  //
  Cmd = 0
      | CMD_LOAD_MODE                   // Mode
      | CMD_TARGET_BANK1                // Target bank
      | ((CMD_AUTOREFRESH_1 - 1) << 5)  // Auto refresh number
      | (0x0230ul << 9)                 // Mode register definition:
      ;                                 //   Burst length:     1
                                        //   Burst type:       Sequential
                                        //   CAS latency:      3
                                        //   Operating mode:   Standard
                                        //   Write burst mode: Single
  //
  // Send command
  //
  FMC_SDCMR = Cmd;
  while (FMC_SDSR & 0x20);
  //
  // Set the refresh rate counter
  //
  FMC_SDRTR |= (REFRESH_COUNT << 1);
}

/*********************************************************************
*
*       _Init_SDRAM()
*/
static void _Init_SDRAM(void) {
  //
  // Pin and DMA initialization
  //
  _PinInit();
  _DMAInit();
  //
  // FMC Configuration
  // FMC SDRAM Bank configuration
  //
  FMC_SDCR1 = 0x000019e5;
  //
  // Timing configuration
  //
  FMC_SDTR1 = 0x01116361;
  //
  // SDRAM initialization sequence
  //
  _SDRAM_InitSequence();
}

void HAL_MPU_Disable(void);
void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef*);
void HAL_MPU_Enable(uint32_t);
#define  MPU_REGION_ENABLE              ((uint8_t)0x01U)
#define  MPU_REGION_DISABLE             ((uint8_t)0x00U)
#define  MPU_REGION_SIZE_8MB            ((uint8_t)0x16U)
#define  MPU_REGION_SIZE_256MB          ((uint8_t)0x1BU)
#define  MPU_REGION_FULL_ACCESS         ((uint8_t)0x03U)
#define  MPU_ACCESS_NOT_BUFFERABLE      ((uint8_t)0x00U)
#define  MPU_ACCESS_CACHEABLE           ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_CACHEABLE       ((uint8_t)0x00U)
#define  MPU_ACCESS_NOT_SHAREABLE       ((uint8_t)0x00U)
#define  MPU_REGION_NUMBER0             ((uint8_t)0x00U)
#define  MPU_REGION_NUMBER1             ((uint8_t)0x01U)
#define  MPU_TEX_LEVEL0                 ((uint8_t)0x00U)
#define  MPU_INSTRUCTION_ACCESS_ENABLE  ((uint8_t)0x00U)
#define  MPU_PRIVILEGED_DEFAULT         ((uint32_t)0x00000004U)

/*********************************************************************
*
*       _MPU_Config()
*
*  Function description:
*    Re-configure device memory type in MPU in order to avoid
*    Hardfaults caused by unaligned accesses to SDRAM.
*/
static void _MPU_Config(void) {
  MPU_Region_InitTypeDef MPU_InitStruct;

  //
  // Disable the MPU
  //
  HAL_MPU_Disable();
  //
  // Configure the MPU attributes for SDRAM
  //
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  //
  // Configure the MPU attributes for SDRAM
  //
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  //
  // Enable MPU
  //
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       MemoryInit()
*
*  Function description
*    Initializes memories.
*
*  Notes
*    This routine is called before the segment initialization.
*
*    MemoryInit() is called from the Embedded Studio startup code
*    when the define MEMORY_INIT is set.
*    __low_level_init() is called from the IAR startup code.
*/
#if (defined(__ICCARM__))
INTERWORK int __low_level_init(void) {
#else
void MemoryInit(void) {
#endif
  //
  // Set up MPU
  //
  _MPU_Config();
  //
  // Init SDRAM
  //
  _Init_SDRAM();

#if (defined(__ICCARM__))
  return 1;       // Always return 1 to enable segment initilization!
#endif
}

/*************************** End of file ****************************/
