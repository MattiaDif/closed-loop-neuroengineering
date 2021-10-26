`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		 Intan Technologies, LLC
// 
// Design Name: 	 RHS2000 Rhythm Stim Interface
// Module Name:    stim_sequencer 
// Project Name:   Opal Kelly FPGA/USB RHS2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Generate stimulation control bits for 16 specified stimulation
//                 waveforms.
//
// Dependencies: 
//
// Revision:       1.0 (26 October 2016)
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module stim_sequencer #(
	parameter MODULE = 0
	)
	(
	input wire			reset,
	input wire			dataclk,
	input wire [31:0] main_state,
	input wire [5:0]	channel,
	input wire [3:0]  prog_channel,
	input wire [3:0]	prog_address,
	input wire [4:0]  prog_module,
	input wire [15:0] prog_word,
	input wire			prog_trig,
	input wire [31:0] triggers,
	output reg [15:0] stim_on,
	output reg [15:0] stim_pol,
	output reg [15:0] amp_settle,
	output reg [15:0] charge_recov,
	output reg        amp_settle_changed,
	input	 wire			reset_sequencer
   );

	reg [15:0] counter[15:0];
	
	reg [4:0] trigger_source[15:0];
	reg [15:0] trigger_on_edge;
	reg [15:0] trigger_polarity;
	reg [15:0] trigger_enable;
	
	reg [7:0] number_of_stim_pulses[15:0];
	reg [1:0] stim_shape[15:0];
	reg [15:0] neg_stim_first;
	
	reg [15:0] waiting_for_trigger, waiting_for_edge;
	reg [7:0] stim_counter[15:0];
	
	reg [15:0] event_amp_settle_on[15:0];
	reg [15:0] event_amp_settle_off[15:0];
	reg [15:0] event_start_stim[15:0];
	reg [15:0] event_stim_phase2[15:0];
	reg [15:0] event_stim_phase3[15:0];
	reg [15:0] event_end_stim[15:0];
	reg [15:0] event_repeat_stim[15:0];
	reg [15:0] event_charge_recov_on[15:0];
	reg [15:0] event_charge_recov_off[15:0];
	reg [15:0] event_amp_settle_on_repeat[15:0];
	reg [15:0] event_amp_settle_off_repeat[15:0];
	reg [15:0] event_end[15:0];
	
	localparam
		BIPHASIC =           2'b00,
		BIPHASIC_DEAD_ZONE = 2'b01,
		TRIPHASIC =          2'b10,
		UNUSED =             2'b11;
				  
	// Trigger selection
	reg [15:0] trigger_in;

	always @(posedge dataclk) begin
		if (channel == 0 && (main_state == 99 || main_state == 100)) begin
			trigger_in[0] <= triggers[trigger_source[0]] ^ trigger_polarity[0];
			trigger_in[1] <= triggers[trigger_source[1]] ^ trigger_polarity[1];
			trigger_in[2] <= triggers[trigger_source[2]] ^ trigger_polarity[2];
			trigger_in[3] <= triggers[trigger_source[3]] ^ trigger_polarity[3];
			trigger_in[4] <= triggers[trigger_source[4]] ^ trigger_polarity[4];
			trigger_in[5] <= triggers[trigger_source[5]] ^ trigger_polarity[5];
			trigger_in[6] <= triggers[trigger_source[6]] ^ trigger_polarity[6];
			trigger_in[7] <= triggers[trigger_source[7]] ^ trigger_polarity[7];
			trigger_in[8] <= triggers[trigger_source[8]] ^ trigger_polarity[8];
			trigger_in[9] <= triggers[trigger_source[9]] ^ trigger_polarity[9];
			trigger_in[10] <= triggers[trigger_source[10]] ^ trigger_polarity[10];
			trigger_in[11] <= triggers[trigger_source[11]] ^ trigger_polarity[11];
			trigger_in[12] <= triggers[trigger_source[12]] ^ trigger_polarity[12];
			trigger_in[13] <= triggers[trigger_source[13]] ^ trigger_polarity[13];
			trigger_in[14] <= triggers[trigger_source[14]] ^ trigger_polarity[14];
			trigger_in[15] <= triggers[trigger_source[15]] ^ trigger_polarity[15];
		end
	end
	
	// Register programming
	always @(posedge prog_trig) begin
		if (prog_module == MODULE) begin
			case (prog_address)
				0: begin
						trigger_source[prog_channel] <= prog_word[4:0];
						trigger_on_edge[prog_channel] <= prog_word[5];
						trigger_polarity[prog_channel] <= prog_word[6];
						trigger_enable[prog_channel] <= prog_word[7];
					end
				1: begin
						number_of_stim_pulses[prog_channel] <= prog_word[7:0];
						stim_shape[prog_channel] <= prog_word[9:8];
						neg_stim_first[prog_channel] <= prog_word[10];
					end
				2: event_amp_settle_on[prog_channel] <= prog_word;
				3: event_amp_settle_off[prog_channel] <= prog_word;
				4: event_start_stim[prog_channel] <= prog_word;
				5: event_stim_phase2[prog_channel] <= prog_word;
				6: event_stim_phase3[prog_channel] <= prog_word;
				7: event_end_stim[prog_channel] <= prog_word;
				8: event_repeat_stim[prog_channel] <= prog_word;
				9: event_charge_recov_on[prog_channel] <= prog_word;
				10: event_charge_recov_off[prog_channel] <= prog_word;
				11: event_amp_settle_on_repeat[prog_channel] <= prog_word;
				12: event_amp_settle_off_repeat[prog_channel] <= prog_word;
				13: event_end[prog_channel] <= prog_word;
			endcase
		end
	end
	
	wire [3:0] addr;
	assign addr = channel[3:0];
	
	// State machine for stim sequencing
	
	always @(posedge dataclk) begin
		if (reset) begin
			stim_on <= 16'b0;
			stim_pol <= 16'b0;
			amp_settle <= 16'b0;
			charge_recov <= 16'b0;
			amp_settle_changed <= 1'b1;
			waiting_for_trigger <=16'hffff;
			waiting_for_edge <=16'hffff;
		end else begin
			if (channel[5:4] == 2'b00) begin // only for channel = 0-15
				case (main_state)
					99: begin
						if (reset_sequencer) begin
							stim_on <= 16'b0;
							stim_pol <= 16'b0;
							amp_settle <= 16'b0;
							charge_recov <= 16'b0;
							amp_settle_changed <= 1'b1;
							waiting_for_trigger <=16'hffff;
							waiting_for_edge <=16'hffff;
						end
					end
					102: begin
						if (waiting_for_edge[addr] && waiting_for_trigger[addr] && trigger_on_edge[addr]) begin
							if (~trigger_in[addr]) begin
								waiting_for_edge[addr] <= 1'b0;
							end
						end
						if (waiting_for_trigger[addr]) begin
							counter[addr] <= 16'b0;
							stim_counter[addr] <= number_of_stim_pulses[addr];
							if (trigger_enable[addr] && trigger_in[addr] && (~trigger_on_edge[addr] || ~waiting_for_edge[addr])) begin
								waiting_for_trigger[addr] <= 1'b0;
							end else begin
								stim_on[addr] <= 1'b0;
								stim_pol[addr] <= 1'b0;
								amp_settle[addr] <= 1'b0;
								charge_recov[addr] <= 1'b0;
							end
						end
						if (channel[3:0] == 4'b0000) begin
							amp_settle_changed <= 1'b0;
						end
					end
					
					106: begin
						if (~waiting_for_trigger[addr]) begin
							if (event_amp_settle_on[addr] == counter[addr] ||
								 (event_amp_settle_on_repeat[addr] == counter[addr] && stim_counter[addr] != 8'b0)) begin
								amp_settle[addr] <= 1'b1;
								amp_settle_changed <= 1'b1;
							end else if ((event_amp_settle_off[addr] == counter[addr] && stim_counter[addr] == 8'b0) ||
								 (event_amp_settle_off_repeat[addr] == counter[addr] && stim_counter[addr] != 8'b0)) begin
								amp_settle[addr] <= 1'b0;
								amp_settle_changed <= 1'b1;
							end
							
							if (event_charge_recov_on[addr] == counter[addr] && stim_counter[addr] == 8'b0) begin
								charge_recov[addr] <= 1'b1;
							end else if (event_charge_recov_off[addr] == counter[addr] && stim_counter[addr] == 8'b0) begin
								charge_recov[addr] <= 1'b0;
							end
						end
					end
					
					110: begin
						if (~waiting_for_trigger[addr]) begin
							if (event_start_stim[addr] == counter[addr]) begin
								stim_on[addr] <= 1'b1;
								stim_pol[addr] <= ~neg_stim_first[addr];
							end
						end
					end
					
					114: begin
						if (~waiting_for_trigger[addr]) begin
							if (event_stim_phase2[addr] == counter[addr]) begin
								if (stim_shape[addr] == BIPHASIC_DEAD_ZONE) begin
									stim_on[addr] <= 1'b0;								
								end else begin
									stim_on[addr] <= 1'b1;
									stim_pol[addr] <= neg_stim_first[addr];
								end
							end
						end
					end

					118: begin
						if (~waiting_for_trigger[addr]) begin
							if (event_stim_phase3[addr] == counter[addr]) begin
								if (stim_shape[addr] == BIPHASIC_DEAD_ZONE) begin
									stim_on[addr] <= 1'b1;
									stim_pol[addr] <= neg_stim_first[addr];							
								end else if (stim_shape[addr] == TRIPHASIC) begin
									stim_on[addr] <= 1'b1;
									stim_pol[addr] <= ~neg_stim_first[addr];
								end
							end
						end
					end

					122: begin
						if (~waiting_for_trigger[addr]) begin
							if (event_end_stim[addr] == counter[addr]) begin
								stim_on[addr] <= 1'b0;
								stim_pol[addr] <= ~neg_stim_first[addr];
							end
						end
					end
					
					126: begin
						if (event_repeat_stim[addr] == counter[addr] && stim_counter[addr] != 8'b0) begin
							counter[addr] <= event_start_stim[addr];
							stim_counter[addr] <= stim_counter[addr] - 1;
						end else if (event_end[addr] == counter[addr] && stim_counter[addr] == 8'b0) begin
							counter[addr] <= 16'b0;
							waiting_for_trigger[addr] <= 1'b1;
							waiting_for_edge[addr] <= trigger_on_edge[addr];
						end else begin
							counter[addr] <= counter[addr] + 1;
						end
					end
					default: begin
					
					end
				endcase
			end
		end
	end
	
endmodule
