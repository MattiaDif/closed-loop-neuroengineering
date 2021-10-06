// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v25\filtering.v
// Created: 2021-10-01 16:03:47
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: filtering
// Source Path: CustArch_v25/cust_architecture/process_and_retrieve/filtering
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module filtering
          (clk,
           reset,
           enb,
           current_sample,
           In2,
           In3,
           filter_output,
           write_porcessing_addr_read_from_storage_addr_1,
           rd_addr);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] current_sample;  // uint16
  input   [7:0] In2;  // uint8
  input   [8:0] In3;  // ufix9
  output  [15:0] filter_output;  // uint16
  output  [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  output  rd_addr;


  reg [8:0] new_previous_sample;  // ufix9
  reg [8:0] reduced_reg [0:1];  // ufix9 [2]
  wire [8:0] reduced_reg_next [0:1];  // ufix9 [2]
  wire [8:0] new_previous_sample_1;  // ufix9
  reg [7:0] Delay_out1;  // uint8
  wire read_from_porcessing;
  reg  write_en;
  reg  [0:1] Delay6_reg;  // ufix1 [2]
  wire [0:1] Delay6_reg_next;  // ufix1 [2]
  wire Delay6_out1;
  wire signed [31:0] previous_sample;  // sfix32_En7
  wire signed [31:0] PreviousData2_storage_out1;  // sfix32_En7
  wire signed [31:0] previous_sample_1;  // sfix32_En7
  wire signed [31:0] new_previous_sample_2;  // sfix32_En7
  wire signed [31:0] PreviousData1_storage_out1;  // sfix32_En7
  reg [8:0] reduced_reg_1 [0:1];  // ufix9 [2]
  wire [8:0] reduced_reg_next_1 [0:1];  // ufix9 [2]
  wire [8:0] new_previous_sample_3;  // ufix9
  reg  [0:1] Delay4_reg;  // ufix1 [2]
  wire [0:1] Delay4_reg_next;  // ufix1 [2]
  wire Delay4_out1;
  wire [8:0] new_previous_sample_4;  // ufix9

  // this memory is to guarantee that the input is already stored
  // 
  // Note: If a read operation occurs simultaneously at the same address 
  // as a write operation, old data appears at the read output port (rd_dout).


  always @(posedge clk or posedge reset)
    begin : Delay2_process
      if (reset == 1'b1) begin
        new_previous_sample <= 9'b000000000;
      end
      else begin
        if (enb) begin
          new_previous_sample <= In3;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : reduced_process
      if (reset == 1'b1) begin
        reduced_reg[0] <= 9'b000000000;
        reduced_reg[1] <= 9'b000000000;
      end
      else begin
        if (enb) begin
          reduced_reg[0] <= reduced_reg_next[0];
          reduced_reg[1] <= reduced_reg_next[1];
        end
      end
    end

  assign new_previous_sample_1 = reduced_reg[1];
  assign reduced_reg_next[0] = new_previous_sample;
  assign reduced_reg_next[1] = reduced_reg[0];



  always @(posedge clk or posedge reset)
    begin : Delay_process
      if (reset == 1'b1) begin
        Delay_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Delay_out1 <= In2;
        end
      end
    end



  assign read_from_porcessing = Delay_out1 == 8'b00000111;



  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        write_en <= 1'b0;
      end
      else begin
        if (enb) begin
          write_en <= read_from_porcessing;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : Delay6_process
      if (reset == 1'b1) begin
        Delay6_reg[0] <= 1'b0;
        Delay6_reg[1] <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay6_reg[0] <= Delay6_reg_next[0];
          Delay6_reg[1] <= Delay6_reg_next[1];
        end
      end
    end

  assign Delay6_out1 = Delay6_reg[1];
  assign Delay6_reg_next[0] = write_en;
  assign Delay6_reg_next[1] = Delay6_reg[0];



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_PreviousData2_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(previous_sample),
                                               .wr_addr(new_previous_sample),
                                               .wr_en(write_en),
                                               .rd_addr(In3),
                                               .wr_dout(PreviousData2_storage_out1),
                                               .rd_dout(previous_sample_1)
                                               );

  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_PreviousData1_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(new_previous_sample_2),
                                               .wr_addr(new_previous_sample_1),
                                               .wr_en(Delay6_out1),
                                               .rd_addr(In3),
                                               .wr_dout(PreviousData1_storage_out1),
                                               .rd_dout(previous_sample)
                                               );

  two_pole_IIR u_two_pole_IIR (.clk(clk),
                               .reset(reset),
                               .enb(enb),
                               .current_sample(current_sample),  // uint16
                               .previous_sample_0(previous_sample),  // sfix32_En7
                               .previous_sample_1(previous_sample_1),  // sfix32_En7
                               .filter_output(filter_output),  // uint16
                               .new_previous_sample(new_previous_sample_2)  // sfix32_En7
                               );

  always @(posedge clk or posedge reset)
    begin : reduced_1_process
      if (reset == 1'b1) begin
        reduced_reg_1[0] <= 9'b000000000;
        reduced_reg_1[1] <= 9'b000000000;
      end
      else begin
        if (enb) begin
          reduced_reg_1[0] <= reduced_reg_next_1[0];
          reduced_reg_1[1] <= reduced_reg_next_1[1];
        end
      end
    end

  assign new_previous_sample_3 = reduced_reg_1[1];
  assign reduced_reg_next_1[0] = new_previous_sample_1;
  assign reduced_reg_next_1[1] = reduced_reg_1[0];



  assign write_porcessing_addr_read_from_storage_addr_1 = new_previous_sample_3;

  always @(posedge clk or posedge reset)
    begin : Delay4_process
      if (reset == 1'b1) begin
        Delay4_reg[0] <= 1'b0;
        Delay4_reg[1] <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay4_reg[0] <= Delay4_reg_next[0];
          Delay4_reg[1] <= Delay4_reg_next[1];
        end
      end
    end

  assign Delay4_out1 = Delay4_reg[1];
  assign Delay4_reg_next[0] = Delay6_out1;
  assign Delay4_reg_next[1] = Delay4_reg[0];



  assign rd_addr = Delay4_out1;

endmodule  // filtering

