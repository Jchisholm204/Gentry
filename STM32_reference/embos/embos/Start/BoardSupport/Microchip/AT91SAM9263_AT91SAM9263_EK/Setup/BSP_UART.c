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
Purpose : UART implementation

Additional information:

  Device : AT91SAM9263
  Board  : AT91SAM9263 EK

  Unit | UART   | Board connector
  ===============================
  0    | USART0 | RS232 COM PORT
  1    | USART1 | -
  2    | USART2 | -

*/

#include "BSP_UART.h"
#include "RTOS.h"      // For OS_ARM_* and OS_INT_* functions.

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SYSTEM_FREQ           (200049178uL)
#define USART_FREQ            (SYSTEM_FREQ / 2uL)

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
//
// Power management controller (PMC)
//
#define PMC_BASE_ADDR         (0xFFFFFC00uL)
#define PMC_PCER              (*(volatile unsigned long*)(PMC_BASE_ADDR + 0x10uL))
//
// Advanced interrupt controller (AIC)
//
#define AIC_BASE_ADDR         (0xFFFFF000uL)
#define AIC_ICCR              (*(volatile unsigned long*)(AIC_BASE_ADDR + 0x128uL))

#define PIOA_ID               (2uL)
#define PIOD_ID               (4uL)
#define USART0_ID             (7uL)
#define USART1_ID             (8uL)
#define USART2_ID             (9uL)
//
// USART0
//
#define USART0_BASE_ADDR      (0xFFF8C000uL)
#define US0_CR                (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x000uL))
#define US0_MR                (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x004uL))
#define US0_IER               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x008uL))
#define US0_IDR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x00CuL))
#define US0_IMR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x010uL))
#define US0_CSR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x014uL))
#define US0_RHR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x018uL))
#define US0_THR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x01CuL))
#define US0_BRGR              (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x020uL))
#define US0_RTOR              (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x024uL))
#define US0_TTGR              (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x028uL))
#define US0_TPR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x108uL))
#define US0_TCR               (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x10cuL))
#define US0_PTCR              (*(volatile unsigned long*)(USART0_BASE_ADDR + 0x120uL))
//
// USART1
//
#define USART1_BASE_ADDR      (0xFFF90000uL)
#define US1_CR                (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x000uL))
#define US1_MR                (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x004uL))
#define US1_IER               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x008uL))
#define US1_IDR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x00CuL))
#define US1_IMR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x010uL))
#define US1_CSR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x014uL))
#define US1_RHR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x018uL))
#define US1_THR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x01CuL))
#define US1_BRGR              (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x020uL))
#define US1_RTOR              (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x024uL))
#define US1_TTGR              (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x028uL))
#define US1_TPR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x108uL))
#define US1_TCR               (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x10cuL))
#define US1_PTCR              (*(volatile unsigned long*)(USART1_BASE_ADDR + 0x120uL))
//
// USART2
//
#define USART2_BASE_ADDR      (0xFFF94000uL)
#define US2_CR                (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x000uL))
#define US2_MR                (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x004uL))
#define US2_IER               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x008uL))
#define US2_IDR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x00CuL))
#define US2_IMR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x010uL))
#define US2_CSR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x014uL))
#define US2_RHR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x018uL))
#define US2_THR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x01CuL))
#define US2_BRGR              (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x020uL))
#define US2_RTOR              (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x024uL))
#define US2_TTGR              (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x028uL))
#define US2_TPR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x108uL))
#define US2_TCR               (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x10cuL))
#define US2_PTCR              (*(volatile unsigned long*)(USART2_BASE_ADDR + 0x120uL))

#define USART_RXRDY           (1uL << 0)  // Rx status flag
#define USART_TXRDY           (1uL << 1)  // Tx RDY Status flag
#define USART_TXEMPTY         (1uL << 9)  // Tx EMPTY Status flag
#define USART_ENDTX           (1uL << 4)  // Tx end flag
#define USART_RX_ERROR_FLAGS  (0xE0uL)    // Parity, framing, overrun error

#define USART_IRQ_PRIO        (0x00uL)    // Lowest priority for UART interrupts

#define USART0                (0uL)
#define USART1                (1uL)
#define USART2                (2uL)

