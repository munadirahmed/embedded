/* Main Application File */
/*
 * Filename: app_main.h
 * Description: Main application header file that defines the interface for the application-specific functions.
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */
#ifndef APP_MAIN_H_
#define APP_MAIN_H_

typedef enum {
    TRAFFIC_LIGHT_COLOR_RED= 0U,
    TRAFFIC_LIGHT_COLOR_YELLOW,
    TRAFFIC_LIGHT_COLOR_GREEN
} trafficLight_Color_t;

void initializeApplication(void);
void runApplication(void);


#endif /* APP_MAIN_H_ */


//EOF
