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
File    : BSP.c
Purpose : BSP for SEGGER emPower Zynq
*/

#include "BSP.h"
#include "Zynq7007S.h"

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
int BSP_FPGA_Init(void);

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NUM_LEDS                    (6)

//
// LED pins physical (MIO) and virtual (EMIO) have been chosen to
// follow a contiguous logic.
//
#define  MIO_BANK0_BIT_LED_0_RED    (0)
#define EMIO_BANK2_BIT_LED_1_RED    (1)
#define EMIO_BANK2_BIT_LED_2_RED    (2)
#define EMIO_BANK2_BIT_LED_0_GREEN  (3)
#define EMIO_BANK2_BIT_LED_1_GREEN  (4)
#define EMIO_BANK2_BIT_LED_2_GREEN  (5)

//
// LED0 red is connected via MIO and the LED is connected active low.
//
#define MASK_LED_0_RED_SET          ((~(1uL <<  MIO_BANK0_BIT_LED_0_RED) << 16) | (0uL <<  MIO_BANK0_BIT_LED_0_RED))
#define MASK_LED_0_RED_CLR          ((~(1uL <<  MIO_BANK0_BIT_LED_0_RED) << 16) | (1uL <<  MIO_BANK0_BIT_LED_0_RED))

//
// LED1..2 red is connected via EMIO and the LEDs are connected active low.
// The negated logic has already been taken care of in the FPGA logic
// which means that from application point of view the LEDs operate active-high.
//
#define MASK_LED_1_RED_SET          ((~(1uL << EMIO_BANK2_BIT_LED_1_RED) << 16) | (1uL << EMIO_BANK2_BIT_LED_1_RED))
#define MASK_LED_1_RED_CLR          ((~(1uL << EMIO_BANK2_BIT_LED_1_RED) << 16) | (0uL << EMIO_BANK2_BIT_LED_1_RED))
#define MASK_LED_2_RED_SET          ((~(1uL << EMIO_BANK2_BIT_LED_2_RED) << 16) | (1uL << EMIO_BANK2_BIT_LED_2_RED))
#define MASK_LED_2_RED_CLR          ((~(1uL << EMIO_BANK2_BIT_LED_2_RED) << 16) | (0uL << EMIO_BANK2_BIT_LED_2_RED))

//
// LED1..2 green is connected via EMIO and the LEDs are connected active low.
// The negated logic has already been taken care of in the FPGA logic
// which means that from application point of view the LEDs operate active-high.
//
#define MASK_LED_0_GREEN_SET        ((~(1uL << EMIO_BANK2_BIT_LED_0_GREEN) << 16) | (1uL << EMIO_BANK2_BIT_LED_0_GREEN))
#define MASK_LED_0_GREEN_CLR        ((~(1uL << EMIO_BANK2_BIT_LED_0_GREEN) << 16) | (0uL << EMIO_BANK2_BIT_LED_0_GREEN))
#define MASK_LED_1_GREEN_SET        ((~(1uL << EMIO_BANK2_BIT_LED_1_GREEN) << 16) | (1uL << EMIO_BANK2_BIT_LED_1_GREEN))
#define MASK_LED_1_GREEN_CLR        ((~(1uL << EMIO_BANK2_BIT_LED_1_GREEN) << 16) | (0uL << EMIO_BANK2_BIT_LED_1_GREEN))
#define MASK_LED_2_GREEN_SET        ((~(1uL << EMIO_BANK2_BIT_LED_2_GREEN) << 16) | (1uL << EMIO_BANK2_BIT_LED_2_GREEN))
#define MASK_LED_2_GREEN_CLR        ((~(1uL << EMIO_BANK2_BIT_LED_2_GREEN) << 16) | (0uL << EMIO_BANK2_BIT_LED_2_GREEN))

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  unsigned long aISR[8];                   // Interrupt Table for  Execution-in-Place
  unsigned long WidthDetection;            // Width Detection
  unsigned long ImageIdentification;       // Image Identification
  unsigned long EncryptionStatus;          // Encryption Status
  unsigned long UserDefined0;              // FSBL/User Defined
  unsigned long SourceOffset;              // Source Offset
  unsigned long LengthOfImage;             // Length of Image
  unsigned long Reserved0;                 // Reserved, set to 0.
  unsigned long StartOfExecution;          // Start of Execution
  unsigned long TotalImageLength;          // Total Image Length
  unsigned long Reserved1;                 // Reserved, set to 0.
  unsigned long Checksum;                  // Header Checksum
  unsigned long aUserDefined1[21];         // FSBL/User Defined(84-Byte)(3)
  unsigned long aRegInit[512];             // Register Initialization (2048-Byte) (4)
  unsigned long aUserDefined2[8];          // FSBL/User Defined (32-Byte) (3)
} _BOOT_HEADER;

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
static unsigned long _LEDState;

#if defined(__ICCARM__)
  #pragma section=".BootHeader"
  const _BOOT_HEADER HW_BootHeader @ ".BootHeader";
#elif (defined(__SES_ARM) || defined(__GNUC__))
  const _BOOT_HEADER HW_BootHeader __attribute__((section(".BootHeader")));
#else
  #error "HW_BootHeader could not be placed in .BootHeader section"
#endif   // __ICCARM__

