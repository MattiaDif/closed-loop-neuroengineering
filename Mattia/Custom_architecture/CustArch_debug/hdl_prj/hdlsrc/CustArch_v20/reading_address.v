// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v20\reading_address.v
// Created: 2021-07-15 12:17:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: reading_address
// Source Path: CustArch_v20/cust_architecture/output_interpret/Interpret_to_RAM_output/get_enabled_timings /reading_address
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module reading_address
          (counter,
           data_stream_en,
           source_device_en,
           source_device);


  input   [7:0] counter;  // uint8
  input   [7:0] data_stream_en;  // uint8
  output  source_device_en;  // ufix1
  output  [7:0] source_device;  // uint8


  wire Compare_To_Constant7_out1;
  wire switch_compare_1;
  wire [7:0] Constant_out1;  // uint8
  wire Compare_To_Constant6_out1;
  wire switch_compare_1_1;
  wire [7:0] Bit_Rotate6_out1;  // uint8
  wire [7:0] Switch7_out1;  // uint8
  wire Compare_To_Constant5_out1;
  wire switch_compare_1_2;
  wire [7:0] Bit_Rotate5_out1;  // uint8
  wire [7:0] Switch6_out1;  // uint8
  wire Compare_To_Constant4_out1;
  wire switch_compare_1_3;
  wire [7:0] Bit_Rotate4_out1;  // uint8
  wire [7:0] Switch5_out1;  // uint8
  wire Compare_To_Constant3_out1;
  wire switch_compare_1_4;
  wire [7:0] Bit_Rotate3_out1;  // uint8
  wire [7:0] Switch4_out1;  // uint8
  wire Compare_To_Constant2_out1;
  wire switch_compare_1_5;
  wire [7:0] Bit_Rotate2_out1;  // uint8
  wire [7:0] Switch3_out1;  // uint8
  wire Compare_To_Constant1_out1;
  wire switch_compare_1_6;
  wire [7:0] Bit_Rotate1_out1;  // uint8
  wire [7:0] Switch2_out1;  // uint8
  wire Compare_To_Constant_out1;
  wire switch_compare_1_7;
  wire [7:0] Bit_Rotate_out1;  // uint8
  wire [7:0] Switch1_out1;  // uint8
  wire [7:0] Switch_out1;  // uint8
  wire Bit_Slice_out1;  // ufix1

  // this is the ugliest thing ever--- but it works.. so far so good


  assign Compare_To_Constant7_out1 = counter == 8'b00000111;



  assign switch_compare_1 = Compare_To_Constant7_out1 > 1'b0;



  assign Constant_out1 = 8'b00000000;



  assign Compare_To_Constant6_out1 = counter == 8'b00000110;



  assign switch_compare_1_1 = Compare_To_Constant6_out1 > 1'b0;



  assign Bit_Rotate6_out1 = {data_stream_en[6:0], data_stream_en[7]};



  assign Switch7_out1 = (switch_compare_1 == 1'b0 ? Constant_out1 :
              Bit_Rotate6_out1);



  assign Compare_To_Constant5_out1 = counter == 8'b00000101;



  assign switch_compare_1_2 = Compare_To_Constant5_out1 > 1'b0;



  assign Bit_Rotate5_out1 = {data_stream_en[5:0], data_stream_en[7:6]};



  assign Switch6_out1 = (switch_compare_1_1 == 1'b0 ? Switch7_out1 :
              Bit_Rotate5_out1);



  assign Compare_To_Constant4_out1 = counter == 8'b00000100;



  assign switch_compare_1_3 = Compare_To_Constant4_out1 > 1'b0;



  assign Bit_Rotate4_out1 = {data_stream_en[4:0], data_stream_en[7:5]};



  assign Switch5_out1 = (switch_compare_1_2 == 1'b0 ? Switch6_out1 :
              Bit_Rotate4_out1);



  assign Compare_To_Constant3_out1 = counter == 8'b00000011;



  assign switch_compare_1_4 = Compare_To_Constant3_out1 > 1'b0;



  assign Bit_Rotate3_out1 = {data_stream_en[3:0], data_stream_en[7:4]};



  assign Switch4_out1 = (switch_compare_1_3 == 1'b0 ? Switch5_out1 :
              Bit_Rotate3_out1);



  assign Compare_To_Constant2_out1 = counter == 8'b00000010;



  assign switch_compare_1_5 = Compare_To_Constant2_out1 > 1'b0;



  assign Bit_Rotate2_out1 = {data_stream_en[2:0], data_stream_en[7:3]};



  assign Switch3_out1 = (switch_compare_1_4 == 1'b0 ? Switch4_out1 :
              Bit_Rotate2_out1);



  assign Compare_To_Constant1_out1 = counter == 8'b00000001;



  assign switch_compare_1_6 = Compare_To_Constant1_out1 > 1'b0;



  assign Bit_Rotate1_out1 = {data_stream_en[1:0], data_stream_en[7:2]};



  assign Switch2_out1 = (switch_compare_1_5 == 1'b0 ? Switch3_out1 :
              Bit_Rotate1_out1);



  assign Compare_To_Constant_out1 = counter == 8'b00000000;



  assign switch_compare_1_7 = Compare_To_Constant_out1 > 1'b0;



  assign Bit_Rotate_out1 = {data_stream_en[0], data_stream_en[7:1]};



  assign Switch1_out1 = (switch_compare_1_6 == 1'b0 ? Switch2_out1 :
              Bit_Rotate_out1);



  assign Switch_out1 = (switch_compare_1_7 == 1'b0 ? Switch1_out1 :
              data_stream_en);



  assign Bit_Slice_out1 = Switch_out1[0];



  assign source_device_en = Bit_Slice_out1;

  assign source_device = counter;

endmodule  // reading_address

