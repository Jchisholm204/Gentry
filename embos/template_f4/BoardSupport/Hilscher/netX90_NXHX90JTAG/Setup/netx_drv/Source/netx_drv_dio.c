/*!************************************************************************//*!
 * \file     netx_drv_dio.c
 * \brief    DIO peripheral module driver.
 *           This file provides firmware functions to manage the following 
 *           functionalities of the General Purpose Input/Output (DIO) peripheral:
 *            + Initialization and de-initialization functions
 *            + IO operation functions
 *            + Interrupt control
 * $Revision: 6124 $
 * $Date: 2019-08-28 19:41:54 +0200 (Mi, 28 Aug 2019) $
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
#ifdef DRV_DIO_MODULE_ENABLED
#include <string.h>

/*lint -save -e685 */
/*lint -save -e568 */

/*! \addtogroup DIO DIO
 * \{
 * \brief This is the DIO driver, defined by DRV_DIO_HANDLE_T.
 *
 * \details
 *
 * The dio driver combines the pio functionality of multiple input and output devices
 * into a single api. Because there is not a one to one connection between a pad or pin of
 * the chip and an input output (io) function, we call an io function that is able to be mapped
 * to a pad a channel. Those channels are addressable as a single channel or as a line
 * or set of channels. Also such channels might not just have an simple io functionality
 * but also generate interrupts or have some extended functionality. Therefore it is possible
 * to set the mode of a channel to the specified functionality like it is set(on) or reset(off)
 * or it is in line mode. Also the capture mode for input has to be defined first, or the
 * channel might not be set for input at all.
 *
 * In the following table we have most of the functionalities listed and if a device is either
 * able to (+), unable to (-) or some channels are and some not (o)
 *
 * Feature          | GPIO | MMIO | PIO | HIF
 * -----------------|------|------|-----|-----
 * Set              |   +  |   +  |  +  |  +
 * Reset            |   +  |   +  |  +  |  +
 * Line             |   +  |   +  |  +  |  +
 * Read             |   +  |   +  |  +  |  +
 * Invert           |   +  |   -  |  +  |  +
 * Capture rising   |   +  |   -  |  +  |  o
 * Capture high     |   +  |   -  |  +  |  o
 * Capture falling  |   +  |   -  |  +  |  o
 * Capture low      |   +  |   -  |  +  |  o
 *
 * The capture mode is also the mode generating the IRQs, so it includes the IRQ functionality.
 */

/*!
 * Static object of the IO driver.
 * It might be used as first parameter in every function. But because there is no need of any context object
 * in the IO driver, it is used statical.
 */
DRV_DIO_HANDLE_T const s_tIODriver = { DRV_GPIO_DEVICE, DRV_PIO_DEVICE, DRV_MMIO_DEVICE, DRV_HIF_IO_DEVICE };

/*!
 * Interrupt attached handler function vector.
 */
DRV_CALLBACK_F s_afnIRQHandler[DRV_DIO_IRQ_COUNT] = { 0 };

/*!
 * Interrupt attached handler context pointers.
 */
void* s_apIRQHandlerHandles[DRV_DIO_IRQ_COUNT] = { 0 };

/*!
 * Define for checking the consistency of the handle or static representation of the driver.
 */
#ifndef DRV_HANDLE_CHECK_INACTIVE
#define DRV_HANDLE_CHECK(handle); \
  if((handle).ptGPIODevice==0||(handle).ptMMIODevice==0||(handle).ptPIODevice==0){ \
    return DRV_ERROR; \
  }
#else
#define DRV_HANDLE_CHECK(handle) ;
#endif

/*! HIF IRQ Mode, derived from the DIO Mode.*/
typedef enum
{
  DRV_DIO_HIF_IRQ_MODE_LOW = 0x0u, /*!< IRQ at low level.*/
  DRV_DIO_HIF_IRQ_MODE_HIGH = 0x1u, /*!< IRQ at high level.*/
  DRV_DIO_HIF_IRQ_MODE_FALLING = 0x2u, /*!< IRQ at falling edge.*/
  DRV_DIO_HIF_IRQ_MODE_RISING = 0x3u /*!< IRQ at rising edge.*/
} DRV_DIO_HIF_IRQ_MODE_E;

/*!
 * \brief Helper function.
 *
 * Sets the output of the defined pio channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_SetPIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptPIODevice->pio_out |= ulChannel & DRV_DIO_LINE_MSK_PIO;
  s_tIODriver.ptPIODevice->pio_oe |= ulChannel & DRV_DIO_LINE_MSK_PIO;
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined pio channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ResetPIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptPIODevice->pio_out &= ~(ulChannel & DRV_DIO_LINE_MSK_PIO );
  s_tIODriver.ptPIODevice->pio_oe |= (ulChannel & DRV_DIO_LINE_MSK_PIO );
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined pio channel and resets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ReadModePIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptPIODevice->pio_out &= ~(ulChannel & DRV_DIO_LINE_MSK_PIO );
  s_tIODriver.ptPIODevice->pio_oe &= ~(ulChannel & DRV_DIO_LINE_MSK_PIO );
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Sets the output of the defined hif channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_SetHIF(uint64_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  ulChannel &= DRV_DIO_LINE_MSK_HIF;
  s_tIODriver.ptHIFIODevice->hif_pio_out0 |= ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_oe0 |= ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_out1 |= (ulChannel >> 32);
  s_tIODriver.ptHIFIODevice->hif_pio_oe1 |= (ulChannel >> 32);
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined hif channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ResetHIF(uint64_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptHIFIODevice->hif_pio_out0 &= ~ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_oe0 |= ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_out1 &= ~(ulChannel >> 32);
  s_tIODriver.ptHIFIODevice->hif_pio_oe1 |= (ulChannel >> 32);
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined hif channel and resets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ReadModeHIF(uint64_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptHIFIODevice->hif_pio_out0 &= ~ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_oe0 &= ~ulChannel;
  s_tIODriver.ptHIFIODevice->hif_pio_out1 &= ~(ulChannel >> 32);
  s_tIODriver.ptHIFIODevice->hif_pio_oe1 &= ~(ulChannel >> 32);
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Sets the output of the defined mmio channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_SetMMIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptMMIODevice->mmio_pio_out_line_set_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  s_tIODriver.ptMMIODevice->mmio_pio_oe_line_set_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined mmio channel and sets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ResetMMIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptMMIODevice->mmio_pio_out_line_reset_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  s_tIODriver.ptMMIODevice->mmio_pio_oe_line_set_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  (void) DRV_IRQ_Enable();
}

/*!
 * \brief Helper function.
 *
 * Resets the output of the defined mmio channel and resets its output enable.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in] ulChannel The bit representing the channel
 */
