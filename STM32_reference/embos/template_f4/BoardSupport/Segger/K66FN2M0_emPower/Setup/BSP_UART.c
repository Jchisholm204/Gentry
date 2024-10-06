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
Purpose : UART implementation

Additional information:

  Device : K66FN2M0
  Board  : SEGGER emPower

  Unit | UART   | Board connector
  ===============================
  0    | UART4  | ExpIF 0 (J500)
  1    | UART2  | ExpIF 1 (J501)
  2    | UART0  | ExpIF 2 (J502)

*/

#include "BSP_UART.h"
#include "RTOS.h"     // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "MK66F18.h"  // Device specific header file, contains CMSIS defines.

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define BSP_UART_BAUDRATE     (38400)
#define BSP_UART_CLOCK        (SystemCoreClock)

#define USART0_BASE_ADDR      (0x4006A000)
#define USART2_BASE_ADDR      (0x4006C000)
#define USART4_BASE_ADDR      (0x400EA000)

//
// Used to set/change the order of USART units.
//
#define BSP_USART0            (2)
#define BSP_USART2            (1)
#define BSP_USART4            (0)

#define USART_BDH(addr)       (*(volatile unsigned char*)((addr) + 0x00u))
#define USART_BDL(addr)       (*(volatile unsigned char*)((addr) + 0x01u))
#define USART_C2(addr)        (*(volatile unsigned char*)((addr) + 0x03u))
#define USART_S1(addr)        (*(volatile unsigned char*)((addr) + 0x04u))
#define USART_C3(addr)        (*(volatile unsigned char*)((addr) + 0x06u))
#define USART_D(addr)         (*(volatile unsigned char*)((addr) + 0x07u))

#define US_RXRDY              (0x20u)  // RXNE
#define US_TXEMPTY            (0x80u)  // TXE
#define USART_RX_ERROR_FLAGS  (0x0Fu)  // ORE/NE/FE/PE

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static BSP_UART_TX_CB* _pfWriteCB[3];
static BSP_UART_RX_CB* _pfReadCB[3];

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

void UART0_RX_TX_IRQHandler(void);
void UART2_RX_TX_IRQHandler(void);
void UART4_RX_TX_IRQHandler(void);

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
  unsigned long  pUSARTBaseAddr;
  unsigned int   bRXWasEnabled;
  unsigned int   bTXWasEnabled;
  unsigned long  Clock;
  unsigned short Rate;

  Clock = SystemCoreClock;
  switch (Unit) {
  case BSP_USART0:
    pUSARTBaseAddr  = USART0_BASE_ADDR;
    break;
  case BSP_USART2:
    pUSARTBaseAddr  = USART2_BASE_ADDR;
    Clock *= (((SIM_CLKDIV1 & 0xF0000000) >> 28) + 1);    // If the UART is based on the BUS clock instead of the PCL, check the divider parameters.
    Clock /= (((SIM_CLKDIV1 & 0x0F000000) >> 24) + 1);    // Re-multiply by the SystemClock divider and divide by the bus clock divider.
    break;
  case BSP_USART4:
    pUSARTBaseAddr  = USART4_BASE_ADDR;
    Clock *= (((SIM_CLKDIV1 & 0xF0000000) >> 28) + 1);    // If the UART is based on the BUS clock instead of the PCL, check the divider parameters.
    Clock /= (((SIM_CLKDIV1 & 0x0F000000) >> 24) + 1);    // Re-multiply by the SystemClock divider and divide by the bus clock divider.
    break;
  default:
    return;
  }

  bRXWasEnabled = 0;
  bTXWasEnabled = 0;
  if (USART_C2(pUSARTBaseAddr) & (1u << 2)) {
    bRXWasEnabled = 1;
    USART_C2(pUSARTBaseAddr) &= ~(1u << 2);               // Disable RX of UART
  }
  if (USART_C2(pUSARTBaseAddr) & (1u << 3)) {
    bTXWasEnabled = 1;
    USART_C2(pUSARTBaseAddr) &= ~(1u << 3);               // Disable TX of UART
  }

  Rate = (unsigned short)(Clock / (Baudrate * 16));
  USART_BDH(pUSARTBaseAddr) &= (0xE0);
  USART_BDH(pUSARTBaseAddr) |= (Rate >> 8);
  USART_BDL(pUSARTBaseAddr)  = (Rate & 0xFF);

  if (bRXWasEnabled == 1) {
    USART_C2(pUSARTBaseAddr) |= (1u << 2);                // Enable RX of UART
  }
  if (bTXWasEnabled == 1) {
    USART_C2(pUSARTBaseAddr) |= (1u << 3);                // Enable TX of UART
  }
}

