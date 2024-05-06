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

  Device : STM32F051
  Board  : IAR STM32F051R8

  Unit | UART   | Board connector
  ===============================
  0    | USART1 | RS232

*/

#include "BSP_UART.h"
#include "RTOS.h"        // For OS_INT_Enter()/OS_INT_Leave(). Remove this line and OS_INT_* functions if not using OS.
#include "stm32f0xx.h"   // Device specific header file, contains CMSIS defines.

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
#define BSP_UART_IRQHandler   (USART1_IRQHandler)

#define USART_BASE_ADDR       (0x40013800u)  // USART 1
#define USART_CR1             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x00u))
#define USART_BRR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x0Cu))
#define USART_ISR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x1Cu))
#define USART_RDR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x24u))
#define USART_TDR             (*(volatile unsigned long*)(USART_BASE_ADDR + 0x28u))

#define GPIO_BASE_ADDR        (0x48000000u)  // GPIO A
#define GPIO_MODER            (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x00u))
#define GPIO_OTYPER           (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x04u))
#define GPIO_OSPEEDR          (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x08u))
#define GPIO_AF_HIGH          (*(volatile unsigned long*)(GPIO_BASE_ADDR + 0x24u))

#define RCC_BASE_ADDR         (0x40021000u)
#define RCC_AHBENR            (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x14u))
#define RCC_APB2ENR           (*(volatile unsigned long*)(RCC_BASE_ADDR + 0x18u))

#define US_RXRDY              (0x20u)   // RXNE
#define US_TXEMPTY            (0x80u)   // TXE
#define USART_RX_ERROR_FLAGS  (0x0Fu)   // ORE/NE/FE/PE
#define USART_BRR_VALUE       (BSP_UART_CLOCK / BSP_UART_BAUDRATE)

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

void BSP_UART_IRQHandler(void);

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
*       BSP_UART_IRQHandler()
*
*  Function description
*    UART Rx & Tx interrupt handler.
*
*  Additional information
*    Needs to inform the OS that we are in interrupt context.
*/
void BSP_UART_IRQHandler(void) {
  unsigned int  Status;
  unsigned char Data;

  OS_INT_EnterNestable();
  Status = USART_ISR;                        // Examine status register
  //
  // Handle Rx.
  //
  do {
    if (Status & US_RXRDY) {                // Data received?
      Data = USART_RDR;
      if ((Status & USART_RX_ERROR_FLAGS) == 0) {  // Any error ?
        if (_pfReadCB) {
          _pfReadCB(0, (unsigned char)Data);
        }
      }
    }
    Status = USART_ISR;                      // Examine current status
  } while (Status & US_RXRDY);
  //
  // Handle Tx.
  //
  if (Status & US_TXEMPTY) {
    if (_pfWriteCB) {
      if (_pfWriteCB(0)) {                  // No more characters to send ?
        USART_CR1 &= ~0x40uL;               // Disable further Tx interrupts
      }
    }
  }
  OS_INT_LeaveNestable();
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
  //
  // Unused parameters. Hard coded USART settings.
  //
  BSP_UART_USE_PARA(Unit);
  BSP_UART_USE_PARA(Baudrate);
  BSP_UART_USE_PARA(NumDataBits);
  BSP_UART_USE_PARA(Parity);
  BSP_UART_USE_PARA(NumStopBits);
  //
  // Setup clocks, GPIO ports and NVIC IRQs.
  //
  RCC_AHBENR    |= (1uL << 17);              // GPIOB CLK enable
  RCC_APB2ENR   |= (1uL << 14);              // Enable USART1 clock
                                             // GPIOC set alternate function for USART3
  GPIO_AF_HIGH  =  (1uL << 4)                // - Set pin_09 to AF1
                |  (1uL << 8);               // - Set pin_10 to AF1
                                             // GPIOC speed setting
  GPIO_OSPEEDR  =  (3uL << 18)               // - Pin_09
                |  (3uL << 20);              // - Pin_10
  GPIO_OTYPER   =  0x0;                      // Output type: push-pull for pin_09 and pin_10
                                             // GPIOC alternate function mode
  GPIO_MODER    |= (2uL << 18)               // - Pin_09 AF
                |  (2uL << 20);              // - Pin_10 AF
  NVIC_SetPriority(USART1_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
  NVIC_EnableIRQ(USART1_IRQn);
  //
  // Initialize USART
  //
  USART_CR1 =  (1uL << 3)                    // Transmitter enable
            |  (1uL << 2)                    // Receiver enable
            |  (1uL << 5);                   // RX interrupt enable
  USART_BRR =  USART_BRR_VALUE;              // Set baudrate
  USART_CR1 |= (1uL << 0);                   // Enable USART
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
  NVIC_DisableIRQ(USART1_IRQn);
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
  BSP_UART_USE_PARA(Unit);
  while ((USART_ISR & (1uL << 7)) == 0);
  USART_TDR  = Data;
  USART_CR1 |= 0x40;  // Enable Tx interrupt
}

/*************************** End of file ****************************/
