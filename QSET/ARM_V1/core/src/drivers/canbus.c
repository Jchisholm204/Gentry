/**
 * @file canbus.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief CAN Bus FreeRTOS Driver
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/canbus.h"
#include "config/nvic.h"
#include <stdio.h>
#include "semphr.h"
#include "queue.h"
#include "stdio.h"
#include <memory.h>
#include <limits.h>

void vCAN_Hndl_tsk(void *pvParams);

eCanError can_init(CAN_t *pHndl, CAN_BITRATE bitrate, pin_t pin_rx, pin_t pin_tx){
    if(pHndl == NULL)
        return eCanNull;
    // Setup CAN hardware
    uint8_t can_status = hal_can_init(pHndl->CAN, bitrate, true, pin_tx, pin_rx);
    if(can_status != HAL_CAN_OK){
        pHndl->state = eCanHW;
        return eCanHW;
    }
    
    // Setup Semaphores
    pHndl->tx_semphr_hndl = xSemaphoreCreateCountingStatic(2, 0, &pHndl->static_tx_semphr);
    pHndl->mailbox_semphr_hndl[0] = xSemaphoreCreateMutexStatic(&pHndl->static_mailbox_semphr[0]);
    pHndl->mailbox_semphr_hndl[1] = xSemaphoreCreateMutexStatic(&pHndl->static_mailbox_semphr[1]);
    pHndl->mailbox_semphr_hndl[2] = xSemaphoreCreateMutexStatic(&pHndl->static_mailbox_semphr[2]);
    xSemaphoreGive(pHndl->tx_semphr_hndl);
    xSemaphoreGive(pHndl->mailbox_semphr_hndl[0]);
    xSemaphoreGive(pHndl->mailbox_semphr_hndl[1]);
    xSemaphoreGive(pHndl->mailbox_semphr_hndl[2]);

    // Ensure that the mailboxes slot is empty
    pHndl->n_mailboxes = 0;


    // Setup the stream buffer for the rx interrupt
    pHndl->rx_hndl = xStreamBufferCreateStatic(configMINIMAL_STACK_SIZE, sizeof(can_msg_t), (uint8_t*)pHndl->rx_buf, &pHndl->rx_streamBuf);
    
    // Setup this CAN task
    pHndl->tsk_hndl = xTaskCreateStatic(
            vCAN_Hndl_tsk, 
            pHndl->pcName, 
            configMINIMAL_STACK_SIZE, 
            (void *)pHndl, 
            // Allow higher priority tasks, but make the CAN task higher than most
            configMAX_PRIORITIES-2, 
            pHndl->tsk_stack,
            &pHndl->tsk_buf
    );

    // Enable CAN rx interrupts
    hal_can_enable_rxne(pHndl->CAN, true);
    NVIC_EnableIRQ(pHndl->IRQn);
    NVIC_SetPriority(pHndl->IRQn, NVIC_Priority_MIN);

    pHndl->state = eCanOK;
    return eCanOK;

}


eCanError can_write(CAN_t *pHndl, can_msg_t *pMsg, TickType_t timeout){
    if(pHndl->state != eCanOK)
        return pHndl->state;
    if(xSemaphoreTake(pHndl->tx_semphr_hndl, timeout) != pdTRUE)
        return eCanSemphr;
    for(uint8_t i = 0; i < 3; i++){
        if(xSemaphoreTake(pHndl->mailbox_semphr_hndl[i], 10) == pdTRUE){
            hal_can_send(pHndl->CAN, pMsg, i);
            while(!hal_can_send_ready(pHndl->CAN, i)) __ASM("nop");
            xSemaphoreGive(pHndl->mailbox_semphr_hndl[i]);
            xSemaphoreGive(pHndl->tx_semphr_hndl);
            return eCanOK;
        }
    }
    pHndl->state = eCanSemphr;
    return eCanSemphr;
}


eCanError can_mailbox_init(CAN_t *pHndl, CanMailbox_t *pMailbox, uint32_t can_id){
    if(pMailbox == NULL || pHndl == NULL)
        return eCanNull;
    pMailbox->pRxHndl = xTaskGetCurrentTaskHandle();
    pMailbox->can_id = can_id;
    pMailbox->msg = (can_msg_t){0, {0}, 0, 0, 0, 0};
    if(pHndl->n_mailboxes >= (MAX_MAILBOXES-1))
        return eCanMailboxFull;
    pHndl->mailbox[pHndl->n_mailboxes++] = pMailbox;
    return eCanOK;
}

eCanError can_mailbox_wait(CanMailbox_t *pMailbox, can_msg_t *pMsg, TickType_t wait_time){
    if(pMailbox == NULL || pMsg == NULL)
        return eCanNull;
    uint32_t ulNotify;
    if(xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotify, wait_time) == pdPASS){
        (void)ulNotify;
        memcpy(pMsg, &pMailbox->msg, sizeof(can_msg_t));
        return eCanOK;
    }
    return eCanEmpty;
}

eCanError can_mailbox_read(CanMailbox_t *pMailbox, can_msg_t *pMsg){
    if(pMailbox == NULL || pMsg == NULL)
        return eCanNull;
    memcpy(pMsg, &pMailbox->msg, sizeof(can_msg_t));
    return eCanOK;
}

void vCAN_Hndl_tsk(void *pvParams){
    // THIS TASK SHOULD NOT RECIEVE NULL PARAMETERS
    if(pvParams == NULL)
        return;
    CAN_t * pHndl = (CAN_t*)pvParams;
    printf("CANRX task live for %s\n", pHndl->pcName);
    for(;;){
        (void)pHndl;
        can_msg_t msg;
        size_t rx_bytes = xStreamBufferReceive(pHndl->rx_hndl, &msg, sizeof(can_msg_t), portMAX_DELAY);
        // This should not happen
        if(rx_bytes < sizeof(can_msg_t))
            continue;
        
        for(size_t i = 0; i < pHndl->n_mailboxes && i < MAX_MAILBOXES; i++){
            CanMailbox_t *pMailbox = pHndl->mailbox[i];
            if(msg.id == pMailbox->can_id){
                memcpy(&pMailbox->msg, &msg, sizeof(can_msg_t));
                xTaskNotify(pMailbox->pRxHndl, 0, eSetBits);
            }
        }

    }
}

// Default Handler for CAN interrupts
void default_handler(CAN_t *pHndl){
    BaseType_t higher_woken = pdFALSE;
    can_msg_t msg;
    // Read from the CAN's HW Mailbox
    hal_can_read(pHndl->CAN, &msg);
    // forward the message to the handler task
    xStreamBufferSendFromISR(pHndl->rx_hndl, &msg, sizeof(can_msg_t), &higher_woken);
    // Yeild to higher prioity tasks (likely CAN handler task)
    portYIELD_FROM_ISR(higher_woken);
}

#if (configUSE_CAN1 == 1)
CAN_t CANBus1 = {
    CAN1,
    CAN1_RX0_IRQn,
    "CAN1",
    NULL,
    {NULL},
    {0},
    {0},
    NULL,
    0,
    NULL,
    {0},
    {0},
    NULL,
    {0},
    {0},
    eCanNoInit
};
void CAN1_RX0_IRQHandler(void){
    default_handler(&CANBus1);
}
#endif
#if (configUSE_CAN2 == 1)
CAN_t CANBus2 = {
    CAN2,
    CAN2_RX0_IRQn,
    "CAN2",
    NULL,
    {NULL},
    {0},
    {0},
    NULL,
    0,
    NULL,
    {0},
    {0},
    NULL,
    {0},
    {0},
    eCanNoInit
};
void CAN2_RX0_IRQHandler(void){
    default_handler(&CANBus2);
}
#endif

