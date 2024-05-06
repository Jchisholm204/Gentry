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

  Device : AT91SAM7S256 and AT91SAM7S64
  Board  : AT91SAM7S EK

  Unit | UART   | Board connector
  ===============================
  0    | USART0 | UART J2

*/

#include "BSP_UART.h"
#include "RTOS.h"      // For OS_ARM_*() ISR functions.


/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SYSTEM_FREQ     (47923200u)
#define USART_FREQ      (SYSTEM_FREQ)
#define USART_BAUDRATE  (38400u)

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/


//
// PMC (power management controller)
//
#define PMC_BASE_ADDR         (0xFFFFFC00u)
#define PMC_PCER              (*(volatile unsigned int*)(PMC_BASE_ADDR + 0x10u))   // Peripheral clock enable register

//
// PIOA
//
#define PIOA_BASE_ADDR        (0xFFFFF400u)
#define PIOA_PDR              (*(volatile unsigned int*)(PIOA_BASE_ADDR + 0x04u))  // PIOA disable register
#define PIOA_ASR              (*(volatile unsigned int*)(PIOA_BASE_ADDR + 0x70u))  // PIOA "A" peripheral select register
#define PIOA_BSR              (*(volatile unsigned int*)(PIOA_BASE_ADDR + 0x74u))  // PIOA "B" peripheral select register

#define PIO_PDR_USART         ((1u << USART_RX_PIN) | (1u << USART_TX_PIN))

//
// USART
//
#define USART0_BASE_ADDR      (0xFFFC0000u)
#define US_CR                 (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x000u))
#define US_MR                 (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x004u))
#define US_IER                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x008u))
#define US_IDR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x00Cu))
#define US_IMR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x010u))
#define US_CSR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x014u))
#define US_RHR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x018u))
#define US_THR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x01Cu))
#define US_BRGR               (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x020u))
#define US_RTOR               (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x024u))
#define US_TTGR               (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x028u))
#define US_TPR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x108u))
#define US_TCR                (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x10Cu))
#define US_PTCR               (*(volatile unsigned int*)(USART0_BASE_ADDR + 0x120u))

#define USART_ID              (6u)
#define USART_RX_PIN          (5u)
#define USART_TX_PIN          (6u)
#define USART_RXRDY           (1u << 0)         // Rx status flag
#define USART_TXRDY           (1u << 1)         // Tx RDY Status flag
#define USART_TXEMPTY         (1u << 9)         // Tx EMPTY Status flag
#define USART_ENDTX           (1u << 4)         // Tx end flag
#define USART_RX_ERROR_FLAGS  (0xE0u)           // Parity, framing, overrun error
#define USART_PRIO            (0x00u)           // Lowest priority for UART interrupts
#define USART_TX_INT_FLAG     (USART_ENDTX)

//
// Advanced interrupt controller (AIC)
//
#define AIC_BASE_ADDR         (0xFFFFF000u)
#define AIC_ICCR              (*(volatile unsigned int*)(AIC_BASE_ADDR + 0x128u))


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

