// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v24\pipe_in_interpret.v
// Created: 2021-09-30 11:54:48
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: pipe_in_interpret
// Source Path: CustArch_v24/cust_architecture/process_and_retrieve/pipe_in_interpret
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module pipe_in_interpret
          (clk,
           reset,
           enb,
           th_reading_address,
           value_from_pipe_in,
           pipe_in_data_0,
           pipe_in_data_1);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] th_reading_address;  // ufix9
  input   [15:0] value_from_pipe_in;  // uint16
  output  [15:0] pipe_in_data_0;  // uint16
  output  [15:0] pipe_in_data_1;  // uint16


  wire [15:0] channel_threshold_out1;  // uint16
  wire [15:0] refractory_out1;  // uint16


  channel_threshold u_channel_threshold (.clk(clk),
                                         .reset(reset),
                                         .enb(enb),
                                         .th_reading_address(th_reading_address),  // ufix9
                                         .value_from_pipe_in(value_from_pipe_in),  // uint16
                                         .th(channel_threshold_out1)  // uint16
                                         );

  assign pipe_in_data_0 = channel_threshold_out1;

  refractory u_refractory (.clk(clk),
                           .reset(reset),
                           .enb(enb),
                           .value_from_pipe_in(value_from_pipe_in),  // uint16
                           .refractory_1(refractory_out1)  // uint16
                           );

  assign pipe_in_data_1 = refractory_out1;

endmodule  // pipe_in_interpret
