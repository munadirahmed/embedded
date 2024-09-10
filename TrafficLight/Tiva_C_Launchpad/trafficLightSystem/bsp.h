/*
 * Filename: bsp.h
 * Description: Header file for the board support package for the EK-TM4C123GXL board
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */

#ifndef __BSP_H__
#define __BSP_H__

#include "app_main.h"

typedef enum {
    OM_DIG_CH_PRIMARY_ROAD=0U,
    OM_DIG_CH_SECONDARY_ROAD,
    OM_TOTAL_OUTPUT_CH
}om_dig_ch_t;

typedef enum {
    ADC_CH_SECONDARY_ROAD_VEHICLE_SENSOR=0U,
    ADC_CH_FAULT_SENSOR,
    TOTAL_ADC_CH
}im_adc_ch_t;

/* system clock setting [Hz] - 20MHz (confirm that RCC/RCC2 register configured for 20MHz) */
#define SYS_CLOCK_HZ (20000000U)

/* Ports used in project*/
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5

/* Pins used in project*/
#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3

#define PE0 0
#define PE1 1
#define PE2 2
#define PE3 3

#define PF0 0
#define PF1 1
#define PF2 2
#define PF3 3

/* Primary Road and Sec Road Lights Lights */
#define PRI_ROAD_LED_RED     (1U << PF1)
#define PRI_ROAD_LED_YELLOW  (1U << PF2)
#define PRI_ROAD_LED_GREEN   (1U << PF3)

#define SEC_ROAD_LED_RED     (1U << PD1)
#define SEC_ROAD_LED_YELLOW  (1U << PD2)
#define SEC_ROAD_LED_GREEN   (1U << PD3)


void sysTickModuleConfig(void);
void configureGPIO_TrafficLightPorts(void);
void configureADC_TrafficLightSecondaryRoadVehicleSensor(void);
void set_digital_out(om_dig_ch_t ch,trafficLight_Color_t lightColor);

uint16_t getSensorRawInputValue(im_adc_ch_t chId);


#endif // __BSP_H__
