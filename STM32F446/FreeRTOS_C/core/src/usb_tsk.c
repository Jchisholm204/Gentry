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
#include "os/hal/usb/hal_usbd_otgfs.h"

void vUSB_tsk(void * pvParams){
    (void)pvParams;

    // Setup USB GPIO
    GPIOA->MODER &= ~(0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DM)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->MODER &= (uint32_t)~(uint32_t)((uint32_t)0x3UL << (uint32_t)(PINNO(PIN_USB_DP)*2UL));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DP)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DP)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DP) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DP) & 7)*4));

    hal_usb_init(true);
    hal_usb_connect(true);

    // for(;;){
    //     // if(reset_detected)
    //     //     printf("USB reset detected\n");
    //     // else
    //     //     USB_RemoteWake();
    //     // if(reset_ended)
    //     //     printf("USB reset finished\n");
    //     vTaskDelay(10);
    // }
    // Main USB task delay to allow for USB processing
    vTaskDelay(100);
    printf("Deleting USB task\n");
    vTaskDelete(NULL);
}


// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    uint32_t gintsts = USB_OTG_FS->GINTSTS;

    // Check for USB reset interrupt (used when the USB host resets the device)
    if (gintsts & USB_OTG_GINTSTS_USBRST) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_USBRST;  // Clear interrupt flag
    }

    // Handle enumeration done (when device enumeration is complete)
    if (gintsts & USB_OTG_GINTSTS_ENUMDNE) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;  // Clear interrupt flag
    }

    // Handle RX FIFO level (data received on control endpoint)
    if (gintsts & USB_OTG_GINTSTS_RXFLVL) {
        // Add RX FIFO processing code here...
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_RXFLVL;   // Clear interrupt flag
    }

    if (gintsts & USB_OTG_GINTSTS_NPTXFE) {
        // Add RX FIFO processing code here...
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_NPTXFE;   // Clear interrupt flag
    }

    // Start of Frame interrupt
    if (gintsts & USB_OTG_GINTSTS_SOF) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_SOF;      // Clear interrupt flag
    }

    // Handle Global IN NAK Effective interrupt
    if (gintsts & USB_OTG_GINTSTS_GINAKEFF) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_GINAKEFF; // Clear interrupt flag
    }

    // Handle other interrupt flag (unknown, 0x10)
    if (gintsts & 0x10) {
        USB_OTG_FS->GINTSTS = 0x10;                    // Clear interrupt flag
    }

    // Handle OTG interrupt
    if (gintsts & USB_OTG_GINTSTS_OTGINT) {
        uint32_t goint = USB_OTG_FS->GOTGINT;
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_OTGINT;   // Clear interrupt flag
    }

    // Clear other necessary interrupt flags if needed
}

