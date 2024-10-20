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
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "os/config/FreeRTOSConfig.h"
#include "task.h"
#include "os/drivers/serial.h"
#include "string.h"
#include "os/hal/hal_clock.h"
#include "os/hal/hal_gpio.h"
#include "os/config/pin_cfg.h"
#include "os/drivers/canbus.h"
#include "tusb_config.h"
#include "tusb_option.h"
#include "device/dcd.h"
#include "portable/st/stm32_fsdev/fsdev_stm32.h"
#include "device/usbd.h"
#include "tusb.h"

void vUSB_tsk(void * pvParams){
    (void)pvParams;
    printf("USB Task Online\n");
    
    tud_init(0);
    // NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN-2);
    // NVIC_EnableIRQ(OTG_FS_IRQn);

    for(;;){
        // tud_task();
        vTaskDelay(1);
    }

    // Main USB task delay to allow for USB processing
    vTaskDelay(100);
    printf("Deleting USB task\n");
    vTaskDelete(NULL);
}


void OTG_FS_IRQHandler(void) {
    // tud_int_handler(BOARD_TUD_RHPORT);
}
