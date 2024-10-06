/*!************************************************************************//*!
 * \file     netx_drv_cortex.c
 * \brief    CORTEX peripheral module driver.
 *           This file provides firmware functions to manage the following 
 *           functionalities of the CORTEX:
 *            + Initialization and de-initialization functions
 *            + Peripheral Control functions 
 * $Revision: 8126 $
 * $Date: 2020-08-28 20:18:26 +0200 (Fr, 28 Aug 2020) $
 * \copyright Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
 * \note Exclusion of Liability for this demo software:
 * The following software is intended for and must only be used for reference and in an
 * evaluation laboratory environment. It is provided without charge and is subject to
 * alterations. There is no warranty for the software, to the extent permitted by
 * applicable law. Except when otherwise stated in writing the copyright holders and/or
 * other parties provide the software "as is" without warranty of any kind, either
 * expressed or implied.
 * Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
 * By installing or otherwise using the software, you accept the terms of this Agreement.
 * If you do not agree to the terms of this Agreement, then do not install or use the
 * Software!
 ******************************************************************************/

#include "netx_drv.h"
#ifdef DRV_CORTEX_MODULE_ENABLED

/*!
 * \addtogroup CORTEX CORTEX
 * \{
 * \brief This is the CORTEX peripheral driver
 *
 * \details
 * Most of the functions of this driver component are just wrappings for the underlying
 * CMSIS functions. Some of them provide convenience functionalities like the recursive
 * interrupt locking of the interrupt enable and disable functions.
 */

/*!
 * \param PriorityGroup
 */
void DRV_NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  /* Set the PRIGROUP[10:8] bits according to the PriorityGroup parameter value */
  NVIC_SetPriorityGrouping(PriorityGroup);
}

/*!
 * \param[in] IRQn
 * \param[in] PreemptPriority
 * \param[in] SubPriority
 */
void DRV_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t prioritygroup = 0x00U;

  prioritygroup = NVIC_GetPriorityGrouping();

  NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
}

/*!
 * \param[in] IRQn
 */
void DRV_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  /* Enable interrupt */
  NVIC_EnableIRQ(IRQn);
}

/*!
 * \param[in] IRQn
 */
void DRV_NVIC_DisableIRQ(IRQn_Type IRQn)
{
  /* Disable interrupt */
  NVIC_DisableIRQ(IRQn);
}

/*!
 * Semaphore value for nested enabling/disabling of all interrupts.
 */
int ulIrqSemaphore = 0;

/*!
 *
 */
void DRV_NVIC_SystemReset(void)
{
  /* System Reset */
  NVIC_SystemReset();
}

/*!
 * \param[in] TicksNumb
 */
uint32_t DRV_SYSTICK_Config(uint32_t TicksNumb)
{
  return SysTick_Config(TicksNumb);
}

/*!
 * \return
 */
uint32_t DRV_NVIC_GetPriorityGrouping(void)
{
  /* Get the PRIGROUP[10:8] field value */
  return NVIC_GetPriorityGrouping();
}

/*!
 * \param[in] IRQn
 * \param[in] PriorityGroup
 * \param[in] pPreemptPriority
 * \param[in] pSubPriority
 */
void DRV_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t *pPreemptPriority, uint32_t *pSubPriority)
{
  /* Get priority for Cortex-M system or device specific interrupts */
  NVIC_DecodePriority(NVIC_GetPriority(IRQn), PriorityGroup, pPreemptPriority, pSubPriority);
}

/*!
 * \param[in] IRQn
 */
void DRV_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  /* Set interrupt pending */
  NVIC_SetPendingIRQ(IRQn);
}

/*!
 * \param[in] IRQn
 */
uint32_t DRV_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  /* Return 1 if pending else 0 */
  return NVIC_GetPendingIRQ(IRQn);
}

/*!
 * \param[in] IRQn
 */
void DRV_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  /* Clear pending interrupt */
  NVIC_ClearPendingIRQ(IRQn);
}

/*!
 * \param[in] IRQn
 */
uint32_t DRV_NVIC_GetActive(IRQn_Type IRQn)
{
  /* Return 1 if active else 0 */
  return NVIC_GetActive(IRQn);
}

/*!
 * \param[in] IRQn
 */
uint32_t DRV_NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
  /* Return 1 if active else 0 */
  return NVIC_GetEnableIRQ(IRQn);
}

/*!
 * \param[in] IRQn
 */
void DRV_NVIC_AttachIRQ(IRQn_Type IRQn, void* pfIRQHandler)
{
#ifdef NVIC_SOFT_VECTOR
  g_avpfVTOR[IRQn + 16] = pfIRQHandler;
#else
#endif
}

