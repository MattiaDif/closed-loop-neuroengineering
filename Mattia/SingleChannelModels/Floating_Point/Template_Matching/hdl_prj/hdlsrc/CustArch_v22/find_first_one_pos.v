// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\find_first_one_pos.v
// Created: 2021-09-20 15:12:27
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: find_first_one_pos
// Source Path: CustArch_v22/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/get_sources_and_RAM_addresses/find_first_one_po
// Hierarchy Level: 5
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module find_first_one_pos
          (clk,
           reset,
           enb,
           In1,
           Enable,
           first_one_pos);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] In1;  // uint8
  input   Enable;
  output  [7:0] first_one_pos;  // uint8


  wire enb_gated;
  wire Bit_Slice7_out1;  // ufix1
  wire Compare_To_Constant7_out1;
  wire switch_compare_1;
  wire [7:0] Constant8_out1;  // uint8
  wire Bit_Slice6_out1;  // ufix1
  wire Compare_To_Constant6_out1;
  wire switch_compare_1_1;
  wire [7:0] Constant7_out1;  // uint8
  wire [7:0] Switch7_out1;  // uint8
  wire Bit_Slice5_out1;  // ufix1
  wire Compare_To_Constant5_out1;
  wire switch_compare_1_2;
  wire [7:0] Constant6_out1;  // uint8
  wire [7:0] Switch6_out1;  // uint8
  wire Bit_Slice4_out1;  // ufix1
  wire Compare_To_Constant4_out1;
  wire switch_compare_1_3;
  wire [7:0] Constant5_out1;  // uint8
  wire [7:0] Switch5_out1;  // uint8
  wire Bit_Slice3_out1;  // ufix1
  wire Compare_To_Constant3_out1;
  wire switch_compare_1_4;
  wire [7:0] Constant4_out1;  // uint8
  wire [7:0] Switch4_out1;  // uint8
  wire Bit_Slice2_out1;  // ufix1
  wire Compare_To_Constant2_out1;
  wire switch_compare_1_5;
  wire [7:0] Constant3_out1;  // uint8
  wire [7:0] Switch3_out1;  // uint8
  wire Bit_Slice1_out1;  // ufix1
  wire Compare_To_Constant1_out1;
  wire switch_compare_1_6;
  wire [7:0] Constant2_out1;  // uint8
  wire [7:0] Switch2_out1;  // uint8
  wire Bit_Slice_out1;  // ufix1
  wire Compare_To_Constant_out1;
  wire switch_compare_1_7;
  wire [7:0] Constant1_out1;  // uint8
  wire [7:0] Switch1_out1;  // uint8
  wire [7:0] Constant_out1;  // uint8
  wire [7:0] Switch_out1;  // uint8
  wire [7:0] Switch_out1_bypass;  // uint8
  reg [7:0] Switch_out1_last_value;  // uint8

  // ugly!! need to find something better


  assign enb_gated = Enable && enb;

  assign Bit_Slice7_out1 = In1[7];



  assign Compare_To_Constant7_out1 = Bit_Slice7_out1 == 1'b1;



  assign switch_compare_1 = Compare_To_Constant7_out1 > 1'b0;



  assign Constant8_out1 = 8'b00001000;



  assign Bit_Slice6_out1 = In1[6];



  assign Compare_To_Constant6_out1 = Bit_Slice6_out1 == 1'b1;



  assign switch_compare_1_1 = Compare_To_Constant6_out1 > 1'b0;



  assign Constant7_out1 = 8'b00000111;



  assign Switch7_out1 = (switch_compare_1 == 1'b0 ? Constant8_out1 :
              Constant7_out1);



  assign Bit_Slice5_out1 = In1[5];



  assign Compare_To_Constant5_out1 = Bit_Slice5_out1 == 1'b1;



  assign switch_compare_1_2 = Compare_To_Constant5_out1 > 1'b0;



  assign Constant6_out1 = 8'b00000110;



  assign Switch6_out1 = (switch_compare_1_1 == 1'b0 ? Switch7_out1 :
              Constant6_out1);



  assign Bit_Slice4_out1 = In1[4];



  assign Compare_To_Constant4_out1 = Bit_Slice4_out1 == 1'b1;



  assign switch_compare_1_3 = Compare_To_Constant4_out1 > 1'b0;



  assign Constant5_out1 = 8'b00000101;



  assign Switch5_out1 = (switch_compare_1_2 == 1'b0 ? Switch6_out1 :
              Constant5_out1);



  assign Bit_Slice3_out1 = In1[3];



  assign Compare_To_Constant3_out1 = Bit_Slice3_out1 == 1'b1;



  assign switch_compare_1_4 = Compare_To_Constant3_out1 > 1'b0;



  assign Constant4_out1 = 8'b00000100;



  assign Switch4_out1 = (switch_compare_1_3 == 1'b0 ? Switch5_out1 :
              Constant4_out1);



  assign Bit_Slice2_out1 = In1[2];



  assign Compare_To_Constant2_out1 = Bit_Slice2_out1 == 1'b1;



  assign switch_compare_1_5 = Compare_To_Constant2_out1 > 1'b0;



  assign Constant3_out1 = 8'b00000011;



  assign Switch3_out1 = (switch_compare_1_4 == 1'b0 ? Switch4_out1 :
              Constant3_out1);



  assign Bit_Slice1_out1 = In1[1];



  assign Compare_To_Constant1_out1 = Bit_Slice1_out1 == 1'b1;



  assign switch_compare_1_6 = Compare_To_Constant1_out1 > 1'b0;



  assign Constant2_out1 = 8'b00000010;



  assign Switch2_out1 = (switch_compare_1_5 == 1'b0 ? Switch3_out1 :
              Constant2_out1);



  assign Bit_Slice_out1 = In1[0];



  assign Compare_To_Constant_out1 = Bit_Slice_out1 == 1'b1;



  assign switch_compare_1_7 = Compare_To_Constant_out1 > 1'b0;



  assign Constant1_out1 = 8'b00000001;



  assign Switch1_out1 = (switch_compare_1_6 == 1'b0 ? Switch2_out1 :
              Constant1_out1);



  assign Constant_out1 = 8'b00000000;



  assign Switch_out1 = (switch_compare_1_7 == 1'b0 ? Switch1_out1 :
              Constant_out1);



  always @(posedge clk or posedge reset)
    begin : first_one_pos_bypass_process
      if (reset == 1'b1) begin
        Switch_out1_last_value <= 8'b00000000;
      end
      else begin
        if (enb_gated) begin
          Switch_out1_last_value <= Switch_out1_bypass;
        end
      end
    end



  assign Switch_out1_bypass = (Enable == 1'b0 ? Switch_out1_last_value :
              Switch_out1);



  assign first_one_pos = Switch_out1_bypass;

endmodule  // find_first_one_pos

