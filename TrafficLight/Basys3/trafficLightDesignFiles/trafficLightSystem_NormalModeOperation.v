//==============================================================================
//  File Description:
// This module implements the traffic light sub-state machine while operating in normal mode

//==============================================================================


`default_nettype none

`include "./trafficLightSystemConstants.v"	

module  trafficLight_normModeSubSm   (  
   input wire clk,   // 10MHz clock used by CC active charging
   input wire reset_n,    // system reset (active-low)
   input wire secondaryRoadSensor,   // sensor indicating if vehicle is waiting on secondary road
   input wire normalModeSsmIdle,   // supervisory controller pushing normal mode substate machine to remain in idle mode

   output reg [2:0] primaryRoadLight_RYB_normMdSM,   // output signals for the primary road lights
   output reg [2:0] secondaryRoadLight_RYB_normMdSM   // output signals for the secondary road lights
	);
      
   
   //----------------------------------------------------------------------------
	// Traffic Light Controller Normal Mode SubState-Machine Parameters
	//----------------------------------------------------------------------------

	
	localparam integer NORMAL_MODE_SSM_IDLE_STATE  = 3'h0;  // not in normal operating mode, do nothing
	localparam integer NORMAL_MODE_SSM_ALL_LANES_RED_STATE = 3'h1;	// both roads in red
	localparam integer NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE	= 3'h2;	// primary road green
	localparam integer NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE = 3'h3;	// primary road yellow
   localparam integer NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE	= 3'h4;	// secondary road green
	localparam integer NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE = 3'h5;	// secondary road yellow
	
   // SubState-machine in the traffic light system when operating in Normal mode
	reg [2:0] trafficLight_NormalModeSubStateMachine;   // state machine variable
   
   // Variable and paremters to indicate whether next road to be serviced is primary or secondary
   reg roadForNextGrnLight;
   localparam integer ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY  = 1'h0;  // service the primary road next
	localparam integer ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY = 1'h1;	// service the secondary road next
   
   reg [STATE_TIMER_BITS-1:0] normalModesubStateTime;
   wire normalModeSsmTimerDone = ~|normalModesubStateTime; 
       
   always @(negedge reset_n, posedge clk) begin
      if(!reset_n) begin // negedge of reset_n      
         // Reset to the idle state
         normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT;
         trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;
         roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
         
         // Assign All output lights (keep both roads off)
         primaryRoadLight_RYB_normMdSM <= RED_LIGHT;
         secondaryRoadLight_RYB_normMdSM <= RED_LIGHT;
         
      end else begin   // posedge of clk
         // Decrement the timers/counters by default unless its at 0
         if (!normalModeSsmTimerDone) begin
            normalModesubStateTime <= normalModesubStateTime - 1'b1;
         end
         
         if(normalModeSsmIdle) begin
            trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;  // if main state machine is not in normal mode keep sus-state machine in idle
            normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT;
         end
         
         // Main state machine implementation
         case (trafficLight_NormalModeSubStateMachine)
            NORMAL_MODE_SSM_IDLE_STATE : 
            begin
               // Assign All output lights (keep both roads off)
               primaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               
               // Check For Transition
               if(!normalModeSsmIdle) begin
                  // Main state machine has transitioned to Normal Operating Mode, transition to 
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT;
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
                  

               end 
            end          
            
            NORMAL_MODE_SSM_ALL_LANES_RED_STATE:
            begin
                // Assign All output lights (keep both roads off)
               
               primaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= RED_LIGHT;

               if(normalModeSsmTimerDone) begin
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT; //reset the timer for green light condition
                  //trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_SELECT_ROAD_STATE;
                  if (roadForNextGrnLight == ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY) begin 
                     // transition from red to green on the primary road once timer is done and if next road to service is primary
                     trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE;
                  end else begin // (roadForNextGrnLight == ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY)
                      // transition from red to green on the secondary road once timer is done and if next road to service is secondary
                     trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE;
                  end
                  
               end
            end
            
            NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE:
            begin
                // Assign light outputs              
               primaryRoadLight_RYB_normMdSM <= GREEN_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               
               //Check for transition:
               if(normalModeSsmTimerDone && secondaryRoadSensor) begin              
                  // transition from green to yellow on the primary road once timer is done and the sensor detects there is a car waiting, set the next road to be serviced as secondary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT; //reset the timer for yelow light condition
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY;
               end
            end
            
            NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE:
            begin
                // Assign light outputs   
               primaryRoadLight_RYB_normMdSM <= YELLOW_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               
               //Check for transition:
               if(normalModeSsmTimerDone) begin  
                  // transition from yellow to both lanes red
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT; //reset the timer for both lates red light condition 
               end                  
            end
            
            NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE:
            begin
                // Assign light outputs   
               primaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= GREEN_LIGHT;
               
               //Check for transition:               
               if(normalModeSsmTimerDone) begin              
                   // transition from freen to yellow on the primary road once timer is done, set the next road to be serviced as primary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT; //reset the timer for yellow light condition
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
               end
            end
            
            NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE:
            begin
                // Assign light outputs   
               primaryRoadLight_RYB_normMdSM <= RED_LIGHT;
               secondaryRoadLight_RYB_normMdSM <= YELLOW_LIGHT;
               
               //Check for transition:               
               if(normalModeSsmTimerDone) begin  
                  // transition from yellow to both lanes red
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_COUNT; //reset the timer for both lates red light condition 
               end             
            end
            
            default: trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;
         endcase
      end
      
   end
   
endmodule