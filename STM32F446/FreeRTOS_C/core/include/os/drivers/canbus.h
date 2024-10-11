/**
 * @file canbus.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief UART Serial Driver
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "os/hal/hal_can.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "os/config/sys_cfg.h"

enum eCanError {
    eCanOK,
    // Hardware Error
    eCanHW,
    // Semaphore lock timeout
    eCanSemphr,
    // Initalization Failed
    eCanInitFail,
    // Interface is uninitialized
    eCanNoInit,
    // Null reference passed
    eCanNull,
};

typedef enum eCanError eCanError;


typedef struct CanMailbox {
    uint32_t id_msk;
    StreamBufferHandle_t buf_hndl;
    StaticStreamBuffer_t static_buf;
    TaskHandle_t tsk_hndl;
    struct CanMailbox *next;
} CanMailbox_t;

typedef struct CAN {
    CAN_TypeDef *CAN;
    IRQn_Type IRQn;
    SemaphoreHandle_t tx_semphr_hndl;
    SemaphoreHandle_t mailbox_semphr_hndl[3];
    StaticSemaphore_t static_tx_semphr;
    StaticSemaphore_t static_mailbox_semphr;
    CanMailbox_t *mailbox;
    eCanError state;
} CAN_t;


extern eCanError can_init(CAN_t *pHndl, CAN_BITRATE bitrate, pin_t pin_rx, pin_t pin_tx);

extern eCanError can_auto_retransmit(CAN_t *pHndl, bool enable);

extern eCanError can_write(CAN_t *pHndl, can_msg_t *pMsg, TickType_t timeout);

extern eCanError can_attach(CAN_t *pHndl, CanMailbox_t *pMailbox);

extern eCanError can_detach(CAN_t *pHndl, CanMailbox_t *pMailbox);

#if (configUSE_CAN1 == 1)
extern void CAN1_RX0_IRQHandler(void);
extern CAN_t CANBus1;
#endif
#if (configUSE_CAN2 == 1)
extern void CAN2_RX0_IRQHandler(void);
extern CAN_t CANBus2;
#endif
