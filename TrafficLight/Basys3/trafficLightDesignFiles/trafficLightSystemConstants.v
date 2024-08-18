//==============================================================================
//  File Description:
//      Constants file used in the Traffic Light System project describing system level parameters
//==============================================================================

`ifndef _TRAFFIC_LIGHT_SYSTEM_CONSTANTS_H
`define _TRAFFIC_LIGHT_SYSTEM_CONSTANTS_H

parameter real CLK_FREQ = 10e+6;   // [Hz] - digital clock frequency 
parameter real CLK_PERIOD = 1.0 / CLK_FREQ; // Clock period (Seconds)
parameter integer STATE_TIMER_BITS = 29;   // [] - max time is 53.68 sec (10MHz clk)

parameter real TRAFFIC_LIGHT_SYSTEM_INIT_TIME_SEC = 5; // [sec] - dwell time in INIT state 

// Define timing for traffic lights system during normal mode and flashing mode (in seconds)
parameter real NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_TIME_SEC = 30; // [sec] - dwell time in green light state for both primary and secondary roads (normal mode)
parameter real NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_TIME_SEC = 3; // [sec] - dwell time in yellow light state for both primary and secondary roads (normal mode) 
parameter real NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC = 3;  // [sec] - dwell time in red light state for both primary and secondary roads  (normal mode)

parameter real FLASHING_MODE_MINIMUM_BOTH_RED_TIME_SEC = 10; // [sec] - dwell time in init state in flashing mode
parameter real FLASHING_MODE_MINIMUM_BOTH_OFF_TIME_SEC = 3; // [sec] - dwell time in both roads off in flashing mode
parameter real FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_TIME_SEC = 3; // [sec] - dwell time when primary road flashes yellow secondary road flashes red

// Determine the counter threshold for each state
parameter [STATE_TIMER_BITS-1:0] TRAFFIC_LIGHT_SYSTEM_INIT_COUNT = (TRAFFIC_LIGHT_SYSTEM_INIT_TIME_SEC/CLK_PERIOD) -1;   // count corresponding to time spend in INIT state

parameter [STATE_TIMER_BITS-1:0] NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT = (NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time
parameter [STATE_TIMER_BITS-1:0] NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT = (NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time
parameter [STATE_TIMER_BITS-1:0] NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT = (NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time

parameter [STATE_TIMER_BITS-1:0] FLASHING_MODE_MINIMUM_BOTH_RED_COUNT =  (FLASHING_MODE_MINIMUM_BOTH_RED_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time
parameter [STATE_TIMER_BITS-1:0] FLASHING_MODE_MINIMUM_BOTH_OFF_COUNT =  (FLASHING_MODE_MINIMUM_BOTH_OFF_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time
parameter [STATE_TIMER_BITS-1:0] FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT =  (FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_TIME_SEC/CLK_PERIOD) - 1; // [] - count corresponding to desired time

`endif // _TRAFFIC_LIGHT_SYSTEM_CONSTANTS_H