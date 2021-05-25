// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v8\Detect_change_ch_cnt.v
// Created: 2021-05-20 11:51:25
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Detect_change_ch_cnt
// Source Path: CustArch_v8/cust_architecture/output_interpret/Interpret_to_RAM_output/get_enabled_timings /Detect 
// change ch cn
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Detect_change_ch_cnt
          (clk,
           reset,
           enb,
           U,
           Y);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] U;  // uint8
  output  Y;


  reg [7:0] U_k_1;  // uint8
  wire FixPt_Relational_Operator_relop1;

  // Edge
  // 
  // U(k)


  // 
  // Store in Global RAM
  always @(posedge clk or posedge reset)
    begin : Delay_Input1_process
      if (reset == 1'b1) begin
        U_k_1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          U_k_1 <= U;
        end
      end
    end



  assign FixPt_Relational_Operator_relop1 = U != U_k_1;



  assign Y = FixPt_Relational_Operator_relop1;

endmodule  // Detect_change_ch_cnt

