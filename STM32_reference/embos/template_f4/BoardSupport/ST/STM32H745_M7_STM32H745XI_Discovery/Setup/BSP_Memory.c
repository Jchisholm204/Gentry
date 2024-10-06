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
Purpose : Initialization for external memories.
*/

#include "BSP.h"
#include "stm32h7xx.h"
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
#define RCC_BASEADDR    ((unsigned int)0x58024400)
#define RCC_AHB3ENR     (*(volatile unsigned int*)(RCC_BASEADDR + 0x0D4))
#define RCC_AHB4ENR     (*(volatile unsigned int*)(RCC_BASEADDR + 0x0E0))

/*********************************************************************
*
*       FMC
*/
#define FMC_BASEADDR    ((unsigned int)0x52004000)
#define FMC_BCR1        (*(volatile unsigned int*)(FMC_BASEADDR + 0x000))
#define FMC_BCR2        (*(volatile unsigned int*)(FMC_BASEADDR + 0x008))
#define FMC_SDCR1       (*(volatile unsigned int*)(FMC_BASEADDR + 0x140))
#define FMC_SDCR2       (*(volatile unsigned int*)(FMC_BASEADDR + 0x144))
#define FMC_SDTR1       (*(volatile unsigned int*)(FMC_BASEADDR + 0x148))
#define FMC_SDTR2       (*(volatile unsigned int*)(FMC_BASEADDR + 0x14C))
#define FMC_SDCMR       (*(volatile unsigned int*)(FMC_BASEADDR + 0x150))
#define FMC_SDRTR       (*(volatile unsigned int*)(FMC_BASEADDR + 0x154))
#define FMC_SDSR        (*(volatile unsigned int*)(FMC_BASEADDR + 0x158))

/*********************************************************************
*
*       MDMA
*/
#define MDMA_BASEADDR    ((unsigned int)0x52000000)
#define MDMA_GISR0       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x00))
#define MDMA_C0ISR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x40))
#define MDMA_C0IFCR      (*(volatile unsigned int*)(MDMA_BASEADDR + 0x44))
#define MDMA_C0ESR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x48))
#define MDMA_C0CR        (*(volatile unsigned int*)(MDMA_BASEADDR + 0x4C))
#define MDMA_C0TCR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x50))
#define MDMA_C0BNDTR     (*(volatile unsigned int*)(MDMA_BASEADDR + 0x54))
#define MDMA_C0SAR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x58))
#define MDMA_C0DAR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x5C))
#define MDMA_C0BRUR      (*(volatile unsigned int*)(MDMA_BASEADDR + 0x60))
#define MDMA_C0LAR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x64))
#define MDMA_C0TBR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x68))
#define MDMA_C0MAR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x70))
#define MDMA_C0MDR       (*(volatile unsigned int*)(MDMA_BASEADDR + 0x74))

/*********************************************************************
*
*       GPIOs
*/
#define GPIOB_BASEADDR  ((unsigned int)0x58020400)
#define GPIOC_BASEADDR  ((unsigned int)0x58020800)
#define GPIOD_BASEADDR  ((unsigned int)0x58020C00)
#define GPIOE_BASEADDR  ((unsigned int)0x58021000)
#define GPIOF_BASEADDR  ((unsigned int)0x58021400)
#define GPIOG_BASEADDR  ((unsigned int)0x58021800)
#define GPIOH_BASEADDR  ((unsigned int)0x58021C00)
#define GPIOI_BASEADDR  ((unsigned int)0x58022000)

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

#define GPIO_PIN_0              (0)
#define GPIO_PIN_1              (1)
#define GPIO_PIN_2              (2)
#define GPIO_PIN_3              (3)
#define GPIO_PIN_4              (4)
#define GPIO_PIN_5              (5)
#define GPIO_PIN_6              (6)
#define GPIO_PIN_7              (7)
#define GPIO_PIN_8              (8)
#define GPIO_PIN_9              (9)
#define GPIO_PIN_10            (10)
#define GPIO_PIN_11            (11)
#define GPIO_PIN_12            (12)
#define GPIO_PIN_13            (13)
#define GPIO_PIN_14            (14)
#define GPIO_PIN_15            (15)

