// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\CustArch_v22\Square_Root1_iv.v
// Created: 2021-09-20 12:48:51
// 
// Generated by MATLAB 9.8 and HDL Coder 3.16
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Square_Root1_iv
// Source Path: CustArch_v22/cust_architecture/process_and_retrieve/spike_detection/LocalMaximaAdaptiveThreshold/RMS_computation/Square 
// Root1/Square Root1_i
// Hierarchy Level: 6
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Square_Root1_iv
          (clk,
           reset,
           enb,
           ain,
           xinit,
           inzero,
           aout);


  input   clk;
  input   reset;
  input   enb;
  input   [39:0] ain;  // ufix40_En40
  output  signed [39:0] xinit;  // sfix40_En36
  output  inzero;  // ufix1
  output  [39:0] aout;  // ufix40_En40


  reg [39:0] ain_reg_reg [0:1];  // ufix40 [2]
  wire [39:0] ain_reg_reg_next [0:1];  // ufix40_En40 [2]
  wire signed [41:0] constA;  // sfix42_En38
  wire signed [39:0] constB;  // sfix40_En36
  wire signed [39:0] xinitreg;  // sfix40_En36
  wire signed [41:0] xinitreg_1;  // sfix42_En38
  wire signed [39:0] constC;  // sfix40_En36
  reg signed [39:0] constC_reg_reg [0:3];  // sfix40 [4]
  wire signed [39:0] constC_reg_reg_next [0:3];  // sfix40_En36 [4]
  wire signed [39:0] constC_p;  // sfix40_En36
  wire [1:0] mstwobit;  // ufix2

  // Polynomial initial value stage of the RecipSqrtSingleRate Implementation using Newton Method


  // Pipeline registers
  always @(posedge clk or posedge reset)
    begin : ain_reg_process
      if (reset == 1'b1) begin
        ain_reg_reg[0] <= 40'h0000000000;
        ain_reg_reg[1] <= 40'h0000000000;
      end
      else begin
        if (enb) begin
          ain_reg_reg[0] <= ain_reg_reg_next[0];
          ain_reg_reg[1] <= ain_reg_reg_next[1];
        end
      end
    end

  assign aout = ain_reg_reg[1];
  assign ain_reg_reg_next[0] = ain;
  assign ain_reg_reg_next[1] = ain_reg_reg[0];



  assign constA = 42'sh065F6FF0AAC;



  assign constB = 40'shCD141D87E2;



  NewtonPolynomialIVStage u_NewtonPolynomialIVStage1 (.clk(clk),
                                                      .reset(reset),
                                                      .enb(enb),
                                                      .ain(ain),  // ufix40_En40
                                                      .mulin(constA),  // sfix42_En38
                                                      .adderin(constB),  // sfix40_En36
                                                      .xinitinterm(xinitreg)  // sfix40_En36
                                                      );

  assign xinitreg_1 = {xinitreg, 2'b00};



  assign constC = 40'sh2A02BA7CC1;



  // Pipeline registers
  always @(posedge clk or posedge reset)
    begin : constC_reg_process
      if (reset == 1'b1) begin
        constC_reg_reg[0] <= 40'sh0000000000;
        constC_reg_reg[1] <= 40'sh0000000000;
        constC_reg_reg[2] <= 40'sh0000000000;
        constC_reg_reg[3] <= 40'sh0000000000;
      end
      else begin
        if (enb) begin
          constC_reg_reg[0] <= constC_reg_reg_next[0];
          constC_reg_reg[1] <= constC_reg_reg_next[1];
          constC_reg_reg[2] <= constC_reg_reg_next[2];
          constC_reg_reg[3] <= constC_reg_reg_next[3];
        end
      end
    end

  assign constC_p = constC_reg_reg[3];
  assign constC_reg_reg_next[0] = constC;
  assign constC_reg_reg_next[1] = constC_reg_reg[0];
  assign constC_reg_reg_next[2] = constC_reg_reg[1];
  assign constC_reg_reg_next[3] = constC_reg_reg[2];



  NewtonPolynomialIVStage u_NewtonPolynomialIVStage2 (.clk(clk),
                                                      .reset(reset),
                                                      .enb(enb),
                                                      .ain(aout),  // ufix40_En40
                                                      .mulin(xinitreg_1),  // sfix42_En38
                                                      .adderin(constC_p),  // sfix40_En36
                                                      .xinitinterm(xinit)  // sfix40_En36
                                                      );

  assign mstwobit = ain[39:38];



  assign inzero = mstwobit == 2'b00;



endmodule  // Square_Root1_iv