const _BOOT_HEADER HW_BootHeader = {   // Make sure in compiler options that this variable is kept, even if not referenced by the code
  .WidthDetection      = 0xAA995566,   // Key value needed by BootROM for QSPI boot
  .ImageIdentification = 0x584C4E58,   // "XLNX" in ASCII. Used by boot ROM to determine if a valid boot header is present
  //
  // 0xA5C3C5A3  Encrypted FSBL/User code (requires eFUSE key source).
  // 0x3A5C3C5A  Encrypted FSBL/User code (requires battery-backed RAM key source).
  // Not 0xA5C3C5A3 or 0x3A5C3C5A. Non-encrypted FSBL/User code (no key).
  //
  .EncryptionStatus    = 0xFFFFFFFF,
  .UserDefined0        = 0x01010000,   // Header Version
  .SourceOffset        = 0x00010000,   // Offset into QSPI where image to copy into iRAM on boot resides. Must be 64-byte aligned and >= 0x8C0
  .LengthOfImage       = 0x00010000,   // Number of bytes to copy into iRAM on boot. <= 192 KB. A value of 0 causes boot ROM to copy nothing but uses execute-in-place instead
  .Reserved0           = 0x00000000,   // Destination RAM address to which to copy the FSBL.
  .StartOfExecution    = 0x00000000,   // Offset from addr. 0x0 where to jump to on boot ROM execution complete (somewhere in copied data)
  .TotalImageLength    = 0x00010000,   // For non-secure boot / non-encrypted boot, same as <LengthOfImage>
  .Reserved1           = 0x00000000,   // QSPI Config Word ,set to 0x01

  .Checksum            = 0xFC165C42,   // Checksum of boot header is build over <WidthDetection> - and including <Reserved1>. Checksum is sum of words and result negated
  .aRegInit[0]         = 0xFFFFFFFF,   // Used to make boot ROM writing some SFRs. Used in pairs: aRegInit[x + 0] = SFRAddr, aRegInit[x + 1] = Data. Addr == 0xFFFFFFFF Makes boot ROM to end SFR init
  .aRegInit[1]         = 0xFFFFFFFF,
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
  unsigned long v;

  //
  //  LEDs are special as our LED pins are in a 1.8V domain.
  //  Meaning we only drive LOW (LED active) and to switch them off, we disable our output and let the 3.3V pull-up do its work
  //
  REG_SLCR_UNLOCK = SLCR_UNLOCK_KEY;
  BSP_FPGA_Init();
  v = 0
    | (1uL << MIO_BANK0_BIT_LED_0_RED)
    ;
  //
  // CAUTION: Writing the DATA registers before setting DIRM to output results in chip to ignore write data
  //          Correct sequence to configure pin for output without glitches:
  //          Disable output driver via OEN
  //          Set direction to output via DIRM
  //          Write output data via DATA register
  //          Enable output driver via OEN
  //
  REG_SLCR_MIO_PIN_00      = (3uL << 9);        // Set pin to type LVCMOS33.
  REG_MIO_OEN_0           &= ~v;                // Disable output of LED pins (LEDs switched off by default)
  REG_MIO_DIRM_0          |=  v;                // 1 == Configure pin as output
  REG_MIO_MASK_DATA_0_LSW  = ((~v) << 16) | v;  // Higher 16 bits specify a mask for which pins the write to the MD register has an effect. 0-bit means that the pin is affected
  REG_MIO_OEN_0           |=  v;                // Enable output driver
  //
  // Init LEDs controlled by FPGA logic
  //
  v = 0
    | (1uL << EMIO_BANK2_BIT_LED_1_RED)
    | (1uL << EMIO_BANK2_BIT_LED_2_RED)
    | (1uL << EMIO_BANK2_BIT_LED_0_GREEN)
    | (1uL << EMIO_BANK2_BIT_LED_1_GREEN)
    | (1uL << EMIO_BANK2_BIT_LED_2_GREEN)
    ;
  REG_MIO_OEN_2           &= ~v;                  // Disable output of LED pins (LEDs switched off by default)
  REG_MIO_DIRM_2          |=  v;                  // 1 == Configure pin as output
  REG_MIO_MASK_DATA_2_LSW  = ((~v) << 16) | 0uL;  // Higher 16 bits specify a mask for which pins the write to the MD register has an effect. 0-bit means that the pin is affected
  REG_MIO_OEN_2           |= v;                   // Enable output driver
  _LEDState = 0;
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  unsigned long Val;

  if (Index < NUM_LEDS) {
    _LEDState |= (1uL << Index);
    switch(Index) {
    case 0:
      Val = MASK_LED_0_GREEN_SET;
      break;
    case 1:
      Val = MASK_LED_1_GREEN_SET;
      break;
    case 2:
      Val = MASK_LED_2_GREEN_SET;
      break;
    case 3:
      REG_MIO_MASK_DATA_0_LSW = MASK_LED_0_RED_SET;
      return;
    case 4:
      Val = MASK_LED_1_RED_SET;
      break;
    case 5:
      Val = MASK_LED_2_RED_SET;
      break;
    default:
      Val = 0;
      break;
    }
    REG_MIO_MASK_DATA_2_LSW = Val;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  unsigned long Val;

  if (Index < NUM_LEDS) {
    _LEDState &= ~(1uL << Index);
    switch (Index) {
    case 0:
      Val = MASK_LED_0_GREEN_CLR;
      break;
    case 1:
      Val = MASK_LED_1_GREEN_CLR;
      break;
    case 2:
      Val = MASK_LED_2_GREEN_CLR;
      break;
    case 3:
      REG_MIO_MASK_DATA_0_LSW = MASK_LED_0_RED_CLR;
      return;
    case 4:
      Val = MASK_LED_1_RED_CLR;
      break;
    case 5:
      Val = MASK_LED_2_RED_CLR;
      break;
    default:
      Val = 0;
      break;
    }
    REG_MIO_MASK_DATA_2_LSW = Val;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < NUM_LEDS) {
    if (_LEDState & (1uL << Index)) {
      BSP_ClrLED(Index);
    } else {
      BSP_SetLED(Index);
    }
  }
}

/*************************** End of file ****************************/