__INLINE static void DRV_DIO_ReadModeMMIO(uint32_t ulChannel)
{
  (void) DRV_IRQ_Disable();
  s_tIODriver.ptMMIODevice->mmio_pio_out_line_reset_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  s_tIODriver.ptMMIODevice->mmio_pio_oe_line_reset_cfg0 = (ulChannel & DRV_DIO_LINE_MSK_MMIO );
  (void) DRV_IRQ_Enable();
}

/*!
 * The Init function is used to reset the digital input and output devices to the
 * correct reset state. It shall be used to guarantee the correct power on reset
 * state as described or to regain it during runtime.
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_Init(void)
{
  (void) DRV_IRQ_Disable();
  memset(s_tIODriver.ptGPIODevice, 0, sizeof(*s_tIODriver.ptGPIODevice));
  s_tIODriver.ptGPIODevice->gpio_app_irq_raw = 0xFFFFFFFF;
  s_tIODriver.ptMMIODevice->mmio_pio_oe_line_cfg0 = 0x0ul;
  s_tIODriver.ptMMIODevice->mmio_pio_out_line_cfg0 = 0x0ul;
  s_tIODriver.ptPIODevice->pio_out = 0x0ul;
  s_tIODriver.ptPIODevice->pio_oe = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_cfg = 0x80000008ul;
  s_tIODriver.ptHIFIODevice->hif_pio_oe0 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_oe1 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_out0 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_out1 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_irq_arm_mask_reset = 0xFFFFFFFF;
  (void) DRV_IRQ_Enable();
  DRV_STATUS_E ret = DRV_OK;
  return ret;
}

/*!
 * The DeInit function is quite similar to the DRV_DIO_Init() function.
 * The correct reset state of all dio devices will be forced. But a check
 * on the static and constant handle is performed. This should be used if
 * a stack corruption is considered.
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_DeInit(void)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  (void) DRV_IRQ_Disable();
  memset(s_tIODriver.ptGPIODevice, 0, sizeof(*s_tIODriver.ptGPIODevice));
  s_tIODriver.ptGPIODevice->gpio_app_irq_raw = 0xFFFFFFFF;
  s_tIODriver.ptMMIODevice->mmio_pio_oe_line_cfg0 = 0x0ul;
  s_tIODriver.ptMMIODevice->mmio_pio_out_line_cfg0 = 0x0ul;
  s_tIODriver.ptPIODevice->pio_out = 0x0ul;
  s_tIODriver.ptPIODevice->pio_oe = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_cfg = 0x80000008ul;
  s_tIODriver.ptHIFIODevice->hif_pio_oe0 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_oe1 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_out0 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_out1 = 0x0ul;
  s_tIODriver.ptHIFIODevice->hif_pio_irq_arm_mask_reset = 0xFFFFFFFF;
  (void) DRV_IRQ_Enable();
  DRV_STATUS_E ret = DRV_OK;
  return ret;
}

/*!
 * The modes will be forcefully set on every channel.
 * The modes are available and explained in the \ref DRV_DIO_MODE_E enumeration.
 * Inversion will be reseted also.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in] ulChannelID The id of the channel to be configured
 * \param[in] eMode   The mode to be set
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelSetMode(DRV_DIO_ID_T ulChannelID, DRV_DIO_MODE_E eMode)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    if(eMode <= DRV_DIO_MODE_BLINK || eMode == DRV_DIO_MODE_PWM2)
    {
      s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = eMode;
      ret = DRV_OK;
    }
    else if(eMode == DRV_DIO_MODE_CAPTURE_CONT_FALLING)
    {
      s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_CAPTURE_CONT_RISING;
      s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].inv = 0x01ul;
      ret = DRV_OK;
    }
    else
    {
      ret = DRV_NSUPP;
    }
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    uint32_t ulMSK = (0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN));
    switch (eMode)
    {
    case DRV_DIO_MODE_SET:
      DRV_DIO_SetPIO(ulMSK);
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_RESET:
      DRV_DIO_ResetPIO(ulMSK);
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_READ:
      DRV_DIO_ReadModePIO(ulMSK);
      ret = DRV_OK;
      break;
#ifdef DRV_DIO_PIO_IRQ_SUPPORTED
    case DRV_DIO_MODE_CAPTURE_CONT_FALLING:
      (void) DRV_IRQ_Disable();
      s_tIODriver.ptPIODevice->pio_oe &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_edge_event |= ulMSK;
      s_tIODriver.ptPIODevice->pio_in_inv |= ulMSK;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_RISING:
      (void) DRV_IRQ_Disable();
      s_tIODriver.ptPIODevice->pio_oe &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_edge_event |= ulMSK;
      s_tIODriver.ptPIODevice->pio_in_inv &= ~ulMSK;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_HIGH:
      (void) DRV_IRQ_Disable();
      s_tIODriver.ptPIODevice->pio_oe &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_edge_event &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_in_inv &= ~ulMSK;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_LOW:
      (void) DRV_IRQ_Disable();
      s_tIODriver.ptPIODevice->pio_oe &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_edge_event &= ~ulMSK;
      s_tIODriver.ptPIODevice->pio_in_inv |= ulMSK;
      (void) DRV_IRQ_Enable();
      ret = DRV_OK;
      break;
#endif
    default:
      ret = DRV_NSUPP;
      break;
    }
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    switch (eMode)
    {
    case DRV_DIO_MODE_SET:
      DRV_DIO_SetMMIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_RESET:
      DRV_DIO_ResetMMIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_READ:
      DRV_DIO_ReadModeMMIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
      ret = DRV_OK;
      break;
    default:
      ret = DRV_NSUPP;
      break;
    }
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    switch (eMode)
    {
    case DRV_DIO_MODE_SET:
      DRV_DIO_SetHIF((uint64_t) (0x1ull << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_RESET:
      DRV_DIO_ResetHIF((uint64_t) (0x1ull << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_READ:
      DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
      ret = DRV_OK;
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_HIGH:
      switch (ulChannelID)
      {
      case DRV_DIO_ID_HIF_DIRQ:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_DIRQ - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_dirq_cfg = DRV_DIO_HIF_IRQ_MODE_HIGH;
        break;
      case DRV_DIO_ID_HIF_A17:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A17 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a17_cfg = DRV_DIO_HIF_IRQ_MODE_HIGH;
        break;
      case DRV_DIO_ID_HIF_A16:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A16 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a16_cfg = DRV_DIO_HIF_IRQ_MODE_HIGH;
        break;
      case DRV_DIO_ID_HIF_D12:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_D12 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_d12_cfg = DRV_DIO_HIF_IRQ_MODE_HIGH;
        break;
      default:
        ret = DRV_NSUPP;
        break;
      }
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_RISING:
      switch (ulChannelID)
      {
      case DRV_DIO_ID_HIF_DIRQ:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_DIRQ - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_dirq_cfg = DRV_DIO_HIF_IRQ_MODE_RISING;
        break;
      case DRV_DIO_ID_HIF_A17:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A17 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a17_cfg = DRV_DIO_HIF_IRQ_MODE_RISING;
        break;
      case DRV_DIO_ID_HIF_A16:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A16 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a16_cfg = DRV_DIO_HIF_IRQ_MODE_RISING;
        break;
      case DRV_DIO_ID_HIF_D12:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_D12 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_d12_cfg = DRV_DIO_HIF_IRQ_MODE_RISING;
        break;
      default:
        ret = DRV_NSUPP;
        break;
      }
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_LOW:
      switch (ulChannelID)
      {
      case DRV_DIO_ID_HIF_DIRQ:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_DIRQ - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_dirq_cfg = DRV_DIO_HIF_IRQ_MODE_LOW;
        break;
      case DRV_DIO_ID_HIF_A17:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A17 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a17_cfg = DRV_DIO_HIF_IRQ_MODE_LOW;
        break;
      case DRV_DIO_ID_HIF_A16:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A16 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a16_cfg = DRV_DIO_HIF_IRQ_MODE_LOW;
        break;
      case DRV_DIO_ID_HIF_D12:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_D12 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_d12_cfg = DRV_DIO_HIF_IRQ_MODE_LOW;
        break;
      default:
        ret = DRV_NSUPP;
        break;
      }
      break;
    case DRV_DIO_MODE_CAPTURE_CONT_FALLING:
      switch (ulChannelID)
      {
      case DRV_DIO_ID_HIF_DIRQ:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_DIRQ - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_dirq_cfg = DRV_DIO_HIF_IRQ_MODE_FALLING;
        break;
      case DRV_DIO_ID_HIF_A17:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A17 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a17_cfg = DRV_DIO_HIF_IRQ_MODE_FALLING;
        break;
      case DRV_DIO_ID_HIF_A16:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_A16 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_a16_cfg = DRV_DIO_HIF_IRQ_MODE_FALLING;
        break;
      case DRV_DIO_ID_HIF_D12:
        DRV_DIO_ReadModeHIF((uint64_t) (0x1ull << (DRV_DIO_ID_HIF_D12 - DRV_DIO_ID_HIF_MIN)));
        s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.irq_hif_d12_cfg = DRV_DIO_HIF_IRQ_MODE_FALLING;
        break;
      default:
        ret = DRV_NSUPP;
        break;
      }
      break;
    default:
      ret = DRV_NSUPP;
      break;
    }
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 * Multiple calls will toggle the inversion.
 * Inversion is only available on GPIO for the nx90 engineering sample and for the final silicon
 * also on the pio pins. It might be available also on the MMIO, but not yet.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in] ulChannelID The id of the channel to be configured
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelInvert(DRV_DIO_ID_T ulChannelID)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    (void) DRV_IRQ_Disable();
    s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].inv ^= 0x1ul;
    (void) DRV_IRQ_Enable();
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
#ifdef DRV_DIO_PIO_INVERT_SUPPORTED
    (void) DRV_IRQ_Disable();
    s_tIODriver.ptPIODevice->pio_in_inv ^= 0x1ul;
    (void) DRV_IRQ_Enable();
    ret = DRV_OK;
#else
    ret = DRV_NSUPP;
#endif
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    //access key protected
    // (void) DRV_IRQ_Disable();
    // ptIODriver.ptMMIODevice->cfg[ulChannelID - DRV_DIO_MMIO_MIN_ID].bf.mmio_out_inv ^= 0x1u;
    // ptIODriver.ptMMIODevice->cfg[ulChannelID - DRV_DIO_MMIO_MIN_ID].bf.mmio_in_inv ^= 0x1u;
    // (void) DRV_IRQ_Enable();
    // ptIODriver.eError = DRV_OK;
    ret = DRV_NSUPP;
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    ret = DRV_NSUPP;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 * The channel out set function sets the mode of the channel specified.
 * If the logic is positive a channel will be set to high mode.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in] ulChannelID  The id of the channel to be set
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelOutSet(DRV_DIO_ID_T ulChannelID)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_SET;
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    DRV_DIO_SetPIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN)));
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    DRV_DIO_SetMMIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    DRV_DIO_SetHIF((uint64_t) (0x1ull << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
    ret = DRV_OK;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 * The channel is put into reset. Reset means, that the value that it has
 * will be a binary zero, or low level. If the channel is inverted, it will
 * show the inverted output on the outside. This means, it will drive the
 * channel to high and so the channel will appear set.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in] ulChannelID  The id of the channel to be reset
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelOutReset(DRV_DIO_ID_T ulChannelID)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_RESET;
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    DRV_DIO_ResetPIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN)));
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    DRV_DIO_ResetMMIO((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    DRV_DIO_ResetHIF((uint64_t) (0x1ull << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
    ret = DRV_OK;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 *
 * Calling Toggle on a channel neither set or reset will modify the channels mode to reset. It is also possible to call
 * DRV_DIO_PinInvert for toggling a channel that was already set or reset.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in] ulChannelID  The id of the channel to be toggled
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelOutToggle(DRV_DIO_ID_T ulChannelID)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    uint32_t in;
    (void) DRV_IRQ_Disable();
    in = s_tIODriver.ptGPIODevice->gpio_app_cfg[ulChannelID - DRV_DIO_ID_GPIO_MIN];
    if((in & gpio_app_gpio_app_cfg_mode_Msk) == (DRV_DIO_MODE_SET << gpio_app_gpio_app_cfg_mode_Pos))
    {
      s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_RESET;
      ret = DRV_OK;
    }
    else if((in & gpio_app_gpio_app_cfg_mode_Msk) == (DRV_DIO_MODE_RESET << gpio_app_gpio_app_cfg_mode_Pos))
    {
      s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_SET;
      ret = DRV_OK;
    }
    else
    {
      ret = DRV_ERROR;
    }
    (void) DRV_IRQ_Enable();
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    (void) DRV_IRQ_Disable();
    s_tIODriver.ptPIODevice->pio_out ^= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN)));
    s_tIODriver.ptPIODevice->pio_oe |= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN)));
    (void) DRV_IRQ_Enable();
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    (void) DRV_IRQ_Disable();
    s_tIODriver.ptMMIODevice->mmio_pio_out_line_cfg0_b.line ^= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
    s_tIODriver.ptMMIODevice->mmio_pio_oe_line_set_cfg0_b.line = ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN)));
    (void) DRV_IRQ_Enable();
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    (void) DRV_IRQ_Disable();
    if((ulChannelID - DRV_DIO_ID_HIF_MIN) < 32u)
    {
      s_tIODriver.ptHIFIODevice->hif_pio_out0 ^= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
      s_tIODriver.ptHIFIODevice->hif_pio_oe0 |= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_HIF_MIN)));
    }
    else
    {
      s_tIODriver.ptHIFIODevice->hif_pio_out1 ^= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_HIF_MIN - 32)));
      s_tIODriver.ptHIFIODevice->hif_pio_oe1 |= ((uint32_t) (0x1ul << (ulChannelID - DRV_DIO_ID_HIF_MIN - 32)));
    }
    (void) DRV_IRQ_Enable();
    ret = DRV_OK;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 * The function does not switch the chosen channel into read mode.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  ulChannelID  The id of the channel to be captured
 * \param[out] peState The state of the channel
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelInGet(DRV_DIO_ID_T ulChannelID, DRV_DIO_STATE_E * const peState)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_ERROR;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    *peState =
      ((s_tIODriver.ptGPIODevice->gpio_app_in & (0x01ul << (ulChannelID - DRV_DIO_ID_GPIO_MIN))) != 0) ? DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    *peState = ((s_tIODriver.ptPIODevice->pio_in & (0x01ul << (ulChannelID - DRV_DIO_ID_PIO_MIN))) != 0) ? DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_MMIO_MAX && ulChannelID >= DRV_DIO_ID_MMIO_MIN)
  {
    *peState =
      ((s_tIODriver.ptMMIODevice->mmio_in_line_status0_b.line & (0x01ul << (ulChannelID - DRV_DIO_ID_MMIO_MIN))) != 0) ?
        DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    ret = DRV_OK;
  }
  else if(ulChannelID <= DRV_DIO_ID_HIF_MAX && ulChannelID >= DRV_DIO_ID_HIF_MIN)
  {
    s_tIODriver.ptHIFIODevice->hif_pio_cfg_b.in_ctrl = 0x1ul;
    if((ulChannelID - DRV_DIO_ID_HIF_MIN) < 32)
    {
      *peState =
        ((s_tIODriver.ptHIFIODevice->hif_pio_in0 & (0x01ul << (ulChannelID - DRV_DIO_ID_HIF_MIN))) != 0) ? DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    }
    else
    {
      *peState =
        ((s_tIODriver.ptHIFIODevice->hif_pio_in1 & (0x01ul << (ulChannelID - (DRV_DIO_ID_HIF_MIN + 32)))) != 0) ?
          DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    }
    ret = DRV_OK;
  }
  else if(ulChannelID == DRV_DIO_ID_BOD)
  {
    *peState = (DRV_ASIC_CTRL_DEVICE->asic_ctrl_irq_raw_b.bod_fail == 0) ? DRV_DIO_STATE_SET : DRV_DIO_STATE_RESET;
    ret = DRV_OK;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  return ret;
}

/*!
 * This function sets the blink mode, initializes the blinking sequence, the speed and counter reference.
 *
 * The blink mode of an dio needs a counter so it will be moved out of this driver in the near future
 *
 * \memberof DRV_DIO_HANDLE_T
 * \private
 * \param[in]  ulChannelID   The id of the channel to be captured
 * \param[in]  sequence The blinking sequence bit encoded
 * \param[in]  seqBitLength   The state length (time to next bit) [0..31]
 * \param[in]  cntr     The counter used as reference. [0..3]
 * \param[in]  once     Boolean indicator if it schould run the sequence once. [0..1] (1=true)
 */
