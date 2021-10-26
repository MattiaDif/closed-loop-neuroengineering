`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 	   Cortical Plasticity Lab
// 
// Design Name:    RHD2000 Rhythm Window Discriminator Upgrade
// Module Name:    Max_8x16
// Project Name:   Opal Kelly FPGA/USB RHD2000 Interface Upgrade
// Target Devices: 
// Tool versions: 
// Description:    Gets the max. value of 8 16-bit inputs.
//				   There is almost certainly a better way. I'm sorry.
//
// Dependencies: 
//
// Revision:       1.0 (01 January 2018)
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module Max_8x16 
	(
		input wire  [15:0]		in1,
		input wire  [15:0]		in2,
		input wire  [15:0]		in3,
		input wire  [15:0]		in4,
		input wire  [15:0]		in5,
		input wire  [15:0]		in6,
		input wire  [15:0]		in7,
		input wire  [15:0]		in8,
		output wire	[15:0]		out
   );
	
	wire [15:0] tree1_1, tree1_2, tree1_3, tree1_4;
	wire [15:0] tree2_1, tree2_2;
   
    // Like a tournament bracket or something

	assign tree1_1 = (in1 >= in2) ? in1 : in2;
	assign tree1_2 = (in3 >= in4) ? in3 : in4;
	assign tree1_3 = (in5 >= in6) ? in5 : in6;
	assign tree1_4 = (in7 >= in8) ? in7 : in8;
	
	assign tree2_1 = (tree1_1 >= tree1_2) ? tree1_1 : tree1_2;
	assign tree2_2 = (tree1_3 >= tree1_4) ? tree1_3 : tree1_4;
   
	assign out = (tree2_1 >= tree2_2) ? tree2_1 : tree2_2;
endmodule
