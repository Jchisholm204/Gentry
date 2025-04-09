/**
 * @file adc_tsk.c
 * @author Jacob Chisholm (https://jchisholm.github.io)
 * @brief QSET ADC Task - Payload Control
 * @date 2025-04-09
 * @version 1.0
 *
 * This file is a mess, I know..
 *  It will be cleaned in time..
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "adc_tsk.h"
#include "pin_cfg.h"
#include "hal/hal_gpio.h"
#include "hal/hal_dma.h"

#define ADC_CHANNELS 2
// Factory calibration values for the internal temperature sensor
#define TS_CAL1_ADDR    ((uint32_t)0x1FFF7A2C)  // TS_CAL1 at 30°C
#define TS_CAL2_ADDR    ((uint32_t)0x1FFF7A2E)  // TS_CAL2 at 110°C
// The VREF (reference voltage) typically 3.3V
#define VREF            3.3f

// ADC resolution for a 12-bit ADC
#define ADC_RESOLUTION  4095.0f

// Temperature sensor characteristics
#define TS_CAL1_TEMP    30.0f  // Temperature at TS_CAL1
#define TS_CAL2_TEMP    110.0f // Temperature at TS_CAL2

// Task Details
static TaskHandle_t adc_hndl;
static StackType_t puADC_stack[configMINIMAL_STACK_SIZE];
static StaticTask_t pxADC_tsk;

uint16_t ADC_READINGS[ADC_CHANNELS];

void adc_tsk(void *pvParams);
void adc_initi(void);

// Initialize the ADC and its task
void adc_init(volatile struct udev_adc_info * const pADC_info){
    // Init STM Temp Sensor
    SET_BIT(ADC123_COMMON->CCR, ADC_CCR_TSVREFE);
    
    adc_initi();
    // Init STM Temp Sensor
    SET_BIT(ADC123_COMMON->CCR, ADC_CCR_TSVREFE);

    adc_hndl = xTaskCreateStatic(
                adc_tsk, "ADC", configMINIMAL_STACK_SIZE, (void*)pADC_info,
                1, puADC_stack, &pxADC_tsk
            );
}


void adc_tsk(void *pvParams){
    volatile struct udev_adc_info * const pInfo = pvParams;
    for(;;){
        // spin(99999UL);
        uint16_t reading = ADC_READINGS[0];
        float val = reading * 3.3;
        val /= 4095;
        pInfo->ADC_13 = val;
        float voltage = ((float)ADC_READINGS[1] / 4095.0f) * 3.3f;
        float temperature = ((voltage - 0.85f) / 0.0025f);// - 25.0f;
        pInfo->temp_internal = temperature;
        vTaskDelay(10);
    }
}

void adc_initi(void){
    
    // Zero out ADC Readings
    for(int i = 0; i < ADC_CHANNELS; i++){
        ADC_READINGS[i] = 0;
    }

    // Reduce the ADC Clock to APB2/8
    SET_BIT(ADC123_COMMON->CCR, 3 << 16);

    // Configure Analog GPIO Pins
    gpio_set_mode(PIN_THERM_ADC, GPIO_MODE_ANALOG);

    // Setup DMA for ADC1
    hal_dma_init(DMA2, DMA2_Stream0, DMA_CHANNEL_0, DMA_PRIORITY_LOW, DMA_MEM_SIZE_16, &(ADC1->DR), ADC_READINGS, ADC_CHANNELS);
    // Setup ADC1
    hal_adc_init(ADC1, ADC_RESOLUTION_12_BIT);
    // Setup Channel Sequence
    // Board ADC
    hal_adc_configChannel(ADC1, 13, ADC_CYCLES_480, ADC_SQ1);
    // STM Temp Sensor
    hal_adc_configChannel(ADC1, 18, ADC_CYCLES_480, ADC_SQ2);
    // Set Sequence Length
    hal_adc_set_sequence_len(ADC1, ADC_CHANNELS);

    spin(9999999UL); // Wait for ADC to stabilize

    // Enable the DMA Stream
    hal_dma_start(DMA2_Stream0);
    // Enable ADC and Start Conversions
    hal_adc_startConversions(ADC1);
}

