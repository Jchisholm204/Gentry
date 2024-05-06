/**
 * \file
 *
 * \brief GCC start-up implementation for the SAM D20
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "samd20.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
void NMI_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
//void HardFault_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void PM_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC6_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC7_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors _vectors = {
        .pvStack               = (void*) (&_estack),
        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnReservedM12        = (void*) (0UL), /* Reserved */
        .pfnReservedM11        = (void*) (0UL), /* Reserved */
        .pfnReservedM10        = (void*) (0UL), /* Reserved */
        .pfnReservedM9         = (void*) (0UL), /* Reserved */
        .pfnReservedM8         = (void*) (0UL), /* Reserved */
        .pfnReservedM7         = (void*) (0UL), /* Reserved */
        .pfnReservedM6         = (void*) (0UL), /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnReservedM4         = (void*) (0UL), /* Reserved */
        .pfnReservedM3         = (void*) (0UL), /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,
        .pfnPM_Handler         = (void*) PM_Handler,             /*  0 Power Manager */
        .pfnSYSCTRL_Handler    = (void*) SYSCTRL_Handler,        /*  1 System Control */
        .pfnWDT_Handler        = (void*) WDT_Handler,            /*  2 Watchdog Timer */
        .pfnRTC_Handler        = (void*) RTC_Handler,            /*  3 Real-Time Counter */
        .pfnEIC_Handler        = (void*) EIC_Handler,            /*  4 External Interrupt Controller */
        .pfnNVMCTRL_Handler    = (void*) NVMCTRL_Handler,        /*  5 Non-Volatile Memory Controller */
        .pfnEVSYS_Handler      = (void*) EVSYS_Handler,          /*  6 Event System Interface */
        .pfnSERCOM0_Handler    = (void*) SERCOM0_Handler,        /*  7 Serial Communication Interface 0 */
        .pfnSERCOM1_Handler    = (void*) SERCOM1_Handler,        /*  8 Serial Communication Interface 1 */
        .pfnSERCOM2_Handler    = (void*) SERCOM2_Handler,        /*  9 Serial Communication Interface 2 */
        .pfnSERCOM3_Handler    = (void*) SERCOM3_Handler,        /* 10 Serial Communication Interface 3 */
        .pfnSERCOM4_Handler    = (void*) SERCOM4_Handler,        /* 11 Serial Communication Interface 4 */
        .pfnSERCOM5_Handler    = (void*) SERCOM5_Handler,        /* 12 Serial Communication Interface 5 */
        .pfnTC0_Handler        = (void*) TC0_Handler,            /* 13 Basic Timer Counter 0 */
        .pfnTC1_Handler        = (void*) TC1_Handler,            /* 14 Basic Timer Counter 1 */
        .pfnTC2_Handler        = (void*) TC2_Handler,            /* 15 Basic Timer Counter 2 */
        .pfnTC3_Handler        = (void*) TC3_Handler,            /* 16 Basic Timer Counter 3 */
        .pfnTC4_Handler        = (void*) TC4_Handler,            /* 17 Basic Timer Counter 4 */
        .pfnTC5_Handler        = (void*) TC5_Handler,            /* 18 Basic Timer Counter 5 */
        .pfnTC6_Handler        = (void*) TC6_Handler,            /* 19 Basic Timer Counter 6 */
        .pfnTC7_Handler        = (void*) TC7_Handler,            /* 20 Basic Timer Counter 7 */
        .pfnADC_Handler        = (void*) ADC_Handler,            /* 21 Analog Digital Converter */
        .pfnAC_Handler         = (void*) AC_Handler,             /* 22 Analog Comparators */
        .pfnDAC_Handler        = (void*) DAC_Handler            /* 23 Digital Analog Converter */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        uint32_t *pSrc, *pDest;

        /* Call SystemInit() */
        SystemInit();

        /* Initialize the relocate segment */
        pSrc = &_etext;
        pDest = &_srelocate;

        if (pSrc != pDest) {
                for (; pDest < &_erelocate;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_szero; pDest < &_ezero;) {
                *pDest++ = 0;
        }

        /* Set the vector table base address */
        pSrc = (uint32_t *) & _sfixed;
        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        /* Initialize the C library */
        __libc_init_array();

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}

/*
*
* Out commented by SEGGER since we are using our own HardFault_Handler
* in RTOSInit_SAMD20_CMSIS
*
void HardFault_Handler(void)
{
	int i = 1;
        while (i == 1) {
        }
}
*/
