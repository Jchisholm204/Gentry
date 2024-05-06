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
File    : RTOSInit_XC7Z007S.c
Purpose : Initializes and handles the hardware for embOS

Literature:
  [1]  Generic Interrupt Controller (GIC-390) Errata Notice
       (\\fileserver\Techinfo\Company\ARM\GIC_GenericInterruptController\GIC_390_Errata_Notice_v6.pdf)
  [2]  Zynq-7000 All Programmable SoC Technical Reference Manual
       (\\fileserver\Techinfo\Company\Xilinx\Zynq_7000\ug585-Zynq-7000-TRM-v1.10.pdf)
*/

#include "RTOS.h"
#include "SEGGER_SYSVIEW.h"
#include "Zynq7007S.h"

#if (defined(__ICCARM__))
  #include <intrinsics.h>
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define MEM_ADDR_QSPI_START  (0xFC010000u)

/*********************************************************************
*
*       System tick settings
*/
#define OS_TIMER_FREQ    (600000000u / 2)
#define OS_TICK_FREQ     (1000u)
#define OS_INT_FREQ      (OS_TICK_FREQ)
#define OS_TIMER_RELOAD  ((OS_TIMER_FREQ / OS_TICK_FREQ) - 1u)

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
// Private timer
//
#define SCU_BASE_ADDR              (0xF8F00000u)
#define CORE_PRIV_TIMER_BASE_ADDR  (SCU_BASE_ADDR + 0x0600u)
#define CORE_PRIV_TIMER_LOAD       (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x00u))
#define CORE_PRIV_TIMER_COUNT      (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x04u))
#define CORE_PRIV_TIMER_CONTROL    (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x08u))
#define CORE_PRIV_TIMER_STATUS     (*(volatile OS_U32*)(CORE_PRIV_TIMER_BASE_ADDR + 0x0Cu))
#define CORE_PRIV_TIMER_INT_ID     (29u)

//
// Interrupt controller GIC
//
#define GIC_BASE_ADDR              (0xF8F00000u)
#define GICD_BASE_ADDR             (GIC_BASE_ADDR + 0x1000u)
#define GICC_BASE_ADDR             (GIC_BASE_ADDR + 0x0100u)
#define GICC_HPPIR                 (*(volatile OS_U32*)(GICC_BASE_ADDR + 0x018u))  // Highest Priority Pending Interrupt Register
#define GICD_IPRIORITY             (*(volatile OS_U32*)(GICD_BASE_ADDR + 0x400u))  // Interrupt Priority Registers

#define NUM_INT_SOURCES            (96u)