static void UART_RX_TX_IRQHandler(void);

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
*       UART_RX_TX_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
static void UART_RX_TX_IRQHandler(void) {
  unsigned int Data;
  unsigned int Status;
  unsigned int Unit;

  Unit   = 0u;
  Status = US_CSR;                                            // Examine status register
  do {
    if (Status & USART_RXRDY) {                               // Data received?
      Data = US_RHR;
      if (Status & USART_RX_ERROR_FLAGS) {                    // Any error ?
        US_CR = (1u << 8);                                    // RSTSTA: Reset Status Bits PARE, FRAME, OVRE and RXBRK
      } else {
        if (_pfReadCB != NULL) {
          _pfReadCB(Unit, Data);                              // Forward the received byte to the callback.
        }
      }
    }
    if (Status & US_IMR & USART_TX_INT_FLAG) {           // Check Tx status => Send next character
      if ((_pfWriteCB != NULL) && (_pfWriteCB(Unit) != 0)) {  // No more characters to send ?
        US_IDR = USART_TX_INT_FLAG;                           // Disable further tx interrupts
      }
    }
    Status = US_CSR;                                      // Examine current status
  } while (Status & US_IMR & (USART_TX_INT_FLAG | USART_RXRDY));
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
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  OS_INT_IncDI();
  //
  // Initializes peripheral clock and USART
  //
  PMC_PCER = (1u << USART_ID);  // Enable peripheral clock for USART0
  PIOA_PDR = PIO_PDR_USART;     // Enable peripheral output signals (disable PIO Port A)
  PIOA_ASR = PIO_PDR_USART;     // Select "A" peripherals on PIO A (USART Rx, TX)
  PIOA_BSR = 0;                 // Deselect "B" peripherals on PIO A
  US_CR    = (1u <<  2)         // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
           | (1u <<  3);        // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
  US_CR    = (0u <<  2)         // RSTRX:  Reset Receiver: 1 = The receiver logic is reset.
           | (0u <<  3)         // RSTTX:  Reset Transmitter: 1 = The transmitter logic is reset.
           | (1u <<  4)         // RXEN:   Receiver Enable: 1 = The receiver is enabled if RXDIS is 0.
           | (0u <<  5)         // RXDIS:  Receiver Disable: 0 = Noeffect.
           | (1u <<  6)         // TXEN:   Transmitter Enable: 1 = The transmitter is enabled if TXDIS is 0.
           | (0u <<  7)         // TXDIS:  Transmitter Disable: 0 = Noeffect.
           | (1u <<  8)         // RSTSTA: Reset Status Bits: 1 = Resets the status bits PARE, FRAME, OVRE and RXBRK in the US_CSR.
           | (0u <<  9)         // STTBRK: Start Break: 0 = Noeffect.
           | (0u << 10)         // STPBRK: Stop Break: 0 = Noeffect.
           | (0u << 11)         // STTTO:  Start Time-out: 0 = Noeffect.
           | (0u << 12);        // SENDA:  Send Address: 0 = Noeffect.
  US_MR    = (0u <<  4)         // USCLKS: Clock Selection: 0 = MCK
           | (3u <<  6)         // CHRL:   Character Length: 3 = Eight bits
           | (0u <<  8)         // SYNC:   Synchronous Mode Select: 0 = USART operates in Asynchronous Mode.
           | (4u <<  9)         // PAR:    Parity Type: 0x4 = No parity
           | (0u << 12)         // NBSTOP: Number of Stop Bits: 0 = 1 stop bit
           | (0u << 14)         // CHMODE: Channel Mode: 0 = Normal mode
           | (0u << 17)         // MODE9:  9-bit Character Length: 0 = CHRL defines character length.
           | (0u << 18);        // CKLO:   Clock Output Select: 0 = The USART does not drive the SCK pin.
  US_BRGR  = (USART_FREQ / (USART_BAUDRATE * 16));
  US_IDR   = 0xFFFFFFFFu;       // Disable all interrupts
  US_IER   = (1u << 0)          // Enable Rx Interrupt
           | (0u << 1);         // Do not Enable Tx Interrupt
  //
  // Setup interrupt controller for USART
  //
  OS_ARM_InstallISRHandler(USART_ID, &UART_RX_TX_IRQHandler);  // OS UART interrupt handler vector
  OS_ARM_ISRSetPrio(USART_ID, USART_PRIO);                     // Level sensitive, selected priority.
  AIC_ICCR = (1u << USART_ID);                                 // Clear OS usart interrupt.
  OS_ARM_EnableISR(USART_ID);                                  // Enable OS usart interrupts
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
  static volatile unsigned char Buffer;

  BSP_UART_USE_PARA(Unit);
  Buffer  = Data;
  US_TPR  = (unsigned int)&Buffer;
  US_TCR  = 1u;
  US_PTCR = (1u << 8);          // Enable DMA transmission
  US_IER  = USART_TX_INT_FLAG;  // Enable Tx interrupt
}

/*************************** End of file ****************************/
