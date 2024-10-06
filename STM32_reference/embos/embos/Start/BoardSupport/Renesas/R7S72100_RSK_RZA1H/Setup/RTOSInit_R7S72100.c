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
File    : RTOSInit_R7S72100.c
Purpose : Initializes and handles the hardware for embOS

Literature:
  [1]  Generic Interrupt Controller (GIC-390) Errata Notice
       (\\fileserver\Techinfo\Company\ARM\GIC_GenericInterruptController\GIC_390_Errata_Notice_v6.pdf)
*/

#include "RTOS.h"
#include "SEGGER_SYSVIEW.h"

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/
#ifndef   COPY_TO_SRAM
  #define COPY_TO_SRAM  (0)  // Code and data may be copied from external Flash into internal SRAM
#endif

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
#define OS_TIMER_FREQ    (399900000u / 12)
#define OS_TICK_FREQ     (1000u)
#define OS_INT_FREQ      (OS_TICK_FREQ)
#define OS_TIMER_RELOAD  ((OS_TIMER_FREQ / OS_INT_FREQ) - 1)

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
// L2 Cache controller
//
#define L2CACHE_BASE_ADDR  (0x3FFFF000u)

//
// System, clock and standby control
//
#define CPG_BASE_ADDR      (0xFCFE0000u)
#define CPG_FRQCR          (*(volatile OS_U16*)(CPG_BASE_ADDR + 0x0010u))
#define CPG_FRQCR2         (*(volatile OS_U16*)(CPG_BASE_ADDR + 0x0014u))
#define CPG_SYSCR3         (*(volatile OS_U8* )(CPG_BASE_ADDR + 0x0408u))
#define CPG_STBCR4         (*(volatile OS_U8* )(CPG_BASE_ADDR + 0x0424u))
#define CPG_STBCR5         (*(volatile OS_U8* )(CPG_BASE_ADDR + 0x0428u))

//
// BUS controller
//
#define BSC_BASE_ADDR      (0x3FFFC000u)
#define BSC_CS0BCR         (*(volatile unsigned long*)(BSC_BASE_ADDR + 0x0004u))
#define BSC_CS0WCR         (*(volatile unsigned long*)(BSC_BASE_ADDR + 0x0028u))

//
// OS timer
//
#define OSTM0_BASE_ADDR    (0xFCFEC000u)
#define OSTM1_BASE_ADDR    (0xFCFEC400u)
#define OSTM0_INT_ID       (134)
#define OSTM1_INT_ID       (135)
#define OSTM0_STB_BIT      (  1)
#define OSTM1_STB_BIT      (  0)
#define OSTM_BASE_ADDR     (OSTM0_BASE_ADDR)  // Select the timer used for OS
#define OSTM_INT_ID        (OSTM0_INT_ID)
#define OSTM_STB_BIT       (OSTM0_STB_BIT)
#define OSTM_CMP           (*(volatile OS_U32*)(OSTM_BASE_ADDR + 0x00u))
#define OSTM_CNT           (*(volatile OS_U32*)(OSTM_BASE_ADDR + 0x04u))
#define OSTM_TE            (*(volatile OS_U8* )(OSTM_BASE_ADDR + 0x10u))
#define OSTM_TS            (*(volatile OS_U8* )(OSTM_BASE_ADDR + 0x14u))
#define OSTM_TT            (*(volatile OS_U8* )(OSTM_BASE_ADDR + 0x18u))
#define OSTM_CTL           (*(volatile OS_U8* )(OSTM_BASE_ADDR + 0x20u))

//
// Watchdog
//
#define WT_BASE_ADDR       (0xFCFE0000u)
#define WT_CSR             (*(volatile OS_U16*)(WT_BASE_ADDR + 0x00u))
#define WT_CNT             (*(volatile OS_U16*)(WT_BASE_ADDR + 0x02u))
#define WR_CSR             (*(volatile OS_U16*)(WT_BASE_ADDR + 0x04u))

