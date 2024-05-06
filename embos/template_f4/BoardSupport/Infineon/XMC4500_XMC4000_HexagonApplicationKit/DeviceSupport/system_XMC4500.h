/**************************************************************************//**
 * @file     System_XMC4500.h
 * @brief    Header file for the XMC4500-Series systeminit
 *           
 * @version  V1.4
 * @date     24 April 2012
 *
 * @note
 * Copyright (C) 2011-2012 Infineon Technologies AG. All rights reserved.

 *
 * @par
 * Infineon Technologies AG (Infineon) is supplying this software for use with Infineon’s microcontrollers.  
 * This file can be freely distributed within development tools that are supporting such microcontrollers. 

 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 ******************************************************************************/


#ifndef __SYSTEM_XMC4500_H
#define __SYSTEM_XMC4500_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock)  */
extern uint32_t AllowPLLInitByStartup(void); /*!< Enable PLL init by startup?*/
void SystemInit(void);


/* Clock definitions, do not modify! */
#define SCU_CLOCK_CRYSTAL              1
				
				
/*				
 * mandatory clock parameters **************************************************				
 */				
/* source for clock generation				
 * range: SCU_CLOCK_CRYSTAL (crystal or external clock at crystal input)				
 *        				
 **************************************************************************************/				
				
#define	SCU_PLL_CLOCK_INPUT	SCU_CLOCK_CRYSTAL
#define	CLOCK_CRYSTAL_FREQUENCY	12000000		
#define	CLOCK_BACK_UP	24000000

/* OSC_HP setup parameters */				
#define	OSC_HP_MODE	0		
#define OSCHPWDGDIV 2
				
/* MAIN PLL setup parameters */				
				
				
#define 	PLL_K1DIV	1		
#define 	PLL_K2DIV	3		
#define 	PLL_PDIV	1		
#define 	PLL_NDIV	79
				
#define 	PLL_K2DIV_STEP_1	19	//PLL output is 24Mhz	
#define 	PLL_K2DIV_STEP_2	7	//PLL output to 60Mhz	
#define 	PLL_K2DIV_STEP_3	4	//PLL output to 96Mhz	

#define 	USBPLL_PDIV     0	
#define 	USBPLL_NDIV     23
#define   	USBDIV          2

/* ====== DEPRECATED DEPRECATED DEPECATED DEPRECATED DEPRECATED =========== */ 
#define	CLOCK_OSC_HP	   (CLOCK_CRYSTAL_FREQUENCY)
#define PLL_DIV_RATIO    ((PLL_NDIV + 1)/((PLL_PDIV + 1) * (PLL_K2DIV + 1)))
#define SYSTEM_FREQUENCY (CLOCK_OSC_HP * PLL_DIV_RATIO)
/* ===================== END OF DEPRECATED SECTION ======================== */

#ifdef __cplusplus
}
#endif


#endif