//
// L2 cache
//
#define L2CACHE_BASE_ADDR          (0xF8F02000u)

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
extern const OS_ARM_L2CACHE_API OS_L2CACHE_PL310;
extern       unsigned int       __vector;  // Start of vector table, defined in startup file

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
__attribute__ ((section (".init._InitPLLs")))
static void _InitPLLs(void) {
  OS_U32 v;

  //
  // Zynq-7000S maximum frequency: 766 MHz
  //
  // The Zynq-7000S provides 3 PLLs:
  // ARM PLL => Can be used to feed the CPU and some peripherals
  // DDR PLL => Exclusively for DDR RAM
  // I/O PLL => Mainly used for I/O peripherals (can be used but does not have to...)
  // Clock tree:
  //   PS_CLK => ARM / DDR / IO PLL (ARM_CLK_CTRL[SRCSEL]) => 6-bit div (ARM_CLK_CTRL[DIVISOR])           => CPU_1x, CPU_2x, CPU_3x2x, CPU_6x4x
  //   PS_CLK => DDR PLL (DDR_PLL_CTRL)                    => 6-bit div (DDR_CLK_CTRL[DDR_3XCLK_DIVISOR]) => DDR_3x
  //   PS_CLK => DDR PLL (DDR_PLL_CTRL)                    => 6-bit div (DDR_CLK_CTRL[DDR_2XCLK_DIVISOR]) => DDR_2x
  //   PS_CLK => IO PLL  (IO_PLL_CTRL)                                                                    => I/O peripherals
  //
  // Explanation:
  // CPU_1x   == PLLOut / Div * 1/6
  // CPU_2x   == (PLLOut / Div) * 2/6
  // CPU_3x2x == (PLLOut / Div) * 2/6   or   (PLLOut / Div) * 3/6 in case CLK_621_TRUE[0] == 1
  // CPU_6x4x == (PLLOut / Div) * 4/6   or   (PLLOut / Div) * 6/6 in case CLK_621_TRUE[0] == 1
  //
  // Configuration for the emPower Zynq:
  //   PS_CLK: 33.3 MHz (external oscillator)
  //   CLK_621_TRUE: 1 (Using 6:3:2:1)
  //   ARM PLLOut: 1200 MHz
  //
  REG_SLCR_UNLOCK = SLCR_UNLOCK_KEY;
  //
  // Init ARM PLL
  // We can change the register values with no risk as they only become active on a PLL reset
  // Small code exception here:
  // To make porting to macro file easier, we always write the value into temp variable v first
  //
  //
  // Values for ARM_PLL_CFG taken from the "PLL Frequency Control Settings" table 25-6 of the
  // Zynq 7000 Technical Reference Manual (TRM)
  //
  v = 0
    | (0x02  <<  4)                    // PLL_RES
    | (0x02  <<  8)                    // PLL_CP
    | (0x113 << 12)                    // LOCK_CNT (for 1.2 GHz)
    ;
  REG_SLCR_ARM_PLL_CFG = v;
  v = 0
    | (   0 <<  1)                    // PLL_PWRDWN: 0 == PLL powered up
    | (   0 <<  3)                    // PLL_BYPASS_QUAL: 0 == Bypass is controlled by PLL_BYPASS_FORCE
    | (   1 <<  4)                    // PLL_BYPASS_FORCE: 1 == Bypass PLL
    | (0x24 << 12)                    // PLL_FDIV: Multiplier for PLL
    ;
  REG_SLCR_ARM_PLL_CTRL = v;
  v = 0                               // Set dividers before PLL gets active so we never get out of spec. with the clocks
    | (0x0  <<  4)                    // SRCSEL (different to other clock control registers!!!): 0 == ARM PLL, 2 == DDR PLL, 3 == IO PLL
    | (0x2  <<  8)                    // DIVISOR
    | (0x1  << 24)                    // CPU_6OR4XCLKACT: CPU_6x4x domain active
    | (0x1  << 25)                    // CPU_3OR2XCLKACT: CPU_3x2x domain active
    | (0x1  << 26)                    // CPU_2XCLKACT:    CPU_2x domain active
    | (0x1  << 27)                    // CPU_1XCLKACT:    CPU_1x domain active
    | (0x1  << 28)                    // CPU_PERI_CLKACT:
    ;
  REG_SLCR_ARM_CLK_CTRL = v;
  v  = REG_SLCR_ARM_PLL_CTRL;
  v |=  (1u << 0);                    // PLL_RESET => Assert reset, new PLL values become active
  REG_SLCR_ARM_PLL_CTRL = v;
  v  = REG_SLCR_ARM_PLL_CTRL;
  v &= ~(1u << 0);                    // PLL_RESET: De-assert
  REG_SLCR_ARM_PLL_CTRL = v;
  do {
    v = REG_SLCR_PLL_STATUS;
  } while ((v & (1u << 0)) == 0);     // Wait until locked
  v  = REG_SLCR_ARM_PLL_CTRL;
  v &= ~(1u << 4);                    // PLL_BYPASS_FORCE: 0 == Remove bypass and switch to PLL
  REG_SLCR_ARM_PLL_CTRL = v;
  //
  // Make sure that we enable CPU_6x and CPU_3x (See clock tree explanation above)
  //
  v  = REG_SLCR_CLK_621_TRUE;
  v |= (1u << 0);                     // CLK_621_TRE CLK_621_TRUE (CPU Clock Ratio Mode Select)
  REG_SLCR_CLK_621_TRUE = v;
  //
  // Init DDR PLL (unused -> power down)
  //
  REG_SLCR_DDR_PLL_CTRL = (1u << 1);  // Power down
  //
  // Init IO_PLL
  //
  v = 0
    | (0x00cu <<  4)                  // PLL_RES
    | (0x002u <<  8)                  // PLL_CP
    | (0x145u << 12)                  // LOCK_CNT
    ;
  REG_SLCR_IO_PLL_CFG = v;
  v = 0
    | (0x00u <<  1)                   // PLL_PWRDWN: 0 == PLL powered up
    | (0x00u <<  3)                   // PLL_BYPASS_QUAL: 0 == Bypass is controlled by PLL_BYPASS_FORCE
    | (0x01u <<  4)                   // PLL_BYPASS_FORCE: 1 == Bypass PLL
    | (0x1Eu << 12)                   // PLL_FDIV: Multiplier for PLL
    ;
  REG_SLCR_IO_PLL_CTRL = v;
  v  = REG_SLCR_IO_PLL_CTRL;
  v |=  (1u << 0);                    // PLL_RESET => Assert reset, new PLL values become active
  REG_SLCR_IO_PLL_CTRL = v;
  v  = REG_SLCR_IO_PLL_CTRL;
  v &= ~(1u << 0);                    // PLL_RESET: De-assert
  REG_SLCR_IO_PLL_CTRL = v;
  do {
    v = REG_SLCR_PLL_STATUS;
  } while ((v & (1u << 2)) == 0);     // Wait until locked
  v  = REG_SLCR_IO_PLL_CTRL;
  v &= ~(1u << 4);                    // PLL_BYPASS_FORCE: 0 == Remove bypass and switch to PLL
  REG_SLCR_IO_PLL_CTRL = v;
  //
  // Init clock sources for FPGA / programmable logic
  //
  v = 0
    | (0x01u << 0)                    // CLKACT
    | (0x00u << 4)                    // SRCSEL
    | (0x05u << 8)                    // DIVISOR
    ;
  REG_SLCR_PCAP_CLK_CTRL = v;
  v = 0                               // FPGA0 => 200 MHz
    | (0x00u <<  4)                   // SRCSEL
    | (0x05u <<  8)                   // DIVISOR0
    | (0x01u << 20)                   // DIVISOR1
    ;
  REG_SLCR_FPGA0_CLK_CTRL = v;
  v = 0                               // FPGA1 => 1 MHz
    | (0x00u <<  4)                   // SRCSEL
    | (0x28u <<  8)                   // DIVISOR0
    | (0x19u << 20)                   // DIVISOR1
    ;
  REG_SLCR_FPGA1_CLK_CTRL = v;
  v = 0                               // FPGA2 => 1 MHz
    | (0x00u <<  4)                   // SRCSEL
    | (0x28u <<  8)                   // DIVISOR0
    | (0x19u << 20)                   // DIVISOR1
    ;
  REG_SLCR_FPGA2_CLK_CTRL = v;
  v = 0                               // FPGA3 => 1 MHz
    | (0x00u <<  4)                   // SRCSEL
    | (0x28u <<  8)                   // DIVISOR0
    | (0x19u << 20)                   // DIVISOR1
    ;
  REG_SLCR_FPGA3_CLK_CTRL = v;
  //
  // Lock access to system registers
  //
  REG_SLCR_LOCK = SLCR_LOCK_KEY;
}

