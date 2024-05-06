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
File    : RTOSInit_AT91SAM9263.c
Purpose : Initializes and handles the hardware for embOS
*/

#include "RTOS.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       Configuration
*/

#ifndef   SDRAM_BASE_ADDR
  #define SDRAM_BASE_ADDR   (0x20000000)
#endif

#ifndef   SDRAM_TARGET           // Target application may run in SDRAM
  #define SDRAM_TARGET      (0)  // May be overwritten by project settings
#endif

/*********************************************************************
*
*       System tick settings
*/
#define OS_TIMER_FREQ  (6251536u)
#define OS_TICK_FREQ   (1000u)
#define OS_INT_FREQ    (OS_TICK_FREQ)

/*********************************************************************
*
*       embOSView settings
*/
#ifndef   OS_VIEW_IFSELECT
  #define OS_VIEW_IFSELECT  OS_VIEW_IF_JLINK
#endif

#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  #include "JLINKDCC.h"
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  #include "BSP_UART.h"
  #define OS_UART      (0u)
  #define OS_BAUDRATE  (38400u)
#endif

/*********************************************************************
*
*       Device specific SFRs
*/
#define MUL_PLLA                 (110)                                             // Multiplier
#define OUT_PLLA                 (0x02)                                            // High speed setting
#define COUNT_PLLA               (0x1F)                                            // startup counter
#define DIV_PLLA                 (9)                                               // Divider

#define PLLAR_VALUE              ((1u           << 29)       \
                                 |((MUL_PLLA-1) << 16)       \
                                 |(OUT_PLLA     << 14)       \
                                 |(COUNT_PLLA   << 8)        \
                                 |(DIV_PLLA     << 0))

#define MUL_PLLB                 (176)                                             // Multiplier
#define OUT_PLLB                 (0x00)                                            // High speed setting
#define COUNT_PLLB               (0x1f)                                            // startup counter
#define DIV_PLLB                 (15)                                              // Divider
#define USB_DIV                  (03)                                              // USB-clock Divider

#define PLLBR_VALUE              ((USB_DIV      << 28)       \
                                 |((MUL_PLLB-1) << 16)       \
                                 |(OUT_PLLB     << 14)       \
                                 |(COUNT_PLLB   << 8)        \
                                 |(DIV_PLLB     << 0))

#define MCKR_MDIV                (0x01)                                            // Main clock is processor clock / 2
#define MCKR_PRES                (0)                                               // Processor clock is selected clock
#define MCKR_CSS                 (0x02)                                            // PLLA is selected clock

#define MCKR_VALUE               ((MCKR_MDIV << 8)   \
                                 |(MCKR_PRES << 2)   \
                                 |(MCKR_CSS  << 0))
//
// Debug unit
//
#define DBGU_BASE_ADDR           (0xFFFFEE00u)
#define DBGU_IMR                 (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x10))      // Interrupt Mask Register
#define DBGU_SR                  (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x14))      // Channel Status Register
#define DBGU_COMMRX              (1u << 31)
#define DBGU_COMMTX              (1u << 30)
#define DBGU_RXBUFF              (1u << 12)
#define DBGU_TXBUFE              (1u << 11)
#define DBGU_TXEMPTY             (1u <<  9)
#define DBGU_PARE                (1u <<  7)
#define DBGU_FRAME               (1u <<  6)
#define DBGU_OVRE                (1u <<  5)
#define DBGU_ENDTX               (1u <<  4)
#define DBGU_ENDRX               (1u <<  3)
#define DBGU_TXRDY               (1u <<  1)
#define DBGU_RXRDY               (1u <<  0)
#define DBGU_MASK_ALL            (DBGU_COMMRX | DBGU_COMMTX  | DBGU_RXBUFF |  \
                                  DBGU_TXBUFE | DBGU_TXEMPTY | DBGU_PARE   |  \
                                  DBGU_FRAME  | DBGU_OVRE    | DBGU_ENDTX  |  \
                                  DBGU_ENDRX  | DBGU_TXRDY   | DBGU_RXRDY)
