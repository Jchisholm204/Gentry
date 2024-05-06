/**
 * \file
 *
 * \brief Low-level initialization functions called upon chip startup.
 *
 * Copyright (c) 2017 Microchip Technology Inc.
 *
 * \asf_license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the Licence at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \asf_license_stop
 *
 */

#include "saml11.h"
#define MCLK_BASE_ADDR     (0x40000800u)
#define MCLK_CTRLA         (*(volatile unsigned int* )(MCLK_BASE_ADDR + 0x00))
#define MCLK_CPUDIV        (*(volatile unsigned char*)(MCLK_BASE_ADDR + 0x04))

#define GCLK_BASE_ADDR     (0x40001C00u)
#define GCLK_GENCTRL0      (*(volatile unsigned int*)(GCLK_BASE_ADDR + 0x20))

#define OSCCTRL_BASE_ADDR  (0x40001000u)
#define OSCCTRL_OSC16M     (*(volatile unsigned char*)(OSCCTRL_BASE_ADDR + 0x18))

/**
 * Initial system clock frequency. The System RC Oscillator (RCSYS) provides
 * the source for the main clock at chip startup.
 */
#define __SYSTEM_CLOCK        (4000000)
#define DFLLULP_OUTPUT_CLOCK  (32000000)
#define DFLLULP_INPUT_CLOCK   (32768)

uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/

/**
 * Initialize the XOSC32K Oscillator (32 kHz On-board Crystal)
 */
static void XOSC32K_Init(void) {

  /*  * Set XOSC32K Startup time (see "Start-Up Time for 32KHz External Crystal Oscillator" table from datasheet)
  * Enable 32.768kHz output
  * Enable Crystal Oscillator mode
  * Enable XOSC32K
  */
  OSC32KCTRL->XOSC32K.reg = (OSC32KCTRL_XOSC32K_STARTUP(0x3) | OSC32KCTRL_XOSC32K_EN32K |OSC32KCTRL_XOSC32K_XTALEN | OSC32KCTRL_XOSC32K_ENABLE);
  while(!(OSC32KCTRL->STATUS.bit.XOSC32KRDY));  /* Write Synchronized */
}

/**
 * Initialize the FDPLL96MHz to generate 64 MHz
 * Configure and Enable Clock Generator 1, XOSC32K as source not divided
 * Enable DPLL Peripheral Channel with GCLK1 as source
 */
static void DPLL_Init(void) {
  /* Set DPLL Ration to generate 32MHz clock (32768 x (1952+1+(2/16)) / 2 ) */
  while((OSCCTRL->DPLLSYNCBUSY.bit.DPLLRATIO));
  OSCCTRL->DPLLRATIO.reg = (OSCCTRL_DPLLRATIO_LDR(1952) | OSCCTRL_DPLLRATIO_LDRFRAC(2));
  while((OSCCTRL->DPLLSYNCBUSY.bit.DPLLPRESC));
  OSCCTRL->DPLLPRESC.reg = (OSCCTRL_DPLLPRESC_PRESC_DIV2);
  /* Disable On-demand */
  OSCCTRL->DPLLCTRLA.bit.ONDEMAND = 0;
  /* Set Lock time as automatic and reference clock to XOSC32KHz */
  OSCCTRL->DPLLCTRLB.reg = (OSCCTRL_DPLLCTRLB_LTIME(0)|OSCCTRL_DPLLCTRLB_REFCLK_XOSC32K);
  /*Enable DPLL */
  OSCCTRL->DPLLCTRLA.bit.ENABLE = 1;
  while(OSCCTRL->DPLLSYNCBUSY.bit.ENABLE);
  /* Wait for DPLL lock flag */
  while(!OSCCTRL->DPLLSTATUS.bit.LOCK);
}


/**
 * Initialize the system
 *
 * Initialize the XOSC32K Oscillator (32 kHz On-board Crystal)
 * Switch Performance level from PL0 (Default after reset) to PL2
 * Configure Flash Wait States based on next Main Clock frequency (32 MHz <-> 2 WS)
 * Initialize the DPLL to generate 64 MHz
 * Set Main clock to DPLL/2
 */
void SystemInit(void)
{
   /* Initialize the XOSC32K Oscillator (32 kHz On-board Crystal) */
  XOSC32K_Init();
  /* Switch Performance level from PL0 (Default after reset) to PL2 */
  PM->PLCFG.bit.PLSEL = 2;
  while(PM->INTFLAG.bit.PLRDY == 0);
  /* Configure Flash Wait States based on next Main Clock frequency (32 MHz <-> 2 WS) */
  NVMCTRL->CTRLB.bit.RWS = 2;
  /* Initialize the DFLLULP to generate 64 MHz */
  DPLL_Init();
  /* - Set Generic clock 0 (MAIN clock for CPU and synchronous clock) */
  while((GCLK->SYNCBUSY.bit.GENCTRL0));
  GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_SRC(0x07) | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN);
  /*  Set Generic clock 1 (GCLK1), XOSC32K as source not divided */
  GCLK->GENCTRL[1].reg = (GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN);

  return;
}

/**
 * Update SystemCoreClock variable
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate(void) {
  unsigned int genclkctrldiv;
  //
  // Read which clock is selected for the main clock
  //
  if ((MCLK_CTRLA >> 2) & 0x1) {  // DFLLULP as main clock source
    while (1) {}                  // not implemented
  } else {                        // GCLKMAIN as main clock source
    //
    // Calculates the divider that the Generic Clock Controller Generator 0 uses
    //
    genclkctrldiv = (GCLK_GENCTRL0 >> 16) & 0xFF;   // Read DIV value
    if (GCLK_GENCTRL0 & (1u << 12)) {               // Read how DIV is used
      genclkctrldiv = (1u << (genclkctrldiv + 1));  // Divider = 2^(DIV + 1)
    } else {                                        // else divider = DIV;
      if (genclkctrldiv == 0) {                     // If DIV equals 0, divider = 1
        genclkctrldiv = 1;
      }
    }
    //
    // Reads the selected clock source of the Generic Clock Controller Generator 0
    //
    switch (GCLK_GENCTRL0 & 0x1Fu) {
    case 0x05:  // OSC16M oscillator output selected
      SystemCoreClock  = (__SYSTEM_CLOCK * (((OSCCTRL_OSC16M >> 2) & 0x3) + 1));
      SystemCoreClock /= (genclkctrldiv * MCLK_CPUDIV);
      return;
    default:    // not implemented
      SystemCoreClock = 32000000u;
      break;
    }
  }
  return;
}
