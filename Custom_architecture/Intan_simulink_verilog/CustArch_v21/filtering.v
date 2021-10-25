// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v21\filtering.v
// Created: 2021-10-18 17:44:13
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: filtering
// Source Path: CustArch_v21/cust_architecture/process_and_retrieve/filtering
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module filtering
          (clk,
           reset,
           enb_1_2_0,
           current_sample,
           In2,
           In3,
           filter_output,
           write_porcessing_addr_read_from_storage_addr_4,
           wr_en);


  input   clk;
  input   reset;
  input   enb_1_2_0;
  input   [15:0] current_sample;  // uint16
  input   [7:0] In2;  // uint8
  input   [8:0] In3;  // ufix9
  output  [15:0] filter_output;  // uint16
  output  [8:0] write_porcessing_addr_read_from_storage_addr_4;  // ufix9
  output  wr_en;


  reg [8:0] new_previous_sample;  // ufix9
  reg [8:0] Delay5_reg [0:5];  // ufix9 [6]
  wire [8:0] Delay5_reg_next [0:5];  // ufix9 [6]
  wire [8:0] write_en;  // ufix9
  wire read_from_porcessing;
  reg  read_from_porcessing_1;
  reg  [0:5] Delay6_reg;  // ufix1 [6]
  wire [0:5] Delay6_reg_next;  // ufix1 [6]
  wire write_en_1;
  wire signed [31:0] write_en_2 [0:1];  // sfix32_En7 [2]
  wire signed [31:0] previous_sample;  // sfix32_En7
  wire signed [31:0] PreviousData2_storage_out1;  // sfix32_En7
  wire signed [31:0] previous_sample_1;  // sfix32_En7
  wire signed [31:0] new_previous_sample_1;  // sfix32_En7
  wire signed [31:0] PreviousData1_storage_out1;  // sfix32_En7
  wire signed [31:0] delayMatch_out_1;  // sfix32_En7
  reg signed [31:0] delayMatch_1_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch_out_2;  // sfix32_En7
  reg signed [31:0] delayMatch_1_reg_1 [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch_1_reg_next [0:1];  // sfix32_En7 [2]
  wire signed [31:0] delayMatch_1_reg_next_1 [0:1];  // sfix32_En7 [2]
  reg [8:0] Delay3_reg [0:4];  // ufix9 [5]
  wire [8:0] Delay3_reg_next [0:4];  // ufix9 [5]
  wire [8:0] Delay3_out1;  // ufix9
  reg  [0:5] Delay4_reg;  // ufix1 [6]
  wire [0:5] Delay4_reg_next;  // ufix1 [6]
  wire Delay4_out1;

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
        if (enb_1_2_0) begin
          new_previous_sample <= In3;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : Delay5_process
      if (reset == 1'b1) begin
        Delay5_reg[0] <= 9'b000000000;
        Delay5_reg[1] <= 9'b000000000;
        Delay5_reg[2] <= 9'b000000000;
        Delay5_reg[3] <= 9'b000000000;
        Delay5_reg[4] <= 9'b000000000;
        Delay5_reg[5] <= 9'b000000000;
      end
      else begin
        if (enb_1_2_0) begin
          Delay5_reg[0] <= Delay5_reg_next[0];
          Delay5_reg[1] <= Delay5_reg_next[1];
          Delay5_reg[2] <= Delay5_reg_next[2];
          Delay5_reg[3] <= Delay5_reg_next[3];
          Delay5_reg[4] <= Delay5_reg_next[4];
          Delay5_reg[5] <= Delay5_reg_next[5];
        end
      end
    end

  assign write_en = Delay5_reg[5];
  assign Delay5_reg_next[0] = new_previous_sample;
  assign Delay5_reg_next[1] = Delay5_reg[0];
  assign Delay5_reg_next[2] = Delay5_reg[1];
  assign Delay5_reg_next[3] = Delay5_reg[2];
  assign Delay5_reg_next[4] = Delay5_reg[3];
  assign Delay5_reg_next[5] = Delay5_reg[4];



  assign read_from_porcessing = In2 == 8'b00000111;



  always @(posedge clk or posedge reset)
    begin : delayMatch1_process
      if (reset == 1'b1) begin
        read_from_porcessing_1 <= 1'b0;
      end
      else begin
        if (enb_1_2_0) begin
          read_from_porcessing_1 <= read_from_porcessing;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : Delay6_process
      if (reset == 1'b1) begin
        Delay6_reg[0] <= 1'b0;
        Delay6_reg[1] <= 1'b0;
        Delay6_reg[2] <= 1'b0;
        Delay6_reg[3] <= 1'b0;
        Delay6_reg[4] <= 1'b0;
        Delay6_reg[5] <= 1'b0;
      end
      else begin
        if (enb_1_2_0) begin
          Delay6_reg[0] <= Delay6_reg_next[0];
          Delay6_reg[1] <= Delay6_reg_next[1];
          Delay6_reg[2] <= Delay6_reg_next[2];
          Delay6_reg[3] <= Delay6_reg_next[3];
          Delay6_reg[4] <= Delay6_reg_next[4];
          Delay6_reg[5] <= Delay6_reg_next[5];
        end
      end
    end

  assign write_en_1 = Delay6_reg[5];
  assign Delay6_reg_next[0] = read_from_porcessing_1;
  assign Delay6_reg_next[1] = Delay6_reg[0];
  assign Delay6_reg_next[2] = Delay6_reg[1];
  assign Delay6_reg_next[3] = Delay6_reg[2];
  assign Delay6_reg_next[4] = Delay6_reg[3];
  assign Delay6_reg_next[5] = Delay6_reg[4];



  DualPortRAM_generic_block #(.AddrWidth(9),
                              .DataWidth(32)
                              )
                            u_PreviousData2_storage (.clk(clk),
                                                     .enb_1_2_0(enb_1_2_0),
                                                     .wr_din(previous_sample),
                                                     .wr_addr(new_previous_sample),
                                                     .wr_en(read_from_porcessing_1),
                                                     .rd_addr(In3),
                                                     .wr_dout(PreviousData2_storage_out1),
                                                     .rd_dout(previous_sample_1)
                                                     );

  DualPortRAM_generic_block #(.AddrWidth(9),
                              .DataWidth(32)
                              )
                            u_PreviousData1_storage (.clk(clk),
                                                     .enb_1_2_0(enb_1_2_0),
                                                     .wr_din(new_previous_sample_1),
                                                     .wr_addr(write_en),
                                                     .wr_en(write_en_1),
                                                     .rd_addr(In3),
                                                     .wr_dout(PreviousData1_storage_out1),
                                                     .rd_dout(previous_sample)
                                                     );

  always @(posedge clk or posedge reset)
    begin : delayMatch_1_process
      if (reset == 1'b1) begin
        delayMatch_1_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch_1_reg[1] <= 32'sb00000000000000000000000000000000;
        delayMatch_1_reg_1[0] <= 32'sb00000000000000000000000000000000;
        delayMatch_1_reg_1[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch_1_reg[0] <= delayMatch_1_reg_next[0];
          delayMatch_1_reg[1] <= delayMatch_1_reg_next[1];
          delayMatch_1_reg_1[0] <= delayMatch_1_reg_next_1[0];
          delayMatch_1_reg_1[1] <= delayMatch_1_reg_next_1[1];
        end
      end
    end

  assign delayMatch_1_reg_next[0] = previous_sample;
  assign delayMatch_1_reg_next[1] = delayMatch_1_reg[0];
  assign delayMatch_out_1 = delayMatch_1_reg[1];
  assign delayMatch_1_reg_next_1[0] = previous_sample_1;
  assign delayMatch_1_reg_next_1[1] = delayMatch_1_reg_1[0];
  assign delayMatch_out_2 = delayMatch_1_reg_1[1];



  assign write_en_2[0] = delayMatch_out_1;
  assign write_en_2[1] = delayMatch_out_2;

  two_pole_IIR u_two_pole_IIR (.clk(clk),
                               .reset(reset),
                               .enb_1_2_0(enb_1_2_0),
                               .current_sample(current_sample),  // uint16
                               .previous_sample_0(write_en_2[0]),  // sfix32_En7
                               .previous_sample_1(write_en_2[1]),  // sfix32_En7
                               .filter_output(filter_output),  // uint16
                               .new_previous_sample(new_previous_sample_1)  // sfix32_En7
                               );

  always @(posedge clk or posedge reset)
    begin : Delay3_process
      if (reset == 1'b1) begin
        Delay3_reg[0] <= 9'b000000000;
        Delay3_reg[1] <= 9'b000000000;
        Delay3_reg[2] <= 9'b000000000;
        Delay3_reg[3] <= 9'b000000000;
        Delay3_reg[4] <= 9'b000000000;
      end
      else begin
        if (enb_1_2_0) begin
          Delay3_reg[0] <= Delay3_reg_next[0];
          Delay3_reg[1] <= Delay3_reg_next[1];
          Delay3_reg[2] <= Delay3_reg_next[2];
          Delay3_reg[3] <= Delay3_reg_next[3];
          Delay3_reg[4] <= Delay3_reg_next[4];
        end
      end
    end

  assign Delay3_out1 = Delay3_reg[4];
  assign Delay3_reg_next[0] = write_en;
  assign Delay3_reg_next[1] = Delay3_reg[0];
  assign Delay3_reg_next[2] = Delay3_reg[1];
  assign Delay3_reg_next[3] = Delay3_reg[2];
  assign Delay3_reg_next[4] = Delay3_reg[3];



  assign write_porcessing_addr_read_from_storage_addr_4 = Delay3_out1;

  always @(posedge clk or posedge reset)
    begin : Delay4_process
      if (reset == 1'b1) begin
        Delay4_reg[0] <= 1'b0;
        Delay4_reg[1] <= 1'b0;
        Delay4_reg[2] <= 1'b0;
        Delay4_reg[3] <= 1'b0;
        Delay4_reg[4] <= 1'b0;
        Delay4_reg[5] <= 1'b0;
      end
      else begin
        if (enb_1_2_0) begin
          Delay4_reg[0] <= Delay4_reg_next[0];
          Delay4_reg[1] <= Delay4_reg_next[1];
          Delay4_reg[2] <= Delay4_reg_next[2];
          Delay4_reg[3] <= Delay4_reg_next[3];
          Delay4_reg[4] <= Delay4_reg_next[4];
          Delay4_reg[5] <= Delay4_reg_next[5];
        end
      end
    end

  assign Delay4_out1 = Delay4_reg[5];
  assign Delay4_reg_next[0] = write_en_1;
  assign Delay4_reg_next[1] = Delay4_reg[0];
  assign Delay4_reg_next[2] = Delay4_reg[1];
  assign Delay4_reg_next[3] = Delay4_reg[2];
  assign Delay4_reg_next[4] = Delay4_reg[3];
  assign Delay4_reg_next[5] = Delay4_reg[4];



  assign wr_en = Delay4_out1;

endmodule  // filtering