/*!
 * \param[in] IRQn
 */
void* DRV_NVIC_GetIRQ(IRQn_Type IRQn)
{
  return ((void**) SCB->VTOR)[IRQn + 16];
}

/**
 \details Inputs a character via the external variable \ref ITM_RxBuffer.
 \return             Received character.
 \return         -1  No character pending.
 */
int32_t DRV_ITM_ReceiveChar(void)
{
  return ITM_ReceiveChar();
}
/**
 \details Checks whether a character is pending for reading in the variable \ref ITM_RxBuffer.
 \return          0  No character available.
 \return          1  Character available.
 */
int32_t DRV_ITM_CheckChar(void)
{
  return ITM_CheckChar();
}
/**
 \details Transmits a character via the ITM channel 0, and
 \li Just returns when no debugger is connected that has booked the output.
 \li Is blocking when a debugger is connected, but the previous character sent has not been transmitted.
 \param [in]     ch  Character to transmit.
 \returns            Character to transmit.
 */
uint32_t DRV_ITM_SendChar(uint32_t ch)
{
  return DRV_ITM_SendChar(ch);
}

/*! Change CPU state to running.
 *
 *  \param  eCpuId    CPU ID
 *  \param  fRunning  1 = running, 0 = not running
 *  \return SUCCESS_HIL_OK on success
 */
DRV_STATUS_E DRV_MCP_ChangeState(DRV_MCP_CPU_ID_E eCpuId, DRV_MCP_CPU_STATE_E fRunning)
{
  if(eCpuId >= DRV_MCP_CPU_ID_MAX || eCpuId < DRV_MCP_CPU_ID_COM)
  {
    return DRV_ERROR_PARAM;
  }
  switch (fRunning)
  {
  case DRV_MCP_CPU_STATE_RESET:
    DRV_MCP_DEVICE->hs_irq_reset_raw |= 0x1u << (eCpuId * 3);
    break;
  case DRV_MCP_CPU_STATE_RUNNING:
    DRV_MCP_DEVICE->hs_irq_set_raw |= 0x1u << (eCpuId * 3);
    break;
  case DRV_MCP_CPU_STATE_RESET_REQUEST:
    DRV_MCP_DEVICE->hs_irq_set_raw |= 0x2u << (eCpuId * 3);
    break;
  default:
    return DRV_ERROR_PARAM;
  }

  return DRV_OK;
}

static DRV_CALLBACK_F s_afnMCPCallbacks[DRV_MCP_CPU_COUNT];
static void* s_apMCPCallbacksUserPointer[DRV_MCP_CPU_COUNT];

void MCP_IRQHandler(void)
{
  uint32_t ulIRQRegister = DRV_MCP_DEVICE->hs_irq_set_raw;
  for(size_t index = 0; index < DRV_MCP_CPU_COUNT; index++)
  {
    if((ulIRQRegister & (0x1u << (index * 3u))) == 1)
    {
      s_afnMCPCallbacks[index](NULL, s_apMCPCallbacksUserPointer[index]);
    }
  }
}

/*! Get CPU state information.
 *
 *  \return Current MCP value
 */
DRV_STATUS_E DRV_MCP_GetState(DRV_MCP_CPU_ID_E eCpuId, DRV_MCP_CPU_STATE_E* epState)
{
  if(eCpuId >= DRV_MCP_CPU_ID_MAX || eCpuId < DRV_MCP_CPU_ID_COM)
  {
    return DRV_ERROR_PARAM;
  }
  *epState = DRV_MCP_DEVICE->hs_irq_reset_raw >> (eCpuId * 3) & 0x7u;
  return DRV_OK;
}

/*! Attaches the given values as interrupt callback and user pointer.
 *
 * If a NULL pointer is given, the interrupt is masked out.
 * A call to this function will mask in the MCP IRQ.
 * The MCP interrupt is never disabled again.
 *
 *  \return void
 */
void DRV_MCP_IRQAttach(DRV_MCP_CPU_ID_E eCpuId, DRV_CALLBACK_F pfnUserClb, void* pvUser)
{
  DRV_NVIC_EnableIRQ(mcp_app_IRQn);
  s_afnMCPCallbacks[eCpuId] = pfnUserClb;
  s_apMCPCallbacksUserPointer[eCpuId] = pvUser;
  if(pfnUserClb != NULL)
  {
    DRV_MCP_DEVICE->hs_irq_set_mask = 0x2u << (eCpuId * 3);
  }
  else
  {
    DRV_MCP_DEVICE->hs_irq_reset_mask = 0x2u << (eCpuId * 3);
  }
}

/*! \} *//* End of group CORTEX */

#endif /* DRV_CORTEX_MODULE_DISABLED */