//
// Reset controller
//
#define RSTC_BASE_ADDR           (0xFFFFFD00u)
#define RSTC_CR                  (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x00u))
#define RSTC_SR                  (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x04u))
#define RSTC_MR                  (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x08u))
#define RSTC_BODIEN              (1u << 16)                                        // Brownout interrupt enable
#define RSTC_URSTIEN             (1u <<  4)                                        // User reset interrupt enable
#define RSTC_URSTEN              (1u <<  0)                                        // User reset enable
#define RSTC_BODSTS              (1u <<  1)                                        // Brownout status
#define RSTC_URSTS               (1u <<  0)                                        // User reset status
//
// Real time timer
//
#define RTT_BASE_ADDR            (0xFFFFFD20u)
#define RTT_MR                   (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x00u))
#define RTT_SR                   (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x0Cu))
#define RTT_RTTINCIEN            (1uL << 17)
#define RTT_ALMIEN               (1uL << 16)
#define RTT_RTTINC               (1uL << 1)
#define RTT_ALMS                 (1uL << 0)
//
// Periodic interval timer
//
#define PIT_BASE_ADDR            (0xFFFFFD30u)
#define PIT_MR                   (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x00u))
#define PIT_SR                   (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x04u))
#define PIT_PIVR                 (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x08u))
#define PIT_PIIR                 (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x0Cu))
//
// Watchdog
//
#define WDT_BASE_ADDR            (0xFFFFFD40u)
#define WDT_CR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x00u))
#define WDT_MR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x04u))
#define WDT_SR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x08u))
#define WDT_WDFIEN               (1u << 12)                                        // Watchdog interrupt enable flag in mode register
#define WDT_WDERR                (1u <<  1)                                        // Watchdog error status flag
#define WDT_WDUNF                (1u <<  0)                                        // Watchdog underflow status flag
//
// PIO control register
//
#define PIOA_BASE_ADDR           (0xfffff200u)
#define PIOB_BASE_ADDR           (0xfffff400u)
#define PIOC_BASE_ADDR           (0xfffff600u)
#define PIOD_BASE_ADDR           (0xfffff800u)
#define PIOE_BASE_ADDR           (0xfffffA00u)
//
// PIOD, used as data BUS D16..D31 for SDRAM (EBI0)
//
#define PIOD_PDR                 (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x04u))     // PIOD disable register
#define PIOD_MDDR                (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x54u))     // PIOD multi driver disable register
#define PIOD_ASR                 (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x70u))     // PIOD peripheral A select register
//
//  Power management controller
//
#define PMC_BASE_ADDR            (0xFFFFFC00u)
#define PMC_CKGR_PLLAR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x28u))      // PLLA register
#define PMC_CKGR_PLLBR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x2cu))      // PLLB register
#define PMC_SCDR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x04u))      // System Clock disable register
#define PMC_SCER                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x10u))      // System clock enable register
#define PMC_PCER                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x10u))      // Peripheral clock enable register
#define PMC_MOR                  (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x20u))      // main oscillator register
#define PMC_PLLR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x2cu))      // PLL register
#define PMC_MCKR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x30u))      // Master clock register
#define PMC_SR                   (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x68u))      // status register
#define PMC_IMR                  (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x6Cu))      // interrupt mask register
#define PMC_MOSCS                (1u <<  0)
#define CKGR_MAINRDY             (1u << 16)
#define PMC_LOCKA                (1u <<  1)
#define PMC_LOCKB                (1u <<  2)
#define PMC_MCKRDY               (1u <<  3)
#define PMC_PCKRDY2              (1u << 10)
#define PMC_PCKRDY1              (1u <<  9)
#define PMC_PCKRDY0              (1u <<  8)
#define PMC_MASK_ALL             (PMC_PCKRDY2 | PMC_PCKRDY1 | PMC_PCKRDY0 | \
                                  PMC_MCKRDY  | PMC_LOCKB   | PMC_MOSCS)
//
// Advanced interrupt controller (AIC)
//
#define AIC_BASE_ADDR            (0xFFFFF000u)
#define AIC_SMR_BASE_ADDR        (AIC_BASE_ADDR + 0x00u)
#define AIC_SVR_BASE_ADDR        (AIC_BASE_ADDR + 0x80u)
#define AIC_SVR0                 (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x00u))
#define AIC_SVR1                 (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x04u))
#define AIC_IVR                  (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x100u))
#define AIC_ISR                  (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x108u))
#define AIC_IPR                  (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x10cu))
#define AIC_IDCR                 (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x124u))
#define AIC_ICCR                 (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x128u))
#define AIC_IECR                 (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x120u))
#define AIC_EOICR                (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x130u))
#define AIC_SPU                  (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x134u))
#define AIC_DCR                  (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x138u))
#define AIC_FFDR                 (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x144u))
//
//  AIC interrupt sources and peripheral IDs
//
#define SYSTEM_IRQ_ID            (1)                                               // System IRQ ID
#define PIOA_ID                  (2)
#define PIOB_ID                  (3)
#define PIOC_ID                  (4)
#define PIOD_ID                  (4)
#define PIOE_ID                  (4)
#define US0IRQ_ID                (7)                                               // USART Channel 0 interrupt
#define US1IRQ_ID                (8)                                               // USART Channel 1 interrupt
#define US2IRQ_ID                (9)                                               // USART Channel 2 interrupt

