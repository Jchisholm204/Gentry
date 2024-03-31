/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * from bare metal programming guide by --
 * @brief simple led blinking program
 * @date 2023-08-30
 * 
 */
#include "inttypes.h"
#include "stdbool.h"

// Macros to make code more readable
#define BIT(x) (1UL << (x))
// Package a pin bank (U8) and pin number (U8) into single package (U16)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
// Retrieve pin number (U8) from pin package (U16)
#define PINNO(pin) (pin & 255)
// Retrieve pin bank (U8) from pin package (U16)
#define PINBANK(pin) (pin >> 8)

// struct for referencing GPIO memory (ex. GPIO bank A)
struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LKR, AFR[2];
};

// Reference memory address for STM32F446RE GPIO banks
#define GPIO(bank) ((struct gpio *) (0x40020000 + 0x400 * (bank)))

// STM32 GPIO Modes (for MODER)
enum GPIO_MODE {
    INPUT, OUTPUT, AF, ANALOG
};

/**
 * @brief Set the PinMode of a GPIO Pin
 * 
 * @param pin PIN(bank, number)
 * @param mode GPIO_MODE::<INPUT, OUTPUT, AF, ANALOG>
 */
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->MODER &= ~(3U << (n*2));
    gpio->MODER |= (mode & 3UL) << (n*2);

}

// Struct for accessing STM32F446 Power Control Module
struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
      RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
      RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
      AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
      RESERVED6[2], SSCGR, PLLI2SCFGR;
};

// STM32F446RE RCC Memory Addr
#define RCC ((struct rcc *) 0x40023800)

/**
 * @brief Write a Digital Value to a pin
 * 
 * @param pin PIN(bank, number)
 * @param value <true, false>
 */
static inline void gpio_write(uint16_t pin, bool value) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
}

/**
 * @brief Simple Counter delay
 * 
 * @param count countto
 */
static inline void count_delay(volatile uint32_t count) {
    while(count--) (void) 0;
}

int main(void){
    uint16_t led1 = PIN('B', 0);
    RCC->AHB1ENR |= BIT(PINBANK(led1));
    gpio_set_mode(led1, OUTPUT);
    for(;;) {
        gpio_write(led1, true);
        count_delay(999999);
        gpio_write(led1, false);
        count_delay(999999);
    }
    return 0;
}


// Startup Code
// Loads main function from flash into SRAM on startup
__attribute__((naked, noreturn)) void _reset(void) {
    // menset .bss to zero and copy the .data section to the ram region
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst=0;
    for (long *dst =&_sdata, *src = &_sidata; dst < & _edata;) *dst ++ = *src++;

    main(); // call main()
    for (;;) (void) 0; // infinite loop if main returns
}

extern void _estack(void); // defined in link.ld

// 16 standard (mandated by arm arch) and 97 stm32 entries (f446RE vector table (table 38))
__attribute__((section(".vectors"))) void (*const tab[16+97])(void) = {
    _estack, _reset
};