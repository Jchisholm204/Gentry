/**
 * @file usb_std.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief USB Standard Definitions
 * @date 2024-10-18
 * @version 0.1
 * 
 */

#ifndef _USB_STD_H_
#define _USB_STD_H_
#include <stm32f4xx.h>
#include <sys/stat.h>

#define USBD_MAX_SUPPORTED_CLASS 4U
#define USBD_MAX_CLASS_ENDPOINTS 5U

enum USBD_CompositeClass
{
  CLASS_TYPE_NONE    = 0,
  CLASS_TYPE_HID     = 1,
  CLASS_TYPE_CDC     = 2,
  CLASS_TYPE_MSC     = 3,
  CLASS_TYPE_DFU     = 4,
  CLASS_TYPE_CHID    = 5,
  CLASS_TYPE_AUDIO   = 6,
  CLASS_TYPE_ECM     = 7,
  CLASS_TYPE_RNDIS   = 8,
  CLASS_TYPE_MTP     = 9,
  CLASS_TYPE_VIDEO   = 10,
  CLASS_TYPE_PRINTER = 11,
  CLASS_TYPE_CCID    = 12,
};

enum USBD_Speed {
    USBD_SPEED_HIGH = 0U,
    USBD_SPEED_FULL = 1U, 
    USBD_SPEED_LOW = 2U
};

typedef struct usbd_ep {
    uint32_t status;
    uint32_t total_len;
    uint32_t rem_len;
    uint32_t maxpacket;
    uint16_t is_used;
    uint16_t bInterval;
} USBD_EP_t;

typedef struct usbd_composite_element {
    enum USBD_CompositeClass classType;
    uint32_t classID;
    uint32_t active;
    uint32_t n_EP;
    USBD_EP_t EP[USBD_MAX_CLASS_ENDPOINTS];
    uint8_t *EpAdd;
    uint32_t numIf;
    uint8_t Ifs[USBD_MAX_CLASS_ENDPOINTS];
    uint32_t CurrPcktSize;
} USBDCompositeElement_t;

typedef struct usbd_setup_req {
    uint8_t bmRequest;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} USBD_SetupReq_t;

typedef struct usbd_decriptor {
    uint8_t *(*GetDeviceDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetLangIDStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetManufacturerStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetProductStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetSerialStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetConfigurationStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetInterfaceStrDescriptor)(enum USBD_Speed speed, uint16_t *length);
    uint8_t *(*GetUserStrDescriptor)(enum USBD_Speed speed, uint8_t idx, uint16_t *length);
    uint8_t *(*GetBOSDescriptor)(enum USBD_Speed speed, uint16_t *length);
} USBD_Descriptor_t;

struct _USBDevice;

typedef struct usbd_class {
    uint8_t (*Init)(struct _USBDevice *pDev, uint8_t cfgidx);
    uint8_t (*DeInit)(struct _USBDevice *pDev, uint8_t cfgidx);
    uint8_t (*Setup)(struct _USBDevice *pDev, USBD_SetupReq_t *pReq);
    uint8_t (*EP0_TxSent)(struct _USBDevice *pDev);
    uint8_t (*EP0_RxRdy)(struct _USBDevice *pDev);
    uint8_t (*DataIn)(struct _USBDevice *pDev, uint8_t epnum);
    uint8_t (*DataOut)(struct _USBDevice *pDev, uint8_t epnum);
    uint8_t (*SOF)(struct _USBDevice *pDev);
    uint8_t (*IsoInIncomplete)(struct _USBDevice *pDev, uint8_t epnum);
    uint8_t (*IsoOutIncomplete)(struct _USBDevice *pDev, uint8_t epnum);

    uint8_t *(*GetConfigurationDescriptor)(uint8_t *length);
    uint8_t *(*GetQualifierDescriptor)(uint8_t *length);
    uint8_t *(*GetUserStrDescriptor)(uint8_t *length);

} USBD_class_t;

typedef struct usbdevice {
    uint8_t id;
    uint32_t dev_config;
    uint32_t dev_default_config;
    uint32_t dev_config_status;
    enum USBD_Speed dev_speed;
    USBD_EP_t ep_in[16];
    USBD_EP_t ep_out[16];
    __IO uint32_t ep0_state;
    uint32_t ep0_data_len;
    __IO uint8_t dev_state;
    __IO uint8_t dev_old_state;
    uint8_t dev_addr;
    uint8_t dev_connection_status;
    uint8_t dev_test_mode;
    uint32_t dev_remote_wake;
    uint8_t ConfIdx;

    USBD_SetupReq_t request;
    USBD_Descriptor_t *pDescriptors;
    USBD_class_t *pClass[USBD_MAX_SUPPORTED_CLASS];
    void *pClassData;
    void *pClassDataCmsit[USBD_MAX_SUPPORTED_CLASS];
    void *pUserData[USBD_MAX_SUPPORTED_CLASS];
    void *pData;
    void *pBosDesc;
    void *pConfDesc;
    uint32_t classID;
    uint32_t n_classes;
    USBDCompositeElement_t tclasslist[USBD_MAX_SUPPORTED_CLASS];

} USBDevice_t;

#endif

