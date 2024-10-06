/*!************************************************************************//*!
 * \file    netx_drv_dio.h
 * \brief   Header file of DIO DRV module.
 * $Revision: 4538 $
 * $Date: 2018-11-22 17:26:55 +0100 (Do, 22 Nov 2018) $
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
#ifndef __NETX_DRV_DIO_H
#define __NETX_DRV_DIO_H

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup netX_Driver
 * \{
 */

/*!
 * \addtogroup DIO
 * \{
 */

/*!
 * \struct DRV_DIO_HANDLE_T
 * \brief The handle of the driver.
 *
 * It SHALL not be modified outside of the driver, even if it appears to be possible.
 */
typedef struct DRV_DIO_HANDLE_Ttag
{
  DRV_GPIO_DEVICE_T* ptGPIODevice;/*!< \private The GPIO device register as bitfield and value unions.*/
  DRV_PIO_DEVICE_T* ptPIODevice;/*!< \private The PIO device register as bitfield and value unions.*/
  DRV_MMIO_DEVICE_T* ptMMIODevice;/*!< \private The MMIO device register as bitfield and value unions.*/
  DRV_HIF_IO_DEVICE_T* ptHIFIODevice;/*!< \private The HIF device register as bitfield and value unions.*/
} DRV_DIO_HANDLE_T;

/*!
 * The enumeration of the dio modules line registers.
 */
typedef enum DRV_DIO_LINE_Etag
{
  DRV_DIO_LINE_NONE =         0x00000000ul,/*!< No line selected.*/
  DRV_DIO_LINE_GPIO =         0x00000001ul,/*!< GPIO line select.*/
  DRV_DIO_LINE_PIO  =         0x00000002ul,/*!< PIO line select.*/
  DRV_DIO_LINE_MMIO =         0x00000003ul,/*!< MMIO line selected.*/
  DRV_DIO_LINE_HIF  =         0x00000004ul,/*!< HIF line select.*/
  DRV_DIO_LINE_MAX  =         DRV_DIO_LINE_HIF/*!< HIF line select.*/
} DRV_DIO_LINE_E;

/*!
 * \brief Type definition for the driver io channel mask.
 */
typedef uint64_t DRV_DIO_MSK_T;
/*!
 * \brief Type definition for the driver io channel ids.
 */
typedef uint32_t DRV_DIO_ID_T;

/*!
 * \brief The mode of the DIO
 */
typedef enum DRV_DIO_MODE_Etag
{
  // Input
  DRV_DIO_MODE_READ = 0x00u,/*!< The DIO read mode.*/
  DRV_DIO_MODE_CAPTURE_CONT_RISING = 0x01u,/*!< The DIO capture mode continued at rising edge (allows irq on each capture).*/
  DRV_DIO_MODE_CAPTURE_ONCE_RISING = 0x02u,/*!< The DIO capture mode once at rising edge (reset irq to capture again).*/
  DRV_DIO_MODE_CAPTURE_ONCE_HIGH = 0x03u,/*!< The DIO capture mode once at high level (reset irq to capture again).*/
  // Output
  DRV_DIO_MODE_RESET = 0x04u,/*!< The DIO reset mode set to 0.*/
  DRV_DIO_MODE_SET = 0x05u,/*!< The DIO set mode set to 1.*/
  DRV_DIO_MODE_LINE = 0x06u,/*!< The DIO line mode.*/
  DRV_DIO_MODE_PWM = 0x07u,/*!< The DIO pwm mode, direct threshold update (might cause hazards on output).*/
  DRV_DIO_MODE_BLINK = 0x08u,/*!< The DIO blink mode.*/
  // Multi channel modes
  DRV_DIO_MODE_PWM2 = 0x0fu,/*!< The DIO PWM2 mode.*/
  DRV_DIO_MODE_CAPTURE_CONT_FALLING = 0x11u,/*!< The DIO capture mode continued at falling edge (allows irq on each capture).*/
  DRV_DIO_MODE_CAPTURE_CONT_HIGH = 0x12u,/*!< The DIO capture mode continued at high level (allows irq on each capture).*/
  DRV_DIO_MODE_CAPTURE_CONT_LOW = 0x13u,/*!< The DIO capture mode continued at low level (allows irq on each capture).*/
} DRV_DIO_MODE_E;

/*!
 * \brief The state of the io port.
 */
typedef enum DRV_DIO_STATE_Etag
{
  DRV_DIO_STATE_SET = 0x01u, /*!< The port is triggered/high.*/
  DRV_DIO_STATE_RESET = 0x00u /*!< The port is not triggered/low.*/
} DRV_DIO_STATE_E;