/*********************************************************************
*
*       _UART_IRQHandler()
*
*  Function description
*    Called by UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
static void _UART_IRQHandler(unsigned int Unit, unsigned long pUSARTBaseAddr) {
  unsigned char Status;
  unsigned char Data;

  OS_INT_EnterNestable();
  Status = USART_S1(pUSARTBaseAddr);                           // Read status register.
  //
  // Handle Rx.
  //
  do {
    if (Status & US_RXRDY) {                                   // Data received?
      Data = USART_D(pUSARTBaseAddr);
      if ((Status & USART_RX_ERROR_FLAGS) == 0) {              // If no errors
        if (_pfReadCB[Unit] != NULL) {
          _pfReadCB[Unit](Unit, Data);                         // Process actual byte
        }
      }
    }
    Status = USART_S1(pUSARTBaseAddr);                         // Examine current status
  } while (Status & US_RXRDY);
  //
  // Handle Tx if interrupt enabled aka we have started sending at least one character.
  //
  if ((USART_C2(pUSARTBaseAddr) & (1uL << 6)) != 0u) {
    if (Status & US_TXEMPTY) {
      if (_pfWriteCB[Unit] != NULL && _pfWriteCB[Unit](Unit)) {  // No more characters to send ?
        USART_C2(pUSARTBaseAddr) &= ~0x40uL;                     // Disable further tx interrupts
      }
    }
  }
  OS_INT_LeaveNestable();
}

/*********************************************************************
*
*       Global functions, IRQ handler
*
**********************************************************************
*/

/*********************************************************************
*
*       UART0_RX_TX_IRQHandler()
*
*  Function description
*    UART0 Rx & Tx interrupt handler.
*/
void UART0_RX_TX_IRQHandler(void) {
  _UART_IRQHandler(BSP_USART0, USART0_BASE_ADDR);
}

/*********************************************************************
*
*       UART2_RX_TX_IRQHandler()
*
*  Function description
*    UART2 Rx & Tx interrupt handler.
*/
void UART2_RX_TX_IRQHandler(void) {
  _UART_IRQHandler(BSP_USART2, USART2_BASE_ADDR);
}

