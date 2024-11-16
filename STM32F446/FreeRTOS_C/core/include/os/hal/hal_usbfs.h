/**
 * @file hal_usbfs.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief STM32 USB FS HAL
 * @date 2024-11-14
 * @version 0.1
 * 
 */

#ifndef _HAL_USBFS_H_
#define _HAL_USBFS_H_
#include <stm32f4xx.h>
#include <stdbool.h>


// Create a reference to the USB OTG Periphrial
#ifndef USB
#define USB USB_OTG_FS
#endif

// Create a reference to the USB Device Specific Registers
#ifndef USBD
#define USBD ((USB_OTG_DeviceTypeDef*)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#endif

// Create a pointer to the USB Power Clock and Gating Control Regiser
#ifndef USB_PCGCCTL
#define USB_PCGCCTL ((uint32_t*)(USB_OTG_FS_PERIPH_BASE+ USB_OTG_PCGCCTL_BASE))
#endif

// Pointer to the USB endpoint FIFOs
#ifndef USB_EP_FIFO
#define USB_EP_FIFO(ep) ((uint32_t*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_FIFO_BASE + (ep*USB_OTG_FIFO_SIZE)))
#endif

#ifndef USB_EP_IN
#define USB_EP_IN(ep) ((USB_OTG_INEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_IN_ENDPOINT_BASE+(ep*sizeof(USB_OTG_INEndpointTypeDef))))
#endif
#ifndef USB_EP_OUT
#define USB_EP_OUT(ep) ((USB_OTG_OUTEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_OUT_ENDPOINT_BASE+(ep*sizeof(USB_OTG_OUTEndpointTypeDef))))
#endif

enum hal_usb_err {
    eHUSB_OK = 0,
    eHUSB_NULL,
    eHUSB_TIMEOUT,
    eHUSB_NOCLK,
    eHUSB_DISCONNECT,
};

enum hal_usb_phy {eHUSB_PHY_ULPI, eHUSB_PHY_EMBEDDED};

enum hal_usb_DCFG_frame_interval {
    USBD_DCFG_FRAME_INTERVAL_80 = 0U,
    USBD_DCFG_FRAME_INTERVAL_85 = 1U,
    USBD_DCFG_FRAME_INTERVAL_90 = 2U,
    USBD_DCFG_FRAME_INTERVAL_95 = 3U
};

static inline enum hal_usb_err hal_usb_flushTxFifo(uint32_t num){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));

    count = 0U;
    SET_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH | (num << USB_OTG_GRSTCTL_TXFNUM_Pos));

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_err hal_usb_flushRxFifo(void){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));
    count = 0U;
    USB->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_err hal_usb_getInfo(void){
    if(!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN)) return eHUSB_NOCLK;
    if(READ_BIT(USBD->DCFG, USB_OTG_DCTL_SDIS)) return eHUSB_DISCONNECT;
    return eHUSB_OK;
}

static inline enum hal_usb_err hal_usb_EP_setStall(uint8_t ep, bool stall){

}


#endif

