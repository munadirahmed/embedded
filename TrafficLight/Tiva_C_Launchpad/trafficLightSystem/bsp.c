/* Board Support Package */
/*
 * Filename: bsp.c
 * Description: Source file for the board support package for the EK-TM4C123GXL board
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */

// Module used
#include <stdint.h>

#include "TM4C123GH6PM.h"
#include "bsp.h"
#include  "core_cm4.h"


// Define constants

#define SYSTICK_ISR_PERIOD_MS 1000U //[ms] = systick ISR period WARNING: at 16MHz max ISR period is 1.048576sec


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
    GPIOF_AHB->DATA_Bits[LED_RED] ^= LED_RED;
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


//EOF
