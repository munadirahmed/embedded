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
#include "bsp.h"

/***************  Define data types and structures ********************/

typedef enum {
    INIT= 0U,
    NORMAL,
    FLASHING,
    ERROR
} trafficLight_MainStates_t;

typedef enum {
    NORMAL_MODE_IDLE= 0U,
    NORMAL_MODE_ALL_LANES_RED,
    NORMAL_MODE_PRIMARY_ROAD_GREEN,
    NORMAL_MODE_PRIMARY_ROAD_YELLOW,
    NORMAL_MODE_SECONDARY_ROAD_GREEN,
    NORMAL_MODE_SECONDARY_ROAD_YELLOW
} trafficLight_NormalModeSubStates_t;

typedef enum {
    FLASHING_MODE_IDLE= 0U,
    FLASHING_MODE_ALL_LANES_RED,
    FLASHING_MODE_PRIMARY_ROAD_YELLOW_SEC_ROAD_RED,
    FLASHING_MODE_ALL_LANES_OFF
} trafficLight_FlashingModeSubStates_t;

typedef enum {
    PRIMARY_ROAD=0U,
    SECONDARY_ROAD
}trafficLight_RoadForNextGreenLight;

typedef enum {
    NO_VEHICLE_PRESENT=0U,
    VEHICLE_PRESENT
}trafficLight_VehicleStatusOnSecRoad_t;

typedef enum {
    NO_FAULT_DETECTED=0U,
    FAULT_DETECTED
}trafficLight_SystemFaultStatus_t;

typedef struct {
    trafficLight_MainStates_t currentMainState;   // holds the current main state for the traffic light
    trafficLight_NormalModeSubStates_t currentNrmlModeSubState;  // holds the current substate when traffic light operating in normal operating mode
    trafficLight_FlashingModeSubStates_t currentFlshingModeSubState;  // holds the current substate when traffic light operating in flashing operating mode
    uint16_t timer; // Timer for managing state transitions
    trafficLight_RoadForNextGreenLight roadForNextGrnLight;
    trafficLight_Color_t primaryRoadTrafficLight;
    trafficLight_Color_t secondaryRoadTrafficLight;
    trafficLight_VehicleStatusOnSecRoad_t vehicleStatusOnSecRoad;
    trafficLight_SystemFaultStatus_t systemFaultStatus;
} trafficLightClass_t;


/***************  Define constants ********************/

/***************  Declare static global variables ********************/

static trafficLightClass_t trafficLightObject;

/*************** Define local function prototypes ********************/

// Main state machine helper functions

