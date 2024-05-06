/*!************************************************************************//*!
 * \file    csp/driver_csp_netx90_app.h
 * \brief   Header file supporting the final netX90 chip.
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
#ifndef DRIVER_CSP_NETX90_APP_H_
#define DRIVER_CSP_NETX90_APP_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup netX_Driver
 * \{
 */

/*!
 * \addtogroup CSP Chip Support Package
 * \{
 */

/*!
 * \addtogroup NETX90 netX90 Final Silicon
 * \{
 */

/*lint -save -esym(18,*) */
#include "netx90_app.h"
/*lint -restore */
#define DRV_CORTEX_MODULE_SUPPORTED
#define DRV_BISS_MODULE_SUPPORTED
#define DRV_CANCTRL_MODULE_SUPPORTED
#define DRV_XPIC_MODULE_SUPPORTED
#define DRV_ETH_XPIC_MODULE_SUPPORTED

typedef asic_ctrl_Type DRV_ASIC_CTRL_T;
#define DRV_ASIC_CTRL_DEVICE            ((DRV_ASIC_CTRL_T*)         asic_ctrl_BASE)

#define DRV_MLED_MODULE_SUPPORTED
typedef mled_ctrl_app_Type DRV_MLED_DEVICE_T;
#define DRV_MLED_DEVICE                            ((DRV_MLED_DEVICE_T*)        mled_ctrl_app_BASE)
#define DRV_MLED_DEVICE_COUNT 1
#define DRV_MLED_DEVICE_LIST { DRV_MLED_DEVICE }

#define DRV_MLED_MSK_0         (uint32_t)0x00000001ull/*!< MLED 0 */
#define DRV_MLED_MSK_1         (uint32_t)0x00000002ull/*!< MLED 1 */
#define DRV_MLED_MSK_2         (uint32_t)0x00000004ull/*!< MLED 2 */
#define DRV_MLED_MSK_3         (uint32_t)0x00000008ull/*!< MLED 3 */
#define DRV_MLED_MSK_4         (uint32_t)0x00000010ull/*!< MLED 4 */
#define DRV_MLED_MSK_5         (uint32_t)0x00000020ull/*!< MLED 5 */
#define DRV_MLED_MSK_6         (uint32_t)0x00000040ull/*!< MLED 6 */
#define DRV_MLED_MSK_7         (uint32_t)0x00000080ull/*!< MLED 7 */
#define DRV_MLED_MSK_8         (uint32_t)0x00000100ull/*!< MLED 8 */
#define DRV_MLED_MSK_9         (uint32_t)0x00000200ull/*!< MLED 9 */
#define DRV_MLED_MSK_10        (uint32_t)0x00000400ull/*!< MLED 10 */
#define DRV_MLED_MSK_11        (uint32_t)0x00000800ull/*!< MLED 11 */
#define DRV_MLED_MSK_12        (uint32_t)0x00001000ull/*!< MLED 12 */
#define DRV_MLED_MSK_13        (uint32_t)0x00002000ull/*!< MLED 13 */
#define DRV_MLED_MSK_14        (uint32_t)0x00004000ull/*!< MLED 14 */
#define DRV_MLED_MSK_15        (uint32_t)0x00008000ull/*!< MLED 15 */
#define DRV_MLED_LINE_MSK      (uint32_t)0x0000FFFFull/*!< MMIO line selected.*/

#define DRV_MLED_ID_MIN DRV_MLED_ID_0/*!< The lowest MMIO channel id.*/
#define DRV_MLED_ID_0    0x000000u/*!< MLED channel 0 ID.*/
#define DRV_MLED_ID_1    0x000001u/*!< MLED channel 1 ID.*/
#define DRV_MLED_ID_2    0x000002u/*!< MLED channel 2 ID.*/
#define DRV_MLED_ID_3    0x000003u/*!< MLED channel 3 ID.*/
#define DRV_MLED_ID_4    0x000004u/*!< MLED channel 4 ID.*/
#define DRV_MLED_ID_5    0x000005u/*!< MLED channel 5 ID.*/
#define DRV_MLED_ID_6    0x000006u/*!< MLED channel 6 ID.*/
#define DRV_MLED_ID_7    0x000007u/*!< MLED channel 7 ID.*/
#define DRV_MLED_ID_8    0x000008u/*!< MLED channel 8 ID.*/
#define DRV_MLED_ID_9    0x000009u/*!< MLED channel 9 ID.*/
#define DRV_MLED_ID_10   0x00000au/*!< MLED channel 10 ID.*/
#define DRV_MLED_ID_11   0x00000bu/*!< MLED channel 11 ID.*/
#define DRV_MLED_ID_12   0x00000cu/*!< MLED channel 12 ID.*/
#define DRV_MLED_ID_13   0x00000du/*!< MLED channel 13 ID.*/
#define DRV_MLED_ID_14   0x00000eu/*!< MLED channel 14 ID.*/
#define DRV_MLED_ID_15   0x00000fu/*!< MLED channel 15 ID.*/
#define DRV_MLED_ID_MAX DRV_MLED_ID_15/*!< The highest MMIO channel id.*/

#define DRV_DIO_MODULE_SUPPORTED
typedef mmio_ctrl_Type DRV_MMIO_DEVICE_T;
typedef hif_io_ctrl_Type DRV_HIF_IO_DEVICE_T;
typedef gpio_app_Type DRV_GPIO_DEVICE_T;
typedef pio_app_Type DRV_PIO_DEVICE_T;
#define DRV_MMIO_DEVICE                            ((DRV_MMIO_DEVICE_T*)            mmio_ctrl_BASE)
#define DRV_HIF_IO_DEVICE                          ((DRV_HIF_IO_DEVICE_T*)        hif_io_ctrl_BASE)
#define DRV_GPIO_DEVICE                            ((DRV_GPIO_DEVICE_T*)             gpio_app_BASE)
#define DRV_GPIO_XPIC_DEVICE                       ((DRV_GPIO_DEVICE_T*)        gpio_xpic_app_BASE)
#define DRV_PIO_DEVICE                             ((DRV_PIO_DEVICE_T*)               pio_app_BASE)
#define DRV_DIO_PIO_INVERT_SUPPORTED
#define DRV_DIO_PIO_IRQ_SUPPORTED
#define DRV_GPIO_IRQ_COUNT 8
#define DRV_GPIO_IRQ_LIST gpio_app0_IRQn, gpio_app1_IRQn, gpio_app2_IRQn, gpio_app3_IRQn, gpio_app4_IRQn, gpio_app5_IRQn, gpio_app6_IRQn, gpio_app7_IRQn
#define DRV_GPIO0_IRQ gpio_app0_IRQn
#define DRV_HIF_IRQ_COUNT 4
#define DRV_HIFPIO_IRQ hif_pio_arm_IRQn
#define DRV_PIO_IRQ_COUNT 29
#define DRV_PIO_IRQ pio0_app_IRQn
#define DRV_BOD_IRQ bod_IRQn
#define DRV_BOD_IRQ_COUNT 1
#define DRV_DIO_IRQ_COUNT DRV_GPIO_IRQ_COUNT + DRV_HIF_IRQ_COUNT + DRV_BOD_IRQ_COUNT + DRV_PIO_IRQ_COUNT

