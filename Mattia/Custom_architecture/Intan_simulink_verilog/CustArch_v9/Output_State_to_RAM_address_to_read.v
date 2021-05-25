// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v9\Output_State_to_RAM_address_to_read.v
// Created: 2021-05-25 14:12:37
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Output_State_to_RAM_address_to_read
// Source Path: CustArch_v9/cust_architecture/output_interpret/Interpret_to_RAM_output/Output_State_to_RAM_address 
// (to read
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Output_State_to_RAM_address_to_read
          (source_channel_en,
           source_channel_0_159,
           OutputState,
           source_device_en,
           source_device,
           converter_cnt,
           rd_enable,
           rd_addr);


  input   source_channel_en;
  input   [7:0] source_channel_0_159;  // uint8
  input   [7:0] OutputState;  // uint8
  input   source_device_en;  // ufix1
  input   [2:0] source_device;  // ufix3
  input   [2:0] converter_cnt;  // ufix3
  output  rd_enable;
  output  [8:0] rd_addr;  // ufix9


  wire DC_out1;
  wire AC_out1;
  wire OR_out1;
  wire Stim_stuff_out1;
  wire Stim_stuffff_out1;
  wire AND_out1;
  wire switch_compare_1;
  wire Constant12_out1;
  wire switch_compare_1_1;
  wire AND2_out1;
  wire Switch12_out1;
  wire idle_output_check_out1;
  wire switch_compare_1_2;
  wire AND1_out1;
  wire Switch11_out1;
  wire Constant11_out1;
  wire Switch10_out1;
  wire state_TTL_in2_out1;
  wire switch_compare_1_3;
  wire [8:0] Constant10_out1;  // ufix9_En1000
  wire [8:0] Constant10_out1_dtc;  // ufix9
  wire state_TTL_in1_out1;
  wire switch_compare_1_4;
  wire [8:0] Constant9_out1;  // ufix9
  wire [8:0] Switch9_out1;  // ufix9
  wire state_ADC_out1;
  wire switch_compare_1_5;
  wire [8:0] Constant8_out1;  // ufix9
  wire [8:0] Switch8_out1;  // ufix9
  wire [5:0] Constant7_out1;  // ufix6
  wire state_DAC_out1;
  wire switch_compare_1_6;
  wire [8:0] Bit_Concat7_out1;  // ufix9
  wire [8:0] Switch7_out1;  // ufix9
  wire [5:0] Constant6_out1;  // ufix6
  wire state_charge_recov_out1;
  wire switch_compare_1_7;
  wire [8:0] Bit_Concat6_out1;  // ufix9
  wire [8:0] Switch6_out1;  // ufix9
  wire [5:0] Constant5_out1;  // ufix6
  wire state_amp_settle_out1;
  wire switch_compare_1_8;
  wire [8:0] Bit_Concat5_out1;  // ufix9
  wire [8:0] Switch5_out1;  // ufix9
  wire [5:0] Constant4_out1;  // ufix6
  wire state_stim_pol_out1;
  wire switch_compare_1_9;
  wire [8:0] Bit_Concat4_out1;  // ufix9
  wire [8:0] Switch4_out1;  // ufix9
  wire [5:0] Constant3_out1;  // ufix6
  wire state_stim_on_off_out1;
  wire switch_compare_1_10;
  wire [8:0] Bit_Concat3_out1;  // ufix9
  wire [8:0] Switch3_out1;  // ufix9
  wire [5:0] Constant2_out1;  // ufix6
  wire state_chan_sample_out1;
  wire switch_compare_1_11;
  wire [8:0] Bit_Concat2_out1;  // ufix9
  wire [8:0] Switch2_out1;  // ufix9
  wire Constant1_out1;
  wire state_DC_sample_out1;
  wire switch_compare_1_12;
  wire [8:0] Bit_Concat1_out1;  // ufix9
  wire [8:0] Switch1_out1;  // ufix9
  wire Constant_out1;
  wire [8:0] Bit_Concat_out1;  // ufix9

  // reading address 
  // to output
  // 
  // enable the RAM reading under specific conditions
  // --> important for the output data timing 
  // 
  // Enable data reading for output 
  // 
  // let's write a matlab/s function to replace this shit :)
  // 
  // TTTL addresses
  // 
  // the adressess depend on the chip number
  // 
  // the adressess depend only on the
  // InputState
  // 
  // the adressess depend on the converter
  // counter
  // 
  // on/off, stim_pol, settle, charge_recov addresses
  // 
  // ADC/DAC addresses
  // 
  // AC/DC channel addresses
  // 
  // the adressess depend on the 
  // source_channel_0_159 variable
  // 
  // Each kind of data has a different RAM
  // address according to the 
  // RAM address definition subsystem during the 
  // input interpretation 
  // 
  // enable reading address 
  // to output


  assign DC_out1 = OutputState == 8'b00000110;



  assign AC_out1 = OutputState == 8'b00000111;



  assign OR_out1 = DC_out1 | AC_out1;



  assign Stim_stuff_out1 = OutputState >= 8'b00001000;



  assign Stim_stuffff_out1 = OutputState <= 8'b00001011;



  assign AND_out1 = Stim_stuff_out1 & Stim_stuffff_out1;



  assign switch_compare_1 = AND_out1 > 1'b0;



  assign Constant12_out1 = 1'b1;



  assign switch_compare_1_1 = OR_out1 > 1'b0;



  assign AND2_out1 = AND_out1 & source_device_en;



  assign Switch12_out1 = (switch_compare_1 == 1'b0 ? Constant12_out1 :
              AND2_out1);



  assign idle_output_check_out1 = OutputState == 8'b00010000;



  assign switch_compare_1_2 = idle_output_check_out1 > 1'b0;



  assign AND1_out1 = OR_out1 & source_channel_en;



  assign Switch11_out1 = (switch_compare_1_1 == 1'b0 ? Switch12_out1 :
              AND1_out1);



  assign Constant11_out1 = 1'b0;



  assign Switch10_out1 = (switch_compare_1_2 == 1'b0 ? Switch11_out1 :
              Constant11_out1);



  assign rd_enable = Switch10_out1;

  assign state_TTL_in2_out1 = OutputState == 8'b00001111;



  assign switch_compare_1_3 = state_TTL_in2_out1 > 1'b0;



  assign Constant10_out1 = 9'b000000000;



  assign Constant10_out1_dtc = 9'b000000000;



  assign state_TTL_in1_out1 = OutputState == 8'b00001110;



  assign switch_compare_1_4 = state_TTL_in1_out1 > 1'b0;



  assign Constant9_out1 = 9'b101110001;



  assign Switch9_out1 = (switch_compare_1_3 == 1'b0 ? Constant10_out1_dtc :
              Constant9_out1);



  assign state_ADC_out1 = OutputState == 8'b00001101;



  assign switch_compare_1_5 = state_ADC_out1 > 1'b0;



  assign Constant8_out1 = 9'b101110000;



  assign Switch8_out1 = (switch_compare_1_4 == 1'b0 ? Switch9_out1 :
              Constant8_out1);



  assign Constant7_out1 = 6'b101101;



  assign state_DAC_out1 = OutputState == 8'b00001100;



  assign switch_compare_1_6 = state_DAC_out1 > 1'b0;



  assign Bit_Concat7_out1 = {Constant7_out1, converter_cnt};



  assign Switch7_out1 = (switch_compare_1_5 == 1'b0 ? Switch8_out1 :
              Bit_Concat7_out1);



  assign Constant6_out1 = 6'b101100;



  assign state_charge_recov_out1 = OutputState == 8'b00001011;



  assign switch_compare_1_7 = state_charge_recov_out1 > 1'b0;



  assign Bit_Concat6_out1 = {Constant6_out1, converter_cnt};



  assign Switch6_out1 = (switch_compare_1_6 == 1'b0 ? Switch7_out1 :
              Bit_Concat6_out1);



  assign Constant5_out1 = 6'b101011;



  assign state_amp_settle_out1 = OutputState == 8'b00001010;



  assign switch_compare_1_8 = state_amp_settle_out1 > 1'b0;



  assign Bit_Concat5_out1 = {Constant5_out1, source_device};



  assign Switch5_out1 = (switch_compare_1_7 == 1'b0 ? Switch6_out1 :
              Bit_Concat5_out1);



  assign Constant4_out1 = 6'b101010;



  assign state_stim_pol_out1 = OutputState == 8'b00001001;



  assign switch_compare_1_9 = state_stim_pol_out1 > 1'b0;



  assign Bit_Concat4_out1 = {Constant4_out1, source_device};



  assign Switch4_out1 = (switch_compare_1_8 == 1'b0 ? Switch5_out1 :
              Bit_Concat4_out1);



  assign Constant3_out1 = 6'b101001;



  assign state_stim_on_off_out1 = OutputState == 8'b00001000;



  assign switch_compare_1_10 = state_stim_on_off_out1 > 1'b0;



  assign Bit_Concat3_out1 = {Constant3_out1, source_device};



  assign Switch3_out1 = (switch_compare_1_9 == 1'b0 ? Switch4_out1 :
              Bit_Concat3_out1);



  assign Constant2_out1 = 6'b101000;



  assign state_chan_sample_out1 = OutputState == 8'b00000111;



  assign switch_compare_1_11 = state_chan_sample_out1 > 1'b0;



  assign Bit_Concat2_out1 = {Constant2_out1, source_device};



  assign Switch2_out1 = (switch_compare_1_10 == 1'b0 ? Switch3_out1 :
              Bit_Concat2_out1);



  assign Constant1_out1 = 1'b1;



  assign state_DC_sample_out1 = OutputState == 8'b00000110;



  assign switch_compare_1_12 = state_DC_sample_out1 > 1'b0;



  assign Bit_Concat1_out1 = {source_channel_0_159, Constant1_out1};



  assign Switch1_out1 = (switch_compare_1_11 == 1'b0 ? Switch2_out1 :
              Bit_Concat1_out1);



  assign Constant_out1 = 1'b0;



  assign Bit_Concat_out1 = {source_channel_0_159, Constant_out1};



  assign rd_addr = (switch_compare_1_12 == 1'b0 ? Switch1_out1 :
              Bit_Concat_out1);



endmodule  // Output_State_to_RAM_address_to_read

