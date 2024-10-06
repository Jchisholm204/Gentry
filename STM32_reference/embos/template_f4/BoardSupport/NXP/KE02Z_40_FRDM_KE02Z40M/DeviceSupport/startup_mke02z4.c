//*****************************************************************************
// MKE02Z4 startup code for use with MCUXpresso IDE
//
// Version : 051217
//*****************************************************************************
//
// The Clear BSD License
// Copyright 2016-2017 NXP
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
    extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK     __attribute__ ((weak))
#define WEAK_AV  __attribute__ ((weak, section(".after_vectors")))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
// Flash Configuration block : 16-byte flash configuration field that stores
// default protection settings (loaded on reset) and security information that
// allows the MCU to restrict access to the Flash Memory module.
// Placed at address 0x400 by the linker script.
//*****************************************************************************
__attribute__ ((used,section(".FlashConfig"))) const struct {
    unsigned int word1;
    unsigned int word2;
    unsigned int word3;
    unsigned int word4;
} Flash_Config = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFEFFFF};
//*****************************************************************************
// Declaration of external SystemInit function
//*****************************************************************************
extern void SystemInit(void);

//*****************************************************************************
// Forward declaration of the core exception handlers.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below
//*****************************************************************************
WEAK void Reserved16_IRQHandler(void);
WEAK void Reserved17_IRQHandler(void);
WEAK void Reserved18_IRQHandler(void);
WEAK void Reserved19_IRQHandler(void);
WEAK void Reserved20_IRQHandler(void);
WEAK void FTMRH_IRQHandler(void);
WEAK void PMC_IRQHandler(void);
WEAK void IRQ_IRQHandler(void);
WEAK void I2C0_IRQHandler(void);
WEAK void Reserved25_IRQHandler(void);
WEAK void SPI0_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void UART0_IRQHandler(void);
WEAK void UART1_IRQHandler(void);
WEAK void UART2_IRQHandler(void);
WEAK void ADC_IRQHandler(void);
WEAK void ACMP0_IRQHandler(void);
WEAK void FTM0_IRQHandler(void);
WEAK void FTM1_IRQHandler(void);
WEAK void FTM2_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void ACMP1_IRQHandler(void);
WEAK void PIT_CH0_IRQHandler(void);
WEAK void PIT_CH1_IRQHandler(void);
WEAK void KBI0_IRQHandler(void);
WEAK void KBI1_IRQHandler(void);
WEAK void Reserved42_IRQHandler(void);
WEAK void ICS_IRQHandler(void);
WEAK void WDOG_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void Reserved16_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved17_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved18_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved19_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved20_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTMRH_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PMC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void IRQ_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved25_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACMP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT_CH0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT_CH1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void KBI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void KBI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved42_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ICS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WDOG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);

//*****************************************************************************
// External declaration for the pointer to the stack top from the Linker Script
//*****************************************************************************
extern void _vStackTop(void);
//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif

__attribute__ ((used, section(".isr_vector")))
const void* __vectors[] = {
    // Core Level - CM0P
    &_vStackTop,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The hard fault handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall handler
    0,                                 // Reserved
    0,                                 // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // Chip Level - MKE02Z4
    Reserved16_IRQHandler,   // 16: Reserved interrupt
    Reserved17_IRQHandler,   // 17: Reserved interrupt
    Reserved18_IRQHandler,   // 18: Reserved interrupt
    Reserved19_IRQHandler,   // 19: Reserved interrupt
    Reserved20_IRQHandler,   // 20: Reserved interrupt
    FTMRH_IRQHandler,        // 21: Command complete and error interrupt
    PMC_IRQHandler,          // 22: Low-voltage warning
    IRQ_IRQHandler,          // 23: External interrupt
    I2C0_IRQHandler,         // 24: Single interrupt vector for all sources
    Reserved25_IRQHandler,   // 25: Reserved interrupt
    SPI0_IRQHandler,         // 26: Single interrupt vector for all sources
    SPI1_IRQHandler,         // 27: Single interrupt vector for all sources
    UART0_IRQHandler,        // 28: Status and error
    UART1_IRQHandler,        // 29: Status and error
    UART2_IRQHandler,        // 30: Status and error
    ADC_IRQHandler,          // 31: ADC conversion complete interrupt
    ACMP0_IRQHandler,        // 32: Analog comparator 0 interrupt
    FTM0_IRQHandler,         // 33: FTM0 single interrupt vector for all sources
    FTM1_IRQHandler,         // 34: FTM1 single interrupt vector for all sources
    FTM2_IRQHandler,         // 35: FTM2 single interrupt vector for all sources
    RTC_IRQHandler,          // 36: RTC overflow
    ACMP1_IRQHandler,        // 37: Analog comparator 1 interrupt
    PIT_CH0_IRQHandler,      // 38: PIT CH0 overflow
    PIT_CH1_IRQHandler,      // 39: PIT CH1 overflow
    KBI0_IRQHandler,         // 40: Keyboard interrupt0
    KBI1_IRQHandler,         // 41: Keyboard interrupt1
    Reserved42_IRQHandler,   // 42: Reserved interrupt
    ICS_IRQHandler,          // 43: Clock loss of lock
    WDOG_IRQHandler,         // 44: Watchdog timeout
}; /* End of g_pfnVectors */

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors.init_data")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
  unsigned int *pulDest = (unsigned int*) start;
  unsigned int *pulSrc = (unsigned int*) romstart;
  unsigned int loop;
  for (loop = 0; loop < len; loop = loop + 4)
    *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors.init_bss")))
