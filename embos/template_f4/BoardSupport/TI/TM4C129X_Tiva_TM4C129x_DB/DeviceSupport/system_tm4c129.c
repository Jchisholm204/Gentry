/**************************************************************************//**
 * @file     system_TM4C129.c
 * @brief    CMSIS Device System Source File for
 *           TI TIVA TM4C129 Snowflake Device Series
 * @version  V1.00
 * @date     15. May 2013
 *
 * @note
 *
 *                                                             modified by Keil
 ******************************************************************************/

#include <stdint.h>
#include "TM4C129.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __SYSTEM_CLOCK    (120000000ul)

// PLL defines
#define SYSCTL_MOSCCTL                 (*(volatile unsigned int*)(0x400FE07Cu))
#define SYSCTL_MEMTIM0                 (*(volatile unsigned int*)(0x400FE0C0u))
#define SYSCTL_RSCLKCFG                (*(volatile unsigned int*)(0x400FE0B0u))
#define SYSCTL_PLLFREQ0                (*(volatile unsigned int*)(0x400FE160u))
#define SYSCTL_PLLFREQ1                (*(volatile unsigned int*)(0x400FE164u))
#define SYSCTL_PLLSTAT                 (*(volatile unsigned int*)(0x400FE168u))
#define SysCtlXtalCfgToIndex(a)        ((a & 0x7c0) >> 6)
#define PLL_M_TO_REG(mi, mf)           ((unsigned int)mi | (unsigned int)(mf << SYSCTL_PLLFREQ0_MFRAC_S))
#define PLL_N_TO_REG(n)                ((unsigned int)(n - 1) << SYSCTL_PLLFREQ1_N_S)
#define MAX_VCO_ENTRIES                2
#define MAX_XTAL_ENTRIES               18
#define SYSCTL_MEMTIM0_EBCHT_M         0x03C00000uL  // EEPROM Clock High Time
#define SYSCTL_MEMTIM0_EBCHT_0_5       0x00000000uL  // System clock high
#define SYSCTL_MEMTIM0_EBCHT_1         0x00400000uL  // 1 system clock period
#define SYSCTL_MEMTIM0_EBCHT_1_5       0x00800000uL  // 1.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2         0x00C00000uL  // 2 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_2_5       0x01000000uL  // 2.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3         0x01400000uL  // 3 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_3_5       0x01800000uL  // 3.5 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4         0x01C00000uL  // 4 system clock periods
#define SYSCTL_MEMTIM0_EBCHT_4_5       0x02000000uL  // 4.5 system clock periods
#define SYSCTL_MEMTIM0_EBCE            0x00200000uL  // EEPROM Bank Clock Edge
#define SYSCTL_MEMTIM0_MB1             0x00100010uL  // Must be one
#define SYSCTL_MEMTIM0_EWS_M           0x000F0000uL  // EEPROM Wait State
#define SYSCTL_MEMTIM0_FBCHT_M         0x000003C0uL  // Flash Bank Clock High Time
#define SYSCTL_MEMTIM0_FBCHT_0_5       0x00000000uL  // System clock high
#define SYSCTL_MEMTIM0_FBCHT_1         0x00000040uL  // 1 system clock period
#define SYSCTL_MEMTIM0_FBCHT_1_5       0x00000080uL  // 1.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2         0x000000C0uL  // 2 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_2_5       0x00000100uL  // 2.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3         0x00000140uL  // 3 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_3_5       0x00000180uL  // 3.5 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4         0x000001C0uL  // 4 system clock periods
#define SYSCTL_MEMTIM0_FBCHT_4_5       0x00000200uL  // 4.5 system clock periods
#define SYSCTL_MEMTIM0_FBCE            0x00000020uL  // Flash Bank Clock Edge
#define SYSCTL_MEMTIM0_FWS_M           0x0000000FuL  // Flash Wait State
#define SYSCTL_MEMTIM0_EWS_S           16
#define SYSCTL_MEMTIM0_FWS_S           0
#define SYSCTL_XTAL_10MHZ              0x00000400    // External crystal is 10 MHz
#define SYSCTL_XTAL_16MHZ              0x00000540    // External crystal is 16 MHz
#define SYSCTL_XTAL_25MHZ              0x00000680    // External crystal is 25.0 MHz
#define SYSCTL_XTAL_5MHZ               0x00000240    // External crystal is 5MHz
#define SYSCTL_USE_PLL                 0x00000000uL  // System clock is the PLL clock
#define SYSCTL_USE_OSC                 0x00003800uL  // System clock is the osc clock
#define SYSCTL_RSCLKCFG_USEPLL         0x10000000uL  // Use PLL
#define SYSCTL_RSCLKCFG_PSYSDIV_S      0
#define SYSCTL_RSCLKCFG_PSYSDIV_M      0x000003FFuL  // PLL System Clock Divisor
#define SYSCTL_RSCLKCFG_PLLSRC_PIOSC   0x00000000uL  // PIOSC is PLL input clock source
#define SYSCTL_RSCLKCFG_PLLSRC_MOSC    0x03000000uL  // MOSC is PLL input clock source
#define SYSCTL_RSCLKCFG_PLLSRC_M       0x0F000000uL  // PLL Source
#define SYSCTL_RSCLKCFG_OSYSDIV_S      10
#define SYSCTL_RSCLKCFG_OSYSDIV_M      0x000FFC00uL  // Oscillator System Clock Divisor
#define SYSCTL_RSCLKCFG_OSCSRC_M       0x00F00000uL  // Oscillator Source
#define SYSCTL_RSCLKCFG_OSCSRC_RTC     0x00400000uL  // RTC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_PIOSC   0x00000000uL  // PIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_LFIOSC  0x00200000uL  // LFIOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_MOSC    0x00300000uL  // MOSC is oscillator source
#define SYSCTL_RSCLKCFG_OSCSRC_RTC     0x00400000uL  // RTC is oscillator source
#define SYSCTL_RSCLKCFG_MEMTIMU        0x80000000uL  // Memory Timing Register Update
#define SYSCTL_RSCLKCFG_NEWFREQ        0x40000000uL  // New PLLFREQ Accept
#define SYSCTL_RSCLKCFG_ACG            0x20000000uL  // Auto Clock Gating Enable
#define SYSCTL_RSCLKCFG_USEPLL         0x10000000uL  // Use PLL
#define SYSCTL_PLLSTAT_LOCK            0x00000001uL  // PLL Lock
#define SYSCTL_PLLFREQ0_PLLPWR         0x00800000uL  // PLL Power
#define SYSCTL_PLLFREQ0_MFRAC_M        0x000FFC00uL  // PLL M Fractional Value
#define SYSCTL_PLLFREQ0_MINT_M         0x000003FFuL  // PLL M Integer Value
#define SYSCTL_PLLFREQ0_MFRAC_S        10
#define SYSCTL_PLLFREQ0_MINT_S         0
#define SYSCTL_PLLFREQ1_Q_M            0x00001F00uL  // PLL Q Value
#define SYSCTL_PLLFREQ1_N_M            0x0000001FuL  // PLL N Value
#define SYSCTL_PLLFREQ1_Q_S            8
#define SYSCTL_PLLFREQ1_N_S            0
#define SYSCTL_CFG_VCO_480             0xF1000000uL  // VCO is 480 MHz
#define SYSCTL_MOSCCTL_NOXTAL          0x00000004uL  // No Crystal Connected
#define SYSCTL_MOSCCTL_OSCRNG          0x00000010uL  // Oscillator Range
#define SYSCTL_MOSCCTL_PWRDN           0x00000008uL  // Power Down
#define SYSCTL_OSC_INT30               0x00000030uL  // Osc source is int. 30 KHz
#define SYSCTL_OSC_INT                 0x00000010uL  // Osc source is int. osc
#define SYSCTL_OSC_EXT32               0x80000038uL  // Osc source is ext. 32 KHz
#define SYSCTL_OSC_MAIN                0x00000000uL  // Osc source is main osc

