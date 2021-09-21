// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\RMS_computation_tc.v
// Created: 2021-09-20 10:52:56
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: RMS_computation_tc
// Source Path: RMS_computation_tc
// Hierarchy Level: 1
// 
// Master clock enable input: clk_enable
// 
// enb_3_1_0   : identical to clk_enable
// enb         : 3x slower than clk with last phase
// enb_1_1_1   : 3x slower than clk with phase 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module RMS_computation_tc
          (clk,
           reset,
           clk_enable,
           enb_3_1_0,
           enb,
           enb_1_1_1);


  input   clk;
  input   reset;
  input   clk_enable;
  output  enb_3_1_0;
  output  enb;
  output  enb_1_1_1;


  reg [1:0] count3;  // ufix2
  wire phase_all;
  reg  phase_0;
  wire phase_0_tmp;
  reg  phase_1;
  wire phase_1_tmp;


  always @ (posedge clk or posedge reset)
    begin: Counter3
      if (reset == 1'b1) begin
        count3 <= 2'b01;
      end
      else begin
        if (clk_enable == 1'b1) begin
          if (count3 >= 2'b10) begin
            count3 <= 2'b00;
          end
          else begin
            count3 <= count3 + 2'b01;
          end
        end
      end
    end // Counter3

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

  assign  phase_0_tmp = (count3 == 2'b10 && clk_enable == 1'b1) ? 1'b1 : 1'b0;

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

  assign  phase_1_tmp = (count3 == 2'b00 && clk_enable == 1'b1) ? 1'b1 : 1'b0;

  assign enb_3_1_0 =  phase_all & clk_enable;

  assign enb =  phase_0 & clk_enable;

  assign enb_1_1_1 =  phase_1 & clk_enable;


endmodule  // RMS_computation_tc
