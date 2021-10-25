// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v19\InputStateMachine.v
// Created: 2021-07-15 14:18:03
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: InputStateMachine
// Source Path: CustArch_v19/cust_architecture/input_interpret_store_retrieve/InputStateMachine
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
// Bad state machine?
// Probably I should use converter counter but it works 
// so let's keep like this for now
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module InputStateMachine
          (clk,
           reset,
           enb,
           FIFO_data_in,
           FIFO_write_to,
           channel_0_19,
           inc_source_device_MSB,
           InputState);

  // Default encoded enumeration values for type is_InputStateMachine
  parameter is_InputStateMachine_IN_state_ADC = 5'd0, is_InputStateMachine_IN_state_ADC1 = 5'd1, is_InputStateMachine_IN_state_DAC = 5'd2, is_InputStateMachine_IN_state_DAC1 = 5'd3, is_InputStateMachine_IN_state_DC_sample = 5'd4, is_InputStateMachine_IN_state_TTL_in = 5'd5, is_InputStateMachine_IN_state_TTL_out = 5'd6, is_InputStateMachine_IN_state_amp_settle = 5'd7, is_InputStateMachine_IN_state_chan_sample = 5'd8, is_InputStateMachine_IN_state_charge_recov = 5'd9, is_InputStateMachine_IN_state_magic_number_0 = 5'd10, is_InputStateMachine_IN_state_magic_number_01 = 5'd11, is_InputStateMachine_IN_state_magic_number_02 = 5'd12, is_InputStateMachine_IN_state_magic_number_03 = 5'd13, is_InputStateMachine_IN_state_stim_on_off = 5'd14, is_InputStateMachine_IN_state_stim_pol = 5'd15, is_InputStateMachine_IN_state_timestamp_0 = 5'd16, is_InputStateMachine_IN_state_timestamp_01 = 5'd17;

  input   clk;
  input   reset;
  input   enb;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] channel_0_19;  // uint8
  input   inc_source_device_MSB;
  output  [7:0] InputState;  // uint8


  reg [4:0] is_InputStateMachine;  // uint8
  reg [7:0] InputState_1;  // uint8
  reg [7:0] b_count_1;  // uint8
  reg [7:0] b_count_2;  // uint8
  reg [4:0] is_InputStateMachine_temp;  // enum type is_InputStateMachine (18 enums)
  reg [7:0] b_count_1_temp;  // uint8
  reg [7:0] b_count_2_temp;  // uint8
  reg [8:0] add_temp;  // ufix9
  reg [8:0] add_temp_0;  // ufix9
  reg [8:0] t_0;  // ufix9
  reg [8:0] t_1;  // ufix9


  always @(posedge clk or posedge reset)
    begin : InputStateMachine_1_process
      if (reset == 1'b1) begin
        b_count_1 <= 8'd0;
        b_count_2 <= 8'd0;
        is_InputStateMachine <= is_InputStateMachine_IN_state_magic_number_0;
      end
      else begin
        if (enb) begin
          add_temp = 9'b000000000;
          add_temp_0 = 9'b000000000;
          t_0 = 9'b000000000;
          t_1 = 9'b000000000;
          b_count_1_temp = b_count_1;
          b_count_2_temp = b_count_2;
          is_InputStateMachine_temp = is_InputStateMachine;
          case ( is_InputStateMachine)
            is_InputStateMachine_IN_state_ADC :
              begin
                b_count_1_temp = 8'd0;
                t_1 = {1'b0, b_count_2};
                add_temp_0 = t_1 + 9'b000000001;
                if (add_temp_0[8] != 1'b0) begin
                  b_count_2_temp = 8'b11111111;
                end
                else begin
                  b_count_2_temp = add_temp_0[7:0];
                end
              end
            is_InputStateMachine_IN_state_ADC1 :
              begin
                b_count_1_temp = 8'd0;
              end
            is_InputStateMachine_IN_state_DAC :
              begin
                t_0 = {1'b0, b_count_1};
                add_temp = t_0 + 9'b000000001;
                if (add_temp[8] != 1'b0) begin
                  b_count_1_temp = 8'b11111111;
                end
                else begin
                  b_count_1_temp = add_temp[7:0];
                end
              end
            is_InputStateMachine_IN_state_DAC1 :
              begin
              end
            is_InputStateMachine_IN_state_DC_sample :
              begin
              end
            is_InputStateMachine_IN_state_TTL_in :
              begin
                b_count_2_temp = 8'd0;
              end
            is_InputStateMachine_IN_state_TTL_out :
              begin
              end
            is_InputStateMachine_IN_state_amp_settle :
              begin
              end
            is_InputStateMachine_IN_state_chan_sample :
              begin
              end
            is_InputStateMachine_IN_state_charge_recov :
              begin
              end
            is_InputStateMachine_IN_state_magic_number_0 :
              begin
              end
            is_InputStateMachine_IN_state_magic_number_01 :
              begin
              end
            is_InputStateMachine_IN_state_magic_number_02 :
              begin
              end
            is_InputStateMachine_IN_state_magic_number_03 :
              begin
              end
            is_InputStateMachine_IN_state_stim_on_off :
              begin
              end
            is_InputStateMachine_IN_state_stim_pol :
              begin
              end
            is_InputStateMachine_IN_state_timestamp_0 :
              begin
              end
            default :
              begin
                //case IN_state_timestamp_01:
              end
          endcase
          case ( is_InputStateMachine)
            is_InputStateMachine_IN_state_ADC :
              begin
                if (b_count_2_temp == 8'b00001000) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_TTL_in;
                end
                else if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_ADC;
                end
                else if ( ! FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_ADC1;
                end
              end
            is_InputStateMachine_IN_state_ADC1 :
              begin
                if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_ADC;
                end
              end
            is_InputStateMachine_IN_state_DAC :
              begin
                if (b_count_1_temp == 8'b00001000) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_ADC;
                end
                else if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DAC;
                end
                else if ( ! FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DAC1;
                end
              end
            is_InputStateMachine_IN_state_DAC1 :
              begin
                if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DAC;
                end
              end
            is_InputStateMachine_IN_state_DC_sample :
              begin
                if (FIFO_write_to && (channel_0_19 < 8'b00010100)) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_chan_sample;
                end
                else if (channel_0_19 == 8'b00010100) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_stim_on_off;
                end
              end
            is_InputStateMachine_IN_state_TTL_in :
              begin
                if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_TTL_out;
                end
              end
            is_InputStateMachine_IN_state_TTL_out :
              begin
                is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_0;
              end
            is_InputStateMachine_IN_state_amp_settle :
              begin
                if (inc_source_device_MSB) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_charge_recov;
                end
              end
            is_InputStateMachine_IN_state_chan_sample :
              begin
                if ((channel_0_19 < 8'b00010100) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DC_sample;
                end
                else if (((channel_0_19 == 8'b00010100) && inc_source_device_MSB) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_stim_on_off;
                end
              end
            is_InputStateMachine_IN_state_charge_recov :
              begin
                if (inc_source_device_MSB) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DAC;
                end
              end
            is_InputStateMachine_IN_state_magic_number_0 :
              begin
                if ((FIFO_data_in == 16'b0010111100001011) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_01;
                end
              end
            is_InputStateMachine_IN_state_magic_number_01 :
              begin
                if ((FIFO_data_in == 16'b0100100101110001) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_02;
                end
                else begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_0;
                end
              end
            is_InputStateMachine_IN_state_magic_number_02 :
              begin
                if ((FIFO_data_in == 16'b0010110010001010) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_03;
                end
                else begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_0;
                end
              end
            is_InputStateMachine_IN_state_magic_number_03 :
              begin
                if ((FIFO_data_in == 16'b1000110101010100) && FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_timestamp_0;
                end
                else begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_0;
                end
              end
            is_InputStateMachine_IN_state_stim_on_off :
              begin
                if (inc_source_device_MSB) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_stim_pol;
                end
              end
            is_InputStateMachine_IN_state_stim_pol :
              begin
                if (inc_source_device_MSB) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_amp_settle;
                end
              end
            is_InputStateMachine_IN_state_timestamp_0 :
              begin
                if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_timestamp_01;
                end
                else begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_magic_number_0;
                end
              end
            default :
              begin
                //case IN_state_timestamp_01:
                if (FIFO_write_to) begin
                  is_InputStateMachine_temp = is_InputStateMachine_IN_state_DC_sample;
                end
              end
          endcase
          b_count_1 <= b_count_1_temp;
          b_count_2 <= b_count_2_temp;
          is_InputStateMachine <= is_InputStateMachine_temp;
        end
      end
    end

  always @(is_InputStateMachine) begin
    case ( is_InputStateMachine)
      is_InputStateMachine_IN_state_ADC :
        begin
          InputState_1 = 8'd13;
        end
      is_InputStateMachine_IN_state_ADC1 :
        begin
          InputState_1 = 8'd13;
        end
      is_InputStateMachine_IN_state_DAC :
        begin
          InputState_1 = 8'd12;
        end
      is_InputStateMachine_IN_state_DAC1 :
        begin
          InputState_1 = 8'd12;
        end
      is_InputStateMachine_IN_state_DC_sample :
        begin
          InputState_1 = 8'd6;
        end
      is_InputStateMachine_IN_state_TTL_in :
        begin
          InputState_1 = 8'd14;
        end
      is_InputStateMachine_IN_state_TTL_out :
        begin
          InputState_1 = 8'd15;
        end
      is_InputStateMachine_IN_state_amp_settle :
        begin
          InputState_1 = 8'd10;
        end
      is_InputStateMachine_IN_state_chan_sample :
        begin
          InputState_1 = 8'd7;
        end
      is_InputStateMachine_IN_state_charge_recov :
        begin
          InputState_1 = 8'd11;
        end
      is_InputStateMachine_IN_state_magic_number_0 :
        begin
          InputState_1 = 8'd0;
        end
      is_InputStateMachine_IN_state_magic_number_01 :
        begin
          InputState_1 = 8'd1;
        end
      is_InputStateMachine_IN_state_magic_number_02 :
        begin
          InputState_1 = 8'd2;
        end
      is_InputStateMachine_IN_state_magic_number_03 :
        begin
          InputState_1 = 8'd3;
        end
      is_InputStateMachine_IN_state_stim_on_off :
        begin
          InputState_1 = 8'd8;
        end
      is_InputStateMachine_IN_state_stim_pol :
        begin
          InputState_1 = 8'd9;
        end
      is_InputStateMachine_IN_state_timestamp_0 :
        begin
          InputState_1 = 8'd4;
        end
      default :
        begin
          //case IN_state_timestamp_01:
          InputState_1 = 8'd5;
        end
    endcase
  end



  assign InputState = InputState_1;

endmodule  // InputStateMachine
