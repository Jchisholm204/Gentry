/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "cytypes.h"
#include "`$INSTANCE_NAME`_view.h"
#include "`$INSTANCE_NAME`_UART.h"
#include "`$INSTANCE_NAME`_rx_isr.h"
#include "`$INSTANCE_NAME`_tx_isr.h"
#include "RTOS.h"

CY_ISR(tx_Interrupt);
CY_ISR(rx_Interrupt);


/*******************************************************************************
* Function Name:
********************************************************************************
* Summary:
*  The start function initializes the UART.
*
*
*
* Parameters:
*
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void)
{
  `$INSTANCE_NAME`_UART_Start();
  `$INSTANCE_NAME`_rx_isr_Start();
  `$INSTANCE_NAME`_tx_isr_Start();
  `$INSTANCE_NAME`_tx_isr_SetVector(tx_Interrupt);
  `$INSTANCE_NAME`_rx_isr_SetVector(rx_Interrupt);
}
/*******************************************************************************
* Function Name:
********************************************************************************
* Summary:
*  The start function initializes the UART.
*
*
*
* Parameters:
*
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_WriteData(uint8 txData)
{
  `$INSTANCE_NAME`_UART_WriteTxData(txData);
  `$INSTANCE_NAME`_tx_isr_Enable();  // enable tx interrupt
}

/*******************************************************************************
* Function Name:
********************************************************************************
* Summary:
*  The tx SetVector function.
*
*
*
* Parameters:
*
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
CY_ISR(tx_Interrupt)
{
  OS_INT_Enter();
  if (OS_COM_OnTx()) {                           // No more characters to send ?
    `$INSTANCE_NAME`_tx_isr_Disable();       // Disable further tx interrupts
  }
  OS_INT_Leave();
}

/*******************************************************************************
* Function Name:
********************************************************************************
* Summary:
*  The rx SetVector function.
*
*
*
* Parameters:
*
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
CY_ISR(rx_Interrupt)
{
  OS_INT_Enter();
  OS_COM_OnRx(`$INSTANCE_NAME`_UART_ReadRxData());                       // Process actual byte
  OS_INT_Leave();
}

/* [] END OF FILE */