#define GPIOA_BitPos            (0)
#define GPIOB_BitPos            (1)
#define GPIOC_BitPos            (2)
#define GPIOD_BitPos            (3)
#define GPIOE_BitPos            (4)
#define GPIOF_BitPos            (5)
#define GPIOG_BitPos            (6)
#define GPIOH_BitPos            (7)
#define GPIOI_BitPos            (8)
#define GPIOJ_BitPos            (9)

#define REFRESH_COUNT        (1539)  // SDRAM refresh counter

#define MDMA_REQUEST_SW                   ((uint32_t)0x40000000U)
#define MDMA_BLOCK_TRANSFER               ((uint32_t)MDMA_CTCR_TRGM_0)
#define MDMA_PRIORITY_HIGH                ((uint32_t)MDMA_CCR_PL_1)
#define MDMA_LITTLE_ENDIANNESS_PRESERVE   ((uint32_t)0x00000000U)
#define MDMA_SRC_INC_WORD                 ((uint32_t)MDMA_CTCR_SINC_1 | (uint32_t)MDMA_CTCR_SINCOS_1)
#define MDMA_DEST_INC_WORD                ((uint32_t)MDMA_CTCR_DINC_1 | (uint32_t)MDMA_CTCR_DINCOS_1)
#define MDMA_SRC_DATASIZE_WORD            ((uint32_t)MDMA_CTCR_SSIZE_1)
#define MDMA_DEST_DATASIZE_WORD           ((uint32_t)MDMA_CTCR_DSIZE_1)
#define MDMA_DATAALIGN_PACKENABLE         ((uint32_t)MDMA_CTCR_PKE)
#define MDMA_SOURCE_BURST_SINGLE          ((uint32_t)0x00000000U)
#define MDMA_DEST_BURST_SINGLE            ((uint32_t)0x00000000U)

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

//
// SDRAM Command mode register
//
#define MT48LC4M32B2_NORMAL_MODE_CMD             0x00000000U
#define MT48LC4M32B2_CLK_ENABLE_CMD              0x00000001U
#define MT48LC4M32B2_PALL_CMD                    0x00000002U
#define MT48LC4M32B2_AUTOREFRESH_MODE_CMD        0x00000003U
#define MT48LC4M32B2_LOAD_MODE_CMD               0x00000004U
#define MT48LC4M32B2_SELFREFRESH_MODE_CMD        0x00000005U
#define MT48LC4M32B2_POWERDOWN_MODE_CMD          0x00000006U

#define WRITE_INTO_REG(Reg, ClrMask, NewBits)    Reg &= ~(ClrMask); \
                                                 Reg |=   NewBits;

/*********************************************************************
*
*       Types, local
*
**********************************************************************
*/
typedef struct {
  uint32_t Request;
  uint32_t TransferTriggerMode;
  uint32_t Priority;
  uint32_t Endianness;
  uint32_t SourceInc;
  uint32_t DestinationInc;
  uint32_t SourceDataSize;
  uint32_t DestDataSize;
  uint32_t DataAlignment;
  uint32_t BufferTransferLength;
  uint32_t SourceBurst;
  uint32_t DestBurst;
  int32_t  SourceBlockAddressOffset;
  int32_t  DestBlockAddressOffset;
} MDMA_InitTypeDef;