/*********************************************************************
*
*       _InitQSPI()
*
*  Function description
*    Initialize QSPI controller
*
*  Notes
*    (1) This function may be called from __low_level_init() and therefore
*        must not use or call any function which relies on any variable,
*        because variables are not initialized before __low_level_init()
*        is called !
*/
__attribute__ ((section (".init._InitQSPI")))
static void _InitQSPI(void) {
  REG_SLCR_UNLOCK         = SLCR_UNLOCK_KEY;
  REG_SLCR_APER_CLK_CTRL |= (1u << 23);  // LQSPI_CPU_1XCLKACT: 1 == Enable clock for QSPI controller
  REG_SLCR_LQSPI_CLK_CTRL = 0
                          | (1u <<  0)   // CLKACT: 1 == Clock enabled
                          | (0u <<  4)   // SRCSEL: 0 == IO_PLL, 2 == ARM_PLL, 3 == DDR_PLL
                          | (5u <<  8)   // DIVISOR: Controller clock == SRCSEL / DIVISOR
                          ;
  //
  // Init PIOs needed for QSPI operation after controller init is done
  //
  REG_SLCR_APER_CLK_CTRL |= (1u << 22);  // GPIO_CPU_1XCLKACT: 1 == Enable for GPIO controller
  REG_SLCR_MIO_PIN_01     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_01: QSPI_#CS
  REG_SLCR_MIO_PIN_02     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_02: QSPI_IO0
  REG_SLCR_MIO_PIN_03     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_03: QSPI_IO1
  REG_SLCR_MIO_PIN_04     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_04: QSPI_IO2
  REG_SLCR_MIO_PIN_05     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_05: QSPI_IO3
  REG_SLCR_MIO_PIN_06     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_06: QSPI_CLK
  REG_SLCR_MIO_PIN_08     = MIO_PIN_OPT_IO_CMOS_3V3 | MIO_PIN_OPT_ENA_SPEED_FAST | MIO_PIN_OPT_LVL01_MUX;  // MIO_PIN_08: QSPI_FBCLK
  //
  // Init QSPI for memory mapped mode
  // We do not directly execute from flash but copy all stuff at boot time, so flash speed is only partially important
  // We only use single pin SPI mode, so we set SPI speed to 50 MHz so we can use the basic read commands that does not need dummy cycles (which may differ from flash to flash / vendor)
  //
  REG_QSPIC_CONFIG_REG = 0
                       | (1u << 31)   // leg_flsh: 1 == Memory mapped or I/O mode (not legacy SPI)
                       | (0u << 26)   // endian: 0 == little endian access when reading/writing from/to Rx/Tx regs
                       | (1u << 19)   // Holdb_dr: 1 == Holdb and WPn pins are actively driven by the qspi controller in 1-bit and 2-bit modes
                       | (1u << 17)   // Reserved, must be 1
                       | (0u << 15)   // Man_start_en: 0 == Auto mode, data + command is send as soon as Tx regs are accessed
                       | (0u << 14)   // Manual_CS: 0 == Auto mode
                       | (0u << 10)   // PCS: Controls state of nCS in case Manual_CS == 1
                       | (0u <<  8)   // REF_CLK: Must be 0
                       | (3u <<  6)   // FIFO_WIDTH: Must be 3 (32-bit)
                       | (0u <<  3)   // BAUD_RATE_DIV: Baudrate = 2 ^ (BAUD_RATE_DIV + 1) => I/O PLL (200 MHz) is used as source clock for QSPI baudrate generation
                       | (0u <<  2)   // CLK_PH: SCK is active between transfers
                       | (0u <<  1)   // CLK_POL: 1 == SCK is HIGH between transfers
                       | (1u <<  0)   // MODE_SEL: 1 == SPI is in master mode
                       ;
  //
  // Configure and enable linear QSPI mode (memory mapped mode)
  // Need to take care of the following before entering linear QSPI mode:
  // TXFIFO and RXFIFO are empty
  // REG_QSPIC_CR[Manual_CS] and REG_QSPIC_CR[PCS] must be 0
  //
  REG_QSPIC_LQSPI_CFG = 0
                     | (1u    << 31)  // LQ_MODE: 1 == Linear quad SPI mode
                     | (0u    << 30)  // TWO_MEM: 1 == Lower and upper memories are active
                     | (0u    << 29)  // SEP_BUS: 1 == Separate memory bus for upper and lower memories
                     | (0u    << 28)  // U_PAGE: 1 == Select upper memory page as visible. Don't care if LQ_MODE == 0 AND TWO_MEM == 1 AND SEP_BUS == 0
                     | (1u    << 26)  // Reserved. Must be 1
                     | (0u    << 25)  // MODE_EN: Enable MODE_BITS[23:16] to be sent. Must be set for dual/quad mode. Must be clear for all other modes
                     | (0u    << 24)  // MODE_ON: 1 ==  instruction code is only sent for the very first read transfer
                     | (0u    << 16)  // MODE_BITS: If bit 25 is set, this value is required for both dual I/O read and quad I/O read. See vendor's datasheet for more information.
                     | (1u    <<  8)  // DUMMY_BYTE: Number of dummy bytes between address and return read data
                     | (0x3Bu <<  0)  // INST_CODE: Read instruction code (0x03: Read, 0xEB: Fast read quad I/O)
                     ;
  REG_QSPIC_TX_THRES_REG = 1u;         // Report Tx-FIFO full as soon as min. 1 byte is in the FIFO
  REG_QSPIC_RX_THRES_REG = 1u;        // Report Rx-FIFO full as soon as min. 1 byte is in the FIFO
  REG_QSPIC_LPBK_DLY_ADJ = 0
                         | (1u << 5)  // USE_LPBK: 1 == Use internal loopback master clock for read data capturing when baud rate divisor (reg 0x00) is 2
                         | (0u << 3)  // DLY1: Must be set to 0 if Loopback clk used
                         | (0u << 0)  // DLY0: Must be set to 0 if Loopback clk used
                         ;
  REG_QSPIC_EN_REG = 1u;              // SPI_EN: Enable SPI
  REG_SLCR_LOCK    = SLCR_LOCK_KEY;
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
  return CORE_PRIV_TIMER_COUNT;
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
  return CORE_PRIV_TIMER_STATUS & (1u << 0);
}

