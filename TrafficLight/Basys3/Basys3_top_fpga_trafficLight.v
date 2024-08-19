//==============================================================================
//  File Description:
// This module interfaces with the BASYS3 board and the traffic light system

//==============================================================================


`default_nettype none

module top_BASYS3(
	input wire [0:0] sw,
	input wire clk_100MHz,
	input wire btnC,
	input wire btnU,
	//input wire btnL,
	//input wire btnR,
	//input wire btnD,
	
	output wire [15:0] led
	//output wire [3:0] an,
	//output wire [6:0] seg
	);
	
   wire systemReset_n = !btnC;
	
	wire clk_10MHz,locked, secondaryRoadCarDetected;
   
   assign secondaryRoadCarDetected = sw[0];

   
   clk_wiz_0  clk_instant(
  // Clock out ports
  .clk_10MHz(clk_10MHz),
  // Status and control signals
  .reset(),
  .locked(locked),
 // Clock in ports
  .clk_in1(clk_100MHz)
   );
	
	trafficLight MUT(
	.clk(clk_10MHz),   // 10MHz clock used by CC active charging
   .reset_n(systemReset_n),    // system reset (active-low)
   //.fault(btnU),    // system fault (acitve-high)
   .secondaryRoadSensor(secondaryRoadCarDetected),   // sensor indicating if vehicle is waiting on secondary road

   .primaryRoadLight_RYG(led[2:0]),   // output signals for the primary road lights
   .secondaryRoadLight_RYG(led[15:13])   // output signals for the secondary road lights
	);
	
	
	assign led[4] = secondaryRoadCarDetected;
	assign led[5] = 1'b1;
	assign led[6] = 1'b0;


endmodule


