// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v27\process_and_retrieve.v
// Created: 2021-10-14 15:24:50
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: process_and_retrieve
// Source Path: CustArch_v27/cust_architecture/process_and_retrieve
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
           OutputState,
           value_from_pipeIn,
           rd_dout);


  input   clk;
  input   reset;
  input   enb;
  input   [8:0] rd_address;  // ufix9
  input   [7:0] InputState;  // uint8
  input   [15:0] current_value;  // uint16
  input   [8:0] read_from_processing_addr;  // ufix9
  input   [7:0] OutputState;  // uint8
  input   [15:0] value_from_pipeIn;  // uint16
  output  [15:0] rd_dout;  // uint16


  reg [7:0] Memory6_out1;  // uint8
  wire switch_compare_1;
  reg [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  wire [15:0] pipe_in_interpret_out1_0;  // uint16
  wire [15:0] pipe_in_interpret_out1_1;  // uint16
  reg [7:0] Memory3_out1;  // uint8
  wire read_from_porcessing;
  reg  write_en;
  wire signed [15:0] new_previous_sample;  // sfix16_E3
  wire signed [15:0] PreviousData_storage_out1;  // sfix16_E3
  wire signed [15:0] previous_sample;  // sfix16_E3
  wire [15:0] filter_output;  // uint16
  wire [15:0] spike;  // uint16
  wire [15:0] ProcessedData_storage_out1;  // uint16
  wire [15:0] filtered_sample;  // uint16
  wire [15:0] Switch_out1;  // uint16

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
    begin : Memory6_process
      if (reset == 1'b1) begin
        Memory6_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Memory6_out1 <= OutputState;
        end
      end
    end



  assign switch_compare_1 = Memory6_out1 > 8'b00000110;



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



  pipe_in_interpret u_pipe_in_interpret (.clk(clk),
                                         .reset(reset),
                                         .enb(enb),
                                         .th_reading_address(write_porcessing_addr_read_from_storage_addr_1),  // ufix9
                                         .value_from_pipe_in(value_from_pipeIn),  // uint16
                                         .pipe_in_data_0(pipe_in_interpret_out1_0),  // uint16
                                         .pipe_in_data_1(pipe_in_interpret_out1_1)  // uint16
                                         );

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
                        .DataWidth(16)
                        )
                      u_PreviousData_storage (.clk(clk),
                                              .enb(enb),
                                              .wr_din(new_previous_sample),
                                              .wr_addr(write_porcessing_addr_read_from_storage_addr_1),
                                              .wr_en(write_en),
                                              .rd_addr(read_from_processing_addr),
                                              .wr_dout(PreviousData_storage_out1),
                                              .rd_dout(previous_sample)
                                              );

  one_pole_IIR u_one_pole_IIR (.current_sample(current_value),  // uint16
                               .previous_sample(previous_sample),  // sfix16_E3
                               .filter_output(filter_output),  // uint16
                               .new_previous_sample(new_previous_sample)  // sfix16_E3
                               );

  spike_detection u_spike_detection (.clk(clk),
                                     .reset(reset),
                                     .enb(enb),
                                     .pipe_in_data_0(pipe_in_interpret_out1_0),  // uint16
                                     .pipe_in_data_1(pipe_in_interpret_out1_1),  // uint16
                                     .filter_output(filter_output),  // uint16
                                     .write_porcessing_addr_read_from_storage_addr_1(write_porcessing_addr_read_from_storage_addr_1),  // ufix9
                                     .rd_addr(rd_address),  // ufix9
                                     .InputState(Memory3_out1),  // uint8
                                     .spike(spike)  // uint16
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
                                               .rd_dout(filtered_sample)
                                               );

  assign Switch_out1 = (switch_compare_1 == 1'b0 ? spike :
              filtered_sample);



  assign rd_dout = Switch_out1;

endmodule  // process_and_retrieve
