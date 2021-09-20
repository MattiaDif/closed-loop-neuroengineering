// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\buffer_counter.v
// Created: 2021-09-20 14:51:01
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: buffer_counter
// Source Path: CustArch_v22/cust_architecture/process_and_retrieve/spike_detection/LocalMaximaAdaptiveThreshold/RMS_computation/buffer_counter
// Hierarchy Level: 5
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module buffer_counter
          (clk,
           reset,
           enb,
           InputState,
           write_porcessing_addr_read_from_storage_addr_1,
           Out1);


  input   clk;
  input   reset;
  input   enb;
  input   InputState;
  input   [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  output  Out1;


  wire [15:0] count_rst_out1;  // uint16
  wire [31:0] count_rst_out1_dtc;  // uint32
  reg [8:0] Delay_out1;  // ufix9
  wire rst_check_out1;
  wire switch_compare_1;
  wire [31:0] square_sum_out2;  // uint32
  wire [31:0] Switch_out1;  // uint32
  wire [31:0] incr_1;  // ufix32
  wire [31:0] incr_out1;  // uint32
  wire [31:0] square_sum_out1;  // uint32


  assign count_rst_out1 = 16'b0000000000000000;



  assign count_rst_out1_dtc = {16'b0, count_rst_out1};



  always @(posedge clk or posedge reset)
    begin : Delay_process
      if (reset == 1'b1) begin
        Delay_out1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          Delay_out1 <= write_porcessing_addr_read_from_storage_addr_1;
        end
      end
    end



  assign switch_compare_1 = rst_check_out1 > 1'b0;



  assign Switch_out1 = (switch_compare_1 == 1'b0 ? square_sum_out2 :
              count_rst_out1_dtc);



  assign incr_1 = {31'b0, InputState};
  assign incr_out1 = incr_1 + Switch_out1;



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(32)
                        )
                      u_square_sum (.clk(clk),
                                    .enb(enb),
                                    .wr_din(incr_out1),
                                    .wr_addr(Delay_out1),
                                    .wr_en(InputState),
                                    .rd_addr(write_porcessing_addr_read_from_storage_addr_1),
                                    .wr_dout(square_sum_out1),
                                    .rd_dout(square_sum_out2)
                                    );

  assign rst_check_out1 = square_sum_out2 == 32'b00000000000000000000000000011110;



  assign Out1 = rst_check_out1;

endmodule  // buffer_counter