#ifndef   NUM_INT_SOURCES
  #define NUM_INT_SOURCES        (32)
#endif

#define INT_PRIORITY_MASK        (0x07)
#define NUM_INT_PRIORITIES       (8)
//
// MATRIX + EBI interface
//
#define MATRIX_BASE_ADDR         (0xFFFFEC00u)                                     // MATRIX Base Address
#define MATRIX_MCFG              (*(volatile OS_U32*)(MATRIX_BASE_ADDR + 0x000u))  // MATRIX Master configuration register
#define MATRIX_EBICSA            (*(volatile OS_U32*)(MATRIX_BASE_ADDR + 0x120u))  // MATRIX EBI Chip Select Assignment register
//
// SDRAM controller
//
#define SDRAMC_BASE_ADDR         (0xFFFFE200u)                                     // SDRAMC Base Address
#define SDRAMC_MR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x00u))   // (SDRAMC) SDRAM Controller Mode Register
#define SDRAMC_TR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x04u))   // (SDRAMC) SDRAM Controller Refresh timer Register
#define SDRAMC_CR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x08u))   // (SDRAMC) SDRAM Controller Configuration Register
#define SDRAMC_LPR               (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x10u))   // (SDRAMC) SDRAM Controller Low Power Register
#define SDRAMC_MDR               (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x24u))   // (SDRAMC) SDRAM Controller Memory Device Register

#define SDRAMC_MODE_NORMAL_CMD   (0x0)                                             // (SDRAMC) Normal Mode
#define SDRAMC_MODE_NOP_CMD      (0x1)                                             // (SDRAMC) Issue a All Banks Precharge Command at every access
#define SDRAMC_MODE_PRCGALL_CMD  (0x2)                                             // (SDRAMC) Issue a All Banks Precharge Command at every access
#define SDRAMC_MODE_LMR_CMD      (0x3)                                             // (SDRAMC) Issue a Load Mode Register at every access
#define SDRAMC_MODE_RFSH_CMD     (0x4)                                             // (SDRAMC) Issue a Refresh
//
// SMC, static memory controller
//
#define SMC_BASE_ADDR            (0xFFFFE400u)                                     // SMC  Base Address
#define SMC2_BASE_ADDR           (SMC_BASE_ADDR + (0x10 * 2))                      // SMC2 Base Address
#define SMC_SETUP2               (*(volatile OS_U32*)(SMC2_BASE_ADDR + 0x00u))
#define SMC_PULSE2               (*(volatile OS_U32*)(SMC2_BASE_ADDR + 0x04u))
#define SMC_CYCLE2               (*(volatile OS_U32*)(SMC2_BASE_ADDR + 0x08u))
#define SMC_MODE2                (*(volatile OS_U32*)(SMC2_BASE_ADDR + 0x0Cu))

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       MMU and cache configuration
*
*  Notes
*    The translation table has to be aligned on 16384 bytes boundary
*    The translation table has to be located in un-initialized memory,
*    because it may be filled from startup, before segments and variables
*    are initialized.
*/
#if   (defined(__ICCARM__))  // IAR
  #pragma data_alignment=16384
  static __no_init OS_U32 _TranslationTable[0x1000];
#elif (defined(__CC_ARM))    // Keil
  #pragma arm section zidata = "NO_INIT"  // Has to be declared as separate section UNINIT in the scatter file
  __align(16384) static OS_U32 _TranslationTable[0x1000];
  #pragma arm section zidata              // back to default (.bss section)
#elif (defined(__GNUC__))    // GCC
  static OS_U32 _TranslationTable[0x1000] __attribute__ ((aligned (16384), section (".non_init._TranslationTable")));
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
/*********************************************************************
*
*       _DCC_OnRx()
*/
static void _DCC_OnRx(unsigned Channel, OS_U8 Data) {
  OS_USE_PARA(Channel);  // Not supported by this version, avoid compiler warning
  OS_COM_OnRx(Data);
}

/*********************************************************************
*
*       _DCC_OnTx()
*/
static void _DCC_OnTx(unsigned Channel) {
  OS_USE_PARA(Channel);  // Not supported by this version, suppress warning
  OS_COM_OnTx();
}

#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
/*********************************************************************
*
*       _OS_OnRX()
*
*  Function description
*    Callback wrapper function for BSP UART module.
*/
static void _OS_OnRX(unsigned int Unit, unsigned char c) {
  OS_USE_PARA(Unit);
  OS_COM_OnRx(c);
}

/*********************************************************************
*
*       _OS_OnTX()
*
*  Function description
*    Callback wrapper function for BSP UART module.
*/
static int _OS_OnTX(unsigned int Unit) {
  OS_USE_PARA(Unit);
  return (int)OS_COM_OnTx();
}
#endif

