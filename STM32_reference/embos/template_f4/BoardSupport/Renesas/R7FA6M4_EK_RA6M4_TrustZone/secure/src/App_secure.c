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
File    : App_secure.c
Purpose : Sample secure application

Notes: [1] This code must be located/executed in the secure world.
*/

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
__attribute__((cmse_nonsecure_entry)) void IncrementCounter_s(void);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static unsigned int Counter_s;

/*********************************************************************
*
*       IncrementCounter_s()
*/
__attribute__((cmse_nonsecure_entry)) void IncrementCounter_s(void) {
  Counter_s++;
}

/*************************** End of file ****************************/
