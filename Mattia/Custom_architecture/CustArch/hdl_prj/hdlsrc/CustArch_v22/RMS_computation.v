// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\RMS_computation.v
// Created: 2021-09-20 14:51:01
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: RMS_computation
// Source Path: CustArch_v22/cust_architecture/process_and_retrieve/spike_detection/LocalMaximaAdaptiveThreshold/RMS_computation
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module RMS_computation
          (clk,
           reset,
           enb,
           filter_output,
           write_porcessing_addr_read_from_storage_addr_1,
           InputState,
           adaptive_th);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] filter_output;  // uint16
  input   [8:0] write_porcessing_addr_read_from_storage_addr_1;  // ufix9
  input   InputState;
  output  [15:0] adaptive_th;  // uint16


  wire buffer_counter_out1;
  wire switch_compare_1;
  wire [63:0] count_rst_out1;  // ufix64
  wire [31:0] Product_out1;  // uint32
  reg [8:0] write_porcessing_addr_read_from_storage_addr_1_1;  // ufix9
  wire [63:0] square_sum;  // ufix64
  wire [63:0] Switch_out1;  // ufix64
  wire [63:0] Sum_1;  // ufix64
  wire [63:0] Sum_out1;  // ufix64
  wire [63:0] square_sum_out1;  // ufix64
  wire [127:0] n_mul_temp;  // ufix128_En68
  wire [39:0] square_sum_n;  // ufix40
  wire [15:0] rms;  // uint16
  wire [15:0] adaptive_th_1;  // uint16
  reg [8:0] reduced_reg [0:15];  // ufix9 [16]
  wire [8:0] reduced_reg_next [0:15];  // ufix9 [16]
  wire [8:0] write_porcessing_addr_read_from_storage_addr_1_2;  // ufix9
  reg [8:0] write_porcessing_addr_read_from_storage_addr_1_3;  // ufix9
  reg  [0:16] Delay1_reg;  // ufix1 [17]
  wire [0:16] Delay1_reg_next;  // ufix1 [17]
  wire Delay1_out1;
  wire [15:0] rms_out1;  // uint16
  wire [15:0] rms_out2;  // uint16


  buffer_counter u_buffer_counter (.clk(clk),
                                   .reset(reset),
                                   .enb(enb),
                                   .InputState(InputState),
                                   .write_porcessing_addr_read_from_storage_addr_1(write_porcessing_addr_read_from_storage_addr_1),  // ufix9
                                   .Out1(buffer_counter_out1)
                                   );

  assign switch_compare_1 = buffer_counter_out1 > 1'b0;



  assign count_rst_out1 = 64'h0000000000000000;



  assign Product_out1 = filter_output * filter_output;



  always @(posedge clk or posedge reset)
    begin : reduced_process
      if (reset == 1'b1) begin
        write_porcessing_addr_read_from_storage_addr_1_1 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          write_porcessing_addr_read_from_storage_addr_1_1 <= write_porcessing_addr_read_from_storage_addr_1;
        end
      end
    end



  assign Switch_out1 = (switch_compare_1 == 1'b0 ? square_sum :
              count_rst_out1);



  assign Sum_1 = {32'b0, Product_out1};
  assign Sum_out1 = Switch_out1 + Sum_1;



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(64)
                        )
                      u_square_sum (.clk(clk),
                                    .enb(enb),
                                    .wr_din(Sum_out1),
                                    .wr_addr(write_porcessing_addr_read_from_storage_addr_1_1),
                                    .wr_en(InputState),
                                    .rd_addr(write_porcessing_addr_read_from_storage_addr_1),
                                    .wr_dout(square_sum_out1),
                                    .rd_dout(square_sum)
                                    );

  assign n_mul_temp = 64'h8888888888888800 * square_sum;
  assign square_sum_n = n_mul_temp[107:68];



  Square_Root1 u_Square_Root1 (.din(square_sum_n),  // ufix40
                               .dout(rms)  // uint16
                               );

  adaptive_th_computation u_adaptive_th_computation (.u(rms),  // uint16
                                                     .Out1(adaptive_th_1)  // uint16
                                                     );

  always @(posedge clk or posedge reset)
    begin : reduced_1_process
      if (reset == 1'b1) begin
        reduced_reg[0] <= 9'b000000000;
        reduced_reg[1] <= 9'b000000000;
        reduced_reg[2] <= 9'b000000000;
        reduced_reg[3] <= 9'b000000000;
        reduced_reg[4] <= 9'b000000000;
        reduced_reg[5] <= 9'b000000000;
        reduced_reg[6] <= 9'b000000000;
        reduced_reg[7] <= 9'b000000000;
        reduced_reg[8] <= 9'b000000000;
        reduced_reg[9] <= 9'b000000000;
        reduced_reg[10] <= 9'b000000000;
        reduced_reg[11] <= 9'b000000000;
        reduced_reg[12] <= 9'b000000000;
        reduced_reg[13] <= 9'b000000000;
        reduced_reg[14] <= 9'b000000000;
        reduced_reg[15] <= 9'b000000000;
      end
      else begin
        if (enb) begin
          reduced_reg[0] <= reduced_reg_next[0];
          reduced_reg[1] <= reduced_reg_next[1];
          reduced_reg[2] <= reduced_reg_next[2];
          reduced_reg[3] <= reduced_reg_next[3];
          reduced_reg[4] <= reduced_reg_next[4];
          reduced_reg[5] <= reduced_reg_next[5];
          reduced_reg[6] <= reduced_reg_next[6];
          reduced_reg[7] <= reduced_reg_next[7];
          reduced_reg[8] <= reduced_reg_next[8];
          reduced_reg[9] <= reduced_reg_next[9];
          reduced_reg[10] <= reduced_reg_next[10];
          reduced_reg[11] <= reduced_reg_next[11];
          reduced_reg[12] <= reduced_reg_next[12];
          reduced_reg[13] <= reduced_reg_next[13];
          reduced_reg[14] <= reduced_reg_next[14];
          reduced_reg[15] <= reduced_reg_next[15];
        end
      end
    end

  assign write_porcessing_addr_read_from_storage_addr_1_2 = reduced_reg[15];
  assign reduced_reg_next[0] = write_porcessing_addr_read_from_storage_addr_1_1;
  assign reduced_reg_next[1] = reduced_reg[0];
  assign reduced_reg_next[2] = reduced_reg[1];
  assign reduced_reg_next[3] = reduced_reg[2];
  assign reduced_reg_next[4] = reduced_reg[3];
  assign reduced_reg_next[5] = reduced_reg[4];
  assign reduced_reg_next[6] = reduced_reg[5];
  assign reduced_reg_next[7] = reduced_reg[6];
  assign reduced_reg_next[8] = reduced_reg[7];
  assign reduced_reg_next[9] = reduced_reg[8];
  assign reduced_reg_next[10] = reduced_reg[9];
  assign reduced_reg_next[11] = reduced_reg[10];
  assign reduced_reg_next[12] = reduced_reg[11];
  assign reduced_reg_next[13] = reduced_reg[12];
  assign reduced_reg_next[14] = reduced_reg[13];
  assign reduced_reg_next[15] = reduced_reg[14];



  always @(posedge clk or posedge reset)
    begin : reduced_2_process
      if (reset == 1'b1) begin
        write_porcessing_addr_read_from_storage_addr_1_3 <= 9'b000000000;
      end
      else begin
        if (enb) begin
          write_porcessing_addr_read_from_storage_addr_1_3 <= write_porcessing_addr_read_from_storage_addr_1_2;
        end
      end
    end



  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        Delay1_reg[0] <= 1'b0;
        Delay1_reg[1] <= 1'b0;
        Delay1_reg[2] <= 1'b0;
        Delay1_reg[3] <= 1'b0;
        Delay1_reg[4] <= 1'b0;
        Delay1_reg[5] <= 1'b0;
        Delay1_reg[6] <= 1'b0;
        Delay1_reg[7] <= 1'b0;
        Delay1_reg[8] <= 1'b0;
        Delay1_reg[9] <= 1'b0;
        Delay1_reg[10] <= 1'b0;
        Delay1_reg[11] <= 1'b0;
        Delay1_reg[12] <= 1'b0;
        Delay1_reg[13] <= 1'b0;
        Delay1_reg[14] <= 1'b0;
        Delay1_reg[15] <= 1'b0;
        Delay1_reg[16] <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay1_reg[0] <= Delay1_reg_next[0];
          Delay1_reg[1] <= Delay1_reg_next[1];
          Delay1_reg[2] <= Delay1_reg_next[2];
          Delay1_reg[3] <= Delay1_reg_next[3];
          Delay1_reg[4] <= Delay1_reg_next[4];
          Delay1_reg[5] <= Delay1_reg_next[5];
          Delay1_reg[6] <= Delay1_reg_next[6];
          Delay1_reg[7] <= Delay1_reg_next[7];
          Delay1_reg[8] <= Delay1_reg_next[8];
          Delay1_reg[9] <= Delay1_reg_next[9];
          Delay1_reg[10] <= Delay1_reg_next[10];
          Delay1_reg[11] <= Delay1_reg_next[11];
          Delay1_reg[12] <= Delay1_reg_next[12];
          Delay1_reg[13] <= Delay1_reg_next[13];
          Delay1_reg[14] <= Delay1_reg_next[14];
          Delay1_reg[15] <= Delay1_reg_next[15];
          Delay1_reg[16] <= Delay1_reg_next[16];
        end
      end
    end

  assign Delay1_out1 = Delay1_reg[16];
  assign Delay1_reg_next[0] = buffer_counter_out1;
  assign Delay1_reg_next[1] = Delay1_reg[0];
  assign Delay1_reg_next[2] = Delay1_reg[1];
  assign Delay1_reg_next[3] = Delay1_reg[2];
  assign Delay1_reg_next[4] = Delay1_reg[3];
  assign Delay1_reg_next[5] = Delay1_reg[4];
  assign Delay1_reg_next[6] = Delay1_reg[5];
  assign Delay1_reg_next[7] = Delay1_reg[6];
  assign Delay1_reg_next[8] = Delay1_reg[7];
  assign Delay1_reg_next[9] = Delay1_reg[8];
  assign Delay1_reg_next[10] = Delay1_reg[9];
  assign Delay1_reg_next[11] = Delay1_reg[10];
  assign Delay1_reg_next[12] = Delay1_reg[11];
  assign Delay1_reg_next[13] = Delay1_reg[12];
  assign Delay1_reg_next[14] = Delay1_reg[13];
  assign Delay1_reg_next[15] = Delay1_reg[14];
  assign Delay1_reg_next[16] = Delay1_reg[15];



  DualPortRAM_generic #(.AddrWidth(9),
                        .DataWidth(16)
                        )
                      u_rms (.clk(clk),
                             .enb(enb),
                             .wr_din(adaptive_th_1),
                             .wr_addr(write_porcessing_addr_read_from_storage_addr_1_3),
                             .wr_en(Delay1_out1),
                             .rd_addr(write_porcessing_addr_read_from_storage_addr_1_2),
                             .wr_dout(rms_out1),
                             .rd_dout(rms_out2)
                             );

  assign adaptive_th = rms_out2;

endmodule  // RMS_computation
