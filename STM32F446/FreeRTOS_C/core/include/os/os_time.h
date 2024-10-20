/**
 * @file os_time.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _OS_TIME_H_
#define _OS_TIME_H_

#include "FreeRTOS.h"
#include "os/config/FreeRTOSConfig.h"

struct ctime {
    int hrs, mins, secs, msecs;
};

static inline void cTimeGet(TickType_t ticks, struct ctime *t){
    if (!t)
        return;
    float tms = ((float)ticks) / ((float)configTICK_RATE_HZ) * 1000.0;
    t->msecs = ((int)tms) % 1000;
    int secs = ((int)(tms + 500) / 1000);
    t->secs = secs % 60;
    int mins = (secs / 60);
    t->mins = mins % 60;
    t->hrs = (mins / 60);
}

#define PRINT_CTIME(ct) "%02d:%02d:%02d.%03d\n", ct.hrs, ct.mins, ct.secs, ct.msecs
// #define PRINT_CTIME(msg, ct) \
    // msg ": " "%02d:%02d:%02d.%03d", ct.hrs, ct.mins, ct.secs, ct.msecs
#endif