/*!
 * The available output lines.
 *
 * Each device has its own line registers.
 */
#define DRV_DIO_LINE_MSK_NONE          (uint64_t)0x0000000000000000ull/*!< No line selected.*/
#define DRV_DIO_LINE_MSK_GPIO          (uint64_t)0x00000000000000FFull/*!< GPIO line select.*/
#define DRV_DIO_LINE_MSK_PIO           (uint64_t)0x000000001FFFFFFFull/*!< PIO line select.*/
#define DRV_DIO_LINE_MSK_MMIO          (uint64_t)0x00000000000000FFull/*!< MMIO line selected.*/
#define DRV_DIO_LINE_MSK_HIF           (uint64_t)0xFE03FFFF0000FFFFull/*!< HIF line select.*/

/*!
 * The available IO channels by bit position usable as mask.
 */
#define DRV_DIO_MSK_GPIO_0         (uint64_t)0x0000000000000001ull/*!< GPIO 0 */
#define DRV_DIO_MSK_GPIO_1         (uint64_t)0x0000000000000002ull/*!< GPIO 1 */
#define DRV_DIO_MSK_GPIO_2         (uint64_t)0x0000000000000004ull/*!< GPIO 2 */
#define DRV_DIO_MSK_GPIO_3         (uint64_t)0x0000000000000008ull/*!< GPIO 3 */
#define DRV_DIO_MSK_GPIO_4         (uint64_t)0x0000000000000010ull/*!< GPIO 4 */
#define DRV_DIO_MSK_GPIO_5         (uint64_t)0x0000000000000020ull/*!< GPIO 5 */
#define DRV_DIO_MSK_GPIO_6         (uint64_t)0x0000000000000040ull/*!< GPIO 6 */
#define DRV_DIO_MSK_GPIO_7         (uint64_t)0x0000000000000080ull/*!< GPIO 7 */

#define DRV_DIO_MSK_PIO_0          (uint64_t)0x0000000000000001ull/*!< PIO 0 */
#define DRV_DIO_MSK_PIO_1          (uint64_t)0x0000000000000002ull/*!< PIO 1 */
#define DRV_DIO_MSK_PIO_2          (uint64_t)0x0000000000000004ull/*!< PIO 2 */
#define DRV_DIO_MSK_PIO_3          (uint64_t)0x0000000000000008ull/*!< PIO 3 */
#define DRV_DIO_MSK_PIO_4          (uint64_t)0x0000000000000010ull/*!< PIO 4 */
#define DRV_DIO_MSK_PIO_5          (uint64_t)0x0000000000000020ull/*!< PIO 5 */
#define DRV_DIO_MSK_PIO_6          (uint64_t)0x0000000000000040ull/*!< PIO 6 */
#define DRV_DIO_MSK_PIO_7          (uint64_t)0x0000000000000080ull/*!< PIO 7 */
#define DRV_DIO_MSK_PIO_8          (uint64_t)0x0000000000000100ull/*!< PIO 8 */
#define DRV_DIO_MSK_PIO_9          (uint64_t)0x0000000000000200ull/*!< PIO 9 */
#define DRV_DIO_MSK_PIO_10         (uint64_t)0x0000000000000400ull/*!< PIO 10 */
#define DRV_DIO_MSK_PIO_11         (uint64_t)0x0000000000000800ull/*!< PIO 11 */
#define DRV_DIO_MSK_PIO_12         (uint64_t)0x0000000000001000ull/*!< PIO 12 */
#define DRV_DIO_MSK_PIO_13         (uint64_t)0x0000000000002000ull/*!< PIO 13 */
#define DRV_DIO_MSK_PIO_14         (uint64_t)0x0000000000004000ull/*!< PIO 14 */
#define DRV_DIO_MSK_PIO_15         (uint64_t)0x0000000000008000ull/*!< PIO 15 */
#define DRV_DIO_MSK_PIO_16         (uint64_t)0x0000000000010000ull/*!< PIO 16 */
#define DRV_DIO_MSK_PIO_17         (uint64_t)0x0000000000020000ull/*!< PIO 17 */
#define DRV_DIO_MSK_PIO_18         (uint64_t)0x0000000000040000ull/*!< PIO 18 */
#define DRV_DIO_MSK_PIO_19         (uint64_t)0x0000000000080000ull/*!< PIO 19 */
#define DRV_DIO_MSK_PIO_20         (uint64_t)0x0000000000100000ull/*!< PIO 20 */
#define DRV_DIO_MSK_PIO_21         (uint64_t)0x0000000000200000ull/*!< PIO 21 */
#define DRV_DIO_MSK_PIO_22         (uint64_t)0x0000000000400000ull/*!< PIO 22 */
#define DRV_DIO_MSK_PIO_23         (uint64_t)0x0000000000800000ull/*!< PIO 23 */
#define DRV_DIO_MSK_PIO_24         (uint64_t)0x0000000001000000ull/*!< PIO 24 */
#define DRV_DIO_MSK_PIO_25         (uint64_t)0x0000000002000000ull/*!< PIO 25 */
#define DRV_DIO_MSK_PIO_26         (uint64_t)0x0000000004000000ull/*!< PIO 26 */
#define DRV_DIO_MSK_PIO_27         (uint64_t)0x0000000008000000ull/*!< PIO 27 */
#define DRV_DIO_MSK_PIO_28         (uint64_t)0x0000000010000000ull/*!< PIO 28 */

#define DRV_DIO_MSK_MMIO_0         (uint64_t)0x0000000000000001ull/*!< MMIO 0 */
#define DRV_DIO_MSK_MMIO_1         (uint64_t)0x0000000000000002ull/*!< MMIO 1 */
#define DRV_DIO_MSK_MMIO_2         (uint64_t)0x0000000000000004ull/*!< MMIO 2 */
#define DRV_DIO_MSK_MMIO_3         (uint64_t)0x0000000000000008ull/*!< MMIO 3 */
#define DRV_DIO_MSK_MMIO_4         (uint64_t)0x0000000000000010ull/*!< MMIO 4 */
#define DRV_DIO_MSK_MMIO_5         (uint64_t)0x0000000000000020ull/*!< MMIO 5 */
#define DRV_DIO_MSK_MMIO_6         (uint64_t)0x0000000000000040ull/*!< MMIO 6 */
#define DRV_DIO_MSK_MMIO_7         (uint64_t)0x0000000000000080ull/*!< MMIO 7 */

