// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v23\pipe_in_interpret.v
// Created: 2021-09-22 17:25:26
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: pipe_in_interpret
// Source Path: CustArch_v23/cust_architecture/pipe_in_interpret
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module pipe_in_interpret
          (clk,
           reset,
           enb,
           value_from_pipe_in,
           source_channel,
           th,
           refractory_1);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] value_from_pipe_in;  // uint16
  input   [7:0] source_channel;  // uint8
  output  [15:0] th;  // uint16
  output  [15:0] refractory_1;  // uint16


  wire [15:0] channel_threshold_out1;  // uint16
  wire [15:0] refractory_out1;  // uint16


  channel_threshold u_channel_threshold (.clk(clk),
                                         .reset(reset),
                                         .enb(enb),
                                         .In1(value_from_pipe_in),  // uint16
                                         .source_channel(source_channel),  // uint8
                                         .th(channel_threshold_out1)  // uint16
                                         );

  assign th = channel_threshold_out1;

  refractory u_refractory1 (.clk(clk),
                            .reset(reset),
                            .enb(enb),
                            .value_from_pipe_in(value_from_pipe_in),  // uint16
                            .refractory_1(refractory_out1)  // uint16
                            );

  assign refractory_1 = refractory_out1;

endmodule  // pipe_in_interpret

