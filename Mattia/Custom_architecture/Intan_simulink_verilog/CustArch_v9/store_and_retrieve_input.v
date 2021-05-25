// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v9\store_and_retrieve_input.v
// Created: 2021-05-25 14:12:37
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: store_and_retrieve_input
// Source Path: CustArch_v9/cust_architecture/input_interpret_store_retrieve/store_and_retrieve_input
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module store_and_retrieve_input
          (clk,
           reset,
           enb,
           read_address,
           wr_din,
           wr_address,
           wr_en,
           InputState,
           read_from_storage_value,
           current_value,
           read_from_porcessing_addr);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] read_address;  // ufix9
  input   [15:0] wr_din;  // uint16
  input   [8:0] wr_address;  // ufix9
  input   wr_en;
  input   [7:0] InputState;  // uint8
  output  [15:0] read_from_storage_value;  // uint16
  output  [15:0] current_value;  // uint16
  output  [8:0] read_from_porcessing_addr;  // ufix9


  wire [15:0] input_storage_out1;  // uint16
  wire [15:0] read_from_storage;  // uint16
  wire Compare_To_Constant_out1;
  reg [8:0] read_from_porcessing_addr_1;  // ufix9
  wire [15:0] input_storage_for_processing_out1;  // uint16
  wire [15:0] current_value_1;  // uint16

  // data sent for processing
  // 
  // current data
  // 
  // this memory is for synchronizing the current value and
  // the read_from_porcessing_addr since to ouput a value
  // the dual port RAM needs a single clock
  // 
  // reading address
  // from output
  // 
  // writing address
  // 
  // data sent to Output
  // 
  // address sent for processing
  // 
  // current data
  // 
  // Two dual-port RAM for storage and processing 


  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_input_storage (.clk(clk),
                                       .enb(enb),
                                       .wr_din(wr_din),
                                       .wr_addr(wr_address),
                                       .wr_en(wr_en),
                                       .rd_addr(read_address),
                                       .wr_dout(input_storage_out1),
                                       .rd_dout(read_from_storage)
                                       );

  assign read_from_storage_value = read_from_storage;

  assign Compare_To_Constant_out1 = InputState == 8'b00000111;



  always @(posedge clk or posedge reset)
    begin : Memory4_process
      if (reset == 1'b1) begin
        read_from_porcessing_addr_1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          read_from_porcessing_addr_1 <= wr_address;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_input_storage_for_processing (.clk(clk),
                                                      .enb(enb),
                                                      .wr_din(wr_din),
                                                      .wr_addr(wr_address),
                                                      .wr_en(Compare_To_Constant_out1),
                                                      .rd_addr(read_from_porcessing_addr_1),
                                                      .wr_dout(input_storage_for_processing_out1),
                                                      .rd_dout(current_value_1)
                                                      );

  assign current_value = current_value_1;

  assign read_from_porcessing_addr = read_from_porcessing_addr_1;

endmodule  // store_and_retrieve_input