#define DRV_DIO_MSK_HIF_D0         (uint64_t)0x0000000000000001ull/*!< HIF D0 */
#define DRV_DIO_MSK_HIF_D1         (uint64_t)0x0000000000000002ull/*!< HIF D1 */
#define DRV_DIO_MSK_HIF_D2         (uint64_t)0x0000000000000004ull/*!< HIF D2 */
#define DRV_DIO_MSK_HIF_D3         (uint64_t)0x0000000000000008ull/*!< HIF D3 */
#define DRV_DIO_MSK_HIF_D4         (uint64_t)0x0000000000000010ull/*!< HIF D4 */
#define DRV_DIO_MSK_HIF_D5         (uint64_t)0x0000000000000020ull/*!< HIF D5 */
#define DRV_DIO_MSK_HIF_D6         (uint64_t)0x0000000000000040ull/*!< HIF D6 */
#define DRV_DIO_MSK_HIF_D7         (uint64_t)0x0000000000000080ull/*!< HIF D7 */
#define DRV_DIO_MSK_HIF_D8         (uint64_t)0x0000000000000100ull/*!< HIF D8 */
#define DRV_DIO_MSK_HIF_D9         (uint64_t)0x0000000000000200ull/*!< HIF D9 */
#define DRV_DIO_MSK_HIF_D10        (uint64_t)0x0000000000000400ull/*!< HIF D10 */
#define DRV_DIO_MSK_HIF_D11        (uint64_t)0x0000000000000800ull/*!< HIF D11 */
#define DRV_DIO_MSK_HIF_D12        (uint64_t)0x0000000000001000ull/*!< HIF D12 */
#define DRV_DIO_MSK_HIF_D13        (uint64_t)0x0000000000002000ull/*!< HIF D13 */
#define DRV_DIO_MSK_HIF_D14        (uint64_t)0x0000000000004000ull/*!< HIF D14 */
#define DRV_DIO_MSK_HIF_D15        (uint64_t)0x0000000000008000ull/*!< HIF D15 */
#define DRV_DIO_MSK_HIF_D_RESERVED (uint64_t)0x00000000FFFF0000ull/*!< RESERVED */

#define DRV_DIO_MSK_HIF_A0         (uint64_t)0x0000000100000000ull/*!< HIF A0 */
#define DRV_DIO_MSK_HIF_A1         (uint64_t)0x0000000200000000ull/*!< HIF A1 */
#define DRV_DIO_MSK_HIF_A2         (uint64_t)0x0000000400000000ull/*!< HIF A2 */
#define DRV_DIO_MSK_HIF_A3         (uint64_t)0x0000000800000000ull/*!< HIF A3 */
#define DRV_DIO_MSK_HIF_A4         (uint64_t)0x0000001000000000ull/*!< HIF A4 */
#define DRV_DIO_MSK_HIF_A5         (uint64_t)0x0000002000000000ull/*!< HIF A5 */
#define DRV_DIO_MSK_HIF_A6         (uint64_t)0x0000004000000000ull/*!< HIF A6 */
#define DRV_DIO_MSK_HIF_A7         (uint64_t)0x0000008000000000ull/*!< HIF A7 */
#define DRV_DIO_MSK_HIF_A8         (uint64_t)0x0000010000000000ull/*!< HIF A8 */
#define DRV_DIO_MSK_HIF_A9         (uint64_t)0x0000020000000000ull/*!< HIF A9 */
#define DRV_DIO_MSK_HIF_A10        (uint64_t)0x0000040000000000ull/*!< HIF A10 */
#define DRV_DIO_MSK_HIF_A11        (uint64_t)0x0000080000000000ull/*!< HIF A11 */
#define DRV_DIO_MSK_HIF_A12        (uint64_t)0x0000100000000000ull/*!< HIF A12 */
#define DRV_DIO_MSK_HIF_A13        (uint64_t)0x0000200000000000ull/*!< HIF A13 */
#define DRV_DIO_MSK_HIF_A14        (uint64_t)0x0000400000000000ull/*!< HIF A14 */
#define DRV_DIO_MSK_HIF_A15        (uint64_t)0x0000800000000000ull/*!< HIF A15 */
#define DRV_DIO_MSK_HIF_A16        (uint64_t)0x0001000000000000ull/*!< HIF A16 */
#define DRV_DIO_MSK_HIF_A17        (uint64_t)0x0002000000000000ull/*!< HIF A17 */
#define DRV_DIO_MSK_HIF_A_RESERVED (uint64_t)0x01fc000000000000ull/*!< RESERVED */
#define DRV_DIO_MSK_HIF_BHEN       (uint64_t)0x0200000000000000ull/*!< HIF BHEN */
#define DRV_DIO_MSK_HIF_RDN        (uint64_t)0x0400000000000000ull/*!< HIF RDN */
#define DRV_DIO_MSK_HIF_WRN        (uint64_t)0x0800000000000000ull/*!< HIF WRN */
#define DRV_DIO_MSK_HIF_CSN        (uint64_t)0x1000000000000000ull/*!< HIF CSN */
#define DRV_DIO_MSK_HIF_RDY        (uint64_t)0x2000000000000000ull/*!< HIF RDY */
#define DRV_DIO_MSK_HIF_DIRQ       (uint64_t)0x4000000000000000ull/*!< HIF DIRQ */
#define DRV_DIO_MSK_HIF_SDCLK      (uint64_t)0x8000000000000000ull/*!< HIF SDCLK */

/*!
 * The available io channels by id.
 */
#define DRV_DIO_ID_GPIO_MIN DRV_DIO_ID_GPIO_0 /*!< The lowest GPIO channel id.*/
#define DRV_DIO_ID_GPIO_0    0x100000u/*!< GPIO channel 0 ID.*/
#define DRV_DIO_ID_GPIO_1    0x100001u/*!< GPIO channel 1 ID.*/
#define DRV_DIO_ID_GPIO_2    0x100002u/*!< GPIO channel 2 ID.*/
#define DRV_DIO_ID_GPIO_3    0x100003u/*!< GPIO channel 3 ID.*/
#define DRV_DIO_ID_GPIO_4    0x100004u/*!< GPIO channel 4 ID.*/
#define DRV_DIO_ID_GPIO_5    0x100005u/*!< GPIO channel 5 ID.*/
#define DRV_DIO_ID_GPIO_6    0x100006u/*!< GPIO channel 6 ID.*/
#define DRV_DIO_ID_GPIO_7    0x100007u/*!< GPIO channel 7 ID.*/
#define DRV_DIO_ID_GPIO_MAX DRV_DIO_ID_GPIO_7/*!< The highest GPIO channel id.*/