//
// Generic interrupt controller
//
#define GIC_BASE_ADDR      (0xE8200000u)
#define GICD_BASE_ADDR     (GIC_BASE_ADDR + 0x1000u)                       // Distributor interface
#define GICC_BASE_ADDR     (GIC_BASE_ADDR + 0x2000u)                       // CPU interface
#define GICC_HPPIR         (*(volatile OS_U32*)(GICC_BASE_ADDR + 0x018u))  // Highest Priority Pending Interrupt Register
#define GICD_IPRIORITY     (*(volatile OS_U32*)(GICD_BASE_ADDR + 0x400u))  // Interrupt Priority Registers
#define NUM_INT_SOURCES    (587u)

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
extern const OS_ARM_L2CACHE_API OS_L2CACHE_L2C310;
extern       unsigned int       __vector;  // Start of vector table, defined in startup file

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Interrupt configuration registers default table.
//
static const OS_U32 _INTC_ICDICFRN_Table[] = {
  0xAAAAAAAA,            // ICDICFR0
  0x00000055,            // ICDICFR1
  0xFFFD5555,            // ICDICFR2
  0x555FFFFF,            // ICDICFR3
  0x55555555,            // ICDICFR4
  0x55555555,            // ICDICFR5
  0x55555555,            // ICDICFR6
  0x55555555,            // ICDICFR7
  0x5555F555,            // ICDICFR8
  0x55555555,            // ICDICFR9
  0x55555555,            // ICDICFR10
  0xF5555555,            // ICDICFR11
  0xF555F555,            // ICDICFR12
  0x5555F555,            // ICDICFR13
  0x55555555,            // ICDICFR14
  0x55555555,            // ICDICFR15
  0x55555555,            // ICDICFR16
  0xFD555555,            // ICDICFR17
  0x55555557,            // ICDICFR18
  0x55555555,            // ICDICFR19
  0xFFD55555,            // ICDICFR20
  0x5F55557F,            // ICDICFR21
  0xFD55555F,            // ICDICFR22
  0x55555557,            // ICDICFR23
  0x55555555,            // ICDICFR24
  0x55555555,            // ICDICFR25
  0x55555555,            // ICDICFR26
  0x55555555,            // ICDICFR27
  0x55555555,            // ICDICFR28
  0x55555555,            // ICDICFR29
  0x55555555,            // ICDICFR30
  0x55555555,            // ICDICFR31
  0x55555555,            // ICDICFR32
  0x55555555,            // ICDICFR33
  0x55555555,            // ICDICFR34
  0x55555555,            // ICDICFR35
  0x55555555             // ICDICFR36
};

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
*       Interrupt vector table
*
*  Notes
*    The translation table has to be located in un-initialized memory,
*    because it may be filled from startup, before segments and variables
*    are initialized.
*/
#if   (defined(__ICCARM__))  // IAR
  #pragma data_alignment=4
  static __no_init OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES];
#elif (defined(__CC_ARM))    // Keil
  #pragma arm section zidata = "NO_INIT"  // Has to be declared as separate section UNINIT in the scatter file
  __align(4) static OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES];
  #pragma arm section zidata              // back to default (.bss section)
