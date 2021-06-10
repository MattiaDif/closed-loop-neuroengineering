// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v11\input_interpret_store_retrieve.v
// Created: 2021-05-28 09:27:01
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: input_interpret_store_retrieve
// Source Path: CustArch_v11/cust_architecture/input_interpret_store_retrieve
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module input_interpret_store_retrieve
          (clk,
           reset,
           enb,
           FIFO_data_in,
           FIFO_write_to,
           read_address,
           data_stream_enable,
           InputState,
           read_from_storage_value,
           current_value,
           read_from_processing_addr);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [8:0] read_address;  // ufix9
  input   [7:0] data_stream_enable;  // uint8
  output  [7:0] InputState;  // uint8
  output  [15:0] read_from_storage_value;  // uint16
  output  [15:0] current_value;  // uint16
  output  [8:0] read_from_processing_addr;  // ufix9


  wire [7:0] Interpret_to_RAM_input_out1;  // uint8
  wire Interpret_to_RAM_input_out2;
  wire [15:0] Interpret_to_RAM_input_out3;  // uint16
  wire [8:0] wr_addr;  // ufix9
  wire Interpret_to_RAM_input_out5;
  wire [15:0] store_and_retrieve_input_out1;  // uint16
  wire [15:0] store_and_retrieve_input_out2;  // uint16
  wire [8:0] read_from_porcessing_addr;  // ufix9

  // data sent to Output
  // 
  // data sent for processing
  // 
  // address sent for processing
  // 
  // reading address
  // from output


  Interpret_to_RAM_input u_Interpret_to_RAM_input (.clk(clk),
                                                   .reset(reset),
                                                   .enb(enb),
                                                   .FIFO_data_in(FIFO_data_in),  // uint16
                                                   .FIFO_write_to(FIFO_write_to),
                                                   .InputState(InputState),  // uint8
                                                   .data_stream_enable(data_stream_enable),  // uint8
                                                   .device_output(Interpret_to_RAM_input_out1),  // uint8
                                                   .inc_source_device(Interpret_to_RAM_input_out2),
                                                   .wr_din(Interpret_to_RAM_input_out3),  // uint16
                                                   .write_address(wr_addr),  // ufix9
                                                   .wr_en(Interpret_to_RAM_input_out5)
                                                   );

  InputStateMachine u_InputStateMachine (.clk(clk),
                                         .reset(reset),
                                         .enb(enb),
                                         .FIFO_data_in(FIFO_data_in),  // uint16
                                         .FIFO_write_to(FIFO_write_to),
                                         .channel_0_19(Interpret_to_RAM_input_out1),  // uint8
                                         .inc_source_device_MSB(Interpret_to_RAM_input_out2),
                                         .InputState(InputState)  // uint8
                                         );

  store_and_retrieve_input u_store_and_retrieve_input (.clk(clk),
                                                       .reset(reset),
                                                       .enb(enb),
                                                       .read_address(read_address),  // ufix9
                                                       .wr_din(Interpret_to_RAM_input_out3),  // uint16
                                                       .wr_address(wr_addr),  // ufix9
                                                       .wr_en(Interpret_to_RAM_input_out5),
                                                       .InputState(InputState),  // uint8
                                                       .read_from_storage_value(store_and_retrieve_input_out1),  // uint16
                                                       .current_value(store_and_retrieve_input_out2),  // uint16
                                                       .read_from_porcessing_addr(read_from_porcessing_addr)  // ufix9
                                                       );

  assign read_from_storage_value = store_and_retrieve_input_out1;

  assign current_value = store_and_retrieve_input_out2;

  assign read_from_processing_addr = read_from_porcessing_addr;

endmodule  // input_interpret_store_retrieve

