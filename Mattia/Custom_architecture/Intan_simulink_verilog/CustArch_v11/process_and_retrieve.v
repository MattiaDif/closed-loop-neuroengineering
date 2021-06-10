// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v11\process_and_retrieve.v
// Created: 2021-05-28 09:27:01
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: process_and_retrieve
// Source Path: CustArch_v11/cust_architecture/process_and_retrieve
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


  reg [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  reg [7:0] Memory3_out1;  // uint8
  wire read_from_porcessing;
  reg  write_en;
  wire signed [31:0] new_previous_sample;  // sfix32_En7
  wire signed [31:0] PreviousData1_storage_out1;  // sfix32_En7
  wire signed [31:0] previous_sample;  // sfix32_En7
  wire [15:0] filter_output;  // uint16
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
  // 
  // delay for saving AC filtered data in the right 
  // AC address


  always @(posedge clk or posedge reset)
    begin : Memory5_process
      if (reset == 1'b1) begin
        write_porcessing_addr_read_from_storage_addr_1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          write_porcessing_addr_read_from_storage_addr_1 <= read_from_processing_addr;
        end
      end
    end



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
    begin : Memory2_process
      if (reset == 1'b1) begin
        write_en <= 1'b0;
      end
      else begin
        if (enb) begin
          write_en <= read_from_porcessing;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_PreviousData1_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(new_previous_sample),
                                               .wr_addr(write_porcessing_addr_read_from_storage_addr_1),
                                               .wr_en(write_en),
                                               .rd_addr(read_from_processing_addr),
                                               .wr_dout(PreviousData1_storage_out1),
                                               .rd_dout(previous_sample)
                                               );

  digital_filter u_digital_filter (.current_sample(current_value),  // uint16
                                   .previous_sample(previous_sample),  // sfix32_En7
                                   .filter_output(filter_output),  // uint16
                                   .new_previous_sample(new_previous_sample)  // sfix32_En7
                                   );

  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_ProcessedData_storage (.clk(clk),
                                               .enb(enb),
                                               .wr_din(filter_output),
                                               .wr_addr(write_porcessing_addr_read_from_storage_addr_1),
                                               .wr_en(write_en),
                                               .rd_addr(rd_address),
                                               .wr_dout(ProcessedData_storage_out1),
                                               .rd_dout(ProcessedData_storage_out2)
                                               );

  assign rd_dout = ProcessedData_storage_out2;

endmodule  // process_and_retrieve

