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
File    : RTOSInit_AT91SAM7S.c
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
*       System tick settings
*/
#define OS_TIMER_FREQ  (2995200u)
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
//
//      Debug unit
//
#define DBGU_BASE_ADDR     (0xFFFFF200u)
#define DBGU_IMR           (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x10u))  // Interrupt Mask Register
#define DBGU_SR            (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x14u))  // Channel Status Register
#define DBGU_CPUID         (*(volatile OS_U32*)(DBGU_BASE_ADDR + 0x40u))  // Chip ID Register

#define DBGU_COMMRX        (1u << 31)
#define DBGU_COMMTX        (1u << 30)
#define DBGU_RXBUFF        (1u << 12)
#define DBGU_TXBUFE        (1u << 11)
#define DBGU_TXEMPTY       (1u <<  9)
#define DBGU_PARE          (1u <<  7)
#define DBGU_FRAME         (1u <<  6)
#define DBGU_OVRE          (1u <<  5)
#define DBGU_ENDTX         (1u <<  4)
#define DBGU_ENDRX         (1u <<  3)
#define DBGU_TXRDY         (1u <<  1)
#define DBGU_RXRDY         (1u <<  0)
#define DBGU_MASK_ALL      (DBGU_COMMRX | DBGU_COMMTX  | DBGU_RXBUFF |  \
                            DBGU_TXBUFE | DBGU_TXEMPTY | DBGU_PARE   |  \
                            DBGU_FRAME  | DBGU_OVRE    | DBGU_ENDTX  |  \
                            DBGU_ENDRX  | DBGU_TXRDY   | DBGU_RXRDY)
//
//      Reset controller
//
#define RSTC_BASE_ADDR     (0xFFFFFD00u)
#define RSTC_CR            (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x00u))
#define RSTC_SR            (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x04u))
#define RSTC_MR            (*(volatile OS_U32*)(RSTC_BASE_ADDR + 0x08u))

#define RSTC_BODIEN        (1u << 16)  // Brownout interrupt enable
#define RSTC_URSTIEN       (1u <<  4)  // User reset interrupt enable
#define RSTC_BODSTS        (1u <<  1)  // Brownout status
#define RSTC_URSTS         (1u <<  0)  // User reset status
//
//      Real time timer
//
#define RTT_BASE_ADDR      (0xFFFFFD20u)
#define RTT_MR             (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x00u))
#define RTT_SR             (*(volatile OS_U32*)(RTT_BASE_ADDR + 0x0Cu))

#define RTT_RTTINCIEN      (1u << 17)
#define RTT_ALMIEN         (1u << 16)
#define RTT_RTTINC         (1u <<  1)
#define RTT_ALMS           (1u <<  0)

//
//      Periodic interval timer
//
#define PIT_BASE_ADDR      (0xFFFFFD30u)
#define PIT_MR             (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x00u))
#define PIT_SR             (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x04u))
#define PIT_PIVR           (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x08u))
#define PIT_PIIR           (*(volatile OS_U32*)(PIT_BASE_ADDR + 0x0Cu))

//
//      Watchdog
//
#define WDT_BASE_ADDR      (0xFFFFFD40u)
#define WDT_CR             (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x00u))
#define WDT_MR             (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x04u))
#define WDT_SR             (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x08u))

#define WDT_WDFIEN         (1u << 12)  // Watchdog interrupt enable flag in mode register
#define WDT_WDERR          (1u <<  1)  // Watchdog error status flag
#define WDT_WDUNF          (1u <<  0)  // Watchdog underflow status flag

//
//      Memory controller
//
#define MC_BASE_ADDR       (0xFFFFFF00u)
#define MC_FMR0            (*(volatile OS_U32*)(MC_BASE_ADDR + 0x60u))
#define MC_FMR1            (*(volatile OS_U32*)(MC_BASE_ADDR + 0x70u))

#define MC_FWS_0FWS        (0u << 8)  // (MC) 1 cycle for Read,  2 for Write operations
#define MC_FWS_1FWS        (1u << 8)  // (MC) 2 cycles for Read, 3 for Write operations
#define MC_FWS_2FWS        (2u << 8)  // (MC) 3 cycles for Read, 4 for Write operations
#define MC_FWS_3FWS        (3u << 8)  // (MC) 4 cycles for Read, 4 for Write operations

#define MC_FWS             (MC_FWS_1FWS)
#define FMCN_VAL           (72)

//
//      PIO control register
//
#define PIOA_BASE_ADDR     (0xFFFFF400u)
#define PIOA_PDR           (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x04u))  // PIOA disable register
#define PIOA_ASR           (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x70u))  // PIOA "A" peripheral select register
#define PIOA_BSR           (*(volatile OS_U32*)(PIOA_BASE_ADDR + 0x74u))  // PIOA "B" peripheral select register

//
//      Power management controller
//
#define PMC_BASE_ADDR      (0xFFFFFC00u)
#define PMC_SCDR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x04u))  // System Clock Disable Register
#define PMC_PCER           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x10u))  // Peripheral clock enable register
#define PMC_MOR            (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x20u))  // main oscillator register
#define PMC_PLLR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x2Cu))  // PLL register
#define PMC_MCKR           (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x30u))  // Master clock register
#define PMC_SR             (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x68u))  // status register
#define PMC_IMR            (*(volatile OS_U32*)(PMC_BASE_ADDR + 0x6Cu))  // interrupt mask register