DRV_STATUS_E DRV_DIO_ChannelBlink(DRV_DIO_ID_T ulChannelID, unsigned int sequence, unsigned int seqBitLength, unsigned int cntr, unsigned int once)
{
  DRV_STATUS_E ret = DRV_ERROR;
  DRV_HANDLE_CHECK(s_tIODriver);
  if(seqBitLength > 0x1fu || cntr > 0x3u || once > 0x1ul || ulChannelID > DRV_DIO_ID_GPIO_MAX)
  {
    return DRV_ERROR_PARAM;
  }
  memset(&(s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN]), 0, 4);
  s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].blink_len = seqBitLength;
  s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].blink_once = once;
  s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].count_ref = cntr;
  s_tIODriver.ptGPIODevice->gpio_app_cfg_b[ulChannelID - DRV_DIO_ID_GPIO_MIN].mode = DRV_DIO_MODE_BLINK;
  s_tIODriver.ptGPIODevice->gpio_app_tc[ulChannelID - DRV_DIO_ID_GPIO_MIN] = sequence;
  return ret;
}

/*!
 * DRV_DIO_ID_GPIO_0..7
 * DRV_DIO_ID_PIO_0..28
 * DRV_DIO_ID_HIF_D12
 * DRV_DIO_ID_HIF_A16
 * DRV_DIO_ID_HIF_A17
 * DRV_DIO_ID_HIF_DIRQ
 *
 * \warning XPic IRQ functionality of HIF IO is not supported.
 *
 * \public
 * \param[in]  ulChannelID   The id of the channel to mask out
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelIRQMask(DRV_DIO_ID_T ulChannelID)
{
  (void) DRV_IRQ_Disable();
  DRV_STATUS_E ret = DRV_OK;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    DRV_GPIO_DEVICE->gpio_app_irq_mask_rst = 0x1ul << (ulChannelID - DRV_DIO_ID_GPIO_MIN);
  }
#ifdef DRV_DIO_PIO_IRQ_SUPPORTED
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    DRV_PIO_DEVICE->pio_irq0_msk_reset = 0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN);
  }
#endif
  else
  {
    switch (ulChannelID)
    {
    case DRV_DIO_ID_HIF_D12:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_reset = 0x1u;
      break;
    case DRV_DIO_ID_HIF_A16:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_reset = 0x2u;
      break;
    case DRV_DIO_ID_HIF_A17:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_reset = 0x4u;
      break;
    case DRV_DIO_ID_HIF_DIRQ:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_reset = 0x8u;
      break;
    case DRV_DIO_ID_BOD:
      DRV_ASIC_CTRL_DEVICE->asic_ctrl_irq_mask_reset = asic_ctrl_asic_ctrl_irq_mask_reset_bod_fail_Pos;
      break;
    default:
      ret = DRV_ERROR_PARAM;
      break;
    }
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 * DRV_DIO_ID_GPIO_0..7
 * DRV_DIO_ID_PIO_0..28
 * DRV_DIO_ID_HIF_D12
 * DRV_DIO_ID_HIF_A16
 * DRV_DIO_ID_HIF_A17
 * DRV_DIO_ID_HIF_DIRQ
 *
 * \warning XPic IRQ functionality of HIF IO is not supported.
 *
 * \public
 * \param[in]  ulChannelID   The id of the channel to mask out
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelIRQUnmask(DRV_DIO_ID_T ulChannelID)
{
  (void) DRV_IRQ_Disable();
  DRV_STATUS_E ret = DRV_OK;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    DRV_GPIO_DEVICE->gpio_app_irq_mask_set = 0x1ul << (ulChannelID - DRV_DIO_ID_GPIO_MIN);
  }
#ifdef DRV_DIO_PIO_IRQ_SUPPORTED
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    DRV_PIO_DEVICE->pio_irq0_msk_set = 0x1ul << (ulChannelID - DRV_DIO_ID_PIO_MIN);
  }
#endif
  else
  {
    switch (ulChannelID)
    {
    case DRV_DIO_ID_HIF_D12:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_set = 0x1u;
      break;
    case DRV_DIO_ID_HIF_A16:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_set = 0x2u;
      break;
    case DRV_DIO_ID_HIF_A17:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_set = 0x4u;
      break;
    case DRV_DIO_ID_HIF_DIRQ:
      DRV_HIF_IO_DEVICE->hif_pio_irq_arm_mask_set = 0x8u;
      break;
    case DRV_DIO_ID_BOD:
      DRV_ASIC_CTRL_DEVICE->asic_ctrl_irq_mask_set = asic_ctrl_asic_ctrl_irq_mask_reset_bod_fail_Pos;
      break;
    default:
      ret = DRV_ERROR_PARAM;
      break;
    }
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 *
 * \warning xPic IRQ functionality of HIF IO is not supported.
 *
 * The ISR are defined as weak functions. It is possible to define them dominant or use
 * this Attach function to set them dynamically. EOI-handling and source clearing not necessary.
 *
 * DRV_DIO_ID_GPIO_0..7
 * DRV_DIO_ID_PIO_0..28
 * DRV_DIO_ID_HIF_D12
 * DRV_DIO_ID_HIF_A16
 * DRV_DIO_ID_HIF_A17
 * DRV_DIO_ID_HIF_DIRQ
 * DRV_DIO_ID_BOD
 *
 * \public
 * \param[in]  ulChannelID    The id of the channel to attach an interrupt handler to
 * \param[in]  pfnUserClb     The interrupt handler to attach
 * \param[in]  pvUser         The handle of the interrupt handler
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_ChannelIRQAttach(DRV_DIO_ID_T ulChannelID, DRV_CALLBACK_F pfnUserClb, void* pvUser)
{
  (void) DRV_IRQ_Disable();
  DRV_STATUS_E ret = DRV_OK;
  if(ulChannelID <= DRV_DIO_ID_GPIO_MAX && ulChannelID >= DRV_DIO_ID_GPIO_MIN)
  {
    s_afnIRQHandler[ulChannelID - DRV_DIO_ID_GPIO_MIN] = pfnUserClb;
    s_apIRQHandlerHandles[ulChannelID - DRV_DIO_ID_GPIO_MIN] = pvUser;
    DRV_NVIC_ClearPendingIRQ((IRQn_Type) (DRV_GPIO0_IRQ + ulChannelID - DRV_DIO_ID_GPIO_MIN));
    if(pfnUserClb != 0)
    {
      DRV_NVIC_EnableIRQ((IRQn_Type) (DRV_GPIO0_IRQ + ulChannelID - DRV_DIO_ID_GPIO_MIN));
    }
    else
    {
      DRV_NVIC_DisableIRQ((IRQn_Type) (DRV_GPIO0_IRQ + ulChannelID - DRV_DIO_ID_GPIO_MIN));
    }
  }
#ifdef DRV_DIO_PIO_IRQ_SUPPORTED
  else if(ulChannelID <= DRV_DIO_ID_PIO_MAX && ulChannelID >= DRV_DIO_ID_PIO_MIN)
  {
    s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT + DRV_BOD_IRQ_COUNT + ulChannelID - DRV_DIO_ID_PIO_MIN] = pfnUserClb;
    s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT + DRV_BOD_IRQ_COUNT + ulChannelID - DRV_DIO_ID_PIO_MIN] = pvUser;
    DRV_NVIC_ClearPendingIRQ(DRV_PIO_IRQ);
    DRV_NVIC_EnableIRQ(DRV_PIO_IRQ);
  }
#endif
  else
  {
    switch (ulChannelID)
    {
    case DRV_DIO_ID_HIF_D12:
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT] = pfnUserClb;
      s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT] = pvUser;
      DRV_NVIC_ClearPendingIRQ(DRV_HIFPIO_IRQ);
      DRV_NVIC_EnableIRQ(DRV_HIFPIO_IRQ);
      break;
    case DRV_DIO_ID_HIF_A16:
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 1] = pfnUserClb;
      s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 1] = pvUser;
      DRV_NVIC_ClearPendingIRQ(DRV_HIFPIO_IRQ);
      DRV_NVIC_EnableIRQ(DRV_HIFPIO_IRQ);
      break;
    case DRV_DIO_ID_HIF_A17:
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 2] = pfnUserClb;
      s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 2] = pvUser;
      DRV_NVIC_ClearPendingIRQ(DRV_HIFPIO_IRQ);
      DRV_NVIC_EnableIRQ(DRV_HIFPIO_IRQ);
      break;
    case DRV_DIO_ID_HIF_DIRQ:
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 3] = pfnUserClb;
      s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 3] = pvUser;
      DRV_NVIC_ClearPendingIRQ(DRV_HIFPIO_IRQ);
      DRV_NVIC_EnableIRQ(DRV_HIFPIO_IRQ);
      break;
    case DRV_DIO_ID_BOD:
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT] = pfnUserClb;
      s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT] = pvUser;
      DRV_NVIC_ClearPendingIRQ(DRV_BOD_IRQ);
      if(pfnUserClb != 0)
      {
        DRV_NVIC_EnableIRQ(DRV_BOD_IRQ);
      }
      else
      {
        DRV_NVIC_DisableIRQ(DRV_BOD_IRQ);
      }
      break;
    default:
      ret = DRV_ERROR_PARAM;
      break;
    }
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 *
 * The filter evaluates a 10ns window by majority.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_HifIRQFilterEnable(void)
{
  DRV_HIF_IO_DEVICE->hif_pio_cfg_b.filter_irqs = 1u;
  return DRV_OK;
}

/*!
 *
 * The filter evaluates a 10ns window by majority.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_HifIRQFilterDisable(void)
{
  DRV_HIF_IO_DEVICE->hif_pio_cfg_b.filter_irqs = 0u;
  return DRV_OK;
}

/*!
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be modified
 * \param[in]  ullMask      The channel mask
 * \param[in]  ullChannels      The channels to be modified
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineMaskedSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullMask, DRV_DIO_MSK_T ullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  uint64_t tmp;
  (void) DRV_IRQ_Disable();
  if(eLine == DRV_DIO_LINE_HIF)
  {
    tmp = s_tIODriver.ptHIFIODevice->hif_pio_out0 & ~(ullMask);
    s_tIODriver.ptHIFIODevice->hif_pio_out0 = tmp | (ullChannels & DRV_DIO_LINE_MSK_HIF & ullMask);
    tmp = s_tIODriver.ptHIFIODevice->hif_pio_out1 & ~(ullMask >> 32u);
    s_tIODriver.ptHIFIODevice->hif_pio_out1 = tmp | ((ullChannels & DRV_DIO_LINE_MSK_HIF & ullMask) >> 32u);
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    tmp = s_tIODriver.ptGPIODevice->gpio_app_line & ~ullMask;
    s_tIODriver.ptGPIODevice->gpio_app_line = tmp | (ullChannels & DRV_DIO_LINE_MSK_GPIO & ullMask);
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    tmp = s_tIODriver.ptPIODevice->pio_out & ~ullMask;
    s_tIODriver.ptPIODevice->pio_out = tmp | (ullChannels & DRV_DIO_LINE_MSK_PIO & ullMask);
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    tmp = s_tIODriver.ptMMIODevice->mmio_pio_out_line_cfg0 & ~ullMask;
    s_tIODriver.ptMMIODevice->mmio_pio_out_line_cfg0 = tmp | (ullChannels & DRV_DIO_LINE_MSK_MMIO & ullMask);
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be set
 * \param[in]  ullChannels      The channels to be set
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineOutSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  (void) DRV_IRQ_Disable();
  if(eLine == DRV_DIO_LINE_HIF)
  {
    s_tIODriver.ptHIFIODevice->hif_pio_out0 |= (ullChannels & DRV_DIO_LINE_MSK_HIF );
    s_tIODriver.ptHIFIODevice->hif_pio_out1 |= ((ullChannels & DRV_DIO_LINE_MSK_HIF ) >> 32u);
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    s_tIODriver.ptGPIODevice->gpio_app_line |= (ullChannels & DRV_DIO_LINE_MSK_GPIO );
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    s_tIODriver.ptPIODevice->pio_out |= (ullChannels & DRV_DIO_LINE_MSK_PIO );
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    s_tIODriver.ptMMIODevice->mmio_pio_out_line_set_cfg0 = (ullChannels & DRV_DIO_LINE_MSK_MMIO );
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }

  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be reset
 * \param[in]  ullChannels      The channels to be reset
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineOutReset(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  (void) DRV_IRQ_Disable();
  if(eLine == DRV_DIO_LINE_HIF)
  {
    s_tIODriver.ptHIFIODevice->hif_pio_out0 &= ~(ullChannels & DRV_DIO_LINE_MSK_HIF );
    s_tIODriver.ptHIFIODevice->hif_pio_out1 &= ~((ullChannels & DRV_DIO_LINE_MSK_HIF ) >> 32u);
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    s_tIODriver.ptGPIODevice->gpio_app_line &= ~(ullChannels & DRV_DIO_LINE_MSK_GPIO );
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    s_tIODriver.ptPIODevice->pio_out &= ~(ullChannels & DRV_DIO_LINE_MSK_PIO );
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    s_tIODriver.ptMMIODevice->mmio_pio_out_line_reset_cfg0 = (ullChannels & DRV_DIO_LINE_MSK_MMIO );
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 * On DIO the mode will be set to line.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be changed
 * \param[in]  ullChannels      The channels to be output enabled
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineOutEnableSet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  (void) DRV_IRQ_Disable();
  if(eLine == DRV_DIO_LINE_HIF)
  {
    s_tIODriver.ptHIFIODevice->hif_pio_oe0 |= (ullChannels & DRV_DIO_LINE_MSK_HIF );
    s_tIODriver.ptHIFIODevice->hif_pio_oe1 |= ((ullChannels & DRV_DIO_LINE_MSK_HIF ) >> 32u);
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    int i = 0;
    unsigned int ch = (ullChannels & DRV_DIO_LINE_MSK_GPIO );
    for(; i < 8; i++)
    {
      if((ch & 0x01u << i) > 0)
      {
        s_tIODriver.ptGPIODevice->gpio_app_cfg[i] = DRV_DIO_MODE_LINE;
      }
    }
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    s_tIODriver.ptPIODevice->pio_oe |= (ullChannels & DRV_DIO_LINE_MSK_PIO );
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    s_tIODriver.ptMMIODevice->mmio_pio_oe_line_set_cfg0 = (ullChannels & DRV_DIO_LINE_MSK_MMIO );
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }

  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 * On DIO the mode will be set to read.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be changed
 * \param[in]  ullChannels      The channels to be output enabled
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineOutEnableReset(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T ullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  (void) DRV_IRQ_Disable();
  if(eLine == DRV_DIO_LINE_HIF)
  {
    s_tIODriver.ptHIFIODevice->hif_pio_oe0 &= ~(ullChannels & DRV_DIO_LINE_MSK_HIF );
    s_tIODriver.ptHIFIODevice->hif_pio_oe1 &= ~((ullChannels & DRV_DIO_LINE_MSK_HIF ) >> 32u);
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    int i = 0;
    unsigned int ch = (ullChannels & DRV_DIO_LINE_MSK_GPIO );
    for(; i < 8; i++)
    {
      if((ch & 0x01u << i) > 0)
      {
        s_tIODriver.ptGPIODevice->gpio_app_cfg[i] = DRV_DIO_MODE_READ;
      }
    }
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    s_tIODriver.ptPIODevice->pio_oe &= ~(ullChannels & DRV_DIO_LINE_MSK_PIO );
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    s_tIODriver.ptMMIODevice->mmio_pio_oe_line_reset_cfg0 = (ullChannels & DRV_DIO_LINE_MSK_MMIO );
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 * The lines input state is written to the given location.
 *
 * \memberof DRV_DIO_HANDLE_T
 * \public
 * \param[in]  eLine     The line to be read
 * \param[out] pullChannels     The channels captured state
 * \return An error code describing the current status of the driver.
 */
