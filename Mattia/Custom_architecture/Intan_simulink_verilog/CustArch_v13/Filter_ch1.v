// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v13\Filter_ch1.v
// Created: 2021-06-18 09:34:56
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Filter_ch1
// Source Path: CustArch_v13/cust_architecture/process_and_retrieve/Filter_ch1
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Filter_ch1
          (clk,
           reset,
           enb,
           enb_1_8_1,
           raw_input_0,
           raw_input_1,
           raw_input_2,
           raw_input_3,
           raw_input_4,
           raw_input_5,
           raw_input_6,
           raw_input_7,
           Enable,
           S);


  input   clk;
  input   reset;
  input   enb;
  input   enb_1_8_1;
  input   [15:0] raw_input_0;  // uint16
  input   [15:0] raw_input_1;  // uint16
  input   [15:0] raw_input_2;  // uint16
  input   [15:0] raw_input_3;  // uint16
  input   [15:0] raw_input_4;  // uint16
  input   [15:0] raw_input_5;  // uint16
  input   [15:0] raw_input_6;  // uint16
  input   [15:0] raw_input_7;  // uint16
  input   Enable;
  output  [15:0] S;  // uint16


  wire [15:0] digital_filter_out1_0;  // uint16
  wire [15:0] digital_filter_out1_1;  // uint16
  wire [15:0] digital_filter_out1_2;  // uint16
  wire [15:0] digital_filter_out1_3;  // uint16
  wire [15:0] digital_filter_out1_4;  // uint16
  wire [15:0] digital_filter_out1_5;  // uint16
  wire [15:0] digital_filter_out1_6;  // uint16
  wire [15:0] digital_filter_out1_7;  // uint16
  wire [15:0] digital_filter_out1 [0:7];  // uint16 [8]
  reg [15:0] Rate_Transition_bypass_reg [0:7];  // ufix16 [8]
  wire [15:0] Rate_Transition_bypass_reg_next [0:7];  // ufix16 [8]
  wire [15:0] Rate_Transition_out1 [0:7];  // uint16 [8]
  reg [2:0] Serializer1D_contl_cnt;  // ufix3
  wire Serializer1D_invldSignal;
  wire [15:0] serial_in_1 [0:7];  // uint16 [8]
  reg [15:0] Serializer1D_data [0:6];  // ufix16 [7]
  wire [15:0] Serializer1D_data_next [0:6];  // ufix16 [7]
  wire [15:0] Serializer1D_data_temp [0:6];  // ufix16 [7]
  wire [15:0] serializer_PostProcessed;  // uint16


  digital_filter u_digital_filter (.clk(clk),
                                   .reset(reset),
                                   .enb(enb),
                                   .raw_input_0(raw_input_0),  // uint16
                                   .raw_input_1(raw_input_1),  // uint16
                                   .raw_input_2(raw_input_2),  // uint16
                                   .raw_input_3(raw_input_3),  // uint16
                                   .raw_input_4(raw_input_4),  // uint16
                                   .raw_input_5(raw_input_5),  // uint16
                                   .raw_input_6(raw_input_6),  // uint16
                                   .raw_input_7(raw_input_7),  // uint16
                                   .Enable(Enable),
                                   .filter_output_0(digital_filter_out1_0),  // uint16
                                   .filter_output_1(digital_filter_out1_1),  // uint16
                                   .filter_output_2(digital_filter_out1_2),  // uint16
                                   .filter_output_3(digital_filter_out1_3),  // uint16
                                   .filter_output_4(digital_filter_out1_4),  // uint16
                                   .filter_output_5(digital_filter_out1_5),  // uint16
                                   .filter_output_6(digital_filter_out1_6),  // uint16
                                   .filter_output_7(digital_filter_out1_7)  // uint16
                                   );

  assign digital_filter_out1[0] = digital_filter_out1_0;
  assign digital_filter_out1[1] = digital_filter_out1_1;
  assign digital_filter_out1[2] = digital_filter_out1_2;
  assign digital_filter_out1[3] = digital_filter_out1_3;
  assign digital_filter_out1[4] = digital_filter_out1_4;
  assign digital_filter_out1[5] = digital_filter_out1_5;
  assign digital_filter_out1[6] = digital_filter_out1_6;
  assign digital_filter_out1[7] = digital_filter_out1_7;

  always @(posedge clk or posedge reset)
    begin : Rate_Transition_bypass_process
      if (reset == 1'b1) begin
        Rate_Transition_bypass_reg[0] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[1] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[2] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[3] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[4] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[5] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[6] <= 16'b0000000000000000;
        Rate_Transition_bypass_reg[7] <= 16'b0000000000000000;
      end
      else begin
        if (enb_1_8_1) begin
          Rate_Transition_bypass_reg[0] <= Rate_Transition_bypass_reg_next[0];
          Rate_Transition_bypass_reg[1] <= Rate_Transition_bypass_reg_next[1];
          Rate_Transition_bypass_reg[2] <= Rate_Transition_bypass_reg_next[2];
          Rate_Transition_bypass_reg[3] <= Rate_Transition_bypass_reg_next[3];
          Rate_Transition_bypass_reg[4] <= Rate_Transition_bypass_reg_next[4];
          Rate_Transition_bypass_reg[5] <= Rate_Transition_bypass_reg_next[5];
          Rate_Transition_bypass_reg[6] <= Rate_Transition_bypass_reg_next[6];
          Rate_Transition_bypass_reg[7] <= Rate_Transition_bypass_reg_next[7];
        end
      end
    end

  assign Rate_Transition_out1[0] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[0] :
              Rate_Transition_bypass_reg[0]);
  assign Rate_Transition_out1[1] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[1] :
              Rate_Transition_bypass_reg[1]);
  assign Rate_Transition_out1[2] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[2] :
              Rate_Transition_bypass_reg[2]);
  assign Rate_Transition_out1[3] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[3] :
              Rate_Transition_bypass_reg[3]);
  assign Rate_Transition_out1[4] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[4] :
              Rate_Transition_bypass_reg[4]);
  assign Rate_Transition_out1[5] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[5] :
              Rate_Transition_bypass_reg[5]);
  assign Rate_Transition_out1[6] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[6] :
              Rate_Transition_bypass_reg[6]);
  assign Rate_Transition_out1[7] = (enb_1_8_1 == 1'b1 ? digital_filter_out1[7] :
              Rate_Transition_bypass_reg[7]);
  assign Rate_Transition_bypass_reg_next[0] = digital_filter_out1[0];
  assign Rate_Transition_bypass_reg_next[1] = digital_filter_out1[1];
  assign Rate_Transition_bypass_reg_next[2] = digital_filter_out1[2];
  assign Rate_Transition_bypass_reg_next[3] = digital_filter_out1[3];
  assign Rate_Transition_bypass_reg_next[4] = digital_filter_out1[4];
  assign Rate_Transition_bypass_reg_next[5] = digital_filter_out1[5];
  assign Rate_Transition_bypass_reg_next[6] = digital_filter_out1[6];
  assign Rate_Transition_bypass_reg_next[7] = digital_filter_out1[7];



  always @(posedge clk or posedge reset)
    begin : Serializer1D_contl_process
      if (reset == 1'b1) begin
        Serializer1D_contl_cnt <= 3'b000;
      end
      else begin
        if (enb) begin
          if (Serializer1D_contl_cnt == 3'b111) begin
            Serializer1D_contl_cnt <= 3'b000;
          end
          else begin
            Serializer1D_contl_cnt <= Serializer1D_contl_cnt + 3'b001;
          end
        end
      end
    end

  assign Serializer1D_invldSignal = Serializer1D_contl_cnt == 3'b000;



  assign serial_in_1[0] = Rate_Transition_out1[0];
  assign serial_in_1[1] = Rate_Transition_out1[1];
  assign serial_in_1[2] = Rate_Transition_out1[2];
  assign serial_in_1[3] = Rate_Transition_out1[3];
  assign serial_in_1[4] = Rate_Transition_out1[4];
  assign serial_in_1[5] = Rate_Transition_out1[5];
  assign serial_in_1[6] = Rate_Transition_out1[6];
  assign serial_in_1[7] = Rate_Transition_out1[7];

  always @(posedge clk or posedge reset)
    begin : Serializer1D_process
      if (reset == 1'b1) begin
        Serializer1D_data[0] <= 16'b0000000000000000;
        Serializer1D_data[1] <= 16'b0000000000000000;
        Serializer1D_data[2] <= 16'b0000000000000000;
        Serializer1D_data[3] <= 16'b0000000000000000;
        Serializer1D_data[4] <= 16'b0000000000000000;
        Serializer1D_data[5] <= 16'b0000000000000000;
        Serializer1D_data[6] <= 16'b0000000000000000;
      end
      else begin
        if (enb) begin
          Serializer1D_data[0] <= Serializer1D_data_next[0];
          Serializer1D_data[1] <= Serializer1D_data_next[1];
          Serializer1D_data[2] <= Serializer1D_data_next[2];
          Serializer1D_data[3] <= Serializer1D_data_next[3];
          Serializer1D_data[4] <= Serializer1D_data_next[4];
          Serializer1D_data[5] <= Serializer1D_data_next[5];
          Serializer1D_data[6] <= Serializer1D_data_next[6];
        end
      end
    end

  assign serializer_PostProcessed = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[0] :
              Serializer1D_data[0]);
  assign Serializer1D_data_temp[0] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[1] :
              Serializer1D_data[1]);
  assign Serializer1D_data_temp[1] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[2] :
              Serializer1D_data[2]);
  assign Serializer1D_data_temp[2] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[3] :
              Serializer1D_data[3]);
  assign Serializer1D_data_temp[3] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[4] :
              Serializer1D_data[4]);
  assign Serializer1D_data_temp[4] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[5] :
              Serializer1D_data[5]);
  assign Serializer1D_data_temp[5] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[6] :
              Serializer1D_data[6]);
  assign Serializer1D_data_temp[6] = (Serializer1D_invldSignal != 1'b0 ? serial_in_1[7] :
              serial_in_1[7]);
  assign Serializer1D_data_next[0] = Serializer1D_data_temp[0];
  assign Serializer1D_data_next[1] = Serializer1D_data_temp[1];
  assign Serializer1D_data_next[2] = Serializer1D_data_temp[2];
  assign Serializer1D_data_next[3] = Serializer1D_data_temp[3];
  assign Serializer1D_data_next[4] = Serializer1D_data_temp[4];
  assign Serializer1D_data_next[5] = Serializer1D_data_temp[5];
  assign Serializer1D_data_next[6] = Serializer1D_data_temp[6];



  assign S = serializer_PostProcessed;

endmodule  // Filter_ch1

