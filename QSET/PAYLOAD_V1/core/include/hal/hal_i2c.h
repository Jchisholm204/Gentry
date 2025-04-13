/**
 * @file hal_i2c.h
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief 
 * @date 2025-04-13
 * @version 0.1
 * 
 */
// I2C Reference Material:
// https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53
#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#include "hal_clock.h"
#include "hal_gpio.h"

// Wait for the condition with the timeout
#ifndef WAIT_FOR
#define WAIT_FOR(timeout, condition) for(int cnt_y = 0; cnt_y < timeout && condition; cnt_y++) __asm__("nop");
#endif

#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT 100000
#endif

/**
 * @brief Initialze an I2C Interface
 *
 * @param I2C I2C interface to init
 * @param pin_sda SDA Pin
 * @param pin_scl SCL Pin
 */
static inline void hal_i2c_init(I2C_TypeDef *I2C, pin_t pin_sda, pin_t pin_scl){
    if(I2C == I2C1) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
    if(I2C == I2C2) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN);
    if(I2C == I2C3) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C3EN);
    // Setup the I2C Pins (Must be Open Drain)
    gpio_set_mode(pin_scl, GPIO_MODE_AF);
    gpio_set_af(pin_scl, GPIO_AF_I2C);
    gpio_set_od(pin_scl, true);
    gpio_pull(pin_scl, GPIO_PULLUP);
    gpio_set_mode(pin_sda, GPIO_MODE_AF);
    gpio_set_af(pin_sda, GPIO_AF_I2C);
    gpio_pull(pin_sda, GPIO_PULLUP);
    gpio_set_od(pin_sda, true);

    // Reset the I2C interface
    SET_BIT(I2C->CR1, I2C_CR1_SWRST);
    WAIT_FOR(I2C_TIMEOUT, 0);
    CLEAR_BIT(I2C->CR1, I2C_CR1_SWRST);
    // Setup I2C Clock Divider
    // APB clock should be 42 MHz?
    CLEAR_REG(I2C->CR2);
    SET_BIT(I2C->CR2, (uint8_t)((uint32_t)APB1_FREQUENCY/1e6) & 0x3F);
    // SCL line frequency (set to 100kHz)
    CLEAR_REG(I2C->CCR);
    SET_BIT(I2C->CCR, APB1_FREQUENCY/(2*100000) & 0xFFF);
    // SET_BIT(I2C->TRISE, 43);
    I2C->TRISE = (APB1_FREQUENCY/1000000) + 1;
    // Enable I2C
    SET_BIT(I2C->CR1, I2C_CR1_PE);
}

/**
 * @brief Write a single byte to an i2c interface
 *
 * @param I2C I2C Interface
 * @param dev_addr Device Address
 * @param reg_addr Device Register Address to write into
 * @param value Value to write into the register
 */
static inline void hal_i2c_write_byte(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr, uint8_t value) {
    // Generate START condition
    SET_BIT(I2C->CR1, I2C_CR1_START);
    SET_BIT(I2C->CR1, I2C_CR1_ACK);
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB));

    // Send device address (write)
    I2C->DR = dev_addr & ~0x01;  // Ensure LSB = 0 for write
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR));

    (void)I2C->SR2;  // Clear ADDR by reading SR1 and SR2

    // Send register address
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));

    // Send data
    I2C->DR = value;
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    // while (!(I2C->SR1 & I2C_SR1_BTF));  // Wait for byte transfer finished

    // Generate STOP condition
    SET_BIT(I2C->CR1, I2C_CR1_STOP);
}

/**
 * @brief Write a stream of bytes to an i2c interface
 *
 * @param I2C I2C Interface
 * @param dev_addr Device Address
 * @param reg_addr Device Register Address to write into
 * @param bytes pointer to the byte stream
 * @param n_bytes The number of bytes to write to the device
 */
