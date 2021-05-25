// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\cust_arch_18\Filter1.v
// Created: 2021-05-25 12:43:17
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Filter1
// Source Path: cust_arch_18/cust_architecture/process_and_retrieve/one_pole_IIR/Filter1
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Filter1
          (Input_rsvd,
           Input1,
           Output_rsvd,
           Output1);


  input   signed [15:0] Input_rsvd;  // int16
  input   signed [15:0] Input1;  // sfix16_E5
  output  signed [15:0] Output_rsvd;  // sfix16_E1
  output  signed [15:0] Output1;  // sfix16_E5


  wire signed [32:0] s_1_mul_temp;  // sfix33_En16
  wire signed [31:0] s_1_cast;  // sfix32_En16
  wire signed [15:0] s_1_out1;  // int16
  wire signed [31:0] a_2_1_mul_temp;  // sfix32_En10
  wire signed [15:0] a_2_1_out1;  // sfix16_E4
  wire signed [20:0] SumA21_sub_cast;  // sfix21
  wire signed [20:0] SumA21_sub_temp;  // sfix21
  wire signed [20:0] SumA21_1;  // sfix21
  wire signed [15:0] SumA21_out1;  // sfix16_E5
  wire signed [16:0] SumB21_sub_cast;  // sfix17_E5
  wire signed [16:0] SumB21_sub_cast_1;  // sfix17_E5
  wire signed [16:0] SumB21_sub_temp;  // sfix17_E5
  wire signed [15:0] SumB21_out1;  // sfix16_E1


  assign s_1_mul_temp = 17'sb01111100000110011 * Input_rsvd;
  assign s_1_cast = s_1_mul_temp[31:0];
  assign s_1_out1 = ((s_1_cast[31] == 1'b0) && (s_1_cast[30:16] == 15'b111111111111111) ? 16'sb0111111111111111 :
              s_1_cast[31:16] + $signed({1'b0, s_1_cast[15]}));



  assign a_2_1_mul_temp = 16'sb1000011111001101 * Input1;
  assign a_2_1_out1 = (((a_2_1_mul_temp[31] == 1'b0) && (a_2_1_mul_temp[30:29] != 2'b00)) || ((a_2_1_mul_temp[31] == 1'b0) && (a_2_1_mul_temp[29:14] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((a_2_1_mul_temp[31] == 1'b1) && (a_2_1_mul_temp[30:29] != 2'b11) ? 16'sb1000000000000000 :
              a_2_1_mul_temp[29:14] + $signed({1'b0, a_2_1_mul_temp[13]})));



  assign SumA21_sub_cast = {a_2_1_out1[15], {a_2_1_out1, 4'b0000}};
  assign SumA21_1 = {{5{s_1_out1[15]}}, s_1_out1};
  assign SumA21_sub_temp = SumA21_1 - SumA21_sub_cast;
  assign SumA21_out1 = ((SumA21_sub_temp[20] == 1'b0) && (SumA21_sub_temp[19:5] == 15'b111111111111111) ? 16'sb0111111111111111 :
              SumA21_sub_temp[20:5] + $signed({1'b0, SumA21_sub_temp[4]}));



  assign SumB21_sub_cast = {SumA21_out1[15], SumA21_out1};
  assign SumB21_sub_cast_1 = {Input1[15], Input1};
  assign SumB21_sub_temp = SumB21_sub_cast - SumB21_sub_cast_1;
  assign SumB21_out1 = ((SumB21_sub_temp[16] == 1'b0) && (SumB21_sub_temp[15:11] != 5'b00000) ? 16'sb0111111111111111 :
              ((SumB21_sub_temp[16] == 1'b1) && (SumB21_sub_temp[15:11] != 5'b11111) ? 16'sb1000000000000000 :
              {SumB21_sub_temp[11:0], 4'b0000}));



  assign Output_rsvd = SumB21_out1;

  assign Output1 = SumA21_out1;

endmodule  // Filter1