/*********************************************************************
*
*       Const data (for PLL init)
*
**********************************************************************
*/
static const unsigned int g_pppui32XTALtoVCO[MAX_VCO_ENTRIES][MAX_XTAL_ENTRIES][2] = {
  {
    //
    // VCO 320 MHz
    //
    { PLL_M_TO_REG(64, 0),   PLL_N_TO_REG(1) },     // 5 MHz
    { PLL_M_TO_REG(62, 512), PLL_N_TO_REG(1) },     // 5.12 MHz
    { PLL_M_TO_REG(160, 0),  PLL_N_TO_REG(3) },     // 6 MHz
    { PLL_M_TO_REG(52, 85),  PLL_N_TO_REG(1) },     // 6.144 MHz
    { PLL_M_TO_REG(43, 412), PLL_N_TO_REG(1) },     // 7.3728 MHz
    { PLL_M_TO_REG(40, 0),   PLL_N_TO_REG(1) },     // 8 MHz
    { PLL_M_TO_REG(39, 64),  PLL_N_TO_REG(1) },     // 8.192 MHz
    { PLL_M_TO_REG(32, 0),   PLL_N_TO_REG(1) },     // 10 MHz
    { PLL_M_TO_REG(80, 0),   PLL_N_TO_REG(3) },     // 12 MHz
    { PLL_M_TO_REG(26, 43),  PLL_N_TO_REG(1) },     // 12.288 MHz
    { PLL_M_TO_REG(23, 613), PLL_N_TO_REG(1) },     // 13.56 MHz
    { PLL_M_TO_REG(22, 358), PLL_N_TO_REG(1) },     // 14.318180 MHz
    { PLL_M_TO_REG(20, 0),   PLL_N_TO_REG(1) },     // 16 MHz
    { PLL_M_TO_REG(19, 544), PLL_N_TO_REG(1) },     // 16.384 MHz
    { PLL_M_TO_REG(160, 0),  PLL_N_TO_REG(9) },     // 18 MHz
    { PLL_M_TO_REG(16, 0),   PLL_N_TO_REG(1) },     // 20 MHz
    { PLL_M_TO_REG(40, 0),   PLL_N_TO_REG(3) },     // 24 MHz
    { PLL_M_TO_REG(64, 0),   PLL_N_TO_REG(5) },     // 25 MHz
  },
  {
    //
    // VCO 480 MHz
    //
    { PLL_M_TO_REG(96, 0),   PLL_N_TO_REG(1) },     // 5 MHz
    { PLL_M_TO_REG(93, 768), PLL_N_TO_REG(1) },     // 5.12 MHz
    { PLL_M_TO_REG(80, 0),   PLL_N_TO_REG(1) },     // 6 MHz
    { PLL_M_TO_REG(78, 128), PLL_N_TO_REG(1) },     // 6.144 MHz
    { PLL_M_TO_REG(65, 107), PLL_N_TO_REG(1) },     // 7.3728 MHz
    { PLL_M_TO_REG(60, 0),   PLL_N_TO_REG(1) },     // 8 MHz
    { PLL_M_TO_REG(58, 608), PLL_N_TO_REG(1) },     // 8.192 MHz
    { PLL_M_TO_REG(48, 0),   PLL_N_TO_REG(1) },     // 10 MHz
    { PLL_M_TO_REG(40, 0),   PLL_N_TO_REG(1) },     // 12 MHz
    { PLL_M_TO_REG(39, 64),  PLL_N_TO_REG(1) },     // 12.288 MHz
    { PLL_M_TO_REG(35, 408), PLL_N_TO_REG(1) },     // 13.56 MHz
    { PLL_M_TO_REG(33, 536), PLL_N_TO_REG(1) },     // 14.318180 MHz
    { PLL_M_TO_REG(30, 0),   PLL_N_TO_REG(1) },     // 16 MHz
    { PLL_M_TO_REG(29, 304), PLL_N_TO_REG(1) },     // 16.384 MHz
    { PLL_M_TO_REG(80, 0),   PLL_N_TO_REG(3) },     // 18 MHz
    { PLL_M_TO_REG(24, 0),   PLL_N_TO_REG(1) },     // 20 MHz
    { PLL_M_TO_REG(20, 0),   PLL_N_TO_REG(1) },     // 24 MHz
    { PLL_M_TO_REG(96, 0),   PLL_N_TO_REG(5) },     // 25 MHz
  },
};

