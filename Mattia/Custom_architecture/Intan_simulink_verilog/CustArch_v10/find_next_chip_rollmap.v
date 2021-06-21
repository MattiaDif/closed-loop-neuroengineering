// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v10\find_next_chip_rollmap.v
// Created: 2021-06-18 10:46:14
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: find_next_chip_rollmap
// Source Path: CustArch_v10/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/get_sources_and_RAM_addresses/find_next_chip_rollma
// Hierarchy Level: 5
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module find_next_chip_rollmap
          (clk,
           reset,
           enb,
           curr_chip_rollmap,
           source_chip,
           Enable,
           bit_masked);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] curr_chip_rollmap;  // uint8
  input   [7:0] source_chip;  // uint8
  input   Enable;
  output  [7:0] bit_masked;  // uint8


  wire enb_gated;
  wire Compare_To_Constant7_out1;
  wire switch_compare_1;
  wire [7:0] Constant8_out1;  // uint8
  wire Compare_To_Constant6_out1;
  wire switch_compare_1_1;
  wire [7:0] Constant7_out1;  // uint8
  wire [7:0] Switch7_out1;  // uint8
  wire Compare_To_Constant5_out1;
  wire switch_compare_1_2;
  wire [7:0] Constant6_out1;  // uint8
  wire [7:0] Switch6_out1;  // uint8
  wire Compare_To_Constant4_out1;
  wire switch_compare_1_3;
  wire [7:0] Constant5_out1;  // uint8
  wire [7:0] Switch5_out1;  // uint8
  wire Compare_To_Constant3_out1;
  wire switch_compare_1_4;
  wire [7:0] Constant4_out1;  // uint8
  wire [7:0] Switch4_out1;  // uint8
  wire Compare_To_Constant2_out1;
  wire switch_compare_1_5;
  wire [7:0] Constant3_out1;  // uint8
  wire [7:0] Switch3_out1;  // uint8
  wire Compare_To_Constant1_out1;
  wire switch_compare_1_6;
  wire [7:0] Constant2_out1;  // uint8
  wire [7:0] Switch2_out1;  // uint8
  wire Compare_To_Constant_out1;
  wire switch_compare_1_7;
  wire [7:0] Constant1_out1;  // uint8
  wire [7:0] Switch1_out1;  // uint8
  wire [7:0] Constant_out1;  // uint8
  wire [7:0] Switch_out1;  // uint8
  wire [7:0] bitmask;  // uint8
  wire [7:0] next_chip_rollmap;  // uint8
  wire [7:0] next_chip_rollmap_bypass;  // uint8
  reg [7:0] next_chip_rollmap_last_value;  // uint8

  // Basically, by means of these bitwise operation, the next_chip_rollmap is computed by removing the 1 in the position 
  // corresponding to th
  // variable source_chip. Example:
  // 
  // InputState == 0
  // curr_chip_rollmap == data_stream_enable == 00010010
  // source_chip == 1 --> 00010010
  // 
  // --> the output of the switch cascade is 2, then 00000010 --> bitwise NOT --> 11111101
  // 
  // bitwise AND between 00010010 and 11111101 --> 00010000 --> I have removed the 1 corresponding to the position 2! 
  // --> ready for the next chip rollmap
  // 
  // let's simplify this in a matlab function!


  assign enb_gated = Enable && enb;

  assign Compare_To_Constant7_out1 = source_chip == 8'b00000111;



  assign switch_compare_1 = Compare_To_Constant7_out1 > 1'b0;



  assign Constant8_out1 = 8'b11111111;



  assign Compare_To_Constant6_out1 = source_chip == 8'b00000110;



  assign switch_compare_1_1 = Compare_To_Constant6_out1 > 1'b0;



  assign Constant7_out1 = 8'b10000000;



  assign Switch7_out1 = (switch_compare_1 == 1'b0 ? Constant8_out1 :
              Constant7_out1);



  assign Compare_To_Constant5_out1 = source_chip == 8'b00000101;



  assign switch_compare_1_2 = Compare_To_Constant5_out1 > 1'b0;



  assign Constant6_out1 = 8'b01000000;



  assign Switch6_out1 = (switch_compare_1_1 == 1'b0 ? Switch7_out1 :
              Constant6_out1);



  assign Compare_To_Constant4_out1 = source_chip == 8'b00000100;



  assign switch_compare_1_3 = Compare_To_Constant4_out1 > 1'b0;



  assign Constant5_out1 = 8'b00100000;



  assign Switch5_out1 = (switch_compare_1_2 == 1'b0 ? Switch6_out1 :
              Constant5_out1);



  assign Compare_To_Constant3_out1 = source_chip == 8'b00000011;



  assign switch_compare_1_4 = Compare_To_Constant3_out1 > 1'b0;



  assign Constant4_out1 = 8'b00010000;



  assign Switch4_out1 = (switch_compare_1_3 == 1'b0 ? Switch5_out1 :
              Constant4_out1);



  assign Compare_To_Constant2_out1 = source_chip == 8'b00000010;



  assign switch_compare_1_5 = Compare_To_Constant2_out1 > 1'b0;



  assign Constant3_out1 = 8'b00001000;



  assign Switch3_out1 = (switch_compare_1_4 == 1'b0 ? Switch4_out1 :
              Constant3_out1);



  assign Compare_To_Constant1_out1 = source_chip == 8'b00000001;



  assign switch_compare_1_6 = Compare_To_Constant1_out1 > 1'b0;



  assign Constant2_out1 = 8'b00000100;



  assign Switch2_out1 = (switch_compare_1_5 == 1'b0 ? Switch3_out1 :
              Constant2_out1);



  assign Compare_To_Constant_out1 = source_chip == 8'b00000000;



  assign switch_compare_1_7 = Compare_To_Constant_out1 > 1'b0;



  assign Constant1_out1 = 8'b00000010;



  assign Switch1_out1 = (switch_compare_1_6 == 1'b0 ? Switch2_out1 :
              Constant1_out1);



  assign Constant_out1 = 8'b00000001;



  assign Switch_out1 = (switch_compare_1_7 == 1'b0 ? Switch1_out1 :
              Constant_out1);



  assign bitmask =  ~ Switch_out1;



  assign next_chip_rollmap = curr_chip_rollmap & bitmask;



  always @(posedge clk or posedge reset)
    begin : bit_masked_bypass_process
      if (reset == 1'b1) begin
        next_chip_rollmap_last_value <= 8'b00000000;
      end
      else begin
        if (enb_gated) begin
          next_chip_rollmap_last_value <= next_chip_rollmap_bypass;
        end
      end
    end



  assign next_chip_rollmap_bypass = (Enable == 1'b0 ? next_chip_rollmap_last_value :
              next_chip_rollmap);



  assign bit_masked = next_chip_rollmap_bypass;

endmodule  // find_next_chip_rollmap

