/*!************************************************************************//*!
 * \file    netx_drv_cortex.h
 * \brief   Header file of CORTEX module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#include "netx_drv.h"
#ifndef __NETX_DRV_CORTEX_H
#define __NETX_DRV_CORTEX_H

#ifdef __cplusplus
extern "C"
{
#endif


/*! \addtogroup netX_Driver
 * \{
 */

/*!
 * \addtogroup CORTEX
 * \{
 */
/*!
 * \brief The enumeration of the states a CPU has.
 *
 * Those states are used .
 */
typedef enum DRV_MCP_CPU_STATE_Etag
{
  DRV_MCP_CPU_STATE_RESET = 0,/*!< Defines the state that the core is running.*/
  DRV_MCP_CPU_STATE_RUNNING,/*!< Defines the state that the core is running.*/
  DRV_MCP_CPU_STATE_RESET_REQUEST,/*!< Defines the state that the core is ready for reset.*/
} DRV_MCP_CPU_STATE_E;


/*!
 * \brief Semaphore value for nested enabling/disabling of all interrupts.
 */
extern int ulIrqSemaphore;

/*!
 * \brief This method sets the priority grouping.
 */
void DRV_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
/*!
 * \brief This function returns the priority grouping.
 */
uint32_t DRV_NVIC_GetPriorityGrouping(void);
/*!
 * \brief This method sets the priority of a given interrupt.
 */
void DRV_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
/*!
 * \brief This method stores the priority of a given interrupt and group into the given locations.
 */
void DRV_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
/*!
 * \brief This method enables a given interrupt.
 */
void DRV_NVIC_EnableIRQ(IRQn_Type IRQn);
/*!
 * \brief This method disables a given interrupt.
 */
void DRV_NVIC_DisableIRQ(IRQn_Type IRQn);
/*!
 * \brief This is a semaphore function for nested enabling the interrupts.
 */
__STATIC_INLINE int DRV_IRQ_Enable(void);
/*!
 * \brief This is a semaphore function for nested disabling the interrupts.
 */
__STATIC_INLINE int DRV_IRQ_Disable(void);
/*!
 * \brief This method resets the NVIC and the Cortex M4 internal registers and restarts the PC at 0.
 */
void DRV_NVIC_SystemReset(void);
/*!
 * \brief This funciton returns the pending state of a given interrupt.
 */
uint32_t DRV_NVIC_GetPendingIRQ(IRQn_Type IRQn);
/*!
 * \brief This method sets a given interrupt pending.
 */
void DRV_NVIC_SetPendingIRQ(IRQn_Type IRQn);
/*!
 * \brief This method clears the pending state of a given interrupt.
 */
void DRV_NVIC_ClearPendingIRQ(IRQn_Type IRQn);
/*!
 * \brief This function returns the active state of a given interrupt.
 */
uint32_t DRV_NVIC_GetActive(IRQn_Type IRQn);
/*!
 * \brief This function returns the enable state of a given interrupt.
 */
uint32_t DRV_NVIC_GetEnableIRQ(IRQn_Type IRQn);
/*!
 * \brief This function may be used to attach or detach an interrupt to the software vector if it is used.
 */
void DRV_NVIC_AttachIRQ(IRQn_Type IRQn, void* pfIRQHandler);
/*!
 * \brief This function may be used to get the pointer of an interrupt.
 */
void* DRV_NVIC_GetIRQ(IRQn_Type IRQn);
/**
 \brief  Driver wrapper for ITM Receive Character
 */
int32_t DRV_ITM_ReceiveChar(void);
/**
 \brief  Driver wrapper for  ITM Check Character
 */
int32_t DRV_ITM_CheckChar(void);
/**
 \brief  Driver wrapper for  ITM Send Character
 */
uint32_t DRV_ITM_SendChar(uint32_t ch);
/**
 \brief  Driver function for changing the MCP State
 */
DRV_STATUS_E DRV_MCP_ChangeState(DRV_MCP_CPU_ID_E eCpuId, DRV_MCP_CPU_STATE_E fRunning);
/**
 \brief  Driver function for finding out the MCP State
 */
DRV_STATUS_E DRV_MCP_GetState(DRV_MCP_CPU_ID_E eCpuId, DRV_MCP_CPU_STATE_E* epState);
/**
 \brief  Driver function for attaching an interrupt to the reset request of another core
 */
void DRV_MCP_IRQAttach(DRV_MCP_CPU_ID_E eCpuId, DRV_CALLBACK_F pfnUserClb, void* pvUser);

/*!
 * \brief Enables the IRQ recursively.
 *
 * Enables and disables were counted. Transition between 0 and 1 lead to state change.
 * From 1 to 0 Enables the Interrupt. Enabling reduces the lock (disable) counter.
 * The used Semaphore is not protected against overflow!
 * [0..INT_MIN] IRQ is enabled
 * [INT_MAX..1] IRQ is disabled
 *
 * \return the counting semaphore
 */
__STATIC_FORCEINLINE int DRV_IRQ_Enable(void)
{
  // Because the interupts should be disabled this is atomic
  ulIrqSemaphore-=1;
  if(ulIrqSemaphore == 0)
  {
    __enable_irq();
  }
  return ulIrqSemaphore;
}

/*!
 * \brief Disables the IRQ recursively.
 *
 * Enables and disables were counted. Transition between 0 and 1 lead to state change.
 * Disables the Interrupt always. Disabling iterates the lock (disable) counter.
 * The used Semaphore is not protected against overflow!
 * IRQ is disabled
 *
 * \return The counting semaphore
 */
__STATIC_FORCEINLINE int DRV_IRQ_Disable(void)
{
  /* Disable interrupt */
  __disable_irq();
  ulIrqSemaphore++;
  return ulIrqSemaphore;
}

/*! \} *//* End of group CORTEX */

/*! \} *//* End of group netx_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __NETX_DRV_CORTEX_H */
