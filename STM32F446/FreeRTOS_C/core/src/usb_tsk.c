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

volatile int iqr_encounters = 0;

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
    hal_usb_IRQ(true);
    SET_BIT(USBD->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
    SET_BIT(USB->GINTMSK, USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM | USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT | USB_OTG_GINTMSK_RXFLVLM);
    // Clear pending interrupts
    USB->GINTSTS = 0xFFFFFFFFUL;
    // Unmask Global Interrupt
    SET_BIT(USB->GAHBCFG, USB_OTG_GAHBCFG_GINT);
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN - 2);
    NVIC_EnableIRQ(OTG_FS_IRQn);
    // for(int i = 0; i < 6; i++){
    //     SET_BIT(USB_EP_OUT(i)->DOEPCTL, USB_OTG_DOEPCTL_SNAK);
    //     SET_BIT(USB_EP_IN(i)->DIEPCTL, USB_OTG_DIEPCTL_SNAK);
    // }
    uint32_t usb_buf[100] = {0};
    uint16_t buf_addr = (uint16_t)usb_buf;

    USB->DIEPTXF[0] = 100 << 16;
    USB->DIEPTXF[0] |= buf_addr;
    USB_EP_OUT(0)->DOEPTSIZ |= 0x3U << USB_OTG_DOEPTSIZ_STUPCNT_Pos;
    gpio_set_mode(PIN_USB_GPIO_OUT, GPIO_MODE_OUTPUT);
    gpio_write(PIN_USB_GPIO_OUT, 0);
    hal_usb_connect(true);
    int vlast = 0;
    for(;;){
        if(vlast != iqr_encounters){
            printf("ISR launched %d times\n", iqr_encounters);
            vlast = iqr_encounters;
        }
        vTaskDelay(10);
    }

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
    iqr_encounters++;
    uint32_t gintsts = USB_OTG_FS->GINTSTS;

    // Check for USB reset interrupt (used when the USB host resets the device)
    if (gintsts & USB_OTG_GINTSTS_USBRST) {
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_USBRST;  // Clear interrupt flag
        for(int i = 0; i < 6; i++){
            SET_BIT(USB_EP_OUT(i)->DOEPCTL, USB_OTG_DOEPCTL_SNAK);
            SET_BIT(USB_EP_IN(i)->DIEPCTL, USB_OTG_DIEPCTL_SNAK);
        }
        // USB_EP_IN(0)->
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

}

