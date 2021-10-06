// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v25\pipe_in_refractory.v
// Created: 2021-10-01 16:03:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: pipe_in_refractory
// Source Path: CustArch_v25/cust_architecture/process_and_retrieve/pipe_in_interpret/pipe_in_refractory
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module pipe_in_refractory
          (clk,
           reset,
           enb,
           value_from_pipe_in,
           refractory);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] value_from_pipe_in;  // uint16
  output  [15:0] refractory;  // uint16


  wire [15:0] Constant_out1;  // uint16
  wire [1:0] Bit_Slice_out1;  // ufix2
  wire Detect_Change1_out1;
  reg [15:0] Delay1_out1;  // uint16
  wire Compare_To_Constant_out1;
  wire Logical_Operator1_out1;
  wire [15:0] refractory_RAM_out1;  // uint16
  wire [15:0] refractory_RAM_out2;  // uint16


  assign Constant_out1 = 16'b0000000000000000;



  assign Bit_Slice_out1 = Constant_out1[1:0];



  Detect_Change1 u_Detect_Change1 (.clk(clk),
                                   .reset(reset),
                                   .enb(enb),
                                   .U(value_from_pipe_in),  // uint16
                                   .Y(Detect_Change1_out1)
                                   );

  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        Delay1_out1 <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          Delay1_out1 <= value_from_pipe_in;
        end
      end
    end



  assign Compare_To_Constant_out1 = Delay1_out1 == 16'b0000000000000001;



  assign Logical_Operator1_out1 = Detect_Change1_out1 & Compare_To_Constant_out1;



  DualPortRAM_generic #(.AddrWidth(2),
                        .DataWidth(16)
                        )
                      u_refractory_RAM (.clk(clk),
                                        .enb(enb),
                                        .wr_din(value_from_pipe_in),
                                        .wr_addr(Bit_Slice_out1),
                                        .wr_en(Logical_Operator1_out1),
                                        .rd_addr(Bit_Slice_out1),
                                        .wr_dout(refractory_RAM_out1),
                                        .rd_dout(refractory_RAM_out2)
                                        );

  assign refractory = refractory_RAM_out2;

endmodule  // pipe_in_refractory

