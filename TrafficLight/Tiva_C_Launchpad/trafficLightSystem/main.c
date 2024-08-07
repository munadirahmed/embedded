/*
 * Filename: main.c
 * Description:
 * This file contains the main entry point for the application. It initializes
 * the system, configures necessary peripherals, and enters the main loop
 * where it performs the primary tasks of the application. The main loop
 * typically handles event processing, system monitoring, and control logic.
 *
 * Primary Functions:
 * - System initialization
 * - Peripheral configuration
 * - Main application loop
 *
 * Author: Munadir Ahmed
 * Date:
 */

// Module used

#include "TM4C123GH6PM.h"
#include "bsp.h"
#include "app_main.h"

// Declare static globals

// Define local function prototypes

void initializeSystem(void);

// Declare function

int main(void) {


    // Configure the system
    initializeSystem();

    // Initialize the application
    initializeApplication();

    __enable_irq();
    while (1) {
        //GPIOF_AHB->DATA_Bits[LED_BLUE] = LED_BLUE;
        //GPIOF_AHB->DATA_Bits[LED_BLUE] = 0U;
    }
    //return 0; // unreachable code
}

/**
 * Initialize all peripherals
 *
 * @param N/A
 *
 * @return - N/A
 */
void initializeSystem(void)
{
    sysTickModuleConfig();  // Configure the System Timer module
    configureGPIO_PortF();
}
