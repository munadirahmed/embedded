//==============================================================================
//  File Description:
// This module implements the traffic light state machine

//==============================================================================


`default_nettype none

`include "./trafficLightSystemConstants.v"	

module  trafficLight   (  
   input wire clk,   // 10MHz clock used by CC active charging
   input wire reset_n,    // system reset (active-low)
   input wire fault,    // system fault (acitve-low)
   input wire secondaryRoadSensor,   // sensor indicating if vehicle is waiting on secondary road

   output reg [2:0] primaryRoadLight_RYG,   // output signals for the primary road lights
   output reg [2:0] secondaryRoadLight_RYG   // output signals for the secondary road lights
	);

   //----------------------------------------------------------------------------
	// Helper variables for traffic light system
	//----------------------------------------------------------------------------
      
   wire normalMdSsmToMainSmConnection_priRdRed;
   wire normalMdSsmToMainSmConnection_priRdYellow;
   wire normalMdSsmToMainSmConnection_priRdGreen;
   
   wire normalMdSsmToMainSmConnection_secRdRed;
   wire normalMdSsmToMainSmConnection_secRdYellow;
   wire normalMdSsmToMainSmConnection_secRdGreen;
   
   wire flashingMdSsmToMainSmConnection_priRdRed;
   wire flashingMdSsmToMainSmConnection_priRdYellow;
   wire flashingMdSsmToMainSmConnection_priRdGreen;
   
   wire flashingMdSsmToMainSmConnection_secRdRed;
   wire flashingMdSsmToMainSmConnection_secRdYellow;
   wire flashingMdSsmToMainSmConnection_secRdGreen;

	//----------------------------------------------------------------------------
	// Traffic Light Controller State-Machine Parameters
	//----------------------------------------------------------------------------

	
	localparam integer MAIN_SM_INIT_STATE  = 2'h0;  // 
	localparam integer MAIN_SM_NORMAL_STATE = 2'h1;	// normal operating mode of traffic light system
	localparam integer MAIN_SM_FLASHING_STATE	= 2'h2;	// flashing operating mode of traffic light system
	localparam integer MAIN_SM_ERROR_STATE = 2'h3;	// normal operating mode of traffic light system
	
	localparam integer NORMAL_MODE_SSM_IDLE_STATE  = 3'h0;  // not in normal operating mode, do nothing
	localparam integer NORMAL_MODE_SSM_ALL_LANES_RED_STATE = 3'h1;	// both roads in red
	localparam integer NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE	= 3'h2;	// primary road green
	localparam integer NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE = 3'h3;	// primary road yellow
   localparam integer NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE	= 3'h4;	// secondary road green
	localparam integer NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE = 3'h5;	// secondary road yellow
   
   // TODO: SubState-machine in the traffic light system when operating in flashing and error mode

	//----------------------------------------------------------------------------
	// Traffic Light Controller Main State-Machine Implementation
	//----------------------------------------------------------------------------

	// Main State-machine in the traffic light system and states
	reg [1:0] trafficLight_mainStateMachine;   // state machine variable
   
   reg [STATE_TIMER_BITS-1:0] mainSmStateTime;
   
   wire mainSmTimerDone = ~|mainSmStateTime; 
   
   always @(negedge reset_n, posedge clk) begin
      if(!reset_n) begin // negedge of reset_n      
         // Reset to the Init state
         mainSmStateTime <= TRAFFIC_LIGHT_SYSTEM_INIT_COUNT;
         trafficLight_mainStateMachine <= MAIN_SM_INIT_STATE;
         
         // Assign All output lights (keep both roads off)
         primaryRoadLight_RYG <= RED_LIGHT;
         secondaryRoadLight_RYG <= RED_LIGHT;
         
      end else begin   // posedge of clk
         // Decrement the timers/counters by default unless its at 0
         if (!mainSmTimerDone) begin
            mainSmStateTime <= mainSmStateTime - 1'b1;
         end
         
         // Highest presidence - if fault is detected ensure SM operating in flashing mode
         if(fault) begin
            trafficLight_mainStateMachine <= MAIN_SM_FLASHING_STATE;
         end
         
         // Main state machine implementation
         case (trafficLight_mainStateMachine)
            MAIN_SM_INIT_STATE : 
            begin
               // Assign All output lights (keep both roads off)
               primaryRoadLight_RYG <= RED_LIGHT;
               secondaryRoadLight_RYG <= RED_LIGHT;
               
               //initial state move to normal state once timer is done and there is no fault
               if(mainSmTimerDone) begin
                  trafficLight_mainStateMachine <= MAIN_SM_NORMAL_STATE;
               end
            end
            
            MAIN_SM_NORMAL_STATE:
            begin
               // In normal mode lights driven by normal mode sub-state machine
               primaryRoadLight_RYG[RED_LIGHT_IDX] = normalMdSsmToMainSmConnection_priRdRed;
               primaryRoadLight_RYG[YELLOW_LIGHT_IDX] = normalMdSsmToMainSmConnection_priRdYellow;
               primaryRoadLight_RYG[GREEN_LIGHT_IDX] = normalMdSsmToMainSmConnection_priRdGreen;
               
               secondaryRoadLight_RYG[RED_LIGHT_IDX] = normalMdSsmToMainSmConnection_secRdRed;
               secondaryRoadLight_RYG[YELLOW_LIGHT_IDX] = normalMdSsmToMainSmConnection_secRdYellow;
               secondaryRoadLight_RYG[GREEN_LIGHT_IDX] = normalMdSsmToMainSmConnection_secRdGreen;
            end
            
            MAIN_SM_FLASHING_STATE:
            begin
               if(!fault) begin
                  // Once fault is removed go back into normal opration via INIT state
                  trafficLight_mainStateMachine <= MAIN_SM_INIT_STATE;
               end
               // In flashing mode lights driven by flashing mode sub-state machine
               primaryRoadLight_RYG[RED_LIGHT_IDX] = flashingMdSsmToMainSmConnection_priRdRed;
               primaryRoadLight_RYG[YELLOW_LIGHT_IDX] = flashingMdSsmToMainSmConnection_priRdYellow;
               primaryRoadLight_RYG[GREEN_LIGHT_IDX] = flashingMdSsmToMainSmConnection_priRdGreen;
               
               secondaryRoadLight_RYG[RED_LIGHT_IDX] = flashingMdSsmToMainSmConnection_secRdRed;
               secondaryRoadLight_RYG[YELLOW_LIGHT_IDX] = flashingMdSsmToMainSmConnection_secRdYellow;
               secondaryRoadLight_RYG[GREEN_LIGHT_IDX] = flashingMdSsmToMainSmConnection_secRdGreen;
            end
            
            MAIN_SM_ERROR_STATE:
            begin
               // TODO: Implementat
            end
            default:  trafficLight_mainStateMachine <= MAIN_SM_INIT_STATE;
         endcase
      end
      
   end
   
   //----------------------------------------------------------------------------
	// Traffic Light Controller Normal Mode SubState-Machine Implementation
	//----------------------------------------------------------------------------

   wire normalModeSsmIdle = (trafficLight_mainStateMachine != MAIN_SM_NORMAL_STATE);  // keep sub-state-machine in idle unless main SM is in normal mode
   wire [2:0] primaryRoadLight_RYB_normMdSM;  // primary road outputs for the red-yellow-green lights in normal operating mode
   wire [2:0] secondaryRoadLight_RYB_normMdSM;  // secondary road outputs for the red-yellow-green lights in normal operating mode
      
   trafficLight_normModeSubSm  instanceTrafficLight_normModeSubSm (  
   .clk(clk),   // 10MHz clock used by CC active charging
   .reset_n(reset_n),    // system reset (active-low)
   .secondaryRoadSensor(secondaryRoadSensor),   // sensor indicating if vehicle is waiting on secondary road
   .normalModeSsmIdle(normalModeSsmIdle),   // supervisory controller pushing normal mode substate machine to remain in idle mode

   .primaryRoadLight_RYB_normMdSM(primaryRoadLight_RYB_normMdSM),   // output signals for the primary road lights
   .secondaryRoadLight_RYB_normMdSM(secondaryRoadLight_RYB_normMdSM)   // output signals for the secondary road lights
	);
   
   // Connect the normal mode state machine outputs to the main state machine
   assign normalMdSsmToMainSmConnection_priRdRed = primaryRoadLight_RYB_normMdSM[RED_LIGHT_IDX];
   assign normalMdSsmToMainSmConnection_priRdYellow = primaryRoadLight_RYB_normMdSM[YELLOW_LIGHT_IDX];
   assign normalMdSsmToMainSmConnection_priRdGreen = primaryRoadLight_RYB_normMdSM[GREEN_LIGHT_IDX];
   
   assign normalMdSsmToMainSmConnection_secRdRed = secondaryRoadLight_RYB_normMdSM[RED_LIGHT_IDX];
   assign normalMdSsmToMainSmConnection_secRdYellow = secondaryRoadLight_RYB_normMdSM[YELLOW_LIGHT_IDX];
   assign normalMdSsmToMainSmConnection_secRdGreen = secondaryRoadLight_RYB_normMdSM[GREEN_LIGHT_IDX];
   
   //----------------------------------------------------------------------------
	// Traffic Light Controller Normal Mode SubState-Machine Implementation
	//----------------------------------------------------------------------------

   wire flashingModeSsmIdle = (trafficLight_mainStateMachine != MAIN_SM_FLASHING_STATE);  // keep sub-state-machine in idle unless main SM is in flashing mode
   wire [2:0] primaryRoadLight_RYB_flashingMdSM;  // primary road outputs for the red-yellow-green lights in normal operating mode
   wire [2:0] secondaryRoadLight_RYB_flashingMdSM;  // secondary road outputs for the red-yellow-green lights in normal operating mode
      
   trafficLight_flashingModeSubSm  instanceTrafficLight_flashingModeSubSm (  
   .clk(clk),   // 10MHz clock used by CC active charging
   .reset_n(reset_n),    // system reset (active-low)
   .flashingModeSsmIdle(flashingModeSsmIdle),   // supervisory controller pushing flashing mode substate machine to remain in idle mode

   .primaryRoadLight_RYB_flashingMdSM(primaryRoadLight_RYB_flashingMdSM),   // output signals for the primary road lights
   .secondaryRoadLight_RYB_flashingMdSM(secondaryRoadLight_RYB_flashingMdSM)   // output signals for the secondary road lights
	);
   
   // Connect the normal mode state machine outputs to the main state machine
   assign flashingMdSsmToMainSmConnection_priRdRed = primaryRoadLight_RYB_flashingMdSM[RED_LIGHT_IDX];
   assign flashingMdSsmToMainSmConnection_priRdYellow = primaryRoadLight_RYB_flashingMdSM[YELLOW_LIGHT_IDX];
   assign flashingMdSsmToMainSmConnection_priRdGreen = primaryRoadLight_RYB_flashingMdSM[GREEN_LIGHT_IDX];
   
   assign flashingMdSsmToMainSmConnection_secRdRed = secondaryRoadLight_RYB_flashingMdSM[RED_LIGHT_IDX];
   assign flashingMdSsmToMainSmConnection_secRdYellow = secondaryRoadLight_RYB_flashingMdSM[YELLOW_LIGHT_IDX];
   assign flashingMdSsmToMainSmConnection_secRdGreen = secondaryRoadLight_RYB_flashingMdSM[GREEN_LIGHT_IDX];
   
endmodule