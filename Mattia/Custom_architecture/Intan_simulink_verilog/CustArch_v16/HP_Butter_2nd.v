// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v16\HP_Butter_2nd.v
// Created: 2021-06-21 17:49:55
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: HP_Butter_2nd
// Source Path: CustArch_v16/cust_architecture/process_and_retrieve/FilterChip1/digital_filter/HP_Butter_2nd
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module HP_Butter_2nd
          (Input_rsvd,
           previous_samples_0,
           previous_samples_1,
           Output_rsvd,
           Output1);


  input   signed [31:0] Input_rsvd;  // sfix32_En15
  input   signed [31:0] previous_samples_0;  // sfix32_En7
  input   signed [31:0] previous_samples_1;  // sfix32_En7
  output  signed [31:0] Output_rsvd;  // sfix32_En15
  output  signed [31:0] Output1;  // sfix32_En7


  wire signed [64:0] s_1_mul_temp;  // sfix65_En47
  wire signed [63:0] s_1_cast;  // sfix64_En47
  wire signed [31:0] s_1_out1;  // sfix32_En16
  wire signed [63:0] a_2_1_mul_temp;  // sfix64_En37
  wire signed [31:0] a_2_1_out1;  // sfix32_En6
  wire signed [31:0] SumA21_sub_cast;  // sfix32_En6
  wire signed [32:0] SumA21_sub_cast_1;  // sfix33_En6
  wire signed [32:0] SumA21_sub_cast_2;  // sfix33_En6
  wire signed [32:0] SumA21_sub_temp;  // sfix33_En6
  wire signed [31:0] SumA21_out1;  // sfix32_En6
  wire signed [64:0] a_3_1_mul_temp;  // sfix65_En39
  wire signed [63:0] a_3_1_cast;  // sfix64_En39
  wire signed [31:0] a_3_1_out1;  // sfix32_En8
  wire signed [32:0] SumA31_sub_cast;  // sfix33_En6
  wire signed [31:0] SumA31_sub_cast_1;  // sfix32_En6
  wire signed [32:0] SumA31_sub_cast_2;  // sfix33_En6
  wire signed [32:0] SumA31_sub_temp;  // sfix33_En6
  wire signed [31:0] SumA31_cast;  // sfix32_En6
  wire signed [31:0] SumA31_out1;  // sfix32_En7
  wire signed [63:0] b_2_1_mul_temp;  // sfix64_En37
  wire signed [63:0] b_2_1_1;  // sfix64
  wire signed [31:0] b_2_1_out1;  // sfix32_En6
  wire signed [31:0] SumB21_add_cast;  // sfix32_En6
  wire signed [32:0] SumB21_add_cast_1;  // sfix33_En6
  wire signed [32:0] SumB21_add_cast_2;  // sfix33_En6
  wire signed [32:0] SumB21_add_temp;  // sfix33_En6
  wire signed [31:0] SumB21_cast;  // sfix32_En6
  wire signed [31:0] SumB21_out1;  // sfix32_En7
  wire signed [32:0] SumB31_add_cast;  // sfix33_En7
  wire signed [32:0] SumB31_add_cast_1;  // sfix33_En7
  wire signed [32:0] SumB31_add_temp;  // sfix33_En7
  wire signed [31:0] SumB31_cast;  // sfix32_En7
  wire signed [31:0] SumB31_out1;  // sfix32_En15


  assign s_1_mul_temp = 33'sh0F4E0044F * Input_rsvd;
  assign s_1_cast = s_1_mul_temp[63:0];
  assign s_1_out1 = (((s_1_cast[63] == 1'b0) && (s_1_cast[62] != 1'b0)) || ((s_1_cast[63] == 1'b0) && (s_1_cast[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((s_1_cast[63] == 1'b1) && (s_1_cast[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              s_1_cast[62:31] + $signed({1'b0, s_1_cast[30]})));



  assign a_2_1_mul_temp = 32'sb10000101101011101111001001111111 * previous_samples_0;
  assign a_2_1_out1 = (((a_2_1_mul_temp[63] == 1'b0) && (a_2_1_mul_temp[62] != 1'b0)) || ((a_2_1_mul_temp[63] == 1'b0) && (a_2_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((a_2_1_mul_temp[63] == 1'b1) && (a_2_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              a_2_1_mul_temp[62:31] + $signed({1'b0, a_2_1_mul_temp[30]})));



  assign SumA21_sub_cast = ({{10{s_1_out1[31]}}, s_1_out1[31:10]}) + $signed({1'b0, s_1_out1[9]});
  assign SumA21_sub_cast_1 = {SumA21_sub_cast[31], SumA21_sub_cast};
  assign SumA21_sub_cast_2 = {a_2_1_out1[31], a_2_1_out1};
  assign SumA21_sub_temp = SumA21_sub_cast_1 - SumA21_sub_cast_2;
  assign SumA21_out1 = ((SumA21_sub_temp[32] == 1'b0) && (SumA21_sub_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumA21_sub_temp[32] == 1'b1) && (SumA21_sub_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumA21_sub_temp[31:0])));



  assign a_3_1_mul_temp = 33'sh0EA3BDB37 * previous_samples_1;
  assign a_3_1_cast = a_3_1_mul_temp[63:0];
  assign a_3_1_out1 = (((a_3_1_cast[63] == 1'b0) && (a_3_1_cast[62] != 1'b0)) || ((a_3_1_cast[63] == 1'b0) && (a_3_1_cast[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((a_3_1_cast[63] == 1'b1) && (a_3_1_cast[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              a_3_1_cast[62:31] + $signed({1'b0, a_3_1_cast[30]})));



  assign SumA31_sub_cast = {SumA21_out1[31], SumA21_out1};
  assign SumA31_sub_cast_1 = ({{2{a_3_1_out1[31]}}, a_3_1_out1[31:2]}) + $signed({1'b0, a_3_1_out1[1]});
  assign SumA31_sub_cast_2 = {SumA31_sub_cast_1[31], SumA31_sub_cast_1};
  assign SumA31_sub_temp = SumA31_sub_cast - SumA31_sub_cast_2;
  assign SumA31_cast = ((SumA31_sub_temp[32] == 1'b0) && (SumA31_sub_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumA31_sub_temp[32] == 1'b1) && (SumA31_sub_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumA31_sub_temp[31:0])));
  assign SumA31_out1 = ((SumA31_cast[31] == 1'b0) && (SumA31_cast[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumA31_cast[31] == 1'b1) && (SumA31_cast[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {SumA31_cast[30:0], 1'b0}));



  // CSD Encoding (2147483648) : 010000000000000000000000000000000; Cost (Adders) = 0
  assign b_2_1_1 = {previous_samples_0[31], {previous_samples_0, 31'b0000000000000000000000000000000}};
  assign b_2_1_mul_temp =  - (b_2_1_1);
  assign b_2_1_out1 = (((b_2_1_mul_temp[63] == 1'b0) && (b_2_1_mul_temp[62] != 1'b0)) || ((b_2_1_mul_temp[63] == 1'b0) && (b_2_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((b_2_1_mul_temp[63] == 1'b1) && (b_2_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              b_2_1_mul_temp[62:31] + $signed({1'b0, b_2_1_mul_temp[30]})));



  assign SumB21_add_cast = ({SumA31_out1[31], SumA31_out1[31:1]}) + $signed({1'b0, SumA31_out1[0]});
  assign SumB21_add_cast_1 = {SumB21_add_cast[31], SumB21_add_cast};
  assign SumB21_add_cast_2 = {b_2_1_out1[31], b_2_1_out1};
  assign SumB21_add_temp = SumB21_add_cast_1 + SumB21_add_cast_2;
  assign SumB21_cast = ((SumB21_add_temp[32] == 1'b0) && (SumB21_add_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumB21_add_temp[32] == 1'b1) && (SumB21_add_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumB21_add_temp[31:0])));
  assign SumB21_out1 = ((SumB21_cast[31] == 1'b0) && (SumB21_cast[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumB21_cast[31] == 1'b1) && (SumB21_cast[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {SumB21_cast[30:0], 1'b0}));



  assign SumB31_add_cast = {SumB21_out1[31], SumB21_out1};
  assign SumB31_add_cast_1 = {previous_samples_1[31], previous_samples_1};
  assign SumB31_add_temp = SumB31_add_cast + SumB31_add_cast_1;
  assign SumB31_cast = ((SumB31_add_temp[32] == 1'b0) && (SumB31_add_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumB31_add_temp[32] == 1'b1) && (SumB31_add_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumB31_add_temp[31:0])));
  assign SumB31_out1 = ((SumB31_cast[31] == 1'b0) && (SumB31_cast[30:23] != 8'b00000000) ? 32'sb01111111111111111111111111111111 :
              ((SumB31_cast[31] == 1'b1) && (SumB31_cast[30:23] != 8'b11111111) ? 32'sb10000000000000000000000000000000 :
              {SumB31_cast[23:0], 8'b00000000}));



  assign Output_rsvd = SumB31_out1;

  assign Output1 = SumA31_out1;

endmodule  // HP_Butter_2nd