#define PMC_PCKRDY2        (1u << 10)
#define PMC_PCKRDY1        (1u <<  9)
#define PMC_PCKRDY0        (1u <<  8)
#define PMC_MCKRDY         (1u <<  3)
#define PMC_LOCK           (1u <<  2)
#define PMC_MOSCS          (1u <<  0)
#define PMC_MASK_ALL       (PMC_PCKRDY2 | PMC_PCKRDY1 | PMC_PCKRDY0 | \
                            PMC_MCKRDY  | PMC_LOCK    | PMC_MOSCS)

//
//      Advanced interrupt controller (AIC)
//
#define AIC_BASE_ADDR      (0xFFFFF000u)
#define AIC_SMR_BASE_ADDR  (AIC_BASE_ADDR + 0x00u)
#define AIC_SVR_BASE_ADDR  (AIC_BASE_ADDR + 0x80u)
#define AIC_SVR0           (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x00u))
#define AIC_SVR1           (*(volatile OS_U32*)(AIC_SVR_BASE_ADDR + 0x04u))
#define AIC_IVR            (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x100u))
#define AIC_ISR            (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x108u))
#define AIC_IPR            (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x10cu))
#define AIC_IDCR           (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x124u))
#define AIC_ICCR           (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x128u))
#define AIC_IECR           (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x120u))
#define AIC_EOICR          (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x130u))
#define AIC_SPU            (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x134u))
#define AIC_DCR            (*(volatile OS_U32*)(AIC_BASE_ADDR + 0x138u))

//
//      AIC interrupt sources and peripheral IDs
//
#define SYSTEM_IRQ_ID      ( 1)  // System IRQ ID
#define US0IRQ_ID          ( 6)  // USART Channel 0 interrupt
#define US1IRQ_ID          ( 7)  // USART Channel 1 interrupt
#define US2IRQ_ID          ( 8)  // USART Channel 2 interrupt
#define TC0IRQ_ID          (12)  // Timer Channel 0 interrupt

#define NUM_INT_SOURCES    (32)

#define INT_PRIORITY_MASK  (0x07)
#define NUM_INT_PRIORITIES (8)


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
  // Setup Flash microsecond cycle numbers and wait states
  //
  MC_FMR0 = ((0xFF << 16) & (FMCN_VAL << 16)) | MC_FWS;
  if ( ((DBGU_CPUID >> 8) & 0xFF) >= 0x09) {
    //
    // The Flash size is above 256KB, initialize the second Flash controller
    //
    MC_FMR1 = ((0xFF << 16) & (FMCN_VAL << 16)) | MC_FWS;
  }
  //
  // Start-up main oscillator
  //
  PMC_MOR  = (1uL << 0)       // Enable main oscillator
            | (0x40 << 8)     // Set startup delay time, 512 slow clock cycles
            ;
  //
  // Wait until end of startup time => MOSCS set to 1
  //
  while ((PMC_SR & (1uL << 0)) == 0);
  //
  // Initialize PLL to run at 96 MHz
  //
  PMC_PLLR = 0x05             // Divider = 5
            | (28  << 8)      //
            | (25  << 16)     //
            | (1uL << 28)     // USBDIV is value + 1 = 2
            ;
  //
  // Wait until pll is locked, until LOCK bit is set to 1
  //
  while((PMC_SR & (1uL << 2)) == 0);
  //
  // Wait until master clock is ready, until MCKRDY is set to 1
  //
  while((PMC_SR & (1uL << 3)) == 0);
  //
  // Select main clock as master clock, set divider
  //
  PMC_MCKR = 0                // Select main clock oscillator as source
            | (1uL <<  2)     // Divide clock by 2
            ;
  //
  // Wait until master clock is ready, until MCKRDY is set to 1
  //
  while((PMC_SR & (1uL << 3)) == 0);
  //
  // Select PLL clock as master clock, set divider
  //
  PMC_MCKR = 3                // Select PLL clock as source
            | (1uL <<  2)     // Divide clock by 2
            ;
  //
  // Wait until master clock is ready, until MCKRDY is set to 1
  //
  while((PMC_SR & (1uL << 3)) == 0);
  //
  // Initialize reset control
  //
  RSTC_MR = (((OS_U32)0xA5u << 24) | (1u << 0));  // write KEY and URSTEN to allow USER RESET
  return 1;
}

/*********************************************************************
*
*       Optional speed optimization for IAR
*
**********************************************************************
*/

#if (defined(__ICCARM__) && defined(OPTIMIZE_SPEED) && (OPTIMIZE_SPEED != 0))
/*********************************************************************
*
*       OS_EnableInt()
*/
OS_INTERWORK __arm __ramfunc void OS_EnableInt(void) {
  __set_CPSR(__get_CPSR() & ~0x80uL);  // Clear IRQ only, keep other bits unchanged
}

/*********************************************************************
*
*       OS_INT_DisablesableInt()
*/
OS_INTERWORK __arm __ramfunc void OS_INT_DisablesableInt(void) {
  __set_CPSR(__get_CPSR() | 0x80);  // Set IRQ only, keep other bits unchanged
}
#endif  // (defined(__ICCARM__) && defined(OPTIMIZE_SPEED) && (OPTIMIZE_SPEED != 0))

/*************************** End of file ****************************/
