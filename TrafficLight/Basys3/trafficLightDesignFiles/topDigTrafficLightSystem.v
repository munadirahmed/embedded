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

   output wire [2:0] primaryRoadLight_RYG,   // output signals for the primary road lights
   output wire [2:0] secondaryRoadLight_RYG   // output signals for the secondary road lights
	);

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
      end else begin   // posedge of clk
         // Decrement the timers/counters by default unless its at 0
         if (!mainSmTimerDone) begin
            mainSmStateTime <= mainSmStateTime - 1'b1;
         end
         
         // Main state machine implementation
         case (trafficLight_mainStateMachine)
            MAIN_SM_INIT_STATE : 
            begin
               //initial state move to normal state once timer is done and there is no fault (active high)
               if(mainSmTimerDone && !fault) begin
                  trafficLight_mainStateMachine <= MAIN_SM_NORMAL_STATE;
               end else if (mainSmTimerDone && fault) begin
                  //TODO: trafficLight_mainStateMachine <= MAIN_SM_FLASHING_STATE;
               end else begin
                  // do nothing
               end
            end
            
            MAIN_SM_NORMAL_STATE:
            begin
            
            end
            
            MAIN_SM_FLASHING_STATE:
            begin
            
            end
            
            MAIN_SM_ERROR_STATE:
            begin
            
            end
            default:  trafficLight_mainStateMachine <= MAIN_SM_INIT_STATE;
         endcase
      end
      
   end
   
   //----------------------------------------------------------------------------
	// Traffic Light Controller Normal Mode SubState-Machine Implementation
	//----------------------------------------------------------------------------
      
	// SubState-machine in the traffic light system when operating in Normal mode
	reg [2:0] trafficLight_NormalModeSubStateMachine;   // state machine variable
   
   // Variable and paremters to indicate whether next road to be serviced is primary or secondary
   reg roadForNextGrnLight;
   localparam integer ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY  = 1'h0;  // service the primary road next
	localparam integer ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY = 1'h1;	// service the secondary road next
   
   reg [STATE_TIMER_BITS-1:0] normalModesubStateTime;
   wire normalModeSsmTimerDone = ~|normalModesubStateTime; 
   
   wire normalModeSsmIdle = (trafficLight_mainStateMachine != MAIN_SM_NORMAL_STATE);  // keep sub-state-machine in idle unless main SM is in normal mode
   
   always @(negedge reset_n, posedge clk) begin
      if(!reset_n) begin // negedge of reset_n      
         // Reset to the idle state
         normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC;
         trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;
         roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
      end else begin   // posedge of clk
         // Decrement the timers/counters by default unless its at 0
         if (!normalModeSsmTimerDone) begin
            normalModesubStateTime <= normalModesubStateTime - 1'b1;
         end
         
         if(normalModeSsmIdle) begin
            trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;  // if main state machine is not in normal mode keep sus-state machine in idle
            normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC;
         end
         
         // Main state machine implementation
         case (trafficLight_NormalModeSubStateMachine)
            NORMAL_MODE_SSM_IDLE_STATE : 
            begin
               if(!normalModeSsmIdle) begin
                  // Main state machine has transitioned to Normal Operating Mode, transition to 
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC;
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
               end 
            end
            
            NORMAL_MODE_SSM_ALL_LANES_RED_STATE:
            begin
               if(normalModeSsmTimerDone && (roadForNextGrnLight == ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY)) begin
                  // transition from red to green on the primary road once timer is done and if next road to service is primary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT; //reset the timer for green light condition
               end else if(normalModeSsmTimerDone && (roadForNextGrnLight == ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY)) begin
                   // transition from red to green on the primary road once timer is done and if next road to service is secondary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_GREEN_LIGHT_SIGNAL_COUNT; //reset the timer for green light condition
               end
            end
            
            NORMAL_MODE_SSM_PRIMARY_ROAD_GREEN_STATE:
            begin
               if(normalModeSsmTimerDone) begin              
                  // transition from freen to yellow on the primary road once timer is done, set the next road to be serviced as secondary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT; //reset the timer for yelow light condition
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_SECONDARY;
               end
            end
            
            NORMAL_MODE_SSM_PRIMARY_ROAD_YELLOW_STATE:
            begin
               if(normalModeSsmTimerDone) begin  
                  // transition from yellow to both lanes red
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC; //reset the timer for both lates red light condition 
               end                  
            end
            
            NORMAL_MODE_SSM_SECONDARY_ROAD_GREEN_STATE:
            begin
               if(normalModeSsmTimerDone) begin              
                   // transition from freen to yellow on the primary road once timer is done, set the next road to be serviced as primary
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_YELLOW_LIGHT_SIGNAL_COUNT; //reset the timer for yellow light condition
                  roadForNextGrnLight <= ROAD_FOR_NEXT_GREEN_LIGHT_PRIMARY;
               end
            end
            
            NORMAL_MODE_SSM_SECONDARY_ROAD_YELLOW_STATE:
            begin
               if(normalModeSsmTimerDone) begin  
                  // transition from yellow to both lanes red
                  trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_ALL_LANES_RED_STATE;
                  normalModesubStateTime <= NORMAL_MODE_MINIMUM_BOTH_LANE_RED_TIME_SEC; //reset the timer for both lates red light condition 
               end             
            end
            
            default: trafficLight_NormalModeSubStateMachine <= NORMAL_MODE_SSM_IDLE_STATE;
         endcase
      end
      
   end

endmodule