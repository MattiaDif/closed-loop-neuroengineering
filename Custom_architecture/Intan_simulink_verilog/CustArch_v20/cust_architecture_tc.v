// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v20\cust_architecture_tc.v
// Created: 2021-07-15 12:17:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: cust_architecture_tc
// Source Path: cust_architecture_tc
// Hierarchy Level: 1
// 
// Master clock enable input: clk_enable
// 
// enb         : identical to clk_enable
// enb_1_1_1   : identical to clk_enable
// enb_1_2_0   : 2x slower than clk with last phase
// enb_1_2_1   : 2x slower than clk with phase 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module cust_architecture_tc
          (clk,
           reset,
           clk_enable,
           enb,
           enb_1_1_1,
           enb_1_2_0,
           enb_1_2_1);


  input   clk;
  input   reset;
  input   clk_enable;
  output  enb;
  output  enb_1_1_1;
  output  enb_1_2_0;
  output  enb_1_2_1;


  reg  count2;
  wire phase_all;
  reg  phase_0;
  wire phase_0_tmp;
  reg  phase_1;
  wire phase_1_tmp;


  always @ (posedge clk or posedge reset)
    begin: Counter2
      if (reset == 1'b1) begin
        count2 <= 1'b1;
      end
      else begin
        if (clk_enable == 1'b1) begin
            count2 <= ~ count2;
        end
      end
    end // Counter2

  assign phase_all = clk_enable ? 1'b1 : 1'b0;

  always @ (posedge clk or posedge reset)
    begin: temp_process1
      if (reset == 1'b1) begin
        phase_0 <= 1'b0;
      end
      else begin
        if (clk_enable == 1'b1) begin
          phase_0 <= phase_0_tmp;
        end
      end
    end // temp_process1

  assign  phase_0_tmp = (count2 == 1'b1 && clk_enable == 1'b1) ? 1'b1 : 1'b0;

  always @ (posedge clk or posedge reset)
    begin: temp_process2
      if (reset == 1'b1) begin
        phase_1 <= 1'b1;
      end
      else begin
        if (clk_enable == 1'b1) begin
          phase_1 <= phase_1_tmp;
        end
      end
    end // temp_process2

  assign  phase_1_tmp = (count2 == 1'b0 && clk_enable == 1'b1) ? 1'b1 : 1'b0;

  assign enb =  phase_all & clk_enable;

  assign enb_1_1_1 =  phase_all & clk_enable;

  assign enb_1_2_0 =  phase_0 & clk_enable;

  assign enb_1_2_1 =  phase_1 & clk_enable;


endmodule  // cust_architecture_tc