static const unsigned int g_pui32Xtals[] = {
  1000000,
  1843200,
  2000000,
  2457600,
  3579545,
  3686400,
  4000000,
  4096000,
  4915200,
  5000000,
  5120000,
  6000000,
  6144000,
  7372800,
  8000000,
  8192000,
  10000000,
  12000000,
  12288000,
  13560000,
  14318180,
  16000000,
  16384000,
  18000000,
  20000000,
  24000000,
  25000000
};

static const unsigned int g_pui32VCOFrequencies[MAX_VCO_ENTRIES] = {
  320000000,                              // VCO 320
  480000000,                              // VCO 480
};

static const struct {
  unsigned int ui32Frequency;
  unsigned int ui32MemTiming;
}
g_sXTALtoMEMTIM[] = {
  { 16000000, (SYSCTL_MEMTIM0_FBCHT_0_5 | SYSCTL_MEMTIM0_FBCE |
               (0 << SYSCTL_MEMTIM0_FWS_S) |
               SYSCTL_MEMTIM0_EBCHT_0_5 | SYSCTL_MEMTIM0_EBCE |
               (0 << SYSCTL_MEMTIM0_EWS_S) |
               SYSCTL_MEMTIM0_MB1) },
  { 30000000, (SYSCTL_MEMTIM0_FBCHT_1 | SYSCTL_MEMTIM0_FBCE |
               (1 << SYSCTL_MEMTIM0_FWS_S) |
               SYSCTL_MEMTIM0_EBCHT_1 | SYSCTL_MEMTIM0_EBCE |
               (1 << SYSCTL_MEMTIM0_EWS_S) |
               SYSCTL_MEMTIM0_MB1) },
  { 50000000, (SYSCTL_MEMTIM0_FBCHT_1_5 | SYSCTL_MEMTIM0_FBCE |
               (2 << SYSCTL_MEMTIM0_FWS_S) |
               SYSCTL_MEMTIM0_EBCHT_1_5 | SYSCTL_MEMTIM0_EBCE |
               (2 << SYSCTL_MEMTIM0_EWS_S) |
               SYSCTL_MEMTIM0_MB1) },
  { 60000000, (SYSCTL_MEMTIM0_FBCHT_2 | (3 << SYSCTL_MEMTIM0_FWS_S) |
               SYSCTL_MEMTIM0_EBCHT_2 | (3 << SYSCTL_MEMTIM0_EWS_S) |
               SYSCTL_MEMTIM0_MB1) },
  { 80000000, (SYSCTL_MEMTIM0_FBCHT_2_5 | (4 << SYSCTL_MEMTIM0_FWS_S) |
               SYSCTL_MEMTIM0_EBCHT_2_5 | (4 << SYSCTL_MEMTIM0_EWS_S) |
               SYSCTL_MEMTIM0_MB1) },
  { 100000000, (SYSCTL_MEMTIM0_FBCHT_3 | (5 << SYSCTL_MEMTIM0_FWS_S) |
                SYSCTL_MEMTIM0_EBCHT_3 | (5 << SYSCTL_MEMTIM0_EWS_S) |
                SYSCTL_MEMTIM0_MB1) },
  { 120000000, (SYSCTL_MEMTIM0_FBCHT_3_5 | (6 << SYSCTL_MEMTIM0_FWS_S) |
                SYSCTL_MEMTIM0_EBCHT_3_5 | (6 << SYSCTL_MEMTIM0_EWS_S) |
                SYSCTL_MEMTIM0_MB1) },
};

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK; /*!< System Clock Frequency (Core Clock)*/


