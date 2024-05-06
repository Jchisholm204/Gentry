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

File    : BSP_Memory.c
Purpose : This file initializes the SDRAM on the STM32f429 discovery.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"

/*********************************************************************
*
*       SFRs
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
#define DMA2_BASEADDR    ((unsigned int)0x40026400)
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

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
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

#define CMD_CLK_ENABLE       (1)
#define CMD_PALL             (2)
#define CMD_AUTOREFRESH_MODE (3)
#define CMD_LOAD_MODE        (4)
#define CMD_TARGET_BANK2     (8)
#define CMD_AUTOREFRESH_1    (1)
#define CMD_AUTOREFRESH_4    (4)

#define REFRESH_COUNT        (1386)   // SDRAM refresh counter

/*********************************************************************
*
*       Types, local
*
**********************************************************************
*/
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
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
  RCC_AHB1ENR |= (1u << 22);
  //
  // Disable DMA2
  //
  DMA2_S0CR  &= ~0x1u;
  //
  //
  //
  DMA2_S0CR   = 0x00025680u;
  DMA2_S0FCR  = 0x00000020u;
  //
  // Enable DMA2
  //
  DMA2_S0CR  |= 0x1u;
}

/*********************************************************************
*
*       _PinInit()
*
*  Pin configuration:
*-------------------+--------------------+--------------------+--------------------+
*                       SDRAM pins assignment                                      +
*-------------------+--------------------+--------------------+--------------------+
* PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   |
* PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   |
* PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG8  <-> FMC_SDCLK |
* PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG15 <-> FMC_NCAS  |
* PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    |--------------------+
* PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    |
* PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  |
*-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |
*                   | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |
*                   | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
*                   | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
*-------------------+--------------------+--------------------+
* PB5 <-> FMC_SDCKE1|
* PB6 <-> FMC_SDNE1 |
* PC0 <-> FMC_SDNWE |
*-------------------+
*/
static void _PinInit(void) {
  //
  // Enable FMC clock
  //
  RCC_AHB3ENR |= 1;
  //
  // Enable GPIOs clock
  //
  RCC_AHB1ENR |= 0x7E;
  //
  // GPIOB Init
  //
  GPIOx_AFRL(GPIOB_BASEADDR)     |= (0xCu << 20)  // PB5, alternate function 12
                                 |  (0xCu << 24)  // PB6, alternate function 12
                                 ;
  GPIOx_MODER(GPIOB_BASEADDR)    |= (0x2u << (GPIO_PIN_5 * 2))  // PB5, alternate function mode
                                 |  (0x2u << (GPIO_PIN_6 * 2))  // PB6, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOB_BASEADDR) |= (0x2u << (GPIO_PIN_5 * 2))  // PB5, fast speed
                                 |  (0x2u << (GPIO_PIN_6 * 2))  // PB6, fast speed
                                 ;

  //
  // GPIOC Init
  //
  GPIOx_AFRL(GPIOC_BASEADDR)     |= (0xCu << (GPIO_PIN_0 * 4))  // PB5, alternate function 12
                                 ;
  GPIOx_MODER(GPIOC_BASEADDR)    |= (0x2u << (GPIO_PIN_0 * 2))  // PB5, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOC_BASEADDR) |= (0x2u << (GPIO_PIN_0 * 2))  // PB5, fast speed
                                 ;

  //
  // GPIOD Init
  //
  GPIOx_AFRL(GPIOD_BASEADDR)     |= (0xCu << 0)  // PD0, alternate function 12
                                 |  (0xCu << 4)  // PD1, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOD_BASEADDR)     |= (0xCu << 0)  // PD8, alternate function 12
                                 |  (0xCu << 4)  // PD9, alternate function 12
                                 |  (0xCu << 8)  // PD10, alternate function 12
                                 |  (0xCu << 24) // PD14, alternate function 12
                                 |  (0xCu << 28) // PD15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOD_BASEADDR)    |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, alternate function mode
                                 |  (0x2u << (GPIO_PIN_9  * 2))  // PB9, alternate function mode
                                 |  (0x2u << (GPIO_PIN_10 * 2))  // PB10, alternate function mode
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOD_BASEADDR) |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x2u << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x2u << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;

  //
  // GPIOE Init
  //
  GPIOx_AFRL(GPIOE_BASEADDR)     |= (0xCu << 0)   // PB0, alternate function 12
                                 |  (0xCu << 4)   // PB1, alternate function 12
                                 |  (0xCu << 28)  // PB7, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOE_BASEADDR)     |= (0xCu << 0)   // PB8, alternate function 12
                                 |  (0xCu << 4)   // PB9, alternate function 12
                                 |  (0xCu << 8)   // PB10, alternate function 12
                                 |  (0xCu << 12)  // PB11, alternate function 12
                                 |  (0xCu << 16)  // PB12, alternate function 12
                                 |  (0xCu << 20)  // PB13, alternate function 12
                                 |  (0xCu << 24)  // PB14, alternate function 12
                                 |  (0xCu << 28)  // PB15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOE_BASEADDR)    |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2u << (GPIO_PIN_7  * 2))  // PB7, alternate function mode
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, alternate function mode
                                 |  (0x2u << (GPIO_PIN_9  * 2))  // PB9, alternate function mode
                                 |  (0x2u << (GPIO_PIN_10 * 2))  // PB10, alternate function mode
                                 |  (0x2u << (GPIO_PIN_11 * 2))  // PB11, alternate function mode
                                 |  (0x2u << (GPIO_PIN_12 * 2))  // PB12, alternate function mode
                                 |  (0x2u << (GPIO_PIN_13 * 2))  // PB13, alternate function mode
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOE_BASEADDR) |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2u << (GPIO_PIN_7 * 2))   // PB7, fast speed
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x2u << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x2u << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x2u << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x2u << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x2u << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  //
  // GPIOF Init
  //
  GPIOx_AFRL(GPIOF_BASEADDR)     |= (0xCu << 0)   // PB0, alternate function 12
                                 |  (0xCu << 4)   // PB1, alternate function 12
                                 |  (0xCu << 8)   // PB2, alternate function 12
                                 |  (0xCu << 12)  // PB3, alternate function 12
                                 |  (0xCu << 16)  // PB4, alternate function 12
                                 |  (0xCu << 20)  // PB5, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOF_BASEADDR)     |= (0xCu << 12)  // PB11, alternate function 12
                                 |  (0xCu << 16)  // PB12, alternate function 12
                                 |  (0xCu << 20)  // PB13, alternate function 12
                                 |  (0xCu << 24)  // PB14, alternate function 12
                                 |  (0xCu << 28)  // PB15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOF_BASEADDR)    |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2u << (GPIO_PIN_2  * 2))  // PB2, alternate function mode
                                 |  (0x2u << (GPIO_PIN_3  * 2))  // PB3, alternate function mode
                                 |  (0x2u << (GPIO_PIN_4  * 2))  // PB4, alternate function mode
                                 |  (0x2u << (GPIO_PIN_5  * 2))  // PB5, alternate function mode
                                 |  (0x2u << (GPIO_PIN_11 * 2))  // PB11, alternate function mode
                                 |  (0x2u << (GPIO_PIN_12 * 2))  // PB12, alternate function mode
                                 |  (0x2u << (GPIO_PIN_13 * 2))  // PB13, alternate function mode
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, alternate function mode
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOF_BASEADDR) |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2u << (GPIO_PIN_2  * 2))  // PB2, fast speed
                                 |  (0x2u << (GPIO_PIN_3  * 2))  // PB3, fast speed
                                 |  (0x2u << (GPIO_PIN_4  * 2))  // PB4, fast speed
                                 |  (0x2u << (GPIO_PIN_5  * 2))  // PB5,  fast speed
                                 |  (0x2u << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x2u << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x2u << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x2u << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  //
  // GPIOG Init
  //
  GPIOx_AFRL(GPIOG_BASEADDR)     |= (0xCu << 0)   // PB0, alternate function 12
                                 |  (0xCu << 4)   // PB1, alternate function 12
                                 |  (0xCu << 16)  // PB4, alternate function 12
                                 |  (0xCu << 20)  // PB5, alternate function 12
                                 ;
  GPIOx_AFRH(GPIOG_BASEADDR)     |= (0xCu << 0)   // PB8, alternate function 12
                                 |  (0xCu << 28)  // PB15, alternate function 12
                                 ;
  GPIOx_MODER(GPIOG_BASEADDR)    |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, alternate function mode
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, alternate function mode
                                 |  (0x2u << (GPIO_PIN_4  * 2))  // PB4, alternate function mode
                                 |  (0x2u << (GPIO_PIN_5  * 2))  // PB5, alternate function mode
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, alternate function mode
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, alternate function mode
                                 ;
  GPIOx_OSPEEDER(GPIOG_BASEADDR) |= (0x2u << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x2u << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x2u << (GPIO_PIN_4  * 2))  // PB4, fast speed
                                 |  (0x2u << (GPIO_PIN_5  * 2))  // PB5, fast speed
                                 |  (0x2u << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x2u << (GPIO_PIN_15 * 2))  // PB15, fast speed
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
      | CMD_TARGET_BANK2                // Target bank
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
      | CMD_TARGET_BANK2                // Target bank
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
      | CMD_TARGET_BANK2                // Target bank
      | ((CMD_AUTOREFRESH_4 - 1) << 5)  // Auto refresh number
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
      | CMD_TARGET_BANK2                // Target bank
      | ((CMD_AUTOREFRESH_1 - 1) << 5)  // Auto refresh number
      | (0x0230u << 9)                  // Mode register definition:
      ;                                 //   Burst length:     1
                                        //   Burst type:       Sequential
                                        //   CAS latency:      3
                                        //   Operating mode:   Standard
                                        //   Write burst mode: Single
  //
  // Send command
  //
  FMC_SDCMR = Cmd;
  while (FMC_SDSR & 0x20u);
  //
  // Set the refresh rate counter
  //
  FMC_SDRTR |= (REFRESH_COUNT << 1);
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
  // Pin and DMA initialization
  //
  _PinInit();
  _DMAInit();
  //
  // FMC Configuration
  // FMC SDRAM Bank configuration
  //
  FMC_SDCR1 = 0x00002ad0u;
  FMC_SDCR2 = 0x000001d4u;
  //
  // Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2)
  // 1 clock cycle = 11.11ns
  // TMRD: 22.22ns (2 x clock cycles)
  // TXSR: 77.77ns (7 x clock cycles)
  // TRAS: 44.44ns (4 x clock cycles)
  // TRC:  77.77ns (7 x clock cycles)
  // TWR:  22.22ns (2 x clock cycles)
  // TRP:  22.22ns (2 x clock cycles)
  // TRCD: 22.22ns (2 x clock cycles)
  //
  FMC_SDTR1 = 0x0f1f6fffu;
  FMC_SDTR2 = 0x01010361u;
  //
  // SDRAM initialization sequence
  //
  _SDRAM_InitSequence();
#if (defined(__ICCARM__))
  return 1;       // Always return 1 to enable segment initilization!
#endif
}

/*************************** End of file ****************************/
