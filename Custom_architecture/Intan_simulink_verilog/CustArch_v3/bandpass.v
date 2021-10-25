// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v3\bandpass.v
// Created: 2021-04-28 17:35:59
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: bandpass
// Source Path: CustArch_v3/cust_architecture/process_and_retrieve/bandpass
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module bandpass
          (clk,
           reset,
           enb,
           In,
           Enable,
           Out);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] In;  // uint16
  input   Enable;
  output  [15:0] Out;  // uint16


  wire [15:0] Constant1_out1;  // uint16
  wire signed [16:0] Add1_sub_temp;  // sfix17
  wire signed [16:0] Add1_1;  // sfix17
  wire signed [16:0] Add1_2;  // sfix17
  wire signed [15:0] Add1_out1;  // sfix16_En4
  wire signed [15:0] scaleconst1;  // sfix16_En15
  wire signed [31:0] multiplier_mul_temp;  // sfix32_En19
  wire signed [38:0] scale1;  // sfix39_En26
  wire signed [15:0] scaletypeconvert1;  // sfix16_En4
  wire signed [33:0] inputconv1;  // sfix34_En14
  wire enb_gated;
  wire signed [15:0] coeff_a2_section1;  // sfix16_En14
  wire enb_gated_1;
  wire signed [15:0] coeff_a3_section1;  // sfix16_En14
  reg signed [15:0] typeconvert1_tap0;  // sfix16_E3
  reg signed [15:0] typeconvert1_tap1;  // sfix16_E3
  wire signed [31:0] multiplier2_mul_temp;  // sfix32_En11
  wire signed [31:0] a3mul1;  // sfix32_En12
  wire signed [33:0] a1sum1_cast2;  // sfix34_En14
  wire signed [15:0] typeconvert1;  // sfix16_E3
  wire signed [31:0] multiplier1_mul_temp;  // sfix32_En11
  wire signed [31:0] a2mul1;  // sfix32_En12
  wire signed [33:0] a2sum1_cast2;  // sfix34_En14
  wire signed [34:0] Sub_sub_cast;  // sfix35_En14
  wire signed [34:0] Sub_sub_cast_1;  // sfix35_En14
  wire signed [34:0] a2sum1_temp;  // sfix35_En14
  wire signed [33:0] a2sum1;  // sfix34_En14
  wire signed [34:0] Sub1_sub_cast;  // sfix35_En14
  wire signed [34:0] Sub1_sub_cast_1;  // sfix35_En14
  wire signed [34:0] a1sum1_temp;  // sfix35_En14
  wire signed [33:0] a1sum1;  // sfix34_En14
  wire signed [31:0] b1mul1;  // sfix32_En12
  wire signed [33:0] b1multypeconvert1;  // sfix34_En14
  wire signed [31:0] gain1_mul_temp;  // sfix32_En11
  wire signed [31:0] gain1_1;  // sfix32
  wire signed [31:0] b2mul1;  // sfix32_En12
  wire signed [34:0] adder_add_cast;  // sfix35_En14
  wire signed [34:0] adder_add_cast_1;  // sfix35_En14
  wire signed [34:0] adder_add_temp;  // sfix35_En14
  wire signed [33:0] b2sum1;  // sfix34_En14
  wire signed [31:0] b3mul1;  // sfix32_En12
  wire signed [34:0] adder1_add_cast;  // sfix35_En14
  wire signed [34:0] adder1_add_cast_1;  // sfix35_En14
  wire signed [34:0] adder1_add_temp;  // sfix35_En14
  wire signed [33:0] b1sum1;  // sfix34_En14
  wire signed [15:0] output_typeconvert;  // sfix16_En5
  wire signed [15:0] scaleconst1_1;  // sfix16_En16
  wire signed [31:0] multiplier3_mul_temp;  // sfix32_En21
  wire signed [32:0] scale1_1;  // sfix33_En21
  wire signed [15:0] scaletypeconvert1_1;  // sfix16_En6
  wire signed [33:0] inputconv1_1;  // sfix34_En24
  wire enb_gated_2;
  wire signed [15:0] coeff_a2_section1_1;  // sfix16_En14
  wire enb_gated_3;
  wire signed [15:0] coeff_a3_section1_1;  // sfix16_En14
  reg signed [15:0] typeconvert1_tap0_1;  // sfix16_En6
  reg signed [15:0] typeconvert1_tap1_1;  // sfix16_En6
  wire signed [31:0] multiplier5_mul_temp;  // sfix32_En20
  wire signed [31:0] a3mul1_1;  // sfix32_En22
  wire signed [33:0] a1sum1_cast2_1;  // sfix34_En24
  wire signed [15:0] typeconvert1_1;  // sfix16_En6
  wire signed [31:0] multiplier4_mul_temp;  // sfix32_En20
  wire signed [31:0] a2mul1_1;  // sfix32_En22
  wire signed [33:0] a2sum1_cast2_1;  // sfix34_En24
  wire signed [34:0] Sub2_sub_cast;  // sfix35_En24
  wire signed [34:0] Sub2_sub_cast_1;  // sfix35_En24
  wire signed [34:0] a2sum1_temp_1;  // sfix35_En24
  wire signed [33:0] a2sum1_1;  // sfix34_En24
  wire signed [34:0] Sub3_sub_cast;  // sfix35_En24
  wire signed [34:0] Sub3_sub_cast_1;  // sfix35_En24
  wire signed [34:0] a1sum1_temp_1;  // sfix35_En24
  wire signed [33:0] a1sum1_1;  // sfix34_En24
  wire signed [31:0] b1mul1_1;  // sfix32_En21
  wire signed [33:0] b1multypeconvert1_1;  // sfix34_En22
  wire signed [31:0] gain4_cast;  // sfix32_En19
  wire signed [31:0] b2mul1_1;  // sfix32_En21
  wire signed [34:0] adder2_add_cast;  // sfix35_En22
  wire signed [33:0] adder2_add_cast_1;  // sfix34_En22
  wire signed [34:0] adder2_add_cast_2;  // sfix35_En22
  wire signed [34:0] adder2_add_temp;  // sfix35_En22
  wire signed [33:0] b2sum1_1;  // sfix34_En22
  wire signed [31:0] b3mul1_1;  // sfix32_En21
  wire signed [34:0] adder3_add_cast;  // sfix35_En22
  wire signed [33:0] adder3_add_cast_1;  // sfix34_En22
  wire signed [34:0] adder3_add_cast_2;  // sfix35_En22
  wire signed [34:0] adder3_add_temp;  // sfix35_En22
  wire signed [33:0] b1sum1_1;  // sfix34_En22
  wire signed [15:0] section_result1;  // sfix16_En4
  wire signed [15:0] scaleconst2;  // sfix16_En16
  wire signed [31:0] multiplier6_mul_temp;  // sfix32_En20
  wire signed [32:0] scale2;  // sfix33_En21
  wire signed [15:0] scaletypeconvert2;  // sfix16_En6
  wire signed [33:0] inputconv2;  // sfix34_En24
  wire enb_gated_4;
  wire signed [15:0] coeff_a2_section2;  // sfix16_En14
  wire signed [15:0] a1sumtypeconvert2;  // sfix16_En6
  reg signed [15:0] delay_section2;  // sfix16_En6
  wire signed [31:0] multiplier7_mul_temp;  // sfix32_En20
  wire signed [31:0] a2mul2;  // sfix32_En22
  wire signed [33:0] a1sum2_cast2;  // sfix34_En24
  wire signed [34:0] Sub4_sub_cast;  // sfix35_En24
  wire signed [34:0] Sub4_sub_cast_1;  // sfix35_En24
  wire signed [34:0] a1sum2_temp;  // sfix35_En24
  wire signed [33:0] a1sum2;  // sfix34_En24
  wire signed [31:0] b1mul2;  // sfix32_En21
  wire signed [33:0] b1multypeconvert2;  // sfix34_En22
  wire signed [31:0] b2mul2;  // sfix32_En21
  wire signed [34:0] adder4_add_cast;  // sfix35_En22
  wire signed [33:0] adder4_add_cast_1;  // sfix34_En22
  wire signed [34:0] adder4_add_cast_2;  // sfix35_En22
  wire signed [34:0] adder4_add_temp;  // sfix35_En22
  wire signed [33:0] b1sum2;  // sfix34_En22
  wire signed [15:0] output_typeconvert_1;  // sfix16_En5
  wire [15:0] Constant_out1;  // uint16
  wire signed [22:0] Add_add_cast;  // sfix23_En5
  wire signed [22:0] Add_add_cast_1;  // sfix23_En5
  wire signed [22:0] Add_add_temp;  // sfix23_En5
  wire [15:0] Add_out1;  // uint16
  wire [15:0] Add_out1_bypass;  // uint16
  reg [15:0] Add_out1_last_value;  // uint16


  assign Constant1_out1 = 16'b1000000000000000;



  assign Add1_1 = {1'b0, In};
  assign Add1_2 = {1'b0, Constant1_out1};
  assign Add1_sub_temp = Add1_1 - Add1_2;
  assign Add1_out1 = {Add1_sub_temp[11:0], 4'b0000};



  assign scaleconst1 = 16'sb0111111000011110;



  assign multiplier_mul_temp = Add1_out1 * scaleconst1;
  assign scale1 = {multiplier_mul_temp, 7'b0000000};



  assign scaletypeconvert1 = (((scale1[38] == 1'b0) && (scale1[37] != 1'b0)) || ((scale1[38] == 1'b0) && (scale1[37:22] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((scale1[38] == 1'b1) && (scale1[37] != 1'b1) ? 16'sb1000000000000000 :
              scale1[37:22] + $signed({1'b0, scale1[21] & (scale1[22] | (|scale1[20:0]))})));



  assign inputconv1 = {{8{scaletypeconvert1[15]}}, {scaletypeconvert1, 10'b0000000000}};



  assign enb_gated = Enable && enb;

  assign coeff_a2_section1 = 16'sb1000000111100101;



  assign enb_gated_1 = Enable && enb;

  assign coeff_a3_section1 = 16'sb0011111000100010;



  always @(posedge clk or posedge reset)
    begin : delay_process_section1_tap1_process
      if (reset == 1'b1) begin
        typeconvert1_tap1 <= 16'sb0000000000000000;
      end
      else begin
        if (enb_gated_1) begin
          typeconvert1_tap1 <= typeconvert1_tap0;
        end
      end
    end



  assign multiplier2_mul_temp = typeconvert1_tap1 * coeff_a3_section1;
  assign a3mul1 = ((multiplier2_mul_temp[31] == 1'b0) && (multiplier2_mul_temp[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((multiplier2_mul_temp[31] == 1'b1) && (multiplier2_mul_temp[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {multiplier2_mul_temp[30:0], 1'b0}));



  assign a1sum1_cast2 = {a3mul1, 2'b00};



  always @(posedge clk or posedge reset)
    begin : delay_process_section1_tap0_process
      if (reset == 1'b1) begin
        typeconvert1_tap0 <= 16'sb0000000000000000;
      end
      else begin
        if (enb_gated) begin
          typeconvert1_tap0 <= typeconvert1;
        end
      end
    end



  assign multiplier1_mul_temp = typeconvert1_tap0 * coeff_a2_section1;
  assign a2mul1 = ((multiplier1_mul_temp[31] == 1'b0) && (multiplier1_mul_temp[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((multiplier1_mul_temp[31] == 1'b1) && (multiplier1_mul_temp[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {multiplier1_mul_temp[30:0], 1'b0}));



  assign a2sum1_cast2 = {a2mul1, 2'b00};



  assign Sub_sub_cast = {inputconv1[33], inputconv1};
  assign Sub_sub_cast_1 = {a2sum1_cast2[33], a2sum1_cast2};
  assign a2sum1_temp = Sub_sub_cast - Sub_sub_cast_1;



  assign a2sum1 = ((a2sum1_temp[34] == 1'b0) && (a2sum1_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((a2sum1_temp[34] == 1'b1) && (a2sum1_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(a2sum1_temp[33:0])));



  assign Sub1_sub_cast = {a2sum1[33], a2sum1};
  assign Sub1_sub_cast_1 = {a1sum1_cast2[33], a1sum1_cast2};
  assign a1sum1_temp = Sub1_sub_cast - Sub1_sub_cast_1;



  assign a1sum1 = ((a1sum1_temp[34] == 1'b0) && (a1sum1_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((a1sum1_temp[34] == 1'b1) && (a1sum1_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(a1sum1_temp[33:0])));



  assign typeconvert1 = (((a1sum1[33] == 1'b0) && (a1sum1[32] != 1'b0)) || ((a1sum1[33] == 1'b0) && (a1sum1[32:17] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((a1sum1[33] == 1'b1) && (a1sum1[32] != 1'b1) ? 16'sb1000000000000000 :
              a1sum1[32:17] + $signed({1'b0, a1sum1[16] & (a1sum1[17] | (|a1sum1[15:0]))})));



  // coeff_b1_section1
  assign b1mul1 = {typeconvert1[15], {typeconvert1, 15'b000000000000000}};



  assign b1multypeconvert1 = {b1mul1, 2'b00};



  // coeff_b2_section1
  // CSD Encoding (32768) : 1000000000000000; Cost (Adders) = 0
  assign gain1_1 = {typeconvert1_tap0[15], {typeconvert1_tap0, 15'b000000000000000}};
  assign gain1_mul_temp =  - (gain1_1);
  assign b2mul1 = ((gain1_mul_temp[31] == 1'b0) && (gain1_mul_temp[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((gain1_mul_temp[31] == 1'b1) && (gain1_mul_temp[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {gain1_mul_temp[30:0], 1'b0}));



  assign adder_add_cast = {b1multypeconvert1[33], b1multypeconvert1};
  assign adder_add_cast_1 = {b2mul1[31], {b2mul1, 2'b00}};
  assign adder_add_temp = adder_add_cast + adder_add_cast_1;
  assign b2sum1 = ((adder_add_temp[34] == 1'b0) && (adder_add_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((adder_add_temp[34] == 1'b1) && (adder_add_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(adder_add_temp[33:0])));



  // coeff_b3_section1
  assign b3mul1 = {typeconvert1_tap1[15], {typeconvert1_tap1, 15'b000000000000000}};



  assign adder1_add_cast = {b2sum1[33], b2sum1};
  assign adder1_add_cast_1 = {b3mul1[31], {b3mul1, 2'b00}};
  assign adder1_add_temp = adder1_add_cast + adder1_add_cast_1;
  assign b1sum1 = ((adder1_add_temp[34] == 1'b0) && (adder1_add_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((adder1_add_temp[34] == 1'b1) && (adder1_add_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(adder1_add_temp[33:0])));



  assign output_typeconvert = (((b1sum1[33] == 1'b0) && (b1sum1[32:24] != 9'b000000000)) || ((b1sum1[33] == 1'b0) && (b1sum1[24:9] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((b1sum1[33] == 1'b1) && (b1sum1[32:24] != 9'b111111111) ? 16'sb1000000000000000 :
              b1sum1[24:9] + $signed({1'b0, b1sum1[8] & (b1sum1[9] | (|b1sum1[7:0]))})));



  assign scaleconst1_1 = 16'sb0001101011110111;



  assign multiplier3_mul_temp = output_typeconvert * scaleconst1_1;
  assign scale1_1 = {multiplier3_mul_temp[31], multiplier3_mul_temp};



  assign scaletypeconvert1_1 = (((scale1_1[32] == 1'b0) && (scale1_1[31:30] != 2'b00)) || ((scale1_1[32] == 1'b0) && (scale1_1[30:15] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((scale1_1[32] == 1'b1) && (scale1_1[31:30] != 2'b11) ? 16'sb1000000000000000 :
              scale1_1[30:15] + $signed({1'b0, scale1_1[14] & (scale1_1[15] | (|scale1_1[13:0]))})));



  assign inputconv1_1 = {scaletypeconvert1_1, 18'b000000000000000000};



  assign enb_gated_2 = Enable && enb;

  assign coeff_a2_section1_1 = 16'sb1011110000010111;



  assign enb_gated_3 = Enable && enb;

  assign coeff_a3_section1_1 = 16'sb0001111011100000;



  always @(posedge clk or posedge reset)
    begin : delay_process_section1_tap1_1_process
      if (reset == 1'b1) begin
        typeconvert1_tap1_1 <= 16'sb0000000000000000;
      end
      else begin
        if (enb_gated_3) begin
          typeconvert1_tap1_1 <= typeconvert1_tap0_1;
        end
      end
    end



  assign multiplier5_mul_temp = typeconvert1_tap1_1 * coeff_a3_section1_1;
  assign a3mul1_1 = ((multiplier5_mul_temp[31] == 1'b0) && (multiplier5_mul_temp[30:29] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((multiplier5_mul_temp[31] == 1'b1) && (multiplier5_mul_temp[30:29] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              {multiplier5_mul_temp[29:0], 2'b00}));



  assign a1sum1_cast2_1 = {a3mul1_1, 2'b00};



  always @(posedge clk or posedge reset)
    begin : delay_process_section1_tap0_1_process
      if (reset == 1'b1) begin
        typeconvert1_tap0_1 <= 16'sb0000000000000000;
      end
      else begin
        if (enb_gated_2) begin
          typeconvert1_tap0_1 <= typeconvert1_1;
        end
      end
    end



  assign multiplier4_mul_temp = typeconvert1_tap0_1 * coeff_a2_section1_1;
  assign a2mul1_1 = ((multiplier4_mul_temp[31] == 1'b0) && (multiplier4_mul_temp[30:29] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((multiplier4_mul_temp[31] == 1'b1) && (multiplier4_mul_temp[30:29] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              {multiplier4_mul_temp[29:0], 2'b00}));



  assign a2sum1_cast2_1 = {a2mul1_1, 2'b00};



  assign Sub2_sub_cast = {inputconv1_1[33], inputconv1_1};
  assign Sub2_sub_cast_1 = {a2sum1_cast2_1[33], a2sum1_cast2_1};
  assign a2sum1_temp_1 = Sub2_sub_cast - Sub2_sub_cast_1;



  assign a2sum1_1 = ((a2sum1_temp_1[34] == 1'b0) && (a2sum1_temp_1[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((a2sum1_temp_1[34] == 1'b1) && (a2sum1_temp_1[33] != 1'b1) ? 34'sh200000000 :
              $signed(a2sum1_temp_1[33:0])));



  assign Sub3_sub_cast = {a2sum1_1[33], a2sum1_1};
  assign Sub3_sub_cast_1 = {a1sum1_cast2_1[33], a1sum1_cast2_1};
  assign a1sum1_temp_1 = Sub3_sub_cast - Sub3_sub_cast_1;



  assign a1sum1_1 = ((a1sum1_temp_1[34] == 1'b0) && (a1sum1_temp_1[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((a1sum1_temp_1[34] == 1'b1) && (a1sum1_temp_1[33] != 1'b1) ? 34'sh200000000 :
              $signed(a1sum1_temp_1[33:0])));



  assign typeconvert1_1 = ((a1sum1_1[33] == 1'b0) && (a1sum1_1[32:18] == 15'b111111111111111) ? 16'sb0111111111111111 :
              a1sum1_1[33:18] + $signed({1'b0, a1sum1_1[17] & (a1sum1_1[18] | (|a1sum1_1[16:0]))}));



  // coeff_b1_section1
  assign b1mul1_1 = {typeconvert1_1[15], {typeconvert1_1, 15'b000000000000000}};



  assign b1multypeconvert1_1 = {b1mul1_1[31], {b1mul1_1, 1'b0}};



  // coeff_b2_section1
  assign gain4_cast = {{2{typeconvert1_tap0_1[15]}}, {typeconvert1_tap0_1, 14'b00000000000000}};
  assign b2mul1_1 = ((gain4_cast[31] == 1'b0) && (gain4_cast[30:29] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((gain4_cast[31] == 1'b1) && (gain4_cast[30:29] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              {gain4_cast[29:0], 2'b00}));



  assign adder2_add_cast = {b1multypeconvert1_1[33], b1multypeconvert1_1};
  assign adder2_add_cast_1 = {b2mul1_1[31], {b2mul1_1, 1'b0}};
  assign adder2_add_cast_2 = {adder2_add_cast_1[33], adder2_add_cast_1};
  assign adder2_add_temp = adder2_add_cast + adder2_add_cast_2;
  assign b2sum1_1 = ((adder2_add_temp[34] == 1'b0) && (adder2_add_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((adder2_add_temp[34] == 1'b1) && (adder2_add_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(adder2_add_temp[33:0])));



  // coeff_b3_section1
  assign b3mul1_1 = {typeconvert1_tap1_1[15], {typeconvert1_tap1_1, 15'b000000000000000}};



  assign adder3_add_cast = {b2sum1_1[33], b2sum1_1};
  assign adder3_add_cast_1 = {b3mul1_1[31], {b3mul1_1, 1'b0}};
  assign adder3_add_cast_2 = {adder3_add_cast_1[33], adder3_add_cast_1};
  assign adder3_add_temp = adder3_add_cast + adder3_add_cast_2;
  assign b1sum1_1 = ((adder3_add_temp[34] == 1'b0) && (adder3_add_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((adder3_add_temp[34] == 1'b1) && (adder3_add_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(adder3_add_temp[33:0])));



  assign section_result1 = ((b1sum1_1[33] == 1'b0) && (b1sum1_1[32:18] == 15'b111111111111111) ? 16'sb0111111111111111 :
              b1sum1_1[33:18] + $signed({1'b0, b1sum1_1[17] & (b1sum1_1[18] | (|b1sum1_1[16:0]))}));



  assign scaleconst2 = 16'sb0100101000001101;



  assign multiplier6_mul_temp = section_result1 * scaleconst2;
  assign scale2 = {multiplier6_mul_temp, 1'b0};



  assign scaletypeconvert2 = (((scale2[32] == 1'b0) && (scale2[31:30] != 2'b00)) || ((scale2[32] == 1'b0) && (scale2[30:15] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((scale2[32] == 1'b1) && (scale2[31:30] != 2'b11) ? 16'sb1000000000000000 :
              scale2[30:15] + $signed({1'b0, scale2[14] & (scale2[15] | (|scale2[13:0]))})));



  assign inputconv2 = {scaletypeconvert2, 18'b000000000000000000};



  assign enb_gated_4 = Enable && enb;

  assign coeff_a2_section2 = 16'sb1110010100000111;



  always @(posedge clk or posedge reset)
    begin : delay_process_section2_process
      if (reset == 1'b1) begin
        delay_section2 <= 16'sb0000000000000000;
      end
      else begin
        if (enb_gated_4) begin
          delay_section2 <= a1sumtypeconvert2;
        end
      end
    end



  assign multiplier7_mul_temp = delay_section2 * coeff_a2_section2;
  assign a2mul2 = ((multiplier7_mul_temp[31] == 1'b0) && (multiplier7_mul_temp[30:29] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((multiplier7_mul_temp[31] == 1'b1) && (multiplier7_mul_temp[30:29] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              {multiplier7_mul_temp[29:0], 2'b00}));



  assign a1sum2_cast2 = {a2mul2, 2'b00};



  assign Sub4_sub_cast = {inputconv2[33], inputconv2};
  assign Sub4_sub_cast_1 = {a1sum2_cast2[33], a1sum2_cast2};
  assign a1sum2_temp = Sub4_sub_cast - Sub4_sub_cast_1;



  assign a1sum2 = ((a1sum2_temp[34] == 1'b0) && (a1sum2_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((a1sum2_temp[34] == 1'b1) && (a1sum2_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(a1sum2_temp[33:0])));



  assign a1sumtypeconvert2 = ((a1sum2[33] == 1'b0) && (a1sum2[32:18] == 15'b111111111111111) ? 16'sb0111111111111111 :
              a1sum2[33:18] + $signed({1'b0, a1sum2[17] & (a1sum2[18] | (|a1sum2[16:0]))}));



  // coeff_b1_section2
  assign b1mul2 = {a1sumtypeconvert2[15], {a1sumtypeconvert2, 15'b000000000000000}};



  assign b1multypeconvert2 = {b1mul2[31], {b1mul2, 1'b0}};



  // coeff_b2_section2
  assign b2mul2 = {delay_section2[15], {delay_section2, 15'b000000000000000}};



  assign adder4_add_cast = {b1multypeconvert2[33], b1multypeconvert2};
  assign adder4_add_cast_1 = {b2mul2[31], {b2mul2, 1'b0}};
  assign adder4_add_cast_2 = {adder4_add_cast_1[33], adder4_add_cast_1};
  assign adder4_add_temp = adder4_add_cast + adder4_add_cast_2;
  assign b1sum2 = ((adder4_add_temp[34] == 1'b0) && (adder4_add_temp[33] != 1'b0) ? 34'sh1FFFFFFFF :
              ((adder4_add_temp[34] == 1'b1) && (adder4_add_temp[33] != 1'b1) ? 34'sh200000000 :
              $signed(adder4_add_temp[33:0])));



  assign output_typeconvert_1 = (((b1sum2[33] == 1'b0) && (b1sum2[32] != 1'b0)) || ((b1sum2[33] == 1'b0) && (b1sum2[32:17] == 16'sb0111111111111111)) ? 16'sb0111111111111111 :
              ((b1sum2[33] == 1'b1) && (b1sum2[32] != 1'b1) ? 16'sb1000000000000000 :
              b1sum2[32:17] + $signed({1'b0, b1sum2[16] & (b1sum2[17] | (|b1sum2[15:0]))})));



  assign Constant_out1 = 16'b1000000000000000;



  assign Add_add_cast = {{7{output_typeconvert_1[15]}}, output_typeconvert_1};
  assign Add_add_cast_1 = {2'b0, {Constant_out1, 5'b00000}};
  assign Add_add_temp = Add_add_cast + Add_add_cast_1;
  assign Add_out1 = Add_add_temp[20:5];



  always @(posedge clk or posedge reset)
    begin : Out_bypass_process
      if (reset == 1'b1) begin
        Add_out1_last_value <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          Add_out1_last_value <= Add_out1_bypass;
        end
      end
    end



  assign Add_out1_bypass = (Enable == 1'b0 ? Add_out1_last_value :
              Add_out1);



  assign Out = Add_out1_bypass;

endmodule  // bandpass
