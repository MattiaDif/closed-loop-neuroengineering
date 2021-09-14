// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\pipe_in_v3\pipe_in_interpret.v
// Created: 2021-09-10 13:33:33
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: pipe_in_interpret
// Source Path: pipe_in_v3/cust_architecture/pipe_in_interpret
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module pipe_in_interpret
          (clk,
           reset,
           enb,
           pipe_in_data,
           pipe_in_en,
           pipe_in,
           PipeInState);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] pipe_in_data;  // uint16
  input   pipe_in_en;
  output  [15:0] pipe_in;  // uint16
  output  [7:0] PipeInState;  // uint8




  assign pipe_in = pipe_in_data;

  Chart1 u_Chart1 (.clk(clk),
                   .reset(reset),
                   .enb(enb),
                   .pipe_in_en(pipe_in_en),
                   .PipeInState(PipeInState)  // uint8
                   );

endmodule  // pipe_in_interpret

