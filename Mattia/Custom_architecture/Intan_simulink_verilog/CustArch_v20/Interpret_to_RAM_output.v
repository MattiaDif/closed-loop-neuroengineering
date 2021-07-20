// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v20\Interpret_to_RAM_output.v
// Created: 2021-07-16 12:05:04
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Interpret_to_RAM_output
// Source Path: CustArch_v20/cust_architecture/output_interpret/Interpret_to_RAM_output
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Interpret_to_RAM_output
          (clk,
           reset,
           enb,
           OutputState,
           data_stream_en,
           rd_enable,
           read_address);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] OutputState;  // uint8
  input   [7:0] data_stream_en;  // uint8
  output  rd_enable;
  output  [8:0] read_address;  // ufix9


  wire get_enabled_timings_out1;
  wire [7:0] get_enabled_timings_out2;  // uint8
  wire y;  // ufix1
  wire [2:0] y_1;  // ufix3
  wire [2:0] y_2;  // ufix3
  wire Output_State_to_RAM_address_to_read_out1;
  wire [8:0] rd_addr;  // ufix9


  get_enabled_timings u_get_enabled_timings (.clk(clk),
                                             .reset(reset),
                                             .enb(enb),
                                             .OutputState(OutputState),  // uint8
                                             .data_stream_en(data_stream_en),  // uint8
                                             .source_channel_en(get_enabled_timings_out1),
                                             .source_channel_0_159(get_enabled_timings_out2),  // uint8
                                             .source_device_enabled(y),  // ufix1
                                             .source_device(y_1),  // ufix3
                                             .converter_cnt(y_2)  // ufix3
                                             );

  Output_State_to_RAM_address_to_read u_Output_State_to_RAM_address_to_read (.source_channel_en(get_enabled_timings_out1),
                                                                             .source_channel_0_159(get_enabled_timings_out2),  // uint8
                                                                             .OutputState(OutputState),  // uint8
                                                                             .source_device_en(y),  // ufix1
                                                                             .source_device(y_1),  // ufix3
                                                                             .converter_cnt(y_2),  // ufix3
                                                                             .rd_enable(Output_State_to_RAM_address_to_read_out1),
                                                                             .rd_addr(rd_addr)  // ufix9
                                                                             );

  assign rd_enable = Output_State_to_RAM_address_to_read_out1;

  assign read_address = rd_addr;

endmodule  // Interpret_to_RAM_output

