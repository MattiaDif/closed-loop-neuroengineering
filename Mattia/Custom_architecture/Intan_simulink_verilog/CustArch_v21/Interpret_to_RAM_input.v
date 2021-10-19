// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v21\Interpret_to_RAM_input.v
// Created: 2021-10-18 17:44:12
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Interpret_to_RAM_input
// Source Path: CustArch_v21/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Interpret_to_RAM_input
          (clk,
           reset,
           enb,
           FIFO_data_in,
           FIFO_write_to,
           InputState,
           data_stream_enable,
           device_output,
           inc_source_device,
           wr_din,
           write_address,
           wr_en);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  input   [7:0] data_stream_enable;  // uint8
  output  [7:0] device_output;  // uint8
  output  inc_source_device;
  output  [15:0] wr_din;  // uint16
  output  [8:0] write_address;  // ufix9
  output  wr_en;


  wire [2:0] y;  // ufix3
  wire interpret_incoming_data_out2;
  wire [7:0] interpret_incoming_data_out3;  // uint8
  wire [7:0] y_1;  // uint8
  wire [2:0] y_2;  // ufix3

  // These subsystems identify the sources of the incoming data (amplifier --> source_device and channel number) and 
  // 
  // determine the address to be associated to the current data for the RAM storage. This statement is valid for all 
  // the kind of
  // data except for the magic number and the timestamp that are directly sent to a different RAM in the Output section


  interpret_incoming_data u_interpret_incoming_data (.clk(clk),
                                                     .reset(reset),
                                                     .enb(enb),
                                                     .FIFO_write_to(FIFO_write_to),
                                                     .InputState(InputState),  // uint8
                                                     .data_stream_enable(data_stream_enable),  // uint8
                                                     .source_device(y),  // ufix3
                                                     .inc_source_device(interpret_incoming_data_out2),
                                                     .device_output(interpret_incoming_data_out3),  // uint8
                                                     .source_channel_0_159(y_1),  // uint8
                                                     .converter_cnt(y_2)  // ufix3
                                                     );

  assign device_output = interpret_incoming_data_out3;

  assign inc_source_device = interpret_incoming_data_out2;

  RAM_address_definition u_RAM_address_definition (.DataIn(FIFO_data_in),  // uint16
                                                   .InputState(InputState),  // uint8
                                                   .source_channel_0_159(y_1),  // uint8
                                                   .source_device(y),  // ufix3
                                                   .converter_cnt(y_2),  // ufix3
                                                   .write_to(FIFO_write_to),
                                                   .wr_din(wr_din),  // uint16
                                                   .wr_addr(write_address),  // ufix9
                                                   .write_enable_6to15(wr_en)
                                                   );

endmodule  // Interpret_to_RAM_input

