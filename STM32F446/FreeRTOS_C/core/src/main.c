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
        printf("Hello from Serial 2\n");
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

volatile bool session_active = false;
void vUSB_tsk(void * pvParams){
    (void)pvParams;
    printf("Initializing USB\n");

    // Enable USB in the RCC (USB OTG FS clock enable)
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);

    // Setup the GPIO pins for USB DM and DP with alternate function and proper speed
    // gpio_set_mode(PIN_USB_DM, GPIO_MODE_AF);
    // gpio_set_af(PIN_USB_DM, GPIO_AF_USB);
    // gpio_set_speed(PIN_USB_DM, GPIO_SPEED_HS);

    // gpio_set_mode(PIN_USB_DP, GPIO_MODE_AF);
    // gpio_pull(PIN_USB_DP, GPIO_PULLUP); // Enable the pull-up for USB DP
    // gpio_set_af(PIN_USB_DP, GPIO_AF_USB);
    // gpio_set_speed(PIN_USB_DP, GPIO_SPEED_HS);

    GPIOA->MODER &= ~(0x3 << (PINNO(PIN_USB_DM)*2));
    GPIOA->MODER |= (0b10 << (PINNO(PIN_USB_DM)*2));

    // GPIOA->OSPEEDR &= ~(0x3 << (PINNO(PIN_USB_DM)*2));
    GPIOA->OSPEEDR |= (0x3 << (PINNO(PIN_USB_DM)*2));

    GPIOA->AFR[1] &= ~(0b1111 << ((PINNO(PIN_USB_DM) & 7)*4));
    GPIOA->AFR[1] |= (0b1010 << ((PINNO(PIN_USB_DM) & 7)*4));

    GPIOA->MODER &= (uint32_t)~(uint32_t)((uint32_t)0x3UL << (uint32_t)(PINNO(PIN_USB_DP)*2UL));
    GPIOA->MODER |= (0b10 << (PINNO(PIN_USB_DP)*2));

    // GPIOA->OSPEEDR &= ~(0x3 << (PINNO(PIN_USB_DM)*2));
    GPIOA->OSPEEDR |= (0x3 << (PINNO(PIN_USB_DP)*2));

    GPIOA->AFR[1] &= ~(0b1111 << ((PINNO(PIN_USB_DP) & 7)*4));
    GPIOA->AFR[1] |= (0b1010 << ((PINNO(PIN_USB_DP) & 7)*4));

    // Core Initialization
    // Reset GAHBCFG to its default value
    USB_OTG_FS->GAHBCFG = 0x00000000;

    // Enable global interrupts in the AHB configuration register
    SET_BIT(USB_OTG_FS->GAHBCFG, USB_OTG_GAHBCFG_GINT);

    // OTG USB Configuration Register - USB turnaround time
    USB_OTG_FS->GUSBCFG = 0x00001440;
    USB_OTG_FS->GUSBCFG |= 0x6 << USB_OTG_GUSBCFG_TRDT_Pos;  // Set TRDT to the recommended value for FS

    // Finalize Interrupts in Interrupt Mask Reg
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_OTGINT);    // OTG interrupts
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_MMISM);     // Mode mismatch interrupt

    // Device Mode Check (CMOD = 0 for device mode)
    bool devon = USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD;
    printf("USB Device Mode? %d\n", !devon);

    // USB Device Initialization
    USB_OTG_DeviceTypeDef *usbdev = ((USB_OTG_DeviceTypeDef*)USB_OTG_DEVICE_BASE);

    // DCFG: Device Configuration Register
    usbdev->DCFG = 0x02200000;  // Full-speed device (FS)
    usbdev->DCFG |= 0x3 << USB_OTG_DCFG_PFIVL_Pos;  // Set periodic frame interval to 95%
    usbdev->DCFG &= ~(USB_OTG_DCFG_DSPD);  // Clear previous speed config
    usbdev->DCFG |= 0x3 << USB_OTG_DCFG_DSPD_Pos;   // Set device speed to Full-Speed

    // Clear the SDIS bit (Soft Disconnect), enabling connection to the host
    usbdev->DCTL &= ~(USB_OTG_DCTL_SDIS);

    // Enable the USB pull-up on D+ (to signal the host)
    usbdev->DCTL |= USB_OTG_DCTL_RWUSIG;  // Remote wakeup signaling, forcing D+ pull-up

    // Enable necessary USB interrupts in GINTMSK
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_USBSUSPM);  // USB Suspend
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_RXFLVLM);   // RX FIFO non-empty interrupt
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_IEPINT);   // IN endpoint interrupt
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_OEPINT);   // OUT endpoint interrupt
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_SOFM);      // Start of Frame interrupt
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_ENUMDNEM);  // Enumeration done interrupt
    // Enable Session Request interrupt
    SET_BIT(USB_OTG_FS->GINTMSK, USB_OTG_GINTMSK_SRQIM);

    // Enable the USB OTG FS interrupt in NVIC
    NVIC_SetPriority(OTG_FS_IRQn, NVIC_Priority_MIN-2);
    NVIC_EnableIRQ(OTG_FS_IRQn);

    gpio_set_mode(PIN_USB_VBUS, GPIO_MODE_INPUT);

    // Enable VBUS sensing in the GCCFG register
    USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBDEN;

    // gpio_write(PIN_USB_VBUS, true);
    for(;;){
        // Optional: Check for session request (Vbus detection)
        if (USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_SRQINT)
        {
            printf("Session Request Detected (Vbus Present)\n");
        }
        bool vbus_active = USB_OTG_FS->GOTGCTL & USB_OTG_GOTGCTL_BSESVLD;
        // if(vbus_active) printf("VBUS active: %d\n", vbus_active);

        // bool session_active = USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_SRQINT;
        if(session_active) printf("Session Request Interrupt: %d\n", session_active);

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
    session_active = USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_SRQINT;

    // Check for USB reset interrupt (used when the USB host resets the device)
    if (gintsts & USB_OTG_GINTSTS_USBRST) {
        // printf("USB Reset Interrupt\n");
        USB_OTG_FS->GINTSTS = USB_OTG_GINTSTS_USBRST;  // Clear interrupt flag
    }

    // Handle enumeration done (when device enumeration is complete)
    if (gintsts & USB_OTG_GINTSTS_ENUMDNE) {
        // printf("USB Enumeration Done\n");
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

    // Handle any other interrupts you need to process
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

