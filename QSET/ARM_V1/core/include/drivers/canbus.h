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

#include "hal/hal_can.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "config/sys_cfg.h"

#define MAX_MAILBOXES 20

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
    // The number of mailbox allocation slots has been exceeded
    eCanMailboxFull,
    eCanFatal,
    eCanReadFail,
};

typedef enum eCanError eCanError;


typedef struct CanMailbox {
    uint32_t can_id;
    can_msg_t msg;
    TaskHandle_t pRxHndl;
} CanMailbox_t;

typedef struct CAN {
    CAN_TypeDef *CAN;
    IRQn_Type IRQn;
    const char *pcName;
    SemaphoreHandle_t tx_semphr_hndl;
    SemaphoreHandle_t mailbox_semphr_hndl[3];
    StaticSemaphore_t static_tx_semphr;
    StaticSemaphore_t static_mailbox_semphr[3];
    CanMailbox_t *mailbox[MAX_MAILBOXES];
    size_t n_mailboxes;
    TaskHandle_t tsk_hndl;
    StaticTask_t tsk_buf;
    StackType_t  tsk_stack[configMINIMAL_STACK_SIZE];
    StreamBufferHandle_t rx_hndl;
    StaticStreamBuffer_t rx_streamBuf;
    can_msg_t rx_buf[configMINIMAL_STACK_SIZE];
    eCanError state;
} CAN_t;


/**
 * @brief Initialize a CAN Interface
 *
 * @param pHndl Handle of the CAN interface
 * @param bitrate CAN bitrate to use
 * @param pin_rx RX Pin
 * @param pin_tx TX Pin
 * @returns Ok or eCanError
 */
extern eCanError can_init(CAN_t *pHndl, CAN_BITRATE bitrate, pin_t pin_rx, pin_t pin_tx);

/**
 * @brief Send a CAN message
 *
 * @param pHndl Handle of the CAN interface to send to
 * @param pMsg Pointer to the message to send
 * @param timeout max time to wait for an avaliable mailbox
 * @returns Ok or eCanError
 */
extern eCanError can_write(CAN_t *pHndl, can_msg_t *pMsg, TickType_t timeout);

/**
 * @brief Initialize a CAN Mailbox
 *
 * @param pHndl Handle of the CAN interface to recieve from
 * @param pMailbox Pointer to locally initialized mailbox
 * @param can_id CAN ID to recieve messages from
 * @returns Ok or eCanError
 */
extern eCanError can_mailbox_init(CAN_t *pHndl, CanMailbox_t *pMailbox, uint32_t can_id);

/**
 * @brief Wait for a message recieve event
 *
 * @param pMailbox Mailbox to wait on
 * @param pMsg Memory to recieve the message into
 * @param wait_time
 * @returns Ok or eCanError
 */
extern eCanError can_mailbox_wait(CanMailbox_t *pMailbox, can_msg_t *pMsg, TickType_t wait_time);

/**
 * @brief Read from a CAN Mailbox (does not wait for new message)
 * Gets the last message sent
 * @param pMailbox 
 * @param pMsg Memory to recieve the message into
 * @returns Ok or eCanError
 */
extern eCanError can_mailbox_read(CanMailbox_t *pMailbox, can_msg_t *pMsg);

#if (configUSE_CAN1 == 1)
extern void CAN1_RX0_IRQHandler(void);
extern CAN_t CANBus1;
#endif
#if (configUSE_CAN2 == 1)
extern void CAN2_RX0_IRQHandler(void);
extern CAN_t CANBus2;
#endif

#endif
