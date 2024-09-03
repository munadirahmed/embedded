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

// Define local function prototypes

// Declare function


// Assert Fail function definition (needs updating)
__attribute__((naked)) void assert_failed (char const *file, int line) {
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

// SysTick ISR function definition
void SysTick_Handler(void) {
    //GPIOF_AHB->DATA_Bits[LED_RED] ^= LED_RED;
    runApplication();
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
    SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */
    SYSCTL->RCGCGPIO  |= (1U << 5); /* enable Run mode for GPIOF */

    GPIOF_AHB->DIR |= (LED_RED | LED_YELLOW | LED_GREEN);  // configure as outputs
    GPIOF_AHB->DEN |= (LED_RED | LED_YELLOW | LED_GREEN);  // enable gpio functionality

    /* turn all LEDs off */
    GPIOF_AHB->DATA_Bits[LED_RED | LED_YELLOW | LED_GREEN] = 0U;

    // Configure Port D (Secondary Road LED)
    SYSCTL->GPIOHBCTL |= (1U << 3); /* enable AHB for GPIOD */
    SYSCTL->RCGCGPIO  |= (1U << 3); /* enable Run mode for GPIOD */

    GPIOD_AHB->DIR |= (LED_RED | LED_YELLOW | LED_GREEN);  // configure as outputs
    GPIOD_AHB->DEN |= (LED_RED | LED_YELLOW | LED_GREEN);  // enable gpio functionality

    /* turn all LEDs off */
    GPIOD_AHB->DATA_Bits[LED_RED | LED_YELLOW | LED_GREEN] = 0U;
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

    SYSCTL->GPIOHBCTL |= (1U << 4); /* enable AHB for GPIOE */
    SYSCTL->RCGCGPIO  |= (1U << 4); /* enable Run mode for GPIOE */

    GPIOE_AHB->AFSEL |= (1U << 3); /* configure PE3 as alternate function - this pin can only be ADC input */
    GPIOE_AHB->DEN &= ~((uint8_t)1 << 4); /*clear bit to configure port E as analog*/
    GPIOE_AHB->AMSEL |= (1U << 3); /* for pin PE3: analog function of the pin is enabled, the isolation is disabled, and the pin is capable of analog functions */

    /********* CONFIGURE SAMPLE SEQUENCER FOR APPLICATION USAGE *********/

    //First disable all the sequencer for programming
    ADC0->ACTSS &= 0xFFFFFFF0UL;

    ADC0->EMUX |= 0x0000000FUL;  // make ADC0 always sample TODO: update sampling scheme to be more efficient
    ADC0->SSMUX0 &= 0xFFFFFFF0UL;  // configure Mux0 to sample AIN0
    ADC0->SSMUX0 |= (1U<<1);  // configure end sampling after the first sample
    ADC0->IM &= 0xFFF0FFF0UL;  // disable all interrupt/triggers

    //Enable the SS0 after programming is finished (only using SS0)
    ADC0->ACTSS |= 0x00000001UL;



}

uint16_t getSensorRawInputValue(void)
{
    uint32_t sensorValue = ADC0->SSFIFO0;

    sensorValue &= 0x00000FFFUL;

    return ( (uint16_t) sensorValue );
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

    uint8_t lightOutput = 0U;
    if(TRAFFIC_LIGHT_COLOR_GREEN == lightColor){
        lightOutput = LED_GREEN;
    }else if(TRAFFIC_LIGHT_COLOR_YELLOW == lightColor)
    {
        lightOutput = LED_YELLOW;
    }else if(TRAFFIC_LIGHT_COLOR_RED == lightColor)
    {
        lightOutput = LED_RED;
    }else // (TRAFFIC_LIGHT_OFF)
    {
        lightOutput = 0U;  // turn all lights off
    }

    /* Set the output for the corresponding digital output channel */
    ptr->DATA_Bits[LED_RED | LED_YELLOW | LED_GREEN] = lightOutput;
}


//EOF
