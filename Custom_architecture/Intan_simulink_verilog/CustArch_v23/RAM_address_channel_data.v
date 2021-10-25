// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v23\RAM_address_channel_data.v
// Created: 2021-09-22 17:25:26
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: RAM_address_channel_data
// Source Path: CustArch_v23/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/get_sources_and_RAM_addresses/RAM_address_channel_dat
// Hierarchy Level: 5
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module RAM_address_channel_data
          (clk,
           reset,
           enb,
           source_device,
           dev_output,
           Enable,
           source_channel_0_159);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] source_device;  // uint8
  input   [7:0] dev_output;  // uint8
  input   Enable;
  output  [7:0] source_channel_0_159;  // uint8


  wire enb_gated;
  wire [4:0] Bit_Slice_out1;  // ufix5
  wire [2:0] Bit_Slice1_out1;  // ufix3
  wire [7:0] source_channel_0_159_1;  // uint8
  wire [7:0] source_channel_0_159_bypass;  // uint8
  reg [7:0] source_channel_0_159_last_value;  // uint8

  // channel data address codification
  // according to this table


  assign enb_gated = Enable && enb;

  assign Bit_Slice_out1 = dev_output[4:0];



  assign Bit_Slice1_out1 = source_device[2:0];



  assign source_channel_0_159_1 = {Bit_Slice_out1, Bit_Slice1_out1};



  always @(posedge clk or posedge reset)
    begin : source_channel_0_159_bypass_1_process
      if (reset == 1'b1) begin
        source_channel_0_159_last_value <= 8'b00000000;
      end
      else begin
        if (enb_gated) begin
          source_channel_0_159_last_value <= source_channel_0_159_bypass;
        end
      end
    end



  assign source_channel_0_159_bypass = (Enable == 1'b0 ? source_channel_0_159_last_value :
              source_channel_0_159_1);



  assign source_channel_0_159 = source_channel_0_159_bypass;

endmodule  // RAM_address_channel_data
