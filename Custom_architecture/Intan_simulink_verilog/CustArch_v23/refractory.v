// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v23\refractory.v
// Created: 2021-09-22 17:25:26
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: refractory
// Source Path: CustArch_v23/cust_architecture/pipe_in_interpret/refractory
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module refractory
          (clk,
           reset,
           enb,
           value_from_pipe_in,
           refractory_1);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] value_from_pipe_in;  // uint16
  output  [15:0] refractory_1;  // uint16


  wire [15:0] Constant1_out1;  // uint16
  wire [1:0] Cast1_out1;  // ufix2
  reg [15:0] Delay_out1;  // uint16
  wire Relational_Operator_relop1;
  wire [15:0] Constant_out1;  // uint16
  wire Relational_Operator1_relop1;
  wire Logical_Operator_out1;
  wire [15:0] Constant2_out1;  // uint16
  wire [1:0] Cast_out1;  // ufix2
  wire [15:0] Dual_Port_RAM_out1;  // uint16
  wire [15:0] Dual_Port_RAM_out2;  // uint16


  assign Constant1_out1 = 16'b0000000000000000;



  assign Cast1_out1 = Constant1_out1[1:0];



  always @(posedge clk or posedge reset)
    begin : Delay_process
      if (reset == 1'b1) begin
        Delay_out1 <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          Delay_out1 <= value_from_pipe_in;
        end
      end
    end



  assign Relational_Operator_relop1 = value_from_pipe_in != Delay_out1;



  assign Constant_out1 = 16'b0000000010000000;



  assign Relational_Operator1_relop1 = Delay_out1 == Constant_out1;



  assign Logical_Operator_out1 = Relational_Operator_relop1 & Relational_Operator1_relop1;



  assign Constant2_out1 = 16'b0000000000000000;



  assign Cast_out1 = Constant2_out1[1:0];



  DualPortRAM_generic #(.AddrWidth(2),
                        .DataWidth(16)
                        )
                      u_Dual_Port_RAM (.clk(clk),
                                       .enb(enb),
                                       .wr_din(value_from_pipe_in),
                                       .wr_addr(Cast1_out1),
                                       .wr_en(Logical_Operator_out1),
                                       .rd_addr(Cast_out1),
                                       .wr_dout(Dual_Port_RAM_out1),
                                       .rd_dout(Dual_Port_RAM_out2)
                                       );

  assign refractory_1 = Dual_Port_RAM_out2;

endmodule  // refractory
