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

enum hal_usb_err {eHUSB_OK = 0, eHUSB_NULL, eHUSB_TIMEOUT};

enum hal_usb_phy {eHUSB_PHY_ULPI, eHUSB_PHY_EMBEDDED};

enum USBD_DCFG_FRAME_INTERVAL {
    USBD_DCFG_FRAME_INTERVAL_80 = 0U,
    USBD_DCFG_FRAME_INTERVAL_85 = 1U,
    USBD_DCFG_FRAME_INTERVAL_90 = 2U,
    USBD_DCFG_FRAME_INTERVAL_95 = 3U
};


struct hal_usb_config {
    uint32_t dev_endpoints;
    bool     en_SOF;
    bool     en_lpm;
    bool     en_vbus_sensing;
    bool     en_external_vbus;
    enum hal_usb_phy phy_sel;
};

static inline enum hal_usb_err hal_usb_setDevMode(void){
    // Force clear of USB Mode
    CLEAR_BIT(USB->GUSBCFG, USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_FHMOD);
    // Force Device Mode
    SET_BIT(USB->GUSBCFG ,USB_OTG_GUSBCFG_FDMOD);
    for(unsigned int i = 0; i < 200000U; i++)
        if(READ_BIT(USB->GINTSTS, USB_OTG_GINTSTS_CMOD) == 0)
            return eHUSB_OK;
    return eHUSB_TIMEOUT;
}

static inline enum hal_usb_err hal_usb_CoreReset(uint32_t timeout_ticks){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > timeout_ticks)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));
    count = 0U;
    SET_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_CSRST);
    do {
        count++;
        if(count > timeout_ticks)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_CSRST) == USB_OTG_GRSTCTL_CSRST);
    return eHUSB_OK;

}

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


static inline enum hal_usb_err hal_usb_coreInit(struct hal_usb_config *pCfg){
    if(!pCfg) return eHUSB_NULL;
    enum hal_usb_err e = eHUSB_OK;;
    if(pCfg->phy_sel == eHUSB_PHY_ULPI){
        // Disable the ULPI PHY
        CLEAR_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN);
        // Initialize the ULPI Interface
        CLEAR_BIT(USB->GUSBCFG,
                USB_OTG_GUSBCFG_TSDPS      |
                USB_OTG_GUSBCFG_ULPIFSLS   |  
                USB_OTG_GUSBCFG_PHYSEL     | // Select USB 2.0 external ULPI PHY
                USB_OTG_GUSBCFG_ULPIEVBUSD | // Reset Vbus drive
                USB_OTG_GUSBCFG_ULPIEVBUSI   // Overcurrent Indicator (Use internal)
                );
        if(pCfg->en_external_vbus){
            SET_BIT(USB->GUSBCFG, USB_OTG_GUSBCFG_ULPIEVBUSD);
        }
        // Wait for Core Reset
        e = hal_usb_CoreReset(200000U);
    }
    else { // Embedded PHY
        // Disable the FS PHY
        SET_BIT(USB->GUSBCFG, USB_OTG_GUSBCFG_PHYSEL);
        // Wait for Core Reset
        e = hal_usb_CoreReset(200000U);
        // Power down the internal core
        CLEAR_BIT(USB->GCCFG, USB_OTG_GCCFG_PWRDWN);
    }
    return e;

}

static inline enum hal_usb_err hal_usb_devInit(struct hal_usb_config *pCfg){
    enum hal_usb_err e = eHUSB_OK;
    for(unsigned int i = 0; i < 15U; i++)
        USB->DIEPTXF[i] = 0U;
    if(pCfg->en_vbus_sensing == false){
        // Ensure the USB device is disconnected
        SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
        // Deactivate VBUS sensing
        CLEAR_BIT(USB->GCCFG, USB_OTG_GCCFG_VBDEN);
        // Enable Valid Session Override
        SET_BIT(USB->GOTGCTL, USB_OTG_GOTGCTL_VBVALOEN | USB_OTG_GOTGCTL_BVALOVAL);
    }
    else {
        // Enable VBUS sensing
        SET_BIT(USB->GCCFG, USB_OTG_GCCFG_VBDEN);
    }

    // Restart the PHY Clock
    *USB_PCGCCTL = 0U;

    // Device mode Configuration
    SET_BIT(USBD->DCFG, USBD_DCFG_FRAME_INTERVAL_80);
    if(pCfg->phy_sel == eHUSB_PHY_ULPI)
        MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DSPD, 1U);
    else
        MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DSPD, 3U);

    // Reset all USB FIFOs
    if((e = hal_usb_flushTxFifo(0x10U)) != eHUSB_OK) return e;
    if((e = hal_usb_flushRxFifo()) != eHUSB_OK) return e;

    // Clear all pending device interrupts
    USBD->DIEPMSK = 0U;
    USBD->DOEPMSK = 0U;
    USBD->DAINTMSK = 0U;

    // Configure Device Endpoints
    for(unsigned int i = 0U; i < pCfg->dev_endpoints; i++){
        if(READ_BIT(USB_EP_IN(i)->DIEPCTL, USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA){
            if(i == 0U){
                USB_EP_IN(i)->DIEPCTL = USB_OTG_DIEPCTL_SNAK;
            }
            else {
                USB_EP_IN(i)->DIEPCTL = USB_OTG_DIEPCTL_SNAK | USB_OTG_DIEPCTL_EPDIS;
            }
        }
        else {
            USB_EP_IN(i)->DIEPCTL = 0U;
        }
    }

}

static inline enum hal_usb_err hal_usb_init(struct hal_usb_config *pCfg){
    enum hal_usb_err e = eHUSB_OK;
    // Core Init
    e = hal_usb_coreInit(pCfg);
    if(e != eHUSB_OK) return e;
    // Force the USB PHY into device mode
    e = hal_usb_setDevMode();
    return e;
}

static inline void hal_usb_IRQ(bool enable){
    (void)enable;
}


#endif


