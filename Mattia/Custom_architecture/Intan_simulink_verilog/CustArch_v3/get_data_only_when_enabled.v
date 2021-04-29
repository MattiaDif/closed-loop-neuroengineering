// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v3\get_data_only_when_enabled.v
// Created: 2021-04-28 17:35:59
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: get_data_only_when_enabled
// Source Path: CustArch_v3/cust_architecture/collect_output/get_data_only_when_enabled
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module get_data_only_when_enabled
          (clk,
           reset,
           enb,
           In1,
           Enable,
           Out1);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] In1;  // uint16
  input   Enable;
  output  [15:0] Out1;  // uint16


  wire enb_gated;
  wire [15:0] In1_bypass;  // uint16
  reg [15:0] In1_last_value;  // uint16


  assign enb_gated = Enable && enb;

  always @(posedge clk or posedge reset)
    begin : Out1_bypass_process
      if (reset == 1'b1) begin
        In1_last_value <= 16'b0000000000000000;
      end
      else begin
        if (enb_gated) begin
          In1_last_value <= In1_bypass;
        end
      end
    end



  assign In1_bypass = (Enable == 1'b0 ? In1_last_value :
              In1);



  assign Out1 = In1_bypass;

endmodule  // get_data_only_when_enabled

