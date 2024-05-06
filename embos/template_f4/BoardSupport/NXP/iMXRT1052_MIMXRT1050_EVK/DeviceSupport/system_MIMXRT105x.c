/*
** ###################################################################
**     Processors:          MIMXRT1052CVJ5B
**                          MIMXRT1052CVL5B
**                          MIMXRT1052DVJ6B
**                          MIMXRT1052DVL6B
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    IMXRT1050RM Rev.1, 03/2018
**     Version:             rev. 0.1, 2017-01-10
**     Build:               b180509
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     The Clear BSD License
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without
**     modification, are permitted (subject to the limitations in the
**     disclaimer below) provided that the following conditions are met:
**
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**
**     * Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from
**       this software without specific prior written permission.
**
**     NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
**     GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
**     HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
**     WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
**     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
**     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
**     CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
**     SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
**     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
**     WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
**     OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
**     IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MIMXRT1052
 * @version 0.1
 * @date 2017-01-10
 * @brief Device specific configuration file for MIMXRT1052 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"

/*********************************************************************
*
*       SFR definitions
*
**********************************************************************
*/
/*********************************************************************
*
*       IOMUX SFRs
*/
#define SW_MUX_CTL_PAD_BASE_ADDR    (0x401F8000uL)
#define SW_MUX_CTL_PAD_GPIO_EMC_00  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x014))
#define SW_MUX_CTL_PAD_GPIO_EMC_01  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x018))
#define SW_MUX_CTL_PAD_GPIO_EMC_02  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x01C))
#define SW_MUX_CTL_PAD_GPIO_EMC_03  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x020))
#define SW_MUX_CTL_PAD_GPIO_EMC_04  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x024))
#define SW_MUX_CTL_PAD_GPIO_EMC_05  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x028))
#define SW_MUX_CTL_PAD_GPIO_EMC_06  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x02C))
#define SW_MUX_CTL_PAD_GPIO_EMC_07  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x030))
#define SW_MUX_CTL_PAD_GPIO_EMC_08  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x034))
#define SW_MUX_CTL_PAD_GPIO_EMC_09  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x038))
#define SW_MUX_CTL_PAD_GPIO_EMC_10  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x03C))
#define SW_MUX_CTL_PAD_GPIO_EMC_11  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x040))
#define SW_MUX_CTL_PAD_GPIO_EMC_12  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x044))
#define SW_MUX_CTL_PAD_GPIO_EMC_13  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x048))
#define SW_MUX_CTL_PAD_GPIO_EMC_14  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x04C))
#define SW_MUX_CTL_PAD_GPIO_EMC_15  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x050))
#define SW_MUX_CTL_PAD_GPIO_EMC_16  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x054))
#define SW_MUX_CTL_PAD_GPIO_EMC_17  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x058))
#define SW_MUX_CTL_PAD_GPIO_EMC_18  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x05C))
#define SW_MUX_CTL_PAD_GPIO_EMC_19  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x060))
#define SW_MUX_CTL_PAD_GPIO_EMC_20  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x064))
#define SW_MUX_CTL_PAD_GPIO_EMC_21  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x068))
#define SW_MUX_CTL_PAD_GPIO_EMC_22  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x06C))
#define SW_MUX_CTL_PAD_GPIO_EMC_23  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x070))
#define SW_MUX_CTL_PAD_GPIO_EMC_24  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x074))
#define SW_MUX_CTL_PAD_GPIO_EMC_25  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x078))
#define SW_MUX_CTL_PAD_GPIO_EMC_26  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x07C))
#define SW_MUX_CTL_PAD_GPIO_EMC_27  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x080))
#define SW_MUX_CTL_PAD_GPIO_EMC_28  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x084))
#define SW_MUX_CTL_PAD_GPIO_EMC_29  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x088))
#define SW_MUX_CTL_PAD_GPIO_EMC_30  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x08C))
#define SW_MUX_CTL_PAD_GPIO_EMC_31  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x090))
#define SW_MUX_CTL_PAD_GPIO_EMC_32  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x094))
#define SW_MUX_CTL_PAD_GPIO_EMC_33  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x098))
#define SW_MUX_CTL_PAD_GPIO_EMC_34  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x09C))
#define SW_MUX_CTL_PAD_GPIO_EMC_35  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0A0))
#define SW_MUX_CTL_PAD_GPIO_EMC_36  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0A4))
#define SW_MUX_CTL_PAD_GPIO_EMC_37  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0A8))
#define SW_MUX_CTL_PAD_GPIO_EMC_38  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0AC))
#define SW_MUX_CTL_PAD_GPIO_EMC_39  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0B0))
#define SW_MUX_CTL_PAD_GPIO_EMC_40  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0B4))
#define SW_MUX_CTL_PAD_GPIO_EMC_41  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x0B8))

