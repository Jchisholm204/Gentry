/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @version V0.2.0
  * @date    11-November-2016
  * @brief   CMSIS Cortex-Mx Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32h7xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "stm32h7xx.h"
#include "RTOS.h"

#define HSI_VALUE                  ((uint32_t)64000000)
#define HSE_VALUE                  ((uint32_t)25000000)
#define CSI_VALUE                  ((uint32_t) 4000000)

#define TIMEOUT_2MS                ((int32_t)    2)  // 2ms timeout value
#define TIMEOUT_5S                 ((int32_t) 5000)  // 5s timeout value
#define RCC_FLAG_MASK              ((uint8_t) 0x1F)
#define RCC_FLAG_HSIRDY            ((uint8_t) 0x22)
#define RCC_FLAG_PLLRDY            ((uint8_t) 0x39)
#define RCC_FLAG_HSIDIV            ((uint8_t) 0x25)
#define RCC_CLOCKTYPE_HCLK         ((uint32_t)0x02)
#define RCC_CLOCKTYPE_SYSCLK       ((uint32_t)0x01)
#define RCC_FLAG_HSERDY            ((uint8_t) 0x31)
#define RCC_FLAG_CSIRDY            ((uint8_t) 0x28)
#define RCC_CLOCKTYPE_D1PCLK1      ((uint32_t)0x04)
#define RCC_CLOCKTYPE_PCLK1        ((uint32_t)0x08)
#define RCC_CLOCKTYPE_PCLK2        ((uint32_t)0x10)
#define RCC_CLOCKTYPE_D3PCLK1      ((uint32_t)0x20)
#define RCC_PLLSOURCE_HSE          ((uint32_t)0x02)
//
// Note: Even though a frequency of 480Mhz is possible, the lifetime estimate
//       with VOS0 is under 5 years. Use VOS1 with 400Mhz instead.
//
#define HSI_CALIBRATION_VALUE      (16)
#define PLLM                       ( 2)
#define PLLN                       (64)
#define PLLP                       ( 2)
#define PLLQ                       ( 2)
#define PLLR                       ( 2)
#define PLLFRACN                   ( 0)
#define PLLVCOSEL                  (((uint32_t)0x0))
#define CLOCK_TYPE                 ((uint32_t)0x3F)
#define GET_RCC_FLAG(__FLAG__)     (((((((__FLAG__) >> 5) == 1)? RCC->CR :((((__FLAG__) >> 5) == 2) ? RCC->BDCR : ((((__FLAG__) >> 5) == 3)? RCC->CSR : ((((__FLAG__) >> 5) == 4)? RCC->RSR :RCC->CIFR))))  & ((uint32_t)1 << ((__FLAG__) & RCC_FLAG_MASK)))!= 0)? 1 : 0)

/******************************************************************************/

uint32_t SystemCoreClock;