DRV_STATUS_E DRV_DIO_LineInGet(DRV_DIO_LINE_E eLine, DRV_DIO_MSK_T * const pullChannels)
{
  DRV_HANDLE_CHECK(s_tIODriver);
  DRV_STATUS_E ret = DRV_OK;
  (void) DRV_IRQ_Disable();
  (*pullChannels) = 0x00ull;
  if(eLine == DRV_DIO_LINE_HIF)
  {
    (*pullChannels) = (uint64_t) s_tIODriver.ptHIFIODevice->hif_pio_in1;
    (*pullChannels) <<= 32;
    (*pullChannels) |= (uint64_t) s_tIODriver.ptHIFIODevice->hif_pio_in0;
  }
  else if(eLine == DRV_DIO_LINE_GPIO)
  {
    *pullChannels = s_tIODriver.ptGPIODevice->gpio_app_in;
  }
  else if(eLine == DRV_DIO_LINE_PIO)
  {
    *pullChannels = s_tIODriver.ptPIODevice->pio_in;
  }
  else if(eLine == DRV_DIO_LINE_MMIO)
  {
    *pullChannels = s_tIODriver.ptMMIODevice->mmio_in_line_status0;
  }
  else
  {
    ret = DRV_ERROR_PARAM;
  }
  (void) DRV_IRQ_Enable();
  return ret;
}

