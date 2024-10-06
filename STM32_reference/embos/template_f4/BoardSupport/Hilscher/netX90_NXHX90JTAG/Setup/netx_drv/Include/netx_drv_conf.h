/*!************************************************************************//*!
 * \file    netx_drv_conf.h
 * \brief   DRV configuration file.
 * $Revision: 6592 $
 * $Date: 2019-12-17 14:14:57 +0100 (Di, 17 Dez 2019) $
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
#ifndef __NETX_DRV_CONF_H
#define __NETX_DRV_CONF_H

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup netX_Driver
 * \{
 */

/*!
 * \addtogroup Configuration
 * \{
 */

/* ****************************************************************************/
/* ************************** DEFAULT Configuration ***************************/
/* ****************************************************************************/
/*!
 * The user has to configure the driver in a file "user_drv_conf.h". As an
 * example of that file one is able to find an annotated default configuration
 * below the include of the users configuration file.
 */
#include "netx_drv_user_conf.h"

/* ****************************************************************************/
/* ************************ Device/Chip Configuration *************************/
/* ****************************************************************************/

#if defined( DRV_DEVICE_NETX_90 )
#include "csp/netx_drv_csp_netx90_app.h"
#elif defined( DRV_DEVICE_NETX_90_MPW )
#error "The MPW chip is no longer supported!"
#else
#error "A device has to be specified!"
#endif

/* ****************************************************************************/
/* ************************* PARAMETER Configuration **************************/
/* ****************************************************************************/

#ifndef DRV_DMAC_LIST_LENGTH
/*!
 * \brief Length of the list used for dma. For 4095 copys by 1 or 4 byte an element is needed.
 */
#define DRV_DMAC_LIST_LENGTH 1u
#endif

/* ****************************************************************************/
/* **************************** RTOS Configuration ****************************/
/* ****************************************************************************/
#ifdef RTOS_USED
#ifndef RTOS_ERROR_IGNORE
#error "In case of using the driver with realtime operating systems the user"\
 "is responsible to ensure that the locking mechanism is functional. This"\
 "warning can be ignored by defining RTOS_ERROR_IGNORE."
#endif

#else

/*!
 * \brief Type used for the element to be locked in default is our mutex implementation.
 */
#define DRV_LOCK_T DRV_MUTEX_T

/*!
 * \brief Initializer value of the type to be locked as rvalue is in default the mutex initializer value.
 */
#define DRV_LOCK_INITIALIZER_VALUE DRV_MUTEX_INITIALIZER_VALUE

/*!
 * \brief Initializer type of the type to be locked as rvalue is in default the mutex initializer type.
 */
#define DRV_LOCK_INITIALIZER_TYPE DRV_MUTEX_INITIALIZER_TYPE

/*!
 * \brief Initializer of the type to be locked as rvalue is in default the mutex initializer.
 */
#define DRV_LOCK_INITIALIZER DRV_MUTEX_INITIALIZER

/*!
 * \brief A function calling the trylock of the mutex and returning locked in case it is blocked.
 */
#define DRV_LOCK(__HANDLE__) if(DRV_MUTEX_TRYLOCK(&(__HANDLE__)->tLock)!=DRV_OK){return DRV_LOCKED;}

/*!
 * \brief The release function used.
 */
#define DRV_UNLOCK(__HANDLE__) DRV_MUTEX_UNLOCK(&(__HANDLE__)->tLock)

#endif /* RTOS_USED */

/* ****************************************************************************/
/* ************************** MODULE Configuration ****************************/
/* ****************************************************************************/
#ifdef DRV_DIO_MODULE_ENABLED
#ifndef DRV_DIO_MODULE_SUPPORTED
#error "DRV_DIO_MODULE is not supported by the chip support package."
#else
#include "netx_drv_dio.h"
#endif/* DRV_DIO_MODULE_SUPPORTED */
#endif /* DRV_DIO_MODULE_ENABLED */

#ifdef DRV_SPI_MODULE_ENABLED
#ifndef DRV_SPI_MODULE_SUPPORTED
#error "DRV_SPI_MODULE is not supported by the chip support package."
#else
#include "netx_drv_cortex.h"
#include "netx_drv_dmac.h"
#include "netx_drv_spi.h"
#endif/* DRV_SPI_MODULE_SUPPORTED */
#endif /* DRV_SPI_MODULE_DISABLED */

