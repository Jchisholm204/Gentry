/**
 * @file canbus.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief CAN Bus FreeRTOS Driver
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _CANBUS_H_
#define _CANBUS_H_

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
    // Mailbox is empty
    eCanEmpty,
    eCanFatal,
    eCanMailboxNotFound,
    eCanReadFail,
    eCanMailboxWriteError
};

typedef enum eCanError eCanError;


typedef struct CanMailbox {
    uint32_t id_msk;
    StreamBufferHandle_t buf_hndl;
    StaticStreamBuffer_t static_buf;
    size_t id;
    struct CanMailbox *next;
} CanMailbox_t;

typedef struct CAN {
    CAN_TypeDef *CAN;
    IRQn_Type IRQn;
    const char *pcName;
    SemaphoreHandle_t tx_semphr_hndl;
    SemaphoreHandle_t mailbox_semphr_hndl[3];
    StaticSemaphore_t static_tx_semphr;
    StaticSemaphore_t static_mailbox_semphr[3];
    CanMailbox_t *mailbox;
    size_t n_mailboxes;
    TaskHandle_t tsk_hndl;
    StaticTask_t tsk_buf;
    StackType_t  tsk_stack[configMINIMAL_STACK_SIZE];
    StreamBufferHandle_t rx_hndl;
    StaticStreamBuffer_t rx_streamBuf;
    can_msg_t rx_buf[configMINIMAL_STACK_SIZE];
    eCanError state;
} CAN_t;


extern eCanError can_init(CAN_t *pHndl, CAN_BITRATE bitrate, pin_t pin_rx, pin_t pin_tx);

extern eCanError can_write(CAN_t *pHndl, can_msg_t *pMsg, TickType_t timeout);

extern eCanError can_attach(CAN_t *pHndl, CanMailbox_t *pMailbox);

extern eCanError can_detach(CAN_t *pHndl, CanMailbox_t *pMailbox);

extern eCanError can_mailbox_init(CanMailbox_t *pMailbox, can_msg_t *const buffer, size_t len);

extern eCanError can_mailbox_addMask(CanMailbox_t *pMailbox, uint32_t id);

extern eCanError can_mailbox_read(CanMailbox_t *pMailbox, can_msg_t *msg, TickType_t timeout);

#if (configUSE_CAN1 == 1)
extern void CAN1_RX0_IRQHandler(void);
extern CAN_t CANBus1;
#endif
#if (configUSE_CAN2 == 1)
extern void CAN2_RX0_IRQHandler(void);
extern CAN_t CANBus2;
#endif

#endif
