// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v24\get_enabled_timings.v
// Created: 2021-09-23 14:17:17
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: get_enabled_timings
// Source Path: CustArch_v24/cust_architecture/output_interpret/Interpret_to_RAM_output/get_enabled_timings 
// Hierarchy Level: 3
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module get_enabled_timings
          (clk,
           reset,
           enb,
           OutputState,
           data_stream_en,
           source_channel_en,
           source_channel_0_159,
           source_device_enabled,
           source_device,
           converter_cnt);


  input   clk;
  input   reset;
  input   enb;
  input   [7:0] OutputState;  // uint8
  input   [7:0] data_stream_en;  // uint8
  output  source_channel_en;
  output  [7:0] source_channel_0_159;  // uint8
  output  source_device_enabled;  // ufix1
  output  [2:0] source_device;  // ufix3
  output  [2:0] converter_cnt;  // ufix3


  wire Compare_To_Constant2_out1;
  wire Compare_To_Constant1_out1;
  wire Compare_To_Constant_out1;
  reg [7:0] channel_counter_out1;  // uint8
  wire Compare_To_Zero_out1;
  wire switch_compare_1;
  reg  Delay2_out1;
  wire switch_compare_1_1;
  wire Detect_change_ch_cnt_out1;
  wire [7:0] next_chip_rollmap;  // uint8
  reg [7:0] Delay1_out1;  // uint8
  wire [7:0] Switch2_out1;  // uint8
  wire Compare_To_Constant3_out1;
  wire channel_flag_out1;
  wire [7:0] Switch1_out1;  // uint8
  wire Bit_Slice1_out1;  // ufix1
  wire enable_source_channel_out1;
  wire Compare_To_Constant4_out1;
  wire Compare_To_Constant5_out1;
  wire AND1_out1;
  reg [7:0] stimulators_counter_out1;  // uint8
  wire y;  // ufix1
  wire [7:0] reading_address_out2;  // uint8
  wire [2:0] Bit_Slice2_out1;  // ufix3
  wire Compare_To_Constant6_out1;
  wire Compare_To_Constant7_out1;
  wire AND2_out1;
  reg [7:0] ADC_DAC_counter_out1;  // uint8
  wire [2:0] Bit_Slice3_out1;  // ufix3

  // reading channel data flag
  // 
  // memories necessary due to
  // a feedback loop
  // 
  // if channel_counter == 0
  // take data_stream_enable
  // else take next_chip_rollmap
  // 
  // amplifier chip under 
  // processing
  // 
  // According to the number of enabled data_stream
  // I set the right on/off, settle, etc. address for the next
  // RAM reading 
  // 
  // convert the string
  // in a boolean
  // 
  // True if you are reading AC/DC channel data
  // 
  // enabling it only if you're reading AC/DC channel data
  // 
  // TTL data use constant specific addresses
  // 
  // reading stimulator data flag
  // 
  // 
  // counter of the converters


  assign Compare_To_Constant2_out1 = OutputState == 8'b00000110;



  assign Compare_To_Constant1_out1 = OutputState == 8'b00001000;



  assign Compare_To_Constant_out1 = OutputState == 8'b00000111;



  // Count limited, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  //  count to value  = 159
  always @(posedge clk or posedge reset)
    begin : channel_counter_process
      if (reset == 1'b1) begin
        channel_counter_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          if (Compare_To_Constant1_out1 == 1'b1) begin
            channel_counter_out1 <= 8'b00000000;
          end
          else if (Compare_To_Constant_out1 == 1'b1) begin
            if (channel_counter_out1 >= 8'b10011111) begin
              channel_counter_out1 <= 8'b00000000;
            end
            else begin
              channel_counter_out1 <= channel_counter_out1 + 8'b00000001;
            end
          end
        end
      end
    end



  assign Compare_To_Zero_out1 = channel_counter_out1 == 8'b00000000;



  assign switch_compare_1 = Compare_To_Zero_out1 > 1'b0;



  always @(posedge clk or posedge reset)
    begin : Delay2_process
      if (reset == 1'b1) begin
        Delay2_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay2_out1 <= Compare_To_Zero_out1;
        end
      end
    end



  assign switch_compare_1_1 = Delay2_out1 > 1'b0;



  Detect_change_ch_cnt u_Detect_change_ch_cnt (.clk(clk),
                                               .reset(reset),
                                               .enb(enb),
                                               .U(channel_counter_out1),  // uint8
                                               .Y(Detect_change_ch_cnt_out1)
                                               );

  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        Delay1_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Delay1_out1 <= next_chip_rollmap;
        end
      end
    end



  assign Switch2_out1 = (switch_compare_1_1 == 1'b0 ? Delay1_out1 :
              data_stream_en);



  find_next_chip_rollmap_block u_find_next_chip_rollmap (.clk(clk),
                                                         .reset(reset),
                                                         .enb(enb),
                                                         .In1(Switch2_out1),  // uint8
                                                         .Trigger(Detect_change_ch_cnt_out1),
                                                         .Out1(next_chip_rollmap)  // uint8
                                                         );

  assign Compare_To_Constant3_out1 = OutputState == 8'b00000111;



  assign channel_flag_out1 = Compare_To_Constant2_out1 | Compare_To_Constant3_out1;



  assign Switch1_out1 = (switch_compare_1 == 1'b0 ? next_chip_rollmap :
              data_stream_en);



  assign Bit_Slice1_out1 = Switch1_out1[0];



  assign enable_source_channel_out1 = channel_flag_out1 & Bit_Slice1_out1;



  assign source_channel_en = enable_source_channel_out1;

  assign source_channel_0_159 = channel_counter_out1;

  assign Compare_To_Constant4_out1 = OutputState >= 8'b00001000;



  assign Compare_To_Constant5_out1 = OutputState <= 8'b00001011;



  assign AND1_out1 = Compare_To_Constant4_out1 & Compare_To_Constant5_out1;



  // Count limited, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  //  count to value  = 7
  always @(posedge clk or posedge reset)
    begin : stimulators_counter_process
      if (reset == 1'b1) begin
        stimulators_counter_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          if (AND1_out1 == 1'b1) begin
            if (stimulators_counter_out1 >= 8'b00000111) begin
              stimulators_counter_out1 <= 8'b00000000;
            end
            else begin
              stimulators_counter_out1 <= stimulators_counter_out1 + 8'b00000001;
            end
          end
        end
      end
    end



  reading_address u_reading_address (.counter(stimulators_counter_out1),  // uint8
                                     .data_stream_en(data_stream_en),  // uint8
                                     .source_device_en(y),  // ufix1
                                     .source_device(reading_address_out2)  // uint8
                                     );

  assign source_device_enabled = y;

  assign Bit_Slice2_out1 = reading_address_out2[2:0];



  assign source_device = Bit_Slice2_out1;

  assign Compare_To_Constant6_out1 = OutputState >= 8'b00001100;



  assign Compare_To_Constant7_out1 = OutputState <= 8'b00001101;



  assign AND2_out1 = Compare_To_Constant6_out1 & Compare_To_Constant7_out1;



  // Count limited, Unsigned Counter
  //  initial value   = 0
  //  step value      = 1
  //  count to value  = 7
  always @(posedge clk or posedge reset)
    begin : ADC_DAC_counter_process
      if (reset == 1'b1) begin
        ADC_DAC_counter_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          if (AND2_out1 == 1'b1) begin
            if (ADC_DAC_counter_out1 >= 8'b00000111) begin
              ADC_DAC_counter_out1 <= 8'b00000000;
            end
            else begin
              ADC_DAC_counter_out1 <= ADC_DAC_counter_out1 + 8'b00000001;
            end
          end
        end
      end
    end



  assign Bit_Slice3_out1 = ADC_DAC_counter_out1[2:0];



  assign converter_cnt = Bit_Slice3_out1;

endmodule  // get_enabled_timings