// All main state machine entry actions for each state
static void onStateEnterTrafficLightMainSm_Init(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightMainSm_Normal(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightMainSm_Flashing(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
//static void onStateEnterTrafficLightMainSm_Error(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions

static inline boolean checkStateTransitionOutCondition_Init(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkStateTransitionOutCondition_Normal(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkStateTransitionOutCondition_Flashing(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkStateTransitionOutCondition_Error(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runStateTrafficLightSm_Init(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runStateTrafficLightSm_Normal(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runStateTrafficLightSm_Flashing(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runStateTrafficLightSm_Error(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runTrafficLightStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

// All Normal sub-state machine helper functions

static void onStateEnterTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions
static void onStateEnterTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);   // Not inlined as could be called from multiple transitions

static inline void runTrafficLightNormalModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

static inline boolean checkSubStateTransitionOutCondition_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkSubStateTransitionOutCondition_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkSubStateTransitionOutCondition_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkSubStateTransitionOutCondition_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkSubStateTransitionOutCondition_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runSubStateTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr);

// All Flashing sub-state machine helper functions

static void onStateEnterTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr);
static void onStateEnterTrafficLightFlashingSubSm_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runTrafficLightFlashingModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr);

static inline boolean checkFlashingSubStateTransitionOutCondition_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkFlashingSubStateTransitionOutCondition_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline boolean checkFlashingSubStateTransitionOutCondition_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr);

static inline void runSubStateTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr);
static inline void runSubStateTrafficLightFlashingSubSm_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr);

// Vehicle sensor helper functions
static inline void updateSecondaryRoadVehicleStatus(trafficLightClass_t *tl_sm_obj_ptr);
static inline void updateSystemFaultDetectionStatus(trafficLightClass_t *tl_sm_obj_ptr);

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
    trafficLightObject.roadForNextGrnLight = PRIMARY_ROAD;
    trafficLightObject.vehicleStatusOnSecRoad = NO_VEHICLE_PRESENT;
    onStateEnterTrafficLightMainSm_Init(&trafficLightObject);
}

/**
 * Main application run program
 *
 * @param - N/A
 *
 * @return - N/A
 */

void runApplication(void)
{
    updateSecondaryRoadVehicleStatus(&trafficLightObject);
    updateSystemFaultDetectionStatus(&trafficLightObject);
    runTrafficLightStateMachine(&trafficLightObject);

}


// Local functions

/**
 * Check for transition out condition of main state machine: INIT state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - true/false on whether to transition out of the state or not
 */
static inline boolean checkStateTransitionOutCondition_Init(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if(FAULT_DETECTED == tl_sm_obj_ptr->systemFaultStatus)
    {
        // Fault detected - immediately go to flashing mode operation
        tl_sm_obj_ptr->currentMainState= FLASHING;
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_ALL_LANES_RED;
        onStateEnterTrafficLightMainSm_Flashing(tl_sm_obj_ptr);
        rtn = true;
    }
    else if( (0U == tl_sm_obj_ptr->timer) )
    {
        // Ready to transition - Initialize all parameters
        tl_sm_obj_ptr->currentMainState= NORMAL;
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_ALL_LANES_RED;
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_IDLE;
        onStateEnterTrafficLightMainSm_Normal(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;

}

/**
 * Check for transition out condition of main state machine: NORMAL state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - true/false on whether to transition out of the state or not
 */
static inline boolean checkStateTransitionOutCondition_Normal(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if(FAULT_DETECTED == tl_sm_obj_ptr->systemFaultStatus)
    {
        // Fault detected - immediately go to flashing mode operation
        tl_sm_obj_ptr->currentMainState= FLASHING;
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_ALL_LANES_RED;
        onStateEnterTrafficLightMainSm_Flashing(tl_sm_obj_ptr);
        rtn = true;
    }
    return rtn;
}

/**
 * Check for transition out condition of main state machine: FLASHING state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - true/false on whether to transition out of the state or not
 */
static inline boolean checkStateTransitionOutCondition_Flashing(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if(NO_FAULT_DETECTED == tl_sm_obj_ptr->systemFaultStatus)
    {
        // Fault detected - immediately go to flashing mode operation
        tl_sm_obj_ptr->currentMainState= INIT;
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_IDLE;
        onStateEnterTrafficLightMainSm_Init(tl_sm_obj_ptr);
        rtn = true;
    }
    return rtn;
}

/**
 * Check for transition out condition of main state machine: ERROR state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - true/false on whether to transition out of the state or not
 */
static inline boolean checkStateTransitionOutCondition_Error(trafficLightClass_t *tl_sm_obj_ptr)
{
    //TODO: implement transitions
    return false;
}

/**
 * Tasks to perform while in normal mode sub-state machine: INIT state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runStateTrafficLightSm_Init(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkStateTransitionOutCondition_Init(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in normal mode sub-state machine: NORMAL state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runStateTrafficLightSm_Normal(trafficLightClass_t *tl_sm_obj_ptr)
{
    runTrafficLightNormalModeSubStateMachine(tl_sm_obj_ptr);
    checkStateTransitionOutCondition_Normal(tl_sm_obj_ptr);

}

/**
 * Check for transition out condition of main state machine: FLASHING state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runStateTrafficLightSm_Flashing(trafficLightClass_t *tl_sm_obj_ptr)
{
    runTrafficLightFlashingModeSubStateMachine(tl_sm_obj_ptr);
    checkStateTransitionOutCondition_Flashing(tl_sm_obj_ptr);
}

/**
 * Check for transition out condition of main state machine: ERROR state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runStateTrafficLightSm_Error(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkStateTransitionOutCondition_Error(tl_sm_obj_ptr);
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
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;

    trafficLightObject.roadForNextGrnLight = PRIMARY_ROAD;   // Always start with primary road when entering Normal mode from other modes

    onStateEnterTrafficLightNormalSubSm_AllLanesRed(tl_sm_obj_ptr);  // Initialize parameters for all lanes red
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);

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
   // TODO: finalize implementation
    tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;  // reset flashing mode sub-state machine
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.currentFlshingModeSubState = FLASHING_MODE_ALL_LANES_RED;

    onStateEnterTrafficLightFlashingSubSm_AllLanesRed(tl_sm_obj_ptr);  // Initialize parameters for all lanes red
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}


/**
 * Tasks to perform upon entry of main state machine to ERROR state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
/*static void onStateEnterTrafficLightMainSm_Error(trafficLightClass_t *tl_sm_obj_ptr)
{
// TODO: add handling of error state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}*/

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
    tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_IDLE;  // set the Normal mode sub-state machine to ALL_LANES_RED
    trafficLightObject.currentFlshingModeSubState = FLASHING_MODE_IDLE; // reset flashing mode sub-state machine to IDLE
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}


/**
 * Tasks to perform upon entry of normal mode sub-state machine: ALL LANES RED state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform upon entry of normal mode sub-state machine: Primary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT;  // Update timer to spend minimum time in sub-state

    tl_sm_obj_ptr->roadForNextGrnLight = SECONDARY_ROAD;    // Now that primary road is already serviced switch to secondary road for next time
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_GREEN;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform upon entry of normal mode sub-state machine: Primary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_YELLOW;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
* Tasks to perform upon entry of normal mode sub-state machine: Secondary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT;  // Update timer to spend minimum time in sub-state

    tl_sm_obj_ptr->roadForNextGrnLight = PRIMARY_ROAD;    // Now that secondary road is already serviced switch to secondary road for next time
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_GREEN;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform upon entry of normal mode sub-state machine: Secondary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_YELLOW;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Check for transition out condition of normal mode sub-state machine: ALL LANES RED state
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - true/false on whether to transition out of the state or not
 */
static inline boolean checkSubStateTransitionOutCondition_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer) &&  (PRIMARY_ROAD == tl_sm_obj_ptr->roadForNextGrnLight) )
    {
        // Ready to transition - Set primary road to green and initialize all parameters for state entry
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_PRIMARY_ROAD_GREEN;
        onStateEnterTrafficLightNormalSubSm_PriRoadGrn(tl_sm_obj_ptr);
        rtn = true;
    }else if( (0U == tl_sm_obj_ptr->timer) &&  (SECONDARY_ROAD == tl_sm_obj_ptr->roadForNextGrnLight) )
    {
        // Ready to transition - Set secondary road to green and initialize all parameters for state entry
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_SECONDARY_ROAD_GREEN;
        onStateEnterTrafficLightNormalSubSm_SecRoadGrn(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of normal mode sub-state machine: Primary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkSubStateTransitionOutCondition_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer) && (VEHICLE_PRESENT == tl_sm_obj_ptr->vehicleStatusOnSecRoad) )
    {
        // Ready to transition - Set primary road to yellow and initialize all parameters for state entry
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_PRIMARY_ROAD_YELLOW;
        onStateEnterTrafficLightNormalSubSm_PriRoadYllw(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of normal mode sub-state machine: Primary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkSubStateTransitionOutCondition_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_ALL_LANES_RED;
        onStateEnterTrafficLightNormalSubSm_AllLanesRed(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of normal mode sub-state machine: Secondary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkSubStateTransitionOutCondition_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        // Ready to transition - Set primary road to yellow and initialize all parameters for state entry
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_SECONDARY_ROAD_YELLOW;
        onStateEnterTrafficLightNormalSubSm_SecRoadYllw(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of normal mode sub-state machine: Secondary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkSubStateTransitionOutCondition_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        tl_sm_obj_ptr->currentNrmlModeSubState = NORMAL_MODE_ALL_LANES_RED;
        onStateEnterTrafficLightNormalSubSm_AllLanesRed(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Tasks to perform while in normal mode sub-state machine: All Lanes Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightNormalSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkSubStateTransitionOutCondition_AllLanesRed(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in normal mode sub-state machine: Primary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightNormalSubSm_PriRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkSubStateTransitionOutCondition_PriRoadGrn(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in normal mode sub-state machine: Primary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightNormalSubSm_PriRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkSubStateTransitionOutCondition_PriRoadYllw(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in normal mode sub-state machine: Secondary Road Green
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightNormalSubSm_SecRoadGrn(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkSubStateTransitionOutCondition_SecRoadGrn(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in normal mode sub-state machine: Secondary Road Yellow
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightNormalSubSm_SecRoadYllw(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkSubStateTransitionOutCondition_SecRoadYllw(tl_sm_obj_ptr);
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
    if( 0U == tl_sm_obj_ptr->timer)
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
        runStateTrafficLightSm_Init(tl_sm_obj_ptr);
        break;
    case NORMAL:
        runStateTrafficLightSm_Normal(tl_sm_obj_ptr);
        break;
    case FLASHING:
        runStateTrafficLightSm_Flashing(tl_sm_obj_ptr);
        break;
    case ERROR:
        runStateTrafficLightSm_Error(tl_sm_obj_ptr);
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
        runSubStateTrafficLightNormalSubSm_AllLanesRed(tl_sm_obj_ptr);
        break;
    case NORMAL_MODE_PRIMARY_ROAD_GREEN:
        runSubStateTrafficLightNormalSubSm_PriRoadGrn(tl_sm_obj_ptr);
        break;
    case NORMAL_MODE_PRIMARY_ROAD_YELLOW:
        runSubStateTrafficLightNormalSubSm_PriRoadYllw(tl_sm_obj_ptr);
        break;
    case NORMAL_MODE_SECONDARY_ROAD_GREEN:
        runSubStateTrafficLightNormalSubSm_SecRoadGrn(tl_sm_obj_ptr);
        break;
    case NORMAL_MODE_SECONDARY_ROAD_YELLOW:
        runSubStateTrafficLightNormalSubSm_SecRoadYllw(tl_sm_obj_ptr);
        break;
    default:
        //Should never get here
        tl_sm_obj_ptr->currentMainState = ERROR;
        break;
    }
}


/**
 * Tasks to perform upon entry of flashing mode sub-state machine: All Lanes Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = FLASHING_MODE_MINIMUM_BOTH_RED_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform upon entry of flashing mode sub-state machine: Primary Road Yellow + Secondary Road Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_YELLOW;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_COLOR_RED;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform upon entry of flashing mode sub-state machine: All Lanes Off
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static void onStateEnterTrafficLightFlashingSubSm_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr)
{
    tl_sm_obj_ptr->timer = FLASHING_MODE_MINIMUM_BOTH_OFF_COUNT;  // Update timer to spend minimum time in sub-state
    trafficLightObject.primaryRoadTrafficLight = TRAFFIC_LIGHT_OFF;
    trafficLightObject.secondaryRoadTrafficLight = TRAFFIC_LIGHT_OFF;
    set_digital_out(OM_DIG_CH_PRIMARY_ROAD, trafficLightObject.primaryRoadTrafficLight);
    set_digital_out(OM_DIG_CH_SECONDARY_ROAD, trafficLightObject.secondaryRoadTrafficLight);
}

/**
 * Tasks to perform while in flashing mode sub-state machine: All Lanes Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightFlashingSubSm_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkFlashingSubStateTransitionOutCondition_AllLanesRed(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in flashing mode sub-state machine: Primary Road Yellow + Secondary Road Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkFlashingSubStateTransitionOutCondition_PriRoadYllwSecRoadRed(tl_sm_obj_ptr);
}

/**
 * Tasks to perform while in flashing mode sub-state machine: all lanes off
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runSubStateTrafficLightFlashingSubSm_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr)
{
    checkFlashingSubStateTransitionOutCondition_AllLanesOff(tl_sm_obj_ptr);
}

/**
 * Check for transition out condition of flashing mode sub-state machine: All Lanes Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkFlashingSubStateTransitionOutCondition_AllLanesRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_PRIMARY_ROAD_YELLOW_SEC_ROAD_RED;
        onStateEnterTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of flashing mode sub-state machine: AllLanesOff
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkFlashingSubStateTransitionOutCondition_AllLanesOff(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_PRIMARY_ROAD_YELLOW_SEC_ROAD_RED;
        onStateEnterTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Check for transition out condition of flashing mode sub-state machine: Primary Road Yellow + Secondary Road Red
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline boolean checkFlashingSubStateTransitionOutCondition_PriRoadYllwSecRoadRed(trafficLightClass_t *tl_sm_obj_ptr)
{
    boolean rtn = false;
    if( (0U == tl_sm_obj_ptr->timer)  )
    {
        tl_sm_obj_ptr->currentFlshingModeSubState = FLASHING_MODE_ALL_LANES_OFF;
        onStateEnterTrafficLightFlashingSubSm_AllLanesOff(tl_sm_obj_ptr);
        rtn = true;
    }else
    {
        //do nothing
    }

    return rtn;
}

/**
 * Execution of the traffic light sub-state machine when in flashing operating mode
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object
 *
 * @return - N/A
 */
static inline void runTrafficLightFlashingModeSubStateMachine(trafficLightClass_t *tl_sm_obj_ptr) {
    switch (tl_sm_obj_ptr->currentFlshingModeSubState)
    {
    case FLASHING_MODE_IDLE:
        //Do nothing To lights
        break;
    case FLASHING_MODE_ALL_LANES_RED:
        runSubStateTrafficLightFlashingSubSm_AllLanesRed(tl_sm_obj_ptr);
        break;
    case FLASHING_MODE_PRIMARY_ROAD_YELLOW_SEC_ROAD_RED:
        runSubStateTrafficLightFlashingSubSm_PriRoadYllwSecRoadRed(tl_sm_obj_ptr);
        break;
    case FLASHING_MODE_ALL_LANES_OFF:
        runSubStateTrafficLightFlashingSubSm_AllLanesOff(tl_sm_obj_ptr);
        break;
    default:
        //Should never get here
        tl_sm_obj_ptr->currentMainState = ERROR;
        break;
    }
}

/**
 * Check sensor to see if vehicle is waiting on secondary road or not and updates the object status
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object.
 *
 * @return - N/A
 */
static inline void updateSecondaryRoadVehicleStatus(trafficLightClass_t *tl_sm_obj_ptr)
{
    uint16_t vehicleSensorData = getSensorRawInputValue();

    //SW Hysteresis implementation
    if( (SENSOR_VEHICLE_PRESENT_DIGITAL_TH) < vehicleSensorData )
    {
        // Comparator above hysteretic threshold
        tl_sm_obj_ptr->vehicleStatusOnSecRoad = VEHICLE_PRESENT;
    }else if( (SENSOR_NO_VEHICLE_PRESENT_DIGITAL_TH) > vehicleSensorData)
    {
        // Comparator below hysteretic threshold
        tl_sm_obj_ptr->vehicleStatusOnSecRoad = NO_VEHICLE_PRESENT;
    }else
    {
        //do nothing
    }

    return;

}

trafficLight_SystemFaultStatus_t testFaultStatus = NO_FAULT_DETECTED;

/**
 * Check system sensors to determine if system has faulted (initial implementation uses only ADC channel data - can be expanded for multiple fault sensors)
 *
 * @param *tl_sm_obj_ptr -> pointer to a traffic light state machine object.
 *
 * @return - N/A
 */
static inline void updateSystemFaultDetectionStatus(trafficLightClass_t *tl_sm_obj_ptr)
{
    // TODO: implement fault sensor detection
    tl_sm_obj_ptr->systemFaultStatus =testFaultStatus;

    return;

}

// EOF