/*********************************************************************
*
*       _SysCtlFrequencyGet()
*
*       Returns PLL frequency
*/
static unsigned int _SysCtlFrequencyGet(unsigned int ui32Xtal) {
  unsigned int ui32Result;
  unsigned int ui16F1, ui16F2;
  unsigned int ui16PInt, ui16PFract;
  unsigned int ui8Q, ui8N;

  //
  // Extract all of the values from the hardware registers.
  //
  ui16PFract = ((SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_MFRAC_M) >>
                SYSCTL_PLLFREQ0_MFRAC_S);
  ui16PInt = SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_MINT_M;
  ui8Q = (((SYSCTL_PLLFREQ1 & SYSCTL_PLLFREQ1_Q_M) >>
           SYSCTL_PLLFREQ1_Q_S) + 1);
  ui8N = (((SYSCTL_PLLFREQ1 & SYSCTL_PLLFREQ1_N_M) >>
           SYSCTL_PLLFREQ1_N_S) + 1);

  //
  // Divide the crystal value by N.
  //
  ui32Xtal /= (unsigned int)ui8N;

  //
  // Calculate the multiplier for bits 9:5.
  //
  ui16F1 = ui16PFract / 32;

  //
  // Calculate the multiplier for bits 4:0.
  //
  ui16F2 = ui16PFract - (ui16F1 * 32);

  //
  // Get the integer portion.
  //
  ui32Result = ui32Xtal * (unsigned int)ui16PInt;

  //
  // Add first fractional bits portion(9:0).
  //
  ui32Result += (ui32Xtal * (unsigned int)ui16F1) / 32;

  //
  // Add the second fractional bits portion(4:0).
  //
  ui32Result += (ui32Xtal * (unsigned int)ui16F2) / 1024;

  //
  // Divide the result by Q.
  //
  ui32Result = ui32Result / (unsigned int)ui8Q;

  //
  // Return the resulting PLL frequency.
  //
  return(ui32Result);
}