typedef struct {
  uint32_t CommandMode;
  uint32_t CommandTarget;
  uint32_t AutoRefreshNumber;
  uint32_t ModeRegisterDefinition;
} FMC_SDRAM_CommandTypeDef;

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
} MPU_InitTypeDef;

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _MDMA_ProcessInit()
*/
static void _MDMA_ProcessInit(MDMA_InitTypeDef * pInit) {
  //
  // Reset MDMA channel control register
  //
  MDMA_C0CR    = 0;
  MDMA_C0TCR   = 0;
  MDMA_C0BNDTR = 0;
  MDMA_C0SAR   = 0;
  MDMA_C0DAR   = 0;
  MDMA_C0BRUR  = 0;
  MDMA_C0LAR   = 0;
  MDMA_C0TBR   = 0;
  MDMA_C0MAR   = 0;
  MDMA_C0MDR   = 0;
  //
  // Prepare MDMA channel configuration
  //
  MDMA_C0CR = pInit->Priority | pInit->Endianness;
  //
  // Write new CTCR Register value
  //
  MDMA_C0TCR =  pInit->SourceInc      | pInit->DestinationInc            |
                pInit->SourceDataSize | pInit->DestDataSize              |
                pInit->DataAlignment  | pInit->SourceBurst               |
                pInit->DestBurst                                         |
              ((pInit->BufferTransferLength - 1U) << MDMA_CTCR_TLEN_Pos) |
                pInit->TransferTriggerMode;
  //
  // Set the CTCR register to SW Request Mode
  //
  MDMA_C0TCR |= (MDMA_CTCR_SWRM | MDMA_CTCR_BWM);
  //
  // Reset CBNDTR Register
  //
  MDMA_C0BNDTR = 0;
  //
  // Write new CBRUR Register value : source repeat block offset
  //
  MDMA_C0BRUR = (pInit->SourceBlockAddressOffset & 0x0000FFFFu);
  //
  // Write new CBRUR Register value : destination repeat block offset
  //
  MDMA_C0BRUR = ((((uint32_t)pInit->DestBlockAddressOffset) & 0x0000FFFFu) << MDMA_CBRUR_DUV_Pos);
  //
  // SW request : reset the CTBR register
  //
  MDMA_C0TBR = 0;
  //
  // Write Link Address Register
  //
  MDMA_C0LAR = 0;
}

/*********************************************************************
*
*       _MDMA_Init()
*/
static void _MDMA_Init(void) {
  MDMA_InitTypeDef Init;

  //
  // Configure MDMA parameters
  //
  Init.Request                  = MDMA_REQUEST_SW;
  Init.TransferTriggerMode      = MDMA_BLOCK_TRANSFER;
  Init.Priority                 = MDMA_PRIORITY_HIGH;
  Init.Endianness               = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  Init.SourceInc                = MDMA_SRC_INC_WORD;
  Init.DestinationInc           = MDMA_DEST_INC_WORD;
  Init.SourceDataSize           = MDMA_SRC_DATASIZE_WORD;
  Init.DestDataSize             = MDMA_DEST_DATASIZE_WORD;
  Init.DataAlignment            = MDMA_DATAALIGN_PACKENABLE;
  Init.SourceBurst              = MDMA_SOURCE_BURST_SINGLE;
  Init.DestBurst                = MDMA_DEST_BURST_SINGLE;
  Init.BufferTransferLength     = 128;
  Init.SourceBlockAddressOffset = 0;
  Init.DestBlockAddressOffset   = 0;
  //
  // Init MDMA
  //
  _MDMA_ProcessInit(&Init);
  //
  // Configure interrupt
  //
  NVIC_SetPriority(MDMA_IRQn, 15);
  NVIC_EnableIRQ(MDMA_IRQn);
}

/*********************************************************************
*
*       _GPIO_SetMODER()
*/
static void _GPIO_SetMODER(unsigned int GPIOAddr, unsigned int Mode, unsigned int Pin) {
  unsigned int v;

  v                     = GPIOx_MODER(GPIOAddr);
  v                    &= ~((0x3  << (Pin  * 2)));  // Clear bits
  v                    |=   (Mode << (Pin  * 2));   // Set mode
  GPIOx_MODER(GPIOAddr) = v;                        // Write back
}