#define DRV_DIO_ID_PIO_MIN DRV_DIO_ID_PIO_0/*!< The lowest PIO channel id.*/
#define DRV_DIO_ID_PIO_0     0x200000u/*!< PIO channel 0 ID.*/
#define DRV_DIO_ID_PIO_1     0x200001u/*!< PIO channel 1 ID.*/
#define DRV_DIO_ID_PIO_2     0x200002u/*!< PIO channel 2 ID.*/
#define DRV_DIO_ID_PIO_3     0x200003u/*!< PIO channel 3 ID.*/
#define DRV_DIO_ID_PIO_4     0x200004u/*!< PIO channel 4 ID.*/
#define DRV_DIO_ID_PIO_5     0x200005u/*!< PIO channel 5 ID.*/
#define DRV_DIO_ID_PIO_6     0x200006u/*!< PIO channel 6 ID.*/
#define DRV_DIO_ID_PIO_7     0x200007u/*!< PIO channel 7 ID.*/
#define DRV_DIO_ID_PIO_8     0x200008u/*!< PIO channel 8 ID.*/
#define DRV_DIO_ID_PIO_9     0x200009u/*!< PIO channel 9 ID.*/
#define DRV_DIO_ID_PIO_10    0x20000au/*!< PIO channel 10 ID.*/
#define DRV_DIO_ID_PIO_11    0x20000bu/*!< PIO channel 11 ID.*/
#define DRV_DIO_ID_PIO_12    0x20000cu/*!< PIO channel 12 ID.*/
#define DRV_DIO_ID_PIO_13    0x20000du/*!< PIO channel 13 ID.*/
#define DRV_DIO_ID_PIO_14    0x20000eu/*!< PIO channel 14 ID.*/
#define DRV_DIO_ID_PIO_15    0x20000fu/*!< PIO channel 15 ID.*/
#define DRV_DIO_ID_PIO_16    0x200010u/*!< PIO channel 16 ID.*/
#define DRV_DIO_ID_PIO_17    0x200011u/*!< PIO channel 17 ID.*/
#define DRV_DIO_ID_PIO_18    0x200012u/*!< PIO channel 18 ID.*/
#define DRV_DIO_ID_PIO_19    0x200013u/*!< PIO channel 19 ID.*/
#define DRV_DIO_ID_PIO_20    0x200014u/*!< PIO channel 20 ID.*/
#define DRV_DIO_ID_PIO_21    0x200015u/*!< PIO channel 21 ID.*/
#define DRV_DIO_ID_PIO_22    0x200016u/*!< PIO channel 22 ID.*/
#define DRV_DIO_ID_PIO_23    0x200017u/*!< PIO channel 23 ID.*/
#define DRV_DIO_ID_PIO_24    0x200018u/*!< PIO channel 24 ID.*/
#define DRV_DIO_ID_PIO_25    0x200019u/*!< PIO channel 25 ID.*/
#define DRV_DIO_ID_PIO_26    0x20001au/*!< PIO channel 26 ID.*/
#define DRV_DIO_ID_PIO_27    0x20001bu/*!< PIO channel 27 ID.*/
#define DRV_DIO_ID_PIO_28    0x20001cu/*!< PIO channel 28 ID.*/
#define DRV_DIO_ID_PIO_MAX DRV_DIO_ID_PIO_28/*!< The highest PIO channel id.*/

#define DRV_DIO_ID_MMIO_MIN DRV_DIO_ID_MMIO_0/*!< The lowest MMIO channel id.*/
#define DRV_DIO_ID_MMIO_0    0x300000u/*!< MMIO channel 0 ID.*/
#define DRV_DIO_ID_MMIO_1    0x300001u/*!< MMIO channel 1 ID.*/
#define DRV_DIO_ID_MMIO_2    0x300002u/*!< MMIO channel 2 ID.*/
#define DRV_DIO_ID_MMIO_3    0x300003u/*!< MMIO channel 3 ID.*/
#define DRV_DIO_ID_MMIO_4    0x300004u/*!< MMIO channel 4 ID.*/
#define DRV_DIO_ID_MMIO_5    0x300005u/*!< MMIO channel 5 ID.*/
#define DRV_DIO_ID_MMIO_6    0x300006u/*!< MMIO channel 6 ID.*/
#define DRV_DIO_ID_MMIO_7    0x300007u/*!< MMIO channel 7 ID.*/
#define DRV_DIO_ID_MMIO_MAX DRV_DIO_ID_MMIO_7/*!< The highest MMIO channel id.*/