/*********************************************************************
*
*       IOMUX PAD SFRs
*/
#define SW_PAD_CTL_PAD_GPIO_EMC_00  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x204))
#define SW_PAD_CTL_PAD_GPIO_EMC_01  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x208))
#define SW_PAD_CTL_PAD_GPIO_EMC_02  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x20C))
#define SW_PAD_CTL_PAD_GPIO_EMC_03  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x210))
#define SW_PAD_CTL_PAD_GPIO_EMC_04  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x214))
#define SW_PAD_CTL_PAD_GPIO_EMC_05  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x218))
#define SW_PAD_CTL_PAD_GPIO_EMC_06  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x21C))
#define SW_PAD_CTL_PAD_GPIO_EMC_07  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x220))
#define SW_PAD_CTL_PAD_GPIO_EMC_08  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x224))
#define SW_PAD_CTL_PAD_GPIO_EMC_09  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x228))
#define SW_PAD_CTL_PAD_GPIO_EMC_10  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x22C))
#define SW_PAD_CTL_PAD_GPIO_EMC_11  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x230))
#define SW_PAD_CTL_PAD_GPIO_EMC_12  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x234))
#define SW_PAD_CTL_PAD_GPIO_EMC_13  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x238))
#define SW_PAD_CTL_PAD_GPIO_EMC_14  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x23C))
#define SW_PAD_CTL_PAD_GPIO_EMC_15  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x240))
#define SW_PAD_CTL_PAD_GPIO_EMC_16  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x244))
#define SW_PAD_CTL_PAD_GPIO_EMC_17  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x248))
#define SW_PAD_CTL_PAD_GPIO_EMC_18  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x24C))
#define SW_PAD_CTL_PAD_GPIO_EMC_19  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x250))
#define SW_PAD_CTL_PAD_GPIO_EMC_20  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x254))
#define SW_PAD_CTL_PAD_GPIO_EMC_21  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x258))
#define SW_PAD_CTL_PAD_GPIO_EMC_22  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x25C))
#define SW_PAD_CTL_PAD_GPIO_EMC_23  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x260))
#define SW_PAD_CTL_PAD_GPIO_EMC_24  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x264))
#define SW_PAD_CTL_PAD_GPIO_EMC_25  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x268))
#define SW_PAD_CTL_PAD_GPIO_EMC_26  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x26C))
#define SW_PAD_CTL_PAD_GPIO_EMC_27  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x270))
#define SW_PAD_CTL_PAD_GPIO_EMC_28  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x274))
#define SW_PAD_CTL_PAD_GPIO_EMC_29  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x278))
#define SW_PAD_CTL_PAD_GPIO_EMC_30  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x27C))
#define SW_PAD_CTL_PAD_GPIO_EMC_31  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x280))
#define SW_PAD_CTL_PAD_GPIO_EMC_32  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x284))
#define SW_PAD_CTL_PAD_GPIO_EMC_33  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x288))
#define SW_PAD_CTL_PAD_GPIO_EMC_34  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x28C))
#define SW_PAD_CTL_PAD_GPIO_EMC_35  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x290))
#define SW_PAD_CTL_PAD_GPIO_EMC_36  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x294))
#define SW_PAD_CTL_PAD_GPIO_EMC_37  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x298))
#define SW_PAD_CTL_PAD_GPIO_EMC_38  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x29C))
#define SW_PAD_CTL_PAD_GPIO_EMC_39  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x2A0))
#define SW_PAD_CTL_PAD_GPIO_EMC_40  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x2A4))
#define SW_PAD_CTL_PAD_GPIO_EMC_41  (*(volatile unsigned int *)(SW_MUX_CTL_PAD_BASE_ADDR + 0x2A8))

