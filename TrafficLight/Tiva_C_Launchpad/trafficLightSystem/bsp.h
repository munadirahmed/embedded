/*
 * Filename: bsp.h
 * Description: Header file for the board support package for the EK-TM4C123GXL board
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */

#ifndef __BSP_H__
#define __BSP_H__

/* system clock setting [Hz] - 16MHz */
#define SYS_CLOCK_HZ 16000000U

/* on-board LEDs */
#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)

void sysTickModuleConfig(void);
void configureGPIO_PortF(void);

#endif // __BSP_H__
