// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\pipe_in_v4\process_and_retrieve.v
// Created: 2021-09-10 15:49:10
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: process_and_retrieve
// Source Path: pipe_in_v4/cust_architecture/process_and_retrieve
// Hierarchy Level: 1
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module process_and_retrieve
          (clk,
           reset,
           enb,
           rd_address_bit_set_AC_during_DC,
           InputState,
           current_value,
           read_from_processing_addr,
           rd_dout);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] rd_address_bit_set_AC_during_DC;  // ufix9
  input   [7:0] InputState;  // uint8
  input   [15:0] current_value;  // uint16
  input   [8:0] read_from_processing_addr;  // ufix9
  output  [15:0] rd_dout;  // uint16


  reg [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  reg [7:0] Memory3_out1;  // uint8
  wire input_state_AC_1;
  reg  Memory2_out1;
  wire [15:0] DCtoAC_storage_out1;  // uint16
  wire [15:0] DCtoAC_storage_out2;  // uint16

  // address sent for processing
  // 
  // data sent for processing
  // 
  // Note: If a read operation occurs simultaneously at the same address 
  // as a write operation, old data appears at the read output port (rd_dout).
  // 
  // note that these RAM are too big.. I just need 160 and thus a reduced address port width  
  // 
  // this memory is to guarantee that the input is already stored
  // 
  // Write in RAM only if an AC data
  // is available
  // 
  // rd_address from Output
  // 
  // data processed for Output
  // 
  // generally, all sample in the processing subsystem
  // are delayed by two samples --> the memories are 
  // needed for this reason
  // 
  // The dual-port RAM in the input interpret subsystem introduces 
  // a sample of delay. Furthermore, I need one sample more of delay
  // due to the IIR filter --> overall delay of two samples


  // memory for saving AC filtered data in the DC
  // address for the next offline comparison
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



  assign input_state_AC_1 = Memory3_out1 == 8'b00000111;



  always @(posedge clk or posedge reset)
    begin : Memory2_process
      if (reset == 1'b1) begin
        Memory2_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Memory2_out1 <= input_state_AC_1;
        end
      end
    end



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_DCtoAC_storage (.clk(clk),
                                        .enb(enb),
                                        .wr_din(current_value),
                                        .wr_addr(write_porcessing_addr_read_from_storage_addr_1),
                                        .wr_en(Memory2_out1),
                                        .rd_addr(rd_address_bit_set_AC_during_DC),
                                        .wr_dout(DCtoAC_storage_out1),
                                        .rd_dout(DCtoAC_storage_out2)
                                        );

  assign rd_dout = DCtoAC_storage_out2;

endmodule  // process_and_retrieve

