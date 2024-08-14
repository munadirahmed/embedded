module  counter #(
	COUNTER_WIDTH = 8
	)(
	input wire clk,
	input wire reset,
	output reg [COUNTER_WIDTH-1:0] counter,
	output reg flag);
	
	localparam integer max_count = 3;
	
	always @(posedge reset, posedge clk) begin
		if(reset) begin
			counter <= {COUNTER_WIDTH{1'b0}};
			flag<=0;
		end else begin
			if(counter<max_count) begin
			  counter <= counter + 1;
			  if(counter == max_count) begin
			     flag <=1;
			  end
			  else begin
			     flag<=0;
			  end
			  
			end else begin
			     counter <=0;
			end
		end
	end

endmodule