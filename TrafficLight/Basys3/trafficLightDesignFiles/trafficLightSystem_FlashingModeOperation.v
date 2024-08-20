//==============================================================================
//  File Description:
// This module implements the traffic light sub-state machine while operating in flashing mode

//==============================================================================


`default_nettype none

`include "./trafficLightSystemConstants.v"	

module  trafficLight_flashingModeSubSm   (  
   input wire clk,   // 10MHz clock used by CC active charging
   input wire reset_n,    // system reset (active-low)
   input wire flashingModeSsmIdle,   // supervisory controller pushing flashing mode substate machine to remain in idle mode

   output reg [2:0] primaryRoadLight_RYB_flashingMdSM,   // output signals for the primary road lights
   output reg [2:0] secondaryRoadLight_RYB_flashingMdSM   // output signals for the secondary road lights
	);
      
   
   //----------------------------------------------------------------------------
	// Traffic Light Controller Flashing Mode SubState-Machine Parameters
	//----------------------------------------------------------------------------

	localparam integer FLASHING_MODE_SSM_IDLE_STATE  = 2'h0;  // not in flashing operating mode, do nothing
	localparam integer FLASHING_MODE_SSM_ALL_LANES_RED_STATE = 2'h1;	// both roads in red
	localparam integer FLASHING_MODE_SSM_PRIMARY_ROAD_YELLOW_SECONDARY_ROAD_RED_STATE = 2'h2;	// primary road yellow and secondary road red
   localparam integer FLASHING_MODE_SSM_PRIMARY_ROAD_SECONDARY_ROAD_BOTH_OFF_STATE	= 2'h3;	// both primary and secondary roads off

	
   // SubState-machine in the traffic light system when operating in flashing mode
	reg [1:0] trafficLight_flashingModeSubStateMachine;   // state machine variable
    
   reg [STATE_TIMER_BITS-1:0] flashingModesubStateTime;
   wire flashingModeSsmTimerDone = ~|flashingModesubStateTime; 
       
   always @(negedge reset_n, posedge clk) begin
      if(!reset_n) begin // negedge of reset_n      
         // Reset to the idle state
         flashingModesubStateTime <= FLASHING_MODE_MINIMUM_BOTH_RED_COUNT;
         trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_IDLE_STATE;
         
         // Assign All output lights (keep both roads off)
         primaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
         secondaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
         
      end else begin   // posedge of clk
         // Decrement the timers/counters by default unless its at 0
         if (!flashingModeSsmTimerDone) begin
            flashingModesubStateTime <= flashingModesubStateTime - 1'b1;
         end
         
         if(flashingModeSsmIdle) begin
            trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_IDLE_STATE;  // if main state machine is not in flashing mode keep sus-state machine in idle
            flashingModesubStateTime <= FLASHING_MODE_MINIMUM_BOTH_RED_COUNT;
         end
         
         // Main state machine implementation
         case (trafficLight_flashingModeSubStateMachine)
            FLASHING_MODE_SSM_IDLE_STATE : 
            begin
               // Assign All output lights (keep both roads off)
               primaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
               
               // Check For Transition
               if(!flashingModeSsmIdle) begin
                  // Main state machine has transitioned to flashing Operating Mode, transition to 
                  trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_ALL_LANES_RED_STATE;
                  flashingModesubStateTime <= FLASHING_MODE_MINIMUM_BOTH_RED_COUNT;
               end 
            end          
            
            FLASHING_MODE_SSM_ALL_LANES_RED_STATE:
            begin
                // Assign All output lights (keep both roads off)
               primaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;

               if(flashingModeSsmTimerDone) begin
                  trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_PRIMARY_ROAD_YELLOW_SECONDARY_ROAD_RED_STATE;
                  flashingModesubStateTime <= FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT;  //reset the timer for FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT 
               end
            end
            
            FLASHING_MODE_SSM_PRIMARY_ROAD_YELLOW_SECONDARY_ROAD_RED_STATE:
            begin
                // Assign light outputs              
               primaryRoadLight_RYB_flashingMdSM <= YELLOW_LIGHT;
               secondaryRoadLight_RYB_flashingMdSM <= RED_LIGHT;
               
               //Check for transition:
               if(flashingModeSsmTimerDone) begin              
                  // transition from green to yellow on the primary road once timer is done and the sensor detects there is a car waiting, set the next road to be serviced as secondary
                  trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_PRIMARY_ROAD_SECONDARY_ROAD_BOTH_OFF_STATE;
                  flashingModesubStateTime <= FLASHING_MODE_MINIMUM_BOTH_OFF_COUNT; //reset the timer for FLASHING_MODE_SSM_PRIMARY_ROAD_SECONDARY_ROAD_BOTH_OFF_STATE
               end
            end
            
            FLASHING_MODE_SSM_PRIMARY_ROAD_SECONDARY_ROAD_BOTH_OFF_STATE:
            begin
                // Assign light outputs   
               primaryRoadLight_RYB_flashingMdSM <= ALL_ROADS_OFF;
               secondaryRoadLight_RYB_flashingMdSM <= ALL_ROADS_OFF;
               
               //Check for transition:
               if(flashingModeSsmTimerDone) begin  
                  // transition from yellow to both lanes red
                  trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_PRIMARY_ROAD_YELLOW_SECONDARY_ROAD_RED_STATE;
                  flashingModesubStateTime <= FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT; //reset the timer for FLASHING_MODE_MINIMUM_PRI_YELLOW_SEC_RED_COUNT
               end                  
            end
            
            default: trafficLight_flashingModeSubStateMachine <= FLASHING_MODE_SSM_IDLE_STATE;
         endcase
      end
      
   end
   
endmodule