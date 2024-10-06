/*
** ###################################################################
**     Processors:          MKE02Z16VFM4
**                          MKE02Z16VLC4
**                          MKE02Z16VLD4
**                          MKE02Z32VFM4
**                          MKE02Z32VLC4
**                          MKE02Z32VLD4
**                          MKE02Z32VLH4
**                          MKE02Z32VQH4
**                          MKE02Z64VFM4
**                          MKE02Z64VLC4
**                          MKE02Z64VLD4
**                          MKE02Z64VLH4
**                          MKE02Z64VQH4
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    MKE02P64M40SF0RM Rev 4
**     Version:             rev. 1.0, 2017-05-19
**     Build:               b180109
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
**     - rev. 1.0 (2017-05-19)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MKE02Z4
 * @version 1.0
 * @date 2017-05-19
 * @brief Device specific configuration file for MKE02Z4 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "BSP.h"

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit(void) {
#if defined(DISABLE_WDOG)
  WDOG->CNT   = WDOG_UPDATE_KEY1;
  WDOG->CNT   = WDOG_UPDATE_KEY2;
  WDOG->TOVAL = 0xFFFFu;
  WDOG->CS1   = (uint8_t) ((WDOG->CS1) & ~WDOG_CS1_EN_MASK) | WDOG_CS1_UPDATE_MASK;
  WDOG->CS2  |= 0u;
#endif /* (DISABLE_WDOG) */
  BOARD_BootClockRUN();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate(void) {
  uint32_t ICSOUTClock; /* Variable to store output clock frequency of the ICS module */
  uint16_t Divider;
  uint16_t Temp;

  //
  // Check whether core clock is supposed to be 40Mhz and then choose the
  // appropriate divider register. For 40MHz core clock the SIM_BUSDIV SFR
  // has to be set to 1, specifying that ICSOUTClock had to be divided by 2.
  // If SIM_BUSDIV is set to 1, BDIV of ICS_CS2 has to be cleared to 0.
  //
  if (SIM->BUSDIV == 1u) {
	Divider = 2;
  } else {
    Divider = (0x01U) << ((ICS->C2 & ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT);
  }

  switch ((ICS->C1 & ICS_C1_CLKS_MASK) >> ICS_C1_CLKS_SHIFT) {
  case 0x0:
    /* FLL */
    if(ICS->C1 & ICS_C1_IREFS_MASK) {
      ICSOUTClock = CPU_INT_IRC_CLK_HZ * 1024UL;
    } else {
      /* Reference Divider */
      Temp = (ICS->C1 & ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT;
      Temp = (Temp + 1) * ((OSC->CR & OSC_CR_RANGE_MASK) ? 32 : 1);
      ICSOUTClock = CPU_XTAL_CLK_HZ / Temp * 1024UL;
    }
    break;
  case 0x1:
    /* Internal IRC */
    ICSOUTClock = CPU_INT_IRC_CLK_HZ;
    break;
  case 0x2:
    /* External OSC */
    ICSOUTClock = CPU_XTAL_CLK_HZ;
    break;
  default:
    return;
  }
  SystemCoreClock = (ICSOUTClock / Divider);
}
