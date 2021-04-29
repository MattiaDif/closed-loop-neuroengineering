// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v3\interpret_incoming_data.v
// Created: 2021-04-28 17:35:59
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: interpret_incoming_data
// Source Path: CustArch_v3/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// dat
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module interpret_incoming_data
          (clk,
           reset,
           enb,
           FIFO_write_to,
           InputState,
           data_stream_enable,
           source_device,
           inc_source_device,
           device_output,
           source_channel_0_159,
           converter_cnt);


  input   clk;
  input   reset;
  input   enb;
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  input   [7:0] data_stream_enable;  // uint8
  output  [2:0] source_device;  // ufix3
  output  inc_source_device;
  output  [7:0] device_output;  // uint8
  output  [7:0] source_channel_0_159;  // uint8
  output  [2:0] converter_cnt;  // ufix3


  wire [7:0] get_sources_and_RAM_addresses_out1;  // uint8
  wire [7:0] get_sources_and_RAM_addresses_out2;  // uint8
  wire [7:0] y;  // uint8
  wire get_sources_and_RAM_addresses_out4;
  wire [2:0] Bit_Slice1_out1;  // ufix3
  wire condition_to_count_DAC_or_ADC_out1;
  reg [7:0] HDL_Counter_out1;  // uint8
  wire [2:0] Bit_Slice_out1;  // ufix3

  // counter of the converters
  // 
  // to reduce the number of bit to encode
  // the ADC/DAC counter since it could takes
  // values from 0 to 7
  // 
  // check ADC/DAC state
  // 
  // ADC/DAC counter
  // (from 0 to 7 for each)
  // 
  // to reduce the number of bit to encode
  // the soure device since it could takes
  // values from 0 to 7
  // 
  // amplifier chip under
  // processing
  // 
  // channel data
  // address
  // 
  // channel (electrode) under
  // processing
  // 
  // necessary to move
  // forward from state 6 to 8


  get_sources_and_RAM_addresses u_get_sources_and_RAM_addresses (.clk(clk),
                                                                 .reset(reset),
                                                                 .enb(enb),
                                                                 .FIFO_write_to(FIFO_write_to),
                                                                 .InputState(InputState),  // uint8
                                                                 .data_stream_enable(data_stream_enable),  // uint8
                                                                 .source_device(get_sources_and_RAM_addresses_out1),  // uint8
                                                                 .device_output(get_sources_and_RAM_addresses_out2),  // uint8
                                                                 .source_channel(y),  // uint8
                                                                 .inc_source_device(get_sources_and_RAM_addresses_out4)
                                                                 );

  assign Bit_Slice1_out1 = get_sources_and_RAM_addresses_out1[2:0];



  assign source_device = Bit_Slice1_out1;

  assign inc_source_device = get_sources_and_RAM_addresses_out4;

  assign device_output = get_sources_and_RAM_addresses_out2;

  assign source_channel_0_159 = y;

  condition_to_count_DAC_or_ADC u_condition_to_count_DAC_or_ADC (.fifo_write_to(FIFO_write_to),
                                                                 .input_state(InputState),  // uint8
                                                                 .Out1(condition_to_count_DAC_or_ADC_out1)
                                                                 );

  // Count limited, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  //  count to value  = 7
  always @(posedge clk or posedge reset)
    begin : HDL_Counter_process
      if (reset == 1'b1) begin
        HDL_Counter_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          if (condition_to_count_DAC_or_ADC_out1 == 1'b1) begin
            if (HDL_Counter_out1 >= 8'b00000111) begin
              HDL_Counter_out1 <= 8'b00000000;
            end
            else begin
              HDL_Counter_out1 <= HDL_Counter_out1 + 8'b00000001;
            end
          end
        end
      end
    end



  assign Bit_Slice_out1 = HDL_Counter_out1[2:0];



  assign converter_cnt = Bit_Slice_out1;

endmodule  // interpret_incoming_data