/*********************************************************************
*
*       _SysCtlMemTimingGet()
*
*       Returns an appropriate flash memory timing
*/
static unsigned int _SysCtlMemTimingGet(unsigned int ui32SysClock) {
  unsigned int ui8Idx;

  //
  // Loop through the flash memory timings.
  //
  for(ui8Idx = 0;
      ui8Idx < (sizeof(g_sXTALtoMEMTIM) / sizeof(g_sXTALtoMEMTIM[0]));
      ui8Idx++) {
      //
      // See if the system clock frequency is less than the maximum frequency
      // for this flash memory timing.
      //
      if(ui32SysClock <= g_sXTALtoMEMTIM[ui8Idx].ui32Frequency) {
          //
          // This flash memory timing is the best choice for the system clock
          // frequency, so return it now.
          //
          return(g_sXTALtoMEMTIM[ui8Idx].ui32MemTiming);
      }
  }

  //
  // An appropriate flash memory timing could not be found, so the device is
  // being clocked too fast.  Return the default flash memory timing.
  //
  return (0);
}

/*********************************************************************
*
*       _SysCtlClockFreqSet()
*
*       Initializes the PLL
*/
static void _SysCtlClockFreqSet(unsigned int ui32Config, unsigned int ui32SysClock) {
  int          i32Timeout, i32VCOIdx, i32XtalIdx;
  unsigned int ui32MOSCCTL;
  unsigned int ui32SysDiv, ui32Osc, ui32OscSelect, ui32RSClkConfig;
  char         bNewPLL;

  //
  // Get the index of the crystal from the ui32Config parameter.
  //
  i32XtalIdx = SysCtlXtalCfgToIndex(ui32Config);

  //
  // Determine which non-PLL source was selected.
  //
  if((ui32Config & 0x38) == SYSCTL_OSC_INT) {
    //
    // Use the nominal frequency for the PIOSC oscillator and set the
    // crystal select.
    //
    ui32Osc = 16000000;
    ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_PIOSC;
    ui32OscSelect |= SYSCTL_RSCLKCFG_PLLSRC_PIOSC;

    //
    // Force the crystal index to the value for 16-MHz.
    //
    i32XtalIdx = SysCtlXtalCfgToIndex(SYSCTL_XTAL_16MHZ);
  } else if((ui32Config & 0x38) == SYSCTL_OSC_INT30) {
    //
    // Use the nominal frequency for the low frequency oscillator.
    //
    ui32Osc = 30000;
    ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_LFIOSC;
  }
  else if((ui32Config & 0x38) == (SYSCTL_OSC_EXT32 & 0x38)) {
    //
    // Use the RTC frequency.
    //
    ui32Osc = 32768;
    ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_RTC;
  } else if((ui32Config & 0x38) == SYSCTL_OSC_MAIN) {
    //
    // Bounds check the source frequency for the main oscillator.  The is
    // because the PLL tables in the g_pppui32XTALtoVCO structure range
    // from 5MHz to 25MHz.
    //
    if ((i32XtalIdx > (SysCtlXtalCfgToIndex(SYSCTL_XTAL_25MHZ))) ||
       (i32XtalIdx < (SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ)))) {
      return;
    }

    ui32Osc = g_pui32Xtals[i32XtalIdx];

    //
    // Set the PLL source select to MOSC.
    //
    ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_MOSC;
    ui32OscSelect |= SYSCTL_RSCLKCFG_PLLSRC_MOSC;

    //
    // Clear MOSC power down, high oscillator range setting, and no crystal
    // present setting.
    //
    ui32MOSCCTL = SYSCTL_MOSCCTL &
                  ~(SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_PWRDN |
                    SYSCTL_MOSCCTL_NOXTAL);

    //
    // Increase the drive strength for MOSC of 10 MHz and above.
    //
    if (i32XtalIdx >= (SysCtlXtalCfgToIndex(SYSCTL_XTAL_10MHZ) -
                      (SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ)))) {
        ui32MOSCCTL |= SYSCTL_MOSCCTL_OSCRNG;
    }

    SYSCTL_MOSCCTL = ui32MOSCCTL;
  } else {
    //
    // This was an invalid request because no oscillator source was
    // indicated.
    //
    ui32Osc = 0;
    ui32OscSelect = SYSCTL_RSCLKCFG_OSCSRC_PIOSC;
  }

  //
  // Check if the running with the PLL enabled was requested.
  //
  if((ui32Config & SYSCTL_USE_OSC) == SYSCTL_USE_PLL) {
    //
    // ui32Config must be SYSCTL_OSC_MAIN or SYSCTL_OSC_INT.
    //
    if(((ui32Config & 0x38) != SYSCTL_OSC_MAIN) &&
       ((ui32Config & 0x38) != SYSCTL_OSC_INT)) {
        return;
    }

    //
    // Get the VCO index out of the ui32Config parameter.
    //
    i32VCOIdx = (ui32Config >> 24) & 7;

    //
    // Set the memory timings for the PIOSC since we are about to switch
    // back to PIOSC.
    //
    SYSCTL_MEMTIM0 = _SysCtlMemTimingGet(16000000);

    //
    // Clear the old PLL divider and source in case it was set.
    //
    ui32RSClkConfig = SYSCTL_RSCLKCFG &
                      ~(SYSCTL_RSCLKCFG_PSYSDIV_M |
                        SYSCTL_RSCLKCFG_OSCSRC_M |
                        SYSCTL_RSCLKCFG_PLLSRC_M | SYSCTL_RSCLKCFG_USEPLL);

    //
    // Update the memory timings to match running from PIOSC.
    //
    ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

    //
    // Update clock configuration to switch back to PIOSC.
    //
    SYSCTL_RSCLKCFG = ui32RSClkConfig;

    //
    // The table starts at 5 MHz so modify the index to match this.
    //
    i32XtalIdx -= SysCtlXtalCfgToIndex(SYSCTL_XTAL_5MHZ);

    //
    // If there were no changes to the PLL do not force the PLL to lock by
    // writing the PLL settings.
    //
    if((SYSCTL_PLLFREQ1 !=
      g_pppui32XTALtoVCO[i32VCOIdx][i32XtalIdx][1]) ||
     (SYSCTL_PLLFREQ0 !=
      (g_pppui32XTALtoVCO[i32VCOIdx][i32XtalIdx][0] |
       SYSCTL_PLLFREQ0_PLLPWR))) {
      bNewPLL = 1;
    } else {
      bNewPLL = 0;
    }

    //
    // If there are new PLL settings write them.
    //
    if(bNewPLL) {
      //
      // Set the oscillator source.
      //
      SYSCTL_RSCLKCFG |= ui32OscSelect;

      //
      // Set the M, N and Q values provided from the table and preserve
      // the power state of the main PLL.
      //
      SYSCTL_PLLFREQ1 = g_pppui32XTALtoVCO[i32VCOIdx][i32XtalIdx][1];
      SYSCTL_PLLFREQ0 = (g_pppui32XTALtoVCO[i32VCOIdx][i32XtalIdx][0] |(SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_PLLPWR));
    }

    //
    // Calculate the System divider such that we get a frequency that is
    // the closest to the requested frequency without going over.
    //
    ui32SysDiv = (g_pui32VCOFrequencies[i32VCOIdx] + ui32SysClock - 1) / ui32SysClock;
    //
    // Calculate the actual system clock.
    //
    ui32SysClock = _SysCtlFrequencyGet(ui32Osc) / ui32SysDiv;

    //
    // Set the Flash and EEPROM timing values.
    //
    SYSCTL_MEMTIM0 = _SysCtlMemTimingGet(ui32SysClock);

    //
    // Check if the PLL is already powered up.
    //
    if(SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_PLLPWR) {
      if(bNewPLL == 1) {
        //
        // Trigger the PLL to lock to the new frequency.
        //
        SYSCTL_RSCLKCFG |= SYSCTL_RSCLKCFG_NEWFREQ;
      }
    } else {
      //
      // Power up the PLL.
      //
      SYSCTL_PLLFREQ0 |= SYSCTL_PLLFREQ0_PLLPWR;
    }

    //
    // Wait until the PLL has locked.
    //
    for(i32Timeout = 32768; i32Timeout > 0; i32Timeout--) {
      if(SYSCTL_PLLSTAT & SYSCTL_PLLSTAT_LOCK) {
        break;
      }
    }

    //
    // If the loop above did not timeout then switch over to the PLL
    //
    if(i32Timeout) {
      ui32RSClkConfig = SYSCTL_RSCLKCFG;
      ui32RSClkConfig |= ((ui32SysDiv - 1) <<
                          SYSCTL_RSCLKCFG_PSYSDIV_S) | ui32OscSelect |
                         SYSCTL_RSCLKCFG_USEPLL;
      ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

      //
      // Set the new clock configuration.
      //
      SYSCTL_RSCLKCFG = ui32RSClkConfig;
    } else {
      ui32SysClock = 0;
    }
  } else {
    //
    // Set the Flash and EEPROM timing values for PIOSC.
    //
    SYSCTL_MEMTIM0 = _SysCtlMemTimingGet(16000000);

    //
    // Make sure that the PLL is powered down since it is not being used.
    //
    SYSCTL_PLLFREQ0 &= ~SYSCTL_PLLFREQ0_PLLPWR;

    //
    // Clear the old PLL divider and source in case it was set.
    //
    ui32RSClkConfig = SYSCTL_RSCLKCFG;
    ui32RSClkConfig &= ~(SYSCTL_RSCLKCFG_OSYSDIV_M |
                         SYSCTL_RSCLKCFG_OSCSRC_M |
                         SYSCTL_RSCLKCFG_USEPLL);

    //
    // Update the memory timings.
    //
    ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

    //
    // Set the new clock configuration.
    //
    SYSCTL_RSCLKCFG = ui32RSClkConfig;

    //
    // If zero given as the system clock then default to divide by 1.
    //
    if(ui32SysClock == 0) {
      ui32SysDiv = 0;
    } else {
      //
      // Calculate the System divider based on the requested
      // frequency.
      //
      ui32SysDiv = ui32Osc / ui32SysClock;

      //
      // If the system divisor is not already zero, subtract one to
      // set the value in the register which requires the value to
      // be n-1.
      //
      if(ui32SysDiv != 0) {
        ui32SysDiv -= 1;
      }

      //
      // Calculate the system clock.
      //
      ui32SysClock = ui32Osc / (ui32SysDiv + 1);
    }

    //
    // Set the memory timing values for the new system clock.
    //
    SYSCTL_MEMTIM0 = _SysCtlMemTimingGet(ui32SysClock);

    //
    // Set the new system clock values.
    //
    ui32RSClkConfig = SYSCTL_RSCLKCFG;
    ui32RSClkConfig |= (ui32SysDiv << SYSCTL_RSCLKCFG_OSYSDIV_S) |
                       ui32OscSelect;

    //
    // Update the memory timings.
    //
    ui32RSClkConfig |= SYSCTL_RSCLKCFG_MEMTIMU;

    //
    // Set the new clock configuration.
    //
    SYSCTL_RSCLKCFG = ui32RSClkConfig;
  }
}

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  /* TODO: Updates required to fully work with TM4C129 series devices */
  SystemCoreClock = __SYSTEM_CLOCK;

}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
  /* TODO: Updates required to fully work with TM4C129 series devices */
  #if (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
  #endif

#ifdef UNALIGNED_SUPPORT_DISABLE
  SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

  SystemCoreClock = __SYSTEM_CLOCK;

  //
  // Run from the PLL at 120 MHz
  //
  _SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), __SYSTEM_CLOCK);
}