/*********************************************************************
*
*       _DefaultFiqHandler()
*
*  Function description
*    A dummy FIQ handler
*/
static void _DefaultFiqHandler(void) {
  while (1) {
    ;  // Not implemented
  }
}

/*********************************************************************
*
*       _DefaultIrqHandler()
*
*  Function description
*    A dummy IRQ handler
*
*  Notes
*    (1) This handler is initially written into all AIC interrupt vectors
*        It is called if no interrupt vector was installed for specific
*        interrupt source.
*        May be used during debugging to detect uninstalled interrupts.
*/
static void _DefaultIrqHandler(void) {
  volatile OS_U32 IrqSource;

  IrqSource = AIC_ISR;     // detect source of uninstalled interrupt
  while (IrqSource > 0u) {
    ;
  }
}

/*********************************************************************
*
*       _SpuriousIrqHandler()
*
*  Function description
*    default spurious IRQ handler
*
*  Notes
*    A spurious interrupt is called under one of the following
*    circumstances:
*    1. An external interrupt source is programmed in level-sensitive mode
*       and an active level occurs for only a short time.
*    2. An internal interrupt source is programmed in level sensitive
*       and the output signal of the corresponding embedded peripheral
*       is activated for a short time. (As in the case for the Watchdog.)
*    3. An interrupt occurs just a few cycles before the software begins
*       to mask it, thus resulting in a pulse on the interrupt source.
*
*    The spurious handler is called to detect these errors.
*/
static void _SpuriousIrqHandler(void) {
#if (OS_DEBUG != 0)
  static OS_U32 _SpuriousIrqCnt;

  _SpuriousIrqCnt++;
#endif
}

/*********************************************************************
*
*       _InitAIC()
*
*  Function description
*    Initialize interrupt controller by setting default vectors
*    and clearing all interrupts
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*/
static void _InitAIC(void) {
  unsigned int     i;
  OS_ISR_HANDLER** papISR;

  AIC_IDCR = 0xFFFFFFFFu;                    // Disable all interrupts
  AIC_ICCR = 0xFFFFFFFFu;                    // Clear all interrupts
  AIC_FFDR = 0xFFFFFFFFu;                    // Reset fast forcingts
  AIC_SVR0 = (int)_DefaultFiqHandler;        // Dummy FIQ handler
  AIC_SPU  = (int) _SpuriousIrqHandler;      // Dummy spurious handler
  papISR   = (OS_ISR_HANDLER**)AIC_SVR_BASE_ADDR;
  for (i = 1u; i < NUM_INT_SOURCES; i++)  {  // Initially set all sources
    *(papISR + i) = &_DefaultIrqHandler;     // to dummy irq handler
  }
  for (i = 0; i < NUM_INT_PRIORITIES; i++) {
    AIC_EOICR = 0;                           // Reset interrupt controller
  }
#if (OS_DEBUG != 0)                          // For debugging activate AIC protected mode
  AIC_DCR |= 0x01u;                          // Enable AIC protected mode
#endif
}

/*********************************************************************
*
*       __writeMemory32()
*/
#define __writeMemory32(Data, Addr, Zone)  ((*(volatile OS_U32*)(Addr)) = Data)

/*********************************************************************
*
*       __delay()
*/
static void __delay(OS_U32 Loops) {
  volatile unsigned int i;
  #define LOOP_CNT  (20004)
  while (Loops) {  // One loop should last 1ms
    for (i = 0; i < LOOP_CNT; i++) {
    }
    Loops--;
  }
}
/*********************************************************************
*
*       _CopyToSDRAM()
*
*  Function description
*    Copy whole application from Flash to SDRAM, which has to be initialized before
*
*  NOTES:
*    (1) Settings used here are for AT91SAM9263 eval board
*        and may have to be modified for other target boards
*    (2) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*    (3) The function has to be called, before MMU is initialized and
*        SDRAM is remapped to 0x0.
*/
#if (SDRAM_TARGET == 1)
#if (defined(__ICCARM__))
#if (__VER__ >= 500)
static void _CopyToSDRAM(void) {
  unsigned long Size;
  #pragma segment="PROGRAM_DATA"
  #pragma section=".intvec"
  Size = (unsigned long) __segment_end("PROGRAM_DATA") - (unsigned long) __segment_begin(".intvec");
  memcpy((void*)SDRAM_BASE_ADDR, (void*) __segment_begin(".intvec"), Size);
}
#else
static void _CopyToSDRAM(void) {
  unsigned long Size;
  #pragma segment="PROGRAM_END"
  #pragma segment="INTVEC"
  Size = (unsigned long) __segment_begin("PROGRAM_END") - (unsigned long) __segment_begin("INTVEC");
  memcpy((void*)_SDRAM_BASE_ADDR, (void*) __segment_begin("INTVEC"), Size);
}
#endif
#endif  // __ICCARM__
#if (defined(__GNUC__))
static void _CopyToSDRAM(void) {
}
#endif
#endif  // (SDRAM_TARGET == 1)

