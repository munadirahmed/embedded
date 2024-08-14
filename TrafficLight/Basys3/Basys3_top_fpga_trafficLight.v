//==============================================================================
//  File Description:
// This module interfaces with the BASYS3 board and the traffic light system

//==============================================================================


`default_nettype none

module top_BASYS3(
	//input [15:0] sw,
	input wire clk_100MHz,
	//input wire btnC,
	input wire btnU,
	//input wire btnL,
	//input wire btnR,
	//input wire btnD,
	
	output wire [7:0] led
	//output wire [3:0] an,
	//output wire [6:0] seg
	);
	
	//default
	
	
	
	counter MUT(
	.clk(clk_100MHz),
	.reset(btnU),
	.counter(led[3:0]),
	.flag(led[7])
	);
	defparam MUT.COUNTER_WIDTH = 4;
	
	assign led[4] = 1'b0;
	assign led[5] = 1'b1;
	assign led[6] = 1'b0;
	
	//assign led <= 4'h0000;
	//assign an = 1'h0;
	//assign seg = 7'b0000000;


endmodule


