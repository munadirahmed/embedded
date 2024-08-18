`timescale 1ns/100ps

module trafficLight_tb();

    // Clock and reset signals
    reg clk_10MHz;
    reg systemReset_n;
    reg secondaryRoadSensor;

    // Output signals for the traffic lights
    wire [2:0] primaryRoadLight_RYG;
    wire [2:0] secondaryRoadLight_RYG;

    // Instantiate the MUT (Module Under Test)
    trafficLight MUT (
        .clk(clk_10MHz),                      // 10MHz clock
        .reset_n(systemReset_n),              // system reset (active-low)
        //.secondaryRoadSensor(secondaryRoadSensor), // sensor indicating vehicle presence on secondary road
        .primaryRoadLight_RYG(primaryRoadLight_RYG),  // primary road light output
        .secondaryRoadLight_RYG(secondaryRoadLight_RYG) // secondary road light output
    );

    // Clock generation: 10 MHz clock (period = 100 ns)
    initial begin
        clk_10MHz = 0;
        forever #50 clk_10MHz = ~clk_10MHz;  // Toggle clock every 50 ns
    end

    // Reset and stimulus
    initial begin
        // Initialize signals
        systemReset_n = 1'b0;  // Assert reset (active-low)
        secondaryRoadSensor = 1'b0;  // No vehicle on secondary road initially

        // Apply reset for a short period
        #100;
        systemReset_n = 1'b1;  // Deassert reset

        // Monitor outputs and simulate sensor activity
        $monitor("Time: %0t | Reset: %b | Secondary Road Sensor: %b | Primary Light: %b | Secondary Light: %b", 
                 $time, systemReset_n, secondaryRoadSensor, primaryRoadLight_RYG, secondaryRoadLight_RYG);

        // Simulate vehicle on secondary road after 500 ns
        #500;
        secondaryRoadSensor = 1'b1;  

        // Simulate no vehicle after 1000 ns
        #1000;
        secondaryRoadSensor = 1'b0;

        // Let the simulation run for a while to observe state transitions
        #2000;

        // End simulation
        $finish;
    end
endmodule
