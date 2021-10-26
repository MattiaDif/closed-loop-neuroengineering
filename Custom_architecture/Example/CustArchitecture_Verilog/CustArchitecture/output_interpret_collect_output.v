// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArchitecture\output_interpret_collect_output.v
// Created: 2021-10-26 14:22:20
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: output_interpret_collect_output
// Source Path: CustArchitecture/cust_architecture/output_interpret_collect_output
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module output_interpret_collect_output
          (clk,
           reset,
           enb,
           FIFO_data_in,
           FIFO_write_to,
           InputState,
           data_stream_enable,
           output_data_input_interpret_store_retrieve,
           filter_output_or_spike,
           comp_data_out_valid,
           comp_data_out,
           read_filter_output_or_spike,
           OutputState,
           read_output_data_input_interpret_store_retrieve);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  input   [7:0] data_stream_enable;  // uint8
  input   [15:0] output_data_input_interpret_store_retrieve;  // uint16
  input   [15:0] filter_output_or_spike;  // uint16
  output  comp_data_out_valid;
  output  [15:0] comp_data_out;  // uint16
  output  [8:0] read_filter_output_or_spike;  // ufix9
  output  [7:0] OutputState;  // uint8
  output  [8:0] read_output_data_input_interpret_store_retrieve;  // ufix9


  wire output_interpret_out2;
  wire [8:0] rd_addr;  // ufix9
  wire collect_output_out1;
  wire [15:0] collect_output_out2;  // uint16
  wire [8:0] rd_addr_1;  // ufix9


  output_interpret u_output_interpret (.clk(clk),
                                       .reset(reset),
                                       .enb(enb),
                                       .data_stream_enable(data_stream_enable),  // uint8
                                       .InputState(InputState),  // uint8
                                       .OutputState(OutputState),  // uint8
                                       .rd_enable(output_interpret_out2),
                                       .read_output_data_input_interpret_store_retrieve(rd_addr)  // ufix9
                                       );

  collect_output u_collect_output (.clk(clk),
                                   .reset(reset),
                                   .enb(enb),
                                   .FIFO_data_in(FIFO_data_in),  // uint16
                                   .FIFO_write_to(FIFO_write_to),
                                   .InputState(InputState),  // uint8
                                   .OutputState(OutputState),  // uint8
                                   .rd_enable(output_interpret_out2),
                                   .rd_address(rd_addr),  // ufix9
                                   .read_from_storage_value(output_data_input_interpret_store_retrieve),  // uint16
                                   .rd_dout(filter_output_or_spike),  // uint16
                                   .comp_data_out_valid(collect_output_out1),
                                   .comp_data_out(collect_output_out2),  // uint16
                                   .rd_address_bit_set_AC_during_DC(rd_addr_1)  // ufix9
                                   );

  assign comp_data_out_valid = collect_output_out1;

  assign comp_data_out = collect_output_out2;

  assign read_filter_output_or_spike = rd_addr_1;

  assign read_output_data_input_interpret_store_retrieve = rd_addr;

endmodule  // output_interpret_collect_output
