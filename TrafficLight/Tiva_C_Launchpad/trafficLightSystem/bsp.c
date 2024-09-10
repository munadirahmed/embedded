/* Board Support Package */
/*
 * Filename: bsp.c
 * Description: Source file for the board support package for the EK-TM4C123GXL board
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */

// Module used
#include <stdint.h>
#include <stdio.h>

#include "TM4C123GH6PM.h"
#include "bsp.h"
#include  "core_cm4.h"
#include "app_main.h"
#include "config.h"

// Define data types and structures

// Define constants

#define SYSTICK_ISR_PERIOD_MS TRAFFIC_LIGHT_SM_EXECUTION_TIME_MS //[ms] = systick ISR period WARNING: at 20MHz max ISR period is 0.83886sec


// Declare static global variables
static uint32_t adcRawData[TOTAL_ADC_CH];

// Define local function prototypes

static void readRawAdcBuffers(void);
static inline void initiateAdc0SamplingSequence(void);
static inline void clearAdc0SamplingSequenceInterrupt(void);

// Declare function


// Assert Fail function definition (needs updating)
__attribute__((naked)) void assert_failed (char const *file, int line) {
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

// SysTick ISR function definition
void SysTick_Handler(void) {
    //GPIOF_AHB->DATA_Bits[LED_RED] ^= LED_RED;
    initiateAdc0SamplingSequence(); // initiate the ADC sampling
    runApplication();
}

/**
 * Initiate the ADC0 SS0 ISR to trigger the SOC of the ADC channels
 *
 * @param N/A
 *
 * @return - N/A
 */
static inline void  initiateAdc0SamplingSequence(void) {
    ADC0->PSSI |= (1U<<0);
}

/**
 * ISR for the ADC0 SS0 interrupt - configured to execute at the EOC of the second sample and used to copy data from FIFO to structure holding appropriate ADC data
 *
 * @param N/A
 *
 * @return - N/A
 */
void ADCSeq0_IRQHandler(void)
{
    clearAdc0SamplingSequenceInterrupt();
    // Read all the ADC values into the raw data array
    readRawAdcBuffers();
}

/**
 * Clear the ADC0 SS0 interrupt
 *
 * @param N/A
 *
 * @return - N/A
 */
static inline void clearAdc0SamplingSequenceInterrupt(void)
{
    ADC0->ISC |= (1U<<0); // Clear the interrupt
}

/**
 * Configure the System Timer (SysTick) Module to execute the SysTick ISR at the desired rate
 *
 * @param N/A
 *
 * @return - N/A
 */
void sysTickModuleConfig(void)
{
    const uint32_t systick_isr_prd_tic = ( (uint32_t)(SYS_CLOCK_HZ / 1000U) ) * SYSTICK_ISR_PERIOD_MS ; // tics corresponding to desired ISR period

    SysTick->LOAD = systick_isr_prd_tic - 1U;  // start value to load into the SysTick Current Value (STCURRENT) register when the counter reaches 0.
    SysTick->VAL  = 0U;

    uint32_t systick_ENABLE_bit = (1U << SysTick_CTRL_ENABLE_Pos);  // enable systick counter
    uint32_t systick_INTEN_bit = (1U << SysTick_CTRL_TICKINT_Pos);  // enable systick interrupt
    uint32_t systick_CLK_SRC_bit = (1U << SysTick_CTRL_CLKSOURCE_Pos);  // use the system clock as the source

    SysTick->CTRL = systick_ENABLE_bit | systick_INTEN_bit | systick_CLK_SRC_bit;
}

/**
 * Configure GPIO Port F as primary road and Port D as secondary road lights
 *
 * @param N/A
 *
 * @return - N/A
 */
void configureGPIO_TrafficLightPorts(void)
{
    // See pg 656, section 10.3 Initialization and Configuration of Tiva™ TM4C123GH6PM Microcontroller datasheet for GPIO configuration recommendations
    // Configure Port F (Main Road LED)
    SYSCTL->GPIOHBCTL |= (1U << PORT_F); /* enable AHB for GPIOF */
    SYSCTL->RCGCGPIO  |= (1U << PORT_F); /* enable Run mode for GPIOF */

    GPIOF_AHB->DIR |= (PRI_ROAD_LED_RED | PRI_ROAD_LED_YELLOW | PRI_ROAD_LED_GREEN);  // configure as outputs
    GPIOF_AHB->DEN |= (PRI_ROAD_LED_RED | PRI_ROAD_LED_YELLOW | PRI_ROAD_LED_GREEN);  // enable gpio functionality

    /* turn all LEDs off */
    GPIOF_AHB->DATA_Bits[PRI_ROAD_LED_RED | PRI_ROAD_LED_YELLOW | PRI_ROAD_LED_GREEN] = 0U;

    // Configure Port D (Secondary Road LED)
    SYSCTL->GPIOHBCTL |= (1U << PORT_D); /* enable AHB for GPIOD */
    SYSCTL->RCGCGPIO  |= (1U << PORT_D); /* enable Run mode for GPIOD */

    GPIOD_AHB->DIR |= (SEC_ROAD_LED_RED | SEC_ROAD_LED_YELLOW | SEC_ROAD_LED_GREEN);  // configure as outputs
    GPIOD_AHB->DEN |= (SEC_ROAD_LED_RED | SEC_ROAD_LED_YELLOW | SEC_ROAD_LED_GREEN);  // enable gpio functionality

    /* turn all LEDs off */
    GPIOD_AHB->DATA_Bits[SEC_ROAD_LED_RED | SEC_ROAD_LED_YELLOW | SEC_ROAD_LED_GREEN] = 0U;
}

/**
 * Configure Pin E3 as secondary road input sensor
 *
 * @param N/A
 *
 * @return - N/A
 */
void configureADC_TrafficLightSecondaryRoadVehicleSensor(void)
{
    /*
     * See pg 817, section 13.4 Initialization and Configuration of Tiva™ TM4C123GH6PM Microcontroller datasheet for ADC configuration recommendations
     * See pg 656, section 10.3 Initialization and Configuration of Tiva™ TM4C123GH6PM Microcontroller datasheet for GPIO configuration recommendations
     */

    /********* CONFIGURE THE PORT TO BE USED AS AN ADC INPUT *********/
    // Configure Port E3 (AIN0) as input sensor using ADC
    SYSCTL->RCGCADC |= (1U << 0); /* enable ADC0 module */

    SYSCTL->GPIOHBCTL |= (1U << PORT_E); /* enable AHB for GPIOE */
    SYSCTL->RCGCGPIO  |= (1U << PORT_E); /* enable Run mode for GPIOE */

    GPIOE_AHB->AFSEL |= ((1 << PE3) | (1 << PE1)); /* configure PE1 & PE3 as alternate function - this pin can only be ADC input */

    GPIOE_AHB->DEN &= ~( (1 << PE3) | (1 << PE1)); /*clear bit to configure PE1 & PE3 as analog*/
    GPIOE_AHB->AMSEL |= ((1 << PE3) | (1 << PE1)); /* for pin PE1 & PE3: analog function of the pin is enabled, the isolation is disabled, and the pin is capable of analog functions */

    /********* CONFIGURE SAMPLE SEQUENCER FOR APPLICATION USAGE *********/

    //First disable all the sequencer for programming
    ADC0->ACTSS &= 0xFFFFFFF0UL;

    uint32_t MUX0 = 0x0;  // mux0 assigned to AIN0
    uint32_t MUX1 = 0x2;  // mux1 assigned to AIN2

    ADC0->EMUX |= 0x00000000UL;  // make ADC0 start sampling via SW trigger
    ADC0->SSMUX0 |= ( (MUX1 << 4)  | ((MUX0 << 0)) ) ;  // configure lowest byte for mux0 and mux1 assignments, leave all other mux's unchanged)
    ADC0->SSCTL0 |= ( (1U<<5) | (1U<<6) ) ;  // configure end sampling after the second sample and configure interrupt to trigger after EOC of 2nd sample
    ADC0->IM &= 0xFFF0FFF0UL;  // disable all interrupt/triggers
    ADC0->IM |= (1 << 0);  // enable interrupt for sample sequencer 0

    __NVIC_EnableIRQ(ADC0SS0_IRQn); // Enable interrupt generation in NVIC for SS0 on ADC0 module

    //Enable the SS0 after programming is finished (only using SS0)
    ADC0->ACTSS |= 0x00000001UL;

}

/**
 * Read input values into buffer
 *
 * @param chId - channel ID for adc conversion result of interest
 *
 * @return - 12-bit conversion result for the ADC channel of interest as a 16-bit integer
 */
uint16_t getSensorRawInputValue(im_adc_ch_t chId)
{
    uint32_t sensorValue = adcRawData[chId];

    sensorValue &= 0x00000FFFUL;

    return ( (uint16_t) sensorValue );
}


/**
 * Read all ADC channels
 *
 * @param N/A
 *
 * @return - N/A
 */
void readRawAdcBuffers(void)
{

    for(uint8_t chIdx = 0; chIdx < (uint8_t)TOTAL_ADC_CH; chIdx++)
    {
        adcRawData[chIdx] = ADC0->SSFIFO0;
    }
}

/**
 * Configure the System Timer (SysTick) Module to execute the SysTick ISR at the desired rate
 *
 * @param N/A
 *
 * @return - N/A
 */
void set_digital_out(om_dig_ch_t ch,trafficLight_Color_t lightColor)
{
    //TODO: refactor to use better structure for looking up HW address: maybe array?
    GPIOA_Type * ptr = 0;
    if(ch == OM_DIG_CH_PRIMARY_ROAD)
    {
        ptr = GPIOF_AHB;
    }else if(ch == OM_DIG_CH_SECONDARY_ROAD)
    {
        ptr = GPIOD_AHB;
    }else
    {
        // Do Nothing
    }

    //TODO: temporary solution but will not work if primary and secondary pin number assignments are not identical!!
    uint8_t lightOutput = 0U;
    if(TRAFFIC_LIGHT_COLOR_GREEN == lightColor){
        lightOutput = PRI_ROAD_LED_GREEN;
    }else if(TRAFFIC_LIGHT_COLOR_YELLOW == lightColor)
    {
        lightOutput = PRI_ROAD_LED_YELLOW;
    }else if(TRAFFIC_LIGHT_COLOR_RED == lightColor)
    {
        lightOutput = PRI_ROAD_LED_RED;
    }else // (TRAFFIC_LIGHT_OFF)
    {
        lightOutput = 0U;  // turn all lights off
    }

    /* Set the output for the corresponding digital output channel */
    ptr->DATA_Bits[PRI_ROAD_LED_RED |PRI_ROAD_LED_YELLOW | PRI_ROAD_LED_GREEN] = lightOutput;
}


//EOF
