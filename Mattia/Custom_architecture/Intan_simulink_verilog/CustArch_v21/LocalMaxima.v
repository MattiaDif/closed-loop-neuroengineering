// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v21\LocalMaxima.v
// Created: 2021-10-18 17:44:13
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: LocalMaxima
// Source Path: CustArch_v21/cust_architecture/process_and_retrieve/spike_detection/LocalMaxima
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module LocalMaxima
          (clk,
           reset,
           enb_1_2_0,
           filter_output,
           wr_en_filt,
           write_porcessing_addr_read_from_storage_addr_1,
           local_maxima_out);


  input   clk;
  input   reset;
  input   enb_1_2_0;
  input   [15:0] filter_output;  // uint16
  input   wr_en_filt;
  input   [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  output  local_maxima_out;


  reg [8:0] write_porcessing_addr_read_from_storage_addr_1_1;  // ufix9
  wire [15:0] sample_t_1_out1;  // uint16
  wire [15:0] sample_t_1_out2;  // uint16
  wire read_from_porcessing;
  wire [15:0] sample_t_2_out1;  // uint16
  wire [15:0] sample_t_2_out2;  // uint16
  wire check_t_1_relop1;
  wire check_t_2_relop1;
  wire Logical_Operator1_out1;


  always @(posedge clk or posedge reset)
    begin : reduced_process
      if (reset == 1'b1) begin
        write_porcessing_addr_read_from_storage_addr_1_1 <= 9'b000000000;
      end
      else begin
        if (enb_1_2_0) begin
          write_porcessing_addr_read_from_storage_addr_1_1 <= write_porcessing_addr_read_from_storage_addr_1;
        end
      end
    end



  DualPortRAM_generic_block #(.AddrWidth(9),
                              .DataWidth(16)
                              )
                            u_sample_t_1 (.clk(clk),
                                          .enb_1_2_0(enb_1_2_0),
                                          .wr_din(filter_output),
                                          .wr_addr(write_porcessing_addr_read_from_storage_addr_1_1),
                                          .wr_en(wr_en_filt),
                                          .rd_addr(write_porcessing_addr_read_from_storage_addr_1),
                                          .wr_dout(sample_t_1_out1),
                                          .rd_dout(sample_t_1_out2)
                                          );

  assign read_from_porcessing = sample_t_1_out2 <= 16'b1000100010111000;



  DualPortRAM_generic_block #(.AddrWidth(9),
                              .DataWidth(16)
                              )
                            u_sample_t_2 (.clk(clk),
                                          .enb_1_2_0(enb_1_2_0),
                                          .wr_din(sample_t_1_out2),
                                          .wr_addr(write_porcessing_addr_read_from_storage_addr_1_1),
                                          .wr_en(wr_en_filt),
                                          .rd_addr(write_porcessing_addr_read_from_storage_addr_1),
                                          .wr_dout(sample_t_2_out1),
                                          .rd_dout(sample_t_2_out2)
                                          );

  assign check_t_1_relop1 = sample_t_1_out2 < sample_t_2_out2;



  assign check_t_2_relop1 = sample_t_1_out2 < filter_output;



  assign Logical_Operator1_out1 = check_t_2_relop1 & (read_from_porcessing & check_t_1_relop1);



  assign local_maxima_out = Logical_Operator1_out1;

endmodule  // LocalMaxima

