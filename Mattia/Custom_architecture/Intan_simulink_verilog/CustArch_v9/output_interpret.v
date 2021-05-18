// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v9\output_interpret.v
// Created: 2021-05-13 14:07:46
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: output_interpret
// Source Path: CustArch_v9/cust_architecture/output_interpret
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module output_interpret
          (clk,
           reset,
           enb,
           data_stream_enable,
           InputState,
           OutputState,
           rd_enable,
           read_address);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] data_stream_enable;  // uint8
  input   [7:0] InputState;  // uint8
  output  [7:0] OutputState;  // uint8
  output  rd_enable;
  output  [8:0] read_address;  // ufix9


  wire Interpret_to_RAM_output_out1;
  wire [8:0] rd_addr;  // ufix9

  // if InputState == 8 I have processed all the 20 channels (16ch + 4aux)
  // and I start with the output encoding


  OutputStateMachine u_OutputStateMachine (.clk(clk),
                                           .reset(reset),
                                           .enb(enb),
                                           .input_state(InputState),  // uint8
                                           .OutputState(OutputState)  // uint8
                                           );

  Interpret_to_RAM_output u_Interpret_to_RAM_output (.clk(clk),
                                                     .reset(reset),
                                                     .enb(enb),
                                                     .OutputState(OutputState),  // uint8
                                                     .data_stream_en(data_stream_enable),  // uint8
                                                     .rd_enable(Interpret_to_RAM_output_out1),
                                                     .read_address(rd_addr)  // ufix9
                                                     );

  assign rd_enable = Interpret_to_RAM_output_out1;

  assign read_address = rd_addr;

endmodule  // output_interpret

