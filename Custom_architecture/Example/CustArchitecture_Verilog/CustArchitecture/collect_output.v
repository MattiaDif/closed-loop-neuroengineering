// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArchitecture\collect_output.v
// Created: 2021-10-26 14:22:20
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: collect_output
// Source Path: CustArchitecture/cust_architecture/output_interpret_collect_output/collect_output
// Hierarchy Level: 2
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module collect_output
          (clk,
           reset,
           enb,
           FIFO_data_in,
           FIFO_write_to,
           InputState,
           OutputState,
           rd_enable,
           rd_address,
           read_from_storage_value,
           rd_dout,
           comp_data_out_valid,
           comp_data_out,
           rd_address_bit_set_AC_during_DC);


  input   clk;
  input   reset;
  input   enb;
  input   [15:0] FIFO_data_in;  // uint16
  input   FIFO_write_to;
  input   [7:0] InputState;  // uint8
  input   [7:0] OutputState;  // uint8
  input   rd_enable;
  input   [8:0] rd_address;  // ufix9
  input   [15:0] read_from_storage_value;  // uint16
  input   [15:0] rd_dout;  // uint16
  output  comp_data_out_valid;
  output  [15:0] comp_data_out;  // uint16
  output  [8:0] rd_address_bit_set_AC_during_DC;  // ufix9


  reg  Delay2_out1;
  reg [7:0] Delay1_out1;  // uint8
  wire Compare_To_Constant_out1;
  wire AND_out1;
  wire switch_compare_1;
  wire output_state_DC_out1;
  wire output_state_DC1_out1;
  wire Logical_Operator_out1;
  reg  Delay3_out1;
  wire switch_compare_1_1;
  wire [15:0] read_from_storage;  // uint16
  wire [15:0] read_from_magic_and_timestamp_out1;  // uint16
  wire [15:0] Switch_out1;  // uint16
  wire [15:0] read_from_storage_post;  // uint16

  // Get data from this or that ram depending on output_state
  // 
  // memorie per temporizzazione per ritardo delle ramp input
  // vale per tutte
  // 
  // if OutputState < 6 e read_enable = 1 --> output magic/timestamp
  // else if OutputState ~= 6 read from storage
  // else read processed data


  always @(posedge clk or posedge reset)
    begin : Delay2_process
      if (reset == 1'b1) begin
        Delay2_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay2_out1 <= rd_enable;
        end
      end
    end



  assign comp_data_out_valid = Delay2_out1;

  always @(posedge clk or posedge reset)
    begin : Delay1_process
      if (reset == 1'b1) begin
        Delay1_out1 <= 8'b00000000;
      end
      else begin
        if (enb) begin
          Delay1_out1 <= OutputState;
        end
      end
    end



  assign Compare_To_Constant_out1 = Delay1_out1 < 8'b00000110;



  assign AND_out1 = Compare_To_Constant_out1 & Delay2_out1;



  assign switch_compare_1 = AND_out1 > 1'b0;



  assign output_state_DC_out1 = OutputState < 8'b00000110;



  assign output_state_DC1_out1 = OutputState > 8'b00000111;



  assign Logical_Operator_out1 = output_state_DC_out1 | output_state_DC1_out1;



  always @(posedge clk or posedge reset)
    begin : Delay3_process
      if (reset == 1'b1) begin
        Delay3_out1 <= 1'b0;
      end
      else begin
        if (enb) begin
          Delay3_out1 <= Logical_Operator_out1;
        end
      end
    end



  assign switch_compare_1_1 = Delay3_out1 > 1'b0;



  assign read_from_storage = (switch_compare_1_1 == 1'b0 ? rd_dout :
              read_from_storage_value);



  read_from_magic_and_timestamp u_read_from_magic_and_timestamp (.clk(clk),
                                                                 .enb(enb),
                                                                 .OutputState(OutputState),  // uint8
                                                                 .FIFO_data_in(FIFO_data_in),  // uint16
                                                                 .FIFO_write_to(FIFO_write_to),
                                                                 .InputState(InputState),  // uint8
                                                                 .read_from_Magic_TS(read_from_magic_and_timestamp_out1)  // uint16
                                                                 );

  assign Switch_out1 = (switch_compare_1 == 1'b0 ? read_from_storage :
              read_from_magic_and_timestamp_out1);



  get_data_only_when_enabled u_get_data_only_when_enabled (.clk(clk),
                                                           .reset(reset),
                                                           .enb(enb),
                                                           .In1(Switch_out1),  // uint16
                                                           .Enable(Delay2_out1),
                                                           .Out1(read_from_storage_post)  // uint16
                                                           );

  assign comp_data_out = read_from_storage_post;

  assign rd_address_bit_set_AC_during_DC = rd_address;

endmodule  // collect_output