#ifdef DRV_UART_MODULE_ENABLED
#ifndef DRV_UART_MODULE_SUPPORTED
#error "DRV_UART_MODULE is not supported by the chip support package."
#else
#include "netx_drv_cortex.h"
#include "netx_drv_dmac.h"
#include "netx_drv_uart.h"
#endif /* DRV_UART_MODULE_SUPPORTED */
#endif /* DRV_UART_MODULE_DISABLED */

#ifdef DRV_TIM_MODULE_ENABLED
#ifndef DRV_TIM_MODULE_SUPPORTED
#error "DRV_TIM_MODULE is not supported by the chip support package."
#else
#include "netx_drv_tim.h"
#endif /* DRV_TIM_MODULE_SUPPORTED */
#endif /* DRV_TIM_MODULE_DISABLED */

#ifdef DRV_I2C_MODULE_ENABLED
#ifndef DRV_I2C_MODULE_SUPPORTED
#error "DRV_I2C_MODULE is not supported by the chip support package."
#else
#include "netx_drv_i2c.h"
#endif /* DRV_I2C_MODULE_SUPPORTED */
#endif /* DRV_I2C_MODULE_DISABLED */

#ifdef DRV_CANCTRL_MODULE_ENABLED
#ifndef DRV_CANCTRL_MODULE_SUPPORTED
#error "DRV_CANCTRL_MODULE is not supported by the chip support package."
#else
#include "netx_drv_canctrl.h"
#endif /* DRV_CANCTRL_MODULE_SUPPORTED */
#endif /* DRV_CANCTRL_MODULE_DISABLED */

#ifdef DRV_CORTEX_MODULE_ENABLED
#ifndef DRV_CORTEX_MODULE_SUPPORTED
#error "DRV_CORTEX_MODULE is not supported by the chip support package."
#else
#include "netx_drv_cortex.h"
#endif /* DRV_CORTEX_MODULE_SUPPORTED */
#endif /* DRV_CORTEX_MODULE_DISABLED */

#ifdef DRV_BISS_MODULE_ENABLED
#ifndef DRV_BISS_MODULE_SUPPORTED
#error "DRV_BISS_MODULE is not supported by the chip support package."
#else
#include "netx_drv_biss.h"
#endif /* DRV_BISS_MODULE_SUPPORTED */
#endif /* DRV_BISS_MODULE_DISABLED */

#ifdef DRV_ETH_XPIC_MODULE_ENABLED
#ifndef DRV_ETH_XPIC_MODULE_SUPPORTED
#error "DRV_ETH_XPIC_MODULE is not supported by the chip support package."
#else
#include "netx_drv_eth_xpic.h"
#include "netx_drv_eth_xpic_def.h"
#include "netx_drv_eth_xpic_ram.h"
#endif /* DRV_ETH_XPIC_MODULE_SUPPORTED */
#endif /* DRV_ETH_XPIC_MODULE_ENABLED */

#ifdef DRV_XPIC_MODULE_ENABLED
#ifndef DRV_XPIC_MODULE_SUPPORTED
#error "DRV_XPIC_MODULE is not supported by the chip support package."
#else
#include "netx_drv_xpic.h"
#endif /* DRV_ETH_XPIC_MODULE_SUPPORTED */
#endif /* DRV_ETH_XPIC_MODULE_ENABLED */

#ifdef DRV_ADC_MODULE_ENABLED
#ifndef DRV_ADC_MODULE_SUPPORTED
#error "DRV_ADC_MODULE is not supported by the chip support package."
#else
#include "netx_drv_adc.h"
#endif /* DRV_ADC_MODULE_SUPPORTED */
#endif /* DRV_ADC_MODULE_DISABLED */

#ifdef DRV_MLED_MODULE_ENABLED
#ifndef DRV_MLED_MODULE_SUPPORTED
#error "DRV_MLED_MODULE is not supported by the chip support package."
#else
#include "netx_drv_mled.h"
#endif /* DRV_MLED_MODULE_SUPPORTED */
#endif /* DRV_MLED_MODULE_DISABLED */

#ifdef DRV_DMAC_MODULE_ENABLED
#ifndef DRV_DMAC_MODULE_SUPPORTED
#error "DRV_DMAC_MODULE is not supported by the chip support package."
#else
#include "netx_drv_cortex.h"
#include "netx_drv_dmac.h"
#endif /* DRV_DMAC_MODULE_SUPPORTED */
#endif /* DRV_DMAC_MODULE_DISABLED */

/*! \} *//* End of group Configuration */

/*! \} *//* End of group netx_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __NETX_DRV_CONF_H */
