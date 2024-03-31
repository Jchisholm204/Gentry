/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief Bootloader modification testing for STM32F446RE
 * @date 2024-03-30
 * 
 */

#include "hal.h"

#define FLASH_APP_ADDR 0x8000000
#define mySection __attribute__((section(".mysection")))

typedef void (*ptrF)(bool on);
typedef void (*pFunction)(void);

void go2App(void);

void mySection write(bool on){
    gpio_write(PIN('B', 1), on);
}

void __attribute__((__section__(".RamFunc"))) ledChangeState(bool state){
    gpio_write(PIN('B', 0), state);
}

void go2App(void){
    uint32_t jmpAddr;
    pFunction jmpTo;
    jmpAddr = *(uint32_t *)(FLASH_APP_ADDR + 4UL);
    jmpTo = (pFunction) jmpAddr;
    __set_MSP(*(uint32_t *)FLASH_APP_ADDR);
    jmpTo();
}

static ptrF Functions[] = {
    write
};

static volatile uint32_t s_ticks = 0xBEEF;
void SysTick_Handler(void){
    s_ticks++;
}

volatile bool led_on = true;
volatile uint64_t increment = 0xDEADBEEF;


uint32_t SystemCoreClock = FREQ;
void SystemInit(void){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(FREQ/1000);
}

int main(void){
    uint16_t led1 = PIN('B', 0);
    uint16_t led2 = PIN('B', 1);
    //SysTick_Config(FREQ / 1000);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    //uart_init(USART2, 9600);
    volatile uint32_t timer = 0, period = 500;
    gpio_write(led2, true);
    gpio_write(led1, false);
    s_ticks = 0x0;
    for(;;) {
        if(timer_expired(&timer, period, s_ticks)){
            //gpio_write(led1, led_on);
            ledChangeState(!led_on);
            (*Functions[0])(led_on);
            led_on = !led_on;
            //printf("HI\n");
        }
    }
    return 0;
}

