// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v10\process_and_retrieve.v
// Created: 2021-06-18 10:46:14
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: process_and_retrieve
// Source Path: CustArch_v10/cust_architecture/process_and_retrieve
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module process_and_retrieve
          (clk,
           reset,
           enb,
           rd_address,
           InputState,
           current_value,
           read_from_processing_addr,
           rd_dout);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] rd_address;  // ufix9
  input   [7:0] InputState;  // uint8
  input   [15:0] current_value;  // uint16
  input   [8:0] read_from_processing_addr;  // ufix9
  output  [15:0] rd_dout;  // uint16


  reg [8:0] Memory8_reg [0:1];  // ufix9 [2]
  wire [8:0] Memory8_reg_next [0:1];  // ufix9 [2]
  wire [8:0] write_porcessing_addr_read_from_storage_addr_2;  // ufix9
  reg [7:0] Memory3_out1;  // uint8
  wire read_from_porcessing;
  reg  [0:1] Memory1_reg;  // ufix1 [2]
  wire [0:1] Memory1_reg_next;  // ufix1 [2]
  wire write_en;
  wire signed [31:0] previous_sample;  // sfix32_En7
  reg signed [31:0] Memory4_out1;  // sfix32_En7
  wire signed [31:0] PreviousData2_storage_out1;  // sfix32_En7
  wire signed [31:0] previous_sample_1;  // sfix32_En7
  wire signed [31:0] new_previous_sample;  // sfix32_En7
  reg signed [31:0] new_previous_sample_1;  // sfix32_En7
  wire signed [31:0] PreviousData1_storage_out1;  // sfix32_En7
  wire [15:0] digital_filter_out1;  // uint16
  reg [15:0] filter_output;  // uint16
  wire [15:0] ProcessedData_storage_out1;  // uint16
  wire [15:0] ProcessedData_storage_out2;  // uint16

  // The dual-port RAM in the input interpret subsystem introduces 
  // a sample of delay. Furthermore, I need one sample more of delay
  // due to the IIR filter --> overall delay of two samples
  // 
  // generally, all sample in the processing subsystem
  // are delayed by two samples --> the memories are 
  // needed for this reason
  // 
  // this memory is to guarantee that the input is already stored
  // 
  // note that these RAM are too big.. I just need 160 and thus a reduced address port width  
  // 
  // Note: If a read operation occurs simultaneously at the same address 
  // as a write operation, old data appears at the read output port (rd_dout).
  // 
  // rd_address from Output
  // 
  // data sent for processing
  // 
  // address sent for processing


  always @(posedge clk or posedge reset)
    begin : Memory8_process
      if (reset == 1'b1) begin
        Memory8_reg[0] <= 9'b000000000;
        Memory8_reg[1] <= 9'b000000000;
      end
      else begin
        if (enb) begin
          Memory8_reg[0] <= Memory8_reg_next[0];
          Memory8_reg[1] <= Memory8_reg_next[1];
        end
      end
    end

  assign write_porcessing_addr_read_from_storage_addr_2 = Memory8_reg[1];
  assign Memory8_reg_next[0] = read_from_processing_addr;
  assign Memory8_reg_next[1] = Memory8_reg[0];



  always @(posedge clk or posedge reset)
    begin : Memory3_process
      if (reset == 1'b1) begin
        Memory3_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Memory3_out1 <= InputState;
        end
      end
    end



  assign read_from_porcessing = Memory3_out1 == 8'b00000111;



  always @(posedge clk or posedge reset)
    begin : Memory1_process
      if (reset == 1'b1) begin
        Memory1_reg[0] <= 1'b0;
        Memory1_reg[1] <= 1'b0;
      end
      else begin
        if (enb) begin
          Memory1_reg[0] <= Memory1_reg_next[0];
          Memory1_reg[1] <= Memory1_reg_next[1];
        end
      end
    end

  assign write_en = Memory1_reg[1];
  assign Memory1_reg_next[0] = read_from_porcessing;
  assign Memory1_reg_next[1] = Memory1_reg[0];



  always @(posedge clk or posedge reset)
    begin : Memory4_process
      if (reset == 1'b1) begin
        Memory4_out1 <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb) begin
          Memory4_out1 <= previous_sample;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_PreviousData2_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(Memory4_out1),
                                               .wr_addr(write_porcessing_addr_read_from_storage_addr_2),
                                               .wr_en(write_en),
                                               .rd_addr(read_from_processing_addr),
                                               .wr_dout(PreviousData2_storage_out1),
                                               .rd_dout(previous_sample_1)
                                               );

  always @(posedge clk or posedge reset)
    begin : Memory6_process
      if (reset == 1'b1) begin
        new_previous_sample_1 <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb) begin
          new_previous_sample_1 <= new_previous_sample;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_PreviousData1_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(new_previous_sample_1),
                                               .wr_addr(write_porcessing_addr_read_from_storage_addr_2),
                                               .wr_en(write_en),
                                               .rd_addr(read_from_processing_addr),
                                               .wr_dout(PreviousData1_storage_out1),
                                               .rd_dout(previous_sample)
                                               );

  digital_filter u_digital_filter (.current_sample(current_value),  // uint16
                                   .previous_sample_0(previous_sample),  // sfix32_En7
                                   .previous_sample_1(previous_sample_1),  // sfix32_En7
                                   .filter_output(digital_filter_out1),  // uint16
                                   .new_previous_sample(new_previous_sample)  // sfix32_En7
                                   );

  always @(posedge clk or posedge reset)
    begin : Memory7_process
      if (reset == 1'b1) begin
        filter_output <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          filter_output <= digital_filter_out1;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_ProcessedData_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(filter_output),
                                               .wr_addr(write_porcessing_addr_read_from_storage_addr_2),
                                               .wr_en(write_en),
                                               .rd_addr(rd_address),
                                               .wr_dout(ProcessedData_storage_out1),
                                               .rd_dout(ProcessedData_storage_out2)
                                               );

  assign rd_dout = ProcessedData_storage_out2;

endmodule  // process_and_retrieve
