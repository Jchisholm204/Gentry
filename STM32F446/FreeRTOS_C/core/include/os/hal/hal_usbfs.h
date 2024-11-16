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

#define MAX_EP          6
#define MAX_RX_PACKET   128
#define MAX_CONTROL_EP  1
#define MAX_FIFO_SZ     320  /*in 32-bit chunks */

#define RX_FIFO_SZ      ((4 * MAX_CONTROL_EP + 6) + ((MAX_RX_PACKET / 4) + 1) + (MAX_EP * 2) + 1)

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

enum hal_usb_ep {
    eHUSB_EP_IN =  0x80,
    eHUSB_EP_OUT = 0x00,
};

// HAL USB Status
enum hal_usb_sts {
    eHUSB_OK = 0,
    eHUSB_NULL,
    eHUSB_TIMEOUT,
    eHUSB_NOCLK,
    eHUSB_DISCONNECT,
    eHUSB_EP_STALL,
};

enum hal_usb_phy {eHUSB_PHY_ULPI, eHUSB_PHY_EMBEDDED};

enum hal_usb_DCFG_frame_interval {
    USBD_DCFG_FRAME_INTERVAL_80 = 0U,
    USBD_DCFG_FRAME_INTERVAL_85 = 1U,
    USBD_DCFG_FRAME_INTERVAL_90 = 2U,
    USBD_DCFG_FRAME_INTERVAL_95 = 3U
};

static inline enum hal_usb_sts hal_usb_ahbIdl(void){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_flushTxFifo(uint32_t num){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    hal_usb_ahbIdl();
    // Flush the TX Fifo corrisponding to the right EP
    SET_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH | (num << USB_OTG_GRSTCTL_TXFNUM_Pos));

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_flushRxFifo(void){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    hal_usb_ahbIdl();
    // Flush the RX Fifo corrisponding to the right EP
    USB->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

    do {
        count++;
        if(count > 200000U)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_getInfo(void){
    if(!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN)) return eHUSB_NOCLK;
    if(READ_BIT(USBD->DCFG, USB_OTG_DCTL_SDIS)) return eHUSB_DISCONNECT;
    return eHUSB_OK;
}


static inline enum hal_usb_sts hal_usb_EP_setStall(uint8_t ep, bool stall){
    if((ep & eHUSB_EP_IN) == eHUSB_EP_IN){
        ep &= (eHUSB_EP_IN-1);
        uint32_t ep_ctl = USB_EP_IN(ep)->DIEPCTL;
        if((ep_ctl & USB_OTG_DIEPCTL_USBAEP) == USB_OTG_DIEPCTL_USBAEP){
            if(stall)
                // Set the Stall Bit
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
            else {
                // Clear the Stall Bit and the NAK bit
                CLEAR_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_SD0PID_SEVNFRM | USB_OTG_DIEPCTL_CNAK);
            }
            USB_EP_IN(ep)->DIEPCTL = ep_ctl;
        }
    }
    else {
        uint32_t ep_ctl = USB_EP_OUT(ep)->DOEPCTL;
        if((ep_ctl & USB_OTG_DIEPCTL_USBAEP) == USB_OTG_DIEPCTL_USBAEP){
            if(stall)
                // Set the Stall Bit
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
            else {
                // Clear the Stall Bit and the NAK bit
                CLEAR_BIT(ep_ctl, USB_OTG_DIEPCTL_STALL);
                SET_BIT(ep_ctl, USB_OTG_DIEPCTL_SD0PID_SEVNFRM | USB_OTG_DIEPCTL_CNAK);
            }
            USB_EP_OUT(ep)->DOEPCTL = ep_ctl;
        }

    }
}

static inline enum hal_usb_sts hal_usb_EP_isStalled(uint8_t ep){
    if((ep & eHUSB_EP_IN) == eHUSB_EP_IN){
        ep &= (eHUSB_EP_IN-1);
        return (READ_BIT(USB_EP_IN(ep)->DIEPCTL, USB_OTG_DIEPCTL_STALL) ? eHUSB_EP_STALL : eHUSB_OK);
    }
    return (READ_BIT(USB_EP_OUT(ep)->DOEPCTL, USB_OTG_DIEPCTL_STALL) ? eHUSB_EP_STALL : eHUSB_OK);
}

static inline enum hal_usb_sts hal_usb_init(bool vbus_detect){
    // Enable the USB CLock
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    if(hal_usb_ahbIdl() != eHUSB_OK) return eHUSB_TIMEOUT;
    // Force device mode, Ensure use of internal PHY
    SET_BIT(USB->GRSTCTL, USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL);
    // Set Turnaround time to reflect higest frequency used on AHB bus
    MODIFY_REG(USB->GRSTCTL, USB_OTG_GUSBCFG_TRDT, 0x6 << USB_OTG_GUSBCFG_TRDT_Pos);
    if(vbus_detect){
        // Enable VBUS detection and enable the USB FS PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_VBDEN | USB_OTG_GCCFG_PWRDWN);
    }
    else {
        // Enable VBUS Override and Set the BValid to 1
        SET_BIT(USB->GOTGCTL, USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL);
        // Enable the USB FS PHY
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN);
    }
    // Restart the USB PHY
    *USB_PCGCCTL = 0;
    // Soft Disconnect Device
    SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    // Setup Periodic Schedule Interval (25%)
    CLEAR_BIT(USBD->DCFG, USB_OTG_DCFG_PERSCHIVL);
    // Set USBD to use internal FS PHY 
    MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DSPD, USB_OTG_DCFG_DSPD);

    // Setup Interrupts
    // Unmask EP Interrupts
    SET_BIT(USBD->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
    // Unmask Core Interrupts
    SET_BIT(USB->GINTMSK,
            // Reset Events
            USB_OTG_GINTMSK_USBRST   |
            // USB Enumeration Done Event
            USB_OTG_GINTMSK_ENUMDNEM |
            // Start of frame
            USB_OTG_GINTMSK_SOFM     |
            // Bus suspended
            USB_OTG_GINTMSK_USBSUSPM |
            // Remote wakeup events
            USB_OTG_GINTMSK_WUIM     |
            // Enable IN endpoint interrupts
            USB_OTG_GINTMSK_IEPINT   |
            // RX Fifo non empty
            USB_OTG_GINTMSK_RXFLVLM
            );
    // Clear all pending interrupts
    USB->GINTSTS = 0xFFFFFFFF;
    // Unmask global interrupt bit
    USB->GAHBCFG = USB_OTG_GAHBCFG_GINT;
    // Set max RX FIFO size
    USB->GRXFSIZ = RX_FIFO_SZ;
    // setting up EP0 TX FIFO SZ as 64 byte
    USB->DIEPTXF0_HNPTXFSIZ = RX_FIFO_SZ | (0x10 << USB_OTG_DIEPTXF_INEPTXFD_Pos);
    return eHUSB_OK;
}

#endif

