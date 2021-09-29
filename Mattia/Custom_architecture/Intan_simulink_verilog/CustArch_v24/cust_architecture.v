// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v24\cust_architecture.v
// Created: 2021-09-29 17:03:18
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// 
// -- -------------------------------------------------------------
// -- Rate and Clocking Details
// -- -------------------------------------------------------------
// Model base rate: 1
// Target subsystem base rate: 1
// 
// 
// Clock Enable  Sample Time
// -- -------------------------------------------------------------
// ce_out        1
// -- -------------------------------------------------------------
// 
// 
// Output Signal                 Clock Enable  Sample Time
// -- -------------------------------------------------------------
// comp_data_out                 ce_out        1
// comp_data_out_valid           ce_out        1
// trig_stim                     ce_out        1
// computing                     ce_out        1
// pipe_out_data                 ce_out        1
// status                        ce_out        1
// -- -------------------------------------------------------------
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: cust_architecture
// Source Path: CustArch_v24/cust_architecture
// Hierarchy Level: 0
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module cust_architecture
          (clk,
           reset,
           clk_enable,
           FIFO_data_in,
           FIFO_write_to,
           data_stream_en,
           pipe_in_data,
           pipe_in_en,
           pipe_out_read,
           cfg,
           ce_out,
           comp_data_out,
           comp_data_out_valid,
           trig_stim,
           computing,
           pipe_out_data,
           status);


  input   clk;
  input   reset;
  input   clk_enable;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] data_stream_en;  // uint8
  input   [15:0] pipe_in_data;  // uint16
  input   pipe_in_en;
  input   pipe_out_read;
  input   [15:0] cfg;  // uint16
  output  ce_out;
  output  [15:0] comp_data_out;  // uint16
  output  comp_data_out_valid;
  output  [7:0] trig_stim;  // uint8
  output  computing;
  output  [15:0] pipe_out_data;  // uint16
  output  [15:0] status;  // uint16


  wire [7:0] InputState;  // uint8
  wire [7:0] OutputState;  // uint8
  wire output_interpret_out2;
  wire [8:0] rd_addr;  // ufix9
  wire [15:0] input_interpret_store_retrieve_out2;  // uint16
  wire [15:0] input_interpret_store_retrieve_out3;  // uint16
  wire [8:0] read_from_porcessing_addr;  // ufix9
  wire [7:0] y;  // uint8
  wire [15:0] pipe_in_interpret_out1_0;  // uint16
  wire [15:0] pipe_in_interpret_out1_1;  // uint16
  wire [8:0] rd_addr_1;  // ufix9
  wire [15:0] process_and_retrieve_out1;  // uint16
  wire collect_output_out1;
  wire [15:0] collect_output_out2;  // uint16
  wire [7:0] Constant_out1;  // uint8
  wire Constant1_out1;
  wire [15:0] Constant3_out1;  // uint16

  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 							Unused
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 							
  // 							Processing
  // 
  // 																						
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 																					Output
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 
  // 											
  // 
  // 												
  // 											Input


  output_interpret u_output_interpret (.clk(clk),
                                       .reset(reset),
                                       .enb(clk_enable),
                                       .data_stream_enable(data_stream_en),  // uint8
                                       .InputState(InputState),  // uint8
                                       .OutputState(OutputState),  // uint8
                                       .rd_enable(output_interpret_out2),
                                       .read_address(rd_addr)  // ufix9
                                       );

  input_interpret_store_retrieve u_input_interpret_store_retrieve (.clk(clk),
                                                                   .reset(reset),
                                                                   .enb(clk_enable),
                                                                   .FIFO_data_in(FIFO_data_in),  // uint16
                                                                   .FIFO_write_to(FIFO_write_to),
                                                                   .read_address(rd_addr),  // ufix9
                                                                   .data_stream_enable(data_stream_en),  // uint8
                                                                   .InputState(InputState),  // uint8
                                                                   .read_from_storage_value(input_interpret_store_retrieve_out2),  // uint16
                                                                   .current_value(input_interpret_store_retrieve_out3),  // uint16
                                                                   .read_from_processing_addr(read_from_porcessing_addr),  // ufix9
                                                                   .source_channel_0_159(y)  // uint8
                                                                   );

  pipe_in_interpret u_pipe_in_interpret (.clk(clk),
                                         .reset(reset),
                                         .enb(clk_enable),
                                         .source_channel_0_159(y),  // uint8
                                         .value_from_pipe_in(pipe_in_data),  // uint16
                                         .pipe_in_data_0(pipe_in_interpret_out1_0),  // uint16
                                         .pipe_in_data_1(pipe_in_interpret_out1_1)  // uint16
                                         );

  process_and_retrieve u_process_and_retrieve (.clk(clk),
                                               .reset(reset),
                                               .enb(clk_enable),
                                               .rd_address(rd_addr_1),  // ufix9
                                               .InputState(InputState),  // uint8
                                               .current_value(input_interpret_store_retrieve_out3),  // uint16
                                               .read_from_processing_addr(read_from_porcessing_addr),  // ufix9
                                               .OutputState(OutputState),  // uint8
                                               .pipe_in_data_0(pipe_in_interpret_out1_0),  // uint16
                                               .pipe_in_data_1(pipe_in_interpret_out1_1),  // uint16
                                               .rd_dout(process_and_retrieve_out1)  // uint16
                                               );

  collect_output u_collect_output (.clk(clk),
                                   .reset(reset),
                                   .enb(clk_enable),
                                   .FIFO_data_in(FIFO_data_in),  // uint16
                                   .FIFO_write_to(FIFO_write_to),
                                   .InputState(InputState),  // uint8
                                   .OutputState(OutputState),  // uint8
                                   .rd_enable(output_interpret_out2),
                                   .rd_address(rd_addr),  // ufix9
                                   .read_from_storage_value(input_interpret_store_retrieve_out2),  // uint16
                                   .rd_dout(process_and_retrieve_out1),  // uint16
                                   .comp_data_out_valid(collect_output_out1),
                                   .comp_data_out(collect_output_out2),  // uint16
                                   .rd_address_bit_set_AC_during_DC(rd_addr_1)  // ufix9
                                   );

  assign comp_data_out = collect_output_out2;

  assign comp_data_out_valid = collect_output_out1;

  assign Constant_out1 = 8'b00000001;



  assign trig_stim = Constant_out1;

  assign Constant1_out1 = 1'b1;



  assign computing = Constant1_out1;

  assign pipe_out_data = pipe_in_data;

  assign Constant3_out1 = 16'b0000000000000001;



  assign status = Constant3_out1;

  assign ce_out = clk_enable;

endmodule  // cust_architecture

