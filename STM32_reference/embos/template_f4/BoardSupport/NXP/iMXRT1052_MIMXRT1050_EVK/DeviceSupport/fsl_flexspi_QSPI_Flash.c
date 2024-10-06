/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_flexspi_nor_flash.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(__CC_ARM) || defined(__GNUC__)
    __attribute__((section(".boot_hdr.conf"), used))
#elif defined(__ICCARM__)
#pragma location=".boot_hdr.conf"
#endif

//
// QSPI boot header
//
const flexspi_nor_config_t Serialflash_config = {
  .memConfig = {
    .tag = FLEXSPI_CFG_BLK_TAG,
    .version = FLEXSPI_CFG_BLK_VERSION,
    .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally,
    .csHoldTime = 3u,
    .csSetupTime = 3u,
    .columnAddressWidth = 0,
    .deviceModeCfgEnable = 0,
    .waitTimeCfgCommands = 0,
    .controllerMiscOption = 0,
    .deviceType = kFlexSpiDeviceType_SerialNOR,
    .sflashPadType = kSerialFlash_4Pads,
    .serialClkFreq = kFlexSpiSerialClk_50MHz,
    .lutCustomSeqEnable = 0,
    .sflashA1Size = 8u * 1024u * 1024u,
    .dataValidTime = {16u, 16u},
    .busyOffset = 1,
    .busyBitPolarity = 0,
    .lookupTable = {
      //
      // Configure LUT for read
      //
      [4 * NOR_CMD_LUT_SEQ_IDX_READ]   = FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x6B, RADDR_SDR, FLEXSPI_1PAD, 0x18),
      [4 * NOR_CMD_LUT_SEQ_IDX_READ+1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x08, READ_SDR,  FLEXSPI_4PAD, 0x04),
    },
  },
  .pageSize = 256u,
  .sectorSize = 4u * 1024u,
  .blockSize = 64u * 1024u,
  .ipcmdSerialClkFreq = 0,
};