//
// PIOA control register
//
#define PIOA_BASE_ADDR        (0xFFFFF200uL)
#define PIOA_PDR              (*(volatile unsigned long*)(PIOA_BASE_ADDR + 0x04uL))
#define PIOA_ASR              (*(volatile unsigned long*)(PIOA_BASE_ADDR + 0x70uL))
#define PIOA_BSR              (*(volatile unsigned long*)(PIOA_BASE_ADDR + 0x74uL))
//
// PIOD control register
//
#define PIOD_BASE_ADDR        (0xFFFFF800uL)
#define PIOD_PDR              (*(volatile unsigned long*)(PIOD_BASE_ADDR + 0x04uL))
#define PIOD_ASR              (*(volatile unsigned long*)(PIOD_BASE_ADDR + 0x70uL))
#define PIOD_BSR              (*(volatile unsigned long*)(PIOD_BASE_ADDR + 0x74uL))

#define PIO_PDR_USART0        ((1uL << 26) | (1uL << 27))
#define PIO_PDR_USART1        ((1uL <<  1) | (1uL <<  0))
#define PIO_PDR_USART2        ((1uL <<  2) | (1uL <<  3))

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static BSP_UART_TX_CB* _pfWriteCB;
static BSP_UART_RX_CB* _pfReadCB;

/*********************************************************************
*
*       Prototypes
*
*  Declare ISR handler here to avoid "no prototype" warning.
*  They are not declared in any CMSIS header.
*
**********************************************************************
*/

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

static void USART0_RX_TX_IRQHandler(void);
static void USART1_RX_TX_IRQHandler(void);
static void USART2_RX_TX_IRQHandler(void);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _SetBaudrate()
*
*  Function description
*    Configures the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
static void _SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
}

/*********************************************************************
*
*       Global functions, IRQ handler
*
**********************************************************************
*/

/*********************************************************************
*
*       USART0_RX_TX_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*/
static void USART0_RX_TX_IRQHandler(void) {
  unsigned int Status;
  unsigned int Unit;
  unsigned int Data;

  Unit   = USART0;
  Status = US0_CSR;                         // Examine status register
  do {
    if (Status & USART_RXRDY) {             // Data received?
      Data = US0_RHR;
      if (Status & USART_RX_ERROR_FLAGS) {  // Any error ?
        US0_CR = (1uL << 8);                // RSTSTA: Reset Status Bits PARE, FRAME, OVRE and RXBRK
      } else {
        if (_pfReadCB != 0) {
          _pfReadCB(Unit, Data);            // Forward the received byte to the callback.
        }
      }
    }
    if (Status & US0_IMR & USART_TXRDY) {   // Check Tx status => Send next character
      if ((_pfWriteCB != 0) && (_pfWriteCB(Unit) != 0)) {
        US0_IDR = USART_TXRDY;              // Disable further tx interrupts
      }
    }
    Status = US0_CSR;                       // Examine status register
  } while (Status & US0_IMR & (USART_TXRDY | USART_RXRDY));
}

/*********************************************************************
*
*       USART1_RX_TX_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*/
static void USART1_RX_TX_IRQHandler(void) {
  unsigned int Status;
  unsigned int Unit;
  unsigned int Data;

  Unit   = USART1;
  Status = US1_CSR;                         // Examine status register
  do {
    if (Status & USART_RXRDY) {             // Data received?
      Data = US1_RHR;
      if (Status & USART_RX_ERROR_FLAGS) {  // Any error ?
        US1_CR = (1uL << 8);                // RSTSTA: Reset Status Bits PARE, FRAME, OVRE and RXBRK
      } else {
        if (_pfReadCB != 0) {
          _pfReadCB(Unit, Data);            // Forward the received byte to the callback.
        }
      }
    }
    if (Status & US1_IMR & USART_TXRDY) {   // Check Tx status => Send next character
      if ((_pfWriteCB != 0) && (_pfWriteCB(Unit) != 0)) {
        US1_IDR = USART_TXRDY;              // Disable further tx interrupts
      }
    }
    Status = US1_CSR;                       // Examine status register
  } while (Status & US1_IMR & (USART_TXRDY | USART_RXRDY));
}

