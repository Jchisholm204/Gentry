/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */


#include "main.h"
#include <stdio.h>
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "os/config/FreeRTOSConfig.h"
#include "task.h"
#include "os/drivers/serial.h"
#include "string.h"
#include "os/hal/hal_clock.h"
#include "os/hal/hal_gpio.h"
#include "os/config/pin_cfg.h"
#include "os/drivers/canbus.h"

#define debug_led PIN('D', 12)


void vTestTask(void * pvParams){
    (void)pvParams;
    char* str = "Hello World from Serial 2\n";
    gpio_set_mode(PIN('B', 0), GPIO_MODE_OUTPUT);
    vTaskDelay(100);
    // hal_uart_init(USART2, 9600, PIN('A', 2), PIN('A', 3));
    for(;;){
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, str, strlen(str), 100);
        // printf("Hello from Serial 2\n");
        // hal_uart_write_buf(USART2, (char*)str, strlen(str));
        // Serial2.write((uint8_t*)str, strlen(str));
        // sleep for 1000 ms
        vTaskDelay(1000);
    }
}

void vUART_FeedBack(void * pvParams){
    (void)pvParams;
    // StreamBufferHandle_t hndl = xStreamBufferCreateStatic(100, 1, NULL, NULL);
    StreamBufferHandle_t rx_buf = xStreamBufferCreate(100, 5);
    // StreamBufferHandle_t xStreamBuffer = xStreamBufferCreate(100, 1);
    // Serial2.attach(xStreamBuffer);
    serial_attach(&Serial3, &rx_buf);
    // char * msg = "Serial 2 got: ";
    vTaskDelay(1000);
    printf("Uart Feedback online\n");
    for(;;){
        char buffer[100] = {0};
        xStreamBufferReceive(rx_buf, (void*)buffer, 100, portMAX_DELAY);
        gpio_write(PIN('B', 0), !gpio_read_odr(PIN('B', 0)));
        // serial_write(&Serial2, msg, strlen(msg), 100);
        // serial_write(&Serial2, buffer, bytes, 100);
        printf("Serial 2 RX: %s\n", buffer);
        // printf("Serial 2 got: %s", buffer);
        // vTaskDelay(10);
    }
}



// void vCanTask(void *pvParams){
//     CanMailbox_t mailbox, mailbox2;
//     // MUST be static
//     static can_msg_t buf[100], buf2[100];
//     // enter critical section for setup
//     printf("Starting CAN1 rx task\n");
//     can_mailbox_init(&mailbox, buf, 100);
//     can_mailbox_init(&mailbox2, buf2, 100);
//     // Allow all can messages
//     can_mailbox_addMask(&mailbox, UINT32_MAX);
//     // Only allow messages with the same mask as 200 and 203
//     // May result in messages such 201 or 202 to pass through
//     can_mailbox_addMask(&mailbox2, 200);
//     can_mailbox_addMask(&mailbox2, 203);
//     // Attach the mailbox to the can task
//     // vTaskSuspend(NULL);
//     can_attach(&CANBus1, &mailbox);
//     can_attach(&CANBus1, &mailbox2);
//     // exit setup
//     vTaskDelay(1000);
//     printf("Entering CAN1 rx task\n");
//     for(;;){
//         can_msg_t msg;
//         if(can_mailbox_read(&mailbox, &msg, 100) == eCanOK)
//             printf("CAN1 Mailbox 1 got msg with id: %d\n", msg.id);
//         if(can_mailbox_read(&mailbox2, &msg, 100) == eCanOK)
//             printf("CAN1 Mailbox 2 got msg with id: %d\n", msg.id);
//         vTaskDelay(100);
//     }
//     can_detach(&CANBus1, &mailbox);
//     printf("Mailbox detatched\n");
//     // EXIT Gracefully (Scheduler will halt if this is not present)
//    vTaskSuspend(NULL);
// // vTaskDelay(portMAX_DELAY);
// }

void USB_Init_Core(void){
    // Enable USB FS interrupts
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN-2);
    NVIC_EnableIRQ(OTG_FS_IRQn);

    // Enable USB clock
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    
    // Setup USB GPIO
    GPIOA->MODER &= ~(0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DM)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DM)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->MODER &= (uint32_t)~(uint32_t)((uint32_t)0x3UL << (uint32_t)(PINNO(PIN_USB_DP)*2UL));
    GPIOA->MODER |= (0x2U << (PINNO(PIN_USB_DP)*2));
    GPIOA->OSPEEDR |= (0x3U << (PINNO(PIN_USB_DP)*2));
    GPIOA->AFR[1] &= ~(0x15U << ((PINNO(PIN_USB_DP) & 7)*4));
    GPIOA->AFR[1] |= (0xAU << ((PINNO(PIN_USB_DP) & 7)*4));

    // Init the USB AHB Interface
    // USB_OTG_FS->GAHBCFG = 0x0UL;
    // Global PHY Interrupt enable
    USB_OTG_FS->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
    // Interrupt on Fully empty TX FIFO
    // USB_OTG_FS->GAHBCFG &= ~USB_OTG_GAHBCFG_TXFELVL;
    // USB_OTG_FS->GINTSTS &= ~USB_OTG_GINTSTS_RXFLVL;
    USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBDEN;  // Enable VBUS sensing
    USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_PWRDWN;  // Power up the USB core

    // Init the USB Core
    // USB_OTG_FS->GUSBCFG = 0x00001440; 
    USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_HNPCAP | USB_OTG_GUSBCFG_SRPCAP;
    USB_OTG_FS->GUSBCFG &= ~(USB_OTG_GUSBCFG_FHMOD);
    USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_TRDT_3;
    USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_TOCAL_2;

    // Unmask Required Interrupt Bits
    // USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_OTGINT | USB_OTG_GINTMSK_MMISM;
    // USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_OTGINT; //| USB_OTG_GINTMSK_MMISM;
    
    bool devmod = !((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD) > 0);
    printf("Device Mode Enabled = %d\n", devmod);

    
}