/*********************************************************************
*
*       SEMC SFRs
*/
#define SEMC_BASE_ADDR  (0x402F0000uL)
#define SEMC_MCR        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x00))
#define SEMC_IOCR       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x04))
#define SEMC_BMCR0      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x08))
#define SEMC_BMCR1      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x0C))
#define SEMC_BR0        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x10))
#define SEMC_BR1        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x14))
#define SEMC_BR2        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x18))
#define SEMC_BR3        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x1C))
#define SEMC_BR4        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x20))
#define SEMC_BR5        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x24))
#define SEMC_BR6        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x28))
#define SEMC_BR7        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x2C))
#define SEMC_BR8        (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x30))
#define SEMC_INTEN      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x38))
#define SEMC_INTR       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x3C))
#define SEMC_SDRAMCR0   (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x40))
#define SEMC_SDRAMCR1   (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x44))
#define SEMC_SDRAMCR2   (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x48))
#define SEMC_SDRAMCR3   (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x4C))
#define SEMC_NANDCR0    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x50))
#define SEMC_NANDCR1    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x54))
#define SEMC_NANDCR2    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x58))
#define SEMC_NANDCR3    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x5C))
#define SEMC_NORCR0     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x60))
#define SEMC_NORCR1     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x64))
#define SEMC_NORCR2     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x68))
#define SEMC_NORCR3     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x6C))
#define SEMC_SRAMCR0    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x70))
#define SEMC_SRAMCR1    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x74))
#define SEMC_SRAMCR2    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x78))
#define SEMC_SRAMCR3    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x7C))
#define SEMC_DBICR0     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x80))
#define SEMC_DBICR1     (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x84))
#define SEMC_IPCR0      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x90))
#define SEMC_IPCR1      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x94))
#define SEMC_IPCR2      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x98))
#define SEMC_IPCMD      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0x9C))
#define SEMC_IPTXDAT    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xA0))
#define SEMC_IPRXDAT    (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xB0))
#define SEMC_STS0       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xC0))
#define SEMC_STS1       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xC4))
#define SEMC_STS2       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xC8))
#define SEMC_STS3       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xCC))
#define SEMC_STS4       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xD0))
#define SEMC_STS5       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xD4))
#define SEMC_STS6       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xD8))
#define SEMC_STS7       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xDC))
#define SEMC_STS8       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xE0))
#define SEMC_STS9       (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xE4))
#define SEMC_STS10      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xE8))
#define SEMC_STS11      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xEC))
#define SEMC_STS12      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xF0))
#define SEMC_STS13      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xF4))
#define SEMC_STS14      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xF8))
#define SEMC_STS15      (*(volatile unsigned int *)(SEMC_BASE_ADDR + 0xFC))

/*********************************************************************
*
*       Clock Controller Module SFRs
*/
#define CCM_BASE_ADDR  (0x400FC000uL)
#define CCM_CACRR      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x010))
#define CCM_CBCDR      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x014))
#define CCM_CBCMR      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x018))
#define CCM_CDHIPR     (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x048))
#define CCM_CCGR0      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x068))
#define CCM_CCGR1      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x06C))
#define CCM_CCGR2      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x070))
#define CCM_CCGR3      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x074))
#define CCM_CCGR4      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x078))
#define CCM_CCGR5      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x07C))
#define CCM_CCGR6      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x080))
#define CCM_CMEOR      (*(volatile unsigned int *)(CCM_BASE_ADDR + 0x088))

#define CCM_ANALOG_BASE_ADDR  0x400D8000uL
#define CCM_ANALOG_PLL_ARM    (*(volatile unsigned int *)(CCM_ANALOG_BASE_ADDR + 0x000))
#define CCM_ANALOG_PLL_SYS    (*(volatile unsigned int *)(CCM_ANALOG_BASE_ADDR + 0x030))
#define CCM_ANALOG_PFD_528    (*(volatile unsigned int *)(CCM_ANALOG_BASE_ADDR + 0x100))

/*********************************************************************
*
*       DCDC SFRs
*/
#define DCDC_BASE_ADDR  (0x40080000uL)
#define DCDC_REG3       (*(volatile unsigned int *)(DCDC_BASE_ADDR + 0xC))