static inline void hal_i2c_write(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr, uint8_t *bytes, size_t n_bytes) {
    // Generate START condition
    SET_BIT(I2C->CR1, I2C_CR1_START);
    SET_BIT(I2C->CR1, I2C_CR1_ACK);
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB));

    // Send device address (write)
    I2C->DR = dev_addr & ~0x01;  // Ensure LSB = 0 for write
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR));

    (void)I2C->SR2;  // Clear ADDR by reading SR1 and SR2

    // Send register address
    // while (!(I2C->SR1 & I2C_SR1_TXE));
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));
    // while (!(I2C->SR1 & I2C_SR1_TXE));

    // Send data
    for(size_t i = 0; i < n_bytes; i++){
        I2C->DR = bytes[i];
        WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    }

    // Generate STOP condition
    SET_BIT(I2C->CR1, I2C_CR1_STOP);
}


/**
 * @brief Read from an I2C Device
 *
 * @param I2C I2C Interface to read from
 * @param dev_addr Address of device to read from
 * @param reg_addr Register Address to read from
 * @returns the byte read from the device
 */
static inline uint8_t hal_i2c_read_byte(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr) {
    volatile uint32_t status;
    uint8_t data;
    
    // 1. Send START and device address (write)
    SET_BIT(I2C->CR1, I2C_CR1_START);
    // Wait for START generated
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB)); 
    
    // Write to device address
    I2C->DR = dev_addr & ~0x01; 
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR)); 
    status = I2C->SR1 | I2C->SR2;
    
    // 2. Send register address
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    // Wait for completion
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    
    // 3. Repeated START and switch to read
    SET_BIT(I2C->CR1, I2C_CR1_START);
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB));
    
    // Read from the device address
    I2C->DR = dev_addr | 0x01;
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR));
    status = I2C->SR1 | I2C->SR2;
    
    // Setup for read
    CLEAR_BIT(I2C->CR1, I2C_CR1_ACK);
    // Ensure the bus is clocked by the i2c master
    SET_BIT(I2C->CR1, I2C_CR1_POS);
    
    // 5. Read data and STOP
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_RXNE)); 
    data = (uint8_t)I2C->DR;
    SET_BIT(I2C->CR1, I2C_CR1_STOP);

    (void)status;
    return data;
}


/**
 * @brief Read from an I2C Device
 *
 * @param I2C I2C Interface to read from
 * @param dev_addr Address of device to read from
 * @param reg_addr Register Address to read from
 * @param bytes Array to fill with the bytes read from the device
 * @param n_bytes Number of bytes to read from the device
 * @returns status information from the I2C interface
 */
static inline uint32_t hal_i2c_read(I2C_TypeDef *I2C, uint8_t dev_addr, uint8_t reg_addr, uint8_t *bytes, size_t n_bytes) {
    volatile uint32_t status;
    
    // 1. Send START and device address (write)
    SET_BIT(I2C->CR1, I2C_CR1_START);
    // Wait for START generated
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB)); 
    
    // Write to device address
    I2C->DR = dev_addr & ~0x01; 
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR)); 
    status = I2C->SR1 | I2C->SR2;
    
    // 2. Send register address
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_TXE));
    I2C->DR = reg_addr;
    // Wait for completion
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_BTF));
    
    // 3. Repeated START and switch to read
    SET_BIT(I2C->CR1, I2C_CR1_START);
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_SB));
    
    // Read from the device address
    I2C->DR = dev_addr | 0x01;
    WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_ADDR));
    status = I2C->SR1 | I2C->SR2;
    
    // Setup for multiread
    SET_BIT(I2C->CR1, I2C_CR1_ACK);
    // Ensure the bus is clocked by the i2c master
    SET_BIT(I2C->CR1, I2C_CR1_POS);
    
    // 5. Read data and STOP
    for(size_t i = 0; i < n_bytes; i++){
        if((i + 1) >= n_bytes){
            SET_BIT(I2C->CR1, I2C_CR1_STOP);
            CLEAR_BIT(I2C->CR1, I2C_CR1_ACK);
        }
        else if((i + 2) >= n_bytes)
            CLEAR_BIT(I2C->CR1, I2C_CR1_POS);

        // Wait for data
        WAIT_FOR(I2C_TIMEOUT, !(I2C->SR1 & I2C_SR1_RXNE)); 
        bytes[i] = (uint8_t)I2C->DR;
    }
    return status;
}


#endif

