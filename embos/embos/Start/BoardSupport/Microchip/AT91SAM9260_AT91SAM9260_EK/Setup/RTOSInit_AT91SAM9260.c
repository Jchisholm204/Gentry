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
File    : RTOSInit_AT91SAM9260.c
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

#ifndef   SDRAM_TARGET
  #define SDRAM_TARGET  (1)
#endif

/*********************************************************************
*
*       Clock settings
*/
#define DIV_PLLA        (  10u)  // Divider
#define MUL_PLLA        (  86u)  // Multiplier
#define OUT_PLLA        (0x02u)  // High speed setting
#define COUNT_PLLA      (0x20u)  // startup counter

#define PLLAR_VALUE     ((1u << 29)           \
                        |((MUL_PLLA-1) << 16) \
                        |(OUT_PLLA     << 14) \
                        |(COUNT_PLLA   << 8)  \
                        |(DIV_PLLA     << 0))

#define MUL_PLLB        (26*2)   // Multiplier
#define OUT_PLLB        (0x00)   // High speed setting
#define COUNT_PLLB      (0x20)   // startup counter
#define DIV_PLLB        (10)     // Divider
#define USB_DIV         (01)     // USB-clock Divider

#define PLLBR_VALUE     ((USB_DIV      << 28) \
                        |((MUL_PLLB-1) << 16) \
                        |(OUT_PLLB     << 14) \
                        |(COUNT_PLLB   << 8)  \
                        |(DIV_PLLB     << 0))

#define MCKR_MDIV       (1u)     // Main clock is processor clock / 2
#define MCKR_PRES       (0u)     // Processor clock is selected clock
#define MCKR_CSS        (2u)     // PLLA is selected clock

#define MCKR_VALUE      ((MCKR_MDIV << 8)  \
                        |(MCKR_PRES << 2)  \
                        |(MCKR_CSS  << 0))

/*********************************************************************
*
*       System tick settings
*/
#define OS_TIMER_FREQ      ((158515200u) / 2 / 16)
#define OS_TICK_FREQ       (1000u)
#define OS_INT_FREQ        (OS_TICK_FREQ)
#define OS_TIMER_RELOAD    ((OS_TIMER_FREQ / OS_INT_FREQ) - 1)

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
//
//      Debug unit
//
#define DBGU_BASE_ADDR           (0xFFFFF200u)
#define DBGU_CR                  (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x00u))      // Control Register
#define DBGU_MR                  (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x04u))      // Mode Register
#define DBGU_IER                 (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x08u))      // Interrupt Enable Register
#define DBGU_IDR                 (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x0Cu))      // Interrupt Disable Register
#define DBGU_IMR                 (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x10u))      // Interrupt Mask Register
#define DBGU_SR                  (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x14u))      // Channel Status Register
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
#define RSTC_BODIEN              (1u << 16)                                         // Brownout interrupt enable
#define RSTC_URSTIEN             (1u <<  4)                                         // User reset interrupt enable
#define RSTC_URSTEN              (1u <<  0)                                         // User reset enable
#define RSTC_BODSTS              (1u <<  1)                                         // Brownout status
#define RSTC_URSTS               (1u <<  0)                                         // User reset status
//
//      Real time timer
//
#define RTT_BASE_ADDR            (0xFFFFFD20u)
#define RTT_MR                   (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x00u))
#define RTT_SR                   (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x0Cu))
#define RTT_RTTINCIEN            (1u << 17)
#define RTT_ALMIEN               (1u << 16)
#define RTT_RTTINC               (1u << 1)
#define RTT_ALMS                 (1u << 0)
//
//      Periodic interval timer
//
#define PIT_BASE_ADDR            (0xFFFFFD30u)
#define PIT_MR                   (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x00u))
#define PIT_SR                   (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x04u))
#define PIT_PIVR                 (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x08u))
#define PIT_PIIR                 (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x0Cu))
//
//      Watchdog
//
#define WDT_BASE_ADDR            (0xFFFFFD40u)
#define WDT_CR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x00u))
#define WDT_MR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x04u))
#define WDT_SR                   (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x08u))
#define WDT_WDFIEN               (1u << 12)                                         // Watchdog interrupt enable flag in mode register
#define WDT_WDERR                (1u <<  1)                                         // Watchdog error status flag
#define WDT_WDUNF                (1u <<  0)                                         // Watchdog underflow status flag
//
//      PIO control register
//
#define PIOA_BASE_ADDR           (0xFFFFF200u)
#define PIOB_BASE_ADDR           (0xFFFFF400u)
#define PIOC_BASE_ADDR           (0xFFFFF600u)
#define PIOD_BASE_ADDR           (0xFFFFF800u)
//
//      Power management controller
//
#define PMC_BASE_ADDR            (0xFFFFFC00u)
#define PMC_CKGR_PLLAR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x28u))       // PLLA register
#define PMC_CKGR_PLLBR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x2cu))       // PLLB register
#define PMC_SCDR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x04u))       // System clock disable register
#define PMC_PCER                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x10u))       // Peripheral clock enable register
#define PMC_MOR                  (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x20u))       // Main oscillator register
#define PMC_PLLR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x2cu))       // PLL register
#define PMC_MCKR                 (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x30u))       // Master clock register
#define PMC_SR                   (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x68u))       // Status register
#define PMC_IMR                  (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x6Cu))       // Interrupt mask register
#define PMC_MOSCS                (1u <<  0)
#define PMC_LOCKA                (1u <<  1)
#define PMC_LOCKB                (1u <<  2)
#define PMC_MCKRDY               (1u <<  3)
#define PMC_PCKRDY2              (1u << 10)
#define PMC_PCKRDY1              (1u <<  9)
#define PMC_PCKRDY0              (1u <<  8)
#define PMC_MASK_ALL             (PMC_PCKRDY2 | PMC_PCKRDY1 | PMC_PCKRDY0 | \
                                  PMC_MCKRDY  | PMC_LOCKB   | PMC_MOSCS)