/*********************************************************************
*
*       _SDRAM_WaitIpCmdDone
*/
static void _SDRAM_WaitIpCmdDone(void) {
  unsigned int reg;

  do {
    reg = SEMC_INTR;
  } while((reg & 0x3) == 0);
}

/*********************************************************************
*
*       _ClockInit
*/
static void _ClockInit(void) {
  //
  // Enable all clocks
  //
  CCM_CCGR0 = 0xffffffff;
  CCM_CCGR1 = 0xffffffff;
  CCM_CCGR2 = 0xffffffff;
  CCM_CCGR3 = 0xffffffff;
  CCM_CCGR4 = 0xffffffff;
  CCM_CCGR5 = 0xffffffff;
  CCM_CCGR6 = 0xffffffff;
  //
  // Initialize clocks
  // ARM clock:        1200MHz
  // AHB/System clock:  600Mhz
  // SDRAM/SEMC:        150MHz
  //
  DCDC_REG3 =  (DCDC_REG3 & 0x1F) | 0x12;  // VDD_SOC to 1,5V in order to config AHB to 600MHz
  //
  // Let the device run on PLL3, so we can configure PLL1
  //
  CCM_CBCDR |=  (1u << 25);  // Switch device to PERIPH_CLK2
  while (CCM_CDHIPR != 0);
  //
  // Initialize PLL1
  //
  CCM_ANALOG_PLL_ARM = 0
                     | (  0u << 16)  // Don't bypass PLL
                     | (  0u << 14)  // Use 24MHZ clock source
                     | (  1u << 13)  // Enable clock output
                     | (  0u << 12)  // Don't power down PLL
                     | (100u <<  0); // Set DIV_SELECT to 100. Fout = Fin * DIV_SELECT / 2 => 1200MHz
  while ((CCM_ANALOG_PLL_ARM & 0x80000000) == 0);
  //
  // Switch back to using PLL1 for the device.
  // Use peripheral clock (600MHz) and divide it by 4 to get 150MHz clock for SEMC.
  //
  CCM_CBCDR = (CCM_CBCDR & ~(1u << 25)) | (3u << 16);
  while (CCM_CDHIPR != 0);
}