#elif (defined(__GNUC__))    // GCC
  static OS_ISR_HANDLER* _apIsrHandler[NUM_INT_SOURCES] __attribute__ ((aligned (4), section (".non_init._apIsrHandler")));
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
*       _OS_ISR_Undefined()
*
*  Function description
*    Is called when a non-installed interrupt was detected.
*    As interrupt pending condition of peripherals has to be reset,
*    program will not continue when interrupt is ignored.
*/
static void _OS_ISR_Undefined(void) {
  volatile int Dummy;

  Dummy = 1;
  while (Dummy > 0) {
    ;  // You may set a breakpoint here to detect non-installed ISR-handler
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
*       _InitGIC()
*
*  Function description
*    Initialize interrupt controller by setting default vectors
*    and clearing all interrupts
*/
static void _InitGIC(void) {
  OS_U32 i;

  //
  // Set GIC base address
  //
  OS_GIC_SetCPUInterfaceBaseAddr(GICC_BASE_ADDR);
  OS_GIC_SetDistributionBaseAddr(GICD_BASE_ADDR);
  //
  // Configure all interrupts
  //
  for (i = 0u; i < NUM_INT_SOURCES; i++) {
    _apIsrHandler[i] = _OS_ISR_Undefined;  // Set default interrupt handler
    OS_GIC_SetIntSecure(i);                // Set to secure
    OS_GIC_DisableInt(i);                  // Disable interrupts
    OS_GIC_ClrPending(i);                  // Clear pending flags
    //
    // Set interrupts to level or edge sensitive
    //
    OS_GIC_SetIntCfg(i, (((OS_U8*)_INTC_ICDICFRN_Table)[i / 4] >> ((i * 2u) % 8u)) & 0x03u);
    //
    // Initialize all Interrupt pointer target registers to address
    // the first core. The first 8 registers are fixed.
    //
    if (i > 8u) {
      OS_GIC_SetTargetProc(i, 0x01u);
    }
  }
  //
  // Configure priority mask to lowest value (highest number) - enables all priorities
  //
  OS_GIC_SetPrioThreshold(0xFFu);
  //
  // This register defines the point at which the priority value fields split
  // into two parts, the group priority field and the sub-priority field.
  //
  OS_GIC_SetPrioGroup(0x00u);
  //
  // Enable the GIC
  //
  OS_GIC_Enable();
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
static void _InitPLL(void) {
  volatile OS_U32  Dummy;
  volatile OS_U32* pMagic ;

  pMagic  = (OS_U32*)0x3FFFFF80u;  // Magic access. Address not described in Manual
  *pMagic = 0x00000001u;
  Dummy   = *pMagic;
  OS_USE_PARA(Dummy);              // Avoid compiler warning about set but unused variable
  //
  // Set up PLL, assuming 13.33 MHz external crystal, Multiply by 30
  //
  CPG_FRQCR  = (0x35u <<  0)  // b7..b0:   Reserved, always 0x35
             | (0x0   <<  8)  // b9..b8:   CPU clock division ratio. 0 => 1/1
             | (0x0   << 10)  // b11..b10: Reserved, always 0
             | (0x1u  << 12)  // b13..b12: Clock output enable: low level in Standby and Deep standby mode
             | (0x0   << 14)  // b14:      Unstable clock output when XTAL gain changes
             ;                // b15 always 0

  CPG_FRQCR2 = 0x0003u;       // Image Processing Clock division ratio. 3 => 1/3
  //
  // Enable on chip data retention RAM
  //
  CPG_SYSCR3 = 0x0Fu;
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
  return OSTM_CNT;
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
  return OS_ARM_IsPending(OSTM_INT_ID);
}

/*********************************************************************
*
*       _OS_ISR_Tick()
*
*  Function description
*    This is the hardware timer exception handler.
*/
static void _OS_ISR_Tick(void) {
  OS_TICK_Handle();
#if (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK)
  JLINKDCC_Process();
#endif
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
  volatile OS_U32 Dummy;
           OS_U32 TickPrio;

  OS_INT_IncDI();
  //
  // Ensure caches are synchronized
  //
  OS_ARM_CACHE_Sync();
  //
  // Set vector table base address
  //
  OS_ARM_SetVBAR((OS_U32)&__vector);
  //
  // Initialize interrupt controller
  //
  _InitGIC();
  //
  // Initialize timer for embOS
  //
  CPG_STBCR5 &= ~(1u << OSTM_STB_BIT);  // Release OS timer standby
  Dummy       = CPG_STBCR5;             // Dummy read (may not be required)
  OS_USE_PARA(Dummy);                   // Avoid compiler warning about set but unused variable
  OSTM_TT     = 0x01u;                  // Stop Timer
  OSTM_CTL    = (0u << 0)               // b0: 0 => Do not enable interrupt on start
              | (0u << 1);              // b1: 0 => Interval timer mode
  OSTM_CMP    = OS_TIMER_RELOAD;        // Set timer interval
  OSTM_TS     = 0x01u;                  // Start timer
  OS_ARM_InstallISRHandler(OSTM_INT_ID, _OS_ISR_Tick);
  OS_ARM_ISRSetPrio(OSTM_INT_ID, 0xFFu);               // Set to lowest level, ALL BITS set
  TickPrio  = OS_ARM_ISRSetPrio(OSTM_INT_ID, 0xFFu);   // Read back priority to examine relevant preemption-level-bits
  TickPrio -= 1;                                       // Set to low preemption level, 1 level higher than lowest
  OS_ARM_ISRSetPrio(OSTM_INT_ID, TickPrio);
  OS_ARM_ClearPendingFlag(OSTM_INT_ID);
  OS_ARM_EnableISR(OSTM_INT_ID);
  //
  // Inform embOS about the timer settings
  //
  {
    OS_SYSTIMER_CONFIG SysTimerConfig = {OS_TIMER_FREQ, OS_INT_FREQ, OS_TIMER_DOWNCOUNTING, _OS_GetHWTimerCycles, _OS_GetHWTimer_IntPending};
    OS_TIME_ConfigSysTimer(&SysTimerConfig);
  }
  //
  // Configure and initialize SEGGER SystemView
  //
#if (OS_SUPPORT_TRACE_API != 0)
  SEGGER_SYSVIEW_Conf();
#endif
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
  volatile OS_U32 Dummy;
  OS_U32          int_id;
  OS_U32          icdipr0_value;

  Dummy = GICC_HPPIR;                              // Erratum 801120 [1] - ensure subsequent ICCIAR data will be correct
  OS_USE_PARA(Dummy);                              // Avoid compiler warning about set but unused variable
  int_id = OS_GIC_AckInt();                        // Acknowledge the interrupt and read the interrupt ID
#if (OS_SUPPORT_TRACE_API != 0)
  SEGGER_SYSVIEW_InterruptId = int_id;             // Save current interrupt ID to display in SystemView
#endif
  //
  // Read data of 0x0 (SGI ID0 from CPU0) is possibly affected by 733075
  // Interrupt IDs 0x3FE and 0x3FF are also possibly affected
  //
  if ((int_id == 0u) || (int_id  >= 0x3FEu)) {
    //
    // Erratum 733075 - prevent possible CPU interface deadlock (ARM Errata Notice, PR258-GENC-008719 v6.0).
    // Assumes SGI ID0 from CPU0 is not used as suggested by ARM.
    //
    icdipr0_value  = GICD_IPRIORITY;               // One way to get the value for the workaround write is to read it first
    GICD_IPRIORITY = icdipr0_value;
    __asm volatile("dsb");                         // Ensure write completes before any future ICCHPIR or ICCIAR reads
  }
  if ((int_id == 0x3FEu) || (int_id == 0x3FFu)) {  // Check for spurious interrupts
     _SpuriousIrqHandler();
  } else {
    if (int_id < NUM_INT_SOURCES) {
      pISR = _apIsrHandler[int_id];                // Get interrupt vector handler
      OS_INT_Enter();
      pISR();                                      // Call interrupt service routine
      OS_INT_Leave();
    }
    OS_GIC_EOI(int_id);                            // End interrupt
  }
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
  OS_ISR_HANDLER* pOldHandler;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  pOldHandler             = _apIsrHandler[ISRIndex];
  _apIsrHandler[ISRIndex] = pISRHandler;
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
  OS_GIC_EnableInt(ISRIndex);
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
  OS_GIC_DisableInt(ISRIndex);
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
  OS_U32 OldPrio;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  OldPrio = OS_GIC_SetPrio(ISRIndex, Prio);
  OS_INT_EnableConditional();
  return (int)OldPrio;
}

/*********************************************************************
*
*       OS_ARM_ClearPendingFlag()
*
*  Function description
*    Clears an interrupt pending flag
*
*  Parameters
*    ISRIndex: Index of the interrupt source for which to clear the flag
*/
void OS_ARM_ClearPendingFlag(int ISRIndex) {
#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  OS_GIC_ClrPending(ISRIndex);
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       OS_ARM_IsPending()
*
*  Function description
*    Checks if an interrupt is pending
*
*  Parameters
*    ISRIndex: Index of the interrupt source that shall be checked
*
*  Return value
*    == 0; Interrupt is not pending
*    != 0: Interrupt is pending
*/
unsigned int OS_ARM_IsPending(int ISRIndex) {
  OS_U32 Result;

#if (OS_DEBUG != 0)
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_INT_Disable();
  Result = OS_GIC_IsPending(ISRIndex);
  OS_INT_EnableConditional();
  return Result;
}

/*********************************************************************
*
*       OS_ARM_SetISRCfg()
*
*  Function description
*    Sets the interrupt configuration
*
*  Parameters
*    ISRIndex: ISR index
*    Cfg     : 0  Corresponding interrupt is level-sensitive.
*              1  Corresponding interrupt is edge-triggered.
*/
void OS_ARM_SetISRCfg(int ISRIndex, OS_U32 Cfg) {
#if OS_DEBUG
  if ((unsigned int)ISRIndex >= NUM_INT_SOURCES) {
    OS_Error(OS_ERR_ISR_INDEX);
  }
#endif
  OS_DI();
  OS_GIC_SetIntCfg(ISRIndex, ((Cfg & 0x01u) << 1));
  OS_RestoreI();
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
  //
  // Initially disable watchdog
  //
  WT_CSR = 0x18u;  // Set to default value (after reset), disabled, interval timer mode
  //
  // Initialize oscillators, clock tree and PLL to run specified frequency and speed up system initialization
  //
  _InitPLL();
  //
  // Initialize memory interface for CS0
  //
  BSC_CS0BCR = 0x10000C00u;
  BSC_CS0WCR = 0x00000B40u;
  //
  // Copy code from external Flash into internal RAM if COPY_TO_SRAM is set
  //
  #if (COPY_TO_SRAM != 0)
    OS_MEMCPY((void*) 0x60100000u, (const void*)0x0u, 0x900000u);
  #endif
  //
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                         Mode                                Access permissions        VAddr  PAddr  Size[MB]
#if (COPY_TO_SRAM == 0)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x000, 0x000, 0x040);  // CS0 space, 64MB NOR Flash (16bit BUS on Eval-Board)
#else
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x000, 0x601, 0x009);  // Internal SRAM remapped to 0
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x009, 0x009, 0x037);  // CS0 space, 64MB NOR Flash (16bit BUS on Eval-Board)
#endif
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x040, 0x040, 0x040);  // CS1 space, 64MB NOR Flash (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x080, 0x080, 0x040);  // CS2 space, 64MB SDRAM (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x0C0, 0x0C0, 0x040);  // CS3 space, 64MB SDRAM (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x100, 0x100, 0x040);  // CS4 space, User area (not used on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x140, 0x140, 0x040);  // CS5 space, User area (not used on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x180, 0x180, 0x040);  // SPI multi I/O BUS space 1, Serial Flash, 64MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x1C0, 0x1C0, 0x040);  // SPI multi I/O BUS space 2, Serial Flash, 64MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_THROUGH,                               0x200, 0x600, 0x00A);  // Map the internal RAM from the MIRROR area, DON'T use the physical RAM here.
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NON_CACHEABLE,                               0x20A, 0x600, 0x00A);  // Map the internal RAM
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x214, 0x214, 0x1EA);  // Reserved area
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_EXECUTE_NEVER, 0x3FE, 0x3FE, 0x001);  // SPI multi SFRs
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x3FF, 0x3FF, 0x001);  // L2 Cache
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x400, 0x400, 0x040);  // CS0 mirror space, 64MB NOR Flash (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x440, 0x440, 0x040);  // CS1 mirror space, 64MB NOR Flash (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x480, 0x480, 0x040);  // CS2 mirror space, 64MB SDRAM (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x4C0, 0x4C0, 0x040);  // CS3 mirror space, 64MB SDRAM (16bit BUS on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x500, 0x500, 0x040);  // CS4 mirror space, User area (not used on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED,                                          0x540, 0x540, 0x040);  // CS5 mirror space, User area (not used on Eval-Board)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x580, 0x580, 0x040);  // SPI multi I/O BUS mirror space 1, Serial Flash, 64MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED,                            0x5C0, 0x5C0, 0x040);  // SPI multi I/O BUS mirror space 2, Serial Flash, 64MB
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC,                            0x600, 0x600, 0x00A);  // 10MB internal RAM mirror space, used as RAM for application
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_EXECUTE_NEVER, 0x60A, 0x60A, 0x9F6);  // SFRs or reserved area, non cacheable, non bufferable
  //
  // Set API functions and base address for L2 Cache
  //
  OS_ARM_AddL2Cache(&OS_L2CACHE_L2C310, (void*)L2CACHE_BASE_ADDR);
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  return 1;
}

/*************************** End of file ****************************/