#define DRV_DIO_ID_HIF_MIN   DRV_DIO_ID_HIF_D0/*!< The lowest HIF channel id.*/
#define DRV_DIO_ID_HIF_D0    0x400000u/*!< HIF channel D0*/
#define DRV_DIO_ID_HIF_D1    0x400001u/*!< HIF channel D1*/
#define DRV_DIO_ID_HIF_D2    0x400002u/*!< HIF channel D2*/
#define DRV_DIO_ID_HIF_D3    0x400003u/*!< HIF channel D3*/
#define DRV_DIO_ID_HIF_D4    0x400004u/*!< HIF channel D4*/
#define DRV_DIO_ID_HIF_D5    0x400005u/*!< HIF channel D5*/
#define DRV_DIO_ID_HIF_D6    0x400006u/*!< HIF channel D6*/
#define DRV_DIO_ID_HIF_D7    0x400007u/*!< HIF channel D7*/
#define DRV_DIO_ID_HIF_D8    0x400008u/*!< HIF channel D8*/
#define DRV_DIO_ID_HIF_D9    0x400009u/*!< HIF channel D9*/
#define DRV_DIO_ID_HIF_D10   0x40000au/*!< HIF channel D10*/
#define DRV_DIO_ID_HIF_D11   0x40000bu/*!< HIF channel D11*/
#define DRV_DIO_ID_HIF_D12   0x40000cu/*!< HIF channel D12*/
#define DRV_DIO_ID_HIF_D13   0x40000du/*!< HIF channel D13*/
#define DRV_DIO_ID_HIF_D14   0x40000eu/*!< HIF channel D14*/
#define DRV_DIO_ID_HIF_D15   0x40000fu/*!< HIF channel D15*/
#define DRV_DIO_ID_HIF_RESERVED0
#define DRV_DIO_ID_HIF_A0    0x400020u/*!< HIF channel A0*/
#define DRV_DIO_ID_HIF_A1    0x400021u/*!< HIF channel A1*/
#define DRV_DIO_ID_HIF_A2    0x400022u/*!< HIF channel A2*/
#define DRV_DIO_ID_HIF_A3    0x400023u/*!< HIF channel A3*/
#define DRV_DIO_ID_HIF_A4    0x400024u/*!< HIF channel A4*/
#define DRV_DIO_ID_HIF_A5    0x400025u/*!< HIF channel A5*/
#define DRV_DIO_ID_HIF_A6    0x400026u/*!< HIF channel A6*/
#define DRV_DIO_ID_HIF_A7    0x400027u/*!< HIF channel A7*/
#define DRV_DIO_ID_HIF_A8    0x400028u/*!< HIF channel A8*/
#define DRV_DIO_ID_HIF_A9    0x400029u/*!< HIF channel A9*/
#define DRV_DIO_ID_HIF_A10   0x40002au/*!< HIF channel A10*/
#define DRV_DIO_ID_HIF_A11   0x40002bu/*!< HIF channel A11*/
#define DRV_DIO_ID_HIF_A12   0x40002cu/*!< HIF channel A12*/
#define DRV_DIO_ID_HIF_A13   0x40002du/*!< HIF channel A13*/
#define DRV_DIO_ID_HIF_A14   0x40002eu/*!< HIF channel A14*/
#define DRV_DIO_ID_HIF_A15   0x40002fu/*!< HIF channel A15*/
#define DRV_DIO_ID_HIF_A16   0x400030u/*!< HIF channel A16*/
#define DRV_DIO_ID_HIF_A17   0x400031u/*!< HIF channel A17*/
#define DRV_DIO_ID_HIF_RESERVED1
#define DRV_DIO_ID_HIF_BHEN  0x400039u/*!< HIF channel BHEN*/
#define DRV_DIO_ID_HIF_RDN   0x40003au/*!< HIF channel RDN*/
#define DRV_DIO_ID_HIF_WRN   0x40003bu/*!< HIF channel WRN*/
#define DRV_DIO_ID_HIF_CSN   0x40003cu/*!< HIF channel CSN*/
#define DRV_DIO_ID_HIF_RDY   0x40003du/*!< HIF channel RDY*/
#define DRV_DIO_ID_HIF_DIRQ  0x40003eu/*!< HIF channel DIRQ*/
#define DRV_DIO_ID_HIF_SDCLK 0x40003fu/*!< HIF channel SDCLK*/
#define DRV_DIO_ID_HIF_MAX   DRV_DIO_ID_HIF_SDCLK/*!< The highest HIF channel id.*/

#define DRV_DIO_ID_BOD   0x500070u/*!< Brown out ID.*/

#define DRV_TIM_MODULE_SUPPORTED
/*!
 * \brief Enumeration of the available timer IDs.
 */
typedef enum DRV_TIM_DEVICE_ID_Etag
{
  DRV_TIM_DEVICE_ID_GPIOCNTR0 = 0x10ul, /*!< GPIO COUNTER 0 ID */
  DRV_TIM_DEVICE_ID_GPIOCNTR1 = 0x11ul, /*!< GPIO COUNTER 1 ID */
  DRV_TIM_DEVICE_ID_GPIOCNTR2 = 0x12ul, /*!< GPIO COUNTER 2 ID */
  DRV_TIM_DEVICE_ID_GPIOCNTR_MIN = DRV_TIM_DEVICE_ID_GPIOCNTR0, /*!< GPIO COUNTER MIN ID */
  DRV_TIM_DEVICE_ID_GPIOCNTR_MAX = DRV_TIM_DEVICE_ID_GPIOCNTR2, /*!< GPIO COINTER MAX ID */

  DRV_TIM_DEVICE_ID_TIMER0 = 0x13ul, /*!< ARM TIMER 0 ID */
  DRV_TIM_DEVICE_ID_TIMER1 = 0x14ul, /*!< ARM TIMER 1 ID */
  DRV_TIM_DEVICE_ID_TIMER2 = 0x15ul, /*!< ARM TIMER 2 ID */
  DRV_TIM_DEVICE_ID_TIMER_MIN = DRV_TIM_DEVICE_ID_TIMER0, /*!< ARM TIMER MIN ID */
  DRV_TIM_DEVICE_ID_TIMER_MAX = DRV_TIM_DEVICE_ID_TIMER2, /*!< ARM TIMER MIN ID */

  DRV_TIM_DEVICE_ID_SYSTIMECOMPARE = 0x16ul, /*!< SYSTEM TIME COMPARATOR */
  DRV_TIM_DEVICE_ID_SYSTICK = 0x17ul, /*!< ARMSYSTEM TICK */

  DRV_TIM_DEVICE_ID_SYSTIME_COM = 0x18ul, /*!< SYSTIME COM ID */
  DRV_TIM_DEVICE_ID_SYSTIME_COM_UC = 0x19ul, /*!< SYSTIME COM UC ID */
  DRV_TIM_DEVICE_ID_SYSTIME_APP = 0x1aul, /*!< SYSTIME APP ID */
  DRV_TIM_DEVICE_ID_SYSTIME_MIN = DRV_TIM_DEVICE_ID_SYSTIME_COM, /*!< SYSTIME MIN ID */
  DRV_TIM_DEVICE_ID_SYSTIME_MAX = DRV_TIM_DEVICE_ID_SYSTIME_APP, /*!< SYSTIME MAX ID */

  DRV_TIM_DEVICE_ID_MIN = DRV_TIM_DEVICE_ID_GPIOCNTR0, /*!< MIN ID */
  DRV_TIM_DEVICE_ID_MAX = DRV_TIM_DEVICE_ID_SYSTIME_APP /*!< MAX ID */
} DRV_TIM_DEVICE_ID_E;
typedef timer_app_Type DRV_TIMER_DEVICE_T;
#define DRV_TIMER_DEVICE            ((DRV_TIMER_DEVICE_T*)         timer_app_BASE)
#define DRV_TIMER_XPIC_DEVICE       ((DRV_TIMER_DEVICE_T*)    timer_xpic_app_BASE)
typedef systime_lt_app_Type DRV_SYSTIME_LT_DEVICE_T;
#define DRV_SYSTIME_LT_DEVICE       ((DRV_SYSTIME_LT_DEVICE_T*)    systime_lt_app_BASE)
#define DRV_SYSTIME_LT_XPIC_DEVICE  ((DRV_SYSTIME_LT_DEVICE_T*)  systime_lt_xpic_app_BASE)
typedef systime_app_Type DRV_SYSTIME_DEVICE_T;
#define DRV_SYSTIME_DEVICE          ((systime_app_Type*)       systime_app_BASE)
#define DRV_GPIO_COUNTER_IRQs gpio_app_timer0_IRQn, gpio_app_timer1_IRQn, gpio_app_timer2_IRQn
#define DRV_TIMER_IRQs timer_app0_IRQn, timer_app1_IRQn, timer_app2_IRQn
#define DRV_SYSTIME_COMPARE_IRQ timer_app_systime_s_IRQn
#define DRV_SYSTICK_IRQ SysTick_IRQn
#define DRV_TIM_IRQ_COUNT 8u
#define DRV_TIM_IRQ_LIST { DRV_GPIO_COUNTER_IRQs, DRV_TIMER_IRQs, DRV_SYSTIME_COMPARE_IRQ, DRV_SYSTICK_IRQ }

