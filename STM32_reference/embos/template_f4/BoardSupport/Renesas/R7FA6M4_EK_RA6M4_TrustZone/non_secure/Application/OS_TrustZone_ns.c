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
File    : OS_TrustZone_ns.c
Purpose : embOS sample program for ARMv8M TrustZone
*/

#include "RTOS.h"

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void          IncrementCounter_s (void);

unsigned long Arm_TZ_GetCONTROL_s(void);
unsigned long Arm_TZ_GetPSP_s    (void);
unsigned long Arm_TZ_GetPSPLIM_s (void);
void          Arm_TZ_SetCONTROL_s(unsigned long CONTROL_s);
void          Arm_TZ_SetPSP_s    (unsigned long PSP_s);
void          Arm_TZ_SetPSPLIM_s (unsigned long PSPLIM_s);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static  OS_STACKPTR int StackHP[128], StackLP[128];  // Task stacks
static  OS_STACKPTR int StackHP_s[256];              // Should be located in secure memory
static  OS_TASK         TCBHP, TCBLP;                // Task control blocks
static  int             Counter_ns;

static OS_ARM_TZ_SECURE_API_LIST Arm_TZ_ApiList = {  // These functions must be placed in the secure memory
   Arm_TZ_GetCONTROL_s
  ,Arm_TZ_GetPSP_s
  ,Arm_TZ_GetPSPLIM_s
  ,Arm_TZ_SetCONTROL_s
  ,Arm_TZ_SetPSP_s
  ,Arm_TZ_SetPSPLIM_s
};

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
static void HPTask(void) {
  //
  // Extend the task context for the secure world.
  //
  OS_ARM_TZ_SetTaskContextExtension(&Arm_TZ_ApiList, StackHP_s, sizeof(StackHP_s));
  while (1) {
    IncrementCounter_s();  // Call secure function and increment secure counter
    Counter_ns++;          // Increment non-secure counter
    OS_TASK_Delay(50);
  }
}

static void LPTask(void) {
  while (1) {
    OS_TASK_Delay(200);
  }
}

/*********************************************************************
*
*       main()
*/
int main(void) {
  OS_Init();    // Initialize embOS
  OS_InitHW();  // Initialize required hardware
  OS_TASK_CREATE(&TCBHP, "HP Task", 100, HPTask, StackHP);
  OS_TASK_CREATE(&TCBLP, "LP Task",  50, LPTask, StackLP);
  OS_Start();   // Start embOS
  return 0;
}

/*************************** End of file ****************************/
