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
#define USBD ((__IO USB_OTG_DeviceTypeDef*)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#endif

// Create a pointer to the USB Power Clock and Gating Control Regiser
#ifndef USB_PCGCCTL
#define USB_PCGCCTL ((__IO uint32_t*)(USB_OTG_FS_PERIPH_BASE+ USB_OTG_PCGCCTL_BASE))
#endif

// Pointer to the USB endpoint FIFOs
#ifndef USB_EP_FIFO
#define USB_EP_FIFO(ep) ((__IO uint32_t*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_FIFO_BASE + (ep*USB_OTG_FIFO_SIZE)))
#endif

#ifndef USB_EP_IN
#define USB_EP_IN(ep) ((__IO USB_OTG_INEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_IN_ENDPOINT_BASE+(ep*sizeof(USB_OTG_INEndpointTypeDef))))
#endif
#ifndef USB_EP_OUT
#define USB_EP_OUT(ep) ((__IO USB_OTG_OUTEndpointTypeDef*)(USB_OTG_FS_PERIPH_BASE+USB_OTG_OUT_ENDPOINT_BASE+(ep*sizeof(USB_OTG_OUTEndpointTypeDef))))
#endif

enum hal_usb_event {
    eHUSB_EVT_RESET,
    // Start of Frame
    eHUSB_EVT_SOF,
    // Suspend
    eHUSB_EVT_SUSP,
    // Wakeup
    eHUSB_EVT_WKUP,
    // USB Packet Transmitted
    eHUSB_EVT_EPTX,
    // USB Packet Recieved
    eHUSB_EVT_EPRX,
    // USB Setup Packet Recieved
    eHUSB_EVT_EPSETUP,
    eHUSB_EVT_ERR,
    eHUSB_EVTn,
};


enum hal_usb_phy {eHUSB_PHY_ULPI, eHUSB_PHY_EMBEDDED};

enum hal_usb_DCFG_frame_interval {
    USBD_DCFG_FRAME_INTERVAL_80 = 0U,
    USBD_DCFG_FRAME_INTERVAL_85 = 1U,
    USBD_DCFG_FRAME_INTERVAL_90 = 2U,
    USBD_DCFG_FRAME_INTERVAL_95 = 3U
};
enum hal_usb_ep {
    eHUSB_EP_IN  = 0x80,
    eHUSB_EP_OUT = 0x00,
};

enum hal_usb_eptype {
    eHUSB_EPTYPE_CONTROL    = 0x0,
    eHUSB_EPTYPE_ISOCHRONUS = 0x1,
    eHUSB_EPTYPE_BULK       = 0x2,
    eHUSB_EPTYPE_INTERRUPT  = 0x3,
};

// HAL USB Status
enum hal_usb_sts {
    eHUSB_OK = 0,
    eHUSB_NULL,
    eHUSB_TIMEOUT,
    eHUSB_NOCLK,
    eHUSB_DISCONNECT,
    eHUSB_EP_STALL,
    eHUSB_EP_FULL,
    eHUSB_EP_NENA,
};

typedef void (*hal_usb_evt_callback)(void *vpdev, enum hal_usb_event evt, uint8_t epn);


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