/*********************************************************************
*
*       _SDRAMInit
*/
static void _SDRAMInit(void) {
  //
  // Config IOMUX for SDRAM
  //
  SW_MUX_CTL_PAD_GPIO_EMC_00 = 0x00000000; // EMC_00
  SW_MUX_CTL_PAD_GPIO_EMC_01 = 0x00000000; // EMC_01
  SW_MUX_CTL_PAD_GPIO_EMC_02 = 0x00000000; // EMC_02
  SW_MUX_CTL_PAD_GPIO_EMC_03 = 0x00000000; // EMC_03
  SW_MUX_CTL_PAD_GPIO_EMC_04 = 0x00000000; // EMC_04
  SW_MUX_CTL_PAD_GPIO_EMC_05 = 0x00000000; // EMC_05
  SW_MUX_CTL_PAD_GPIO_EMC_06 = 0x00000000; // EMC_06
  SW_MUX_CTL_PAD_GPIO_EMC_07 = 0x00000000; // EMC_07
  SW_MUX_CTL_PAD_GPIO_EMC_08 = 0x00000000; // EMC_08
  SW_MUX_CTL_PAD_GPIO_EMC_09 = 0x00000000; // EMC_09
  SW_MUX_CTL_PAD_GPIO_EMC_10 = 0x00000000; // EMC_10
  SW_MUX_CTL_PAD_GPIO_EMC_11 = 0x00000000; // EMC_11
  SW_MUX_CTL_PAD_GPIO_EMC_12 = 0x00000000; // EMC_12
  SW_MUX_CTL_PAD_GPIO_EMC_13 = 0x00000000; // EMC_13
  SW_MUX_CTL_PAD_GPIO_EMC_14 = 0x00000000; // EMC_14
  SW_MUX_CTL_PAD_GPIO_EMC_15 = 0x00000000; // EMC_15
  SW_MUX_CTL_PAD_GPIO_EMC_16 = 0x00000000; // EMC_16
  SW_MUX_CTL_PAD_GPIO_EMC_17 = 0x00000000; // EMC_17
  SW_MUX_CTL_PAD_GPIO_EMC_18 = 0x00000000; // EMC_18
  SW_MUX_CTL_PAD_GPIO_EMC_19 = 0x00000000; // EMC_19
  SW_MUX_CTL_PAD_GPIO_EMC_20 = 0x00000000; // EMC_20
  SW_MUX_CTL_PAD_GPIO_EMC_21 = 0x00000000; // EMC_21
  SW_MUX_CTL_PAD_GPIO_EMC_22 = 0x00000000; // EMC_22
  SW_MUX_CTL_PAD_GPIO_EMC_23 = 0x00000000; // EMC_23
  SW_MUX_CTL_PAD_GPIO_EMC_24 = 0x00000000; // EMC_24
  SW_MUX_CTL_PAD_GPIO_EMC_25 = 0x00000000; // EMC_25
  SW_MUX_CTL_PAD_GPIO_EMC_26 = 0x00000000; // EMC_26
  SW_MUX_CTL_PAD_GPIO_EMC_27 = 0x00000000; // EMC_27
  SW_MUX_CTL_PAD_GPIO_EMC_28 = 0x00000000; // EMC_28
  SW_MUX_CTL_PAD_GPIO_EMC_29 = 0x00000000; // EMC_29
  SW_MUX_CTL_PAD_GPIO_EMC_30 = 0x00000000; // EMC_30
  SW_MUX_CTL_PAD_GPIO_EMC_31 = 0x00000000; // EMC_31
  SW_MUX_CTL_PAD_GPIO_EMC_32 = 0x00000000; // EMC_32
  SW_MUX_CTL_PAD_GPIO_EMC_33 = 0x00000000; // EMC_33
  SW_MUX_CTL_PAD_GPIO_EMC_34 = 0x00000000; // EMC_34
  SW_MUX_CTL_PAD_GPIO_EMC_35 = 0x00000000; // EMC_35
  SW_MUX_CTL_PAD_GPIO_EMC_36 = 0x00000000; // EMC_36
  SW_MUX_CTL_PAD_GPIO_EMC_37 = 0x00000000; // EMC_37
  SW_MUX_CTL_PAD_GPIO_EMC_38 = 0x00000000; // EMC_38
  SW_MUX_CTL_PAD_GPIO_EMC_39 = 0x00000010; // EMC_39, DQS PIN, enable SION
  SW_MUX_CTL_PAD_GPIO_EMC_40 = 0x00000000; // EMC_40
  SW_MUX_CTL_PAD_GPIO_EMC_41 = 0x00000000; // EMC_41
  //
  // PAD ctrl drive strength = 0x7 to increase drive strength otherwise the data7 bit may fail.
  //
  SW_PAD_CTL_PAD_GPIO_EMC_00 = 0x000110F9; // EMC_00
  SW_PAD_CTL_PAD_GPIO_EMC_01 = 0x000110F9; // EMC_01
  SW_PAD_CTL_PAD_GPIO_EMC_02 = 0x000110F9; // EMC_02
  SW_PAD_CTL_PAD_GPIO_EMC_03 = 0x000110F9; // EMC_03
  SW_PAD_CTL_PAD_GPIO_EMC_04 = 0x000110F9; // EMC_04
  SW_PAD_CTL_PAD_GPIO_EMC_05 = 0x000110F9; // EMC_05
  SW_PAD_CTL_PAD_GPIO_EMC_06 = 0x000110F9; // EMC_06
  SW_PAD_CTL_PAD_GPIO_EMC_07 = 0x000110F9; // EMC_07
  SW_PAD_CTL_PAD_GPIO_EMC_08 = 0x000110F9; // EMC_08
  SW_PAD_CTL_PAD_GPIO_EMC_09 = 0x000110F9; // EMC_09
  SW_PAD_CTL_PAD_GPIO_EMC_10 = 0x000110F9; // EMC_10
  SW_PAD_CTL_PAD_GPIO_EMC_11 = 0x000110F9; // EMC_11
  SW_PAD_CTL_PAD_GPIO_EMC_12 = 0x000110F9; // EMC_12
  SW_PAD_CTL_PAD_GPIO_EMC_13 = 0x000110F9; // EMC_13
  SW_PAD_CTL_PAD_GPIO_EMC_14 = 0x000110F9; // EMC_14
  SW_PAD_CTL_PAD_GPIO_EMC_15 = 0x000110F9; // EMC_15
  SW_PAD_CTL_PAD_GPIO_EMC_16 = 0x000110F9; // EMC_16
  SW_PAD_CTL_PAD_GPIO_EMC_17 = 0x000110F9; // EMC_17
  SW_PAD_CTL_PAD_GPIO_EMC_18 = 0x000110F9; // EMC_18
  SW_PAD_CTL_PAD_GPIO_EMC_19 = 0x000110F9; // EMC_19
  SW_PAD_CTL_PAD_GPIO_EMC_20 = 0x000110F9; // EMC_20
  SW_PAD_CTL_PAD_GPIO_EMC_21 = 0x000110F9; // EMC_21
  SW_PAD_CTL_PAD_GPIO_EMC_22 = 0x000110F9; // EMC_22
  SW_PAD_CTL_PAD_GPIO_EMC_23 = 0x000110F9; // EMC_23
  SW_PAD_CTL_PAD_GPIO_EMC_24 = 0x000110F9; // EMC_24
  SW_PAD_CTL_PAD_GPIO_EMC_25 = 0x000110F9; // EMC_25
  SW_PAD_CTL_PAD_GPIO_EMC_26 = 0x000110F9; // EMC_26
  SW_PAD_CTL_PAD_GPIO_EMC_27 = 0x000110F9; // EMC_27
  SW_PAD_CTL_PAD_GPIO_EMC_28 = 0x000110F9; // EMC_28
  SW_PAD_CTL_PAD_GPIO_EMC_29 = 0x000110F9; // EMC_29
  SW_PAD_CTL_PAD_GPIO_EMC_30 = 0x000110F9; // EMC_30
  SW_PAD_CTL_PAD_GPIO_EMC_31 = 0x000110F9; // EMC_31
  SW_PAD_CTL_PAD_GPIO_EMC_32 = 0x000110F9; // EMC_32
  SW_PAD_CTL_PAD_GPIO_EMC_33 = 0x000110F9; // EMC_33
  SW_PAD_CTL_PAD_GPIO_EMC_34 = 0x000110F9; // EMC_34
  SW_PAD_CTL_PAD_GPIO_EMC_35 = 0x000110F9; // EMC_35
  SW_PAD_CTL_PAD_GPIO_EMC_36 = 0x000110F9; // EMC_36
  SW_PAD_CTL_PAD_GPIO_EMC_37 = 0x000110F9; // EMC_37
  SW_PAD_CTL_PAD_GPIO_EMC_38 = 0x000110F9; // EMC_38
  SW_PAD_CTL_PAD_GPIO_EMC_39 = 0x000110F9; // EMC_39
  SW_PAD_CTL_PAD_GPIO_EMC_40 = 0x000110F9; // EMC_40
  SW_PAD_CTL_PAD_GPIO_EMC_41 = 0x000110F9; // EMC_41
  //
  // Set up SEMC
  //
  SEMC_MCR      = 0x10000004;
  SEMC_BMCR0    = 0x00030524;
  SEMC_BMCR1    = 0x06030524;
  SEMC_BR0      = 0x8000001B;
  SEMC_BR1      = 0x00000000;
  SEMC_BR2      = 0x00000000;
  SEMC_BR3      = 0x00000000;
  SEMC_BR4      = 0x00000000;
  SEMC_BR5      = 0x00000000;
  SEMC_BR6      = 0x00000000;
  SEMC_BR7      = 0x00000000;
  SEMC_BR8      = 0x00000000;
  SEMC_IOCR     = 0x000079A8;
  SEMC_SDRAMCR0 = 0x00000F31;
  SEMC_SDRAMCR1 = 0x00652922;
  SEMC_SDRAMCR2 = 0x00010920;
  SEMC_SDRAMCR3 = 0x50210A08;
  SEMC_DBICR0   = 0x00000021;
  SEMC_DBICR1   = 0x00888888;
  SEMC_IPCR1    = 0x00000002;
  SEMC_IPCR2    = 0x00000000;
  SEMC_IPCR0    = 0x80000000;
  SEMC_IPCMD    = 0xA55A000F;
  _SDRAM_WaitIpCmdDone();
  SEMC_IPCR0    = 0x80000000;
  SEMC_IPCMD    = 0xA55A000C;
  _SDRAM_WaitIpCmdDone();
  SEMC_IPCR0    = 0x80000000;
  SEMC_IPCMD    = 0xA55A000C;
  _SDRAM_WaitIpCmdDone();
  SEMC_IPTXDAT  = 0x00000033;
  SEMC_IPCR0    = 0x80000000;
  SEMC_IPCMD    = 0xA55A000A;
  _SDRAM_WaitIpCmdDone();
  SEMC_SDRAMCR3 = 0x50210A09;
}

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