/*********************************************************************
*
*       _InitRTT()
*
*  Function description
*    Initialize Real time timer.
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*    (2) Not used by embOS, we disable interrupts here to avoid unhandled interrupts
*        May be modified by user if RTT is required for application.
*/
static void _InitRTT(void) {
  //
  // Disable Real-Time Timer interrupts
  //
  RTT_MR &= ~(RTT_RTTINCIEN | RTT_ALMIEN);
}

/*********************************************************************
*
*       _InitPLL()
*
*  Function description
*    Initialize main clock and PLLs
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called!
*/
static void _InitPLL(OS_U32 PLLA_Val, OS_U32 PLLB_Val, OS_U32 MCKR_Val) {
  //
  // Enable main clock OSC and wait until clock is stable
  //
  PMC_MOR = ((64/8) << 8) + 1u;  // Startup time = 64 slow clocks, Main oscillator enable
  while ((PMC_SR & PMC_MOSCS) == 0) {
  }
  //
  // Initialize PLL A for Main clock
  //
  PMC_CKGR_PLLAR = PLLA_Val;
  //
  // Wait until PLLA locks
  //
  while ((PMC_SR & PMC_LOCKA) == 0) {
  }
  //
  // Wait until the master clock is stable
  //
  while ((PMC_SR & PMC_MCKRDY) == 0) {
  }
  //
  // Initialize PLL B for USB clock
  //
  PMC_CKGR_PLLBR = PLLB_Val;
  //
  // Wait until PLLB locks
  //
  while ((PMC_SR & PMC_LOCKB) == 0)  {
  }
  //
  // Wait until the master clock is stable
  //
  while ((PMC_SR & PMC_MCKRDY) == 0) {
  }
  //
  // Select PLL A clock as clock source
  //
  PMC_MCKR = MCKR_Val;
  //
  // Wait until the master clock is stable
  //
  while ((PMC_SR & PMC_MCKRDY) == 0) {
  }
}

/*********************************************************************
*
*       _InitSDRAM()
*
*  Function description
*    Initialize SDRAM controller
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*    (2) Settings used here are for AT91SAM9263-EK eval board
*        and may have to be modified for other target board
*/
static void _InitSDRAM(void) {
  //
  // Setup bus interface for SDRAM if not already initialized
  //
  if ((MATRIX_EBICSA & (1uL << 1)) == 0) {
    // Configure PIOs
    // AT91F_PIO_CfgPeriph( AT91C_BASE_PIOD, AT91C_PC16_D16 to AT91C_PC16_D31
    // pPio->PIO_ASR = periphAEnable; AT91C_PIOC_ASR ((AT91_REG *) 0xFFFFF870)  // (PIOC) Select A Register
    // pPio->PIO_BSR = periphBEnable;AT91C_PIOC_BSR  ((AT91_REG *) 0xFFFFF874)  // (PIOC) Select B Register
    // pPio->PIO_PDR = (periphAEnable | periphBEnable);                         // Set in Periph mode
    //-I- Configure PIOD as peripheral (D16/D31)
    __writeMemory32(0xFFFF0000,0xFFFFF870,"Memory");
    __writeMemory32(0x00000000,0xFFFFF874,"Memory");
    __writeMemory32(0xFFFF0000,0xFFFFF804,"Memory");
    // Init MATRIX to support EBI0 CS1 for SDRAM"
    // AT91PS_CCFG ((AT91_REG *)     0xFFFFED20)
    // (CCFG)  EBI0 Chip Select Assignement Register
    __writeMemory32(0x10002,0xFFFFED20,"Memory");
    // psdrc->SDRAMC_CR =  AT91C_SDRAMC_NC_9  | AT91C_SDRAMC_NR_13 | AT91C_SDRAMC_CAS_2 |
    // AT91C_SDRAMC_NB_4_BANKS | AT91C_SDRAMC_DBW_32_BITS | AT91C_SDRAMC_TWR_2 | AT91C_SDRAMC_TRC_7 |
    // AT91C_SDRAMC_TRP_2 | AT91C_SDRAMC_TRCD_2 | AT91C_SDRAMC_TRAS_5 | AT91C_SDRAMC_TXSR_8 ;
    __writeMemory32(0x85227259,0xFFFFE208,"Memory");
    __delay(1);
    //    psdrc->SDRAMC_MR    = 0x00000002;                               // Set PRCHG AL
    __writeMemory32(0x00000002,0xFFFFE200,"Memory");
    //    *AT91C_SDRAM    = 0x00000000;                                  // Perform PRCHG
    __writeMemory32(0x00000000,0x20000000,"Memory");
    __delay(1);
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 1st CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+4)    = 0x00000001;                              // Perform CBR
    __writeMemory32(0x00000001,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = 0x00000004;                       // Set 2 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+8)    = 0x00000002;                              // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 3 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0xc)    = 0x00000003;                            // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 4 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x10)    = 0x00000004;                           // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 5 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x14)    = 0x00000005;                           // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 6 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x18)    = 0x00000006;                           // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 7 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x1c)    = 0x00000007;                           // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_RFSH_CMD;       // Set 8 CBR
    __writeMemory32(0x00000004,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x20)    = 0x00000008;                           // Perform CBR
    __writeMemory32(0x00000000,0x20000000,"Memory");
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_LMR_CMD;        // Set LMR operation
    __writeMemory32(0x00000003,0xFFFFE200,"Memory");
    //    *(AT91C_SDRAM+0x20)    = 0xcafedede;                           // Perform LMR burst=1, lat=2
    __writeMemory32(0xcafedede,0x20000020,"Memory");
    //
    // Setup refresh timer. We assume one refresh cycle every 7.81 micro seconds.
    // If SDRAM runs with other refresh rates (for example 15.61 us), this may be changed
    //
    SDRAMC_TR     = ((781 * ((200049178uL) / 1000000)) / 100);
    //    psdrc->AT91C_SDRAMC0_MR    = AT91C_SDRAMC_MODE_NORMAL_CMD;     // Set Normal mode
    __writeMemory32(0x00000000,0xFFFFE200,"Memory");
    //    *AT91C_SDRAM    = 0x00000000;    // Perform Normal mode
    __writeMemory32(0x00000000,0x20000000,"Memory");
  }
}

