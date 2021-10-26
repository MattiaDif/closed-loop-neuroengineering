`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		 Intan Technologies, LLC
// 
// Design Name: 	 RHD2000 Rhythm USB3 Interface
// Module Name:    output_scalable_HPF 
// Project Name:   Opal Kelly FPGA/USB RHD2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Generates SPI control signals for Analog Devices AD5662 16-bit DAC
//                 with adjustable gain and noise suppression, and built-in digital
//                 one-pole high-pass filter.  Optional software reference subtraction.
//
// Dependencies: 
//
// Revision:       2.0 (15 October 2016)
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module scalable_filter #(
	parameter ms_wait  	= 99,
   parameter ms_clk1_a 	= 100,
   parameter ms_clk9_d 	= 135,
	parameter ms_clk18_c = 170,
   parameter ms_clk27_b = 205
	)
	(
	input wire			reset,
	input wire			dataclk,
	input wire [31:0] main_state,
	input wire [5:0]	channel,
	input wire [15:0]	filter_in,
	input wire [15:0] sequencer_in,
	input wire			use_sequencer,
	input wire			en,
	input wire [2:0]  gain,
	input wire [6:0]  noise_suppress,
	output reg			SYNC,
	output reg			SCLK,
	output reg			DIN,
	input wire [15:0]	thrsh,
	input wire			thrsh_pol,
	output wire			thrsh_out,
	input wire [15:0] fsm_start_win_in,     // MM - UPDATE - 1/16/18
	input wire [15:0] fsm_stop_win_in,      // MM - UPDATE - 1/16/18
	input wire [15:0] fsm_state_counter_in, // MM - UPDATE - 1/16/18
	output wire			fsm_inwin_out,		  	 // MM - UPDATE - 1/16/18
	input wire [15:0] filter_coefficient,
	input wire			filter_en,
	input wire			software_reference_mode,
	input wire [15:0] software_reference,
	output wire [15:0] register,
	input wire			filter_type								// 0 LPF vs 1 HPF
   );

	wire [15:0]    input_twos_comp, input_offset, register_pre, subtract_result, add_result;
	reg  [15:0]    input_suppressed, input_scaled;
	wire [10:0] 	noise_suppress_x_16;
	wire [17:0] 	filter_input, multiplier_in;
	wire [18:0] 	multiplier_in_before_limit;
	wire [15:0]    filter_output;
	wire [31:0]    filter_new_state;
	wire [35:0]		multiplier_out;
	reg  [31:0] 	filter_state;
	wire 				positive_overflow, negative_overflow;
	reg				state_clk;
	wire [15:0]		pre_ref_input_twos_comp, software_reference_twos_comp, input_minus_ref;
	wire [16:0]		input_minus_ref_before_limit;
	wire				negative_overflow_ref, positive_overflow_ref;
	
	// MM - UPDATE - WINDOW DISCRIMINATOR - 1/16/18
	// Return whether counter is within window for state machine
	assign fsm_inwin_out = (fsm_state_counter_in >= fsm_start_win_in) && (fsm_stop_win_in > fsm_state_counter_in);
	// END UPDATE
	
	// Optionally, subtract "software reference" value from input
	
	// Convert input and software_reference from unsigned (offset) representation to signed
	// (two's complement) represention by inverting the MSB.
	assign pre_ref_input_twos_comp = {~filter_in[15], filter_in[14:0]};
	assign software_reference_twos_comp = {~software_reference[15], software_reference[14:0]};
	
	assign input_minus_ref_before_limit = pre_ref_input_twos_comp + ~software_reference_twos_comp + 1; // pre_ref_input - software_reference
	assign negative_overflow_ref = pre_ref_input_twos_comp[15] & ~software_reference_twos_comp[15] & ~input_minus_ref_before_limit[15]; // detect negative overflow from subtraction
	assign positive_overflow_ref = ~pre_ref_input_twos_comp[15] & software_reference_twos_comp[15] & input_minus_ref_before_limit[15]; // detect positive overflow from subtraction
	assign input_minus_ref = positive_overflow_ref ? 16'h7fff : (negative_overflow_ref ? 16'h8000 : input_minus_ref_before_limit[15:0]);  // limit subtractor output
	
	// Convert input from unsigned (offset) representation to signed (two's complement)
	// represention by inverting the MSB.	And extend to 18 bits since we use an 18-bit multiplier.
	assign filter_input = software_reference_mode ? { input_minus_ref, 2'b00 } : {~filter_input[15], filter_input[14:0], 2'b00};
	
	// Implement single-pole butterworth state filter
	
	assign multiplier_in_before_limit = filter_input + filter_state[31:14] + 1; 
	assign negative_overflow = filter_input[17] & ~filter_state[31] & ~multiplier_in_before_limit[17]; // detect negative overflow from subtraction
	assign positive_overflow = ~filter_input[17] & filter_state[31] & multiplier_in_before_limit[17]; // detect positive overflow from subtraction
	assign multiplier_in = positive_overflow ? 18'h1ffff : (negative_overflow ? 18'h20000 : multiplier_in_before_limit[17:0]); // limit subtractor output
	
	
	// Multiplier with two 18-bit signed inputs and 36-bit output
	multiplier_18x18 multiplier_1 (
		.clk	(dataclk),
		.a		(multiplier_in),
		.b		({1'b0, filter_coefficient, 1'b0}),
		.p		(multiplier_out)
	);

	// In total: new state = A*state + B*sample = (1-B)*state + B*sample = state + B*(sample - state)
	assign filter_new_state = filter_state + multiplier_out[34:3];
	
	// HPF output = sample - state
	// If filter_type == 1, high-pass (filter_input - filter_state ["slow" component]) otherwise, low-pass (filter_state // "slow" component)
	assign filter_output = filter_en ? (filter_type ?  multiplier_in[17:2] : filter_state): filter_input[17:2];
	
	always @(posedge state_clk) begin
		if (reset) begin
			filter_state <= 32'b0;
		end else begin
			filter_state <= filter_new_state;
		end
		
	end
	
	// End of high-pass filter
	
	assign input_twos_comp = filter_output;
	
	// Convert back from signed (two's complement) representation to unsigned (offset)
	// representation for thresholding.
	
	assign input_offset = {~input_twos_comp[15], input_twos_comp[14:0]};
	
	// Implement simple threshold comparator function on unscaled DAC input
	
	assign thrsh_out = en ? (thrsh_pol ? (input_offset >= thrsh) : (input_offset <= thrsh)) : 1'b0;

	// Now if the input is positive, subtract noise_suppress from it, limiting at zero.
	// If the input is negative, add noise_suppress to it, also limiting at zero.  This
	// has the effect of chopping out the noise between +noise_suppress and -noise_suppress,
	// which makes neural spikes much easier to hear on audio.
	// (Note: noise_suppress is scaled up by 2^4 since very low values of this variable
	// have negligible effect.)
	
	assign noise_suppress_x_16 = {noise_suppress, 4'b0000};
	
	assign subtract_result = input_twos_comp - noise_suppress_x_16;
	assign add_result = input_twos_comp + noise_suppress_x_16;
	
	always @(*) begin
		case ({input_twos_comp[15], subtract_result[15], add_result[15]})
			3'b000: input_suppressed <= subtract_result;
			3'b001: input_suppressed <= subtract_result;
			3'b010: input_suppressed <= 16'b0;
			3'b011: input_suppressed <= 16'b0;
			3'b100: input_suppressed <= 16'b0;
			3'b101: input_suppressed <= add_result;
			3'b110: input_suppressed <= 16'b0;
			3'b111: input_suppressed <= add_result;
		endcase
	end
	
	// Next, scale the input by a factor of 2^gain by left shifting, but preserving the
	// sign and saturating if the scaling exceeds the range of a 16-bit signed number.
	
	always @(*) begin
		case (gain)
			0: input_scaled <= input_suppressed;
			1: input_scaled <= (input_suppressed[14] == {1{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[13:0], 1'b0} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			2: input_scaled <= (input_suppressed[14:13] == {2{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[12:0], 2'b00} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			3: input_scaled <= (input_suppressed[14:12] == {3{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[11:0], 3'b000} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			4: input_scaled <= (input_suppressed[14:11] == {4{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[10:0], 4'b0000} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			5: input_scaled <= (input_suppressed[14:10] == {5{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[9:0], 5'b00000} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			6: input_scaled <= (input_suppressed[14:9] == {6{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[8:0], 6'b000000} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
			7: input_scaled <= (input_suppressed[14:8] == {7{input_suppressed[15]}}) ?
											{input_suppressed[15], input_suppressed[7:0], 7'b0000000} :
											{input_suppressed[15], {15{~input_suppressed[15]}}};
		endcase
	end

	// Finally, convert back from signed (two's complement) representation to unsigned (offset)
	// representation for input to the DAC.  If en == 0, set the DAC to midrange (zero volts).
	
	assign register_pre = use_sequencer ? sequencer_in : {~input_scaled[15], input_scaled[14:0]};
	
	assign register = (en | use_sequencer) ? register_pre : 16'b1000000000000000;
	

	// AD5662 16-bit DAC SPI output logic
	// (See Analog Devices AD5662 datasheet for more information.)
		
	always @(posedge dataclk) begin
		if (reset) begin
			SYNC <= 1'b1;
			SCLK <= 1'b0;
			DIN <= 1'b0;
			state_clk <= 1'b0;
		end else begin
			state_clk <= 1'b0;
			case (main_state)

				ms_wait: begin
					SYNC <= 1'b1;
					SCLK <= 1'b0;
					DIN <= 1'b0;
				end
			
				ms_clk1_a: begin
					case (channel)
					
						0: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
							state_clk <= 1'b1;
						end
						
						1: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						2: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						3: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						4: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
					
						5: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						6: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						7: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						8: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						9: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						10: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						11: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						12: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[15];
						end
						
						13: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[13];
						end
						
						14: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[11];
						end
						
						15: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[9];
						end
						
						16: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[7];
						end
						
						17: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[5];
						end
						
						18: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[3];
						end
						
						19: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[1];
						end
						
					endcase
				end

				ms_clk9_d: begin
					SCLK <= 1'b0;
				end

				ms_clk18_c: begin
					case (channel)
					
						0: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
							state_clk <= 1'b1;
						end
						
						1: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						2: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						3: begin
							SYNC <= 1'b1;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						4: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
					
						5: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						6: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						7: begin
							SYNC <= 1'b0;
							SCLK <= 1'b0;
							DIN <= 1'b0;
						end
						
						8: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						9: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						10: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						11: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= 1'b0;
						end
						
						12: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[14];
						end
						
						13: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[12];
						end
						
						14: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[10];
						end
						
						15: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[8];
						end
						
						16: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[6];
						end
						
						17: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[4];
						end
						
						18: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[2];
						end
						
						19: begin
							SYNC <= 1'b0;
							SCLK <= 1'b1;
							DIN <= register[0];
						end
						
					endcase
				end

				ms_clk27_b: begin
					SCLK <= 1'b0;
				end				
			
			endcase
		end
	end

endmodule