#if defined(__MCUXPRESSO)
    extern uint32_t g_pfnVectors[];  // Vector table defined in startup code
    SCB->VTOR = (uint32_t)g_pfnVectors;
#endif

/* Watchdog disable */
#if (DISABLE_WDOG)
    if (WDOG1->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG1->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    if (WDOG2->WCR & WDOG_WCR_WDE_MASK)
    {
        WDOG2->WCR &= ~WDOG_WCR_WDE_MASK;
    }
    RTWDOG->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    RTWDOG->TOVAL = 0xFFFF;
    RTWDOG->CS = (uint32_t) ((RTWDOG->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
#endif /* (DISABLE_WDOG) */

    /* Disable Systick which might be enabled by bootrom */
    if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }

/* Enable instruction and data caches */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    if (SCB_CCR_IC_Msk != (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_EnableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk != (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_EnableDCache();
    }
#endif
  _ClockInit();
  _SDRAMInit();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate(void) {

    uint32_t freq;
    uint32_t PLL1MainClock;
    uint32_t PLL2MainClock;
    uint64_t ValuePLL_SYS_DENOM;

    /* Periph_clk2_clk ---> Periph_clk */
    if (CCM->CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK)
    {
        switch (CCM->CBCMR & CCM_CBCMR_PERIPH_CLK2_SEL_MASK)
        {
            /* Pll3_sw_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(0U):
                if(CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_BYPASS_MASK)
                {
                    freq = (((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                           CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
                }
                else
                {
                    freq = (CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK) ? 22U : 20U));
                }
                break;

            /* Osc_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(1U):
                freq = CPU_XTAL_CLK_HZ;
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(2U):
                freq = (((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(3U):
            default:
                freq = 0U;
                break;
        }

        freq /= (((CCM->CBCDR & CCM_CBCDR_PERIPH_CLK2_PODF_MASK) >> CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT) + 1U);
    }
    /* Pre_Periph_clk ---> Periph_clk */
    else
    {
        /* check if pll is bypassed */
        if(CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_BYPASS_MASK)
        {
            PLL1MainClock = (((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
        }
        else
        {
            PLL1MainClock = ((CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >>
                                             CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT)) >> 1U);
        }

        /* check if pll is bypassed */
        if(CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_MASK)
        {
            PLL2MainClock = (((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_MASK) >> CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_SHIFT) == 0U) ?
                   CPU_XTAL_CLK_HZ : CPU_CLK1_HZ;
        }
        else
        {
            PLL2MainClock = (CPU_XTAL_CLK_HZ * ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK) ? 22U : 20U));
        }
        ValuePLL_SYS_DENOM = (uint64_t)(CCM_ANALOG->PLL_SYS_DENOM);
        PLL2MainClock += ((uint64_t)CPU_XTAL_CLK_HZ * ((uint64_t)(CCM_ANALOG->PLL_SYS_NUM))) / ValuePLL_SYS_DENOM;


        switch (CCM->CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK)
        {
            /* PLL2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U):
                freq = PLL2MainClock;
                break;

            /* PLL2 PFD2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT) * 18U;
                break;

            /* PLL2 PFD0 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U):
                freq = PLL2MainClock / ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT) * 18U;
                break;

            /* PLL1 divided(/2) ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U):
                freq = PLL1MainClock / (((CCM->CACRR & CCM_CACRR_ARM_PODF_MASK) >> CCM_CACRR_ARM_PODF_SHIFT) + 1U);
                break;

            default:
                freq = 0U;
                break;
        }
    }

    SystemCoreClock = (freq / (((CCM->CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U));

}