/*********************************************************************
*
*       _OS_ISR_Tick()
*
*  Function description
*    This is the hardware timer exception handler.
*/
static void _OS_ISR_Tick(void) {
  CORE_PRIV_TIMER_STATUS = (1u << 0);  // Clear timer interrupt pending flag
  OS_TICK_Handle();
#if ((defined(__ICCARM__) && (OS_DEBUG != 0)) || (OS_VIEW_IFSELECT == OS_VIEW_IF_JLINK))
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
  OS_ARM_InstallISRHandler(CORE_PRIV_TIMER_INT_ID, _OS_ISR_Tick);
  OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, 0xFFu);              // Set to lowest level, ALL BITS set
  TickPrio  = OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, 0xFFu);  // Read back priority to examine relevant preemption-level-bits
  TickPrio -= 1;                                                 // Set to low preemption level, 1 level higher than lowest
  OS_ARM_ISRSetPrio(CORE_PRIV_TIMER_INT_ID, TickPrio);
  OS_ARM_EnableISR(CORE_PRIV_TIMER_INT_ID);
  CORE_PRIV_TIMER_CONTROL = 0x00u;            // Stop Timer, set Prescaler to 1
  CORE_PRIV_TIMER_LOAD    = OS_TIMER_RELOAD;  // Set timer interval
  CORE_PRIV_TIMER_CONTROL = (1u << 0)         // Enable Timer
                          | (1u << 1)         // Set auto reload mode
                          | (1u << 2);        // Enable timer interrupt
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
      //
      // When uncommenting this line, please be aware device
      // specific issues could occur.
      // Therefore, we do not call __WFI() by default.
      //
      //__asm volatile("wfi");        // Switch CPU into sleep mode
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
  //
  // Save the interrupt id for SEGGER_SYSVIEW_X_GetInterruptId()
  //