/*!
 * \brief The counters chooseable as reference for the IOs.
 */
typedef enum DRV_DIO_CNTR_REF_Etag
{
  DRV_DIO_CNTR_REF_0 = 0x00u, /*!< The counter nr 0.*/
  DRV_DIO_CNTR_REF_1 = 0x01u, /*!< The counter nr 1.*/
  DRV_DIO_CNTR_REF_2 = 0x02u, /*!< The counter nr 2.*/
  DRV_DIO_CNTR_REF_SYSTIME = 0x03u /*!< The system time as cntr.*/
} DRV_DIO_CNTR_REF_E;

/*!
 * \brief This function initializes the global DRV_DIO_APP_HANDLE_T object.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_Init(void);

/*!
 * \brief This function disables the global DRV_DIO_APP_HANDLE_T object.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_DeInit(void);

/*!
 * \brief This function (re)initializes a specific channel and sets its mode.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelSetMode(DRV_DIO_ID_T ulChannelID, DRV_DIO_MODE_E eMode);

/*!
 * \brief This function inverts the input/output of a specific channel.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelInvert(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function sets the mode of the specified channel to set (high or inverted high).
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelOutSet(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function sets the mode of the specified channel to reset (low or inverted low).
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelOutReset(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function toggles the mode of the specified channel if it was set or reset before (from high to low or inverse).
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelOutToggle(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function stores the channel state at the given position.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelInGet(DRV_DIO_ID_T ulChannelID, DRV_DIO_STATE_E * const peState);

/*!
 * \brief This function masks a io channels interrupt out.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelIRQMask(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function unmasks a io channels interrupt.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelIRQUnmask(DRV_DIO_ID_T ulChannelID);

/*!
 * \brief This function attaches a given function to a DIO channels interrupt.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_ChannelIRQAttach(DRV_DIO_ID_T ulChannelID, DRV_CALLBACK_F pfnUserClb, void* pvUser);

/*!
 * \brief This function activates the filter on the HIF channels regarding the interrupt sampling.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_HifIRQFilterEnable(void);

/*!
 * \brief This function deactivates the filter on the HIF channels regarding the interrupt sampling.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_HifIRQFilterDisable(void);

/*!
 * \brief This function sets the channels defined by the mask.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineMaskedSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullMask, DRV_DIO_MSK_T ullChannels);

/*!
 * \brief This function sets the channels defined.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineOutSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels);

/*!
 * \brief This function resets the channels defined.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineOutReset(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels);

/*!
 * \brief This function enables line output capability on the channels defined.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineOutEnableSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels);

/*!
 * \brief This function Disables output capability on the channels defined.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineOutEnableReset(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels);

/*!
 * \brief This function stores the state readable of all channels of all given lines into the channel value.
 * \memberof DRV_DIO_HANDLE_T
 */
DRV_STATUS_E DRV_DIO_LineInGet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T * const pullChannels);

/*!
 * \brief This is the callback of the GPIO 0 IRQ.
 * \public
 */
void DRV_DIO_GPIO0_Callback(void);

/*!
 * \brief This is the callback of the GPIO 1 IRQ.
 * \public
 */
void DRV_DIO_GPIO1_Callback(void);

/*!
 * \brief This is the callback of the GPIO 2 IRQ.
 * \public
 */
void DRV_DIO_GPIO2_Callback(void);

/*!
 * \brief This is the callback of the GPIO 3 IRQ.
 * \public
 */
void DRV_DIO_GPIO3_Callback(void);

/*!
 * \brief This is the callback of the GPIO 4 IRQ.
 * \public
 */
void DRV_DIO_GPIO4_Callback(void);

/*!
 * \brief This is the callback of the GPIO 5 IRQ.
 * \public
 */
void DRV_DIO_GPIO5_Callback(void);

/*!
 * \brief This is the callback of the GPIO 6 IRQ.
 * \public
 */
void DRV_DIO_GPIO6_Callback(void);

/*!
 * \brief This is the callback of the GPIO 7 IRQ.
 * \public
 */
void DRV_DIO_GPIO7_Callback(void);

/*!
 * \brief This is the callback of the BOD IRQ.
 * \public
 */
void DRV_DIO_BOD_Callback(void);

/*!
 * \brief This is the callback of the HIFPIO IRQ.
 * \public
 */
void DRV_DIO_HIFPIO_Callback(void);

/*!
 * \brief This is the callback of the PIO0 IRQ.
 * \public
 */
void DRV_DIO_PIO_Callback(void);

/*! \} *//* End of group DIO */

/*! \} *//* End of group netx_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __NETX_DRV_DIO_H */