/*********************************************************************
*
*       _PinInit()
*/
static void _PinInit(void) {
  //
  // Enable FMC clock
  //
  RCC_AHB3ENR |= (0x1 << 12);
  //
  // Enable MDMA clock
  //
  RCC_AHB3ENR |= (0x1 << 0);
  //
  // Enable GPIOs clock
  //
  RCC_AHB4ENR |= (0x1ul << GPIOD_BitPos)
              |  (0x1ul << GPIOE_BitPos)
              |  (0x1ul << GPIOF_BitPos)
              |  (0x1ul << GPIOG_BitPos)
              |  (0x1ul << GPIOH_BitPos)
              |  (0x1ul << GPIOJ_BitPos) // ST does this
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
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_0);  // PB0, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_1);  // PB1, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_8);  // PB8, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_9);  // PB9, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_10);  // PB10, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_14);  // PB14, alternate function mode
  _GPIO_SetMODER(GPIOD_BASEADDR , 0x2, GPIO_PIN_15);  // PB15, alternate function mode
  GPIOx_OSPEEDER(GPIOD_BASEADDR) |= (0x3ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x3ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x3ul << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x3ul << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x3ul << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x3ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x3ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
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
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_0);  // PB0, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_1);  // PB1, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_7);  // PB7, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_8);  // PB8, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_9);  // PB9, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_10);  // PB10, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_11);  // PB11, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_12);  // PB12, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_13);  // PB13, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_14);  // PB14, alternate function mode
  _GPIO_SetMODER(GPIOE_BASEADDR , 0x2, GPIO_PIN_15);  // PB15, alternate function mode
  GPIOx_OSPEEDER(GPIOE_BASEADDR) |= (0x3ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x3ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x3ul << (GPIO_PIN_7  * 2))  // PB7, fast speed
                                 |  (0x3ul << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x3ul << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x3ul << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x3ul << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x3ul << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x3ul << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x3ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x3ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
                                 ;
  // set Pullup with GPIOx_PUPDR
  GPIOx_PUPDR(GPIOE_BASEADDR)    |= (0x1 << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x1 << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x1 << (GPIO_PIN_7  * 2))  // PB7, fast speed
                                 |  (0x1 << (GPIO_PIN_8  * 2))  // PB8, fast speed
                                 |  (0x1 << (GPIO_PIN_9  * 2))  // PB9, fast speed
                                 |  (0x1 << (GPIO_PIN_10 * 2))  // PB10, fast speed
                                 |  (0x1 << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x1 << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x1 << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x1 << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x1 << (GPIO_PIN_15 * 2))  // PB15, fast speed
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
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_0);  // PB0, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_1);  // PB1, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_2);  // PB2, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_3);  // PB3, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_4);  // PB4, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_5);  // PB5, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_11);  // PB11, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_12);  // PB12, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_13);  // PB13, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_14);  // PB14, alternate function mode
  _GPIO_SetMODER(GPIOF_BASEADDR , 0x2, GPIO_PIN_15);  // PB15, alternate function mode
  GPIOx_OSPEEDER(GPIOF_BASEADDR) |= (0x3ul << (GPIO_PIN_0  * 2))  // PB0, fast speed
                                 |  (0x3ul << (GPIO_PIN_1  * 2))  // PB1, fast speed
                                 |  (0x3ul << (GPIO_PIN_2  * 2))  // PB2, fast speed
                                 |  (0x3ul << (GPIO_PIN_3  * 2))  // PB3, fast speed
                                 |  (0x3ul << (GPIO_PIN_4  * 2))  // PB4, fast speed
                                 |  (0x3ul << (GPIO_PIN_5  * 2))  // PB5,  fast speed
                                 |  (0x3ul << (GPIO_PIN_11 * 2))  // PB11, fast speed
                                 |  (0x3ul << (GPIO_PIN_12 * 2))  // PB12, fast speed
                                 |  (0x3ul << (GPIO_PIN_13 * 2))  // PB13, fast speed
                                 |  (0x3ul << (GPIO_PIN_14 * 2))  // PB14, fast speed
                                 |  (0x3ul << (GPIO_PIN_15 * 2))  // PB15, fast speed
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
  // Pin 0,1,4,5,8,15
  // Alternate function 12
  //
  GPIOx_AFRL(GPIOG_BASEADDR)     |= (0xCul << (GPIO_PIN_0  * 4))
                                 |  (0xCul << (GPIO_PIN_1  * 4))
                                 |  (0xCul << (GPIO_PIN_4  * 4))
                                 |  (0xCul << (GPIO_PIN_5  * 4))
                                 ;
  GPIOx_AFRH(GPIOG_BASEADDR)     |= (0xCul << ((GPIO_PIN_8  - GPIO_PIN_8) * 4))
                                 |  (0xCul << ((GPIO_PIN_15 - GPIO_PIN_8) * 4))
                                 ;
  // Set Alternate Function Push Pull Mode (0x02)
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_0);
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_1);
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_4);
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_5);
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_8);
  _GPIO_SetMODER(GPIOG_BASEADDR , 0x2, GPIO_PIN_15);
  // set GPIO_SPEED to "fast speed" = 0x3
  GPIOx_OSPEEDER(GPIOG_BASEADDR) |= (0x3ul << (GPIO_PIN_0  * 2))
                                 |  (0x3ul << (GPIO_PIN_1  * 2))
                                 |  (0x3ul << (GPIO_PIN_4  * 2))
                                 |  (0x3ul << (GPIO_PIN_5  * 2))
                                 |  (0x3ul << (GPIO_PIN_8  * 2))
                                 |  (0x3ul << (GPIO_PIN_15 * 2))
                                 ;
  // set GPIO_PULLUP("Pull-up activation") (0x01)
  GPIOx_PUPDR(GPIOG_BASEADDR)    |= (0x1ul << (GPIO_PIN_0  * 2))
                                 |  (0x1ul << (GPIO_PIN_1  * 2))
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
  GPIOx_AFRL(GPIOH_BASEADDR)     |= (0xCul << (GPIO_PIN_5  * 4))
                                 |  (0xCul << (GPIO_PIN_6  * 4))
                                 |  (0xCul << (GPIO_PIN_7  * 4))
                                 ;
  // Set Alternate Function Push Pull Mode (0x02)
  _GPIO_SetMODER(GPIOH_BASEADDR , 0x2, GPIO_PIN_5);
  _GPIO_SetMODER(GPIOH_BASEADDR , 0x2, GPIO_PIN_6);
  _GPIO_SetMODER(GPIOH_BASEADDR , 0x2, GPIO_PIN_7);
  // set GPIO_SPEED to "very high speed" = 0x03
  GPIOx_OSPEEDER(GPIOH_BASEADDR) |= (0x3ul << (GPIO_PIN_5  * 2))
                                 |  (0x3ul << (GPIO_PIN_6  * 2))
                                 |  (0x3ul << (GPIO_PIN_7  * 2))
                                 ;
  // set GPIO_PULLUP("Pull-up activation") (0x01)
  GPIOx_PUPDR(GPIOH_BASEADDR)    |= (0x1ul << (GPIO_PIN_5  * 2))
                                 |  (0x1ul << (GPIO_PIN_6  * 2))
                                 |  (0x1ul << (GPIO_PIN_7  * 2))
                                 ;
}