/*!
 * This is the interrupt service routine of the GPIO 0 overwriting the weak definition in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO0_IRQHandler(void)
{
  DRV_DIO_GPIO0_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app0_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO0_Callback(void)
{
  static unsigned const int nr = 0;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_0, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 1 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO1_IRQHandler(void)
{
  DRV_DIO_GPIO1_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app1_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO1_Callback(void)
{
  static unsigned const int nr = 1;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_1, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 2 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO2_IRQHandler(void)
{
  DRV_DIO_GPIO2_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app2_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO2_Callback(void)
{
  static unsigned const int nr = 2;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_2, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 3 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO3_IRQHandler(void)
{
  DRV_DIO_GPIO3_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app3_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO3_Callback(void)
{
  static unsigned const int nr = 3;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_3, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 4 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO4_IRQHandler(void)
{
  DRV_DIO_GPIO4_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app4_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO4_Callback(void)
{
  static unsigned const int nr = 4;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_4, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 5 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO5_IRQHandler(void)
{
  DRV_DIO_GPIO5_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app5_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO5_Callback(void)
{
  static unsigned const int nr = 5;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_5, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 6 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO6_IRQHandler(void)
{
  DRV_DIO_GPIO6_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app6_Msk;
}

/*!
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO6_Callback(void)
{
  static unsigned const int nr = 6;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_6, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the GPIO 7 overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void GPIO7_IRQHandler(void)
{
  DRV_DIO_GPIO7_Callback();
  DRV_GPIO_DEVICE->gpio_app_irq_raw = gpio_app_gpio_app_irq_mask_rst_gpio_app7_Msk;
}

/*!
 *
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_GPIO7_Callback(void)
{
  static unsigned const int nr = 7;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_GPIO_7, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the BOD overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void ASIC_IRQHandler(void)
{
  DRV_DIO_BOD_Callback();
  DRV_ASIC_CTRL_DEVICE->asic_ctrl_irq_raw = asic_ctrl_asic_ctrl_irq_raw_bod_fail_Pos;
}

/*!
 *
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 */
__WEAK void DRV_DIO_BOD_Callback(void)
{
  static unsigned const int nr = 12;
  if(s_afnIRQHandler[nr] != 0)
  {
    s_afnIRQHandler[nr]((void*) DRV_DIO_ID_BOD, s_apIRQHandlerHandles[nr]);
  }
}