/*********************************************************************
*
*       UART4_RX_TX_IRQHandler()
*
*  Function description
*    UART4 Rx & Tx interrupt handler.
*/
void UART4_RX_TX_IRQHandler(void) {
  _UART_IRQHandler(BSP_USART4, USART4_BASE_ADDR);
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
  unsigned long pUSARTBaseAddr;

  //
  // Unused parameters. Hard coded USART settings.
  //
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  //
  // Setup clocks, GPIO ports and NVIC IRQs.
  //
  switch (Unit) {
  case BSP_USART0:
    pUSARTBaseAddr  = USART0_BASE_ADDR;
    SIM_SCGC5      |= (1uL << 10);                                    // Enable Port B clock
    SIM_SCGC4      |= (1uL << 10);                                    // Enable clock for UART 0
    PORTB_PCR16     = ((0x03u << 8) & 0x700u);                        // Set Pin PTB16 MUX to GPIO (Rx)
    PORTB_PCR17     = ((0x03u << 8) & 0x700u);                        // Set Pin PTB17 MUX to GPIO (Tx)
    NVIC_SetPriority(UART0_RX_TX_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(UART0_RX_TX_IRQn);
    break;
  case BSP_USART2:
    pUSARTBaseAddr  = USART2_BASE_ADDR;
    SIM_SCGC5      |= (1uL << 12);                                    // Enable Port D clock
    SIM_SCGC4      |= (1uL << 12);                                    // Enable clock for UART 2
    PORTD_PCR2      = ((0x03u << 8) & 0x700u);                        // Set Pin PTD2 MUX to GPIO (Rx)
    PORTD_PCR3      = ((0x03u << 8) & 0x700u);                        // Set Pin PTD3 MUX to GPIO (Tx)
    NVIC_SetPriority(UART2_RX_TX_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(UART2_RX_TX_IRQn);
    break;
  case BSP_USART4:
    pUSARTBaseAddr  = USART4_BASE_ADDR;
    SIM_SCGC5      |= (1uL << 13);                                    // Enable Port E clock
    SIM_SCGC1      |= (1uL << 10);                                    // Enable clock for UART 4
    PORTE_PCR25     = ((0x03u << 8) & 0x700u);                        // Set Pin PTE25 MUX to GPIO (Rx)
    PORTE_PCR24     = ((0x03u << 8) & 0x700u);                        // Set Pin PTE24 MUX to GPIO (Tx)
    NVIC_SetPriority(UART4_RX_TX_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(UART4_RX_TX_IRQn);
    break;
  default:
    return;
  }
  //
  // Initialize USART
  //
  USART_C2(pUSARTBaseAddr)   = 0               // Configure C2 register
                             | (0 << 6)        // Transmission complete interrupt enable, this an TxEmpty flag, enable only when sending, clear when done
                             | (1 << 5)        // Receiver full interrupt enable
                             | (1 << 3)        // Transmitter enable
                             | (1 << 2)        // Receiver enable
                             ;
  USART_C3(pUSARTBaseAddr)   = 0
                             | (1 << 3)        // Overrun interrupt enable
                             | (1 << 2)        // Noise error interrupt enable
                             | (1 << 1)        // Framing error interrupt enable
                             | (1 << 0)        // Parity error interrupt enable
                             ;
  //
  // Set the desired baudrate.
  // Use a default of 38400 baud for old code that did not specify a baudrate.
  //
  if (Baudrate == 0u) {
    Baudrate = 38400u;
  }
  _SetBaudrate(Unit, Baudrate);
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
  switch (Unit) {
  case BSP_USART0:
    USART_C2(USART0_BASE_ADDR) = 0;
    SIM_SCGC4      &= ~(1uL << 10);      // Disable clock for UART 0
    NVIC_DisableIRQ(UART0_RX_TX_IRQn);
    break;
  case BSP_USART2:
    USART_C2(USART2_BASE_ADDR) = 0;
    SIM_SCGC4      &= ~(1uL << 12);      // Disable clock for UART 2
    NVIC_DisableIRQ(UART2_RX_TX_IRQn);
    break;
  case BSP_USART4:
    USART_C2(USART4_BASE_ADDR) = 0;
    SIM_SCGC1      &= ~(1uL << 10);      // Disable clock for UART 4
    NVIC_DisableIRQ(UART4_RX_TX_IRQn);
    break;
  default:
    break;
  }
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
  _pfReadCB[Unit] = pf;
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
  _pfWriteCB[Unit] = pf;
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
  unsigned long pUSARTBaseAddr;

  switch (Unit) {
  case BSP_USART0:
    pUSARTBaseAddr = USART0_BASE_ADDR;
    break;
  case BSP_USART2:
    pUSARTBaseAddr = USART2_BASE_ADDR;
    break;
  case BSP_USART4:
    pUSARTBaseAddr = USART4_BASE_ADDR;
    break;
  default:
    return;
  }
  USART_D(pUSARTBaseAddr)   = Data;
  USART_C2(pUSARTBaseAddr) |= (1uL << 6);  // Enable tx interrupt
}

/*************************** End of file ****************************/
