/* Main Application File */
/*
 * Filename: app_main.c
 * Description: Contains the implementation of application-specific functions
 * Author: Munadir Ahmed
 * Date: YYYY-MM-DD
 */

/***************  Module used ********************/

#include <stdint.h>

#include "app_main.h"
#include "config.h"

/***************  Define data types and structures ********************/

typedef enum {
    INIT= 0u,
    NORMAL,
    FLASHING,
    ERROR
} trafficLight_MainStates_t;

typedef enum {
    NORMAL_MODE_IDLE= 0u,
    NORMAL_MODE_ALL_LANES_RED,
    NORMAL_MODE_PRIMARY_ROAD_GREEN,
    NORMAL_MODE_PRIMARY_ROAD_YELLOW,
    NORMAL_MODE_SECONDARY_ROAD_GREEN,
    NORMAL_MODE_SECONDARY_ROAD_YELLOW
} trafficLight_NormalModeSubStates_t;

typedef enum {
    FLASHING_MODE_IDLE= 0u,
    FLASHING_MODE_ALL_LANES_RED,
    FLASHING_MODE_PRIMARY_ROAD_YELLOW_SEC_ROAD_RED,
    FLASHING_MODE_ALL_LANES_OFF
} trafficLight_FlashingModeSubStates_t;

typedef struct {
    trafficLight_MainStates_t currentMainState;   // holds the current main state for the traffic light
    trafficLight_NormalModeSubStates_t currentNrmlModeSubState;  // holds the current substate when traffic light operating in normal operating mode
    trafficLight_FlashingModeSubStates_t currentFlshingModeSubState;  // holds the current substate when traffic light operating in flashing operating mode
    uint16_t timer; // Timer for managing state transitions
} trafficLightClass_t;


/***************  Define constants ********************/

/***************  Declare static global variables ********************/

static trafficLightClass_t trafficLightObject;

/*************** Define local function prototypes ********************/

// Main state machine helper functions

// All main state machine entry actions for each state
static void onStateEnterTrafficLightMainSm_Init(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightMainSm_Normal(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightMainSm_Flashing(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightMainSm_Error(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runTrafficLightStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

// All Normal sub-state machine helper functions

static void onStateEnterTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runTrafficLightNormalModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runSubStateTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

// All Flashing sub-state machine helper functions

static void onStateEnterTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runTrafficLightFlashingModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runSubStateTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

/***************  Declare function (local and global)  ********************/

// Functions that interact with external modules
/**
 * Initialize application level parameters
 *
 * @param - N/A
 *
 * @return - N/A
 */
void initializeApplication(void)
{
    trafficLightObject.timer = 0U;
    trafficLightObject.currentMainState = INIT;
    trafficLightObject.currentNrmlModeSubState = NORMAL_MODE_IDLE;
    trafficLightObject.currentFlshingModeSubState = FLASHING_MODE_IDLE;
    onStateEnterTrafficLightMainSm_Init(&trafficLightObject);
}

void runApplication(void)
{
    runTrafficLightStateMachine(&trafficLightObject);
}


// Local functions

/**
 * Tasks to perform upon entry of main state machine to INIT state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightMainSm_Init(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = TRAFFIC_LIGHT_SYSTEM_INIT_COUNT;
}

/**
 * Tasks to perform upon entry of main state machine to NORMAL state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightMainSm_Normal(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_ALL_LANES_RED;  // set the Normal mode sub-state machine to ALL_LANES_RED
    trafficLightObject.currentFlshingModeSubState = FLASHING_MODE_IDLE; // reset flashing mode sub-state machine to IDLE

    onStateEnterTrafficLightNormalSubSm_AllLanesRed(tl_sm_obj_ptr);  // Initialize parameters for all lanes red

}

/**
 * Tasks to perform upon entry of main state machine to FLASHING state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightMainSm_Flashing(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;  // reset flashing mode sub-state machine
    trafficLightObject.currentFlshingModeSubState = FLASHING_MODE_ALL_LANES_RED;

    onStateEnterTrafficLightFlashingSubSm_AllLanesRed(tl_sm_obj_ptr);  // Initialize parameters for all lanes red
}


/**
 * Tasks to perform upon entry of main state machine to ERROR state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightMainSm_Error(trafficLightClass_t *tl_sm_obj_ptr)
{
// TODO: add handling of error state
}

/**
 * Execution of the traffic light sub-state machine when in normal operating mode
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runTrafficLightStateMachine(trafficLightClass_t *tl_sm_obj_ptr)
{

    //Decrement the timer if not zero and saturate to 0
    if(1U >= tl_sm_obj_ptr->timer)
    {
        tl_sm_obj_ptr->timer = 0U;
    }else
    {
        tl_sm_obj_ptr->timer--;
    }

    // Main State machine
    switch (tl_sm_obj_ptr->currentMainState)
    {
    case INIT:
        //Do nothing To lights
        break;
    case NORMAL:
        runTrafficLightNormalModeSubStateMachine(tl_sm_obj_ptr);
        break;
    case FLASHING:
        //TODO: add handling for runTrafficLightFlashingModeSubStateMachine(tl_sm_obj_ptr);
        break;
    case ERROR:
        //TODO:add handling for error state
        break;
    default:
        //Should never get here
        tl_sm_obj_ptr->currentMainState = INIT;
        break;
    }
}

/**
 * Execution of the traffic light sub-state machine when in normal operating mode
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runTrafficLightNormalModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr) {
    switch (tl_sm_obj_ptr->currentNrmlModeSubState)
    {
    case NORMAL_MODE_IDLE:
        //Do nothing To lights
        break;
    case NORMAL_MODE_ALL_LANES_RED:
        //handlePrimaryGreen(tl_sm_obj);
        break;
    case NORMAL_MODE_PRIMARY_ROAD_GREEN:
        //handlePrimaryYellow(tl_sm_obj);
        break;
    case NORMAL_MODE_PRIMARY_ROAD_YELLOW:
        //handlePrimaryRed(tl_sm_obj);
        break;
    case NORMAL_MODE_SECONDARY_ROAD_GREEN:
        //handleSecondaryGreen(tl_sm_obj);
        break;
    case NORMAL_MODE_SECONDARY_ROAD_YELLOW:
        //handleSecondaryYellow(tl_sm_obj);
        break;
    default:
        //Should never get here
        tl_sm_obj_ptr->currentMainState = ERROR;
        break;
    }
}

// EOF
