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
File    : Arm_TZ_SecureFunctions.c
Purpose : Support for the ARMv8-M secure world

Notes: [1] This code must be located/executed in the secure world.
*/

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetCONTROL_s(void);
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetPSP_s    (void);
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetPSPLIM_s (void);
__attribute__((cmse_nonsecure_entry)) void          Arm_TZ_SetCONTROL_s(unsigned long CONTROL_s);
__attribute__((cmse_nonsecure_entry)) void          Arm_TZ_SetPSP_s    (unsigned long PSP_s);
__attribute__((cmse_nonsecure_entry)) void          Arm_TZ_SetPSPLIM_s (unsigned long PSPLIM_s);

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Arm_TZ_GetCONTROL_s()
*
*  Function description
*    Returns the CONTROL_s value
*
*  Return value
*    CONTROL_s value
*/
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetCONTROL_s(void) {
  register unsigned long Reg;
  Reg = 0u;
  __asm volatile("MRS %0, CONTROL"
                 : "+r" (Reg)  // Output result
                 :             // Inputs
                 :);           // Clobbered list
  return Reg;
}

/*********************************************************************
*
*       Arm_TZ_GetPSP_s()
*
*  Function description
*    Returns the PSP_s value
*
*  Return value
*    PSP_s value
*/
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetPSP_s(void) {
  register unsigned long Reg;
  Reg = 0u;
  __asm volatile("MRS %0, PSP"
                 : "+r" (Reg)  // Output result
                 :             // Inputs
                 :);           // Clobbered list
  return Reg;
}

/*********************************************************************
*
*       Arm_TZ_GetPSPLIM_s()
*
*  Function description
*    Returns the PSPLIM_s value
*
*  Return value
*    PSPLIM_s value
*/
__attribute__((cmse_nonsecure_entry)) unsigned long Arm_TZ_GetPSPLIM_s(void) {
  register unsigned long Reg;
  Reg = 0u;
  __asm volatile("MRS %0, PSPLIM"
                 : "+r" (Reg)  // Output result
                 :             // Inputs
                 :);           // Clobbered list
  return Reg;
}


/*********************************************************************
*
*       Arm_TZ_SetCONTROL_s()
*
*  Function description
*    Sets the CONTROL_s value
*
*  Parameters
*    CONTROL_s: New CONTROL_s value
*/
__attribute__((cmse_nonsecure_entry)) void Arm_TZ_SetCONTROL_s(unsigned long CONTROL_s) {
  __asm volatile("MSR CONTROL, %0"
                 :                  // Output result
                 : "r" (CONTROL_s)  // Inputs
                 :);                // Clobbered list
}

/*********************************************************************
*
*       Arm_TZ_SetPSP_s()
*
*  Function description
*    Sets the PSP_s value
*
*  Parameters
*    PSP_s: New PSP_s value
*/
__attribute__((cmse_nonsecure_entry)) void Arm_TZ_SetPSP_s(unsigned long PSP_s) {
  __asm volatile("MSR PSP, %0"
                  :              // Output result
                  : "r" (PSP_s)  // Inputs
                  );             // Clobbered list
}

/*********************************************************************
*
*       Arm_TZ_SetPSPLIM_s()
*
*  Function description
*    Sets the PSPLIM_s value
*
*  Parameters
*    PSPLIM_s: New PSPLIM_s value
*/
__attribute__((cmse_nonsecure_entry)) void Arm_TZ_SetPSPLIM_s(unsigned long PSPLIM_s) {
  __asm volatile("MSR PSPLIM, %0"
                 :                 // Output result
                 : "r" (PSPLIM_s)  // Inputs
                 );                // Clobbered list
}


/*************************** End of file ****************************/
