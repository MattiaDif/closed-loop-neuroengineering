// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\spike_detection.v
// Created: 2021-09-20 14:51:01
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: spike_detection
// Source Path: CustArch_v22/cust_architecture/process_and_retrieve/spike_detection
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module spike_detection
          (clk,
           reset,
           enb,
           filter_output,
           write_porcessing_addr_read_from_storage_addr_1,
           rd_addr,
           InputState,
           spike);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] filter_output;  // uint16
  input   [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  input   [8:0] rd_addr;  // ufix9
  input   InputState;
  output  [15:0] spike;  // uint16


  wire LocalMaximaAdaptiveThreshold_out1;
  wire refractory_period_out1;
  wire Logical_Operator_out1;
  wire [15:0] refractory_period_out2;  // uint16

  // spike detected with one sample of delay
  // due to the local maxima check which needs
  // current sample, t-1 and t-2
  // 
  // introduced delay = 1/dataclk


  LocalMaximaAdaptiveThreshold u_LocalMaximaAdaptiveThreshold (.clk(clk),
                                                               .reset(reset),
                                                               .enb(enb),
                                                               .filter_output(filter_output),  // uint16
                                                               .InputState(InputState),
                                                               .write_porcessing_addr_read_from_storage_addr_1(write_porcessing_addr_read_from_storage_addr_1),  // ufix9
                                                               .local_maxima_out(LocalMaximaAdaptiveThreshold_out1)
                                                               );

  assign Logical_Operator_out1 = LocalMaximaAdaptiveThreshold_out1 & refractory_period_out1;



  refractory_period u_refractory_period (.clk(clk),
                                         .reset(reset),
                                         .enb(enb),
                                         .InputState(InputState),
                                         .wr_en(Logical_Operator_out1),
                                         .write_porcessing_addr_read_from_storage_addr_1(write_porcessing_addr_read_from_storage_addr_1),  // ufix9
                                         .rd_addr(rd_addr),  // ufix9
                                         .refractory_check(refractory_period_out1),
                                         .spike(refractory_period_out2)  // uint16
                                         );

  assign spike = refractory_period_out2;

endmodule  // spike_detection
