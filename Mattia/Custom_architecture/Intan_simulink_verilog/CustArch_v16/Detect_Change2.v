// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v16\Detect_Change2.v
// Created: 2021-06-21 17:49:55
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Detect_Change2
// Source Path: CustArch_v16/cust_architecture/process_and_retrieve/FilterChip1/Detect Change2
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Detect_Change2
          (clk,
           reset,
           enb,
           U,
           Y);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] U;  // ufix9
  output  Y;


  reg [8:0] U_k_1;  // ufix9
  wire FixPt_Relational_Operator_relop1;

  // Edge
  // 
  // U(k)


  // 
  // Store in Global RAM
  always @(posedge clk or posedge reset)
    begin : Delay_Input1_process
      if (reset == 1'b1) begin
        U_k_1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          U_k_1 <= U;
        end
      end
    end



  assign FixPt_Relational_Operator_relop1 = U != U_k_1;



  assign Y = FixPt_Relational_Operator_relop1;

endmodule  // Detect_Change2