#define DRV_DMAC_MODULE_SUPPORTED
/*!
 * Enumeration of the available dmac channels.
 */
typedef enum DRV_DMAC_DEVICE_ID_Etag
{

  DRV_DMAC_DEVICE_ID_0 = 0x8u,/*!< The DMAC drivers id for channel 0.*/
  DRV_DMAC_DEVICE_ID_1 = 0x9u,/*!< The DMAC drivers id for channel 1.*/
  DRV_DMAC_DEVICE_ID_2 = 0xau,/*!< The DMAC drivers id for channel 2.*/
  DRV_DMAC_DEVICE_ID_3 = 0xbu,/*!< The DMAC drivers id for channel 3.*/
  DRV_DMAC_DEVICE_ID_MIN = DRV_DMAC_DEVICE_ID_0, /*!< The ADC Sequencer drivers first device ID.*/
  DRV_DMAC_DEVICE_ID_MAX = DRV_DMAC_DEVICE_ID_3 /*!< The ADC Sequencer drivers last device ID.*/
} DRV_DMAC_DEVICE_ID_E;
typedef dmac_app_ch_Type DRV_DMAC_CH_DEVICE_T;
#define DMAC_APP_CH0_DEVICE         ((DRV_DMAC_CH_DEVICE_T*)      dmac_app_ch_BASE)
#define DMAC_APP_CH1_DEVICE         ((DRV_DMAC_CH_DEVICE_T*)      dmac_app_ch_BASE+1)
#define DMAC_APP_CH2_DEVICE         ((DRV_DMAC_CH_DEVICE_T*)      dmac_app_ch_BASE+2)
#define DMAC_APP_CH3_DEVICE         ((DRV_DMAC_CH_DEVICE_T*)      dmac_app_ch_BASE+3)
#define DRV_DMAC_CH_DEVICE_COUNT 4
#define DRV_DMAC_REG_IRQ dmac_app_IRQn
#define DRV_DMAC_CH_DEVICE_LIST { DMAC_APP_CH0_DEVICE, DMAC_APP_CH1_DEVICE, DMAC_APP_CH2_DEVICE, DMAC_APP_CH3_DEVICE }
typedef dmac_app_reg_Type DRV_DMAC_REG_DEVICE_T;
#define DMAC_APP_REG_DEVICE         ((DRV_DMAC_REG_DEVICE_T*)      dmac_app_reg_BASE)
typedef dmac_mux_app_Type DRV_DMAC_MUX_DEVICE_T;
#define DMAC_MUX_APP_DEVICE         ((DRV_DMAC_MUX_DEVICE_T*)      dmac_mux_app_BASE)
typedef enum DRV_DMAC_PERIPHERAL_Etag
{
  DRV_DMAC_PERIPHERAL_UART_SHARED_RX = 0u,
  DRV_DMAC_PERIPHERAL_UART_SHARED_TX = 1u,
  DRV_DMAC_PERIPHERAL_UART_APP_RX = 2u,
  DRV_DMAC_PERIPHERAL_UART_APP_TX = 3u,
  DRV_DMAC_PERIPHERAL_I2C_APP_MASTER = 4u,
  DRV_DMAC_PERIPHERAL_I2C_APP_SLAVE = 5u,
  DRV_DMAC_PERIPHERAL_SPI0_APP_RX = 6u,
  DRV_DMAC_PERIPHERAL_SPI0_APP_TX = 7u,
  DRV_DMAC_PERIPHERAL_SPI1_APP_RX = 8u,
  DRV_DMAC_PERIPHERAL_SPI1_APP_TX = 9u,
  DRV_DMAC_PERIPHERAL_SPI2_APP_RX = 10u,
  DRV_DMAC_PERIPHERAL_SPI2_APP_TX = 11u,
  DRV_DMAC_PERIPHERAL_SQI0_APP_RX = 12u,
  DRV_DMAC_PERIPHERAL_SQI0_APP_TX = 13u,
  DRV_DMAC_PERIPHERAL_SQI1_APP_RX = 14u,
  DRV_DMAC_PERIPHERAL_SQI1_APP_TX = 15u,
  DRV_DMAC_PERIPHERAL_UART_XPIC_APP_RX = 16u,
  DRV_DMAC_PERIPHERAL_UART_XPIC_APP_TX = 17u,
  DRV_DMAC_PERIPHERAL_I2C_XPIC_APP_MASTER = 18u,
  DRV_DMAC_PERIPHERAL_I2C_XPIC_APP_SLAVE = 19u,
  DRV_DMAC_PERIPHERAL_SPI_XPIC_APP_RX = 20u,
  DRV_DMAC_PERIPHERAL_SPI_XPIC_APP_TX = 21u,
  DRV_DMAC_PERIPHERAL_SQI_RX = 22u,
  DRV_DMAC_PERIPHERAL_SQI_TX = 23u,
  DRV_DMAC_PERIPHERAL_ETH_RX = 24u,
  DRV_DMAC_PERIPHERAL_ETH_TX = 25u,
  DRV_DMAC_PERIPHERAL_HASH = 26u,
  DRV_DMAC_PERIPHERAL_AES_IN = 27u,
  DRV_DMAC_PERIPHERAL_AES_OUT = 28u,
  DRV_DMAC_PERIPHERAL_NC = 29u,
  DRV_DMAC_PERIPHERAL_30_RESERVED = 30u,
  DRV_DMAC_PERIPHERAL_31_RESERVED = 31u,
  DRV_DMAC_PERIPHERAL_MEMORY = 32u
} DRV_DMAC_PERIPHERAL_E;

#define DRV_UART_MODULE_SUPPORTED
/*!
 * The UART device IDs.
 *
 * They are used to identify the spi devices in the driver context.
 */