/*!
 * This is the interrupt service routine of the HIFPIO overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void HIFPIO_IRQHandler(void)
{
  DRV_DIO_HIFPIO_Callback();
  DRV_HIF_IO_DEVICE->hif_pio_irq_raw = 0xFFFFFFFF;
}

/*!
 *
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 *
 */
__WEAK void DRV_DIO_HIFPIO_Callback(void)
{
  if(DRV_HIF_IO_DEVICE->hif_pio_irq_arm_masked_b.irq_hif_d12)
  {
    if(s_afnIRQHandler[DRV_GPIO_IRQ_COUNT] != 0)
    {
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT]((void*) DRV_DIO_ID_HIF_D12, s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT]);
    }
    DRV_HIF_IO_DEVICE->hif_pio_irq_raw = hif_io_ctrl_hif_pio_irq_raw_irq_hif_d12_Msk;
  }
  if(DRV_HIF_IO_DEVICE->hif_pio_irq_arm_masked_b.irq_hif_a16)
  {
    if(s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 1] != 0)
    {
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 1]((void*) DRV_DIO_ID_HIF_A16, s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 1]);
    }
    DRV_HIF_IO_DEVICE->hif_pio_irq_raw = hif_io_ctrl_hif_pio_irq_raw_irq_hif_a16_Msk;
  }
  if(DRV_HIF_IO_DEVICE->hif_pio_irq_arm_masked_b.irq_hif_a17)
  {
    if(s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 2] != 0)
    {
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 2]((void*) DRV_DIO_ID_HIF_A17, s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 2]);
    }
    DRV_HIF_IO_DEVICE->hif_pio_irq_raw = hif_io_ctrl_hif_pio_irq_raw_irq_hif_a17_Msk;
  }
  if(DRV_HIF_IO_DEVICE->hif_pio_irq_arm_masked_b.irq_hif_dirq)
  {
    if(s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 3] != 0)
    {
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + 3]((void*) DRV_DIO_ID_HIF_DIRQ, s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT + 3]);
    }
    DRV_HIF_IO_DEVICE->hif_pio_irq_raw = hif_io_ctrl_hif_pio_irq_raw_irq_hif_dirq_Msk;
  }
}