/*********************************************************************
*
*       _InitSMC()
*
*  Function description
*    Initialize static memory controller
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*    (2) Settings used here are for AT91SAM9263-EK eval board
*        and may have to be modified for other target boards
*/
static void _InitSMC(void) {
  #define   NRD_PULSE_2     (3)
  #define   NWR_PULSE_2     (3)
  #define   NRD_SETUP_2     (1)
  #define   NWR_SETUP_2     (1)
  #define   NCS_RD_SETUP_2  (0)
  #define   NCS_WR_SETUP_2  (0)
  #define   NCS_RD_PULSE_2  (NRD_SETUP_2 + NRD_PULSE_2 + 1 - NCS_RD_SETUP_2)
  #define   NCS_WR_PULSE_2  (NWR_SETUP_2 + NWR_PULSE_2 + 1 - NCS_WR_SETUP_2)
  #define   NRD_HOLD_2      (8)
  #define   NWR_HOLD_2      (9)
  SMC_SETUP2 = (NCS_RD_SETUP_2 << 24)
             | (NRD_SETUP_2    << 16)
             | (NCS_WR_SETUP_2 <<  8)
             | (NWR_SETUP_2    <<  0)
             ;
  SMC_PULSE2 = (NCS_RD_PULSE_2 << 24)
             | (NRD_PULSE_2    << 16)
             | (NCS_WR_PULSE_2 <<  8)
             | (NWR_PULSE_2    <<  0)
             ;
  SMC_CYCLE2 = ((NRD_SETUP_2 + NRD_PULSE_2 + NRD_HOLD_2) << 16)  // NRD_SETUP + NRD_PULSE + NRD_HOLD
             | ((NWR_SETUP_2 + NWR_PULSE_2 + NWR_HOLD_2) <<  0)  // NWR_SETUP + WWR_PULSE + NWR_HOLD
             ;
  SMC_MODE2  = 0
             | (1u << 12)      // 16bit bus width
             | (1u <<  8)      // Enable BAT mode for write access
             | (0u <<  4)      // Disable NWAIT mode (not connected)
             | (1u <<  1)      // Set WRITE_MODE for write cycles (write controlled)
             | (1u <<  0)      // Set READ_MODE for read cycles (read controlled)
             ;
}

/*********************************************************************
*
*       _OS_GetHWTimerCycles()
*
*  Function description
*    Returns the current hardware timer count value.
*
*  Return value
*    Current timer count value.
*/
static unsigned int _OS_GetHWTimerCycles(void) {
  return (PIT_PIIR & 0x000FFFFFu);
}

/*********************************************************************
*
*       _OS_GetHWTimer_IntPending()
*
*  Function description
*    Returns if the hardware timer interrupt pending flag is set.
*
*  Return value
*    == 0: Interrupt pending flag not set.
*    != 0: Interrupt pending flag set.
*/
static unsigned int _OS_GetHWTimer_IntPending(void) {
  return (PIT_PIIR & 0xFFF00000u);
}