//
//      MATRIX + EBI interface
//
#define MATRIX_BASE_ADDR         (0xFFFFEE00u)                                      // MATRIX Base Address
#define MATRIX_MCFG              (*(volatile OS_U32*)(MATRIX_BASE_ADDR + 0x000u))  // MATRIX Master configuration register
#define MATRIX_EBICSA            (*(volatile OS_U32*)(MATRIX_BASE_ADDR + 0x11Cu))  // MATRIX EBI Chip Select Assignment register
//
//      PIOC, used as data BUS
//
#define PIOC_PDR                 (*(volatile OS_U32*)(PIOC_BASE_ADDR + 0x04u))      // PIOC disable register
#define PIOC_MDDR                (*(volatile OS_U32*)(PIOC_BASE_ADDR + 0x54u))      // PIOC multi driver disable register
#define PIOC_ASR                 (*(volatile OS_U32*)(PIOC_BASE_ADDR + 0x70u))      // PIOC peripheral A select register
//
//      SDRAM controller
//
#define SDRAMC_BASE_ADDR         (0xFFFFEA00u)                                      // SDRAMC Base Address
#define SDRAMC_MR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x00u))    // (SDRAMC) SDRAM Controller Mode Register
#define SDRAMC_TR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x04u))    // (SDRAMC) SDRAM Controller Refresh timer Register
#define SDRAMC_CR                (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x08u))    // (SDRAMC) SDRAM Controller Configuration Register
#define SDRAMC_LPR               (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x10u))    // (SDRAMC) SDRAM Controller Low Power Register
#define SDRAMC_MDR               (*(volatile OS_U32*)(SDRAMC_BASE_ADDR + 0x24u))    // (SDRAMC) SDRAM Controller Memory Device Register

#define SDRAMC_MODE_NORMAL_CMD   (0u)                                               // (SDRAMC) Normal Mode
#define SDRAMC_MODE_NOP_CMD      (1u)                                               // (SDRAMC) Issue a All Banks Precharge Command at every access
#define SDRAMC_MODE_PRCGALL_CMD  (2u)                                               // (SDRAMC) Issue a All Banks Precharge Command at every access
#define SDRAMC_MODE_LMR_CMD      (3u)                                               // (SDRAMC) Issue a Load Mode Register at every access
#define SDRAMC_MODE_RFSH_CMD     (4u)                                               // (SDRAMC) Issue a Refresh

