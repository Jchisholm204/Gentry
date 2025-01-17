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


void vUSB_tsk(void * pvParams){
    udev_init();
    (void)pvParams;
    // vTaskDelete(NULL);
    for(;;){
        // printf("Writing to USB\n");
        struct systime t;
        systime_fromTicks(xTaskGetTickCount(), &t);
        udev_write(t.str, SYSTIME_STR_LEN);
        // int strsize = sprintf(msg, PRINT_CTIME(time));
        // // usbd_ep_unstall(&udev, CDC_TXD_EP);
        // usbd_ep_write(&udev, CDC_TXD_EP, msg, strsize);
        // int32_t rx_len = usbd_ep_read(&udev, CDC_RXD_EP, rx_buf, sizeof(rx_buf));
        // if(rx_len > 0){
        //     usbd_ep_write(&udev, CDC_TXD_EP, rx_buf, rx_len);
        //     printf("USBRX %d bytes: %s\n", rx_len, rx_buf);
        // }
        vTaskDelay(1000);
    }
}



