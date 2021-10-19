// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v21\HP_Butter_2nd.v
// Created: 2021-10-18 17:44:13
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: HP_Butter_2nd
// Source Path: CustArch_v21/cust_architecture/process_and_retrieve/filtering/two_pole_IIR/HP_Butter_2nd
// Hierarchy Level: 4
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module HP_Butter_2nd
          (clk,
           reset,
           enb_1_2_0,
           Input_rsvd,
           previous_samples_0,
           previous_samples_1,
           Output_rsvd,
           Output1);


  input   clk;
  input   reset;
  input   enb_1_2_0;
  input   signed [31:0] Input_rsvd;  // sfix32_En15
  input   signed [31:0] previous_samples_0;  // sfix32_En7
  input   signed [31:0] previous_samples_1;  // sfix32_En7
  output  signed [31:0] Output_rsvd;  // sfix32_En15
  output  signed [31:0] Output1;  // sfix32_En7


  wire signed [64:0] s_1_mul_temp;  // sfix65_En47
  wire signed [63:0] s_1_cast;  // sfix64_En47
  wire signed [31:0] s_1_out1;  // sfix32_En16
  reg signed [31:0] delayMatch7_reg [0:3];  // sfix32 [4]
  wire signed [31:0] delayMatch7_reg_next [0:3];  // sfix32_En16 [4]
  wire signed [31:0] s_1_out1_1;  // sfix32_En16
  wire signed [63:0] a_2_1_mul_temp;  // sfix64_En37
  wire signed [31:0] a_2_1_out1;  // sfix32_En6
  reg signed [31:0] delayMatch1_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch1_reg_next [0:1];  // sfix32_En6 [2]
  wire signed [31:0] a_2_1_out1_1;  // sfix32_En6
  wire signed [42:0] SumA21_sub_cast;  // sfix43_En16
  wire signed [42:0] SumA21_sub_cast_1;  // sfix43_En16
  wire signed [42:0] SumA21_sub_temp;  // sfix43_En16
  wire signed [31:0] SumA21_out1;  // sfix32_En6
  reg signed [31:0] delayMatch4_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch4_reg_next [0:1];  // sfix32_En6 [2]
  wire signed [31:0] SumA21_out1_1;  // sfix32_En6
  wire signed [64:0] a_3_1_mul_temp;  // sfix65_En39
  wire signed [63:0] a_3_1_cast;  // sfix64_En39
  wire signed [31:0] a_3_1_out1;  // sfix32_En8
  reg signed [31:0] delay_8_reg [0:3];  // sfix32 [4]
  wire signed [31:0] delay_8_reg_next [0:3];  // sfix32_En8 [4]
  wire signed [31:0] delay_8_out1;  // sfix32_En8
  wire signed [32:0] SumA31_sub_cast;  // sfix33_En6
  wire signed [31:0] SumA31_sub_cast_1;  // sfix32_En6
  wire signed [32:0] SumA31_sub_cast_2;  // sfix33_En6
  wire signed [32:0] SumA31_sub_temp;  // sfix33_En6
  wire signed [31:0] SumA31_cast;  // sfix32_En6
  wire signed [31:0] SumA31_out1;  // sfix32_En7
  reg signed [31:0] delay_3_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delay_3_reg_next [0:1];  // sfix32_En7 [2]
  wire signed [31:0] delay_3_out1;  // sfix32_En7
  wire signed [63:0] b_2_1_mul_temp;  // sfix64_En37
  wire signed [63:0] b_2_1_1;  // sfix64
  wire signed [31:0] b_2_1_out1;  // sfix32_En6
  reg signed [31:0] delay_7_reg [0:5];  // sfix32 [6]
  wire signed [31:0] delay_7_reg_next [0:5];  // sfix32_En6 [6]
  wire signed [31:0] delay_7_out1;  // sfix32_En6
  wire signed [33:0] SumB21_add_cast;  // sfix34_En7
  wire signed [33:0] SumB21_add_cast_1;  // sfix34_En7
  wire signed [33:0] SumB21_add_temp;  // sfix34_En7
  wire signed [31:0] SumB21_out1;  // sfix32_En7
  reg signed [31:0] delayMatch5_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch5_reg_next [0:1];  // sfix32_En7 [2]
  wire signed [31:0] SumB21_out1_1;  // sfix32_En7
  reg signed [31:0] delay_9_reg [0:7];  // sfix32 [8]
  wire signed [31:0] delay_9_reg_next [0:7];  // sfix32_En7 [8]
  wire signed [31:0] delay_9_out1;  // sfix32_En7
  wire signed [32:0] SumB31_add_cast;  // sfix33_En7
  wire signed [32:0] SumB31_add_cast_1;  // sfix33_En7
  wire signed [32:0] SumB31_add_temp;  // sfix33_En7
  wire signed [31:0] SumB31_cast;  // sfix32_En7
  wire signed [31:0] SumB31_out1;  // sfix32_En15
  reg signed [31:0] delayMatch6_reg [0:1];  // sfix32 [2]
  wire signed [31:0] delayMatch6_reg_next [0:1];  // sfix32_En15 [2]
  wire signed [31:0] SumB31_out1_1;  // sfix32_En15


  assign s_1_mul_temp = 33'sh0F4E0044F * Input_rsvd;
  assign s_1_cast = s_1_mul_temp[63:0];
  assign s_1_out1 = (((s_1_cast[63] == 1'b0) && (s_1_cast[62] != 1'b0)) || ((s_1_cast[63] == 1'b0) && (s_1_cast[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((s_1_cast[63] == 1'b1) && (s_1_cast[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              s_1_cast[62:31] + $signed({1'b0, s_1_cast[30]})));



  always @(posedge clk or posedge reset)
    begin : delayMatch7_process
      if (reset == 1'b1) begin
        delayMatch7_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch7_reg[1] <= 32'sb00000000000000000000000000000000;
        delayMatch7_reg[2] <= 32'sb00000000000000000000000000000000;
        delayMatch7_reg[3] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch7_reg[0] <= delayMatch7_reg_next[0];
          delayMatch7_reg[1] <= delayMatch7_reg_next[1];
          delayMatch7_reg[2] <= delayMatch7_reg_next[2];
          delayMatch7_reg[3] <= delayMatch7_reg_next[3];
        end
      end
    end

  assign s_1_out1_1 = delayMatch7_reg[3];
  assign delayMatch7_reg_next[0] = s_1_out1;
  assign delayMatch7_reg_next[1] = delayMatch7_reg[0];
  assign delayMatch7_reg_next[2] = delayMatch7_reg[1];
  assign delayMatch7_reg_next[3] = delayMatch7_reg[2];



  assign a_2_1_mul_temp = 32'sb10000101101011101111001001111111 * previous_samples_0;
  assign a_2_1_out1 = (((a_2_1_mul_temp[63] == 1'b0) && (a_2_1_mul_temp[62] != 1'b0)) || ((a_2_1_mul_temp[63] == 1'b0) && (a_2_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((a_2_1_mul_temp[63] == 1'b1) && (a_2_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              a_2_1_mul_temp[62:31] + $signed({1'b0, a_2_1_mul_temp[30]})));



  always @(posedge clk or posedge reset)
    begin : delayMatch1_process
      if (reset == 1'b1) begin
        delayMatch1_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch1_reg[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch1_reg[0] <= delayMatch1_reg_next[0];
          delayMatch1_reg[1] <= delayMatch1_reg_next[1];
        end
      end
    end

  assign a_2_1_out1_1 = delayMatch1_reg[1];
  assign delayMatch1_reg_next[0] = a_2_1_out1;
  assign delayMatch1_reg_next[1] = delayMatch1_reg[0];



  assign SumA21_sub_cast = {{11{s_1_out1_1[31]}}, s_1_out1_1};
  assign SumA21_sub_cast_1 = {a_2_1_out1_1[31], {a_2_1_out1_1, 10'b0000000000}};
  assign SumA21_sub_temp = SumA21_sub_cast - SumA21_sub_cast_1;
  assign SumA21_out1 = (((SumA21_sub_temp[42] == 1'b0) && (SumA21_sub_temp[41] != 1'b0)) || ((SumA21_sub_temp[42] == 1'b0) && (SumA21_sub_temp[41:10] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((SumA21_sub_temp[42] == 1'b1) && (SumA21_sub_temp[41] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              SumA21_sub_temp[41:10] + $signed({1'b0, SumA21_sub_temp[9]})));



  always @(posedge clk or posedge reset)
    begin : delayMatch4_process
      if (reset == 1'b1) begin
        delayMatch4_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch4_reg[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch4_reg[0] <= delayMatch4_reg_next[0];
          delayMatch4_reg[1] <= delayMatch4_reg_next[1];
        end
      end
    end

  assign SumA21_out1_1 = delayMatch4_reg[1];
  assign delayMatch4_reg_next[0] = SumA21_out1;
  assign delayMatch4_reg_next[1] = delayMatch4_reg[0];



  assign a_3_1_mul_temp = 33'sh0EA3BDB37 * previous_samples_1;
  assign a_3_1_cast = a_3_1_mul_temp[63:0];
  assign a_3_1_out1 = (((a_3_1_cast[63] == 1'b0) && (a_3_1_cast[62] != 1'b0)) || ((a_3_1_cast[63] == 1'b0) && (a_3_1_cast[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((a_3_1_cast[63] == 1'b1) && (a_3_1_cast[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              a_3_1_cast[62:31] + $signed({1'b0, a_3_1_cast[30]})));



  always @(posedge clk or posedge reset)
    begin : delay_8_process
      if (reset == 1'b1) begin
        delay_8_reg[0] <= 32'sb00000000000000000000000000000000;
        delay_8_reg[1] <= 32'sb00000000000000000000000000000000;
        delay_8_reg[2] <= 32'sb00000000000000000000000000000000;
        delay_8_reg[3] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delay_8_reg[0] <= delay_8_reg_next[0];
          delay_8_reg[1] <= delay_8_reg_next[1];
          delay_8_reg[2] <= delay_8_reg_next[2];
          delay_8_reg[3] <= delay_8_reg_next[3];
        end
      end
    end

  assign delay_8_out1 = delay_8_reg[3];
  assign delay_8_reg_next[0] = a_3_1_out1;
  assign delay_8_reg_next[1] = delay_8_reg[0];
  assign delay_8_reg_next[2] = delay_8_reg[1];
  assign delay_8_reg_next[3] = delay_8_reg[2];



  assign SumA31_sub_cast = {SumA21_out1_1[31], SumA21_out1_1};
  assign SumA31_sub_cast_1 = ({{2{delay_8_out1[31]}}, delay_8_out1[31:2]}) + $signed({1'b0, delay_8_out1[1]});
  assign SumA31_sub_cast_2 = {SumA31_sub_cast_1[31], SumA31_sub_cast_1};
  assign SumA31_sub_temp = SumA31_sub_cast - SumA31_sub_cast_2;
  assign SumA31_cast = ((SumA31_sub_temp[32] == 1'b0) && (SumA31_sub_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumA31_sub_temp[32] == 1'b1) && (SumA31_sub_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumA31_sub_temp[31:0])));
  assign SumA31_out1 = ((SumA31_cast[31] == 1'b0) && (SumA31_cast[30] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumA31_cast[31] == 1'b1) && (SumA31_cast[30] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              {SumA31_cast[30:0], 1'b0}));



  always @(posedge clk or posedge reset)
    begin : delay_3_process
      if (reset == 1'b1) begin
        delay_3_reg[0] <= 32'sb00000000000000000000000000000000;
        delay_3_reg[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delay_3_reg[0] <= delay_3_reg_next[0];
          delay_3_reg[1] <= delay_3_reg_next[1];
        end
      end
    end

  assign delay_3_out1 = delay_3_reg[1];
  assign delay_3_reg_next[0] = SumA31_out1;
  assign delay_3_reg_next[1] = delay_3_reg[0];



  // CSD Encoding (2147483648) : 010000000000000000000000000000000; Cost (Adders) = 0
  assign b_2_1_1 = {previous_samples_0[31], {previous_samples_0, 31'b0000000000000000000000000000000}};
  assign b_2_1_mul_temp =  - (b_2_1_1);
  assign b_2_1_out1 = (((b_2_1_mul_temp[63] == 1'b0) && (b_2_1_mul_temp[62] != 1'b0)) || ((b_2_1_mul_temp[63] == 1'b0) && (b_2_1_mul_temp[62:31] == 32'sb01111111111111111111111111111111)) ? 32'sb01111111111111111111111111111111 :
              ((b_2_1_mul_temp[63] == 1'b1) && (b_2_1_mul_temp[62] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              b_2_1_mul_temp[62:31] + $signed({1'b0, b_2_1_mul_temp[30]})));



  always @(posedge clk or posedge reset)
    begin : delay_7_process
      if (reset == 1'b1) begin
        delay_7_reg[0] <= 32'sb00000000000000000000000000000000;
        delay_7_reg[1] <= 32'sb00000000000000000000000000000000;
        delay_7_reg[2] <= 32'sb00000000000000000000000000000000;
        delay_7_reg[3] <= 32'sb00000000000000000000000000000000;
        delay_7_reg[4] <= 32'sb00000000000000000000000000000000;
        delay_7_reg[5] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delay_7_reg[0] <= delay_7_reg_next[0];
          delay_7_reg[1] <= delay_7_reg_next[1];
          delay_7_reg[2] <= delay_7_reg_next[2];
          delay_7_reg[3] <= delay_7_reg_next[3];
          delay_7_reg[4] <= delay_7_reg_next[4];
          delay_7_reg[5] <= delay_7_reg_next[5];
        end
      end
    end

  assign delay_7_out1 = delay_7_reg[5];
  assign delay_7_reg_next[0] = b_2_1_out1;
  assign delay_7_reg_next[1] = delay_7_reg[0];
  assign delay_7_reg_next[2] = delay_7_reg[1];
  assign delay_7_reg_next[3] = delay_7_reg[2];
  assign delay_7_reg_next[4] = delay_7_reg[3];
  assign delay_7_reg_next[5] = delay_7_reg[4];



  assign SumB21_add_cast = {{2{delay_3_out1[31]}}, delay_3_out1};
  assign SumB21_add_cast_1 = {delay_7_out1[31], {delay_7_out1, 1'b0}};
  assign SumB21_add_temp = SumB21_add_cast + SumB21_add_cast_1;
  assign SumB21_out1 = ((SumB21_add_temp[33] == 1'b0) && (SumB21_add_temp[32:31] != 2'b00) ? 32'sb01111111111111111111111111111111 :
              ((SumB21_add_temp[33] == 1'b1) && (SumB21_add_temp[32:31] != 2'b11) ? 32'sb10000000000000000000000000000000 :
              $signed(SumB21_add_temp[31:0])));



  always @(posedge clk or posedge reset)
    begin : delayMatch5_process
      if (reset == 1'b1) begin
        delayMatch5_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch5_reg[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch5_reg[0] <= delayMatch5_reg_next[0];
          delayMatch5_reg[1] <= delayMatch5_reg_next[1];
        end
      end
    end

  assign SumB21_out1_1 = delayMatch5_reg[1];
  assign delayMatch5_reg_next[0] = SumB21_out1;
  assign delayMatch5_reg_next[1] = delayMatch5_reg[0];



  always @(posedge clk or posedge reset)
    begin : delay_9_process
      if (reset == 1'b1) begin
        delay_9_reg[0] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[1] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[2] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[3] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[4] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[5] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[6] <= 32'sb00000000000000000000000000000000;
        delay_9_reg[7] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delay_9_reg[0] <= delay_9_reg_next[0];
          delay_9_reg[1] <= delay_9_reg_next[1];
          delay_9_reg[2] <= delay_9_reg_next[2];
          delay_9_reg[3] <= delay_9_reg_next[3];
          delay_9_reg[4] <= delay_9_reg_next[4];
          delay_9_reg[5] <= delay_9_reg_next[5];
          delay_9_reg[6] <= delay_9_reg_next[6];
          delay_9_reg[7] <= delay_9_reg_next[7];
        end
      end
    end

  assign delay_9_out1 = delay_9_reg[7];
  assign delay_9_reg_next[0] = previous_samples_1;
  assign delay_9_reg_next[1] = delay_9_reg[0];
  assign delay_9_reg_next[2] = delay_9_reg[1];
  assign delay_9_reg_next[3] = delay_9_reg[2];
  assign delay_9_reg_next[4] = delay_9_reg[3];
  assign delay_9_reg_next[5] = delay_9_reg[4];
  assign delay_9_reg_next[6] = delay_9_reg[5];
  assign delay_9_reg_next[7] = delay_9_reg[6];



  assign SumB31_add_cast = {SumB21_out1_1[31], SumB21_out1_1};
  assign SumB31_add_cast_1 = {delay_9_out1[31], delay_9_out1};
  assign SumB31_add_temp = SumB31_add_cast + SumB31_add_cast_1;
  assign SumB31_cast = ((SumB31_add_temp[32] == 1'b0) && (SumB31_add_temp[31] != 1'b0) ? 32'sb01111111111111111111111111111111 :
              ((SumB31_add_temp[32] == 1'b1) && (SumB31_add_temp[31] != 1'b1) ? 32'sb10000000000000000000000000000000 :
              $signed(SumB31_add_temp[31:0])));
  assign SumB31_out1 = ((SumB31_cast[31] == 1'b0) && (SumB31_cast[30:23] != 8'b00000000) ? 32'sb01111111111111111111111111111111 :
              ((SumB31_cast[31] == 1'b1) && (SumB31_cast[30:23] != 8'b11111111) ? 32'sb10000000000000000000000000000000 :
              {SumB31_cast[23:0], 8'b00000000}));



  always @(posedge clk or posedge reset)
    begin : delayMatch6_process
      if (reset == 1'b1) begin
        delayMatch6_reg[0] <= 32'sb00000000000000000000000000000000;
        delayMatch6_reg[1] <= 32'sb00000000000000000000000000000000;
      end
      else begin
        if (enb_1_2_0) begin
          delayMatch6_reg[0] <= delayMatch6_reg_next[0];
          delayMatch6_reg[1] <= delayMatch6_reg_next[1];
        end
      end
    end

  assign SumB31_out1_1 = delayMatch6_reg[1];
  assign delayMatch6_reg_next[0] = SumB31_out1;
  assign delayMatch6_reg_next[1] = delayMatch6_reg[0];



  assign Output_rsvd = SumB31_out1_1;

  assign Output1 = SumA31_out1;

endmodule  // HP_Butter_2nd