/*********************************************************************
*
*       _SDRAM_SendCommand()
*/
static void _SDRAM_SendCommand(FMC_SDRAM_CommandTypeDef * pCmd) {
  unsigned int ClrMask;
  unsigned int BitMask;

  ClrMask = (FMC_SDCMR_MODE_Msk |
             FMC_SDCMR_CTB1_Msk |
             FMC_SDCMR_CTB2_Msk |
             FMC_SDCMR_NRFS_Msk |
             FMC_SDCMR_MRD_Msk);
  BitMask = (pCmd->CommandMode             << FMC_SDCMR_MODE_Pos) |
            (pCmd->CommandTarget           << FMC_SDCMR_CTB2_Pos) |
           ((pCmd->AutoRefreshNumber - 1u) << FMC_SDCMR_NRFS_Pos) |
            (pCmd->ModeRegisterDefinition  << FMC_SDCMR_MRD_Pos);
  WRITE_INTO_REG(FMC_SDCMR, ClrMask, BitMask);
}

/*********************************************************************
*
*       _SDRAM_InitSequence()
*/
static void _SDRAM_InitSequence(void) {
  FMC_SDRAM_CommandTypeDef Cmd;
  unsigned int             n;

  //
  // Step 1: Configure clock configuration enable command
  //
  Cmd.CommandMode            = MT48LC4M32B2_CLK_ENABLE_CMD;
  Cmd.CommandTarget          = 1;
  Cmd.AutoRefreshNumber      = 1;
  Cmd.ModeRegisterDefinition = 0;
  _SDRAM_SendCommand(&Cmd);
  //
  // Step 2: 100 us minimum delay
  //
  n = ((480000000 / 1000));
  while (n--) {
    __NOP();
  }
  //
  // Step 3: Send PALL command
  //
  Cmd.CommandMode            = MT48LC4M32B2_PALL_CMD;
  Cmd.CommandTarget          = 1;
  Cmd.AutoRefreshNumber      = 1;
  Cmd.ModeRegisterDefinition = 0;
  _SDRAM_SendCommand(&Cmd);

  //
  // Step 4: Send refresh command
  //
  Cmd.CommandMode            = MT48LC4M32B2_AUTOREFRESH_MODE_CMD;
  Cmd.CommandTarget          = 1;
  Cmd.AutoRefreshNumber      = 8;
  Cmd.ModeRegisterDefinition = 0;
  _SDRAM_SendCommand(&Cmd);
  //
  // Step 5: Program the external memory mode register
  //
  Cmd.CommandMode            = MT48LC4M32B2_LOAD_MODE_CMD;
  Cmd.CommandTarget          = 1;
  Cmd.AutoRefreshNumber      = 1;
  Cmd.ModeRegisterDefinition = 0x230; // Mode register definition is composed of:
                                      //   BurstLength
                                      //   BurstType
                                      //   CASLatency
                                      //   OperationMode
                                      //   WriteBurstMode
  _SDRAM_SendCommand(&Cmd);
  //
  // Step 6: Set refresh rate counter
  //
  FMC_SDRTR = (REFRESH_COUNT << FMC_SDRTR_COUNT_Pos);
}