void USB_Init_Device(void){
    USB_OTG_DeviceTypeDef *usbdev = (USB_OTG_DeviceTypeDef*)USB_OTG_DEVICE_BASE;
    // Set Periodic Frame interval to 90% | Device Speed to Full Speed using FS PHY
    usbdev->DCFG |= USB_OTG_DCFG_PFIVL_1 | (0x3 << USB_OTG_DCFG_DSPD_Pos);
    // Send out packet to the application and send NACK packet to host
    usbdev->DCFG &= ~(USB_OTG_DCFG_NZLSOHSK);
    // Clear Soft Disconnect Bit
    usbdev->DCTL &= ~(USB_OTG_DCTL_SDIS);

    // Enable Interrupt Bits
    // USB_OTG_HS->GINTMSK |= USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_ESUSPM | USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_SOFM;
    USB_OTG_HS->GINTMSK |= USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_SOFM;
}

static inline void USB_RemoteWake(void){
    // printf("Attempting to wake Host\n");
    USB_OTG_DeviceTypeDef *usbdev = (USB_OTG_DeviceTypeDef*)USB_OTG_DEVICE_BASE;
    usbdev->DCTL |= USB_OTG_DCTL_RWUSIG;
    // Wait 2ms and clear bit
    vTaskDelay(2);
    usbdev->DCTL &= ~(USB_OTG_DCTL_RWUSIG);
}

volatile bool reset_detected = false, reset_ended = false;

void vUSB_tsk(void * pvParams){
    (void)pvParams;

    printf("Initializing USB Core\n");
    USB_Init_Core();
    printf("USB Core Initialized\n");
    printf("Initializing USB Device\n");
    USB_Init_Device();
    printf("USB Device Initialized\n");
    printf("Attempting to wake Host\n");
    USB_RemoteWake();

    for(;;){
        if(reset_detected)
            printf("USB reset detected\n");
        else
            USB_RemoteWake();
        if(reset_ended)
            printf("USB reset finished\n");
        vTaskDelay(10);
    }
    // Main USB task delay to allow for USB processing
    vTaskDelay(100);
    printf("Deleting USB task\n");
    vTaskDelete(NULL);
}


// Interrupt handler for USB OTG FS
void OTG_FS_IRQHandler(void) {
    uint32_t gintsts = USB_OTG_FS->GINTSTS;

    // Check for USB reset interrupt (used when the USB host resets the device)
    if (gintsts & USB_OTG_GINTSTS_USBRST) {
        // printf("USB Reset Interrupt\n");
        reset_detected = !reset_detected;
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_USBRST;  // Clear interrupt flag
    }

    // Handle enumeration done (when device enumeration is complete)
    if (gintsts & USB_OTG_GINTSTS_ENUMDNE) {
        // printf("USB Enumeration Done\n");
        reset_ended = !reset_ended;
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;  // Clear interrupt flag
    }

    // Handle RX FIFO level (data received on control endpoint)
    if (gintsts & USB_OTG_GINTSTS_RXFLVL) {
        // printf("RX FIFO Level Interrupt (data received)\n");
        // Add RX FIFO processing code here...
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_RXFLVL;   // Clear interrupt flag
    }

    // Start of Frame interrupt
    if (gintsts & USB_OTG_GINTSTS_SOF) {
        // printf("Start of Frame\n");
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_SOF;      // Clear interrupt flag
    }

    if(gintsts & USB_OTG_GINTSTS_GINAKEFF){
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_GINAKEFF;      // Clear interrupt flag
    }

    if(gintsts & 0x10){
        USB_OTG_FS->GINTSTS = 0x10;      // Clear interrupt flag
    }

    if(gintsts & USB_OTG_GINTSTS_OTGINT){
        uint32_t goint = USB_OTG_HS->GOTGINT;
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_OTGINT;      // Clear interrupt flag
    }
    // Handle any other interrupts you need to process
    // Clear Everthing
    // USB_OTG_FS->GINTSTS = gintsts;
}

// Initialize all system Interfaces
void Init(void){
    // Initialize UART
    serial_init(&Serial3, 9600, PIN_USART3_RX, PIN_USART3_TX);
    // Required for MockECU Board
    // gpio_set_mode(PIN('A', 10), GPIO_MODE_OUTPUT);
    // gpio_write(PIN('A', 10), false);
    // gpio_set_mode(PIN_LED2, GPIO_MODE_OUTPUT);
    // Initialize CAN
    // can_init(&CANBus1, CAN_1000KBPS, PIN_CAN1_RX, PIN_CAN1_TX);

}


int main(void){

    Init();
    
    // THESE SHOULD BE STATIC
    xTaskCreate(vTestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUART_FeedBack, "S2 Echo", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vUSB_tsk, "USB tst", configMINIMAL_STACK_SIZE<<1, NULL, 1, NULL);
    // xTaskCreate(vCanTask, "Can1RX", configMINIMAL_STACK_SIZE, NULL, 0, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        asm("nop");
        gpio_write(PIN_LED2, true);
    }
    return 0;
}