typedef enum DRV_UART_DEVICE_ID_Etag
{
  DRV_UART_DEVICE_ID_UART0 = 0x07ul,/*!< The UART drivers internal id for UART of the APP.*/
  DRV_UART_DEVICE_ID_UART1 = 0x08ul,/*!< The UART drivers internal id for UART of the XPIC.*/
  DRV_UART_DEVICE_ID_UART2 = 0x09ul,/*!< The UART drivers internal id for UART of the SHARED.*/
  DRV_UART_DEVICE_ID_MIN = DRV_UART_DEVICE_ID_UART0, /*!< The UART drivers last device ID.*/
  DRV_UART_DEVICE_ID_MAX = DRV_UART_DEVICE_ID_UART2 /*!< The UART drivers last device ID.*/
} DRV_UART_DEVICE_ID_E;
typedef uart_app_Type DRV_UART_DEVICE_T;
#define UART_DEVICE_SHARED           ((DRV_UART_DEVICE_T*)          uart_BASE)
#define DRV_UART_IRQ_HANDLER0 UARTAPP_IRQHandler
#define UART_DEVICE_APP              ((DRV_UART_DEVICE_T*)          uart_app_BASE)
#define DRV_UART_IRQ_HANDLER1 UARTXPIC_IRQHandler
#define UART_DEVICE_XPIC_APP         ((DRV_UART_DEVICE_T*)     uart_xpic_app_BASE)
#define DRV_UART_IRQ_HANDLER2 UART_IRQHandler
#define DRV_UART_DEVICE_COUNT 3
#define DRV_UART_DEVICE_LIST { UART_DEVICE_APP, UART_DEVICE_XPIC_APP, UART_DEVICE_SHARED }
#define DRV_UART_DEVICE_IRQ_LIST { uart_app_IRQn, uart_xpic_app_IRQn, uart_IRQn }
#define DRV_UART_DEVICE_DMA_LIST { DRV_DMAC_PERIPHERAL_UART_APP_RX, DRV_DMAC_PERIPHERAL_UART_XPIC_APP_RX, DRV_DMAC_PERIPHERAL_UART_SHARED_RX }

#define DRV_SPI_MODULE_SUPPORTED

/*!
 * \brief The SPI device IDs.
 *
 * They are used to identify the spi devices in the driver context.
 */
typedef enum DRV_SPI_DEVICE_ID_Etag
{
  DRV_SPI_DEVICE_ID_DEFAULT = 0x00ul,/*!< The SPI drivers internal id for SPI 0.*/
  DRV_SPI_DEVICE_ID_SPI0 = 0x10ul,/*!< The SPI drivers internal id for SPI 0.*/
#define DRV_SPI_IRQ_HANDLER0 SPI0_IRQHandler
  DRV_SPI_DEVICE_ID_SPI1 = 0x11ul,/*!< The SPI drivers internal id for SPI 1.*/
#define DRV_SPI_IRQ_HANDLER1 SPI1_IRQHandler
  DRV_SPI_DEVICE_ID_SPI2 = 0x12ul,/*!< The SPI drivers internal id for SPI 2.*/
#define DRV_SPI_IRQ_HANDLER2 SPI2_IRQHandler
  DRV_SPI_DEVICE_ID_SPI3 = 0x13ul,/*!< The SPI drivers internal id for SPI of the XPIC.*/
#define DRV_SPI_IRQ_HANDLER3 SPIXPIC_IRQHandler
  DRV_SPI_DEVICE_ID_SQI0 = 0x14ul,/*!< The SPI drivers internal id for SQI SHARED.*/
#define DRV_SPI_IRQ_HANDLER4 SQI_IRQHandler
  DRV_SPI_DEVICE_ID_SQI1 = 0x15ul,/*!< The SPI drivers internal id for SQI0.*/
#define DRV_SPI_IRQ_HANDLER5 SQI0_IRQHandler
  DRV_SPI_DEVICE_ID_SQI2 = 0x16ul,/*!< The SPI drivers internal id for SQI1.*/
#define DRV_SPI_IRQ_HANDLER6 SQI1_IRQHandler
  DRV_SPI_DEVICE_ID_MIN = DRV_SPI_DEVICE_ID_SPI0,/*!< The SPI driver minimal value.*/
  DRV_SPI_DEVICE_ID_MAX = DRV_SPI_DEVICE_ID_SQI2, /*!< The SPI drivers last device ID.*/
  DRV_SPI_DEVICE_ID_SQI_BORDER = DRV_SPI_DEVICE_ID_SQI0,/*!< The border value from were a SPI device is an SQI device.*/
  DRV_SPI_DEVICE_ID_QSPI_BORDER = DRV_SPI_DEVICE_ID_MAX+1, /*!< The border value from were a SQI device is an QSPI device.*/
} DRV_SPI_DEVICE_ID_E;
typedef spi0_app_Type DRV_SPI_DEVICE_T;
typedef sqi_Type DRV_SQI_DEVICE_T;
typedef union DRV_SPI_DEVICE_Utag
{
  uintptr_t pt;
  DRV_SPI_DEVICE_T* ptSPI;
  DRV_SQI_DEVICE_T* ptSQI;
} DRV_SPI_DEVICE_U;
#define SPI_DEVICE_APP0             ((DRV_SPI_DEVICE_T*)          spi0_app_BASE)
#define SPI_DEVICE_APP1             ((DRV_SPI_DEVICE_T*)          spi1_app_BASE)
#define SPI_DEVICE_APP2             ((DRV_SPI_DEVICE_T*)          spi2_app_BASE)
#define SPI_DEVICE_APP3             ((DRV_SPI_DEVICE_T*)          spi_xpic_app_BASE)
#define SQI_DEVICE_SHARED           ((DRV_SQI_DEVICE_T*)          sqi_BASE)
#define SQI_DEVICE_APP0             ((DRV_SQI_DEVICE_T*)          sqi0_app_BASE)
#define SQI_DEVICE_APP1             ((DRV_SQI_DEVICE_T*)          sqi1_app_BASE)
#define DRV_SPI_DEVICE_COUNT 7
#define DRV_SPI_DEVICE_LIST { {(uintptr_t) SPI_DEVICE_APP0}, {(uintptr_t) SPI_DEVICE_APP1}, {(uintptr_t) SPI_DEVICE_APP2}, {(uintptr_t) SPI_DEVICE_APP3}, {(uintptr_t) SQI_DEVICE_SHARED}, {(uintptr_t) SQI_DEVICE_APP0}, {(uintptr_t) SQI_DEVICE_APP1}}
#define DRV_SPI_DEVICE_IRQ_LIST { spi0_app_IRQn, spi1_app_IRQn, spi2_app_IRQn, spi_xpic_app_IRQn, sqi_IRQn, sqi0_app_IRQn, sqi1_app_IRQn }
#define DRV_SPI_DEVICE_DMA_LIST { DRV_DMAC_PERIPHERAL_SPI0_APP_RX, DRV_DMAC_PERIPHERAL_SPI1_APP_RX, DRV_DMAC_PERIPHERAL_SPI2_APP_RX, DRV_DMAC_PERIPHERAL_SPI_XPIC_APP_RX, DRV_DMAC_PERIPHERAL_SQI_RX, DRV_DMAC_PERIPHERAL_SQI0_APP_RX, DRV_DMAC_PERIPHERAL_SQI1_APP_RX }

