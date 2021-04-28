// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v2\get_sources_and_RAM_addresses.v
// Created: 2021-04-27 12:06:56
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: get_sources_and_RAM_addresses
// Source Path: CustArch_v2/cust_architecture/input_interpret_store_retrieve/Interpret_to_RAM_input/interpret incoming 
// data/get_sources_and_RAM_addresse
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module get_sources_and_RAM_addresses
          (clk,
           reset,
           enb,
           FIFO_write_to,
           InputState,
           data_stream_enable,
           source_device,
           device_output,
           source_channel,
           inc_source_device);


  input   clk;
  input   reset;
  input   enb;
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  input   [7:0] data_stream_enable;  // uint8
  output  [7:0] source_device;  // uint8
  output  [7:0] device_output;  // uint8
  output  [7:0] source_channel;  // uint8
  output  inc_source_device;


  wire Compare_To_Constant1_out1;
  wire switch_compare_1;
  wire Compare_To_Constant4_out1;
  wire Compare_To_Constant3_out1;
  wire Compare_To_Constant6_out1;
  wire AND1_out1;
  wire OR_out1;
  wire enable_RAM_indexing_out1;
  wire [7:0] next_chip_rollmap;  // uint8
  reg [7:0] Memory_out1;  // uint8
  wire [7:0] curr_chip_rollmap;  // uint8
  wire [7:0] source_device_1;  // uint8
  wire Compare_To_Constant5_out1;
  reg  Memory1_out1;
  wire switch_compare_1_1;
  wire [7:0] Switch1_out1;  // uint8
  wire Compare_To_Constant2_out1;
  reg  Memory2_out1;
  wire AND3_out1;
  reg [7:0] device_output_1;  // uint8
  wire [7:0] source_channel_0_159;  // uint8
  wire AND2_out1;

  // necessary to move forward from state 6 to 8
  // 
  // amplifier chip under
  // processing
  // 
  // the channel_counter increments the 
  // device_output variable every time the 
  // current_chip rollmap is equal to 00000000
  // (rollmap concluded)
  // 
  // if InputState == 13
  // reset the channel counter
  // 
  // whether or not enable the address definition
  // for the RAM writing 
  // 
  // if InputState == 0 (starting state corresponding to the first byte
  // of the magic number) the data_stream_enable goes through the switch
  // 
  // 0-159 --> 20 (16 electrodes + 4 aux commands) x 8 chips
  // 
  // to identify the first position in which 
  // I am able to find a 1
  // 
  // if next_chip_rollmap == 00000000 I have cycled among all the chip so let's restart from data_stream_enable
  // 
  // the data_stream_enable is a 8-bit string in which if a bit is 1 the data stream of that chip is enabled
  // 
  // channel (electrode) under
  // processing
  // 
  // channel data
  // address
  // 
  // TTL data use constant specific addresses


  assign Compare_To_Constant1_out1 = InputState == 8'b00000000;



  assign switch_compare_1 = Compare_To_Constant1_out1 > 1'b0;



  assign Compare_To_Constant4_out1 = InputState >= 8'b00001000;



  assign Compare_To_Constant3_out1 = InputState == 8'b00000110;



  assign Compare_To_Constant6_out1 = InputState <= 8'b00001011;



  assign AND1_out1 = Compare_To_Constant4_out1 & Compare_To_Constant6_out1;



  assign OR_out1 = Compare_To_Constant3_out1 | AND1_out1;



  assign enable_RAM_indexing_out1 = FIFO_write_to & OR_out1;



  always @(posedge clk or posedge reset)
    begin : Memory_process
      if (reset == 1'b1) begin
        Memory_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Memory_out1 <= next_chip_rollmap;
        end
      end
    end



  find_next_chip_rollmap u_find_next_chip_rollmap (.clk(clk),
                                                   .reset(reset),
                                                   .enb(enb),
                                                   .curr_chip_rollmap(curr_chip_rollmap),  // uint8
                                                   .source_chip(source_device_1),  // uint8
                                                   .Enable(enable_RAM_indexing_out1),
                                                   .bit_masked(next_chip_rollmap)  // uint8
                                                   );

  assign Compare_To_Constant5_out1 = next_chip_rollmap == 8'b00000000;



  always @(posedge clk or posedge reset)
    begin : Memory1_process
      if (reset == 1'b1) begin
        Memory1_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Memory1_out1 <= Compare_To_Constant5_out1;
        end
      end
    end



  assign switch_compare_1_1 = Memory1_out1 > 1'b0;



  assign Switch1_out1 = (switch_compare_1_1 == 1'b0 ? Memory_out1 :
              data_stream_enable);



  assign curr_chip_rollmap = (switch_compare_1 == 1'b0 ? Switch1_out1 :
              data_stream_enable);



  find_first_one_pos u_find_first_one_pos (.clk(clk),
                                           .reset(reset),
                                           .enb(enb),
                                           .In1(curr_chip_rollmap),  // uint8
                                           .Enable(enable_RAM_indexing_out1),
                                           .first_one_pos(source_device_1)  // uint8
                                           );

  enable_source_chip_transmission u_enable_source_chip_transmission (.clk(clk),
                                                                     .reset(reset),
                                                                     .enb(enb),
                                                                     .In1(source_device_1),  // uint8
                                                                     .Enable(enable_RAM_indexing_out1),
                                                                     .Out1(source_device)  // uint8
                                                                     );

  assign Compare_To_Constant2_out1 = InputState == 8'b00001101;



  always @(posedge clk or posedge reset)
    begin : Memory2_process
      if (reset == 1'b1) begin
        Memory2_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Memory2_out1 <= enable_RAM_indexing_out1;
        end
      end
    end



  assign AND3_out1 = Memory1_out1 & Memory2_out1;



  // Count limited, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  //  count to value  = 20
  always @(posedge clk or posedge reset)
    begin : channel_counter_process
      if (reset == 1'b1) begin
        device_output_1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          if (Compare_To_Constant2_out1 == 1'b1) begin
            device_output_1 <= 8'b00000000;
          end
          else if (AND3_out1 == 1'b1) begin
            if (device_output_1 >= 8'b00010100) begin
              device_output_1 <= 8'b00000000;
            end
            else begin
              device_output_1 <= device_output_1 + 8'b00000001;
            end
          end
        end
      end
    end



  RAM_address_channel_data u_RAM_address_channel_data (.clk(clk),
                                                       .reset(reset),
                                                       .enb(enb),
                                                       .source_device(source_device),  // uint8
                                                       .dev_output(device_output_1),  // uint8
                                                       .Enable(enable_RAM_indexing_out1),
                                                       .source_channel_0_159(source_channel_0_159)  // uint8
                                                       );

  assign source_channel = source_channel_0_159;

  assign AND2_out1 = enable_RAM_indexing_out1 & Compare_To_Constant5_out1;



  assign inc_source_device = AND2_out1;

  assign device_output = device_output_1;

endmodule  // get_sources_and_RAM_addresses

