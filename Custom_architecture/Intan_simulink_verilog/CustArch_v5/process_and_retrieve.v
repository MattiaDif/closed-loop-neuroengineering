// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v5\process_and_retrieve.v
// Created: 2021-05-19 18:03:41
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: process_and_retrieve
// Source Path: CustArch_v5/cust_architecture/process_and_retrieve
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


  reg [15:0] Delay1_out1;  // uint16
  wire input_state_AC_TRUE;
  reg [8:0] reduced_reg [0:1];  // ufix9 [2]
  wire [8:0] reduced_reg_next [0:1];  // ufix9 [2]
  wire [8:0] read_from_processing_addr_1;  // ufix9
  wire Compare_To_Constant_out1;
  reg  [0:2] Delay3_reg;  // ufix1 [3]
  wire [0:2] Delay3_reg_next;  // ufix1 [3]
  wire input_state_AC_TRUE_3;
  wire Logical_Operator_out1;
  wire signed [15:0] new_previous_sample;  // sfix16_E3
  wire signed [15:0] previous_sample;  // sfix16_E3
  reg signed [15:0] Delay6_out1;  // sfix16_E3
  wire [15:0] processed_sample;  // uint16
  wire [15:0] DCtoAC_storage_out1;  // uint16
  wire [15:0] DCtoAC_storage_out2;  // uint16

  // delay for saving AC filtered data in the right 
  // AC address
  // 
  // The dual-port RAM in the input interpret subsystem introduces 
  // a sample of delay. Furthermore, I need one sample more of delay
  // due to the IIR filter --> overall delay of two samples
  // 
  // generally, all sample in the processing subsystem
  // are delayed by two samples --> the memories are 
  // needed for this reason
  // 
  // data processed for Output
  // 
  // rd_address from Output
  // 
  // Write in RAM only if an AC data
  // is available
  // 
  // note that these RAM are too big.. I just need 160 and thus a reduced address port width  
  // 
  // data sent for processing
  // 
  // address sent for processing
  // 
  // Note: If a read operation occurs simultaneously at the same address 
  // as a write operation, old data appears at the read output port (rd_dout).


  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        Delay1_out1 <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          Delay1_out1 <= current_value;
        end
      end
    end



  assign input_state_AC_TRUE = InputState == 8'b00000111;



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

  assign read_from_processing_addr_1 = reduced_reg[1];
  assign reduced_reg_next[0] = read_from_processing_addr;
  assign reduced_reg_next[1] = reduced_reg[0];



  assign Compare_To_Constant_out1 = read_from_processing_addr_1 == 9'b000000001;



  always @(posedge clk or posedge reset)
    begin : Delay3_process
      if (reset == 1'b1) begin
        Delay3_reg[0] <= 1'b0;
        Delay3_reg[1] <= 1'b0;
        Delay3_reg[2] <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay3_reg[0] <= Delay3_reg_next[0];
          Delay3_reg[1] <= Delay3_reg_next[1];
          Delay3_reg[2] <= Delay3_reg_next[2];
        end
      end
    end

  assign input_state_AC_TRUE_3 = Delay3_reg[2];
  assign Delay3_reg_next[0] = input_state_AC_TRUE;
  assign Delay3_reg_next[1] = Delay3_reg[0];
  assign Delay3_reg_next[2] = Delay3_reg[1];



  assign Logical_Operator_out1 = Compare_To_Constant_out1 & input_state_AC_TRUE_3;



  previous_sample_storage u_previous_sample_storage (.clk(clk),
                                                     .reset(reset),
                                                     .enb(enb),
                                                     .new_previous_sample(new_previous_sample),  // sfix16_E3
                                                     .input_state_AC_TRUE(input_state_AC_TRUE),
                                                     .write_porcessing_addr_read_from_storage_addr_1(read_from_processing_addr),  // ufix9
                                                     .previous_sample(previous_sample)  // sfix16_E3
                                                     );

  always @(posedge clk or posedge reset)
    begin : Delay6_process
      if (reset == 1'b1) begin
        Delay6_out1 <= 16'sb0000000000000000;
      end
      else begin
        if (enb) begin
          Delay6_out1 <= previous_sample;
        end
      end
    end



  digital_filter u_digital_filter (.clk(clk),
                                   .reset(reset),
                                   .enb(enb),
                                   .current_sample(Delay1_out1),  // uint16
                                   .previous_sample(Delay6_out1),  // sfix16_E3
                                   .Enable(Logical_Operator_out1),
                                   .filter_output(processed_sample),  // uint16
                                   .new_previous_sample(new_previous_sample)  // sfix16_E3
                                   );

  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_DCtoAC_storage (.clk(clk),
                                        .enb(enb),
                                        .wr_din(processed_sample),
                                        .wr_addr(read_from_processing_addr_1),
                                        .wr_en(Logical_Operator_out1),
                                        .rd_addr(rd_address_bit_set_AC_during_DC),
                                        .wr_dout(DCtoAC_storage_out1),
                                        .rd_dout(DCtoAC_storage_out2)
                                        );

  assign rd_dout = DCtoAC_storage_out2;

endmodule  // process_and_retrieve