void bss_init(unsigned int start, unsigned int len) {
  unsigned int *pulDest = (unsigned int*) start;
  unsigned int loop;
  for (loop = 0; loop < len; loop = loop + 4)
    *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

static void CopyDataToRAM(void) {
  unsigned int LoadAddr, ExeAddr, SectionLen;
  unsigned int *SectionTableAddr;

  // Load base address of Global Section Table
  SectionTableAddr = &__data_section_table;
  // Copy the data sections from flash to SRAM.
  while (SectionTableAddr < &__data_section_table_end) {
    LoadAddr = *SectionTableAddr++;
    ExeAddr = *SectionTableAddr++;
    SectionLen = *SectionTableAddr++;
    data_init(LoadAddr, ExeAddr, SectionLen);
  }
  // At this point, SectionTableAddr = &__bss_section_table;
  // Zero fill the bss segment
  while (SectionTableAddr < &__bss_section_table_end) {
    ExeAddr = *SectionTableAddr++;
    SectionLen = *SectionTableAddr++;
    bss_init(ExeAddr, SectionLen);
  }
}

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((section(".after_vectors.reset")))
void ResetISR(void) {
  __asm volatile ("cpsid i");  // Disable interrupts
  SystemInit();
  CopyDataToRAM();
#if defined (__cplusplus)
    __libc_init_array();  // Call C++ library initialisation
#endif
  __asm volatile ("cpsie i");  // Reenable interrupts
#if defined (__REDLIB__)
  // Call the Redlib library, which in turn calls main()
  __main();
#else
  main();
#endif

  //
  // main() shouldn't return, but if it does, we'll just enter an infinite loop
  //
  while (1) {
    ;
  }
}

//*****************************************************************************
// Default core exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
WEAK_AV void NMI_Handler(void)
{ while(1) {}
}

WEAK_AV void HardFault_Handler(void)
{ while(1) {}
}

WEAK_AV void SVC_Handler(void)
{ while(1) {}
}

WEAK_AV void PendSV_Handler(void)
{ while(1) {}
}

WEAK_AV void SysTick_Handler(void)
{ while(1) {}
}

//*****************************************************************************
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//*****************************************************************************
WEAK_AV void IntDefaultHandler(void)
{ while(1) {}
}

//*****************************************************************************
// Default application exception handlers. Override the ones here by defining
// your own handler routines in your application code. These routines call
// driver exception handlers or IntDefaultHandler() if no driver exception
// handler is included.
//*****************************************************************************
WEAK_AV void Reserved16_IRQHandler(void)
{   Reserved16_DriverIRQHandler();
}

WEAK_AV void Reserved17_IRQHandler(void)
{   Reserved17_DriverIRQHandler();
}

WEAK_AV void Reserved18_IRQHandler(void)
{   Reserved18_DriverIRQHandler();
}

WEAK_AV void Reserved19_IRQHandler(void)
{   Reserved19_DriverIRQHandler();
}

WEAK_AV void Reserved20_IRQHandler(void)
{   Reserved20_DriverIRQHandler();
}

WEAK_AV void FTMRH_IRQHandler(void)
{   FTMRH_DriverIRQHandler();
}

WEAK_AV void PMC_IRQHandler(void)
{   PMC_DriverIRQHandler();
}

WEAK_AV void IRQ_IRQHandler(void)
{   IRQ_DriverIRQHandler();
}

WEAK_AV void I2C0_IRQHandler(void)
{   I2C0_DriverIRQHandler();
}

WEAK_AV void Reserved25_IRQHandler(void)
{   Reserved25_DriverIRQHandler();
}

WEAK_AV void SPI0_IRQHandler(void)
{   SPI0_DriverIRQHandler();
}

WEAK_AV void SPI1_IRQHandler(void)
{   SPI1_DriverIRQHandler();
}

WEAK_AV void UART0_IRQHandler(void)
{   UART0_DriverIRQHandler();
}

WEAK_AV void UART1_IRQHandler(void)
{   UART1_DriverIRQHandler();
}

WEAK_AV void UART2_IRQHandler(void)
{   UART2_DriverIRQHandler();
}

WEAK_AV void ADC_IRQHandler(void)
{   ADC_DriverIRQHandler();
}

WEAK_AV void ACMP0_IRQHandler(void)
{   ACMP0_DriverIRQHandler();
}

WEAK_AV void FTM0_IRQHandler(void)
{   FTM0_DriverIRQHandler();
}

WEAK_AV void FTM1_IRQHandler(void)
{   FTM1_DriverIRQHandler();
}

WEAK_AV void FTM2_IRQHandler(void)
{   FTM2_DriverIRQHandler();
}

WEAK_AV void RTC_IRQHandler(void)
{   RTC_DriverIRQHandler();
}

WEAK_AV void ACMP1_IRQHandler(void)
{   ACMP1_DriverIRQHandler();
}

WEAK_AV void PIT_CH0_IRQHandler(void)
{   PIT_CH0_DriverIRQHandler();
}

WEAK_AV void PIT_CH1_IRQHandler(void)
{   PIT_CH1_DriverIRQHandler();
}

WEAK_AV void KBI0_IRQHandler(void)
{   KBI0_DriverIRQHandler();
}

WEAK_AV void KBI1_IRQHandler(void)
{   KBI1_DriverIRQHandler();
}

WEAK_AV void Reserved42_IRQHandler(void)
{   Reserved42_DriverIRQHandler();
}

WEAK_AV void ICS_IRQHandler(void)
{   ICS_DriverIRQHandler();
}

WEAK_AV void WDOG_IRQHandler(void)
{   WDOG_DriverIRQHandler();
}

//*****************************************************************************