#if (OS_SUPPORT_TRACE_API != 0)
  SEGGER_SYSVIEW_InterruptId = int_id;
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
  void (*pf)(void);

  REG_SLCR_UNLOCK  = SLCR_UNLOCK_KEY;
  REG_SLCR_OCM_CFG = (1u << 4);  // [2] seems to be set and must kept set it seems. Doc does not say anything about this bit...
  REG_SLCR_LOCK    = SLCR_LOCK_KEY;
  //
  // Initialize PLL, DDR RAM and QSPI flash
  //
  // Initialize PLL and QSPI flash
  //
  // The device boots from QSPI flash. The QSPI flash contains a header called HW_BootHeader.
  // HW_BootHeader tells the Zynq BootROM where the FSBL is located in flash (HW_BootHeader.SourceOffset),
  // how big the FSBL is (HW_BootHeader.LengthOfImage) and where the execution entry point within the FSBL
  // is (HW_BootHeader.StartOfExecution). The Zynq BootROM copies the FSBL from QSPI into the OCM on boot.
  // The BootROM can transfer up to 192KB into OCM, which limits the size of the FSBL.
  //
  // This BSP is set up to let the BootROM copy 64 KB from QSPI flash (0xFC010000-0xFC020000) into OCM
  // (0x00000000) and let the FSBL start executing from 0x00000000. This address contains the vector table
  // (which was copied from flash) with a branch into the Reset_Handler located in flash. This means that
  // the FSBL executes the code from flash. However, changing QSPI clock and QSPI settings while executing
  // from QSPI flash is not possible.
  //
  // To get around this, we do the following:
  // We make sure that the _InitPLLs() and _InitQSPI() is in this 64 KB that are copied (made sure by linking
  // those function into .init.* section, and the linker file linking .init.* directly after vectors).
  // When calling this function, we are not calling it directly but via function pointer instead.
  // This way, we can make sure that we are calling the function copy that is in OCM and execute it from there.
  // It will initialize the PLL and re-inits the QSPI, so that it can safely return to QSPI from the function copy.
  //
  pf = (void (*)(void))((OS_U32)_InitPLLs - MEM_ADDR_QSPI_START);
  pf();
  pf = (void (*)(void))((OS_U32)_InitQSPI - MEM_ADDR_QSPI_START);
  pf();
  //
  //
  // Init MMU and caches. This defines the virtual memory map, which is used during execution.
  // Code below fills all 4096 entries in ascending VAddr order.
  //
  OS_ARM_MMU_InitTT(_TranslationTable);
  //                                         Mode                                Permissions            Execute Never             VAddr  PAddr  Size[MB]
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC | OS_ARM_MMU_READWRITE,                            0x000, 0x000, 0x001);  // 256 KB iRAM, needs to be write back allocate on Zynq since iRAM is very slow (23 cycles)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_READWRITE | OS_ARM_MMU_EXECUTE_NEVER, 0x001, 0x001, 0xF9F);  // SFRs
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED               | OS_ARM_MMU_NOACCESS  | OS_ARM_MMU_EXECUTE_NEVER, 0xFA0, 0xFA0, 0x010);  // Unused
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_STRONGLY_ORDERED | OS_ARM_MMU_READWRITE,                            0xFB0, 0x000, 0x001);  // Mapped to 256 KB iRAM @ address 0x0. Strongly-ordered memory. Necessary for IP stack.
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_NON_CACHEABLE    | OS_ARM_MMU_READWRITE,                            0xFB1, 0x000, 0x001);  // Mapped to 256 KB iRAM @ address 0x0. Non-cacheable Normal memory. Necessary for RTT.
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED               | OS_ARM_MMU_NOACCESS  | OS_ARM_MMU_EXECUTE_NEVER, 0xFB2, 0xFB2, 0x00E);  // Unused
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_CACHEMODE_WRITE_BACK_ALLOC | OS_ARM_MMU_READONLY,                             0xFC0, 0xFC0, 0x010);  // 16 MB QSPI flash, QSPI is write back allocate. Seems it needs to be write back alloc to make read caching work correctly. Using write through it seems that L2 lockdown does not have any effect (continuous accesses to QSPI can be seen...)
  OS_ARM_MMU_AddTTEntries(_TranslationTable, OS_ARM_MMU_UNMAPPED               | OS_ARM_MMU_NOACCESS  | OS_ARM_MMU_EXECUTE_NEVER, 0xFD0, 0xFD0, 0x030);  // Unused
  //
  // Set API functions and base address for L2 Cache
  //
  OS_ARM_AddL2Cache(&OS_L2CACHE_XilinxZynq7000, (void*)L2CACHE_BASE_ADDR);
  //
  // Now enable MMU, Caches and branch prediction unit
  //
  OS_ARM_MMU_Enable(_TranslationTable);
  OS_ARM_ICACHE_Enable();
  OS_ARM_DCACHE_Enable();
  return 1;
}

/*************************** End of file ****************************/
