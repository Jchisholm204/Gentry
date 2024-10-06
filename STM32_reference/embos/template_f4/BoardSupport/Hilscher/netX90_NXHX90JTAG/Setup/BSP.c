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
File    : BSP.c
Purpose : BSP for Hilscher NXHX 90-JTAG eval board.
*/

#include "BSP.h"
#include "netx_drv.h"

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  DRV_DIO_Init();
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  switch (Index) {
    case 0:
      DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_4);
      break;
    case 1:
      DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_5);
      break;
    case 2:
      DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_6);
      break;
    case 3:
      DRV_DIO_ChannelOutSet(DRV_DIO_ID_MMIO_7);
      break;
    default:
      break;
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  switch (Index) {
    case 0:
      DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_4);
      break;
    case 1:
      DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_5);
      break;
    case 2:
      DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_6);
      break;
    case 3:
      DRV_DIO_ChannelOutReset(DRV_DIO_ID_MMIO_7);
      break;
    default:
      break;
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  switch (Index) {
    case 0:
      DRV_DIO_ChannelOutToggle(DRV_DIO_ID_MMIO_4);
      break;
    case 1:
      DRV_DIO_ChannelOutToggle(DRV_DIO_ID_MMIO_5);
      break;
    case 2:
      DRV_DIO_ChannelOutToggle(DRV_DIO_ID_MMIO_6);
      break;
    case 3:
      DRV_DIO_ChannelOutToggle(DRV_DIO_ID_MMIO_7);
      break;
    default:
      break;
  }
}

/*************************** End of file ****************************/
