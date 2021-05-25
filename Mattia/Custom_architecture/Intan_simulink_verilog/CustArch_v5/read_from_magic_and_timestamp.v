// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v5\read_from_magic_and_timestamp.v
// Created: 2021-05-19 18:03:41
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: read_from_magic_and_timestamp
// Source Path: CustArch_v5/cust_architecture/collect_output/read_from_magic_and_timestamp
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module read_from_magic_and_timestamp
          (clk,
           enb,
           OutputState,
           FIFO_data_in,
           FIFO_write_to,
           InputState,
           read_from_Magic_TS);


  input   clk;
  input   enb;
  input   [7:0] OutputState;  // uint8
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  output  [15:0] read_from_Magic_TS;  // uint16


  wire [7:0] getting_address_and_enable_for_Magic_and_Timestamps_out1;  // uint8
  wire getting_address_and_enable_for_Magic_and_Timestamps_out2;
  wire [15:0] input_RAM_for_MagicNumber_TimeStamps_out1;  // uint16

  // This subsystem in needed for save in a specific RAM the magic number
  // and the timestamp


  getting_address_and_enable_for_Magic_and_Timestamps u_getting_address_and_enable_for_Magic_and_Timestamps (.InputState(InputState),  // uint8
                                                                                                             .FIFO_write_to(FIFO_write_to),
                                                                                                             .wr_address(getting_address_and_enable_for_Magic_and_Timestamps_out1),  // uint8
                                                                                                             .wr_enable(getting_address_and_enable_for_Magic_and_Timestamps_out2)
                                                                                                             );

  DualPortRAM_generic #(.AddrWidth(8),
                        .DataWidth(16)
                        )
                      u_input_RAM_for_MagicNumber_TimeStamps (.clk(clk),
                                                              .enb(enb),
                                                              .wr_din(FIFO_data_in),
                                                              .wr_addr(getting_address_and_enable_for_Magic_and_Timestamps_out1),
                                                              .wr_en(getting_address_and_enable_for_Magic_and_Timestamps_out2),
                                                              .rd_addr(OutputState),
                                                              .wr_dout(input_RAM_for_MagicNumber_TimeStamps_out1),
                                                              .rd_dout(read_from_Magic_TS)
                                                              );

endmodule  // read_from_magic_and_timestamp