#define SDRAM_BASE_ADDR          (0x20000000u)
//
//      Advanced interrupt controller (AIC)
//
#define AIC_BASE_ADDR            (0xFFFFF000u)
#define AIC_SMR_BASE_ADDR        (AIC_BASE_ADDR + 0x00u)
#define AIC_SVR_BASE_ADDR        (AIC_BASE_ADDR + 0x80u)
#define AIC_SVR0                 (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x000u))
#define AIC_SVR1                 (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x004u))
#define AIC_IVR                  (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x100u))
#define AIC_ISR                  (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x108u))
#define AIC_IPR                  (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x10cu))
#define AIC_IDCR                 (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x124u))
#define AIC_ICCR                 (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x128u))
#define AIC_IECR                 (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x120u))
#define AIC_EOICR                (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x130u))
#define AIC_SPU                  (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x134u))
#define AIC_DCR                  (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x138u))
#define AIC_FFDR                 (*(volatile OS_U32*)(AIC_BASE_ADDR     + 0x144u))
//
//      AIC interrupt sources and peripheral IDs
//
#define SYSTEM_IRQ_ID       (1u)   // System IRQ ID
#define PIOD_ID             (4u)

#define NUM_INT_SOURCES     (32u)

#define INT_PRIORITY_MASK   (7u)
#define NUM_INT_PRIORITIES  (8u)

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
#if  (defined(__ICCARM__))  // IAR
  #pragma data_alignment=16384
  static __no_init OS_U32 _TranslationTable[0x1000];
#elif (defined(__CC_ARM))    // Keil
  #pragma arm section zidata = "NO_INIT"  // Has to be declared as separate section UNINIT in the scatter file
  __align(16384) static OS_U32 _TranslationTable[0x1000];
  #pragma arm section zidata              // back to default (.bss section)
#elif (defined(__GNUC__))    // GCC
  static OS_U32 _TranslationTable[0x1000] __attribute__ ((aligned (16384), section (".non_init")));
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
*       _HandlePmcIrq()
*
*  Function description
*    Power management controller interrupt
*
*  Notes
*    (1) If not used for application, this handler may be removed
*/
static void _HandlePmcIrq(void) {
#if (OS_DEBUG != 0)
  OS_U32 IrqSource;

  IrqSource  = PMC_IMR;
  IrqSource &= (PMC_SR & PMC_MASK_ALL);
  if (IrqSource) {  // PMC interrupt pending?
    while (1) {
      ;             // Not implemented
    }
  }
#endif
}

/*********************************************************************
*
*       _HandleRttIrq()
*
*  Function description
*    Real time timer interrupt handler
*
*  Notes
*    (1) If not used for application, this handler may be removed
*/
static void _HandleRttIrq(void) {
#if (OS_DEBUG != 0)
  OS_U32 IrqStatus;
  OS_U32 IrqEnabled;

  IrqEnabled = RTT_MR & (RTT_RTTINCIEN  | RTT_ALMIEN);
  IrqStatus  = RTT_SR & (RTT_RTTINC | RTT_ALMS);
  if ((IrqStatus & RTT_RTTINC) && (IrqEnabled & RTT_RTTINCIEN )) {  // RTT inc. interrupt pending?
    while (1) {
      ;                                                             // Not implemented
    }
  }
  if ((IrqStatus & RTT_ALMS) && (IrqEnabled & RTT_ALMIEN )) {       // Alarm interrupt pending?
    while (1) {
      ;                                                             // Not implemented
    }
  }
#endif
}

/*********************************************************************
*
*       _HandleDbguIrq()
*
*  Function description
*    Debug unit interrupt handler
*
*  Notes
*    (1) If not used for application, this handler may be removed
*/
static void _HandleDbguIrq(void) {
#if (OS_DEBUG != 0)
  OS_U32 IrqSource;

  IrqSource  = DBGU_IMR;
  IrqSource &= (DBGU_SR & DBGU_MASK_ALL);
  if (IrqSource) {  // Any interrupt pending?
    while (1) {
      ;             // Not implemented
    }
  }
#endif
}

/*********************************************************************
*
*       _HandleRstcIrq()
*
*  Function description
*    Reset controller interrupt handler
*
*  Notes
*    (1) If not used for application, this handler may be removed
*/
static void _HandleRstcIrq(void) {
#if (OS_DEBUG != 0)
  OS_U32 IrqStatus;
  OS_U32 IrqEnabled;

  IrqEnabled = RSTC_MR & (RSTC_BODIEN | RSTC_URSTIEN);
  IrqStatus  = RSTC_SR & (RSTC_BODSTS | RSTC_URSTS);
  if ((IrqStatus & RSTC_BODSTS) && (IrqEnabled & RSTC_BODIEN )) {  // Brownout interrupt pending?
    while (1) {
      ;                                                            // Not implemented
    }
  }
  if ((IrqStatus & RSTC_URSTS) && (IrqEnabled & RSTC_URSTIEN )) {  // User reset interrupt pending?
    while (1) {
      ;                                                            // Not implemented
    }
  }
#endif
}

