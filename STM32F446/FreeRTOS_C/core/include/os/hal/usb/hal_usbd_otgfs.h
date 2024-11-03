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

struct hal_usb_config {
    uint32_t dev_endpoints;
    bool     en_SOF;
    bool     en_lpm;
    bool     en_vbus_sensing;
    bool     en_external_vbus;
    enum hal_usb_phy phy_sel;
};

static inline enum hal_usb_err hal_usb_CoreReset(uint32_t timeout_ticks){
    __IO uint32_t count = 0U;
    // Wait for AHB to be Idle
    do {
        count++;
        if(count > timeout_ticks)
            return eHUSB_TIMEOUT;
    } while(READ_BIT(USB->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL));

}

static inline enum hal_usb_err hal_usb_coreInit(struct hal_usb_config *pCfg){
    if(!pCfg) return eHUSB_NULL;
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


    }

}

static inline void hal_usb_init(bool vbus_detection){
    (void)vbus_detection;
}

static inline void hal_usb_IRQ(bool enable){
    (void)enable;
}


#endif