/*********************************************************************
*
*       _OS_ISR_Tick()
*
*  Function description
*    This is the hardware timer exception handler.
*/
static void _OS_ISR_Tick(void) {
  volatile OS_U32 Dummy;

  if (PIT_SR & (1u << 0)) {  // Timer interrupt pending?
    Dummy = PIT_PIVR;        // Reset interrupt pending condition
    OS_USE_PARA(Dummy);
    OS_TICK_Handle();
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
    JLINKDCC_Process();
#endif
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_InitHW()
*
*  Function description
*    Initialize the hardware required for embOS to run.
*/
void OS_InitHW(void) {
  OS_INT_IncDI();
  //
  // Ensure caches are synchronized
  //
  OS_ARM_CACHE_Sync();
  //
  // Initialize interrupt controller
  //
  _InitAIC();
  //
  // Initialize real time timer
  //
  _InitRTT();
  //
  // Initialize timer for embOS
  //
  PIT_MR = ((((OS_TIMER_FREQ / OS_INT_FREQ) - 1) & 0x000FFFFFu) | (1u << 25) | (1u << 24));
  OS_ARM_InstallISRHandler(SYSTEM_IRQ_ID, _OS_ISR_Tick);
  OS_ARM_EnableISR(SYSTEM_IRQ_ID);
  //
  // Inform embOS about the timer settings
  //
  {
    OS_SYSTIMER_CONFIG SysTimerConfig = {OS_TIMER_FREQ, OS_INT_FREQ, OS_TIMER_UPCOUNTING, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending};
    OS_TIME_ConfigSysTimer(&SysTimerConfig);
  }
  //
  // Initialize communication for embOSView
  //
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  JLINKDCC_SetpfOnRx(1, _DCC_OnRx);
  JLINKDCC_SetpfOnTx(1, _DCC_OnTx);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  BSP_UART_Init(OS_UART, OS_BAUDRATE, BSP_UART_DATA_BITS_8, BSP_UART_PARITY_NONE, BSP_UART_STOP_BITS_1);
  BSP_UART_SetReadCallback(OS_UART, _OS_OnRX);
  BSP_UART_SetWriteCallback(OS_UART, _OS_OnTX);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_ETHERNET)
  UDP_Process_Init();
#endif
  OS_INT_DecRI();
}

/*********************************************************************
*
*       OS_Idle()
*
*  Function description
*    This code is executed whenever no task, software timer, or
*    interrupt is ready for execution.
*
*  Additional information
*    The idle loop does not have a stack of its own, therefore no
*    functionality should be implemented that relies on the stack
*    to be preserved.
*/
void OS_Idle(void) {  // Idle loop: No task is ready to execute
  while (1) {         // Nothing to do ... wait for interrupt
    #if ((OS_VIEW_IFSELECT != OS_VIEW_IF_JLINK) && (OS_DEBUG == 0))
                      // Switch CPU into sleep mode
    #endif
  }
}

/*********************************************************************
*
*       Optional communication with embOSView
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_COM_Send1()
*
*  Function description
*    Sends one character.
*/
void OS_COM_Send1(OS_U8 c) {
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  JLINKDCC_SendCharOnChannelNB(1, c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_UART)
  BSP_UART_Write1(OS_UART, c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_IF_ETHERNET)
  UDP_Process_Send1(c);
#elif (OS_VIEW_IFSELECT == OS_VIEW_DISABLED)
  OS_USE_PARA(c);          // Avoid compiler warning
  OS_COM_ClearTxActive();  // Let embOS know that Tx is not busy
#endif
}

/*********************************************************************
*
*       embOS interrupt handler and ISR specific functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_irq_handler()
*
*  Function description
*    Detect reason for IRQ and call corresponding service routine.
*    OS_irq_handler is called from the IRQ_handler function
*    found in RTOS.s.
*    IRQ_Handler has to be inserted in the vector table in the
*    startup code.
*/
OS_INTERWORK void OS_irq_handler(void) {
  OS_ISR_HANDLER* pISR;

  pISR = (OS_ISR_HANDLER*)AIC_IVR;  // Read interrupt vector to release NIRQ to CPU core
#if (OS_DEBUG != 0)
  AIC_IVR = (OS_U32)pISR;           // Write back any value to IVR register to allow interrupt stacking in protected mode
#endif
  OS_INT_EnterNestable();           // Inform OS that interrupt handler is running
  pISR();                           // Call interrupt service routine
  OS_INT_Disable();                 // Disable interrupts and unlock
  AIC_EOICR = 0;                    // interrupt controller =>  Restore previous priority
  OS_INT_LeaveNestable();           // Leave interrupt, perform task switch if required
}

/*********************************************************************
*
*       OS_ARM_InstallISRHandler()
*
*  Function description
*    Installs an interrupt handler
*
*  Parameters
*    ISRIndex:    Index of the interrupt source, usually the interrupt vector number
*    pISRHandler: Address of the interrupt handler function
*
*  Return value
*    The address of the interrupt handler that was previously installed with the addressed interrupt source
*/
OS_ISR_HANDLER* OS_ARM_InstallISRHandler(int ISRIndex, OS_ISR_HANDLER* pISRHandler) {
  OS_ISR_HANDLER*  pOldHandler;
  OS_ISR_HANDLER** papISR;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  papISR = (OS_ISR_HANDLER**)AIC_SVR_BASE_ADDR;
  pOldHandler          = *(papISR + ISRIndex);
  *(papISR + ISRIndex) = pISRHandler;
  OS_INT_EnableConditional();
  return pOldHandler;
}

/*********************************************************************
*
*       OS_ARM_EnableISR()
*
*  Function description
*    Enables an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source which should be enabled
*/
void OS_ARM_EnableISR(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  AIC_IECR = (1u << ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_DisableISR()
*
*  Function description
*    Disables an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source which should be disabled
*/
void OS_ARM_DisableISR(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  AIC_IDCR = (1u << ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_ISRSetPrio()
*
*  Function description
*    Sets the priority of an interrupt
*
*  Parameters
*    ISRIndex: Index of the interrupt source for which to configure the priority
*    Prio    : The priority which should be set for the specific interrupt
*/
int OS_ARM_ISRSetPrio(int ISRIndex, int Prio) {
  OS_U32* pPrio;
  int     OldPrio;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  pPrio           = (OS_U32*)AIC_SMR_BASE_ADDR;
  OldPrio         = pPrio[ISRIndex];
  pPrio[ISRIndex] = (OldPrio & ~INT_PRIORITY_MASK) | (Prio & INT_PRIORITY_MASK);
  OS_INT_EnableConditional();
  return OldPrio & INT_PRIORITY_MASK;
}

/*********************************************************************
*
*       __low_level_init()
*
*  Function description
*    Initialize memory controller, clock generation and PLL.
*
*    Has to be modified if another CPU clock frequency should be
*    used. This function is called during startup and has to
*    return 1 to perform segment initialization.
*
*    Because variables are not initialized before __low_level_init() is called,
*    no access or call which relies on any variable can be used here.
*/
OS_INTERWORK int __low_level_init(void);
OS_INTERWORK int __low_level_init(void) {
  WDT_MR = (1u << 15);  // Initially disable watchdog
  //
  // Initialize main clock oscillator and PLLs
  //
  _InitPLL(PLLAR_VALUE, PLLBR_VALUE, MCKR_VALUE);
  //
  // Initialize SDRAM
  //
  _InitSDRAM();
  //
  //  Initialize static memory controller
  //
  _InitSMC();
  //
  // Copy program to SDRAM
  //
#if (SDRAM_TARGET != 0)
  _CopyToSDRAM();
#endif
  //
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                         Mode                    VAddr  PAddr  Size[MB]
#if (SDRAM_TARGET != 0)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,   0x000, 0x200, 0x001);  // SDRAM, remapped
#else
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,   0x000, 0x003, 0x001);  // Internal SRAM2, remapped
#endif
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x001, 0x000, 0x002);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x003, 0x003, 0x001);  // Internal SRAM2, original address
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x004, 0x000, 0x001);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x005, 0x005, 0x001);  // Internal SRAM0, original address
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x006, 0x000, 0x001);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x007, 0x007, 0x001);  // LCD controller
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x008, 0x000, 0x002);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x00A, 0x00A, 0x001);  // USB host controller OHCI
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x00B, 0x000, 0x1F5);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,   0x200, 0x200, 0x020);  // SDRAM, first 32MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x220, 0x220, 0x020);  // SDRAM, second 32MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x240, 0x200, 0x020);  // SDRAM, first 32MB, uncached
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x260, 0x000, 0x0A0);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x300, 0x300, 0x001);  // External SRAM area CS2, used for Ethernet controller
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x301, 0x000, 0x0FF);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x400, 0x400, 0x020);  // NAND controller
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x420, 0x000, 0xBDF);  // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0xfff, 0xfff, 0x001);  // SFR area
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  //
  // Initialize reset control
  //
  RSTC_MR = ((0xA5u << 24) | RSTC_URSTEN);  // write KEY and URSTEN to allow USER RESET
  return 1;
}

/*************************** End of file ****************************/
