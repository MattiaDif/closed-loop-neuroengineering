// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v25\condition_to_count_DAC_or_ADC.v
// Created: 2021-10-01 16:03:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: condition_to_count_DAC_or_ADC
// Source Path: CustArch_v25/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/condition to count DAC or ADC
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module condition_to_count_DAC_or_ADC
          (fifo_write_to,
           input_state,
           Out1);


  input   fifo_write_to;
  input   [7:0] input_state;  // uint8
  output  Out1;


  wire is_state_DAC_out1;
  wire is_state_ADC_out1;
  wire OR_out1;
  wire AND_out1;


  assign is_state_DAC_out1 = input_state == 8'b00001100;



  assign is_state_ADC_out1 = input_state == 8'b00001101;



  assign OR_out1 = is_state_DAC_out1 | is_state_ADC_out1;



  assign AND_out1 = fifo_write_to & OR_out1;



  assign Out1 = AND_out1;

endmodule  // condition_to_count_DAC_or_ADC

