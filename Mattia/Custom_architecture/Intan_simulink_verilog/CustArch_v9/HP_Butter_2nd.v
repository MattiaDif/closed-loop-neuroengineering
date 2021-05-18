// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v9\HP_Butter_2nd.v
// Created: 2021-05-13 14:07:46
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: HP_Butter_2nd
// Source Path: CustArch_v9/cust_architecture/process_and_retrieve/digital_filter/HP_Butter_2nd
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module HP_Butter_2nd
          (new_raw,
           previous_samples_0,
           previous_samples_1,
           new_fitlered,
           new_previous);


  input   signed [15:0] new_raw;  // sfix16_En1
  input   signed [31:0] previous_samples_0;  // sfix32_En9
  input   signed [31:0] previous_samples_1;  // sfix32_En9
  output  signed [31:0] new_fitlered;  // sfix32_En20
  output  signed [31:0] new_previous;  // sfix32_En9


  wire signed [48:0] s_1_1_mul_temp;  // sfix49_En33
  wire signed [47:0] s_1_1_cast;  // sfix48_En33
  wire signed [31:0] s_1_1_out1;  // sfix32_En17
  wire signed [31:0] t_1;  // sfix32_En9
  wire signed [63:0] a_2_1_1_mul_temp;  // sfix64_En39
  wire signed [31:0] a_2_1_1_out1;  // sfix32_En8
  wire signed [41:0] SumA1_sub_cast;  // sfix42_En17
  wire signed [41:0] SumA1_sub_cast_1;  // sfix42_En17
  wire signed [41:0] SumA1_sub_temp;  // sfix42_En17
  wire signed [31:0] SumA1_out1;  // sfix32_En8
  wire signed [31:0] t_2;  // sfix32_En9
  wire signed [64:0] a_3_1_1_mul_temp;  // sfix65_En41
  wire signed [63:0] a_3_1_1_cast;  // sfix64_En41
  wire signed [31:0] a_3_1_1_out1;  // sfix32_En9
  wire signed [33:0] SumA2_sub_cast;  // sfix34_En9
  wire signed [33:0] SumA2_sub_cast_1;  // sfix34_En9
  wire signed [33:0] SumA2_sub_temp;  // sfix34_En9
  wire signed [31:0] SumA2_out1;  // sfix32_En9
  wire signed [63:0] b_2_1_1_mul_temp;  // sfix64_En39
  wire signed [63:0] b_2_1_1_1;  // sfix64
  wire signed [31:0] b_2_1_1_out1;  // sfix32_En8
  wire signed [33:0] SumB1_add_cast;  // sfix34_En9
  wire signed [33:0] SumB1_add_cast_1;  // sfix34_En9
  wire signed [33:0] SumB1_add_temp;  // sfix34_En9
  wire signed [31:0] SumB1_out1;  // sfix32_En9
  wire signed [32:0] SumB2_add_cast;  // sfix33_En9
  wire signed [32:0] SumB2_add_cast_1;  // sfix33_En9
  wire signed [32:0] SumB2_add_temp;  // sfix33_En9
  wire signed [31:0] SumB2_out1;  // sfix32_En20


  assign s_1_1_mul_temp = 33'sh0F4E0044F * new_raw;
  assign s_1_1_cast = s_1_1_mul_temp[47:0];
  assign s_1_1_out1 = ((s_1_1_cast[47] == 1'b0) && (s_1_1_cast[46:16] == 31'b1111111111111111111111111111111) ? 32'sb01111111111111111111111111111111 :
              s_1_1_cast[47:16] + $signed({1'b0, s_1_1_cast[15]}));



  assign t_1 = previous_samples_1;

  assign a_2_1_1_mul_temp = 32'sb10000101101011101111001001111111 * t_1;
  assign a_2_1_1_out1 = (((a_2_1_1_mul_temp[63] == 1'b0) && (a_2_1_1_mul_temp[62] != 1'b0)) || ((a_2_1_1_mul_temp[63] == 1'b0) && (a_2_1_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((a_2_1_1_mul_temp[63] == 1'b1) && (a_2_1_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              a_2_1_1_mul_temp[62:31] + $signed({1'b0, a_2_1_1_mul_temp[30]})));



  assign SumA1_sub_cast = {{10{s_1_1_out1[31]}}, s_1_1_out1};
  assign SumA1_sub_cast_1 = {a_2_1_1_out1[31], {a_2_1_1_out1, 9'b000000000}};
  assign SumA1_sub_temp = SumA1_sub_cast - SumA1_sub_cast_1;
  assign SumA1_out1 = (((SumA1_sub_temp[41] == 1'b0) && (SumA1_sub_temp[40] != 1'b0)) || ((SumA1_sub_temp[41] == 1'b0) && (SumA1_sub_temp[40:9] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((SumA1_sub_temp[41] == 1'b1) && (SumA1_sub_temp[40] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              SumA1_sub_temp[40:9] + $signed({1'b0, SumA1_sub_temp[8]})));



  assign t_2 = previous_samples_0;

  assign a_3_1_1_mul_temp = 33'sh0EA3BDB37 * t_2;
  assign a_3_1_1_cast = a_3_1_1_mul_temp[63:0];
  assign a_3_1_1_out1 = ((a_3_1_1_cast[63] == 1'b0) && (a_3_1_1_cast[62:32] == 31'b1111111111111111111111111111111) ? 32'sb01111111111111111111111111111111 :
              a_3_1_1_cast[63:32] + $signed({1'b0, a_3_1_1_cast[31]}));



  assign SumA2_sub_cast = {SumA1_out1[31], {SumA1_out1, 1'b0}};
  assign SumA2_sub_cast_1 = {{2{a_3_1_1_out1[31]}}, a_3_1_1_out1};
  assign SumA2_sub_temp = SumA2_sub_cast - SumA2_sub_cast_1;
  assign SumA2_out1 = ((SumA2_sub_temp[33] == 1'b0) && (SumA2_sub_temp[32:31] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((SumA2_sub_temp[33] == 1'b1) && (SumA2_sub_temp[32:31] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              $signed(SumA2_sub_temp[31:0])));



  // CSD Encoding (2147483648) : 010000000000000000000000000000000; Cost (Adders) = 0
  assign b_2_1_1_1 = {t_1[31], {t_1, 31'b0000000000000000000000000000000}};
  assign b_2_1_1_mul_temp =  - (b_2_1_1_1);
  assign b_2_1_1_out1 = (((b_2_1_1_mul_temp[63] == 1'b0) && (b_2_1_1_mul_temp[62] != 1'b0)) || ((b_2_1_1_mul_temp[63] == 1'b0) && (b_2_1_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((b_2_1_1_mul_temp[63] == 1'b1) && (b_2_1_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              b_2_1_1_mul_temp[62:31] + $signed({1'b0, b_2_1_1_mul_temp[30]})));



  assign SumB1_add_cast = {{2{SumA2_out1[31]}}, SumA2_out1};
  assign SumB1_add_cast_1 = {b_2_1_1_out1[31], {b_2_1_1_out1, 1'b0}};
  assign SumB1_add_temp = SumB1_add_cast + SumB1_add_cast_1;
  assign SumB1_out1 = ((SumB1_add_temp[33] == 1'b0) && (SumB1_add_temp[32:31] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((SumB1_add_temp[33] == 1'b1) && (SumB1_add_temp[32:31] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              $signed(SumB1_add_temp[31:0])));



  assign SumB2_add_cast = {SumB1_out1[31], SumB1_out1};
  assign SumB2_add_cast_1 = {t_2[31], t_2};
  assign SumB2_add_temp = SumB2_add_cast + SumB2_add_cast_1;
  assign SumB2_out1 = ((SumB2_add_temp[32] == 1'b0) && (SumB2_add_temp[31:20] != 12'b000000000000) ? 32'sb01111111111111111111111111111111 :
              ((SumB2_add_temp[32] == 1'b1) && (SumB2_add_temp[31:20] != 12'b111111111111) ? 32'sb10000000000000000000000000000000 :
              {SumB2_add_temp[20:0], 11'b00000000000}));



  assign new_fitlered = SumB2_out1;

  assign new_previous = SumA2_out1;

endmodule  // HP_Butter_2nd