#ifdef DRV_DIO_PIO_IRQ_SUPPORTED

/*!
 * This is the interrupt service routine of the HIFPIO overwriting the weak implementation in the CMSIS.
 *
 * The ISR is implemented as an weak, empty function in the CMSIS startup file. This IRQ handler calls the
 * callback which is also implemented weak but calls an user defined function attached to this IRQ by
 * \ref DRV_DIO_ChannelIRQAttach.
 */
void PIO0_IRQHandler(void)
{
  DRV_DIO_PIO_Callback();
}

/*!
 *
 * It is called from the Interrupt service routine. And calls the function attached to the interrupt by
 * \ref DRV_DIO_ChannelIRQAttach() , if there is one. Because the implementation has the attribute weak it
 * is possible to implement the callback inside the application context.
 * EOI-handling and source clearing not necessary.
 *
 */
__WEAK void DRV_DIO_PIO_Callback(void)
{
  uint32_t ulIRQ = DRV_PIO_DEVICE->pio_irq_raw & DRV_PIO_DEVICE->pio_irq0_masked;
  uint32_t ulChannelID = 0;
  uint32_t ulMSK = 0x01u;
  while(ulIRQ != 0)
  {
    if(ulIRQ & ulMSK)
    {
      s_afnIRQHandler[DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT + DRV_BOD_IRQ_COUNT + ulChannelID]((void*) ((uintptr_t) DRV_DIO_ID_PIO_0 + ulChannelID), s_apIRQHandlerHandles[DRV_GPIO_IRQ_COUNT
        + DRV_HIF_IRQ_COUNT + DRV_BOD_IRQ_COUNT + ulChannelID]
      );
      DRV_PIO_DEVICE->pio_irq_raw = ulMSK;
      ulIRQ &= ~ulMSK;
    }
    ulMSK = ulMSK << 1;
    if(DRV_PIO_IRQ_COUNT == ulChannelID++)
    {
      break;
    }
  }
}

#endif /* DRV_DIO_PIO_IRQ_SUPPORTED */

/*! \} *//* End of group DIO */

/*lint -restore */
/*lint -restore */

#endif /* DRV_DIO_MODULE_DISABLED */
