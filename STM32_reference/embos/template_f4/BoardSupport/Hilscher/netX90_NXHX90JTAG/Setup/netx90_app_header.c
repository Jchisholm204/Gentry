/**************************************************************************//**
 * @file    netx90_app_hboot_header.c
 * @brief   netx90 app hboot header
 * $Revision: 6562 $
 * $Date: 2019-12-06 16:57:23 +0100 (Fr, 06 Dez 2019) $
 ******************************************************************************/

/**************************************************************************************
 Exclusion of Liability for this demo software:
 The following software is intended for and must only be used for reference and in an
 evaluation laboratory environment. It is provided without charge and is subject to
 alterations. There is no warranty for the software, to the extent permitted by
 applicable law. Except when otherwise stated in writing the copyright holders and/or
 other parties provide the software "as is" without warranty of any kind, either
 expressed or implied.
 Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
 By installing or otherwise using the software, you accept the terms of this Agreement.
 If you do not agree to the terms of this Agreement, then do not install or use the
 Software!
 **************************************************************************************/

/**************************************************************************************

 Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.


 **************************************************************************************/
#include <stdio.h>

#include "netx_drv.h"

NETX_CPU_BOOTBLOCK_T app_cpu_hboot_header_nai __attribute__ ((section (".app_cpu_hboot_boot_header_nai"))) =
{
  .ulMagic = NETX_CPU_BOOTBLOCK_MAGIC,
  .ulFlashOffsetBytes = 0,
  .pulNextHeader = 0,
  .pulDestination = 0,
  .ulImageSizeDword = 0,
  .ulFlashSelection = 0,
  .ulSignature = NETX_CPU_BOOTBLOCK_SIGNATURE,
  .ulReserved0 = 0,
  .aulHash = { 0 },
  .ulBootHeaderChecksum = 0,
};

NETX_CPU_BOOTBLOCK_T app_cpu_hboot_header_nae __attribute__ ((section (".app_cpu_hboot_boot_header_nae"))) =
{
  .ulMagic = NETX_CPU_BOOTBLOCK_MAGIC,
  .ulFlashOffsetBytes = 0,
  .pulNextHeader = 0,
  .pulDestination = 0,
  .ulImageSizeDword = 0,
  .ulFlashSelection = 0,
  .ulSignature = NETX_CPU_BOOTBLOCK_SIGNATURE,
  .ulReserved0 = 0,
  .aulHash = { 0 },
  .ulBootHeaderChecksum = 0,
};

void print_app_hboot_header(NETX_CPU_BOOTBLOCK_T* ptHeader)
{
  printf("\nprint_app_hboot_header\n");
  printf("Pointer: 0x%08x\n", (uintptr_t) ptHeader);
  printf("  .ulMagic               = 0x%08x\n", (unsigned int) ptHeader->ulMagic);
  printf("  .ulFlashOffsetBytes    = 0x%08x\n", (unsigned int) ptHeader->ulFlashOffsetBytes);
  printf("  .pulNextHeader         = 0x%08x\n", (unsigned int) ptHeader->pulNextHeader);
  printf("  .pulDestination        = 0x%08x\n", (unsigned int) ptHeader->pulDestination);
  printf("  .ulImageSizeDword      = 0x%08x\n", (unsigned int) ptHeader->ulImageSizeDword);
  printf("  .ulFlashSelection      = 0x%08x\n", (unsigned int) ptHeader->ulFlashSelection);
  printf("  .ulSignature           = 0x%08x\n", (unsigned int) ptHeader->ulSignature);
  printf("  .aulHash               = 0x%08x\n", (unsigned int) ptHeader->aulHash[0]);
  printf("  .ulBootHeaderChecksum  = 0x%08x\n", (unsigned int) ptHeader->ulBootHeaderChecksum);
}
