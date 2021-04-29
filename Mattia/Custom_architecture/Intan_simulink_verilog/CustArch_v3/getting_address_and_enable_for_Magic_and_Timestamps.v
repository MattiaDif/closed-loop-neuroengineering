// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v3\getting_address_and_enable_for_Magic_and_Timestamps.v
// Created: 2021-04-28 17:35:59
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: getting_address_and_enable_for_Magic_and_Timestamps
// Source Path: CustArch_v3/cust_architecture/collect_output/read_from_magic_and_timestamp/getting address and enable 
// for Magic and Timestamp
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module getting_address_and_enable_for_Magic_and_Timestamps
          (InputState,
           FIFO_write_to,
           wr_address,
           wr_enable);


  input   [7:0] InputState;  // uint8
  input   FIFO_write_to;
  output  [7:0] wr_address;  // uint8
  output  wr_enable;


  wire Compare_To_Constant11_out1;
  wire switch_compare_1;
  wire [7:0] Constant13_out1;  // uint8
  wire Compare_To_Constant10_out1;
  wire switch_compare_1_1;
  wire [7:0] Constant12_out1;  // uint8
  wire [7:0] Switch11_out1;  // uint8
  wire Compare_To_Constant9_out1;
  wire switch_compare_1_2;
  wire [7:0] Constant11_out1;  // uint8
  wire [7:0] Switch10_out1;  // uint8
  wire Compare_To_Constant8_out1;
  wire switch_compare_1_3;
  wire [7:0] Constant10_out1;  // uint8
  wire [7:0] Switch9_out1;  // uint8
  wire Compare_To_Constant7_out1;
  wire switch_compare_1_4;
  wire [7:0] Constant9_out1;  // uint8
  wire [7:0] Switch8_out1;  // uint8
  wire Compare_To_Constant6_out1;
  wire switch_compare_1_5;
  wire [7:0] Constant8_out1;  // uint8
  wire [7:0] Switch7_out1;  // uint8
  wire [7:0] Constant7_out1;  // uint8
  wire [7:0] Switch6_out1;  // uint8
  wire Compare_To_Constant12_out1;
  wire AND1_out1;

  // this associates a RAM address to the magic number and timestamp
  // data according to the InputState
  // 
  // InputState from 0 to 5 for magic number and timestamp
  // 
  // LET�S REPLACE THIS SHIT (WHICH WORKS) WITH A MATLAB FUNCTION
  // 
  // enable the RAM writing if the InputState is between 0 and 5


  assign Compare_To_Constant11_out1 = InputState == 8'b00000101;



  assign switch_compare_1 = Compare_To_Constant11_out1 > 1'b0;



  assign Constant13_out1 = 8'b00000110;



  assign Compare_To_Constant10_out1 = InputState == 8'b00000100;



  assign switch_compare_1_1 = Compare_To_Constant10_out1 > 1'b0;



  assign Constant12_out1 = 8'b00000101;



  assign Switch11_out1 = (switch_compare_1 == 1'b0 ? Constant13_out1 :
              Constant12_out1);



  assign Compare_To_Constant9_out1 = InputState == 8'b00000011;



  assign switch_compare_1_2 = Compare_To_Constant9_out1 > 1'b0;



  assign Constant11_out1 = 8'b00000100;



  assign Switch10_out1 = (switch_compare_1_1 == 1'b0 ? Switch11_out1 :
              Constant11_out1);



  assign Compare_To_Constant8_out1 = InputState == 8'b00000010;



  assign switch_compare_1_3 = Compare_To_Constant8_out1 > 1'b0;



  assign Constant10_out1 = 8'b00000011;



  assign Switch9_out1 = (switch_compare_1_2 == 1'b0 ? Switch10_out1 :
              Constant10_out1);



  assign Compare_To_Constant7_out1 = InputState == 8'b00000001;



  assign switch_compare_1_4 = Compare_To_Constant7_out1 > 1'b0;



  assign Constant9_out1 = 8'b00000010;



  assign Switch8_out1 = (switch_compare_1_3 == 1'b0 ? Switch9_out1 :
              Constant9_out1);



  assign Compare_To_Constant6_out1 = InputState == 8'b00000000;



  assign switch_compare_1_5 = Compare_To_Constant6_out1 > 1'b0;



  assign Constant8_out1 = 8'b00000001;



  assign Switch7_out1 = (switch_compare_1_4 == 1'b0 ? Switch8_out1 :
              Constant8_out1);



  assign Constant7_out1 = 8'b00000000;



  assign Switch6_out1 = (switch_compare_1_5 == 1'b0 ? Switch7_out1 :
              Constant7_out1);



  assign wr_address = Switch6_out1;

  assign Compare_To_Constant12_out1 = InputState <= 8'b00000101;



  assign AND1_out1 = Compare_To_Constant12_out1 & FIFO_write_to;



  assign wr_enable = AND1_out1;

endmodule  // getting_address_and_enable_for_Magic_and_Timestamps

