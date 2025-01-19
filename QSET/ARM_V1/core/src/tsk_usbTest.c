/**
 * @file usb_tsk.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"
#include <stdio.h>
#include <memory.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usbd/usb_device.h"
#include "systime.h"
#include "tsk_usbTest.h"

char buf[100] = {0};
void vTsk_usbTest(void * pvParams){
    udev_init();
    (void)pvParams;
    // vTaskDelete(NULL);
    for(;;){
        // printf("Writing to USB\n");
        struct systime t;
        // char msg[] = "Hello ";
        // udev_write(msg, sizeof(msg));
        systime_fromTicks(xTaskGetTickCount(), &t);
        // int slen = sprintf(buf, "%s", t.str);
        // udev_write(t.str, 8);
        // udev_write("\n", 1);
        vTaskDelay(1000);
    }
}