/*********************************************************************
*
*       _HandleWdtIrq()
*
*  Function description
*    watchdog timer interrupt handler
*
*  Notes
*    (1) If not used for application, this handler may be removed
*/
static void _HandleWdtIrq(void) {
#if (OS_DEBUG != 0)
  OS_U32 IrqStatus;

  IrqStatus = WDT_SR & (WDT_WDERR | WDT_WDUNF);
  if (IrqStatus && (WDT_MR & WDT_WDFIEN)) {  // Watchdog error interrupt pending?
    while (1) {
      ;                                      // Not implemented
    }
  }
#endif
}

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

  IrqSource = AIC_ISR;  // detect source of uninstalled interrupt
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
*       _InitClocks()
*
*  Function description
*    Initialize main clock and PLLs
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called!
*    (2) Settings used here are for AT91SAM9260-EK eval board
*        and may have to be modified for other target board or frequency
*/
static void _InitClocks(OS_U32 PLLA_Val, OS_U32 PLLB_Val, OS_U32 MCKR_Val) {
  //
  // Enable main clock OSC and wait until clock is stable
  //
  PMC_MOR = ((48/8) << 8) + 1u;  // Startup time = 48 slow clocks, Main oscillator enable
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
  while ((PMC_SR & PMC_LOCKB) == 0) {
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
*    (2) Settings used here are for AT91SAM9260-EK eval board
*        and may have to be modified for other target boards
*/
static void _InitSDRAM(void) {
#if (SDRAM_TARGET == 0)                // Must not be initialized, if program was downloaded to SDRAM
  volatile int i;
  //
  // Setup bus interface for SDRAM if not already initialized
  //
  if ((MATRIX_EBICSA & (1u << 1)) == 0) {
    //
    // Activate CS1 for SDRAM
    //
    MATRIX_EBICSA |= (1u <<  1)       // Activate CS1 for SDRAM
                  |  (1u << 16)       // Memory is powered from 3.3V
                  ;
    //
    // Setup PIO for SDRAM
    //
    PMC_PCER  |= (1u << PIOD_ID);     // Enable peripheral clock for PIOC
    PIOC_MDDR  = 0xFFFF0000;          // Disable Multi driver for data lines on PIOC
    PIOC_ASR   = 0xFFFF0000;          // Upper 16 lines are data lines for SDRAM
    PIOC_PDR   = 0xFFFF0000;          // Disable PIO for upper 16 bits
    //
    // Initialize SDRAM controller
    //
    SDRAMC_MR = SDRAMC_MODE_NOP_CMD;  // Output one NOP via mode register
    //
    // Setup refresh timer. We assume one refresh cycle every 7.81 micro seconds.
    // If SDRAM runs with other refresh rates (for example 15.61 us), this may be changed
    //
    SDRAMC_TR = (781 * ((158515200u) / 2000)) / 100000;
    SDRAMC_CR = (0x01u <<  0)         // 9 Column bits
              | (0x02u <<  2)         // 13 Rows
              | (0x01u <<  4)         // 4 Banks
              | (0x02u <<  5)         // 2 CAS cycles
              | (0x00u <<  7)         // 32 bit Data BUS
              | (0x02u <<  8)         // 2 write recovery cycles
              | (0x07u << 12)         // 7 RAS cycles
              | (0x02u << 16)         // 2 RAS precharge time cycles
              | (0x02u << 20)         // 2 cycles RAS to CAS delay
              | (0x05u << 24)         // 5 cycles RAS active time
              | (0x08u << 28)         // 8 cycles command recovery time
              ;
    SDRAMC_LPR    = 0;                // Reset Low Power register
    SDRAMC_MDR    = 0;                // Set Memory device register to SDRAM device
    //
    // Delay at least 200 us
    //
    for (i = 0; i < 10000; i++) {     // Delay
    }
    *(volatile unsigned int*)(SDRAM_BASE_ADDR) = 0;  // Output a NOP to SDRAM
    for (i = 0; i < 10000; i++) {     // Delay
    }
    //
    // Perform a "precharge All" sequence
    //
    SDRAMC_MR = SDRAMC_MODE_PRCGALL_CMD;
    *(volatile unsigned int*)(SDRAM_BASE_ADDR) = 1u;  // Write to SDRAM
    //
    // Perform 8 auto-refresh (CBR) cycles.
    //
    SDRAMC_MR = SDRAMC_MODE_RFSH_CMD;                 // Set mode
    for (i = 0; i < 8; i++) {                         // Auto-Refresh, 8 times
      *(volatile unsigned int*)SDRAM_BASE_ADDR = i;   // Dummy write
    }
    //
    // Issue a mode register cycle
    //
    SDRAMC_MR = SDRAMC_MODE_LMR_CMD;
    *(volatile unsigned int*)(SDRAM_BASE_ADDR) = 0x55555555;   // Write to SDRAM, the value does not matter
    //
    // Finally set controller to normal mode and perform a dummy write to any location
    //
    SDRAMC_MR = SDRAMC_MODE_NORMAL_CMD;
    *(volatile unsigned int*)(SDRAM_BASE_ADDR) = 0xababcafeu;  // Write to SDRAM, the value does not matter
  }
#endif
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
  //
  // Call to following handlers may be removed if not used by application
  //
  _HandlePmcIrq();
  _HandleRttIrq();
  _HandleDbguIrq();
  _HandleRstcIrq();
  _HandleWdtIrq();
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
  // Initialize timer for embOS
  //
  PIT_MR = ((OS_TIMER_RELOAD & 0x000FFFFFu) | (1u << 25) | (1u << 24));
  OS_ARM_InstallISRHandler(SYSTEM_IRQ_ID, _OS_ISR_Tick);
  OS_ARM_EnableISR(SYSTEM_IRQ_ID);
  //
  // Inform embOS about the timer settings
  //
  {
    OS_SYSTIMER_CONFIG SysTimerConfig = {OS_TIMER_FREQ, OS_INT_FREQ, OS_TIMER_DOWNCOUNTING, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending};
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
void OS_Idle(void) {    // Idle loop: No task is ready to execute
  while (1) {           // Nothing to do ... wait for interrupt
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

  pISR = (OS_ISR_HANDLER*)AIC_IVR;   // Read interrupt vector to release NIRQ to CPU core
#if (OS_DEBUG != 0)
  AIC_IVR = (OS_U32)pISR;            // Write back any value to IVR register to allow interrupt stacking in protected mode
#endif
  OS_INT_EnterNestable();            // Inform OS that interrupt handler is running
  pISR();                            // Call interrupt service routine
  OS_INT_Disable();                  // Disable interrupts and unlock
  AIC_EOICR = 0;                     // interrupt controller =>  Restore previous priority
  OS_INT_LeaveNestable();            // Leave interrupt, perform task switch if required
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
  pPrio   = (OS_U32*)AIC_SMR_BASE_ADDR;
  OldPrio = pPrio[ISRIndex];
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
  WDT_MR = (1u << 15);                    // Initially disable watchdog
  //
  // Initialize main clock oscillator and PLLs
  //
  _InitClocks(PLLAR_VALUE, PLLBR_VALUE, MCKR_VALUE);
  //
  // Initialize real time timer
  //
  _InitRTT();
  //
  // Initialize SDRAM
  //
  _InitSDRAM();
  //
  //
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                             Mode                    VAddr  PAddr  Size[MB]
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,   0x000, 0x200, 0x001);    // SDRAM, remap 1MB to address 0x0 to place interrupt vectors
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x001, 0x0,   0x001);    // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x002, 0x002, 0x001);    // Internal SRAM0, original address, non-cacheable, non-bufferable
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x003, 0x003, 0x001);    // Internal SRAM1, NON cacheable, NON bufferable
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x004, 0x0,   0x001);    // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x005, 0x005, 0x001);    // UHP, USB Host controller (OHCI compliant)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x006, 0x0,   0x1FA);    // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_C_B,   0x200, 0x200, 0x040);    // SDRAM, 64MB, cacheable, bufferable
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x240, 0x200, 0x040);    // SDRAM, 64MB, NON cacheable, NON bufferable, same physical address cacheable SDRAM
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x280, 0x0,   0x180);    // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0x400, 0x400, 0x020);    // NAND controller, NON cacheable, NON bufferable
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,    0x420, 0x0,   0xBDF);    // Unmapped area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NC_NB, 0xfff, 0xfff, 0x001);    // SFR area NON cacheable, NON bufferable
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  //
  // Initialize interrupt controller
  //
  _InitAIC();
  //
  // Initialize reset control
  //
  RSTC_MR = ((0xA5u << 24) | RSTC_URSTEN);  // write KEY and URSTEN to allow USER RESET
  return 1;
}

/*************************** End of file ****************************/
