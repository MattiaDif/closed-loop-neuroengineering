// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v19\enable_source_chip_transmission.v
// Created: 2021-07-15 14:18:03
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: enable_source_chip_transmission
// Source Path: CustArch_v19/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/get_sources_and_RAM_addresses/enable_source_chip_transmissio
// Hierarchy Level: 5
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module enable_source_chip_transmission
          (clk,
           reset,
           enb,
           In1,
           Enable,
           Out1);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] In1;  // uint8
  input   Enable;
  output  [7:0] Out1;  // uint8


  wire enb_gated;
  wire [7:0] In1_bypass;  // uint8
  reg [7:0] In1_last_value;  // uint8


  assign enb_gated = Enable && enb;

  always @(posedge clk or posedge reset)
    begin : Out1_bypass_process
      if (reset == 1'b1) begin
        In1_last_value <= 8'b00000000;
      end
      else begin
        if (enb_gated) begin
          In1_last_value <= In1_bypass;
        end
      end
    end



  assign In1_bypass = (Enable == 1'b0 ? In1_last_value :
              In1);



  assign Out1 = In1_bypass;

endmodule  // enable_source_chip_transmission

