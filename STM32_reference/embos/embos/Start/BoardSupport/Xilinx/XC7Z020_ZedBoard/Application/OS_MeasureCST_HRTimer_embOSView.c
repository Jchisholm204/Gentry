/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2022 SEGGER Microcontroller GmbH                  *
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
*       OS version: V5.18.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
File    : OS_MeasureCST_HRTimer_embOSView.c
Purpose : embOS sample program that measures the embOS context
          switching time and displays the result in the terminal
          window of embOSView. It is completely generic and runs on
          every target that is configured for embOSView.
*/

#include "RTOS.h"
#include <stdio.h>

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static OS_STACKPTR int StackHP[128], StackLP[128];  // Task stacks
static OS_TASK         TCBHP, TCBLP;                // Task control blocks
static OS_U64          Time;
static char            acBuffer[100];               // Output buffer

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       HPTask()
*/
static void HPTask(void) {
  while (1) {
    OS_TASK_Suspend(NULL);               // Suspend high priority task
    Time = OS_TIME_Get_Cycles() - Time;  // Stop measurement
  }
}

/*********************************************************************
*
*       LPTask()
*/
static void LPTask(void) {
  OS_U64 MeasureOverhead;  // Time for Measure Overhead
  OS_U32 v;                // Real context switching time

  while (1) {
    //
    // Measure overhead for time measurement so we can take this into account by subtracting it
    // This is done inside the while()-loop to mitigate possible effects of an instruction cache
    //
    MeasureOverhead = OS_TIME_Get_Cycles();
    MeasureOverhead = OS_TIME_Get_Cycles() - MeasureOverhead;
    //
    // Perform actual measurements
    //
    Time = OS_TIME_Get_Cycles();                  // Start measurement
    OS_TASK_Resume(&TCBHP);                       // Resume high priority task to force task switch
    Time -= MeasureOverhead;                      // Calculate real context switching time (w/o overhead measurement)
    v  = (OS_U32)OS_TIME_ConvertCycles2ns(Time);  // Convert cycles to nanoseconds
    sprintf(acBuffer, "Context switch time: %lu.%.3lu usec\r", (v / 1000uL), (v % 1000uL));  // Create result text
    OS_COM_SendString(acBuffer);                  // Print out result
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

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
