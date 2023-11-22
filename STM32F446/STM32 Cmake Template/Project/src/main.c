/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * from bare metal programming guide by --
 * @brief simple led blinking program
 * @date 2023-08-30
 * 
 */

#include "hal.h"

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
    float i = 0.0;
    uint16_t led1 = PIN('B', 0);
    uint16_t led2 = PIN('B', 1);
    //SysTick_Config(FREQ / 1000);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    uart_init(USART2, 9600);
    volatile uint32_t timer = 0, period = 100;
    gpio_write(led2, true);
    s_ticks = 0x0;
    for(;;) {
        if(timer_expired(&timer, period, s_ticks)){
            gpio_write(led1, led_on);
            led_on = !led_on;
            int f = (int)(i*100);
            printf("LED: %d, %0.2f\r\n", led_on, i);//print bricks
            //printf("HI\n");
            i+= 0.2;

            //uart_write_buf(USART2, "HI\n", 4);
        }
    }
    return 0;
}