/*********************************************************************
*
*       _ConfigureFMC()
*/
static void _ConfigureFMC(void) {
  //
  // Write settings to memory bank 1
  //
  WRITE_INTO_REG(FMC_SDCR1,
                (FMC_SDCRx_RPIPE_Msk  |
                 FMC_SDCRx_RBURST_Msk |
                 FMC_SDCRx_SDCLK_Msk),
                ((0x0 << FMC_SDCRx_RPIPE_Pos)  |  // Read pipe delay: No fmc_ker_ck clock cycle delay
                 (0x1 << FMC_SDCRx_RBURST_Pos) |  // Burst read enabled
                 (0x3 << FMC_SDCRx_SDCLK_Pos)));  // Clock frequency = 3 x fmc_ker_ck periods
  //
  // Write settings to memory bank 2
  //
  WRITE_INTO_REG(FMC_SDCR2,
                (FMC_SDCRx_WP_Msk   |
                 FMC_SDCRx_CAS_Msk  |
                 FMC_SDCRx_NB_Msk   |
                 FMC_SDCRx_MWID_Msk |
                 FMC_SDCRx_NR_Msk   |
                 FMC_SDCRx_NC_Msk),
               ((0x0 << FMC_SDCRx_WP_Pos)   |  // Write accesses allowed
                (0x3 << FMC_SDCRx_CAS_Pos)  |  // CAS latency: 3 cycles
                (0x1 << FMC_SDCRx_NB_Pos)   |  // 4 internal banks
                (0x1 << FMC_SDCRx_MWID_Pos) |  // Memory data width: 16 bits
                (0x1 << FMC_SDCRx_NR_Pos)   |  // Number of row address bits: 11 bits
                (0x0 << FMC_SDCRx_NC_Pos)));   // Number of column address bits: 8 bits
  //
  // Configure timing for bank 2
  //
  WRITE_INTO_REG(FMC_SDTR1, (FMC_SDTRx_TRC_Msk |
                 FMC_SDTRx_TRP_Msk),
               (((7 - 1u) << FMC_SDTRx_TRC_Pos) |   // Row cycle delay
                ((2 - 1u) << FMC_SDTRx_TRP_Pos)));  // RP delay
  //
  WRITE_INTO_REG(FMC_SDTR2, (FMC_SDTRx_TMRD_Msk |
                 FMC_SDTRx_TXSR_Msk |
                 FMC_SDTRx_TRAS_Msk |
                 FMC_SDTRx_TRC_Msk  |
                 FMC_SDTRx_TWR_Msk  |
                 FMC_SDTRx_TRP_Msk  |
                 FMC_SDTRx_TRCD_Msk),
               (((2 - 1u) << FMC_SDTRx_TMRD_Pos) |
                ((7 - 1u) << FMC_SDTRx_TXSR_Pos) |
                ((4 - 1u) << FMC_SDTRx_TRAS_Pos) |
                ((2 - 1u) << FMC_SDTRx_TWR_Pos)  |
                ((2 - 1u) << FMC_SDTRx_TRCD_Pos)));
  //
  // Enable FMC
  //
  FMC_BCR1 |= 0x80000000;
}