/*********************************************************************
*
*       USART2_RX_TX_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*/
static void USART2_RX_TX_IRQHandler(void) {
  unsigned int Status;
  unsigned int Unit;
  unsigned int Data;

  Unit   = USART2;
  Status = US2_CSR;                         // Examine status register
  do {
    if (Status & USART_RXRDY) {             // Data received?
      Data = US2_RHR;
      if (Status & USART_RX_ERROR_FLAGS) {  // Any error ?
        US2_CR = (1uL << 8);                // RSTSTA: Reset Status Bits PARE, FRAME, OVRE and RXBRK
      } else {
        if (_pfReadCB != 0) {
          _pfReadCB(Unit, Data);            // Forward the received byte to the callback.
        }
      }
    }
    if (Status & US2_IMR & USART_TXRDY) {   // Check Tx status => Send next character
      if ((_pfWriteCB != 0) && (_pfWriteCB(Unit) != 0)) {
        US2_IDR = USART_TXRDY;              // Disable further tx interrupts
      }
    }
    Status = US2_CSR;                       // Examine status register
  } while (Status & US2_IMR & (USART_TXRDY | USART_RXRDY));
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_UART_Init()
*
*  Function description
*    Initializes the UART peripheral.
*
*  Parameters
*    Unit       : Unit number (typically zero-based).
*    Baudrate   : Baud rate to configure [Hz].
*    NumDataBits: Number of data bits to use.
*    Parity     : One of the following values:
*                   * BSP_UART_PARITY_NONE
*                   * BSP_UART_PARITY_ODD
*                   * BSP_UART_PARITY_EVEN
*    NumStopBits: Number of stop bits to use.
*/
void BSP_UART_Init(unsigned int Unit, unsigned long Baudrate, unsigned char NumDataBits, unsigned char Parity, unsigned char NumStopBits) {
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  OS_INT_IncDI();
  switch (Unit) {
  case USART0:
    PMC_PCER = (1uL << USART0_ID)       // Enable peripheral clock for selected USART
             | (1uL << PIOA_ID);        // Enable the PIO port where UART pins are located
    PIOA_PDR = PIO_PDR_USART0;          // Enable peripheral output signals (disable PIO Port)
    PIOA_ASR = PIO_PDR_USART0;          // Select peripheral function A ob according to selected USART
    PIOA_BSR = 0;                       // De-select "B" peripherals on PIO for USART
    US0_IDR  = 0xFFFFFFFFuL;            // Initially disable all USART interrupts
    US0_CR   = (1uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (1uL <<  3);             // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
    US0_CR   = (0uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (0uL <<  3)              // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
             | (1uL <<  4)              // RXEN:   Receiver Enable: 1 = The receiver is enabled if RXDIS is 0.
             | (0uL <<  5)              // RXDIS:  Receiver Disable: 0 = Noeffect.
             | (1uL <<  6)              // TXEN:   Transmitter Enable: 1 = The transmitter is enabled if TXDIS is 0.
             | (0uL <<  7)              // TXDIS:  Transmitter Disable: 0 = Noeffect.
             | (1uL <<  8)              // RSTSTA: Reset Status Bits: 1 = Resets the status bits PARE, FRAME, OVRE and RXBRK in the US_CSR.
             | (0uL <<  9)              // STTBRK: Start Break: 0 = Noeffect.
             | (0uL << 10)              // STPBRK: Stop Break: 0 = Noeffect.
             | (0uL << 11)              // STTTO:  Start Time-out: 0 = Noeffect.
             | (0uL << 12);             // SENDA:  Send Address: 0 = Noeffect.
    US0_MR   = (0uL <<  4)              // USCLKS: Clock Selection: 0 = MCK
             | (3uL <<  6)              // CHRL:   Character Length: 3 = Eight bits
             | (0uL <<  8)              // SYNC:   Synchronous Mode Select: 0 = USART operates in Asynchronous Mode.
             | (4uL <<  9)              // PAR:    Parity Type: 0x4 = No parity
             | (0uL << 12)              // NBSTOP: Number of Stop Bits: 0 = 1 stop bit
             | (0uL << 14)              // CHMODE: Channel Mode: 0 = Normal mode
             | (0uL << 17)              // MODE9:  9-bit Character Length: 0 = CHRL defines character length.
             | (0uL << 18);             // CKLO:   Clock Output Select: 0 = The USART does not drive the SCK pin.
    US0_BRGR = (USART_FREQ / (Baudrate * 16));
    US0_IER  = (1uL <<  0);             // Enable Rx Interrupt
    //
    // Setup interrupt controller for USART
    //
    OS_ARM_InstallISRHandler(USART0_ID, &USART0_RX_TX_IRQHandler);  // OS UART interrupt handler vector
    OS_ARM_ISRSetPrio(USART0_ID, USART_IRQ_PRIO);                   // Level sensitive, selected priority.
    AIC_ICCR = (1uL << USART0_ID);                                  // Clear OS usart interrupt.
    OS_ARM_EnableISR(USART0_ID);                                    // Enable OS usart interrupts
    break;
  case USART1:
    PMC_PCER = (1uL << USART1_ID)       // Enable peripheral clock for selected USART
             | (1uL << PIOD_ID);        // Enable the PIO port where UART pins are located
    PIOD_PDR = PIO_PDR_USART1;          // Enable peripheral output signals (disable PIO Port)
    PIOD_ASR = PIO_PDR_USART1;          // Select peripheral function A ob according to selected USART
    PIOD_BSR = 0;                       // De-select "B" peripherals on PIO for USART
    US1_IDR  = 0xFFFFFFFFuL;            // Initially disable all USART interrupts
    US1_CR   = (1uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (1uL <<  3);             // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
    US1_CR   = (0uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (0uL <<  3)              // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
             | (1uL <<  4)              // RXEN:   Receiver Enable: 1 = The receiver is enabled if RXDIS is 0.
             | (0uL <<  5)              // RXDIS:  Receiver Disable: 0 = Noeffect.
             | (1uL <<  6)              // TXEN:   Transmitter Enable: 1 = The transmitter is enabled if TXDIS is 0.
             | (0uL <<  7)              // TXDIS:  Transmitter Disable: 0 = Noeffect.
             | (1uL <<  8)              // RSTSTA: Reset Status Bits: 1 = Resets the status bits PARE, FRAME, OVRE and RXBRK in the US_CSR.
             | (0uL <<  9)              // STTBRK: Start Break: 0 = Noeffect.
             | (0uL << 10)              // STPBRK: Stop Break: 0 = Noeffect.
             | (0uL << 11)              // STTTO:  Start Time-out: 0 = Noeffect.
             | (0uL << 12);             // SENDA:  Send Address: 0 = Noeffect.
    US1_MR   = (0uL <<  4)              // USCLKS: Clock Selection: 0 = MCK
             | (3uL <<  6)              // CHRL:   Character Length: 3 = Eight bits
             | (0uL <<  8)              // SYNC:   Synchronous Mode Select: 0 = USART operates in Asynchronous Mode.
             | (4uL <<  9)              // PAR:    Parity Type: 0x4 = No parity
             | (0uL << 12)              // NBSTOP: Number of Stop Bits: 0 = 1 stop bit
             | (0uL << 14)              // CHMODE: Channel Mode: 0 = Normal mode
             | (0uL << 17)              // MODE9:  9-bit Character Length: 0 = CHRL defines character length.
             | (0uL << 18);             // CKLO:   Clock Output Select: 0 = The USART does not drive the SCK pin.
    US1_BRGR = (USART_FREQ / (Baudrate * 16));
    US1_IER  = (1uL <<  0);             // Enable Rx Interrupt
    //
    // Setup interrupt controller for USART
    //
    OS_ARM_InstallISRHandler(USART1_ID, &USART1_RX_TX_IRQHandler);  // OS UART interrupt handler vector
    OS_ARM_ISRSetPrio(USART1_ID, USART_IRQ_PRIO);                   // Level sensitive, selected priority.
    AIC_ICCR = (1uL << USART1_ID);                                  // Clear OS usart interrupt.
    OS_ARM_EnableISR(USART1_ID);                                    // Enable OS usart interrupts
    break;
  case USART2:
    PMC_PCER = (1uL << USART2_ID)       // Enable peripheral clock for selected USART
             | (1uL << PIOD_ID);        // Enable the PIO port where UART pins are located
    PIOD_PDR = PIO_PDR_USART2;          // Enable peripheral output signals (disable PIO Port)
    PIOD_ASR = PIO_PDR_USART2;          // Select peripheral function A ob according to selected USART
    PIOD_BSR = 0;                       // De-select "B" peripherals on PIO for USART
    US2_IDR  = 0xFFFFFFFFuL;            // Initially disable all USART interrupts
    US2_CR   = (1uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (1uL <<  3);             // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
    US2_CR   = (0uL <<  2)              // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
             | (0uL <<  3)              // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
             | (1uL <<  4)              // RXEN:   Receiver Enable: 1 = The receiver is enabled if RXDIS is 0.
             | (0uL <<  5)              // RXDIS:  Receiver Disable: 0 = Noeffect.
             | (1uL <<  6)              // TXEN:   Transmitter Enable: 1 = The transmitter is enabled if TXDIS is 0.
             | (0uL <<  7)              // TXDIS:  Transmitter Disable: 0 = Noeffect.
             | (1uL <<  8)              // RSTSTA: Reset Status Bits: 1 = Resets the status bits PARE, FRAME, OVRE and RXBRK in the US_CSR.
             | (0uL <<  9)              // STTBRK: Start Break: 0 = Noeffect.
             | (0uL << 10)              // STPBRK: Stop Break: 0 = Noeffect.
             | (0uL << 11)              // STTTO:  Start Time-out: 0 = Noeffect.
             | (0uL << 12);             // SENDA:  Send Address: 0 = Noeffect.
    US2_MR   = (0uL <<  4)              // USCLKS: Clock Selection: 0 = MCK
             | (3uL <<  6)              // CHRL:   Character Length: 3 = Eight bits
             | (0uL <<  8)              // SYNC:   Synchronous Mode Select: 0 = USART operates in Asynchronous Mode.
             | (4uL <<  9)              // PAR:    Parity Type: 0x4 = No parity
             | (0uL << 12)              // NBSTOP: Number of Stop Bits: 0 = 1 stop bit
             | (0uL << 14)              // CHMODE: Channel Mode: 0 = Normal mode
             | (0uL << 17)              // MODE9:  9-bit Character Length: 0 = CHRL defines character length.
             | (0uL << 18);             // CKLO:   Clock Output Select: 0 = The USART does not drive the SCK pin.
    US2_BRGR = (USART_FREQ / (Baudrate * 16));
    US2_IER  = (1uL <<  0);             // Enable Rx Interrupt
    //
    // Setup interrupt controller for USART
    //
    OS_ARM_InstallISRHandler(USART2_ID, &USART2_RX_TX_IRQHandler);  // OS UART interrupt handler vector
    OS_ARM_ISRSetPrio(USART2_ID, USART_IRQ_PRIO);                   // Level sensitive, selected priority.
    AIC_ICCR = (1uL << USART2_ID);                                  // Clear OS usart interrupt.
    OS_ARM_EnableISR(USART2_ID);                                    // Enable OS usart interrupts
    break;
  }
  OS_INT_DecRI();
}

/*********************************************************************
*
*       BSP_UART_DeInit()
*
*  Function description
*    De-initializes the UART peripheral.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*/
void BSP_UART_DeInit(unsigned int Unit) {
  BSP_UART_USE_PARA(Unit);
}

/*********************************************************************
*
*       BSP_UART_SetBaudrate()
*
*  Function description
*    Configures/changes the UART baud rate.
*
*  Parameters
*    Unit    : Unit number (typically zero-based).
*    Baudrate: Baud rate to configure [Hz].
*/
void BSP_UART_SetBaudrate(unsigned int Unit, unsigned long Baudrate) {
  _SetBaudrate(Unit, Baudrate);
}

/*********************************************************************
*
*       BSP_UART_SetReadCallback()
*
*  Function description
*    Sets the callback to execute upon an Rx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetReadCallback(unsigned Unit, BSP_UART_RX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfReadCB = pf;
}

/*********************************************************************
*
*       BSP_UART_SetWriteCallback()
*
*  Function description
*    Sets the callback to execute upon a Tx interrupt.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    pf  : Callback to execute.
*/
void BSP_UART_SetWriteCallback(unsigned int Unit, BSP_UART_TX_CB* pf) {
  BSP_UART_USE_PARA(Unit);
  _pfWriteCB = pf;
}

/*********************************************************************
*
*       BSP_UART_Write1()
*
*  Function description
*    Sends one byte via UART.
*
*  Parameters
*    Unit: Unit number (typically zero-based).
*    Data: (First) data byte to send.
*
*  Additional information
*    The first byte of a transfer is typically sent from application
*    context. Further bytes of the transfer are then sent from the
*    Tx interrupt handler by also calling this function from interrupt
*    context.
*/
void BSP_UART_Write1(unsigned int Unit, unsigned char Data) {
  switch (Unit) {
  case USART0:
    while ((US0_CSR & USART_TXRDY) == 0) {
      ;  // Wait until THR becomes available
    }
    US0_THR = Data;
    US0_IER = USART_TXRDY;  // Enable Tx interrupt
    break;
  case USART1:
    while ((US1_CSR & USART_TXRDY) == 0) {
      ;  // Wait until THR becomes available
    }
    US1_THR = Data;
    US1_IER = USART_TXRDY;  // Enable Tx interrupt
    break;
  case USART2:
    while ((US2_CSR & USART_TXRDY) == 0) {
      ;  // Wait until THR becomes available
    }
    US2_THR = Data;
    US2_IER = USART_TXRDY;  // Enable Tx interrupt
    break;
  }
}

/*************************** End of file ****************************/
