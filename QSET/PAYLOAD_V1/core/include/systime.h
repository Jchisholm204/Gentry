/**
 * @file ctime.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Function and macros to get the system current tiem as a string
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _CTIME_H_
#define _CTIME_H_
#include "FreeRTOS.h"


#define SYSTIME_USE_STR

#ifdef SYSTIME_USE_STR
#define SYSTIME_STR_LEN 13
#else
#define SYSTIME_PRINT(st) "%02d:%02d:%02d.%03d\n", st.hrs, st.mins, st.secs, st.msecs
#endif

struct systime {
    int hrs, mins, secs, msecs;
#ifdef SYSTIME_USE_STR
    char str[SYSTIME_STR_LEN];
#endif
};

#ifdef SYSTIME_USE_STR
// Convert the systemtime struct into a string
// SYSTIME_USE_STR must be defined
// String is stored in the systime.str field
static inline size_t systime_getStr(struct systime *pT){
#if (SYSTIME_STR_LEN >= 13)
    if(pT == NULL) return 0;
    char *s = pT->str;
    s[0] = (char)(pT->hrs / 10) + '0';
    s[1] = (char)(pT->hrs % 10) + '0';
    s[2] = ':';
    s[3] = (char)(pT->mins / 10) + '0';
    s[4] = (char)(pT->mins % 10) + '0';
    s[5] = ':';
    s[6] = (char)(pT->secs / 10) + '0';
    s[7] = (char)(pT->secs % 10) + '0';
    s[8] = '.';
    // Ensure milliseconds are always 3 digits
    int msecs = pT->msecs;
    s[9] = (char)((msecs / 100) % 10) + '0'; // Hundreds place
    s[10] = (char)((msecs / 10) % 10) + '0'; // Tens place
    s[11] = (char)(msecs % 10) + '0';        // Units place
    s[12] = '\n';
    s[13] = '\0';
    return SYSTIME_STR_LEN;
#else
    return 0;
#endif
}
#endif

/**
 * @brief Get the current time from a ticks value
 *
 * @param ticks System ticks
 * @param pT pointer to the ctime struct
 */
static inline void systime_fromTicks(TickType_t ticks, struct systime *pT){
    if (!pT)
        return;
    // Setup the struct
    float tms = ((float)ticks) / ((float)configTICK_RATE_HZ) * 1000;
    pT->msecs = ((int)tms) % 1000;
    int secs = ((int)(tms + 500) / 1000);
    pT->secs = secs % 60;
    int mins = (secs / 60);
    pT->mins = mins % 60;
    pT->hrs = (mins / 60);
#ifdef SYSTIME_USE_STR
    // Get the string representation
    (void)systime_getStr(pT);
#endif
}


#endif