/*********************************************************************
*
*       _Init_SDRAM()
*/
static void _Init_SDRAM(void) {
  //
  // Pin and MDMA initialization
  //
  _PinInit();
  _MDMA_Init();
  //
  // FMC Configuration
  // FMC SDRAM Bank configuration
  //
  _ConfigureFMC();
  //
  // SDRAM initialization sequence
  //
  _SDRAM_InitSequence();
}

/*********************************************************************
*
*       _MPU_ConfigRegion()
*/
static void _MPU_ConfigRegion(MPU_InitTypeDef * MPU_Init) {
  MPU->RBAR = MPU_Init->BaseAddress;
  MPU->RASR = ((unsigned int)MPU_Init->DisableExec             << MPU_RASR_XN_Pos)   |
              ((unsigned int)MPU_Init->AccessPermission        << MPU_RASR_AP_Pos)   |
              ((unsigned int)MPU_Init->TypeExtField            << MPU_RASR_TEX_Pos)  |
              ((unsigned int)MPU_Init->IsShareable             << MPU_RASR_S_Pos)    |
              ((unsigned int)MPU_Init->IsCacheable             << MPU_RASR_C_Pos)    |
              ((unsigned int)MPU_Init->IsBufferable            << MPU_RASR_B_Pos)    |
              ((unsigned int)MPU_Init->SubRegionDisable        << MPU_RASR_SRD_Pos)  |
              ((unsigned int)MPU_Init->Size                    << MPU_RASR_SIZE_Pos) |
              ((unsigned int)MPU_Init->Enable                  << MPU_RASR_ENABLE_Pos);
}


/*********************************************************************
*
*       _MPU_Config()
*
*  Function description:
*    Re-configure device memory type in MPU in order to avoid
*    Hardfaults caused by unaligned accesses to SDRAM.
*/
static void _MPU_Config(void) {
  MPU_InitTypeDef MPU_InitStruct;

  //
  // Disable the MPU
  //
  MPU->CTRL = 0;
  //
  // Configure the MPU attributes for SDRAM
  //
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = 0xD0000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  _MPU_ConfigRegion(&MPU_InitStruct);
  //
  // Configure the MPU attributes for SDRAM
  //
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = 0xD0000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  _MPU_ConfigRegion(&MPU_InitStruct);
  //
  // Enable MPU
  //
  MPU->CTRL = MPU_PRIVILEGED_DEFAULT | MPU_CTRL_ENABLE_Msk;
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
