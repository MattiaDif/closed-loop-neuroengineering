`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		 Intan Technologies, LLC
// 
// Design Name: 	 RHS2000 Rhythm Stim Interface
// Module Name:    analog_out_sequencer 
// Project Name:   Opal Kelly FPGA/USB RHS2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Generate voltage pulses for 8 analog outputs ports.
//
// Dependencies: 
//
// Revision:       1.0 (26 October 2016)
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module analog_out_sequencer #(
	parameter MODULE = 0
	)
	(
	input wire			reset,
	input wire			dataclk,
	input wire [31:0] main_state,
	input wire [5:0]	channel,
	input wire [3:0]	prog_address,
	input wire [4:0]  prog_module,
	input wire [15:0] prog_word,
	input wire			prog_trig,
	input wire [31:0] triggers,
	output wire			DAC_sequencer_en,
	output reg [15:0] DAC_out,
	input wire			shutdown,
	input wire			reset_sequencer
   );

	reg [15:0] counter;
	
	reg [4:0] trigger_source;
	reg trigger_on_edge;
	reg trigger_polarity;
	reg trigger_enable;
	
	assign DAC_sequencer_en = trigger_enable;
	
	reg [7:0] number_of_stim_pulses;
	reg [1:0] stim_shape;
	reg neg_stim_first;
	
	reg waiting_for_trigger, waiting_for_edge;
	reg [7:0] stim_counter;
	
	reg [15:0] event_amp_settle_on;
	reg [15:0] event_amp_settle_off;
	reg [15:0] event_start_stim;
	reg [15:0] event_stim_phase2;
	reg [15:0] event_stim_phase3;
	reg [15:0] event_end_stim;
	reg [15:0] event_repeat_stim;
	reg [15:0] event_end;
	
	reg [15:0] DAC_baseline;
	reg [15:0] DAC_positive;
	reg [15:0] DAC_negative;
	
	localparam
		BIPHASIC =           2'b00,
		BIPHASIC_DEAD_ZONE = 2'b01,
		TRIPHASIC =          2'b10,
		MONOPHASIC =         2'b11;
				  
	// Trigger selection
	reg trigger_in;

	always @(posedge dataclk) begin
		if (channel == 0 && (main_state == 99 || main_state == 100)) begin
			trigger_in <= triggers[trigger_source] ^ trigger_polarity;
		end
	end
	
	// Register programming
	always @(posedge prog_trig) begin
		if (prog_module == MODULE) begin
			case (prog_address)
				0: begin
						trigger_source <= prog_word[4:0];
						trigger_on_edge <= prog_word[5];
						trigger_polarity <= prog_word[6];
						trigger_enable <= prog_word[7];
					end
				1: begin
						number_of_stim_pulses <= prog_word[7:0];
						stim_shape <= prog_word[9:8];
						neg_stim_first <= prog_word[10];
					end
				4: event_start_stim <= prog_word;
				5: event_stim_phase2 <= prog_word;
				6: event_stim_phase3 <= prog_word;
				7: event_end_stim <= prog_word;
				8: event_repeat_stim <= prog_word;
				9: DAC_baseline <= prog_word;
				10: DAC_positive <= prog_word;
				11: DAC_negative <= prog_word;
				13: event_end <= prog_word;
			endcase
		end
	end
	
	// State machine for stim sequencing
	
	always @(posedge dataclk) begin
		if (reset) begin
			DAC_out <= DAC_baseline;
			waiting_for_trigger <=1'b1;
			waiting_for_edge <=1'b1;
		end else begin
			if (channel[5:0] == 6'b000000) begin
				case (main_state)
					99: begin
						if (reset_sequencer) begin
							DAC_out <= DAC_baseline;
							waiting_for_trigger <=1'b1;
							waiting_for_edge <=1'b1;						
						end
					end
					110: begin
						if (waiting_for_edge && waiting_for_trigger && trigger_on_edge) begin
							if (~trigger_in) begin
								waiting_for_edge <= 1'b0;
							end
						end
						if (waiting_for_trigger) begin
							counter <= 16'b0;
							stim_counter <= number_of_stim_pulses;
							if (trigger_enable && trigger_in && (~trigger_on_edge || ~waiting_for_edge)) begin
								waiting_for_trigger <= 1'b0;
							end else begin
								DAC_out <= DAC_baseline;
							end
						end
					end
					
					120: begin
						if (~waiting_for_trigger) begin
							if (event_start_stim == counter) begin
								DAC_out <= (neg_stim_first ? DAC_negative : DAC_positive);
							end
						end
					end
					
					130: begin
						if (~waiting_for_trigger) begin
							if (event_stim_phase2 == counter) begin
								if (stim_shape == BIPHASIC_DEAD_ZONE) begin
									DAC_out <= DAC_baseline;								
								end else if (stim_shape != MONOPHASIC) begin
									DAC_out <= (neg_stim_first ? DAC_positive : DAC_negative);
								end
							end
						end
					end

					140: begin
						if (~waiting_for_trigger) begin
							if (event_stim_phase3 == counter) begin
								if (stim_shape == BIPHASIC_DEAD_ZONE) begin
									DAC_out <= (neg_stim_first ? DAC_positive : DAC_negative);
								end else if (stim_shape == TRIPHASIC) begin
									DAC_out <= (neg_stim_first ? DAC_negative : DAC_positive);
								end
							end
						end
					end

					150: begin
						if (~waiting_for_trigger) begin
							if (event_end_stim == counter) begin
								DAC_out <= DAC_baseline;
							end
						end
					end
					
					160: begin
						if (shutdown) begin
							DAC_out <= DAC_baseline;
						end
					end
					
					170: begin
						if (event_repeat_stim == counter && stim_counter != 8'b0) begin
							counter <= event_start_stim;
							stim_counter <= stim_counter - 1;
						end else if (event_end == counter && stim_counter == 8'b0) begin
							counter <= 16'b0;
							waiting_for_trigger <= 1'b1;
							waiting_for_edge <= trigger_on_edge;
						end else begin
							counter <= counter + 1;
						end
					end
					default: begin
					
					end
				endcase
			end
		end
	end
	
endmodule