#if (defined __GCC)
void SystemInit(void) __attribute__ ((optimize(O3)));
#else
void SystemInit(void);
#endif
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= RCC_CR_HSION;
  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;
  /* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEAF6ED7F;
  /* Reset D1CFGR register */
  RCC->D1CFGR = 0x00000000;
  /* Reset D2CFGR register */
  RCC->D2CFGR = 0x00000000;
  /* Reset D3CFGR register */
  RCC->D3CFGR = 0x00000000;
  /* Reset PLLCKSELR register */
  RCC->PLLCKSELR = 0x00000000;
  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000000;
  /* Reset PLL1DIVR register */
  RCC->PLL1DIVR = 0x00000000;
  /* Reset PLL1FRACR register */
  RCC->PLL1FRACR = 0x00000000;
  /* Reset PLL2DIVR register */
  RCC->PLL2DIVR = 0x00000000;
  /* Reset PLL2FRACR register */
  RCC->PLL2FRACR = 0x00000000;
  /* Reset PLL3DIVR register */
  RCC->PLL3DIVR = 0x00000000;
  /* Reset PLL3FRACR register */
  RCC->PLL3FRACR = 0x00000000;
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Supply configuration update enable */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  /* Configure the main internal regulator output voltage */
  MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, ((PWR_D3CR_VOS_1 | PWR_D3CR_VOS_0)));
  /* Delay after an RCC peripheral clock enabling */
  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY);

  /*----------------------------- HSE Configuration --------------------------*/
  /* Enable the External High Speed oscillator (with 25MHz crystal) */
  MODIFY_REG(RCC->CR, RCC_CR_HSEON, (uint32_t)(RCC_CR_HSEON));
  /* Wait till HSI is ready */
  while (GET_RCC_FLAG(RCC_FLAG_HSERDY) == RESET) {
  }

  /*-------------------------------- PLL Configuration -----------------------*/
  /* Check if the PLL is used as system clock or not */
  if (((uint32_t)(RCC->CFGR & RCC_CFGR_SWS)) != RCC_CFGR_SWS_PLL1) {
    /* Disable the main PLL. */
    CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);
    /* Wait till PLL is ready */
    while(GET_RCC_FLAG(RCC_FLAG_PLLRDY) != RESET) {
    }
    /* Configure the main PLL clock source, multiplication and division factors. */
    MODIFY_REG(RCC->PLLCKSELR, (RCC_PLLCKSELR_PLLSRC | RCC_PLLCKSELR_DIVM1), (RCC_PLLSOURCE_HSE | ( (PLLM) << 4U)));
    WRITE_REG(RCC->PLL1DIVR, (((PLLN - 1U) & RCC_PLL1DIVR_N1) | (((PLLP -1U) << 9U) & RCC_PLL1DIVR_P1) | (((PLLQ - 1U) << 16U) & RCC_PLL1DIVR_Q1) | (((PLLR - 1U) << 24U) & RCC_PLL1DIVR_R1)));
     /* Configure PLL  PLL1FRACN */
     MODIFY_REG(RCC->PLL1FRACR, RCC_PLL1FRACR_FRACN1, (uint32_t)(PLLFRACN) << POSITION_VAL(RCC_PLL1FRACR_FRACN1));
    /* Select PLL1 input reference frequency range: VCI */
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1RGE, (RCC_PLLCFGR_PLL1RGE_3));
    /* Select PLL1 output frequency range : VCO */
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1VCOSEL, (PLLVCOSEL));
    /* Enable PLL System Clock output. */
    SET_BIT(RCC->PLLCFGR, (RCC_PLLCFGR_DIVP1EN));
    /* Enable PLL1Q Clock output. */
    SET_BIT(RCC->PLLCFGR, (RCC_PLLCFGR_DIVQ1EN));
    /* Enable PLL1R  Clock output. */
    SET_BIT(RCC->PLLCFGR, (RCC_PLLCFGR_DIVR1EN));
    /* Enable PLL1FRACN. */
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN);
    /* Enable the main PLL. */
    SET_BIT(RCC->CR, RCC_CR_PLL1ON);
    /* Wait till PLL is ready */
    while (GET_RCC_FLAG(RCC_FLAG_PLLRDY) == RESET) {
    }
  } else {
    while (1);
  }

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
  must be correctly programmed according to the frequency of the CPU clock
  (HCLK) and the supply voltage of the device. */

  /* Increasing the CPU frequency */
  if(FLASH_ACR_LATENCY_7WS > (FLASH->ACR & FLASH_ACR_LATENCY)) {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_ACR_LATENCY_7WS));
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_7WS) {
      while (1);
    }
  }
   /*-------------------------- HCLK Configuration --------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK) {
    MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE, RCC_D1CFGR_HPRE_DIV2);
  }
  /*------------------------- SYSCLK Configuration -------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK) {
    MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1CPRE, RCC_D1CFGR_D1CPRE_DIV1);
    /* Check the PLL ready flag */
    if(GET_RCC_FLAG(RCC_FLAG_PLLRDY) == RESET) {
      while (1);
    }
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL1);
    while (((uint32_t)(RCC->CFGR & RCC_CFGR_SWS)) != RCC_CFGR_SWS_PLL1) {
    }
  }
  /*-------------------------- D1PCLK1 Configuration ---------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_D1PCLK1) == RCC_CLOCKTYPE_D1PCLK1) {
    MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, RCC_D1CFGR_D1PPRE_DIV4);
  }
  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1) {
    MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, (RCC_D2CFGR_D2PPRE1_DIV4));
  }
  /*-------------------------- PCLK2 Configuration ---------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2) {
    MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, (RCC_D2CFGR_D2PPRE2_DIV4));
  }
  /*-------------------------- D3PCLK1 Configuration ---------------------------*/
  if(((CLOCK_TYPE) & RCC_CLOCKTYPE_D3PCLK1) == RCC_CLOCKTYPE_D3PCLK1) {
    MODIFY_REG(RCC->D3CFGR, RCC_D2CFGR_D2PPRE1, (RCC_D3CFGR_D3PPRE_DIV4));
  }
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock , it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is CSI, SystemCoreClock will contain the CSI_VALUE(*)
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the CSI_VALUE(*),
  *             HSI_VALUE(**) or HSE_VALUE(***) multiplied/divided by the PLL factors.
  *
  *         (*) CSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *         (**) HSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             64 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (***)HSE_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void) {
  uint32_t pllp = 2, pllsource = 0, pllm = 2, pllfracen  =0, hsivalue = 0;
  float fracn1, pllvco = 0 ;

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS) {
  case 0x00:  /* HSI used as system clock source */
    if (GET_RCC_FLAG(RCC_FLAG_HSIDIV) != 0U) {
      SystemCoreClock = (uint32_t)(HSI_VALUE >> ((uint32_t)(READ_BIT(RCC->CR, RCC_CR_HSIDIV)) >> 3));
    } else {
      SystemCoreClock = (uint32_t)HSI_VALUE;
    }
    break;
  case 0x08:  /* CSI used as system clock  source */
    SystemCoreClock = CSI_VALUE;
    break;
  case 0x10:  /* HSE used as system clock  source */
    SystemCoreClock = HSE_VALUE;
    break;
  case 0x18:  /* PLL1 used as system clock  source */
    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1)>> 4)  ;
    pllfracen = RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN;
    fracn1 = (pllfracen* ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1)>> 3));
    switch (pllsource) {
    case 0x00:  /* HSI used as PLL clock source */
      if (GET_RCC_FLAG(RCC_FLAG_HSIDIV) != 0U) {
        hsivalue = (HSI_VALUE >> ((uint32_t)(READ_BIT(RCC->CR, RCC_CR_HSIDIV))>> 3)) ;
        pllvco = (hsivalue/ pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x1FFF) +1 );
      } else {
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x1FFF) +1 );
      }
      break;
    case 0x01:  /* CSI used as PLL clock source */
      pllvco = (CSI_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x1FFF) +1 );
      break;
    case 0x02:  /* HSE used as PLL clock source */
      pllvco = (HSE_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x1FFF) +1 );
      break;
    default:
      pllvco = (CSI_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x1FFF) +1 );
      break;
    }
    pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >>9) + 1 ) ;
    SystemCoreClock = (uint32_t) (pllvco/pllp);
    break;
  default:
    SystemCoreClock = CSI_VALUE;
    break;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
