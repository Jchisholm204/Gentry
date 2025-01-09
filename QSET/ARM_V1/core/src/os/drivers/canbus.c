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

#include "os/drivers/canbus.h"
#include "os/config/nvic.h"
#include <stdio.h>
#include "semphr.h"
#include "queue.h"
#include "stdio.h"

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
    pHndl->mailbox = NULL;
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

eCanError can_attach(CAN_t *pHndl, CanMailbox_t *pMailbox){
    if(pMailbox == NULL || pHndl == NULL)
        return eCanNull;
    if(pHndl->state != eCanOK)
        return pHndl->state;
    CanMailbox_t *mailbox = pHndl->mailbox;
    if(mailbox == NULL){
        pHndl->mailbox = pMailbox;
    }
    else{
        while (mailbox->next != NULL)
            mailbox = mailbox->next;
        mailbox->next = pMailbox;
    }
    pHndl->n_mailboxes++;
    pMailbox->id = pHndl->n_mailboxes;
    return eCanOK;
}

eCanError can_detach(CAN_t *pHndl, CanMailbox_t *pMailbox){
    if(pMailbox == NULL || pHndl == NULL)
        return eCanNull;
    if(pHndl->state != eCanOK)
        return pHndl->state;
    CanMailbox_t *mailbox = pHndl->mailbox;
    // Handle the case of only one mailbox
    if(mailbox->next == NULL){
        if(mailbox->id != pMailbox->id)
            return eCanMailboxNotFound;
        pHndl->mailbox = NULL;
        pHndl->n_mailboxes--;
        return eCanOK;
    }
    for(size_t i = 0; i < pHndl->n_mailboxes; i++){
        if(mailbox->next == NULL)
            return eCanMailboxNotFound;
        // Check if the next mailbox is the one to remove
        if(mailbox->next->id == pMailbox->id){
            mailbox->next = mailbox->next->next;
            pHndl->n_mailboxes--;
            return eCanOK;
        }
        // Advance to next mailbox
        mailbox = mailbox->next;
    }
}

eCanError can_mailbox_init(CanMailbox_t *pMailbox, can_msg_t *const buffer, size_t len){
    if(pMailbox == NULL || buffer == NULL)
        return eCanNull;
    pMailbox->buf_hndl = xStreamBufferCreateStatic(len*sizeof(can_msg_t), sizeof(can_msg_t), (uint8_t*)buffer, &pMailbox->static_buf);
    pMailbox->id = 0;
    pMailbox->next = NULL;
    return eCanOK;
}

eCanError can_mailbox_addMask(CanMailbox_t *pMailbox, uint32_t id){
    if(pMailbox == NULL)
        return eCanNull;
    pMailbox->id_msk |= id;
    return eCanOK;
}

eCanError can_mailbox_read(CanMailbox_t *pMailbox, can_msg_t *msg, TickType_t timeout){
    if(pMailbox == NULL || msg == NULL)
        return eCanNull;
    if(pMailbox->buf_hndl == NULL)
        return eCanNull;
    size_t rx_bytes = xStreamBufferReceive(pMailbox->buf_hndl, msg, sizeof(can_msg_t), timeout);
    if(rx_bytes < sizeof(can_msg_t))
        return eCanReadFail;
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
        CanMailbox_t *pMailbox = pHndl->mailbox;
        // if there are no mailboxes, discard the message and move on
        // vPortEnterCritical();
        if(pMailbox == NULL){
            // vPortExitCritical();
            continue;
        }
        // Dump the message in the matching mailboxes
        for(size_t i = 0; i < pHndl->n_mailboxes; i++){
            // Check that this mailbox is compatible with this message
            if((msg.id & pMailbox->id_msk) != 0){
                // printf("Attempting to send to mailbox %d\n", i);
                size_t aval_bytes =  xStreamBufferSpacesAvailable(pMailbox->buf_hndl);
                if(aval_bytes > sizeof(can_msg_t)){
                    size_t tx_bytes = xStreamBufferSend(pMailbox->buf_hndl, &msg, sizeof(can_msg_t), 10);
                    if (tx_bytes != sizeof(can_msg_t))
                    {
                        pHndl->state = eCanMailboxWriteError;
                    }
                }
            }
            // Advance to the next mailbox
            pMailbox = pMailbox->next;
            // Check that the next mailbox is not NULL
            if(pMailbox == NULL)
                break;
        }
        // vPortExitCritical();
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

