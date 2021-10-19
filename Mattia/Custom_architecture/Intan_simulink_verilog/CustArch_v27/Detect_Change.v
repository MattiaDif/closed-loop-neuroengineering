// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v27\Detect_Change.v
// Created: 2021-10-14 15:24:50
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Detect_Change
// Source Path: CustArch_v27/cust_architecture/process_and_retrieve/pipe_in_interpret/channel_threshold/Detect Change
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Detect_Change
          (clk,
           reset,
           enb,
           U,
           Y);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] U;  // uint16
  output  Y;


  reg [15:0] U_k_1;  // uint16
  wire FixPt_Relational_Operator_relop1;

  // Edge
  // 
  // U(k)


  // 
  // Store in Global RAM
  always @(posedge clk or posedge reset)
    begin : Delay_Input1_process
      if (reset == 1'b1) begin
        U_k_1 <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          U_k_1 <= U;
        end
      end
    end



  assign FixPt_Relational_Operator_relop1 = U != U_k_1;



  assign Y = FixPt_Relational_Operator_relop1;

endmodule  // Detect_Change

