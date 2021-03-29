`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:43:32 06/18/2019
// Design Name:   CUST_HP_filter
// Module Name:   C:/Users/BuccelliLab/Documents/GitHub/intan_custom_arch/FPGA/New code/Working_project/RHS2000InterfaceXEM6010 release 180814_cust_arch/cust_hp_filter_tb.v
// Project Name:  RHS2000InterfaceXEM6010
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: CUST_HP_filter
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module cust_hp_filter_tb;

	// Inputs
	reg clk;
	reg reset;
	reg [15:0] chan_in_sample;
	reg [7:0] chan_in_num;
	reg chan_in_valid;
	reg chan_out_read;
	reg [15:0] coeff;

	// Outputs
	wire chan_in_read;
	wire [15:0] chan_out_sample;
	wire [7:0] chan_out_num;
	wire chan_out_valid;
	
	// additional stuff
	reg [7:0]count;

	integer f_comp_data_out;
	parameter WIDTH = 640000;
	reg [15:0] RHS_data_stored [0:WIDTH-1]; //WIDTH 16-bits words is the length of RHS_data_flow.txt


	// Instantiate the Unit Under Test (UUT)
	CUST_HP_filter uut (
		.clk(clk), 
		.reset(reset), 
		.chan_in_sample(chan_in_sample), 
		.chan_in_num(chan_in_num), 
		.chan_in_valid(chan_in_valid), 
		.chan_in_read(chan_in_read), 
		.chan_out_sample(chan_out_sample), 
		.chan_out_num(chan_out_num), 
		.chan_out_valid(chan_out_valid), 
		.chan_out_read(chan_out_read), 
		.coeff(coeff)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		chan_in_sample = 0;
		chan_in_num = 0;
		chan_in_valid = 0;
		chan_out_read = 0;
		coeff = 3000;
		count = 0;
		// Wait 100 ns for global reset to finish
		#500;
      reset=0;
		// Add stimulus here
		$readmemb("data_for_filter.txt", RHS_data_stored); // read data from txt in binary format
		f_comp_data_out = $fopen("filter_output.txt","w");
	end
   
	always begin
		#20 
		clk =  ! clk;
		if (reset==0) begin
			if (clk==1) begin
				if (count>159) begin
					count = 0;
				end
					chan_in_sample = RHS_data_stored[{count, 1'b1}];
					chan_in_num = count;
					chan_in_valid=1;
					chan_out_read=1;
					count = count + 1;
			end
		end
	end
	
endmodule