static inline enum hal_usb_sts hal_usb_connect(bool connect){
    if(connect)
        CLEAR_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    else
        SET_BIT(USBD->DCTL, USB_OTG_DCTL_SDIS);
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_setAddress(uint8_t addr){
    // Set Device Address
    MODIFY_REG(USBD->DCFG, USB_OTG_DCFG_DAD, (uint32_t)(addr << USB_OTG_DCFG_DAD_Pos));
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_setTXFIFO(uint8_t ep, uint16_t ep_size){
    uint32_t _fsa = USB->DIEPTXF0_HNPTXFSIZ;
    /* calculating initial TX FIFO address. next from EP0 TX fifo */
    _fsa = 0xFFFF & (_fsa + (_fsa >> 16));
    /* looking for next free TX fifo address */
    for (int i = 0; i < (MAX_EP - 1); i++) {
        uint32_t _t = USB->DIEPTXF[i];
        if ((_t & 0xFFFF) < 0x200) {
            _t = 0xFFFF & (_t + (_t >> 16));
            if (_t > _fsa) {
                _fsa = _t;
            }
        }
    }
    /* calculating requited TX fifo size */
    /* getting in 32 bit terms */
    ep_size = (ep_size + 0x03) >> 2;
    /* it must be 16 32-bit words minimum */
    if (ep_size < 0x10) ep_size = 0x10;
    /* checking for the available fifo */
    if ((_fsa + ep_size) > MAX_FIFO_SZ) return false;
    /* programming fifo register */
    _fsa |= (uint32_t)(ep_size << 16U);
    USB->DIEPTXF[ep - 1] = _fsa;
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_EP0_config(uint16_t ep_size){
    /* configureing control endpoint EP0 */
    uint32_t mp_size;
    if (ep_size <= 0x08) {
        ep_size = 0x08;
        mp_size = 0x03;
    } else if (ep_size <= 0x10) {
        ep_size = 0x10;
        mp_size = 0x02;
    } else if (ep_size <= 0x20) {
        ep_size = 0x20;
        mp_size = 0x01;
    } else {
        ep_size = 0x40;
        mp_size = 0x00;
    }
    /* EP0 TX FIFO size is setted on init level */
    /* enabling RX and TX interrupts from EP0 */
    USBD->DAINTMSK |= 0x00010001;
    /* setting up EP0 TX and RX registers */
    /*EPIN(ep)->DIEPTSIZ  = epsize;*/
    USB_EP_IN(0)->DIEPCTL = mp_size | USB_OTG_DIEPCTL_SNAK;
    /* 1 setup packet, 1 packets total */
    USB_EP_OUT(0)->DOEPTSIZ = ep_size | USB_OTG_DOEPTSIZ_STUPCNT_0 | (1 << USB_OTG_DOEPTSIZ_PKTCNT_Pos);
    USB_EP_OUT(0)->DOEPCTL = mp_size | USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK;
    return eHUSB_OK;

}

static inline enum hal_usb_sts hal_usb_EPIN_config(uint8_t epn, enum hal_usb_eptype ep_type, uint16_t ep_size){
    enum hal_usb_sts status = eHUSB_OK;
    epn &= (eHUSB_EP_IN-1);
    __IO USB_OTG_INEndpointTypeDef *ep = USB_EP_IN(epn);

    // Set up TX fifo and size register
    if(ep_type == eHUSB_EPTYPE_ISOCHRONUS || ep_type == eHUSB_EPTYPE_BULK)
        status = hal_usb_setTXFIFO(epn, (uint16_t)(ep_size << 1));
    else
        status = hal_usb_setTXFIFO(epn, (uint16_t)(ep_size));
    if(status != eHUSB_OK) return status;

    // Enable the EP TX Interrupt
    SET_BIT(USBD->DAINTMSK, (0x1UL << epn));

    // Clear any previous Configuration settings
    CLEAR_REG(ep->DIEPCTL);
    // Set up the endpoint types
    switch(ep_type){
        case eHUSB_EPTYPE_ISOCHRONUS:
            SET_BIT(ep->DIEPCTL,
                    // Endpoint Enable
                    USB_OTG_DIEPCTL_EPENA                         |
                    // Clear endoint NAK
                    USB_OTG_DIEPCTL_CNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
        case eHUSB_EPTYPE_BULK:
            SET_BIT(ep->DIEPCTL,
                    // Set endoint NAK (No data ready)
                    USB_OTG_DIEPCTL_SNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
        case eHUSB_EPTYPE_INTERRUPT:
        case eHUSB_EPTYPE_CONTROL:
        default:
            SET_BIT(ep->DIEPCTL,
                    // Set endoint NAK (No data ready)
                    USB_OTG_DIEPCTL_SNAK                          |
                    // Set endpoint transfer mode
                    (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
                    // Set the endpoint as active
                    USB_OTG_DIEPCTL_USBAEP                        |
                    // Set Even frame
                    USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
                    // Set TX fifo number
                    (uint32_t)(epn << USB_OTG_DIEPCTL_TXFNUM_Pos) |
                    (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
                );
            break;
    }
    return status;
}

static inline enum hal_usb_sts hal_usb_EPOUT_config(uint8_t epn, enum hal_usb_eptype ep_type, uint16_t ep_size){
    __IO USB_OTG_OUTEndpointTypeDef *ep = USB_EP_OUT(epn);
    // Clear any previous Configuration settings
    CLEAR_REG(ep->DOEPCTL);
    // Set up the endpoint type
    SET_BIT(ep->DOEPCTL,
            // Endpoint Enable
            USB_OTG_DIEPCTL_EPENA                         |
            // Clear endoint NAK
            USB_OTG_DIEPCTL_CNAK                          |
            // Set endpoint transfer mode
            (ep_type << USB_OTG_DIEPCTL_EPTYP_Pos)        |
            // Set the endpoint as active
            USB_OTG_DIEPCTL_USBAEP                        |
            // Set Even frame
            USB_OTG_DIEPCTL_SD0PID_SEVNFRM                |
            (uint32_t)(ep_size <<  USB_OTG_DIEPCTL_MPSIZ_Pos)
           );
    return eHUSB_OK;
}

static inline enum hal_usb_sts hal_usb_EP_config(uint8_t epn, enum hal_usb_eptype eptype, uint16_t epsize) {
    if (epn == 0) return hal_usb_EP0_config(epsize);
    if (epn & 0x80) return hal_usb_EPIN_config(epn, eptype, epsize);
    return hal_usb_EPOUT_config(epn, eptype, epsize);
}

static inline enum hal_usb_sts hal_usb_EP_deconfig(uint8_t epn){
    epn &= (eHUSB_EP_IN-1);
    __IO USB_OTG_INEndpointTypeDef *epi = USB_EP_IN(epn);
    __IO USB_OTG_OUTEndpointTypeDef *epo = USB_EP_OUT(epn);

    // Clear the EP IN interrupt
    CLEAR_BIT(USBD->DAINTMSK, 0x1UL << epn);
    // Deactivate the IN endpoint
    CLEAR_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_USBAEP);
    // Flush the FIFO
    hal_usb_flushTxFifo(epn);
    // Do not disable EP0 or previously disabled EPs
    if(READ_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA) && (epn!=0)){
        // Set the endpoint to disabled
        epi->DIEPCTL = USB_OTG_DIEPCTL_EPDIS;
    }
    // Clear any pending interrupts
    epi->DIEPINT = 0xFF;
    if(epn > 0){
        // Reset the FIFO size register
        USB->DIEPTXF[epn-1] = 0x02000200 + 0x200 * epn;
    }
    
    // Deactivate the OUT endpoint
    CLEAR_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_USBAEP);
    if(READ_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_EPENA) && (epn!=0)){
        // Set the endpoint to disabled
        epo->DOEPCTL = USB_OTG_DOEPCTL_EPDIS;
    }
    // Clear any pending interrupts
    epo->DOEPINT = 0xFF;
    return eHUSB_OK;
}

static inline int32_t hal_usb_EP_read(uint8_t epn, void *buf, uint16_t blen){
    if(!buf) return -1;
    epn &= (eHUSB_EP_IN-1);
    __IO uint32_t *FIFO = USB_EP_FIFO(0);
    __IO USB_OTG_OUTEndpointTypeDef *epo = USB_EP_OUT(epn);
    // Check that there is data waiting in one of the TX FIFOs
    if(!READ_BIT(USB->GINTSTS, USB_OTG_GINTSTS_RXFLVL)) return -1;
    // Check that the packet belongs to the current EP
    if(READ_BIT(USB->GRXSTSR, USB_OTG_GRXSTSP_EPNUM) != epn) return -1;
    uint16_t pktlen = (USB->GRXSTSP & USB_OTG_GRXSTSP_BCNT_Msk) >> USB_OTG_GRXSTSP_BCNT_Pos;
    uint32_t tmp = 0;
    // Read from FIFO in words, write to buffer in bytes
    for(uint16_t i = 0; i < pktlen; i++){
        if((i & 0x03U) == 0x00)
            tmp = *FIFO;
        if(i < blen){
            ((uint8_t*)buf)[i] = tmp &0xFF;
            tmp = tmp >> 8;
        }
    }
    // ACK the packet and reset the EP enable bit (core will clear it after RX)
    SET_BIT(epo->DOEPCTL, USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
    return (pktlen < blen) ? pktlen : blen;
}

static inline enum hal_usb_sts hal_usb_EP_write(uint8_t epn, const void *buf, uint16_t blen){
    if(!buf) return eHUSB_NULL;
    epn &= (eHUSB_EP_IN-1);
    __IO uint32_t *FIFO = USB_EP_FIFO(epn);
    __IO USB_OTG_INEndpointTypeDef *epi = USB_EP_IN(epn);
    // Transfer data in 32 bit words
    uint16_t len = (blen + 3) >> 2;
    // Check for space in the TX FIFO
    if(len > epi->DTXFSTS) return eHUSB_EP_FULL;
    if(epn != 0 && READ_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA)) return eHUSB_EP_NENA;
    // Clear the IN EP transfer register
    CLEAR_REG(epi->DIEPTSIZ);
    // Set the IN EP transfer reg to 1 packet | size
    epi->DIEPTSIZ = (1 << USB_OTG_DIEPTSIZ_PKTCNT_Pos) | blen;
    CLEAR_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_STALL);
    SET_BIT(epi->DIEPCTL, USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK);
    uint32_t tmp = 0;
    for(uint16_t i = 0; i < blen; i++){
        tmp |= (uint32_t)((const uint8_t*)buf)[i] << ((i & 0x03U) << 3);
        if((i & 0x03U) == 0x03U || (i+1)==blen){
            *FIFO = tmp;
            tmp = 0;
        }
    }
    return eHUSB_OK;
}

static inline uint16_t hal_usb_getFrame(void){
    return (USBD->DSTS & USB_OTG_DSTS_FNSOF) >> USB_OTG_DSTS_FNSOF_Pos;
}


static inline void hal_usb_evt_reset(enum hal_usb_event *evt, uint8_t *epn){
    (void)evt; (void)epn;
    USB->GINTSTS = USB_OTG_GINTSTS_USBRST;
    for(uint8_t i = 0; i < MAX_EP; i++)
        (void)hal_usb_EP_deconfig(i);
    (void)hal_usb_flushRxFifo();
}

static inline void hal_usb_evt_iepint(enum hal_usb_event *evt, uint8_t *epn){
    for(*epn = 0;; (*epn)++){
        __IO USB_OTG_INEndpointTypeDef *epi = USB_EP_IN((*epn));
        if(*epn > MAX_EP) return;
        if (epi->DIEPINT & USB_OTG_DIEPINT_XFRC) {
            epi->DIEPINT = USB_OTG_DIEPINT_XFRC;
            *evt = eHUSB_EVT_EPTX;
            *epn |= 0x80;
            break;
        }
    }
}

static inline void hal_usb_evt_rxflvl(enum hal_usb_event *evt, uint8_t *epn){
    uint32_t GRXSTSR = USB->GRXSTSR;
    *epn = GRXSTSR & USB_OTG_GRXSTSP_EPNUM;
    switch ((GRXSTSR & USB_OTG_GRXSTSP_PKTSTS) >> USB_OTG_GRXSTSP_PKTSTS_Pos) {
        case 0x02:
            *evt = eHUSB_EVT_EPRX;
            break;
        case 0x06:
            *evt = eHUSB_EVT_EPSETUP;
            break;
        default:
            USB->GRXSTSP;
    }
}


static void hal_usb_irq(void *pvdev, hal_usb_evt_callback callback){
    enum hal_usb_event evt;
    uint8_t epn = 0;
    for(;;){
        uint32_t GINTSTS = USB->GINTSTS;
        if(GINTSTS & USB_OTG_GINTSTS_USBRST)
            hal_usb_evt_reset(&evt, &epn);
        else if(GINTSTS & USB_OTG_GINTSTS_ENUMDNE){
            USB->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;
            evt = eHUSB_EVT_RESET;
        }
        else if(GINTSTS & USB_OTG_GINTSTS_IEPINT)
            hal_usb_evt_iepint(&evt, &epn);
        else if(GINTSTS & USB_OTG_GINTSTS_RXFLVL)
            hal_usb_evt_rxflvl(&evt, &epn);
        else if(GINTSTS & USB_OTG_GINTSTS_SOF){
            USB->GINTSTS = USB_OTG_GINTSTS_SOF;
            evt = eHUSB_EVT_SOF;
        }
        else if(GINTSTS & USB_OTG_GINTSTS_SOF){
            USB->GINTSTS = USB_OTG_GINTSTS_SOF;
            evt = eHUSB_EVT_SOF;
        }
        else if(GINTSTS & USB_OTG_GINTSTS_USBSUSP){
            USB->GINTSTS = USB_OTG_GINTSTS_USBSUSP;
            evt = eHUSB_EVT_SUSP;
        }
        else if(GINTSTS & USB_OTG_GINTSTS_WKUINT){
            USB->GINTSTS = USB_OTG_GINTSTS_WKUINT;
            evt = eHUSB_EVT_WKUP;
        }
        else return;
        callback(pvdev, evt, epn);
    }
}

#endif

