/*!************************************************************************//*!
 * \file    netx_drv_def.h
 * \brief   This file contains the peripheral driver common defines, enumeration,
 *          macros and structures definitions.
 * $Revision: 8158 $
 * $Date: 2020-09-18 18:50:56 +0200 (Fr, 18 Sep 2020) $
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
#ifndef __NETX_DRV_DEF
#define __NETX_DRV_DEF

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup generals
 * \{
 */

#include <stddef.h>

/*!
 * \brief If there is the need to outsmart the compiler or linter.
 */
#define UNUSED(x) ((void)(x))

/*!
 * \brief The definition of callbacks used in the driver.
 *
 * In the dio, the ptDriverHandle is a DRV_DIO_ID_T so one should cast as following:
 * (DRV_DIO_ID_T) ptDriverHandle; to get an integer of the given pointer
 */
typedef void (*DRV_CALLBACK_F)(void * pvDriverHandle, void * pvUserHandle);

/*!
 * \brief DRV Status structures definition
 */
typedef enum DRV_STATUS_Etag
{
  DRV_OK = 0U,/*!< Everything is ok and works as intended. */
  DRV_ERROR = -1,/*!< A general error occurred. */
  DRV_BUSY = -2,/*!< The driver or device is busy. */
  DRV_TOUT = -3,/*!< The driver or device timed out. */
  DRV_NSUPP = -4,/*!< The requested function is not supported for the given parameters. */
  DRV_NIMPL = -5,/*!< The requested function is not implemented for the given parameters. */
  DRV_ERROR_PARAM = -6,/*!< The given parameters are incorrect. */
  DRV_LOCKED = -7,/*!< The drivers api is locked by an other execution context. */
  DRV_ERROR_BUFFER = -8,/*!< The buffer is corrupted. */
} DRV_STATUS_E, *DRV_STATUS_EP;

/*!
 * \brief Enumeration of the operation mode.
 */
typedef enum
{
  DRV_OPERATION_MODE_POLL = 0x0u,/*!< Default. The state signaling an event is read greedy. */
  DRV_OPERATION_MODE_IRQ = 0x1u,/*!< This mode uses interrupts to signal events. */
  DRV_OPERATION_MODE_DMA = 0x2u,/*!< This mode uses DMA to copy data and interrupts to signal events. */
  DRV_OPERATION_MODE_MIN = DRV_OPERATION_MODE_POLL,/*!< The minimum enumeration value for boundary check. */
  DRV_OPERATION_MODE_MAX = DRV_OPERATION_MODE_DMA/*!< The maximum enumeration value for boundary check. */
} DRV_OPERATION_MODE_E;

/*! \} *//* End of group generals */

#ifdef __cplusplus
}
#endif

#endif /* __NETX_DRV_DEF */
