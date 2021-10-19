// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v26\refractory_period.v
// Created: 2021-10-13 00:27:28
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: refractory_period
// Source Path: CustArch_v26/cust_architecture/process_and_retrieve/spike_detection/refractory_period
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module refractory_period
          (clk,
           reset,
           enb,
           InputState,
           wr_en,
           write_porcessing_addr_read_from_storage_addr_1,
           rd_addr,
           In5,
           refractory_check,
           spike);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] InputState;  // uint8
  input   wr_en;
  input   [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  input   [8:0] rd_addr;  // ufix9
  input   [15:0] In5;  // uint16
  output  refractory_check;
  output  [15:0] spike;  // uint16


  wire read_from_porcessing;
  reg [31:0] HDL_Counter_out1;  // uint32
  reg [8:0] Memory4_out1;  // ufix9
  wire [31:0] timestamp_before_out1;  // uint32
  wire [31:0] timestamp_before_out2;  // uint32
  wire signed [32:0] Sum_1;  // sfix33
  wire signed [32:0] Sum_2;  // sfix33
  wire signed [32:0] Sum_out1;  // sfix33
  wire signed [32:0] Relational_Operator_1_1;  // sfix33
  wire Relational_Operator_relop1;
  reg [7:0] Memory1_out1;  // uint8
  wire read_from_porcessing_1;
  wire Logical_Operator_out1;
  wire switch_compare_1;
  wire [15:0] Constant1_out1;  // uint16
  wire [15:0] Constant_out1;  // uint16
  wire [15:0] Switch_out1;  // uint16
  wire [15:0] spikes_out1;  // uint16
  wire [15:0] spikes_out2;  // uint16


  assign read_from_porcessing = InputState == 8'b00000101;



  // Free running, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  always @(posedge clk or posedge reset)
    begin : HDL_Counter_process
      if (reset == 1'b1) begin
        HDL_Counter_out1 <= 32'b00000000000000000000000000000000;
      end
      else begin
        if (enb && read_from_porcessing) begin
          HDL_Counter_out1 <= HDL_Counter_out1 + 32'b00000000000000000000000000000001;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : Memory4_process
      if (reset == 1'b1) begin
        Memory4_out1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          Memory4_out1 <= write_porcessing_addr_read_from_storage_addr_1;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_timestamp_before (.clk(clk),
                                          .enb(enb),
                                          .wr_din(HDL_Counter_out1),
                                          .wr_addr(Memory4_out1),
                                          .wr_en(wr_en),
                                          .rd_addr(write_porcessing_addr_read_from_storage_addr_1),
                                          .wr_dout(timestamp_before_out1),
                                          .rd_dout(timestamp_before_out2)
                                          );

  assign Sum_1 = {1'b0, HDL_Counter_out1};
  assign Sum_2 = {1'b0, timestamp_before_out2};
  assign Sum_out1 = Sum_1 - Sum_2;



  assign Relational_Operator_1_1 = {17'b0, In5};
  assign Relational_Operator_relop1 = Sum_out1 > Relational_Operator_1_1;



  always @(posedge clk or posedge reset)
    begin : Memory1_process
      if (reset == 1'b1) begin
        Memory1_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Memory1_out1 <= InputState;
        end
      end
    end



  assign read_from_porcessing_1 = Memory1_out1 == 8'b00000111;



  assign Logical_Operator_out1 = Relational_Operator_relop1 & read_from_porcessing_1;



  assign refractory_check = Logical_Operator_out1;

  assign switch_compare_1 = wr_en > 1'b0;



  assign Constant1_out1 = 16'b0000000000000000;



  assign Constant_out1 = 16'b0000000000000001;



  assign Switch_out1 = (switch_compare_1 == 1'b0 ? Constant1_out1 :
              Constant_out1);



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_spikes (.clk(clk),
                                .enb(enb),
                                .wr_din(Switch_out1),
                                .wr_addr(Memory4_out1),
                                .wr_en(read_from_porcessing_1),
                                .rd_addr(rd_addr),
                                .wr_dout(spikes_out1),
                                .rd_dout(spikes_out2)
                                );

  assign spike = spikes_out2;

endmodule  // refractory_period

