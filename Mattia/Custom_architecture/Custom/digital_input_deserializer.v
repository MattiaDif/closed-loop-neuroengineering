`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		 Intan Technologies, LLC
// 
// Design Name: 	 RHD2000 Rhythm Interface
// Module Name:    digital_input_deserializer 
// Project Name:   Opal Kelly FPGA/USB RHD2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Deserialize digital data from 16-bit shift registers.
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module digital_input_deserializer #(
	parameter ms_wait  	= 99,
   parameter ms_clk1_a 	= 100,
	parameter ms_clk11_a = 140
	)
	(
	input wire			reset,
	input wire			dataclk,
	input wire [31:0]	main_state,
	input wire [5:0]	channel,
	input wire			serial_in,
	input wire			serial_in_exp,
	input wire			TTL_in_direct_1,
	input wire			TTL_in_direct_2,
	output reg			serial_CLK,
	output reg			serial_LOAD,
	output reg [15:0]	TTL_parallel,
	output reg [15:0] TTL_parallel_exp
	);

	reg [15:0] TTL_save, TTL_save_exp;

	always @(posedge dataclk) begin
		if (reset) begin
			serial_LOAD <= 1'b1;
			serial_CLK <= 1'b0;
		end else begin
			case (main_state)
			
				ms_wait: begin
					serial_LOAD <= 1'b1;
					serial_CLK <= 1'b0;
				end
			
				ms_clk1_a: begin
					case (channel)
					
						0: begin
							serial_LOAD <= 1'b0;
							serial_CLK <= 1'b0;
						end

						1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15: begin
							serial_LOAD <= 1'b1;
							serial_CLK <= 1'b1;
						end
						
						default: begin
							serial_LOAD <= 1'b1;
							serial_CLK <= 1'b0;
						end
						
					endcase
				end

				ms_clk11_a: begin
					serial_LOAD <= 1'b1;
					serial_CLK <= 1'b0;
					
					case (channel)

						0: begin
							TTL_save_exp[0] <= TTL_in_direct_1;
							TTL_save_exp[1] <= TTL_in_direct_2;
							TTL_save[15] <= serial_in;
							TTL_save_exp[15] <= serial_in_exp;
						end
						
						1: begin
							TTL_save[14] <= serial_in;
							TTL_save_exp[14] <= serial_in_exp;							
						end
						
						2: begin
							TTL_save[13] <= serial_in;
							TTL_save_exp[13] <= serial_in_exp;							
						end

						3: begin
							TTL_save[12] <= serial_in;
							TTL_save_exp[12] <= serial_in_exp;							
						end

						4: begin
							TTL_save[11] <= serial_in;
							TTL_save_exp[11] <= serial_in_exp;							
						end

						5: begin
							TTL_save[10] <= serial_in;
							TTL_save_exp[10] <= serial_in_exp;							
						end

						6: begin
							TTL_save[9] <= serial_in;
							TTL_save_exp[9] <= serial_in_exp;							
						end

						7: begin
							TTL_save[8] <= serial_in;
							TTL_save_exp[8] <= serial_in_exp;							
						end

						8: begin
							TTL_save[7] <= serial_in;
							TTL_save_exp[7] <= serial_in_exp;							
						end

						9: begin
							TTL_save[6] <= serial_in;
							TTL_save_exp[6] <= serial_in_exp;							
						end

						10: begin
							TTL_save[5] <= serial_in;
							TTL_save_exp[5] <= serial_in_exp;							
						end

						11: begin
							TTL_save[4] <= serial_in;
							TTL_save_exp[4] <= serial_in_exp;							
						end

						12: begin
							TTL_save[3] <= serial_in;
							TTL_save_exp[3] <= serial_in_exp;							
						end

						13: begin
							TTL_save[2] <= serial_in;
							TTL_save_exp[2] <= serial_in_exp;							
						end

						14: begin
							TTL_save[1] <= serial_in;							
						end

						15: begin
							TTL_save[0] <= serial_in;							
						end

						16: begin
							TTL_parallel <= TTL_save;
							TTL_parallel_exp <= TTL_save_exp;
						end

					endcase

				end

			endcase
		end
	end

endmodule
