// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v7\Detect_Change.v
// Created: 2021-05-04 15:27:11
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Detect_Change
// Source Path: CustArch_v7/cust_architecture/process_and_retrieve/previous_sample_storage2/Detect Change
// Hierarchy Level: 3
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
  input   [9:0] U;  // ufix10
  output  Y;


  reg [9:0] U_k_1;  // ufix10
  wire FixPt_Relational_Operator_relop1;

  // Edge
  // 
  // U(k)


  // 
  // Store in Global RAM
  always @(posedge clk or posedge reset)
    begin : Delay_Input1_process
      if (reset == 1'b1) begin
        U_k_1 <= 10'b0000000000;
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
