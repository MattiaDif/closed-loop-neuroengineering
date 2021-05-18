// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v9\OutputStateMachine.v
// Created: 2021-05-13 14:07:46
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: OutputStateMachine
// Source Path: CustArch_v9/cust_architecture/output_interpret/OutputStateMachine
// Hierarchy Level: 2
// 
// Moore machine
// State machine to cycle between the different states according 
// to its input variables.
// The InputState (output of the machine) determines the class of the
// retrieved data changing its processing in the next steps
// The Moore machine output is a function of the current 
// state only. At every time step the input variables dependent
// on the current state update the output of the state machine.
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module OutputStateMachine
          (clk,
           reset,
           enb,
           input_state,
           OutputState);

  // Default encoded enumeration values for type is_OutputStateMachine
  parameter is_OutputStateMachine_IN_idle_state = 5'd0, is_OutputStateMachine_IN_state_ADC = 5'd1, is_OutputStateMachine_IN_state_DAC = 5'd2, is_OutputStateMachine_IN_state_DC_sample = 5'd3, is_OutputStateMachine_IN_state_TTL_in = 5'd4, is_OutputStateMachine_IN_state_TTL_out = 5'd5, is_OutputStateMachine_IN_state_amp_settle = 5'd6, is_OutputStateMachine_IN_state_chan_sample = 5'd7, is_OutputStateMachine_IN_state_charge_recov = 5'd8, is_OutputStateMachine_IN_state_magic_number_0 = 5'd9, is_OutputStateMachine_IN_state_magic_number_01 = 5'd10, is_OutputStateMachine_IN_state_magic_number_02 = 5'd11, is_OutputStateMachine_IN_state_magic_number_03 = 5'd12, is_OutputStateMachine_IN_state_stim_on_off = 5'd13, is_OutputStateMachine_IN_state_stim_pol = 5'd14, is_OutputStateMachine_IN_state_timestamp_0 = 5'd15, is_OutputStateMachine_IN_state_timestamp_01 = 5'd16;

  input   clk;
  input   reset;
  input   enb;
  input   [7:0] input_state;  // uint8
  output  [7:0] OutputState;  // uint8


  reg [4:0] is_OutputStateMachine;  // uint8
  reg [7:0] OutputState_1;  // uint8
  reg [7:0] count_1;  // uint8
  reg [7:0] count_2;  // uint8
  reg [7:0] count_stim_pol;  // uint8
  reg [7:0] count_stim_on_off;  // uint8
  reg [7:0] count_amp_settle;  // uint8
  reg [7:0] count_state_charge;  // uint8
  reg [7:0] count_chs;  // uint8
  reg [4:0] is_OutputStateMachine_temp;  // enum type is_OutputStateMachine (17 enums)
  reg [7:0] count_1_temp;  // uint8
  reg [7:0] count_2_temp;  // uint8
  reg [7:0] count_state_charge_temp;  // uint8
  reg [7:0] count_chs_temp;  // uint8
  reg [7:0] count_stim_pol_temp;  // uint8
  reg [7:0] count_amp_settle_temp;  // uint8
  reg [7:0] count_stim_on_off_temp;  // uint8
  reg [8:0] add_temp;  // ufix9
  reg [8:0] add_temp_0;  // ufix9
  reg [8:0] add_temp_1;  // ufix9
  reg [8:0] add_temp_2;  // ufix9
  reg [8:0] add_temp_3;  // ufix9
  reg [8:0] add_temp_4;  // ufix9
  reg [8:0] add_temp_5;  // ufix9
  reg [8:0] t_0;  // ufix9
  reg [8:0] t_1;  // ufix9
  reg [8:0] t_2;  // ufix9
  reg [8:0] t_3;  // ufix9
  reg [8:0] t_4;  // ufix9
  reg [8:0] t_5;  // ufix9
  reg [8:0] t_6;  // ufix9


  always @(posedge clk or posedge reset)
    begin : OutputStateMachine_1_process
      if (reset == 1'b1) begin
        count_1 <= 8'd0;
        count_2 <= 8'd0;
        count_stim_pol <= 8'd0;
        count_stim_on_off <= 8'd0;
        count_amp_settle <= 8'd0;
        count_state_charge <= 8'd0;
        count_chs <= 8'd0;
        is_OutputStateMachine <= is_OutputStateMachine_IN_idle_state;
      end
      else begin
        if (enb) begin
          add_temp = 9'b000000000;
          add_temp_0 = 9'b000000000;
          add_temp_1 = 9'b000000000;
          add_temp_2 = 9'b000000000;
          add_temp_3 = 9'b000000000;
          add_temp_4 = 9'b000000000;
          add_temp_5 = 9'b000000000;
          t_0 = 9'b000000000;
          t_1 = 9'b000000000;
          t_2 = 9'b000000000;
          t_3 = 9'b000000000;
          t_4 = 9'b000000000;
          t_5 = 9'b000000000;
          t_6 = 9'b000000000;
          count_1_temp = count_1;
          count_2_temp = count_2;
          count_amp_settle_temp = count_amp_settle;
          count_chs_temp = count_chs;
          count_state_charge_temp = count_state_charge;
          count_stim_on_off_temp = count_stim_on_off;
          count_stim_pol_temp = count_stim_pol;
          is_OutputStateMachine_temp = is_OutputStateMachine;
          case ( is_OutputStateMachine)
            is_OutputStateMachine_IN_idle_state :
              begin
              end
            is_OutputStateMachine_IN_state_ADC :
              begin
                count_1_temp = 8'd0;
                t_1 = {1'b0, count_2};
                add_temp_0 = t_1 + 9'b000000001;
                if (add_temp_0[8] != 1'b0) begin
                  count_2_temp = 8'b11111111;
                end
                else begin
                  count_2_temp = add_temp_0[7:0];
                end
              end
            is_OutputStateMachine_IN_state_DAC :
              begin
                count_state_charge_temp = 8'd0;
                t_2 = {1'b0, count_1};
                add_temp_1 = t_2 + 9'b000000001;
                if (add_temp_1[8] != 1'b0) begin
                  count_1_temp = 8'b11111111;
                end
                else begin
                  count_1_temp = add_temp_1[7:0];
                end
              end
            is_OutputStateMachine_IN_state_DC_sample :
              begin
                t_0 = {1'b0, count_chs};
                add_temp = t_0 + 9'b000000001;
                if (add_temp[8] != 1'b0) begin
                  count_chs_temp = 8'b11111111;
                end
                else begin
                  count_chs_temp = add_temp[7:0];
                end
              end
            is_OutputStateMachine_IN_state_TTL_in :
              begin
                count_2_temp = 8'd0;
              end
            is_OutputStateMachine_IN_state_TTL_out :
              begin
              end
            is_OutputStateMachine_IN_state_amp_settle :
              begin
                count_stim_pol_temp = 8'd0;
                t_3 = {1'b0, count_amp_settle};
                add_temp_2 = t_3 + 9'b000000001;
                if (add_temp_2[8] != 1'b0) begin
                  count_amp_settle_temp = 8'b11111111;
                end
                else begin
                  count_amp_settle_temp = add_temp_2[7:0];
                end
              end
            is_OutputStateMachine_IN_state_chan_sample :
              begin
              end
            is_OutputStateMachine_IN_state_charge_recov :
              begin
                count_amp_settle_temp = 8'd0;
                t_4 = {1'b0, count_state_charge};
                add_temp_3 = t_4 + 9'b000000001;
                if (add_temp_3[8] != 1'b0) begin
                  count_state_charge_temp = 8'b11111111;
                end
                else begin
                  count_state_charge_temp = add_temp_3[7:0];
                end
              end
            is_OutputStateMachine_IN_state_magic_number_0 :
              begin
              end
            is_OutputStateMachine_IN_state_magic_number_01 :
              begin
              end
            is_OutputStateMachine_IN_state_magic_number_02 :
              begin
              end
            is_OutputStateMachine_IN_state_magic_number_03 :
              begin
              end
            is_OutputStateMachine_IN_state_stim_on_off :
              begin
                count_chs_temp = 8'd0;
                t_5 = {1'b0, count_stim_on_off};
                add_temp_4 = t_5 + 9'b000000001;
                if (add_temp_4[8] != 1'b0) begin
                  count_stim_on_off_temp = 8'b11111111;
                end
                else begin
                  count_stim_on_off_temp = add_temp_4[7:0];
                end
              end
            is_OutputStateMachine_IN_state_stim_pol :
              begin
                count_stim_on_off_temp = 8'd0;
                t_6 = {1'b0, count_stim_pol};
                add_temp_5 = t_6 + 9'b000000001;
                if (add_temp_5[8] != 1'b0) begin
                  count_stim_pol_temp = 8'b11111111;
                end
                else begin
                  count_stim_pol_temp = add_temp_5[7:0];
                end
              end
            is_OutputStateMachine_IN_state_timestamp_0 :
              begin
              end
            default :
              begin
                //case IN_state_timestamp_01:
              end
          endcase
          case ( is_OutputStateMachine)
            is_OutputStateMachine_IN_idle_state :
              begin
                if (input_state == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_magic_number_0;
                end
              end
            is_OutputStateMachine_IN_state_ADC :
              begin
                if (count_2_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_TTL_in;
                end
                else begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_ADC;
                end
              end
            is_OutputStateMachine_IN_state_DAC :
              begin
                if (count_1_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_ADC;
                end
                else begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_DAC;
                end
              end
            is_OutputStateMachine_IN_state_DC_sample :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_chan_sample;
              end
            is_OutputStateMachine_IN_state_TTL_in :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_TTL_out;
              end
            is_OutputStateMachine_IN_state_TTL_out :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_idle_state;
              end
            is_OutputStateMachine_IN_state_amp_settle :
              begin
                if (count_amp_settle_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_charge_recov;
                end
              end
            is_OutputStateMachine_IN_state_chan_sample :
              begin
                if (count_chs_temp < 8'b10100000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_DC_sample;
                end
                else if (count_chs_temp == 8'b10100000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_stim_on_off;
                end
              end
            is_OutputStateMachine_IN_state_charge_recov :
              begin
                if (count_state_charge_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_DAC;
                end
              end
            is_OutputStateMachine_IN_state_magic_number_0 :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_magic_number_01;
              end
            is_OutputStateMachine_IN_state_magic_number_01 :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_magic_number_02;
              end
            is_OutputStateMachine_IN_state_magic_number_02 :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_magic_number_03;
              end
            is_OutputStateMachine_IN_state_magic_number_03 :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_timestamp_0;
              end
            is_OutputStateMachine_IN_state_stim_on_off :
              begin
                if (count_stim_on_off_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_stim_pol;
                end
              end
            is_OutputStateMachine_IN_state_stim_pol :
              begin
                if (count_stim_pol_temp == 8'b00001000) begin
                  is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_amp_settle;
                end
              end
            is_OutputStateMachine_IN_state_timestamp_0 :
              begin
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_timestamp_01;
              end
            default :
              begin
                //case IN_state_timestamp_01:
                is_OutputStateMachine_temp = is_OutputStateMachine_IN_state_DC_sample;
              end
          endcase
          count_1 <= count_1_temp;
          count_2 <= count_2_temp;
          count_amp_settle <= count_amp_settle_temp;
          count_chs <= count_chs_temp;
          count_state_charge <= count_state_charge_temp;
          count_stim_on_off <= count_stim_on_off_temp;
          count_stim_pol <= count_stim_pol_temp;
          is_OutputStateMachine <= is_OutputStateMachine_temp;
        end
      end
    end

  always @(is_OutputStateMachine) begin
    case ( is_OutputStateMachine)
      is_OutputStateMachine_IN_idle_state :
        begin
          OutputState_1 = 8'd16;
        end
      is_OutputStateMachine_IN_state_ADC :
        begin
          OutputState_1 = 8'd13;
        end
      is_OutputStateMachine_IN_state_DAC :
        begin
          OutputState_1 = 8'd12;
        end
      is_OutputStateMachine_IN_state_DC_sample :
        begin
          OutputState_1 = 8'd6;
        end
      is_OutputStateMachine_IN_state_TTL_in :
        begin
          OutputState_1 = 8'd14;
        end
      is_OutputStateMachine_IN_state_TTL_out :
        begin
          OutputState_1 = 8'd15;
        end
      is_OutputStateMachine_IN_state_amp_settle :
        begin
          OutputState_1 = 8'd10;
        end
      is_OutputStateMachine_IN_state_chan_sample :
        begin
          OutputState_1 = 8'd7;
        end
      is_OutputStateMachine_IN_state_charge_recov :
        begin
          OutputState_1 = 8'd11;
        end
      is_OutputStateMachine_IN_state_magic_number_0 :
        begin
          OutputState_1 = 8'd0;
        end
      is_OutputStateMachine_IN_state_magic_number_01 :
        begin
          OutputState_1 = 8'd1;
        end
      is_OutputStateMachine_IN_state_magic_number_02 :
        begin
          OutputState_1 = 8'd2;
        end
      is_OutputStateMachine_IN_state_magic_number_03 :
        begin
          OutputState_1 = 8'd3;
        end
      is_OutputStateMachine_IN_state_stim_on_off :
        begin
          OutputState_1 = 8'd8;
        end
      is_OutputStateMachine_IN_state_stim_pol :
        begin
          OutputState_1 = 8'd9;
        end
      is_OutputStateMachine_IN_state_timestamp_0 :
        begin
          OutputState_1 = 8'd4;
        end
      default :
        begin
          //case IN_state_timestamp_01:
          OutputState_1 = 8'd5;
        end
    endcase
  end



  assign OutputState = OutputState_1;

endmodule  // OutputStateMachine

