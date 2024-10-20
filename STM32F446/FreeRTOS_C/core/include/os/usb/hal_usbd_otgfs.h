/**
 * @file hal_usbd_otgfs.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief USB OTG FS Device HAL
 * @date 2024-10-18
 * @version 0.1
 * 
 */

#ifndef _HAL_USBD_OTGFS_H_
#define _HAL_USBD_OTGFS_H_

#include "os/hal/hal_gpio.h"
#include "os/config/nvic.h"
#include <stm32f4xx.h>

#define USB_MAX_EP 6U
#define USB_MAX_RX_PACKET 128U
#define USB_MAX_CTL_EP 1U
#define USB_MAX_FIFO_SIZE 320U // In 32 bit chunks
#define USB_RX_FIFO_SIZE ((4*USB_MAX_CTL_EP+6) + ((USB_MAX_RX_PACKET/4)+1) + (USB_MAX_EP*2)+1)

#define USB USB_OTG_FS
#define USBD ((USB_OTG_DeviceTypeDef*)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_PCGCCTL ((uint32_t*)(USB_OTG_FS_PERIPH_BASE+ USB_OTG_PCGCCTL_BASE))
#define USB_EP_FIFO(ep) ((uint32_t*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_FIFO_BASE + (ep*USB_OTG_FIFO_SIZE)))
#define USB_EP_IN(ep) ((USB_OTG_INEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_IN_ENDPOINT_BASE+(ep*sizeof(USB_OTG_INEndpointTypeDef))))
#define USB_EP_OUT(ep) ((USB_OTG_OUTEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_OUT_ENDPOINT_BASE+(ep*sizeof(USB_OTG_OUTEndpointTypeDef))))

static inline void hal_usb_init(bool vbus_detection){
    // Enable USB Clock
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    // Wait for AHB Idle
    while(!READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));
    // Configure USB as device only | Use USB PHY | Timing Config
    SET_BIT(USB->GUSBCFG, USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | (0x06U << USB_OTG_GUSBCFG_TRDT_Pos));
    if(vbus_detection){
        // Powerup USB PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN | USB_OTG_GCCFG_VBDEN);
    }
    else {
        // Ignore voltage sensing and assume valid USB session
        SET_BIT(USB->GOTGCTL, USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL);
        // Powerup USB PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN);
    }
    // Restart PHY
    *USB_PCGCCTL = 0x200B8000;
    // Soft Disconnect Device
    SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    // Setup Speed and frame interval
    CLEAR_BIT(USBD->DCFG, USB_OTG_DCFG_PERSCHIVL | USB_OTG_DCFG_DSPD);
    SET_BIT(USBD->DCFG, (0x3U << USB_OTG_DCFG_DSPD_Pos));
    // Set up max RX FIFO size
    USB->GRXFSIZ = USB_RX_FIFO_SIZE;
    USB->DIEPTXF[0] = USB_RX_FIFO_SIZE | (0x10 << 16);
}

static inline void hal_usb_IRQ(bool enable){
    if(enable){
        // Unmask endpoint interrupts
        SET_BIT(USBD->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
        SET_BIT(USB->GINTMSK, USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM | USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT | USB_OTG_GINTMSK_RXFLVLM);
        // Clear pending interrupts
        USB->GINTSTS = 0xFFFFFFFFUL;
        // Unmask Global Interrupt
        SET_BIT(USB->GAHBCFG, USB_OTG_GAHBCFG_GINT);
        NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN-2);
        NVIC_EnableIRQ(OTG_FS_IRQn);
    }
    else{
        // Mask endpoint interrupts
        CLEAR_BIT(USBD->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
        CLEAR_BIT(USB->GINTMSK, USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM | USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT | USB_OTG_GINTMSK_RXFLVLM);
        // Clear pending interrupts
        USB->GINTSTS = 0xFFFFFFFFUL;
        // Unmask Global Interrupt
        CLEAR_BIT(USB->GAHBCFG, USB_OTG_GAHBCFG_GINT);
        NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}

static inline void hal_usb_connect(bool connect){
    if (connect) {
        CLEAR_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    } else {
        SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    }
}

#endif


