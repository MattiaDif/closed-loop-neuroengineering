// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v21\two_pole_IIR.v
// Created: 2021-10-18 17:44:13
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: two_pole_IIR
// Source Path: CustArch_v21/cust_architecture/process_and_retrieve/filtering/two_pole_IIR
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module two_pole_IIR
          (clk,
           reset,
           enb_1_2_0,
           current_sample,
           previous_sample_0,
           previous_sample_1,
           filter_output,
           new_previous_sample);


  input   clk;
  input   reset;
  input   enb_1_2_0;
  input   [15:0] current_sample;  // uint16
  input   signed [31:0] previous_sample_0;  // sfix32_En7
  input   signed [31:0] previous_sample_1;  // sfix32_En7
  output  [15:0] filter_output;  // uint16
  output  signed [31:0] new_previous_sample;  // sfix32_En7


  wire [15:0] Constant_out1;  // uint16
  wire signed [31:0] Subtract_sub_cast;  // sfix32_En14
  wire signed [31:0] Subtract_sub_cast_1;  // sfix32_En14
  wire signed [31:0] Subtract_sub_temp;  // sfix32_En14
  wire signed [31:0] Subtract_out1;  // sfix32_En15
  wire signed [31:0] HP_Butter_2nd_out1;  // sfix32_En15
  wire signed [31:0] HP_Butter_2nd_out2;  // sfix32_En7
  wire [15:0] Constant1_out1;  // uint16
  wire signed [31:0] Subtract1_add_cast;  // sfix32_En14
  wire signed [31:0] Subtract1_add_cast_1;  // sfix32_En14
  wire signed [31:0] Subtract1_add_temp;  // sfix32_En14
  wire [15:0] Subtract1_out1;  // uint16


  assign Constant_out1 = 16'b1000000000000000;



  assign Subtract_sub_cast = {2'b0, {current_sample, 14'b00000000000000}};
  assign Subtract_sub_cast_1 = {2'b0, {Constant_out1, 14'b00000000000000}};
  assign Subtract_sub_temp = Subtract_sub_cast - Subtract_sub_cast_1;
  assign Subtract_out1 = {Subtract_sub_temp[30:0], 1'b0};



  HP_Butter_2nd u_HP_Butter_2nd (.clk(clk),
                                 .reset(reset),
                                 .enb_1_2_0(enb_1_2_0),
                                 .Input_rsvd(Subtract_out1),  // sfix32_En15
                                 .previous_samples_0(previous_sample_0),  // sfix32_En7
                                 .previous_samples_1(previous_sample_1),  // sfix32_En7
                                 .Output_rsvd(HP_Butter_2nd_out1),  // sfix32_En15
                                 .Output1(HP_Butter_2nd_out2)  // sfix32_En7
                                 );

  assign Constant1_out1 = 16'b1000000000000000;



  assign Subtract1_add_cast = {HP_Butter_2nd_out1[31], HP_Butter_2nd_out1[31:1]};
  assign Subtract1_add_cast_1 = {2'b0, {Constant1_out1, 14'b00000000000000}};
  assign Subtract1_add_temp = Subtract1_add_cast + Subtract1_add_cast_1;
  assign Subtract1_out1 = Subtract1_add_temp[29:14];



  assign filter_output = Subtract1_out1;

  assign new_previous_sample = HP_Butter_2nd_out2;

endmodule  // two_pole_IIR