#define DRV_I2C_MODULE_SUPPORTED

/*!
 * The I2C device IDs.
 *
 * They are used to identify the I2C devices in the driver context.
 */
typedef enum DRV_I2C_Device_ID_Etag
{
  DRV_I2C_DEVICE_ID_I2C0 = 0x01ul,/*!< The I2C drivers internal id for I2C 0.*/
  DRV_I2C_DEVICE_ID_I2C1 = 0x02ul,/*!< The I2C drivers internal id for I2C of the XPIC.*/
  DRV_I2C_DEVICE_ID_MIN = DRV_I2C_DEVICE_ID_I2C0, /*!< The I2C drivers last device ID.*/
  DRV_I2C_DEVICE_ID_MAX = DRV_I2C_DEVICE_ID_I2C1 /*!< The I2C drivers last device ID.*/
} DRV_I2C_DEVICE_ID_E;
typedef i2c_app_Type DRV_I2C_DEVICE_T;
#define I2C_DEVICE_APP0              ((DRV_I2C_DEVICE_T*)          i2c_app_BASE)
#define DRV_I2C_IRQ_HANDLER0 I2C_IRQHandler
#define I2C_DEVICE_APP1              ((DRV_I2C_DEVICE_T*)          i2c_xpic_app_BASE)
#define DRV_I2C_IRQ_HANDLER1 I2CXPIC_IRQHandler
#define DRV_I2C_DEVICE_COUNT 2
#define DRV_I2C_DEVICE_LIST { I2C_DEVICE_APP0, I2C_DEVICE_APP1 }
#define DRV_I2C_DEVICE_IRQ_LIST { i2c_app_IRQn, i2c_xpic_app_IRQn }

/*!
 * The adc device IDs.
 *
 * They are used to identify the adc devices in the driver context.
 */
typedef enum DRV_ADC_SEQ_DEVICE_ID_Etag
{
  DRV_ADC_SEQ_DEVICE_ID_ADC0 = 0x10u,/*!< The ADC drivers internal id for ADC0.*/
  DRV_ADC_SEQ_DEVICE_ID_ADC1 = 0x11u,/*!< The ADC drivers internal id for ADC1.*/
  DRV_ADC_SEQ_DEVICE_ID_ADC2 = 0x12u,/*!< The ADC drivers internal id for ADC2.*/
  DRV_ADC_SEQ_DEVICE_ID_ADC3 = 0x13u,/*!< The ADC drivers internal id for ADC3.*/
  DRV_ADC_SEQ_DEVICE_ID_MIN = DRV_ADC_SEQ_DEVICE_ID_ADC0, /*!< Min value for boundary checks.*/
  DRV_ADC_SEQ_DEVICE_ID_MAX = DRV_ADC_SEQ_DEVICE_ID_ADC3 /*!< Max value for boundary checks.*/
} DRV_ADC_SEQ_DEVICE_ID_E;
typedef madc_Type DRV_ADC_DEVICE_T;
typedef madc_seq0_Type DRV_ADC_SEQ_DEVICE_T;

#define ADC_APP_DEVICE              ((DRV_ADC_DEVICE_T*)          madc_BASE)
#define ADC_SEQ0_APP_DEVICE         ((DRV_ADC_SEQ_DEVICE_T*)      madc_seq0_BASE)
#define ADC_SEQ1_APP_DEVICE         ((DRV_ADC_SEQ_DEVICE_T*)      madc_seq1_BASE)
#define ADC_SEQ2_APP_DEVICE         ((DRV_ADC_SEQ_DEVICE_T*)      madc_seq2_BASE)
#define ADC_SEQ3_APP_DEVICE         ((DRV_ADC_SEQ_DEVICE_T*)      madc_seq3_BASE)

#define DRV_ADC_DEVICE_COUNT 1
#define DRV_ADC_DEVICE_LIST { ADC_APP_DEVICE }

#define DRV_ADC_SEQ_DEVICE_COUNT 4
#define DRV_ADC_SEQ_DEVICE_LIST { ADC_SEQ0_APP_DEVICE, ADC_SEQ1_APP_DEVICE, ADC_SEQ2_APP_DEVICE, ADC_SEQ3_APP_DEVICE }
#define DRV_ADC_SEQ_DEVICE_IRQ_LIST { madc_seq0_IRQn, madc_seq1_IRQn, madc_seq2_IRQn, madc_seq3_IRQn }

#define DRV_ADC_COUNT 4
#define DRV_ADC_INPUTS_MAX 8
#define DRV_ADC_MEASUREMENTS_MAX 8
#define DRV_ADC_INPUT_COUNT01 4
#define DRV_ADC_INPUT_COUNT23 8

#define DRV_ADC_MODULE_SUPPORTED

/*!
 * The CPU device IDs.
 *
 * They are used to identify the processors.
 */
typedef enum DRV_MCP_CPU_ID_Etag
{
  DRV_MCP_CPU_ID_COM,
  DRV_MCP_CPU_ID_APP0,
  DRV_MCP_CPU_ID_APP1,
  DRV_MCP_CPU_ID_XPIC0,
  DRV_MCP_CPU_ID_XPIC1,
  DRV_MCP_CPU_ID_XPIC2,
  DRV_MCP_CPU_ID_XPIC3,
  DRV_MCP_CPU_ID_MAX
} DRV_MCP_CPU_ID_E;
typedef mcp_app_Type DRV_MCP_DEVICE_T;

#define DRV_MCP_CPU_COUNT 8

#define DRV_MCP_DEVICE            ((DRV_MCP_DEVICE_T*)           mcp_app_BASE)

/*! \} *//* End of group NETX90 */

/*! \} *//* End of group CSP */

/*! \} *//* End of group netx_Driver */

#ifdef __cplusplus
}
#endif
#endif /* DRIVER_CSP_NETX90_APP_H_ */
