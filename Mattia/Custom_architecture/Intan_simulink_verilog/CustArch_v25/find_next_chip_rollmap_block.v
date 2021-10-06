// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v25\find_next_chip_rollmap_block.v
// Created: 2021-10-01 16:03:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: find_next_chip_rollmap_block
// Source Path: CustArch_v25/cust_architecture/output_interpret/Interpret_to_RAM_output/get_enabled_timings /find_next_chip_rollmap
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module find_next_chip_rollmap_block
          (clk,
           reset,
           enb,
           In1,
           Trigger,
           Out1);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] In1;  // uint8
  input   Trigger;
  output  [7:0] Out1;  // uint8


  wire enb_gated;
  reg  Trigger_delayed;
  wire Trigger_emulated;
  wire [7:0] Bit_Rotate_out1;  // uint8
  wire [7:0] Bit_Rotate_out1_bypass;  // uint8
  reg [7:0] Bit_Rotate_out1_last_value;  // uint8

  // this is to identify the next chip for decoding process 
  // 
  // bit rotation --> circular rotation of the data_stream_enable 8-bit string
  // 
  // Example: data_stream_enable == 01001000
  // right rotation length 1 --> 00100100
  // right rotation length 3 --> 10000100


  always @(posedge clk or posedge reset)
    begin : Trigger_delay
      if (reset) begin
        Trigger_delayed <= 1'b1;
      end
      else begin
        if (enb) begin
          Trigger_delayed <= Trigger;
        end
      end
    end

  assign Trigger_emulated = !Trigger_delayed && Trigger;

  assign enb_gated = Trigger_emulated && enb;

  assign Bit_Rotate_out1 = {In1[0], In1[7:1]};



  always @(posedge clk or posedge reset)
    begin : Out1_bypass_process
      if (reset == 1'b1) begin
        Bit_Rotate_out1_last_value <= 8'b00000000;
      end
      else begin
        if (enb_gated) begin
          Bit_Rotate_out1_last_value <= Bit_Rotate_out1_bypass;
        end
      end
    end



  assign Bit_Rotate_out1_bypass = (Trigger_emulated == 1'b0 ? Bit_Rotate_out1_last_value :
              Bit_Rotate_out1);



  assign Out1 = Bit_Rotate_out1_bypass;

endmodule  // find_next_chip_rollmap_block

