//////////////////////////////////////////////////////////////////////////////////
// Company:        Intan Technologies, LLC
//                 Copyright (c) 2013-2017 Intan Technologies LLC
// 
// Design Name:    RHS000 Rhythm Stim Interface
// Module Name:    main, command_selector
// Project Name:   Opal Kelly FPGA/USB RHS2000 Interface
// Target Devices: 
// Tool versions: 
// Description:    Uses Opal Kelly XEM6010 USB/FPGA board to interface multiple
//                 Intan Technologies RHS2000-series chips to a computer via a
//                 USB 2.0 connection.
//
//                 This software is provided 'as-is', without any express or implied
//                 warranty.  In no event will the authors be held liable for any
//                 damages arising from the use of this software.
//
//                 Permission is granted to anyone to use this software for any
//                 applications that use Intan Technologies integrated circuits, and
//                 to alter it and redistribute it freely.
//
//                 See http://www.intantech.com for documentation and product information.
//
// Dependencies: 
//
// Revision:        1.0 (23 October 2016)
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Note on starting Xilinx projects for Opal Kelly XEM6010-LX45 board based on
// the Opal Kelly RAMTester sample:
//
// Start a new project for device xc6slx45-2fgg484, preferred language: Verilog
//
// Include the following files from the XEM6010 RAMTester sample directory
// (C:\Program Files\Opal Kelly\FrontPanelUSB\Samples\RAMTester\XEM6010-Verilog) and
// FrontPanel directory (C:\Program Files\Opal Kelly\FrontPanelUSB\FrontPanelHDL\XEM6010-LX45)
// using Project --> Add Source...:
//   iodrp_mcb_controller.v
//   iodrp_controller.v
//   mcb_soft_calibration.v
//   mcb_soft_calibration_top.v
//   mcb_raw_wrapper.v
//   okLibrary.v
//   memc3_wrapper.v
//   memc3_infrastructure.v  (the one in the root directory of the XEM6010 RAMTester sample)
//   fifo_w64_512_r16_2048.v
//   fifo_w16_2048_r64_512.v
//   ddr2_test.v
//   ramtest.v
//   xem6010.ucf
// 
// Copy the associated *.ngc files to the main Xilinx project directory.  Make sure the
// fifo_w16_2048_r64_512.ngc and fifo_w64_512_r16_2048.ngc are in the main directory, not
// the Core subdirectory.
//------------------------------------------------------------------------


/*
#######  #####  ###    ## ######  ######   ######  ##   ##     ###    ###  ######  ######   ######
##      ##   ## ####   ## ##   ## ##   ## ##    ##  ## ##      ####  #### ##    ## ##   ## ##
####### ####### ## ##  ## ##   ## ######  ##    ##   ###       ## #### ## ##    ## ##   ## #######
     ## ##   ## ##  ## ## ##   ## ##   ## ##    ##  ## ##      ##  ##  ## ##    ## ##   ##      ##
####### ##   ## ##   #### ######  ######   ######  ##   ##     ##      ##  ######  ######  ###### 


    some modifications were added to the original design from Intan to add custom computation in
    a sandboxed environment. These modifications have some consequences...
    
    Architecture:
        The sandbox computing has been added between the control FSM and the data FIFO that
        contains data waiting for USB transfer. Therefore, if CST_enable is O, sandbox computing
        is disabled, and the system will perform as the original design.
        
        if CST_enable is 1, then data acquired by the FSM is sent to the custom computation and
        the custom computation module outputs data to the USB FIFO.
        
        if CSTtrig_to_DAC si 1, the DAC outputs obey to the trig_stim output of the computation
        instead of the HPF output (DACs must still be enabled).

        FIXME: It is still possible to output data from the FSM (original behavior) whatever the
        value of CST_enable is. Data is then secured to keep integrity, the price is that it will
        be much more difficult to validate/debug the system. If this is the required behavior,
        make a feature request.

    Commands:
        The point of these hack is to keep the original behavior of the system to avoid interfering
        with original commands (and reduce work on original system updates). Therefore configuration
        requires minimal OK resources for USB communication.
            PipeIn  : # 0x88 : sandboxed custom computation configuration
            PipeOut : # 0xA1 : configuration feedback and eventual basic debugging
            WireIn  : # 0x0B : lowlevel sandbox control
            WireOut : # 0x27 : sandbox low level status
        
        PipeIn / PipeOut protocol is user defined, so will not be documented here
        WireIn :
               0 : CST_enable (must be set to enable custom computation)
               1 : spare (who knows ;)
               2 : CSTtrig_to_DAC
            15-3 : unused
        WireOut :
             7-0 : number of elements in command output FIFO
            15-8 : unused

*/







/*
##     ##  ######      ######  ####### #######  ###### ######  ## ######  ######## ##  ######  ###    ##
##    ##  ##    ##     ##   ## ##      ##      ##      ##   ## ## ##   ##    ##    ## ##    ## ####   ##
##   ##   ##    ##     ##   ## #####   ####### ##      ######  ## ######     ##    ## ##    ## ## ##  ##
##  ##    ##    ##     ##   ## ##           ## ##      ##   ## ## ##         ##    ## ##    ## ##  ## ##
## ##      ######      ######  ####### #######  ###### ##   ## ## ##         ##    ##  ######  ##   ####
*/
`timescale 1ns/1ps



module main #(
    // All of these parameters for the 'main' module relate to the SDRAM interface
    parameter C3_P0_MASK_SIZE           = 4,
    parameter C3_P0_DATA_PORT_SIZE      = 32,
    parameter C3_P1_MASK_SIZE           = 4,
    parameter C3_P1_DATA_PORT_SIZE      = 32,
    parameter DEBUG_EN                  = 0,       
    parameter C3_MEMCLK_PERIOD          = 3200,       
    parameter C3_CALIB_SOFT_IP          = "TRUE",       
    parameter C3_SIMULATION             = "FALSE",       
    parameter C3_HW_TESTING             = "FALSE",       
    parameter C3_RST_ACT_LOW            = 0,       
    parameter C3_INPUT_CLK_TYPE         = "DIFFERENTIAL",       
    parameter C3_MEM_ADDR_ORDER         = "ROW_BANK_COLUMN",
    parameter C3_NUM_DQ_PINS            = 16,       
    parameter C3_MEM_ADDR_WIDTH         = 13,       
    parameter C3_MEM_BANKADDR_WIDTH     = 3        
    )
    (
    // Clock
    input  wire        clk1_in, // CY22393 CLKA, f = 100MHz
    
    // SDRAM
    inout  wire [C3_NUM_DQ_PINS-1:0]         mcb3_dram_dq,
    output wire [C3_MEM_ADDR_WIDTH-1:0]      mcb3_dram_a,
    output wire [C3_MEM_BANKADDR_WIDTH-1:0]  mcb3_dram_ba,
    output wire                              mcb3_dram_ras_n,
    output wire                              mcb3_dram_cas_n,
    output wire                              mcb3_dram_we_n,
    output wire                              mcb3_dram_odt,
    output wire                              mcb3_dram_cke,
    output wire                              mcb3_dram_dm,
    inout  wire                              mcb3_dram_udqs,
    inout  wire                              mcb3_dram_udqs_n,
    inout  wire                              mcb3_rzq,
    inout  wire                              mcb3_zio,
    output wire                              mcb3_dram_udm,
    inout  wire                              mcb3_dram_dqs,
    inout  wire                              mcb3_dram_dqs_n,
    output wire                              mcb3_dram_ck,
    output wire                              mcb3_dram_ck_n,
    output wire                              mcb3_dram_cs_n,

    // SPI
    output wire                              CS_b_A,
    output wire                              SCLK_A,
    output wire                              MOSI1_A,
    output wire                              MOSI2_A,
    input  wire                              MISO1_A,
    input  wire                              MISO2_A,
   
    output wire                              CS_b_B,
    output wire                              SCLK_B,
    output wire                              MOSI1_B,
    output wire                              MOSI2_B,
    input  wire                              MISO1_B,
    input  wire                              MISO2_B,

    output wire                              CS_b_C,
    output wire                              SCLK_C,
    output wire                              MOSI1_C,
    output wire                              MOSI2_C,
    input  wire                              MISO1_C,
    input  wire                              MISO2_C,

    output wire                              CS_b_D,
    output wire                              SCLK_D,
    output wire                              MOSI1_D,
    output wire                              MOSI2_D,
    input  wire                              MISO1_D,
    input  wire                              MISO2_D,

    output wire                              CS_b_E,
    output wire                              SCLK_E,
    output wire                              MOSI1_E,
    output wire                              MOSI2_E,
    input  wire                              MISO1_E,
    input  wire                              MISO2_E,

    output wire                              CS_b_F,
    output wire                              SCLK_F,
    output wire                              MOSI1_F,
    output wire                              MOSI2_F,
    input  wire                              MISO1_F,
    input  wire                              MISO2_F,

    output wire                              CS_b_G,
    output wire                              SCLK_G,
    output wire                              MOSI1_G,
    output wire                              MOSI2_G,
    input  wire                              MISO1_G,
    input  wire                              MISO2_G,

    output wire                              CS_b_H,
    output wire                              SCLK_H,
    output wire                              MOSI1_H,
    output wire                              MOSI2_H,
    input  wire                              MISO1_H,
    input  wire                              MISO2_H,
   
    output wire [7:0]                        SPI_port_LEDs,
   
    // USB
    input  wire [7:0]                        hi_in,
    output wire [1:0]                        hi_out,
    inout  wire [15:0]                       hi_inout,
    inout  wire                              hi_aa,

    output wire                              i2c_sda,
    output wire                              i2c_scl,
    output wire                              hi_muxsel,

    // LEDs
    output wire [7:0]                          led,

    // DACs
    output wire                              DAC_SYNC,
    output wire                              DAC_SCLK,
    output wire                              DAC_DIN_1,
    output wire                              DAC_DIN_2,
    output wire                              DAC_DIN_3,
    output wire                              DAC_DIN_4,
    output wire                              DAC_DIN_5,
    output wire                              DAC_DIN_6,
    output wire                              DAC_DIN_7,
    output wire                              DAC_DIN_8,

    // ADCs
    output wire                              ADC_CS,
    output wire                              ADC_SCLK,
    input  wire                              ADC_DOUT_1,
    input  wire                              ADC_DOUT_2,
    input  wire                              ADC_DOUT_3,
    input  wire                              ADC_DOUT_4,
    input  wire                              ADC_DOUT_5,
    input  wire                              ADC_DOUT_6,
    input  wire                              ADC_DOUT_7,
    input  wire                              ADC_DOUT_8,

    // Digital IO
    input  wire                              TTL_in_direct_1,
    input  wire                              TTL_in_direct_2,
    output wire                              serial_LOAD,
    output wire                              serial_CLK,
    input wire                               TTL_in_serial,
    input wire                               TTL_in_serial_exp,
    output wire                              spare_1,
    input wire                               expander_detect,
    input wire                               expander_ID_1,
    output wire [15:0]                       TTL_out_direct,
    output reg                               sample_CLK_out,
    output wire                              mark_out,
    output wire [2:0]                        status_LEDs,
    
    // configuration bits
    input  wire [3:0]                        board_mode,
    
    // high-speed LVDS interface (future expansion)
    input wire                               LVDS_1_p,
    input wire                               LVDS_1_n,
    input wire                               LVDS_2_p,
    input wire                               LVDS_2_n,
    input wire                               LVDS_3_p,
    input wire                               LVDS_3_n,
    output wire                              LVDS_4_p,
    output wire                              LVDS_4_n,
    output wire                              I2C_SDA,
    output wire                              I2C_SCK
    );



/*
######  #######  ###### ##       #####  ######   #####  ######## ##  ######  ###    ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ####   ## ##
##   ## #####   ##      ##      ####### ######  #######    ##    ## ##    ## ## ##  ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ##  ## ##      ##
######  #######  ###### ####### ##   ## ##   ## ##   ##    ##    ##  ######  ##   #### #######
*/

    localparam header_magic_number = 64'h8d542c8a49712f0b;  // Fixed 64-bit "magic number" that begins each data frame
                                                        // to aid in synchronization.

    // Clock
    wire            clk1;            // buffered 100 MHz clock
    wire            dataclk;         // programmable frequency clock (f = 2800 * per-channel amplifier sampling rate)
    wire            dataclk_locked, DCM_prog_done;
    wire [8:0]      dataclk_M, dataclk_D;
    wire            DCM_prog_trigger;

    // USB
    wire            ti_clk;      // 48 MHz clock from Opal Kelly USB interface
      
    assign i2c_sda    = 1'bz;
    assign i2c_scl    = 1'bz;
    assign hi_muxsel  = 1'b0;


    // high-speed LVDS interface bus (future expansion)
    
    wire LVDS_1, LVDS_2, LVDS_3, LVDS_4;

    IBUFDS lvds_receiver_in_1 (.O(LVDS_1), .I(LVDS_1_p), .IB(LVDS_1_n));
    IBUFDS lvds_receiver_in_2 (.O(LVDS_2), .I(LVDS_2_p), .IB(LVDS_2_n));
    IBUFDS lvds_receiver_in_3 (.O(LVDS_3), .I(LVDS_3_p), .IB(LVDS_3_n));
    
    OBUFDS lvds_driver_out_1  (.O(LVDS_4_p), .OB(LVDS_4_n), .I(LVDS_4));   

    assign LVDS_4  = 0;

    assign I2C_SDA = 0;
    assign I2C_SCK = 0;
    

    // SPI bus output pins
    
    reg CS_b, SCLK;
    reg MOSI_A1, MOSI_B1, MOSI_C1, MOSI_D1;
    reg MOSI_A2, MOSI_B2, MOSI_C2, MOSI_D2;
    
    assign  CS_b_A = CS_b;
    assign  SCLK_A = SCLK;
    assign MOSI1_A = MOSI_A1;
    assign MOSI2_A = MOSI_A2;
    
    assign  CS_b_B = 0;
    assign  SCLK_B = 0;
    assign MOSI1_B = 0;
    assign MOSI2_B = 0;

    assign  CS_b_C = CS_b;
    assign  SCLK_C = SCLK;
    assign MOSI1_C = MOSI_B1;
    assign MOSI2_C = MOSI_B2;

    assign  CS_b_D = 0;
    assign  SCLK_D = 0;
    assign MOSI1_D = 0;
    assign MOSI2_D = 0;

    assign  CS_b_E = CS_b;
    assign  SCLK_E = SCLK;
    assign MOSI1_E = MOSI_C1;
    assign MOSI2_E = MOSI_C2;

    assign  CS_b_F = 0;
    assign  SCLK_F = 0;
    assign MOSI1_F = 0;
    assign MOSI2_F = 0;

    assign  CS_b_G = CS_b;
    assign  SCLK_G = SCLK;
    assign MOSI1_G = MOSI_D1;
    assign MOSI2_G = MOSI_D2;

    assign  CS_b_H = 0;
    assign  SCLK_H = 0;
    assign MOSI1_H = 0;
    assign MOSI2_H = 0;
   
    // SPI bus input pins
   
    wire   MISO_A1, MISO_A2;
    wire   MISO_B1, MISO_B2;
    wire   MISO_C1, MISO_C2;
    wire   MISO_D1, MISO_D2;
   
    assign MISO_A1 = MISO1_A;
    assign MISO_A2 = MISO2_A;

    assign MISO_B1 = MISO1_C;
    assign MISO_B2 = MISO2_C;

    assign MISO_C1 = MISO1_E;
    assign MISO_C2 = MISO2_E;

    assign MISO_D1 = MISO1_G;
    assign MISO_D2 = MISO2_G;
      
    // Board ID number and version
   
    localparam BOARD_ID = 16'd800;
    localparam BOARD_VERSION = 16'd1;
   
   
    // Wires and registers
   
	//YB: names have been replaced to be more explicit : these signals refer to the FIFO in which
    //    data waits to be transfered through USB, they were previously named USB_FIFO_data_in...
	reg  [15:0]     USB_FIFO_data_in;
	reg             USB_FIFO_write_to;
	wire [15:0]     USB_FIFO_data_out;
	wire            USB_FIFO_read_from;
	wire [31:0]     num_words_in_USB_FIFO;
    
    //YB: added to implement an extra FIFO dedicated to add some computation
    //    ACQ prefixed regs are for commands from the acquisition state machine
    //    CST prefixed regs are for the fifo that goes to the custom computing area
    //    and its output to the USB FifO
    reg  [15:0]     ACQ_FSM_data_out;
    reg             ACQ_FSM_data_valid;
    reg  [15:0]     CST_FIFO_data_in;
    reg             CST_FIFO_write_to;
	wire [15:0] 	CST_comp_data_out;
	wire			CST_comp_data_valid;



    reg [12:0]      RAM_addr_rd;
    wire [15:0]     RAM_data_out_1_MSW, RAM_data_out_2_MSW, RAM_data_out_3_MSW, RAM_data_out_4_MSW;
    wire [15:0]     RAM_data_out_1_LSW, RAM_data_out_2_LSW, RAM_data_out_3_LSW, RAM_data_out_4_LSW;
   
    reg             shutdown;
      
    reg [5:0]       channel, channel_MISO;  // varies from 0-19 (amplfier channels 0-15, plus 4 auxiliary commands)
    reg [31:0]      MOSI_cmd_A1, MOSI_cmd_B1, MOSI_cmd_C1, MOSI_cmd_D1;
    reg [31:0]      MOSI_cmd_A2, MOSI_cmd_B2, MOSI_cmd_C2, MOSI_cmd_D2;
   
    reg [133:0]     in4x_A1, in4x_A2;
    reg [133:0]     in4x_B1, in4x_B2;
    reg [133:0]     in4x_C1, in4x_C2;
    reg [133:0]     in4x_D1, in4x_D2;
    wire [31:0]     in_A1, in_A2;
    wire [31:0]     in_B1, in_B2;
    wire [31:0]     in_C1, in_C2;
    wire [31:0]     in_D1, in_D2;
   
    wire [3:0]      delay_A, delay_B, delay_C, delay_D;
   
    reg [31:0]      result_A1, result_A2;
    reg [31:0]      result_B1, result_B2;
    reg [31:0]      result_C1, result_C2;
    reg [31:0]      result_D1, result_D2;

    reg [31:0]      timestamp;          
    reg [31:0]      max_timestep;
    wire [31:0]     max_timestep_in;
    wire [31:0]     data_stream_timestamp;
//    wire [63:0]     header_magic_number;  YB: now defined as localparam...
   
    wire [31:0]     data_stream_1, data_stream_2, data_stream_3, data_stream_4;
    wire [31:0]     data_stream_5, data_stream_6, data_stream_7, data_stream_8;
    reg             data_stream_1_en, data_stream_2_en, data_stream_3_en, data_stream_4_en;
    reg             data_stream_5_en, data_stream_6_en, data_stream_7_en, data_stream_8_en;
    wire            data_stream_1_en_in, data_stream_2_en_in, data_stream_3_en_in, data_stream_4_en_in;
    wire            data_stream_5_en_in, data_stream_6_en_in, data_stream_7_en_in, data_stream_8_en_in;
    wire [ 7:0]     data_stream_en; // because it's just more convenient to have a single vector...
   
    reg [15:0]      data_stream_TTL_in, data_stream_TTL_out;
    wire [15:0]     data_stream_ADC_1, data_stream_ADC_2, data_stream_ADC_3, data_stream_ADC_4;
    wire [15:0]     data_stream_ADC_5, data_stream_ADC_6, data_stream_ADC_7, data_stream_ADC_8;
    reg [7:0]       ADC_triggers;
    wire [15:0]     ADC_threshold;
    wire [15:0]     manual_triggers;
   
    wire            reset, SPI_start, SPI_run_continuous;
    reg             SPI_running;

    wire            DSP_settle;
    wire            amp_settle_mode, charge_recov_mode;

    wire [31:0]     MOSI_cmd_selected_A1, MOSI_cmd_selected_B1, MOSI_cmd_selected_C1, MOSI_cmd_selected_D1;
    wire [31:0]     MOSI_cmd_selected_A2, MOSI_cmd_selected_B2, MOSI_cmd_selected_C2, MOSI_cmd_selected_D2;

    reg [31:0]      aux_cmd;
    reg [12:0]      aux_cmd_index_1, aux_cmd_index_2, aux_cmd_index_3, aux_cmd_index_4;
    reg [12:0]      max_aux_cmd_index_1_in, max_aux_cmd_index_2_in, max_aux_cmd_index_3_in, max_aux_cmd_index_4_in;
    reg [12:0]      max_aux_cmd_index_1, max_aux_cmd_index_2, max_aux_cmd_index_3, max_aux_cmd_index_4;
    reg [12:0]      loop_aux_cmd_index_1, loop_aux_cmd_index_2, loop_aux_cmd_index_3, loop_aux_cmd_index_4;

    wire aux_enable_A1, aux_enable_A2, aux_enable_B1, aux_enable_B2, aux_enable_C1, aux_enable_C2, aux_enable_D1, aux_enable_D2;
    wire settle_all_headstages, settle_whole_headstage_A, settle_whole_headstage_B, settle_whole_headstage_C, settle_whole_headstage_D;
   
    wire [4:0]      DAC_channel_sel_1, DAC_channel_sel_2, DAC_channel_sel_3, DAC_channel_sel_4;
    wire [4:0]      DAC_channel_sel_5, DAC_channel_sel_6, DAC_channel_sel_7, DAC_channel_sel_8;
    wire [3:0]      DAC_stream_sel_1, DAC_stream_sel_2, DAC_stream_sel_3, DAC_stream_sel_4;
    wire [3:0]      DAC_stream_sel_5, DAC_stream_sel_6, DAC_stream_sel_7, DAC_stream_sel_8;
    wire            DAC_en_1, DAC_en_2, DAC_en_3, DAC_en_4;
    wire            DAC_en_5, DAC_en_6, DAC_en_7, DAC_en_8;
    reg [15:0]      DAC_pre_register_1, DAC_pre_register_2, DAC_pre_register_3, DAC_pre_register_4;
    reg [15:0]      DAC_pre_register_5, DAC_pre_register_6, DAC_pre_register_7, DAC_pre_register_8;
    reg [15:0]      DAC_register_1, DAC_register_2, DAC_register_3, DAC_register_4;
    reg [15:0]      DAC_register_5, DAC_register_6, DAC_register_7, DAC_register_8;

    reg [15:0]      DAC_manual;
    wire [6:0]      DAC_noise_suppress;
    wire [2:0]      DAC_gain;
   
    reg [15:0]      DAC_thresh_1, DAC_thresh_2, DAC_thresh_3, DAC_thresh_4;
    reg [15:0]      DAC_thresh_5, DAC_thresh_6, DAC_thresh_7, DAC_thresh_8;
    reg             DAC_thresh_pol_1, DAC_thresh_pol_2, DAC_thresh_pol_3, DAC_thresh_pol_4;
    reg             DAC_thresh_pol_5, DAC_thresh_pol_6, DAC_thresh_pol_7, DAC_thresh_pol_8;
    wire [7:0]      DAC_thresh_out;
   
    reg             HPF_en;
    reg [15:0]      HPF_coefficient;
   
    wire            RAM_addr_reset;
   
    wire [7:0]      led_in;

    wire            DAC_reref_mode;
    wire [2:0]      DAC_reref_stream_sel;
    wire [4:0]      DAC_reref_channel_sel;
    reg [15:0]      DAC_reref_register, DAC_reref_pre_register;
    wire            DAC_1_input_is_ref, DAC_2_input_is_ref,  DAC_3_input_is_ref,  DAC_4_input_is_ref;
    wire            DAC_5_input_is_ref, DAC_6_input_is_ref,  DAC_7_input_is_ref,  DAC_8_input_is_ref;
   
    wire            serial_CLK_manual, serial_LOAD_manual;
    wire            reset_sequencers;
   
    // Opal Kelly USB Host Interface
   
    wire [30:0] ok1;
    wire [16:0] ok2;

    wire [15:0] ep00wirein, ep01wirein, ep02wirein, ep03wirein, ep04wirein, ep05wirein, ep06wirein, ep07wirein;
    wire [15:0] ep08wirein, ep09wirein, ep0awirein, ep0bwirein, ep0cwirein, ep0dwirein, ep0ewirein, ep0fwirein;
    wire [15:0] ep10wirein, ep11wirein, ep12wirein, ep13wirein, ep14wirein, ep15wirein, ep16wirein, ep17wirein;
    wire [15:0] ep18wirein, ep19wirein, ep1awirein, ep1bwirein, ep1cwirein, ep1dwirein, ep1ewirein, ep1fwirein;

    wire [15:0] ep20wireout, ep21wireout, ep22wireout, ep23wireout, ep24wireout, ep25wireout, ep26wireout, ep27wireout;
    wire [15:0] ep28wireout, ep29wireout, ep2awireout, ep2bwireout, ep2cwireout, ep2dwireout, ep2ewireout, ep2fwireout;
    wire [15:0] ep30wireout, ep31wireout, ep32wireout, ep33wireout, ep34wireout, ep35wireout, ep36wireout, ep37wireout;
    wire [15:0] ep38wireout, ep39wireout, ep3awireout, ep3bwireout, ep3cwireout, ep3dwireout, ep3ewireout, ep3fwireout;

    wire [15:0] ep40trigin, ep41trigin, ep42trigin, ep43trigin, ep44trigin, ep45trigin;

    wire        pipe_in_write_1_MSW, pipe_in_write_2_MSW, pipe_in_write_3_MSW, pipe_in_write_4_MSW;
    wire        pipe_in_write_1_LSW, pipe_in_write_2_LSW, pipe_in_write_3_LSW, pipe_in_write_4_LSW;
    wire [15:0] pipe_in_data_1_MSW, pipe_in_data_2_MSW, pipe_in_data_3_MSW, pipe_in_data_4_MSW;
    wire [15:0] pipe_in_data_1_LSW, pipe_in_data_2_LSW, pipe_in_data_3_LSW, pipe_in_data_4_LSW;

    wire        stim_cmd_en, prog_trig;
    wire [3:0]  prog_channel, prog_address;
    wire [4:0]  prog_module;
    wire [15:0] prog_word;
    
    
    //YB: the next signals were added to provide communication to the custom computation part
    wire        cst_computing_pending;
    wire [15:0] pipe_usb_to_cst_data;
    wire        pipe_usb_to_cst_en;
    wire [15:0] pipe_cst_to_usb_data;
    wire        pipe_cst_to_usb_read;
    wire [15:0] CST_status;
    wire [7:0]  CST_trig_stim;          // the eight stim triggers for outputs
    wire        CST_enable;             // this one is to determine whether we should enable 
                                        // the custom computation part
    wire        CSTtrig_to_DAC;

    assign status_LEDs = { TTL_in_direct_2, TTL_in_direct_1, SPI_running };
    // Other I/O
    assign spare_1 = 0;
    assign mark_out = SPI_running;
    // assign mark_out = shutdown;

    // Input deserializer
    wire [15:0] TTL_in, TTL_parallel;
    wire serial_CLK_auto, serial_LOAD_auto;

    wire [15:0] DAC_output_register_1, DAC_output_register_2, DAC_output_register_3, DAC_output_register_4;
    wire [15:0] DAC_output_register_5, DAC_output_register_6, DAC_output_register_7, DAC_output_register_8;


/*
##    ## ####### ######      ##     ## ## ######  ####### ## ###    ##     ## ###    ## ######  ##    ## ######## #######
##    ## ##      ##   ##     ##     ## ## ##   ## ##      ## ####   ##     ## ####   ## ##   ## ##    ##    ##    ##
##    ## ####### ######      ##  #  ## ## ######  #####   ## ## ##  ##     ## ## ##  ## ######  ##    ##    ##    #######
##    ##      ## ##   ##     ## ### ## ## ##   ## ##      ## ##  ## ##     ## ##  ## ## ##      ##    ##    ##         ##
 ######  ####### ######       ### ###  ## ##   ## ####### ## ##   ####     ## ##   #### ##       ######     ##    #######
*/

  
   
    // USB WireIn inputs

    assign reset =                    ep00wirein[0];
    assign SPI_run_continuous =       ep00wirein[1];
    assign DSP_settle =               ep00wirein[2];
    assign amp_settle_mode =          ep00wirein[3];
    assign charge_recov_mode =        ep00wirein[4];
    assign DAC_noise_suppress =       ep00wirein[12:6];
    assign DAC_gain =                 ep00wirein[15:13];

    assign max_timestep_in[15:0] =    ep01wirein;
    assign max_timestep_in[31:16] =   ep02wirein;

    always @(posedge dataclk) begin
        max_timestep <= max_timestep_in;
    end

    assign dataclk_M =                { 1'b0, ep03wirein[15:8] };
    assign dataclk_D =                { 1'b0, ep03wirein[7:0] };

    assign delay_A =                  ep04wirein[3:0];
    assign delay_B =                  ep04wirein[7:4];
    assign delay_C =                  ep04wirein[11:8];
    assign delay_D =                  ep04wirein[15:12];
    
    assign stim_cmd_en =              ep05wirein[0];
         
    assign prog_address =             ep06wirein[3:0];
    assign prog_channel =             ep06wirein[7:4];
    assign prog_module =              ep06wirein[12:8];
   
    assign prog_word =                ep07wirein;

    wire DC_amp_convert;
    assign DC_amp_convert =           ep08wirein[0];
   
    wire [15:0] extra_states;
    reg [15:0] extra_state_counter;
    assign extra_states =             ep09wirein;

    assign DAC_reref_channel_sel =    ep0awirein[4:0];
    assign DAC_reref_stream_sel =     ep0awirein[7:5];
    assign DAC_reref_mode =           ep0awirein[8];
   
    //YB: ep0B used for custom computation module
    assign CST_enable =               ep0bwirein[0];
    assign CSTtrig_to_DAC =           ep0bwirein[2];
   
    assign aux_enable_A1 =            ep0cwirein[0];
    assign aux_enable_A2 =            ep0cwirein[1];
    assign aux_enable_B1 =            ep0cwirein[2];
    assign aux_enable_B2 =            ep0cwirein[3];
    assign aux_enable_C1 =            ep0cwirein[4];
    assign aux_enable_C2 =            ep0cwirein[5];
    assign aux_enable_D1 =            ep0cwirein[6];
    assign aux_enable_D2 =            ep0cwirein[7];

    assign settle_whole_headstage_A = ep0dwirein[0];
    assign settle_whole_headstage_B = ep0dwirein[1];
    assign settle_whole_headstage_C = ep0dwirein[2];
    assign settle_whole_headstage_D = ep0dwirein[3];
    assign settle_all_headstages =    ep0dwirein[4];
   
    // ep0e currently unused

    assign ADC_threshold =            ep0fwirein;
    
    assign serial_CLK_manual =        ep10wirein[0];
    assign serial_LOAD_manual =       ep10wirein[1];
   
    assign led_in =                   ep11wirein[7:0];
    assign SPI_port_LEDs =            ep11wirein[15:8];
   
    assign manual_triggers =          ep12wirein;
   
    wire [7:0] TTL_out_mode;
    assign TTL_out_mode =             ep13wirein[7:0];

    assign data_stream_1_en_in =      ep14wirein[0];
    assign data_stream_2_en_in =      ep14wirein[1];
    assign data_stream_3_en_in =      ep14wirein[2];
    assign data_stream_4_en_in =      ep14wirein[3];
    assign data_stream_5_en_in =      ep14wirein[4];
    assign data_stream_6_en_in =      ep14wirein[5];
    assign data_stream_7_en_in =      ep14wirein[6];
    assign data_stream_8_en_in =      ep14wirein[7];
 
    // ep15 currently unused
   
    assign DAC_channel_sel_1 =        ep16wirein[4:0];
    assign DAC_stream_sel_1 =         ep16wirein[8:5];
    assign DAC_en_1 =                 ep16wirein[9];
   
    assign DAC_channel_sel_2 =        ep17wirein[4:0];
    assign DAC_stream_sel_2 =         ep17wirein[8:5];
    assign DAC_en_2 =                 ep17wirein[9];
   
    assign DAC_channel_sel_3 =        ep18wirein[4:0];
    assign DAC_stream_sel_3 =         ep18wirein[8:5];
    assign DAC_en_3 =                 ep18wirein[9];
   
    assign DAC_channel_sel_4 =        ep19wirein[4:0];
    assign DAC_stream_sel_4 =         ep19wirein[8:5];
    assign DAC_en_4 =                 ep19wirein[9];
   
    assign DAC_channel_sel_5 =        ep1awirein[4:0];
    assign DAC_stream_sel_5 =         ep1awirein[8:5];
    assign DAC_en_5 =                 ep1awirein[9];
   
    assign DAC_channel_sel_6 =        ep1bwirein[4:0];
    assign DAC_stream_sel_6 =         ep1bwirein[8:5];
    assign DAC_en_6 =                 ep1bwirein[9];
   
    assign DAC_channel_sel_7 =        ep1cwirein[4:0];
    assign DAC_stream_sel_7 =         ep1cwirein[8:5];
    assign DAC_en_7 =                 ep1cwirein[9];
   
    assign DAC_channel_sel_8 =        ep1dwirein[4:0];
    assign DAC_stream_sel_8 =         ep1dwirein[8:5];
    assign DAC_en_8 =                 ep1dwirein[9];
   
    always @(posedge dataclk) begin
        DAC_manual <=                 ep1ewirein;
    end

   
/*
##    ## ####### ######      ######## ######  ##  ######   ######  ####### ######  ## ###    ##     ## ###    ## ######  ##    ## ######## #######
##    ## ##      ##   ##        ##    ##   ## ## ##       ##       ##      ##   ## ## ####   ##     ## ####   ## ##   ## ##    ##    ##    ##
##    ## ####### ######         ##    ######  ## ##   ### ##   ### #####   ######  ## ## ##  ##     ## ## ##  ## ######  ##    ##    ##    #######
##    ##      ## ##   ##        ##    ##   ## ## ##    ## ##    ## ##      ##   ## ## ##  ## ##     ## ##  ## ## ##      ##    ##    ##         ##
 ######  ####### ######         ##    ##   ## ##  ######   ######  ####### ##   ## ## ##   ####     ## ##   #### ##       ######     ##    #######
*/


    assign DCM_prog_trigger =         ep40trigin[0];
   
    assign SPI_start =                ep41trigin[0];

    assign reset_sequencers =         ep41trigin[1];
   
    assign RAM_addr_reset =           ep42trigin[0];

    assign prog_trig =                ep42trigin[1];

    always @(posedge ep43trigin[0]) begin
        DAC_thresh_1 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[1]) begin
        DAC_thresh_2 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[2]) begin
        DAC_thresh_3 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[3]) begin
        DAC_thresh_4 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[4]) begin
        DAC_thresh_5 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[5]) begin
        DAC_thresh_6 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[6]) begin
        DAC_thresh_7 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[7]) begin
        DAC_thresh_8 <=               ep1fwirein;
    end
    always @(posedge ep43trigin[8]) begin
        DAC_thresh_pol_1 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[9]) begin
        DAC_thresh_pol_2 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[10]) begin
        DAC_thresh_pol_3 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[11]) begin
        DAC_thresh_pol_4 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[12]) begin
        DAC_thresh_pol_5 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[13]) begin
        DAC_thresh_pol_6 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[14]) begin
        DAC_thresh_pol_7 <=           ep1fwirein[0];
    end
    always @(posedge ep43trigin[15]) begin
        DAC_thresh_pol_8 <=           ep1fwirein[0];
    end

    always @(posedge ep44trigin[0]) begin
        HPF_en <=                     ep1fwirein[0];
    end
    always @(posedge ep44trigin[1]) begin
        HPF_coefficient <=            ep1fwirein;
    end
   
    always @(posedge ep45trigin[0]) begin
        max_aux_cmd_index_1_in <=     ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[1]) begin
        max_aux_cmd_index_2_in <=     ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[2]) begin
        max_aux_cmd_index_3_in <=     ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[3]) begin
        max_aux_cmd_index_4_in <=     ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[4]) begin
        loop_aux_cmd_index_1 <=       ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[5]) begin
        loop_aux_cmd_index_2 <=       ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[6]) begin
        loop_aux_cmd_index_3 <=       ep1fwirein[12:0];
    end
    always @(posedge ep45trigin[7]) begin
        loop_aux_cmd_index_4 <=       ep1fwirein[12:0];
    end
   
/*
##    ## ####### ######      ##     ## ## ######  #######  ######  ##    ## ########      ######  ##    ## ######## ######  ##    ## ######## #######
##    ## ##      ##   ##     ##     ## ## ##   ## ##      ##    ## ##    ##    ##        ##    ## ##    ##    ##    ##   ## ##    ##    ##    ##
##    ## ####### ######      ##  #  ## ## ######  #####   ##    ## ##    ##    ##        ##    ## ##    ##    ##    ######  ##    ##    ##    #######
##    ##      ## ##   ##     ## ### ## ## ##   ## ##      ##    ## ##    ##    ##        ##    ## ##    ##    ##    ##      ##    ##    ##         ##
 ######  ####### ######       ### ###  ## ##   ## #######  ######   ######     ##         ######   ######     ##    ##       ######     ##    #######
*/


    assign ep20wireout =             num_words_in_USB_FIFO[15:0];
    assign ep21wireout =             num_words_in_USB_FIFO[31:16];
   
    assign ep22wireout =             { 15'b0, SPI_running };
      
    assign ep23wireout =             TTL_in;
   
    assign ep24wireout =             { 14'b0, DCM_prog_done, dataclk_locked };
   
    assign ep25wireout =             { 12'b0, board_mode };
   
    assign ep26wireout =             { 12'b0, expander_ID_1, expander_detect, TTL_in_serial_exp, TTL_in_serial };

    // Unused; future expansion
    assign ep27wireout =             CST_status;    // status ouput for custom computation
    assign ep28wireout =             16'h0000;
    assign ep29wireout =             16'h0000;
    assign ep2awireout =             16'h0000;
    assign ep2bwireout =             16'h0000;
    assign ep2cwireout =             16'h0000;
    assign ep2dwireout =             16'h0000;
    assign ep2ewireout =             16'h0000;
    assign ep2fwireout =             16'h0000;
    assign ep30wireout =             16'h0000;
    assign ep31wireout =             16'h0000;
    assign ep32wireout =             16'h0000;
    assign ep33wireout =             16'h0000;
    assign ep34wireout =             16'h0000;
    assign ep35wireout =             16'h0000;
    assign ep36wireout =             16'h0000;
    assign ep37wireout =             16'h0000;
    assign ep38wireout =             16'h0000;
    assign ep39wireout =             16'h0000;
    assign ep3awireout =             16'h0000;
    assign ep3bwireout =             16'h0000;
    assign ep3cwireout =             16'h0000;
    assign ep3dwireout =             16'h0000;
    
    assign ep3ewireout =             BOARD_ID;
    assign ep3fwireout =             BOARD_VERSION;
   
   
    // 8-LED Display on Opal Kelly board
   
    assign led = ~{ led_in };


/*
 ###### ##       ######   ###### ##   ##      ######  ####### ###    ## ####### ######   #####  ########  ######  ######
##      ##      ##    ## ##      ##  ##      ##       ##      ####   ## ##      ##   ## ##   ##    ##    ##    ## ##   ##
##      ##      ##    ## ##      #####       ##   ### #####   ## ##  ## #####   ######  #######    ##    ##    ## ######
##      ##      ##    ## ##      ##  ##      ##    ## ##      ##  ## ## ##      ##   ## ##   ##    ##    ##    ## ##   ##
 ###### #######  ######   ###### ##   ##      ######  ####### ##   #### ####### ##   ## ##   ##    ##     ######  ##   ##
*/
   
    // Variable frequency data clock generator
   
    variable_freq_clk_generator #(
        .M_DEFAULT     (42),      // default sample frequency = 30 kS/s/channel
        .D_DEFAULT      (25)
        )
    variable_freq_clk_generator_inst
        (
        .clk1               (clk1),
        .ti_clk            (ti_clk),
        .reset            (reset),
        .M                  (dataclk_M),
        .D                  (dataclk_D),
        .DCM_prog_trigger   (DCM_prog_trigger),
        .clkout            (dataclk),
        .DCM_prog_done      (DCM_prog_done),
        .locked            (dataclk_locked)
        );


/*
####### ## #######  ######      ####### ####### ##      #######  ###### ########  ######  ######
##      ## ##      ##    ##     ##      ##      ##      ##      ##         ##    ##    ## ##   ##
#####   ## #####   ##    ##     ####### #####   ##      #####   ##         ##    ##    ## ######
##      ## ##      ##    ##          ## ##      ##      ##      ##         ##    ##    ## ##   ##
##      ## ##       ######      ####### ####### ####### #######  ######    ##     ######  ##   ##


This part added by YB to make it possible to add computation designs while keeping the possibility
to perform the originalbehavior...
*/


	always @(posedge dataclk) begin
		if (reset) begin
			USB_FIFO_data_in  <= 16'b0;
			USB_FIFO_write_to <=  1'b0;
			CST_FIFO_data_in  <= 16'b0;
			CST_FIFO_write_to <=  1'b0;
		end else begin					
			if (CST_enable) begin
                // custom computation is enabled, so data from the acquisition FSM goes to the
                // custom computation part, and data to the USB FIFO comes from the custom
                // computation part
    			USB_FIFO_data_in  <= CST_comp_data_out;
    			USB_FIFO_write_to <= CST_comp_data_valid;
    			CST_FIFO_data_in  <= ACQ_FSM_data_out;
    			CST_FIFO_write_to <= ACQ_FSM_data_valid;
			end else begin
                // custom computation is disabled, so data from the acquisition FSM directly
                //  goes to the USB FIFO.
    			USB_FIFO_data_in  <= ACQ_FSM_data_out;
    			USB_FIFO_write_to <= ACQ_FSM_data_valid;
    			CST_FIFO_data_in  <= 16'b0;
    			CST_FIFO_write_to <=  1'b0;
			end
		end
	end




/*
####### ## #######  ######      #######  ######  ######      ##    ## ####### ######
##      ## ##      ##    ##     ##      ##    ## ##   ##     ##    ## ##      ##   ##
#####   ## #####   ##    ##     #####   ##    ## ######      ##    ## ####### ######
##      ## ##      ##    ##     ##      ##    ## ##   ##     ##    ##      ## ##   ##
##      ## ##       ######      ##       ######  ##   ##      ######  ####### ######
*/




    // SDRAM FIFO that regulates data flow from Xilinx FPGA to USB interface
   
    SDRAM_FIFO  #(
        .C3_P0_MASK_SIZE           (C3_P0_MASK_SIZE),
        .C3_P0_DATA_PORT_SIZE      (C3_P0_DATA_PORT_SIZE),
        .C3_P1_MASK_SIZE           (C3_P1_MASK_SIZE),
        .C3_P1_DATA_PORT_SIZE      (C3_P1_DATA_PORT_SIZE),
        .DEBUG_EN                  (DEBUG_EN),       
        .C3_MEMCLK_PERIOD          (C3_MEMCLK_PERIOD),       
        .C3_CALIB_SOFT_IP          (C3_CALIB_SOFT_IP),       
        .C3_SIMULATION             (C3_SIMULATION),       
        .C3_HW_TESTING             (C3_HW_TESTING),       
        .C3_RST_ACT_LOW            (C3_RST_ACT_LOW),       
        .C3_INPUT_CLK_TYPE         (C3_INPUT_CLK_TYPE),       
        .C3_MEM_ADDR_ORDER         (C3_MEM_ADDR_ORDER),       
        .C3_NUM_DQ_PINS            (C3_NUM_DQ_PINS),       
        .C3_MEM_ADDR_WIDTH         (C3_MEM_ADDR_WIDTH),       
        .C3_MEM_BANKADDR_WIDTH     (C3_MEM_BANKADDR_WIDTH)
        )
    SDRAM_FIFO_inst
        (
        .ti_clk                    (ti_clk),
        .data_in_clk               (dataclk),
        .clk1_in                   (clk1_in),
        .clk1_out                  (clk1),
        .reset                     (reset),
        .FIFO_write_to             (USB_FIFO_write_to),
        .FIFO_data_in              (USB_FIFO_data_in),
        .FIFO_read_from            (USB_FIFO_read_from),
        .FIFO_data_out             (USB_FIFO_data_out),
        .num_words_in_FIFO         (num_words_in_USB_FIFO),
        .mcb3_dram_dq              (mcb3_dram_dq),
        .mcb3_dram_a               (mcb3_dram_a),
        .mcb3_dram_ba              (mcb3_dram_ba),
        .mcb3_dram_ras_n           (mcb3_dram_ras_n),
        .mcb3_dram_cas_n           (mcb3_dram_cas_n),
        .mcb3_dram_we_n            (mcb3_dram_we_n),
        .mcb3_dram_odt             (mcb3_dram_odt),
        .mcb3_dram_cke             (mcb3_dram_cke),
        .mcb3_dram_dm              (mcb3_dram_dm),
        .mcb3_dram_udqs            (mcb3_dram_udqs),
        .mcb3_dram_udqs_n          (mcb3_dram_udqs_n),
        .mcb3_rzq                  (mcb3_rzq),
        .mcb3_zio                  (mcb3_zio),
        .mcb3_dram_udm             (mcb3_dram_udm),
        .mcb3_dram_dqs             (mcb3_dram_dqs),
        .mcb3_dram_dqs_n           (mcb3_dram_dqs_n),
        .mcb3_dram_ck              (mcb3_dram_ck),
        .mcb3_dram_ck_n            (mcb3_dram_ck_n),
        .mcb3_dram_cs_n            (mcb3_dram_cs_n)
        );


/*
 ###### ##    ## ####### ########  ######  ###    ###     ######  ######   ######   ###### ####### ####### ####### ## ###    ##  ######
##      ##    ## ##         ##    ##    ## ####  ####     ##   ## ##   ## ##    ## ##      ##      ##      ##      ## ####   ## ##
##      ##    ## #######    ##    ##    ## ## #### ##     ######  ######  ##    ## ##      #####   ####### ####### ## ## ##  ## ##   ###
##      ##    ##      ##    ##    ##    ## ##  ##  ##     ##      ##   ## ##    ## ##      ##           ##      ## ## ##  ## ## ##    ##
 ######  ######  #######    ##     ######  ##      ##     ##      ##   ##  ######   ###### ####### ####### ####### ## ##   ####  ######

This part added by YB to provide a subarchitecture isolated from the original files, hoping it will help future developments
*/

     
   // two elements where absent in the orignal cust_arch: clk_enable, ce_out, (generated by simulink, still not clear how they work)
wire ce_out;           // SB not sure about this!!
reg clk_enable=1'b1;

    cust_architecture (
        .clk                       (dataclk), // SB not sure about this!!
        .reset                     (reset),
        .FIFO_write_to             (CST_FIFO_write_to),         // data format is equivalent to USB transfers
        .FIFO_data_in              (CST_FIFO_data_in),
        .comp_data_out_valid       (CST_comp_data_valid),       // If used, the module directly output data to the USB FIFO, therefore
        .comp_data_out             (CST_comp_data_out),         //   it uses the same format
        .trig_stim                 (CST_trig_stim),             // output value for triggers 
        .computing                 (cst_computing_pending),     // this value is True when data is actually being computed
        .pipe_in_data              (pipe_usb_to_cst_data),      // input pipe from USB for individual control
        .pipe_in_en                (pipe_usb_to_cst_en),
        .pipe_out_data             (pipe_cst_to_usb_data),      // output pipe from USB for individual control
        .pipe_out_read             (pipe_cst_to_usb_read),
        .cfg                       (ep0bwirein),
        .data_stream_en            (data_stream_en),
        .status                    (CST_status),                 // module status output
		  .clk_enable					  (clk_enable), // SB not sure about this!!
		  .ce_out						  (ce_out)   // SB not sure about this!!
        );
		  
  /*  CUST_ARCH  #(
        .BOARD_ID                  (BOARD_ID),
        .BOARD_VERSION             (BOARD_VERSION), 
        .HEADER_MAGIC_NUMBER       (header_magic_number)
        )
    custom_proc_architecture
        (
        .clk                       (dataclk),
        .reset                     (reset),
        .FIFO_write_to             (CST_FIFO_write_to),         // data format is equivalent to USB transfers
        .FIFO_data_in              (CST_FIFO_data_in),
        .comp_data_out_valid       (CST_comp_data_valid),       // If used, the module directly output data to the USB FIFO, therefore
        .comp_data_out             (CST_comp_data_out),         //   it uses the same format
        .trig_stim                 (CST_trig_stim),             // output value for triggers 
        .computing                 (cst_computing_pending),     // this value is True when data is actually being computed
        .pipe_in_data              (pipe_usb_to_cst_data),      // input pipe from USB for individual control
        .pipe_in_en                (pipe_usb_to_cst_en),
        .pipe_out_data             (pipe_cst_to_usb_data),      // output pipe from USB for individual control
        .pipe_out_read             (pipe_cst_to_usb_read),
        .cfg                       (ep0bwirein),
        .data_stream_en            (data_stream_en),
        .status                    (CST_status)                 // module status output
        );*/


        assign data_stream_en =    {data_stream_8_en, data_stream_7_en, data_stream_6_en, data_stream_5_en,
                                    data_stream_4_en, data_stream_3_en, data_stream_2_en, data_stream_1_en};

/*
######   #####  ###    ###     ######   #####  ###    ## ##   ## #######
##   ## ##   ## ####  ####     ##   ## ##   ## ####   ## ##  ##  ##
######  ####### ## #### ##     ######  ####### ## ##  ## #####   #######
##   ## ##   ## ##  ##  ##     ##   ## ##   ## ##  ## ## ##  ##       ##
##   ## ##   ## ##      ##     ######  ##   ## ##   #### ##   ## #######
*/


   // MOSI auxiliary command sequence RAM banks

    reg [12:0] RAM_addr_wr_counter;
   
    always @(negedge ti_clk) begin
        if (reset) begin
            RAM_addr_wr_counter <= 13'b0;
        end else begin
            if (RAM_addr_reset)
                RAM_addr_wr_counter <= 13'b0;
            else if (pipe_in_write_1_MSW | pipe_in_write_1_LSW | pipe_in_write_2_MSW | pipe_in_write_2_LSW | pipe_in_write_3_MSW | pipe_in_write_3_LSW | pipe_in_write_4_MSW | pipe_in_write_4_LSW)
                RAM_addr_wr_counter <= RAM_addr_wr_counter + 1;
        end
    end

    RAM_bank RAM_bank_1_MSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_1_MSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_1_MSW),
        .RAM_we(pipe_in_write_1_MSW),
        .reset(reset)
    );
   
    RAM_bank RAM_bank_1_LSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_1_LSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_1_LSW),
        .RAM_we(pipe_in_write_1_LSW),
        .reset(reset)
    );

    RAM_bank RAM_bank_2_MSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_2_MSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_2_MSW),
        .RAM_we(pipe_in_write_2_MSW),
        .reset(reset)
    );
   
    RAM_bank RAM_bank_2_LSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_2_LSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_2_LSW),
        .RAM_we(pipe_in_write_2_LSW),
        .reset(reset)
    );

    RAM_bank RAM_bank_3_MSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_3_MSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_3_MSW),
        .RAM_we(pipe_in_write_3_MSW),
        .reset(reset)
    );
   
    RAM_bank RAM_bank_3_LSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_3_LSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_3_LSW),
        .RAM_we(pipe_in_write_3_LSW),
        .reset(reset)
    );

    RAM_bank RAM_bank_4_MSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_4_MSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_4_MSW),
        .RAM_we(pipe_in_write_4_MSW),
        .reset(reset)
    );
   
    RAM_bank RAM_bank_4_LSW(
        .clk_A(ti_clk),
        .clk_B(dataclk),
        .RAM_addr_A(RAM_addr_wr_counter),
        .RAM_addr_B(RAM_addr_rd),
        .RAM_data_in(pipe_in_data_4_LSW),
        .RAM_data_out_A(),
        .RAM_data_out_B(RAM_data_out_4_LSW),
        .RAM_we(pipe_in_write_4_LSW),
        .reset(reset)
    );

/*
 ######  ######  ###    ### ###    ###  #####  ###    ## ######      ####### ####### ##      #######  ###### ########  ######  ######
##      ##    ## ####  #### ####  #### ##   ## ####   ## ##   ##     ##      ##      ##      ##      ##         ##    ##    ## ##   ##
##      ##    ## ## #### ## ## #### ## ####### ## ##  ## ##   ##     ####### #####   ##      #####   ##         ##    ##    ## ######
##      ##    ## ##  ##  ## ##  ##  ## ##   ## ##  ## ## ##   ##          ## ##      ##      ##      ##         ##    ##    ## ##   ##
 ######  ######  ##      ## ##      ## ##   ## ##   #### ######      ####### ####### ####### #######  ######    ##     ######  ##   ##
*/


    wire [15:0] stim_on_A1, stim_on_A2, stim_on_B1, stim_on_B2, stim_on_C1, stim_on_C2, stim_on_D1, stim_on_D2;
    wire [15:0] stim_pol_A1, stim_pol_A2, stim_pol_B1, stim_pol_B2, stim_pol_C1, stim_pol_C2, stim_pol_D1, stim_pol_D2;
    wire [15:0] charge_recov_A1, charge_recov_A2, charge_recov_B1, charge_recov_B2, charge_recov_C1, charge_recov_C2, charge_recov_D1, charge_recov_D2;
    wire [15:0] amp_settle_A1, amp_settle_A2, amp_settle_B1, amp_settle_B2, amp_settle_C1, amp_settle_C2, amp_settle_D1, amp_settle_D2;
    wire [15:0] amp_settle_A1_pre, amp_settle_A2_pre, amp_settle_B1_pre, amp_settle_B2_pre, amp_settle_C1_pre, amp_settle_C2_pre, amp_settle_D1_pre, amp_settle_D2_pre;
    wire amp_settle_changed_A1, amp_settle_changed_A2, amp_settle_changed_B1, amp_settle_changed_B2;
    wire amp_settle_changed_C1, amp_settle_changed_C2, amp_settle_changed_D1, amp_settle_changed_D2;
   
    command_selector_stim command_selector_stim_A1 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_A1), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_A1), .stim_pol(stim_pol_A1), .charge_recov(charge_recov_A1), .amp_settle(amp_settle_A1),
        .amp_settle_changed(amp_settle_changed_A1), .MOSI_cmd(MOSI_cmd_selected_A1));   
   
    command_selector_stim command_selector_stim_A2 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_A2), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_A2), .stim_pol(stim_pol_A2), .charge_recov(charge_recov_A2), .amp_settle(amp_settle_A2),
        .amp_settle_changed(amp_settle_changed_A2), .MOSI_cmd(MOSI_cmd_selected_A2));   

    command_selector_stim command_selector_stim_B1 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_B1), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_B1), .stim_pol(stim_pol_B1), .charge_recov(charge_recov_B1), .amp_settle(amp_settle_B1),
        .amp_settle_changed(amp_settle_changed_B1), .MOSI_cmd(MOSI_cmd_selected_B1));   
   
    command_selector_stim command_selector_stim_B2 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_B2), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_B2), .stim_pol(stim_pol_B2), .charge_recov(charge_recov_B2), .amp_settle(amp_settle_B2),
        .amp_settle_changed(amp_settle_changed_B2), .MOSI_cmd(MOSI_cmd_selected_B2));   

    command_selector_stim command_selector_stim_C1 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_C1), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_C1), .stim_pol(stim_pol_C1), .charge_recov(charge_recov_C1), .amp_settle(amp_settle_C1),
        .amp_settle_changed(amp_settle_changed_C1), .MOSI_cmd(MOSI_cmd_selected_C1));   
   
    command_selector_stim command_selector_stim_C2 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_C2), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_C2), .stim_pol(stim_pol_C2), .charge_recov(charge_recov_C2), .amp_settle(amp_settle_C2),
        .amp_settle_changed(amp_settle_changed_C2), .MOSI_cmd(MOSI_cmd_selected_C2));   

    command_selector_stim command_selector_stim_D1 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_D1), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_D1), .stim_pol(stim_pol_D1), .charge_recov(charge_recov_D1), .amp_settle(amp_settle_D1),
        .amp_settle_changed(amp_settle_changed_D1), .MOSI_cmd(MOSI_cmd_selected_D1));   
   
    command_selector_stim command_selector_stim_D2 (
        .channel(channel), .shutdown(shutdown), .DSP_settle(DSP_settle), .amp_settle_mode(amp_settle_mode), .charge_recov_mode(charge_recov_mode),
        .aux_cmd(aux_cmd), .use_aux_cmd(aux_enable_D2), .DC_amp_convert(DC_amp_convert), .stim_en(stim_cmd_en),
        .stim_on(stim_on_D2), .stim_pol(stim_pol_D2), .charge_recov(charge_recov_D2), .amp_settle(amp_settle_D2),
        .amp_settle_changed(amp_settle_changed_D2), .MOSI_cmd(MOSI_cmd_selected_D2));   



/*
####### ####### ###    ###     ######  #######  ###### ##       #####  ######   #####  ######## ##  ######  ###    ##
##      ##      ####  ####     ##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ####   ##
#####   ####### ## #### ##     ##   ## #####   ##      ##      ####### ######  #######    ##    ## ##    ## ## ##  ##
##           ## ##  ##  ##     ##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ##  ## ##
##      ####### ##      ##     ######  #######  ###### ####### ##   ## ##   ## ##   ##    ##    ##  ######  ##   ####
*/

   
   
   
    integer main_state;
    localparam
                ms_wait    = 99,
                ms_clk1_a  = 100,
                ms_clk1_b  = 101,
                ms_clk1_c  = 102,
                ms_clk1_d  = 103,
                ms_clk2_a  = 104,
                ms_clk2_b  = 105,
                ms_clk2_c  = 106,
                ms_clk2_d  = 107,
                ms_clk3_a  = 108,
                ms_clk3_b  = 109,
                ms_clk3_c  = 110,
                ms_clk3_d  = 111,
                ms_clk4_a  = 112,
                ms_clk4_b  = 113,
                ms_clk4_c  = 114,
                ms_clk4_d  = 115,
                ms_clk5_a  = 116,
                ms_clk5_b  = 117,
                ms_clk5_c  = 118,
                ms_clk5_d  = 119,
                ms_clk6_a  = 120,
                ms_clk6_b  = 121,
                ms_clk6_c  = 122,
                ms_clk6_d  = 123,
                ms_clk7_a  = 124,
                ms_clk7_b  = 125,
                ms_clk7_c  = 126,
                ms_clk7_d  = 127,
                ms_clk8_a  = 128,
                ms_clk8_b  = 129,
                ms_clk8_c  = 130,
                ms_clk8_d  = 131,
                ms_clk9_a  = 132,
                ms_clk9_b  = 133,
                ms_clk9_c  = 134,
                ms_clk9_d  = 135,
                ms_clk10_a = 136,
                ms_clk10_b = 137,
                ms_clk10_c = 138,
                ms_clk10_d = 139,
                ms_clk11_a = 140,
                ms_clk11_b = 141,
                ms_clk11_c = 142,
                ms_clk11_d = 143,
                ms_clk12_a = 144,
                ms_clk12_b = 145,
                ms_clk12_c = 146,
                ms_clk12_d = 147,
                ms_clk13_a = 148,
                ms_clk13_b = 149,
                ms_clk13_c = 150,
                ms_clk13_d = 151,
                ms_clk14_a = 152,
                ms_clk14_b = 153,
                ms_clk14_c = 154,
                ms_clk14_d = 155,
                ms_clk15_a = 156,
                ms_clk15_b = 157,
                ms_clk15_c = 158,
                ms_clk15_d = 159,
                ms_clk16_a = 160,
                ms_clk16_b = 161,
                ms_clk16_c = 162,
                ms_clk16_d = 163,
                ms_clk17_a = 164,
                ms_clk17_b = 165,
                ms_clk17_c = 166,
                ms_clk17_d = 167,
                ms_clk18_a = 168,
                ms_clk18_b = 169,
                ms_clk18_c = 170,
                ms_clk18_d = 171,
                ms_clk19_a = 172,
                ms_clk19_b = 173,
                ms_clk19_c = 174,
                ms_clk19_d = 175,
                ms_clk20_a = 176,
                ms_clk20_b = 177,
                ms_clk20_c = 178,
                ms_clk20_d = 179,
                ms_clk21_a = 180,
                ms_clk21_b = 181,
                ms_clk21_c = 182,
                ms_clk21_d = 183,
                ms_clk22_a = 184,
                ms_clk22_b = 185,
                ms_clk22_c = 186,
                ms_clk22_d = 187,
                ms_clk23_a = 188,
                ms_clk23_b = 189,
                ms_clk23_c = 190,
                ms_clk23_d = 191,
                ms_clk24_a = 192,
                ms_clk24_b = 193,
                ms_clk24_c = 194,
                ms_clk24_d = 195,
                ms_clk25_a = 196,
                ms_clk25_b = 197,
                ms_clk25_c = 198,
                ms_clk25_d = 199,
                ms_clk26_a = 200,
                ms_clk26_b = 201,
                ms_clk26_c = 202,
                ms_clk26_d = 203,
                ms_clk27_a = 204,
                ms_clk27_b = 205,
                ms_clk27_c = 206,
                ms_clk27_d = 207,
                ms_clk28_a = 208,
                ms_clk28_b = 209,
                ms_clk28_c = 210,
                ms_clk28_d = 211,
                ms_clk29_a = 212,
                ms_clk29_b = 213,
                ms_clk29_c = 214,
                ms_clk29_d = 215,
                ms_clk30_a = 216,
                ms_clk30_b = 217,
                ms_clk30_c = 218,
                ms_clk30_d = 219,
                ms_clk31_a = 220,
                ms_clk31_b = 221,
                ms_clk31_c = 222,
                ms_clk31_d = 223,
                ms_clk32_a = 224,
                ms_clk32_b = 225,
                ms_clk32_c = 226,
                ms_clk32_d = 227,
              
                ms_clk33_a = 228,
                ms_clk33_b = 229,
              
                ms_cs_a    = 230,
                ms_cs_b    = 231,
                ms_cs_c    = 232,
                ms_cs_d    = 233,
                ms_cs_e    = 234,
                ms_cs_f    = 235,
                ms_cs_g    = 236,
                ms_cs_h    = 237,
                ms_cs_i    = 238,
                ms_cs_j    = 239;


/*
####### ####### ###    ###     ######  ####### ####### ## ###    ## ## ######## ##  ######  ###    ##
##      ##      ####  ####     ##   ## ##      ##      ## ####   ## ##    ##    ## ##    ## ####   ##
#####   ####### ## #### ##     ##   ## #####   #####   ## ## ##  ## ##    ##    ## ##    ## ## ##  ##
##           ## ##  ##  ##     ##   ## ##      ##      ## ##  ## ## ##    ##    ## ##    ## ##  ## ##
##      ####### ##      ##     ######  ####### ##      ## ##   #### ##    ##    ##  ######  ##   ####
*/




    always @(posedge dataclk) begin
        if (reset) begin
            main_state <= ms_wait;
            timestamp <= 0;
            sample_CLK_out <= 0;
            channel <= 0;
            CS_b <= 1'b1;
            SCLK <= 1'b0;
            MOSI_A1 <= 1'b0;
            MOSI_B1 <= 1'b0;
            MOSI_C1 <= 1'b0;
            MOSI_D1 <= 1'b0;
            MOSI_A2 <= 1'b0;
            MOSI_B2 <= 1'b0;
            MOSI_C2 <= 1'b0;
            MOSI_D2 <= 1'b0;
            ACQ_FSM_data_out <= 16'b0;
            ACQ_FSM_data_valid <= 1'b0;
            ADC_triggers <= 8'b0;
            shutdown <= 1'b0;
        end else begin
            CS_b <= 1'b0;
            SCLK <= 1'b0;
            ACQ_FSM_data_out <= 16'b0;
            ACQ_FSM_data_valid <= 1'b0;

            case (main_state)
         
                ms_wait: begin
                    timestamp <= 0;
                    sample_CLK_out <= 0;
                    channel <= 0;
                    channel_MISO <= 18;   // channel of MISO output, accounting for 2-cycle pipeline in RHS2000 SPI interface
                    CS_b <= 1'b1;
                    SCLK <= 1'b0;
                    MOSI_A1 <= 1'b0;
                    MOSI_B1 <= 1'b0;
                    MOSI_C1 <= 1'b0;
                    MOSI_D1 <= 1'b0;
                    MOSI_A2 <= 1'b0;
                    MOSI_B2 <= 1'b0;
                    MOSI_C2 <= 1'b0;
                    MOSI_D2 <= 1'b0;
                    ACQ_FSM_data_out <= 16'b0;
                    ACQ_FSM_data_valid <= 1'b0;
                    aux_cmd_index_1 <= 0;
                    aux_cmd_index_2 <= 0;
                    aux_cmd_index_3 <= 0;
                    aux_cmd_index_4 <= 0;
                    max_aux_cmd_index_1 <= max_aux_cmd_index_1_in;
                    max_aux_cmd_index_2 <= max_aux_cmd_index_2_in;
                    max_aux_cmd_index_3 <= max_aux_cmd_index_3_in;
                    max_aux_cmd_index_4 <= max_aux_cmd_index_4_in;
                   
                    data_stream_1_en <= data_stream_1_en_in;      // can only change USB streams after stopping SPI
                    data_stream_2_en <= data_stream_2_en_in;
                    data_stream_3_en <= data_stream_3_en_in;
                    data_stream_4_en <= data_stream_4_en_in;
                    data_stream_5_en <= data_stream_5_en_in;
                    data_stream_6_en <= data_stream_6_en_in;
                    data_stream_7_en <= data_stream_7_en_in;
                    data_stream_8_en <= data_stream_8_en_in;
                   
                    DAC_pre_register_1 <= 16'h8000;      // set DACs to midrange, initially, to avoid loud 'pop' in audio at start
                    DAC_pre_register_2 <= 16'h8000;
                    DAC_pre_register_3 <= 16'h8000;
                    DAC_pre_register_4 <= 16'h8000;
                    DAC_pre_register_5 <= 16'h8000;
                    DAC_pre_register_6 <= 16'h8000;
                    DAC_pre_register_7 <= 16'h8000;
                    DAC_pre_register_8 <= 16'h8000;
                    DAC_reref_pre_register <= 16'h8000;
                   
                    extra_state_counter <= 0;
                   
                    SPI_running <= 1'b0;
                    shutdown <= 1'b0;

                    if (SPI_start) begin
                        main_state <= ms_cs_j;
                    end
                end

                ms_cs_j: begin
                    SPI_running <= 1'b1;
                    MOSI_cmd_A1 <= MOSI_cmd_selected_A1;
                    MOSI_cmd_B1 <= MOSI_cmd_selected_B1;
                    MOSI_cmd_C1 <= MOSI_cmd_selected_C1;
                    MOSI_cmd_D1 <= MOSI_cmd_selected_D1;
                    MOSI_cmd_A2 <= MOSI_cmd_selected_A2;
                    MOSI_cmd_B2 <= MOSI_cmd_selected_B2;
                    MOSI_cmd_C2 <= MOSI_cmd_selected_C2;
                    MOSI_cmd_D2 <= MOSI_cmd_selected_D2;
                    CS_b <= 1'b1;
                    if (extra_state_counter == 0) begin
                        main_state <= ms_clk1_a;
                    end else begin
                        extra_state_counter <= extra_state_counter - 1;
                        main_state <= ms_cs_j;
                    end
                end

                ms_clk1_a: begin
                    extra_state_counter <= extra_states;
                    if (channel == 0 && ~shutdown) begin   // sample clock goes high during channel 0 SPI command
                        sample_CLK_out <= 1'b1;
                    end else begin
                        sample_CLK_out <= 1'b0;
                    end

                    if (channel == 0 && ~shutdown) begin      // grab TTL inputs, and grab current state of TTL outputs and manual DAC outputs
                        data_stream_TTL_in <= TTL_in;
                        data_stream_TTL_out <= TTL_out_direct;               
                    end

                    if (channel == 0 && ~shutdown) begin      // update all DAC registers simultaneously
                        DAC_register_1 <= DAC_pre_register_1;
                        DAC_register_2 <= DAC_pre_register_2;
                        DAC_register_3 <= DAC_pre_register_3;
                        DAC_register_4 <= DAC_pre_register_4;
                        DAC_register_5 <= DAC_pre_register_5;
                        DAC_register_6 <= DAC_pre_register_6;
                        DAC_register_7 <= DAC_pre_register_7;
                        DAC_register_8 <= DAC_pre_register_8;
                        DAC_reref_register <= DAC_reref_pre_register;
                    end

                    MOSI_A1 <= MOSI_cmd_A1[31];
                    MOSI_B1 <= MOSI_cmd_B1[31];
                    MOSI_C1 <= MOSI_cmd_C1[31];
                    MOSI_D1 <= MOSI_cmd_D1[31];
                    MOSI_A2 <= MOSI_cmd_A2[31];
                    MOSI_B2 <= MOSI_cmd_B2[31];
                    MOSI_C2 <= MOSI_cmd_C2[31];
                    MOSI_D2 <= MOSI_cmd_D2[31];
                    main_state <= ms_clk1_b;
                end

                ms_clk1_b: begin
                    // Note: After selecting a new RAM_addr_rd, we must wait two clock cycles before reading from the RAM
                    if (channel == 15 && ~shutdown) begin
                        RAM_addr_rd <= aux_cmd_index_1;
                    end else if (channel == 16 && ~shutdown) begin
                        RAM_addr_rd <= aux_cmd_index_2;
                    end else if (channel == 17 && ~shutdown) begin
                        RAM_addr_rd <= aux_cmd_index_3;
                    end else if (channel == 18 && ~shutdown) begin
                        RAM_addr_rd <= aux_cmd_index_4;
                    end

                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= header_magic_number[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    main_state <= ms_clk1_c;
                end

                ms_clk1_c: begin
                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= header_magic_number[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[0] <= MISO_A1; in4x_A2[0] <= MISO_A2;
                    in4x_B1[0] <= MISO_B1; in4x_B2[0] <= MISO_B2;
                    in4x_C1[0] <= MISO_C1; in4x_C2[0] <= MISO_C2;
                    in4x_D1[0] <= MISO_D1; in4x_D2[0] <= MISO_D2;               
                    main_state <= ms_clk1_d;
                end
                
                ms_clk1_d: begin
                    if (channel == 15 && ~shutdown) begin
                        aux_cmd <= {RAM_data_out_1_MSW, RAM_data_out_1_LSW};
                    end else if (channel == 16 && ~shutdown) begin
                        aux_cmd <= {RAM_data_out_2_MSW, RAM_data_out_2_LSW};
                    end else if (channel == 17 && ~shutdown) begin
                        aux_cmd <= {RAM_data_out_3_MSW, RAM_data_out_3_LSW};
                    end else if (channel == 18 && ~shutdown) begin
                        aux_cmd <= {RAM_data_out_4_MSW, RAM_data_out_4_LSW};
                    end

                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= header_magic_number[47:32];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[1] <= MISO_A1; in4x_A2[1] <= MISO_A2;
                    in4x_B1[1] <= MISO_B1; in4x_B2[1] <= MISO_B2;
                    in4x_C1[1] <= MISO_C1; in4x_C2[1] <= MISO_C2;
                    in4x_D1[1] <= MISO_D1; in4x_D2[1] <= MISO_D2;            
                    main_state <= ms_clk2_a;
                end

                ms_clk2_a: begin
                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= header_magic_number[63:48];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    MOSI_A1 <= MOSI_cmd_A1[30];
                    MOSI_B1 <= MOSI_cmd_B1[30];
                    MOSI_C1 <= MOSI_cmd_C1[30];
                    MOSI_D1 <= MOSI_cmd_D1[30];
                    MOSI_A2 <= MOSI_cmd_A2[30];
                    MOSI_B2 <= MOSI_cmd_B2[30];
                    MOSI_C2 <= MOSI_cmd_C2[30];
                    MOSI_D2 <= MOSI_cmd_D2[30];
                    in4x_A1[2] <= MISO_A1; in4x_A2[2] <= MISO_A2;
                    in4x_B1[2] <= MISO_B1; in4x_B2[2] <= MISO_B2;
                    in4x_C1[2] <= MISO_C1; in4x_C2[2] <= MISO_C2;
                    in4x_D1[2] <= MISO_D1; in4x_D2[2] <= MISO_D2;            
                    main_state <= ms_clk2_b;
                end

                ms_clk2_b: begin
                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= timestamp[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    in4x_A1[3] <= MISO_A1; in4x_A2[3] <= MISO_A2;
                    in4x_B1[3] <= MISO_B1; in4x_B2[3] <= MISO_B2;
                    in4x_C1[3] <= MISO_C1; in4x_C2[3] <= MISO_C2;
                    in4x_D1[3] <= MISO_D1; in4x_D2[3] <= MISO_D2;            
                    main_state <= ms_clk2_c;
                end

                ms_clk2_c: begin
                    if (channel == 0 && ~shutdown) begin
                        ACQ_FSM_data_out <= timestamp[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[4] <= MISO_A1; in4x_A2[4] <= MISO_A2;
                    in4x_B1[4] <= MISO_B1; in4x_B2[4] <= MISO_B2;
                    in4x_C1[4] <= MISO_C1; in4x_C2[4] <= MISO_C2;
                    in4x_D1[4] <= MISO_D1; in4x_D2[4] <= MISO_D2;               
                    main_state <= ms_clk2_d;
                end
                
                ms_clk2_d: begin
                    if (data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_1[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[5] <= MISO_A1; in4x_A2[5] <= MISO_A2;
                    in4x_B1[5] <= MISO_B1; in4x_B2[5] <= MISO_B2;
                    in4x_C1[5] <= MISO_C1; in4x_C2[5] <= MISO_C2;
                    in4x_D1[5] <= MISO_D1; in4x_D2[5] <= MISO_D2;            
                    main_state <= ms_clk3_a;
                end
                
                ms_clk3_a: begin
                    if (data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_1[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    MOSI_A1 <= MOSI_cmd_A1[29];
                    MOSI_B1 <= MOSI_cmd_B1[29];
                    MOSI_C1 <= MOSI_cmd_C1[29];
                    MOSI_D1 <= MOSI_cmd_D1[29];
                    MOSI_A2 <= MOSI_cmd_A2[29];
                    MOSI_B2 <= MOSI_cmd_B2[29];
                    MOSI_C2 <= MOSI_cmd_C2[29];
                    MOSI_D2 <= MOSI_cmd_D2[29];
                    in4x_A1[6] <= MISO_A1; in4x_A2[6] <= MISO_A2;
                    in4x_B1[6] <= MISO_B1; in4x_B2[6] <= MISO_B2;
                    in4x_C1[6] <= MISO_C1; in4x_C2[6] <= MISO_C2;
                    in4x_D1[6] <= MISO_D1; in4x_D2[6] <= MISO_D2;            
                    main_state <= ms_clk3_b;
                end

                ms_clk3_b: begin
                    if (data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_2[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    in4x_A1[7] <= MISO_A1; in4x_A2[7] <= MISO_A2;
                    in4x_B1[7] <= MISO_B1; in4x_B2[7] <= MISO_B2;
                    in4x_C1[7] <= MISO_C1; in4x_C2[7] <= MISO_C2;
                    in4x_D1[7] <= MISO_D1; in4x_D2[7] <= MISO_D2;            
                    main_state <= ms_clk3_c;
                end

                ms_clk3_c: begin
                    if (data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_2[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[8] <= MISO_A1; in4x_A2[8] <= MISO_A2;
                    in4x_B1[8] <= MISO_B1; in4x_B2[8] <= MISO_B2;
                    in4x_C1[8] <= MISO_C1; in4x_C2[8] <= MISO_C2;
                    in4x_D1[8] <= MISO_D1; in4x_D2[8] <= MISO_D2;               
                    main_state <= ms_clk3_d;
                end
                
                ms_clk3_d: begin
                    if (data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_3[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[9] <= MISO_A1; in4x_A2[9] <= MISO_A2;
                    in4x_B1[9] <= MISO_B1; in4x_B2[9] <= MISO_B2;
                    in4x_C1[9] <= MISO_C1; in4x_C2[9] <= MISO_C2;
                    in4x_D1[9] <= MISO_D1; in4x_D2[9] <= MISO_D2;            
                    main_state <= ms_clk4_a;
                end

                ms_clk4_a: begin
                    if (data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_3[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    MOSI_A1 <= MOSI_cmd_A1[28];
                    MOSI_B1 <= MOSI_cmd_B1[28];
                    MOSI_C1 <= MOSI_cmd_C1[28];
                    MOSI_D1 <= MOSI_cmd_D1[28];
                    MOSI_A2 <= MOSI_cmd_A2[28];
                    MOSI_B2 <= MOSI_cmd_B2[28];
                    MOSI_C2 <= MOSI_cmd_C2[28];
                    MOSI_D2 <= MOSI_cmd_D2[28];
                    in4x_A1[10] <= MISO_A1; in4x_A2[10] <= MISO_A2;
                    in4x_B1[10] <= MISO_B1; in4x_B2[10] <= MISO_B2;
                    in4x_C1[10] <= MISO_C1; in4x_C2[10] <= MISO_C2;
                    in4x_D1[10] <= MISO_D1; in4x_D2[10] <= MISO_D2;            
                    main_state <= ms_clk4_b;
                end

                ms_clk4_b: begin
                    if (data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_4[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    in4x_A1[11] <= MISO_A1; in4x_A2[11] <= MISO_A2;
                    in4x_B1[11] <= MISO_B1; in4x_B2[11] <= MISO_B2;
                    in4x_C1[11] <= MISO_C1; in4x_C2[11] <= MISO_C2;
                    in4x_D1[11] <= MISO_D1; in4x_D2[11] <= MISO_D2;            
                    main_state <= ms_clk4_c;
                end

                ms_clk4_c: begin
                    if (data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_4[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[12] <= MISO_A1; in4x_A2[12] <= MISO_A2;
                    in4x_B1[12] <= MISO_B1; in4x_B2[12] <= MISO_B2;
                    in4x_C1[12] <= MISO_C1; in4x_C2[12] <= MISO_C2;
                    in4x_D1[12] <= MISO_D1; in4x_D2[12] <= MISO_D2;               
                    main_state <= ms_clk4_d;
                end
                
                ms_clk4_d: begin
                    if (data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_5[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    SCLK <= 1'b1;
                    in4x_A1[13] <= MISO_A1; in4x_A2[13] <= MISO_A2;
                    in4x_B1[13] <= MISO_B1; in4x_B2[13] <= MISO_B2;
                    in4x_C1[13] <= MISO_C1; in4x_C2[13] <= MISO_C2;
                    in4x_D1[13] <= MISO_D1; in4x_D2[13] <= MISO_D2;            
                    main_state <= ms_clk5_a;
                end
            
                ms_clk5_a: begin
                    if (data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_5[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end

                    MOSI_A1 <= MOSI_cmd_A1[27];
                    MOSI_B1 <= MOSI_cmd_B1[27];
                    MOSI_C1 <= MOSI_cmd_C1[27];
                    MOSI_D1 <= MOSI_cmd_D1[27];
                    MOSI_A2 <= MOSI_cmd_A2[27];
                    MOSI_B2 <= MOSI_cmd_B2[27];
                    MOSI_C2 <= MOSI_cmd_C2[27];
                    MOSI_D2 <= MOSI_cmd_D2[27];
                    in4x_A1[14] <= MISO_A1; in4x_A2[14] <= MISO_A2;
                    in4x_B1[14] <= MISO_B1; in4x_B2[14] <= MISO_B2;
                    in4x_C1[14] <= MISO_C1; in4x_C2[14] <= MISO_C2;
                    in4x_D1[14] <= MISO_D1; in4x_D2[14] <= MISO_D2;            
                    main_state <= ms_clk5_b;
                end

                ms_clk5_b: begin
                    if (data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_6[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    in4x_A1[15] <= MISO_A1; in4x_A2[15] <= MISO_A2;
                    in4x_B1[15] <= MISO_B1; in4x_B2[15] <= MISO_B2;
                    in4x_C1[15] <= MISO_C1; in4x_C2[15] <= MISO_C2;
                    in4x_D1[15] <= MISO_D1; in4x_D2[15] <= MISO_D2;            
                    main_state <= ms_clk5_c;
                end

                ms_clk5_c: begin
                    if (data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_6[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    SCLK <= 1'b1;
                    in4x_A1[16] <= MISO_A1; in4x_A2[16] <= MISO_A2;
                    in4x_B1[16] <= MISO_B1; in4x_B2[16] <= MISO_B2;
                    in4x_C1[16] <= MISO_C1; in4x_C2[16] <= MISO_C2;
                    in4x_D1[16] <= MISO_D1; in4x_D2[16] <= MISO_D2;               
                    main_state <= ms_clk5_d;
                end
                
                ms_clk5_d: begin
                    if (data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_7[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    SCLK <= 1'b1;
                    in4x_A1[17] <= MISO_A1; in4x_A2[17] <= MISO_A2;
                    in4x_B1[17] <= MISO_B1; in4x_B2[17] <= MISO_B2;
                    in4x_C1[17] <= MISO_C1; in4x_C2[17] <= MISO_C2;
                    in4x_D1[17] <= MISO_D1; in4x_D2[17] <= MISO_D2;            
                    main_state <= ms_clk6_a;
                end
            
                ms_clk6_a: begin
                    if (data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_7[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    MOSI_A1 <= MOSI_cmd_A1[26];
                    MOSI_B1 <= MOSI_cmd_B1[26];
                    MOSI_C1 <= MOSI_cmd_C1[26];
                    MOSI_D1 <= MOSI_cmd_D1[26];
                    MOSI_A2 <= MOSI_cmd_A2[26];
                    MOSI_B2 <= MOSI_cmd_B2[26];
                    MOSI_C2 <= MOSI_cmd_C2[26];
                    MOSI_D2 <= MOSI_cmd_D2[26];
                    in4x_A1[18] <= MISO_A1; in4x_A2[18] <= MISO_A2;
                    in4x_B1[18] <= MISO_B1; in4x_B2[18] <= MISO_B2;
                    in4x_C1[18] <= MISO_C1; in4x_C2[18] <= MISO_C2;
                    in4x_D1[18] <= MISO_D1; in4x_D2[18] <= MISO_D2;            
                    main_state <= ms_clk6_b;
                end

                ms_clk6_b: begin
                    if (data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_8[15:0];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    in4x_A1[19] <= MISO_A1; in4x_A2[19] <= MISO_A2;
                    in4x_B1[19] <= MISO_B1; in4x_B2[19] <= MISO_B2;
                    in4x_C1[19] <= MISO_C1; in4x_C2[19] <= MISO_C2;
                    in4x_D1[19] <= MISO_D1; in4x_D2[19] <= MISO_D2;            
                    main_state <= ms_clk6_c;
                end

                ms_clk6_c: begin
                    if (data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_8[31:16];
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                   
                    SCLK <= 1'b1;
                    in4x_A1[20] <= MISO_A1; in4x_A2[20] <= MISO_A2;
                    in4x_B1[20] <= MISO_B1; in4x_B2[20] <= MISO_B2;
                    in4x_C1[20] <= MISO_C1; in4x_C2[20] <= MISO_C2;
                    in4x_D1[20] <= MISO_D1; in4x_D2[20] <= MISO_D2;               
                    main_state <= ms_clk6_d;
                end
                
                ms_clk6_d: begin
                    if (channel == 19 && data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_A1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[21] <= MISO_A1; in4x_A2[21] <= MISO_A2;
                    in4x_B1[21] <= MISO_B1; in4x_B2[21] <= MISO_B2;
                    in4x_C1[21] <= MISO_C1; in4x_C2[21] <= MISO_C2;
                    in4x_D1[21] <= MISO_D1; in4x_D2[21] <= MISO_D2;            
                    main_state <= ms_clk7_a;
                end
            
                ms_clk7_a: begin
                    if (channel == 19 && data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_A2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[25];
                    MOSI_B1 <= MOSI_cmd_B1[25];
                    MOSI_C1 <= MOSI_cmd_C1[25];
                    MOSI_D1 <= MOSI_cmd_D1[25];
                    MOSI_A2 <= MOSI_cmd_A2[25];
                    MOSI_B2 <= MOSI_cmd_B2[25];
                    MOSI_C2 <= MOSI_cmd_C2[25];
                    MOSI_D2 <= MOSI_cmd_D2[25];
                    in4x_A1[22] <= MISO_A1; in4x_A2[22] <= MISO_A2;
                    in4x_B1[22] <= MISO_B1; in4x_B2[22] <= MISO_B2;
                    in4x_C1[22] <= MISO_C1; in4x_C2[22] <= MISO_C2;
                    in4x_D1[22] <= MISO_D1; in4x_D2[22] <= MISO_D2;            
                    main_state <= ms_clk7_b;
                end

                ms_clk7_b: begin
                    if (channel == 19 && data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_B1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[23] <= MISO_A1; in4x_A2[23] <= MISO_A2;
                    in4x_B1[23] <= MISO_B1; in4x_B2[23] <= MISO_B2;
                    in4x_C1[23] <= MISO_C1; in4x_C2[23] <= MISO_C2;
                    in4x_D1[23] <= MISO_D1; in4x_D2[23] <= MISO_D2;            
                    main_state <= ms_clk7_c;
                end

                ms_clk7_c: begin
                    if (channel == 19 && data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_B2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    SCLK <= 1'b1;
                    in4x_A1[24] <= MISO_A1; in4x_A2[24] <= MISO_A2;
                    in4x_B1[24] <= MISO_B1; in4x_B2[24] <= MISO_B2;
                    in4x_C1[24] <= MISO_C1; in4x_C2[24] <= MISO_C2;
                    in4x_D1[24] <= MISO_D1; in4x_D2[24] <= MISO_D2;               
                    main_state <= ms_clk7_d;
                end
                
                ms_clk7_d: begin
                    if (channel == 19 && data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_C1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    SCLK <= 1'b1;
                    in4x_A1[25] <= MISO_A1; in4x_A2[25] <= MISO_A2;
                    in4x_B1[25] <= MISO_B1; in4x_B2[25] <= MISO_B2;
                    in4x_C1[25] <= MISO_C1; in4x_C2[25] <= MISO_C2;
                    in4x_D1[25] <= MISO_D1; in4x_D2[25] <= MISO_D2;            
                    main_state <= ms_clk8_a;
                end

                ms_clk8_a: begin
                    if (channel == 19 && data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_C2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    MOSI_A1 <= MOSI_cmd_A1[24];
                    MOSI_B1 <= MOSI_cmd_B1[24];
                    MOSI_C1 <= MOSI_cmd_C1[24];
                    MOSI_D1 <= MOSI_cmd_D1[24];
                    MOSI_A2 <= MOSI_cmd_A2[24];
                    MOSI_B2 <= MOSI_cmd_B2[24];
                    MOSI_C2 <= MOSI_cmd_C2[24];
                    MOSI_D2 <= MOSI_cmd_D2[24];
                    in4x_A1[26] <= MISO_A1; in4x_A2[26] <= MISO_A2;
                    in4x_B1[26] <= MISO_B1; in4x_B2[26] <= MISO_B2;
                    in4x_C1[26] <= MISO_C1; in4x_C2[26] <= MISO_C2;
                    in4x_D1[26] <= MISO_D1; in4x_D2[26] <= MISO_D2;            
                    main_state <= ms_clk8_b;
                end

                ms_clk8_b: begin
                    if (channel == 19 && data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_D1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    in4x_A1[27] <= MISO_A1; in4x_A2[27] <= MISO_A2;
                    in4x_B1[27] <= MISO_B1; in4x_B2[27] <= MISO_B2;
                    in4x_C1[27] <= MISO_C1; in4x_C2[27] <= MISO_C2;
                    in4x_D1[27] <= MISO_D1; in4x_D2[27] <= MISO_D2;            
                    main_state <= ms_clk8_c;
                end

                ms_clk8_c: begin
                    if (channel == 19 && data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_on_D2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[28] <= MISO_A1; in4x_A2[28] <= MISO_A2;
                    in4x_B1[28] <= MISO_B1; in4x_B2[28] <= MISO_B2;
                    in4x_C1[28] <= MISO_C1; in4x_C2[28] <= MISO_C2;
                    in4x_D1[28] <= MISO_D1; in4x_D2[28] <= MISO_D2;               
                    main_state <= ms_clk8_d;
                end
                
                ms_clk8_d: begin
                    if (channel == 19 && data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_A1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[29] <= MISO_A1; in4x_A2[29] <= MISO_A2;
                    in4x_B1[29] <= MISO_B1; in4x_B2[29] <= MISO_B2;
                    in4x_C1[29] <= MISO_C1; in4x_C2[29] <= MISO_C2;
                    in4x_D1[29] <= MISO_D1; in4x_D2[29] <= MISO_D2;            
                    main_state <= ms_clk9_a;
                end

                ms_clk9_a: begin
                    if (channel == 19 && data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_A2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[23];
                    MOSI_B1 <= MOSI_cmd_B1[23];
                    MOSI_C1 <= MOSI_cmd_C1[23];
                    MOSI_D1 <= MOSI_cmd_D1[23];
                    MOSI_A2 <= MOSI_cmd_A2[23];
                    MOSI_B2 <= MOSI_cmd_B2[23];
                    MOSI_C2 <= MOSI_cmd_C2[23];
                    MOSI_D2 <= MOSI_cmd_D2[23];
                    in4x_A1[30] <= MISO_A1; in4x_A2[30] <= MISO_A2;
                    in4x_B1[30] <= MISO_B1; in4x_B2[30] <= MISO_B2;
                    in4x_C1[30] <= MISO_C1; in4x_C2[30] <= MISO_C2;
                    in4x_D1[30] <= MISO_D1; in4x_D2[30] <= MISO_D2;            
                    main_state <= ms_clk9_b;
                end

                ms_clk9_b: begin
                    if (channel == 19 && data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_B1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    in4x_A1[31] <= MISO_A1; in4x_A2[31] <= MISO_A2;
                    in4x_B1[31] <= MISO_B1; in4x_B2[31] <= MISO_B2;
                    in4x_C1[31] <= MISO_C1; in4x_C2[31] <= MISO_C2;
                    in4x_D1[31] <= MISO_D1; in4x_D2[31] <= MISO_D2;            
                    main_state <= ms_clk9_c;
                end

                ms_clk9_c: begin
                    if (channel == 19 && data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_B2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[32] <= MISO_A1; in4x_A2[32] <= MISO_A2;
                    in4x_B1[32] <= MISO_B1; in4x_B2[32] <= MISO_B2;
                    in4x_C1[32] <= MISO_C1; in4x_C2[32] <= MISO_C2;
                    in4x_D1[32] <= MISO_D1; in4x_D2[32] <= MISO_D2;               
                    main_state <= ms_clk9_d;
                end
                
                ms_clk9_d: begin
                    if (channel == 19 && data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_C1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[33] <= MISO_A1; in4x_A2[33] <= MISO_A2;
                    in4x_B1[33] <= MISO_B1; in4x_B2[33] <= MISO_B2;
                    in4x_C1[33] <= MISO_C1; in4x_C2[33] <= MISO_C2;
                    in4x_D1[33] <= MISO_D1; in4x_D2[33] <= MISO_D2;            
                    main_state <= ms_clk10_a;
                end

                ms_clk10_a: begin
                    if (channel == 19 && data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_C2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[22];
                    MOSI_B1 <= MOSI_cmd_B1[22];
                    MOSI_C1 <= MOSI_cmd_C1[22];
                    MOSI_D1 <= MOSI_cmd_D1[22];
                    MOSI_A2 <= MOSI_cmd_A2[22];
                    MOSI_B2 <= MOSI_cmd_B2[22];
                    MOSI_C2 <= MOSI_cmd_C2[22];
                    MOSI_D2 <= MOSI_cmd_D2[22];
                    in4x_A1[34] <= MISO_A1; in4x_A2[34] <= MISO_A2;
                    in4x_B1[34] <= MISO_B1; in4x_B2[34] <= MISO_B2;
                    in4x_C1[34] <= MISO_C1; in4x_C2[34] <= MISO_C2;
                    in4x_D1[34] <= MISO_D1; in4x_D2[34] <= MISO_D2;            
                    main_state <= ms_clk10_b;
                end

                ms_clk10_b: begin
                    if (channel == 19 && data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_D1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[35] <= MISO_A1; in4x_A2[35] <= MISO_A2;
                    in4x_B1[35] <= MISO_B1; in4x_B2[35] <= MISO_B2;
                    in4x_C1[35] <= MISO_C1; in4x_C2[35] <= MISO_C2;
                    in4x_D1[35] <= MISO_D1; in4x_D2[35] <= MISO_D2;            
                    main_state <= ms_clk10_c;
                end

                ms_clk10_c: begin
                    if (channel == 19 && data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= stim_pol_D2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[36] <= MISO_A1; in4x_A2[36] <= MISO_A2;
                    in4x_B1[36] <= MISO_B1; in4x_B2[36] <= MISO_B2;
                    in4x_C1[36] <= MISO_C1; in4x_C2[36] <= MISO_C2;
                    in4x_D1[36] <= MISO_D1; in4x_D2[36] <= MISO_D2;               
                    main_state <= ms_clk10_d;
                end
                
                ms_clk10_d: begin
                    if (channel == 19 && data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_A1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[37] <= MISO_A1; in4x_A2[37] <= MISO_A2;
                    in4x_B1[37] <= MISO_B1; in4x_B2[37] <= MISO_B2;
                    in4x_C1[37] <= MISO_C1; in4x_C2[37] <= MISO_C2;
                    in4x_D1[37] <= MISO_D1; in4x_D2[37] <= MISO_D2;            
                    main_state <= ms_clk11_a;
                end

                ms_clk11_a: begin
                    if (channel == 19 && data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_A2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[21];
                    MOSI_B1 <= MOSI_cmd_B1[21];
                    MOSI_C1 <= MOSI_cmd_C1[21];
                    MOSI_D1 <= MOSI_cmd_D1[21];
                    MOSI_A2 <= MOSI_cmd_A2[21];
                    MOSI_B2 <= MOSI_cmd_B2[21];
                    MOSI_C2 <= MOSI_cmd_C2[21];
                    MOSI_D2 <= MOSI_cmd_D2[21];
                    in4x_A1[38] <= MISO_A1; in4x_A2[38] <= MISO_A2;
                    in4x_B1[38] <= MISO_B1; in4x_B2[38] <= MISO_B2;
                    in4x_C1[38] <= MISO_C1; in4x_C2[38] <= MISO_C2;
                    in4x_D1[38] <= MISO_D1; in4x_D2[38] <= MISO_D2;            
                    main_state <= ms_clk11_b;
                end

                ms_clk11_b: begin
                    if (channel == 19 && data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_B1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[39] <= MISO_A1; in4x_A2[39] <= MISO_A2;
                    in4x_B1[39] <= MISO_B1; in4x_B2[39] <= MISO_B2;
                    in4x_C1[39] <= MISO_C1; in4x_C2[39] <= MISO_C2;
                    in4x_D1[39] <= MISO_D1; in4x_D2[39] <= MISO_D2;            
                    main_state <= ms_clk11_c;
                end

                ms_clk11_c: begin
                    if (channel == 19 && data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_B2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[40] <= MISO_A1; in4x_A2[40] <= MISO_A2;
                    in4x_B1[40] <= MISO_B1; in4x_B2[40] <= MISO_B2;
                    in4x_C1[40] <= MISO_C1; in4x_C2[40] <= MISO_C2;
                    in4x_D1[40] <= MISO_D1; in4x_D2[40] <= MISO_D2;               
                    main_state <= ms_clk11_d;
                end
            
                ms_clk11_d: begin
                    if (channel == 19 && data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_C1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[41] <= MISO_A1; in4x_A2[41] <= MISO_A2;
                    in4x_B1[41] <= MISO_B1; in4x_B2[41] <= MISO_B2;
                    in4x_C1[41] <= MISO_C1; in4x_C2[41] <= MISO_C2;
                    in4x_D1[41] <= MISO_D1; in4x_D2[41] <= MISO_D2;            
                    main_state <= ms_clk12_a;
                end

                ms_clk12_a: begin
                    if (channel == 19 && data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_C2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[20];
                    MOSI_B1 <= MOSI_cmd_B1[20];
                    MOSI_C1 <= MOSI_cmd_C1[20];
                    MOSI_D1 <= MOSI_cmd_D1[20];
                    MOSI_A2 <= MOSI_cmd_A2[20];
                    MOSI_B2 <= MOSI_cmd_B2[20];
                    MOSI_C2 <= MOSI_cmd_C2[20];
                    MOSI_D2 <= MOSI_cmd_D2[20];
                    in4x_A1[42] <= MISO_A1; in4x_A2[42] <= MISO_A2;
                    in4x_B1[42] <= MISO_B1; in4x_B2[42] <= MISO_B2;
                    in4x_C1[42] <= MISO_C1; in4x_C2[42] <= MISO_C2;
                    in4x_D1[42] <= MISO_D1; in4x_D2[42] <= MISO_D2;            
                    main_state <= ms_clk12_b;
                end

                ms_clk12_b: begin
                    if (channel == 19 && data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_D1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[43] <= MISO_A1; in4x_A2[43] <= MISO_A2;
                    in4x_B1[43] <= MISO_B1; in4x_B2[43] <= MISO_B2;
                    in4x_C1[43] <= MISO_C1; in4x_C2[43] <= MISO_C2;
                    in4x_D1[43] <= MISO_D1; in4x_D2[43] <= MISO_D2;            
                    main_state <= ms_clk12_c;
                end

                ms_clk12_c: begin
                    if (channel == 19 && data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= amp_settle_D2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[44] <= MISO_A1; in4x_A2[44] <= MISO_A2;
                    in4x_B1[44] <= MISO_B1; in4x_B2[44] <= MISO_B2;
                    in4x_C1[44] <= MISO_C1; in4x_C2[44] <= MISO_C2;
                    in4x_D1[44] <= MISO_D1; in4x_D2[44] <= MISO_D2;               
                    main_state <= ms_clk12_d;
                end
                
                ms_clk12_d: begin
                    if (channel == 19 && data_stream_1_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_A1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[45] <= MISO_A1; in4x_A2[45] <= MISO_A2;
                    in4x_B1[45] <= MISO_B1; in4x_B2[45] <= MISO_B2;
                    in4x_C1[45] <= MISO_C1; in4x_C2[45] <= MISO_C2;
                    in4x_D1[45] <= MISO_D1; in4x_D2[45] <= MISO_D2;            
                    main_state <= ms_clk13_a;
                end

                ms_clk13_a: begin
                    if (channel == 19 && data_stream_2_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_A2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[19];
                    MOSI_B1 <= MOSI_cmd_B1[19];
                    MOSI_C1 <= MOSI_cmd_C1[19];
                    MOSI_D1 <= MOSI_cmd_D1[19];
                    MOSI_A2 <= MOSI_cmd_A2[19];
                    MOSI_B2 <= MOSI_cmd_B2[19];
                    MOSI_C2 <= MOSI_cmd_C2[19];
                    MOSI_D2 <= MOSI_cmd_D2[19];
                    in4x_A1[46] <= MISO_A1; in4x_A2[46] <= MISO_A2;
                    in4x_B1[46] <= MISO_B1; in4x_B2[46] <= MISO_B2;
                    in4x_C1[46] <= MISO_C1; in4x_C2[46] <= MISO_C2;
                    in4x_D1[46] <= MISO_D1; in4x_D2[46] <= MISO_D2;            
                    main_state <= ms_clk13_b;
                end

                ms_clk13_b: begin
                    if (channel == 19 && data_stream_3_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_B1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[47] <= MISO_A1; in4x_A2[47] <= MISO_A2;
                    in4x_B1[47] <= MISO_B1; in4x_B2[47] <= MISO_B2;
                    in4x_C1[47] <= MISO_C1; in4x_C2[47] <= MISO_C2;
                    in4x_D1[47] <= MISO_D1; in4x_D2[47] <= MISO_D2;            
                    main_state <= ms_clk13_c;
                end

                ms_clk13_c: begin
                    if (channel == 19 && data_stream_4_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_B2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[48] <= MISO_A1; in4x_A2[48] <= MISO_A2;
                    in4x_B1[48] <= MISO_B1; in4x_B2[48] <= MISO_B2;
                    in4x_C1[48] <= MISO_C1; in4x_C2[48] <= MISO_C2;
                    in4x_D1[48] <= MISO_D1; in4x_D2[48] <= MISO_D2;               
                    main_state <= ms_clk13_d;
                end
                
                ms_clk13_d: begin
                    if (channel == 19 && data_stream_5_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_C1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[49] <= MISO_A1; in4x_A2[49] <= MISO_A2;
                    in4x_B1[49] <= MISO_B1; in4x_B2[49] <= MISO_B2;
                    in4x_C1[49] <= MISO_C1; in4x_C2[49] <= MISO_C2;
                    in4x_D1[49] <= MISO_D1; in4x_D2[49] <= MISO_D2;            
                    main_state <= ms_clk14_a;
                end

                ms_clk14_a: begin
                    if (channel == 19 && data_stream_6_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_C2;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[18];
                    MOSI_B1 <= MOSI_cmd_B1[18];
                    MOSI_C1 <= MOSI_cmd_C1[18];
                    MOSI_D1 <= MOSI_cmd_D1[18];
                    MOSI_A2 <= MOSI_cmd_A2[18];
                    MOSI_B2 <= MOSI_cmd_B2[18];
                    MOSI_C2 <= MOSI_cmd_C2[18];
                    MOSI_D2 <= MOSI_cmd_D2[18];
                    in4x_A1[50] <= MISO_A1; in4x_A2[50] <= MISO_A2;
                    in4x_B1[50] <= MISO_B1; in4x_B2[50] <= MISO_B2;
                    in4x_C1[50] <= MISO_C1; in4x_C2[50] <= MISO_C2;
                    in4x_D1[50] <= MISO_D1; in4x_D2[50] <= MISO_D2;            
                    main_state <= ms_clk14_b;
                end

                ms_clk14_b: begin
                    if (channel == 19 && data_stream_7_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_D1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end            
                    in4x_A1[51] <= MISO_A1; in4x_A2[51] <= MISO_A2;
                    in4x_B1[51] <= MISO_B1; in4x_B2[51] <= MISO_B2;
                    in4x_C1[51] <= MISO_C1; in4x_C2[51] <= MISO_C2;
                    in4x_D1[51] <= MISO_D1; in4x_D2[51] <= MISO_D2;            
                    main_state <= ms_clk14_c;
                end

                ms_clk14_c: begin
                    if (channel == 19 && data_stream_8_en == 1'b1 && ~shutdown) begin
                        ACQ_FSM_data_out <= charge_recov_D1;
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[52] <= MISO_A1; in4x_A2[52] <= MISO_A2;
                    in4x_B1[52] <= MISO_B1; in4x_B2[52] <= MISO_B2;
                    in4x_C1[52] <= MISO_C1; in4x_C2[52] <= MISO_C2;
                    in4x_D1[52] <= MISO_D1; in4x_D2[52] <= MISO_D2;               
                    main_state <= ms_clk14_d;
                end
                
                ms_clk14_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[53] <= MISO_A1; in4x_A2[53] <= MISO_A2;
                    in4x_B1[53] <= MISO_B1; in4x_B2[53] <= MISO_B2;
                    in4x_C1[53] <= MISO_C1; in4x_C2[53] <= MISO_C2;
                    in4x_D1[53] <= MISO_D1; in4x_D2[53] <= MISO_D2;            
                    main_state <= ms_clk15_a;
                end

                ms_clk15_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[17];
                    MOSI_B1 <= MOSI_cmd_B1[17];
                    MOSI_C1 <= MOSI_cmd_C1[17];
                    MOSI_D1 <= MOSI_cmd_D1[17];
                    MOSI_A2 <= MOSI_cmd_A2[17];
                    MOSI_B2 <= MOSI_cmd_B2[17];
                    MOSI_C2 <= MOSI_cmd_C2[17];
                    MOSI_D2 <= MOSI_cmd_D2[17];
                    in4x_A1[54] <= MISO_A1; in4x_A2[54] <= MISO_A2;
                    in4x_B1[54] <= MISO_B1; in4x_B2[54] <= MISO_B2;
                    in4x_C1[54] <= MISO_C1; in4x_C2[54] <= MISO_C2;
                    in4x_D1[54] <= MISO_D1; in4x_D2[54] <= MISO_D2;            
                    main_state <= ms_clk15_b;
                end

                ms_clk15_b: begin
                    in4x_A1[55] <= MISO_A1; in4x_A2[55] <= MISO_A2;
                    in4x_B1[55] <= MISO_B1; in4x_B2[55] <= MISO_B2;
                    in4x_C1[55] <= MISO_C1; in4x_C2[55] <= MISO_C2;
                    in4x_D1[55] <= MISO_D1; in4x_D2[55] <= MISO_D2;            
                    main_state <= ms_clk15_c;
                end

                ms_clk15_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[56] <= MISO_A1; in4x_A2[56] <= MISO_A2;
                    in4x_B1[56] <= MISO_B1; in4x_B2[56] <= MISO_B2;
                    in4x_C1[56] <= MISO_C1; in4x_C2[56] <= MISO_C2;
                    in4x_D1[56] <= MISO_D1; in4x_D2[56] <= MISO_D2;               
                    main_state <= ms_clk15_d;
                end
                
                ms_clk15_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[57] <= MISO_A1; in4x_A2[57] <= MISO_A2;
                    in4x_B1[57] <= MISO_B1; in4x_B2[57] <= MISO_B2;
                    in4x_C1[57] <= MISO_C1; in4x_C2[57] <= MISO_C2;
                    in4x_D1[57] <= MISO_D1; in4x_D2[57] <= MISO_D2;            
                    main_state <= ms_clk16_a;
                end

                ms_clk16_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[16];
                    MOSI_B1 <= MOSI_cmd_B1[16];
                    MOSI_C1 <= MOSI_cmd_C1[16];
                    MOSI_D1 <= MOSI_cmd_D1[16];
                    MOSI_A2 <= MOSI_cmd_A2[16];
                    MOSI_B2 <= MOSI_cmd_B2[16];
                    MOSI_C2 <= MOSI_cmd_C2[16];
                    MOSI_D2 <= MOSI_cmd_D2[16];
                    in4x_A1[58] <= MISO_A1; in4x_A2[58] <= MISO_A2;
                    in4x_B1[58] <= MISO_B1; in4x_B2[58] <= MISO_B2;
                    in4x_C1[58] <= MISO_C1; in4x_C2[58] <= MISO_C2;
                    in4x_D1[58] <= MISO_D1; in4x_D2[58] <= MISO_D2;            
                    main_state <= ms_clk16_b;
                end

                ms_clk16_b: begin
                    in4x_A1[59] <= MISO_A1; in4x_A2[59] <= MISO_A2;
                    in4x_B1[59] <= MISO_B1; in4x_B2[59] <= MISO_B2;
                    in4x_C1[59] <= MISO_C1; in4x_C2[59] <= MISO_C2;
                    in4x_D1[59] <= MISO_D1; in4x_D2[59] <= MISO_D2;            
                    main_state <= ms_clk16_c;
                end

                ms_clk16_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[60] <= MISO_A1; in4x_A2[60] <= MISO_A2;
                    in4x_B1[60] <= MISO_B1; in4x_B2[60] <= MISO_B2;
                    in4x_C1[60] <= MISO_C1; in4x_C2[60] <= MISO_C2;
                    in4x_D1[60] <= MISO_D1; in4x_D2[60] <= MISO_D2;               
                    main_state <= ms_clk16_d;
                end
                
                ms_clk16_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[61] <= MISO_A1; in4x_A2[61] <= MISO_A2;
                    in4x_B1[61] <= MISO_B1; in4x_B2[61] <= MISO_B2;
                    in4x_C1[61] <= MISO_C1; in4x_C2[61] <= MISO_C2;
                    in4x_D1[61] <= MISO_D1; in4x_D2[61] <= MISO_D2;            
                    main_state <= ms_clk17_a;
                end

                ms_clk17_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[15];
                    MOSI_B1 <= MOSI_cmd_B1[15];
                    MOSI_C1 <= MOSI_cmd_C1[15];
                    MOSI_D1 <= MOSI_cmd_D1[15];
                    MOSI_A2 <= MOSI_cmd_A2[15];
                    MOSI_B2 <= MOSI_cmd_B2[15];
                    MOSI_C2 <= MOSI_cmd_C2[15];
                    MOSI_D2 <= MOSI_cmd_D2[15];
                    in4x_A1[62] <= MISO_A1; in4x_A2[62] <= MISO_A2;
                    in4x_B1[62] <= MISO_B1; in4x_B2[62] <= MISO_B2;
                    in4x_C1[62] <= MISO_C1; in4x_C2[62] <= MISO_C2;
                    in4x_D1[62] <= MISO_D1; in4x_D2[62] <= MISO_D2;            
                    main_state <= ms_clk17_b;
                end

                ms_clk17_b: begin
                    in4x_A1[63] <= MISO_A1; in4x_A2[63] <= MISO_A2;
                    in4x_B1[63] <= MISO_B1; in4x_B2[63] <= MISO_B2;
                    in4x_C1[63] <= MISO_C1; in4x_C2[63] <= MISO_C2;
                    in4x_D1[63] <= MISO_D1; in4x_D2[63] <= MISO_D2;            
                    main_state <= ms_clk17_c;
                end

                ms_clk17_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[64] <= MISO_A1; in4x_A2[64] <= MISO_A2;
                    in4x_B1[64] <= MISO_B1; in4x_B2[64] <= MISO_B2;
                    in4x_C1[64] <= MISO_C1; in4x_C2[64] <= MISO_C2;
                    in4x_D1[64] <= MISO_D1; in4x_D2[64] <= MISO_D2;               
                    main_state <= ms_clk17_d;
                end
                
                ms_clk17_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[65] <= MISO_A1; in4x_A2[65] <= MISO_A2;
                    in4x_B1[65] <= MISO_B1; in4x_B2[65] <= MISO_B2;
                    in4x_C1[65] <= MISO_C1; in4x_C2[65] <= MISO_C2;
                    in4x_D1[65] <= MISO_D1; in4x_D2[65] <= MISO_D2;            
                    main_state <= ms_clk18_a;
                end

                ms_clk18_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[14];
                    MOSI_B1 <= MOSI_cmd_B1[14];
                    MOSI_C1 <= MOSI_cmd_C1[14];
                    MOSI_D1 <= MOSI_cmd_D1[14];
                    MOSI_A2 <= MOSI_cmd_A2[14];
                    MOSI_B2 <= MOSI_cmd_B2[14];
                    MOSI_C2 <= MOSI_cmd_C2[14];
                    MOSI_D2 <= MOSI_cmd_D2[14];
                    in4x_A1[66] <= MISO_A1; in4x_A2[66] <= MISO_A2;
                    in4x_B1[66] <= MISO_B1; in4x_B2[66] <= MISO_B2;
                    in4x_C1[66] <= MISO_C1; in4x_C2[66] <= MISO_C2;
                    in4x_D1[66] <= MISO_D1; in4x_D2[66] <= MISO_D2;            
                    main_state <= ms_clk18_b;
                end

                ms_clk18_b: begin
                    in4x_A1[67] <= MISO_A1; in4x_A2[67] <= MISO_A2;
                    in4x_B1[67] <= MISO_B1; in4x_B2[67] <= MISO_B2;
                    in4x_C1[67] <= MISO_C1; in4x_C2[67] <= MISO_C2;
                    in4x_D1[67] <= MISO_D1; in4x_D2[67] <= MISO_D2;            
                    main_state <= ms_clk18_c;
                end

                ms_clk18_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[68] <= MISO_A1; in4x_A2[68] <= MISO_A2;
                    in4x_B1[68] <= MISO_B1; in4x_B2[68] <= MISO_B2;
                    in4x_C1[68] <= MISO_C1; in4x_C2[68] <= MISO_C2;
                    in4x_D1[68] <= MISO_D1; in4x_D2[68] <= MISO_D2;               
                    main_state <= ms_clk18_d;
                end
                
                ms_clk18_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[69] <= MISO_A1; in4x_A2[69] <= MISO_A2;
                    in4x_B1[69] <= MISO_B1; in4x_B2[69] <= MISO_B2;
                    in4x_C1[69] <= MISO_C1; in4x_C2[69] <= MISO_C2;
                    in4x_D1[69] <= MISO_D1; in4x_D2[69] <= MISO_D2;            
                    main_state <= ms_clk19_a;
                end

                ms_clk19_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[13];
                    MOSI_B1 <= MOSI_cmd_B1[13];
                    MOSI_C1 <= MOSI_cmd_C1[13];
                    MOSI_D1 <= MOSI_cmd_D1[13];
                    MOSI_A2 <= MOSI_cmd_A2[13];
                    MOSI_B2 <= MOSI_cmd_B2[13];
                    MOSI_C2 <= MOSI_cmd_C2[13];
                    MOSI_D2 <= MOSI_cmd_D2[13];
                    in4x_A1[70] <= MISO_A1; in4x_A2[70] <= MISO_A2;
                    in4x_B1[70] <= MISO_B1; in4x_B2[70] <= MISO_B2;
                    in4x_C1[70] <= MISO_C1; in4x_C2[70] <= MISO_C2;
                    in4x_D1[70] <= MISO_D1; in4x_D2[70] <= MISO_D2;            
                    main_state <= ms_clk19_b;
                end

                ms_clk19_b: begin
                    in4x_A1[71] <= MISO_A1; in4x_A2[71] <= MISO_A2;
                    in4x_B1[71] <= MISO_B1; in4x_B2[71] <= MISO_B2;
                    in4x_C1[71] <= MISO_C1; in4x_C2[71] <= MISO_C2;
                    in4x_D1[71] <= MISO_D1; in4x_D2[71] <= MISO_D2;            
                    main_state <= ms_clk19_c;
                end

                ms_clk19_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[72] <= MISO_A1; in4x_A2[72] <= MISO_A2;
                    in4x_B1[72] <= MISO_B1; in4x_B2[72] <= MISO_B2;
                    in4x_C1[72] <= MISO_C1; in4x_C2[72] <= MISO_C2;
                    in4x_D1[72] <= MISO_D1; in4x_D2[72] <= MISO_D2;               
                    main_state <= ms_clk19_d;
                end
                
                ms_clk19_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[73] <= MISO_A1; in4x_A2[73] <= MISO_A2;
                    in4x_B1[73] <= MISO_B1; in4x_B2[73] <= MISO_B2;
                    in4x_C1[73] <= MISO_C1; in4x_C2[73] <= MISO_C2;
                    in4x_D1[73] <= MISO_D1; in4x_D2[73] <= MISO_D2;            
                    main_state <= ms_clk20_a;
                end

                ms_clk20_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[12];
                    MOSI_B1 <= MOSI_cmd_B1[12];
                    MOSI_C1 <= MOSI_cmd_C1[12];
                    MOSI_D1 <= MOSI_cmd_D1[12];
                    MOSI_A2 <= MOSI_cmd_A2[12];
                    MOSI_B2 <= MOSI_cmd_B2[12];
                    MOSI_C2 <= MOSI_cmd_C2[12];
                    MOSI_D2 <= MOSI_cmd_D2[12];
                    in4x_A1[74] <= MISO_A1; in4x_A2[74] <= MISO_A2;
                    in4x_B1[74] <= MISO_B1; in4x_B2[74] <= MISO_B2;
                    in4x_C1[74] <= MISO_C1; in4x_C2[74] <= MISO_C2;
                    in4x_D1[74] <= MISO_D1; in4x_D2[74] <= MISO_D2;            
                    main_state <= ms_clk20_b;
                end

                ms_clk20_b: begin
                    in4x_A1[75] <= MISO_A1; in4x_A2[75] <= MISO_A2;
                    in4x_B1[75] <= MISO_B1; in4x_B2[75] <= MISO_B2;
                    in4x_C1[75] <= MISO_C1; in4x_C2[75] <= MISO_C2;
                    in4x_D1[75] <= MISO_D1; in4x_D2[75] <= MISO_D2;            
                    main_state <= ms_clk20_c;
                end

                ms_clk20_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[76] <= MISO_A1; in4x_A2[76] <= MISO_A2;
                    in4x_B1[76] <= MISO_B1; in4x_B2[76] <= MISO_B2;
                    in4x_C1[76] <= MISO_C1; in4x_C2[76] <= MISO_C2;
                    in4x_D1[76] <= MISO_D1; in4x_D2[76] <= MISO_D2;               
                    main_state <= ms_clk20_d;
                end
                
                ms_clk20_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[77] <= MISO_A1; in4x_A2[77] <= MISO_A2;
                    in4x_B1[77] <= MISO_B1; in4x_B2[77] <= MISO_B2;
                    in4x_C1[77] <= MISO_C1; in4x_C2[77] <= MISO_C2;
                    in4x_D1[77] <= MISO_D1; in4x_D2[77] <= MISO_D2;            
                    main_state <= ms_clk21_a;
                end

                ms_clk21_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[11];
                    MOSI_B1 <= MOSI_cmd_B1[11];
                    MOSI_C1 <= MOSI_cmd_C1[11];
                    MOSI_D1 <= MOSI_cmd_D1[11];
                    MOSI_A2 <= MOSI_cmd_A2[11];
                    MOSI_B2 <= MOSI_cmd_B2[11];
                    MOSI_C2 <= MOSI_cmd_C2[11];
                    MOSI_D2 <= MOSI_cmd_D2[11];
                    in4x_A1[78] <= MISO_A1; in4x_A2[78] <= MISO_A2;
                    in4x_B1[78] <= MISO_B1; in4x_B2[78] <= MISO_B2;
                    in4x_C1[78] <= MISO_C1; in4x_C2[78] <= MISO_C2;
                    in4x_D1[78] <= MISO_D1; in4x_D2[78] <= MISO_D2;            
                    main_state <= ms_clk21_b;
                end

                ms_clk21_b: begin
                    in4x_A1[79] <= MISO_A1; in4x_A2[79] <= MISO_A2;
                    in4x_B1[79] <= MISO_B1; in4x_B2[79] <= MISO_B2;
                    in4x_C1[79] <= MISO_C1; in4x_C2[79] <= MISO_C2;
                    in4x_D1[79] <= MISO_D1; in4x_D2[79] <= MISO_D2;            
                    main_state <= ms_clk21_c;
                end

                ms_clk21_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[80] <= MISO_A1; in4x_A2[80] <= MISO_A2;
                    in4x_B1[80] <= MISO_B1; in4x_B2[80] <= MISO_B2;
                    in4x_C1[80] <= MISO_C1; in4x_C2[80] <= MISO_C2;
                    in4x_D1[80] <= MISO_D1; in4x_D2[80] <= MISO_D2;               
                    main_state <= ms_clk21_d;
                end
                
                ms_clk21_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[81] <= MISO_A1; in4x_A2[81] <= MISO_A2;
                    in4x_B1[81] <= MISO_B1; in4x_B2[81] <= MISO_B2;
                    in4x_C1[81] <= MISO_C1; in4x_C2[81] <= MISO_C2;
                    in4x_D1[81] <= MISO_D1; in4x_D2[81] <= MISO_D2;            
                    main_state <= ms_clk22_a;
                end
                
                ms_clk22_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[10];
                    MOSI_B1 <= MOSI_cmd_B1[10];
                    MOSI_C1 <= MOSI_cmd_C1[10];
                    MOSI_D1 <= MOSI_cmd_D1[10];
                    MOSI_A2 <= MOSI_cmd_A2[10];
                    MOSI_B2 <= MOSI_cmd_B2[10];
                    MOSI_C2 <= MOSI_cmd_C2[10];
                    MOSI_D2 <= MOSI_cmd_D2[10];
                    in4x_A1[82] <= MISO_A1; in4x_A2[82] <= MISO_A2;
                    in4x_B1[82] <= MISO_B1; in4x_B2[82] <= MISO_B2;
                    in4x_C1[82] <= MISO_C1; in4x_C2[82] <= MISO_C2;
                    in4x_D1[82] <= MISO_D1; in4x_D2[82] <= MISO_D2;            
                    main_state <= ms_clk22_b;
                end

                ms_clk22_b: begin
                    in4x_A1[83] <= MISO_A1; in4x_A2[83] <= MISO_A2;
                    in4x_B1[83] <= MISO_B1; in4x_B2[83] <= MISO_B2;
                    in4x_C1[83] <= MISO_C1; in4x_C2[83] <= MISO_C2;
                    in4x_D1[83] <= MISO_D1; in4x_D2[83] <= MISO_D2;            
                    main_state <= ms_clk22_c;
                end

                    ms_clk22_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[84] <= MISO_A1; in4x_A2[84] <= MISO_A2;
                    in4x_B1[84] <= MISO_B1; in4x_B2[84] <= MISO_B2;
                    in4x_C1[84] <= MISO_C1; in4x_C2[84] <= MISO_C2;
                    in4x_D1[84] <= MISO_D1; in4x_D2[84] <= MISO_D2;               
                    main_state <= ms_clk22_d;
                end
                
                ms_clk22_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[85] <= MISO_A1; in4x_A2[85] <= MISO_A2;
                    in4x_B1[85] <= MISO_B1; in4x_B2[85] <= MISO_B2;
                    in4x_C1[85] <= MISO_C1; in4x_C2[85] <= MISO_C2;
                    in4x_D1[85] <= MISO_D1; in4x_D2[85] <= MISO_D2;            
                    main_state <= ms_clk23_a;
                end            

                ms_clk23_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[9];
                    MOSI_B1 <= MOSI_cmd_B1[9];
                    MOSI_C1 <= MOSI_cmd_C1[9];
                    MOSI_D1 <= MOSI_cmd_D1[9];
                    MOSI_A2 <= MOSI_cmd_A2[9];
                    MOSI_B2 <= MOSI_cmd_B2[9];
                    MOSI_C2 <= MOSI_cmd_C2[9];
                    MOSI_D2 <= MOSI_cmd_D2[9];
                    in4x_A1[86] <= MISO_A1; in4x_A2[86] <= MISO_A2;
                    in4x_B1[86] <= MISO_B1; in4x_B2[86] <= MISO_B2;
                    in4x_C1[86] <= MISO_C1; in4x_C2[86] <= MISO_C2;
                    in4x_D1[86] <= MISO_D1; in4x_D2[86] <= MISO_D2;            
                    main_state <= ms_clk23_b;
                end

                ms_clk23_b: begin
                    in4x_A1[87] <= MISO_A1; in4x_A2[87] <= MISO_A2;
                    in4x_B1[87] <= MISO_B1; in4x_B2[87] <= MISO_B2;
                    in4x_C1[87] <= MISO_C1; in4x_C2[87] <= MISO_C2;
                    in4x_D1[87] <= MISO_D1; in4x_D2[87] <= MISO_D2;            
                    main_state <= ms_clk23_c;
                end

                ms_clk23_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[88] <= MISO_A1; in4x_A2[88] <= MISO_A2;
                    in4x_B1[88] <= MISO_B1; in4x_B2[88] <= MISO_B2;
                    in4x_C1[88] <= MISO_C1; in4x_C2[88] <= MISO_C2;
                    in4x_D1[88] <= MISO_D1; in4x_D2[88] <= MISO_D2;               
                    main_state <= ms_clk23_d;
                end
                
                ms_clk23_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[89] <= MISO_A1; in4x_A2[89] <= MISO_A2;
                    in4x_B1[89] <= MISO_B1; in4x_B2[89] <= MISO_B2;
                    in4x_C1[89] <= MISO_C1; in4x_C2[89] <= MISO_C2;
                    in4x_D1[89] <= MISO_D1; in4x_D2[89] <= MISO_D2;            
                    main_state <= ms_clk24_a;
                end            
                
                ms_clk24_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[8];
                    MOSI_B1 <= MOSI_cmd_B1[8];
                    MOSI_C1 <= MOSI_cmd_C1[8];
                    MOSI_D1 <= MOSI_cmd_D1[8];
                    MOSI_A2 <= MOSI_cmd_A2[8];
                    MOSI_B2 <= MOSI_cmd_B2[8];
                    MOSI_C2 <= MOSI_cmd_C2[8];
                    MOSI_D2 <= MOSI_cmd_D2[8];
                    in4x_A1[90] <= MISO_A1; in4x_A2[90] <= MISO_A2;
                    in4x_B1[90] <= MISO_B1; in4x_B2[90] <= MISO_B2;
                    in4x_C1[90] <= MISO_C1; in4x_C2[90] <= MISO_C2;
                    in4x_D1[90] <= MISO_D1; in4x_D2[90] <= MISO_D2;            
                    main_state <= ms_clk24_b;
                end

                ms_clk24_b: begin
                    in4x_A1[91] <= MISO_A1; in4x_A2[91] <= MISO_A2;
                    in4x_B1[91] <= MISO_B1; in4x_B2[91] <= MISO_B2;
                    in4x_C1[91] <= MISO_C1; in4x_C2[91] <= MISO_C2;
                    in4x_D1[91] <= MISO_D1; in4x_D2[91] <= MISO_D2;            
                    main_state <= ms_clk24_c;
                end

                ms_clk24_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[92] <= MISO_A1; in4x_A2[92] <= MISO_A2;
                    in4x_B1[92] <= MISO_B1; in4x_B2[92] <= MISO_B2;
                    in4x_C1[92] <= MISO_C1; in4x_C2[92] <= MISO_C2;
                    in4x_D1[92] <= MISO_D1; in4x_D2[92] <= MISO_D2;               
                    main_state <= ms_clk24_d;
                end
                
                ms_clk24_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[93] <= MISO_A1; in4x_A2[93] <= MISO_A2;
                    in4x_B1[93] <= MISO_B1; in4x_B2[93] <= MISO_B2;
                    in4x_C1[93] <= MISO_C1; in4x_C2[93] <= MISO_C2;
                    in4x_D1[93] <= MISO_D1; in4x_D2[93] <= MISO_D2;            
                    main_state <= ms_clk25_a;
                end   
                
                ms_clk25_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[7];
                    MOSI_B1 <= MOSI_cmd_B1[7];
                    MOSI_C1 <= MOSI_cmd_C1[7];
                    MOSI_D1 <= MOSI_cmd_D1[7];
                    MOSI_A2 <= MOSI_cmd_A2[7];
                    MOSI_B2 <= MOSI_cmd_B2[7];
                    MOSI_C2 <= MOSI_cmd_C2[7];
                    MOSI_D2 <= MOSI_cmd_D2[7];
                    in4x_A1[94] <= MISO_A1; in4x_A2[94] <= MISO_A2;
                    in4x_B1[94] <= MISO_B1; in4x_B2[94] <= MISO_B2;
                    in4x_C1[94] <= MISO_C1; in4x_C2[94] <= MISO_C2;
                    in4x_D1[94] <= MISO_D1; in4x_D2[94] <= MISO_D2;            
                    main_state <= ms_clk25_b;
                end

                ms_clk25_b: begin
                    in4x_A1[95] <= MISO_A1; in4x_A2[95] <= MISO_A2;
                    in4x_B1[95] <= MISO_B1; in4x_B2[95] <= MISO_B2;
                    in4x_C1[95] <= MISO_C1; in4x_C2[95] <= MISO_C2;
                    in4x_D1[95] <= MISO_D1; in4x_D2[95] <= MISO_D2;            
                    main_state <= ms_clk25_c;
                end

                ms_clk25_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[96] <= MISO_A1; in4x_A2[96] <= MISO_A2;
                    in4x_B1[96] <= MISO_B1; in4x_B2[96] <= MISO_B2;
                    in4x_C1[96] <= MISO_C1; in4x_C2[96] <= MISO_C2;
                    in4x_D1[96] <= MISO_D1; in4x_D2[96] <= MISO_D2;               
                    main_state <= ms_clk25_d;
                end
                
                ms_clk25_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[97] <= MISO_A1; in4x_A2[97] <= MISO_A2;
                    in4x_B1[97] <= MISO_B1; in4x_B2[97] <= MISO_B2;
                    in4x_C1[97] <= MISO_C1; in4x_C2[97] <= MISO_C2;
                    in4x_D1[97] <= MISO_D1; in4x_D2[97] <= MISO_D2;            
                    main_state <= ms_clk26_a;
                end            
                
                ms_clk26_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[6];
                    MOSI_B1 <= MOSI_cmd_B1[6];
                    MOSI_C1 <= MOSI_cmd_C1[6];
                    MOSI_D1 <= MOSI_cmd_D1[6];
                    MOSI_A2 <= MOSI_cmd_A2[6];
                    MOSI_B2 <= MOSI_cmd_B2[6];
                    MOSI_C2 <= MOSI_cmd_C2[6];
                    MOSI_D2 <= MOSI_cmd_D2[6];
                    in4x_A1[98] <= MISO_A1; in4x_A2[98] <= MISO_A2;
                    in4x_B1[98] <= MISO_B1; in4x_B2[98] <= MISO_B2;
                    in4x_C1[98] <= MISO_C1; in4x_C2[98] <= MISO_C2;
                    in4x_D1[98] <= MISO_D1; in4x_D2[98] <= MISO_D2;            
                    main_state <= ms_clk26_b;
                end

                ms_clk26_b: begin
                    in4x_A1[99] <= MISO_A1; in4x_A2[99] <= MISO_A2;
                    in4x_B1[99] <= MISO_B1; in4x_B2[99] <= MISO_B2;
                    in4x_C1[99] <= MISO_C1; in4x_C2[99] <= MISO_C2;
                    in4x_D1[99] <= MISO_D1; in4x_D2[99] <= MISO_D2;            
                    main_state <= ms_clk26_c;
                end

                ms_clk26_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[100] <= MISO_A1; in4x_A2[100] <= MISO_A2;
                    in4x_B1[100] <= MISO_B1; in4x_B2[100] <= MISO_B2;
                    in4x_C1[100] <= MISO_C1; in4x_C2[100] <= MISO_C2;
                    in4x_D1[100] <= MISO_D1; in4x_D2[100] <= MISO_D2;               
                    main_state <= ms_clk26_d;
                end
                
                ms_clk26_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[101] <= MISO_A1; in4x_A2[101] <= MISO_A2;
                    in4x_B1[101] <= MISO_B1; in4x_B2[101] <= MISO_B2;
                    in4x_C1[101] <= MISO_C1; in4x_C2[101] <= MISO_C2;
                    in4x_D1[101] <= MISO_D1; in4x_D2[101] <= MISO_D2;            
                    main_state <= ms_clk27_a;
                end            
                
                ms_clk27_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[5];
                    MOSI_B1 <= MOSI_cmd_B1[5];
                    MOSI_C1 <= MOSI_cmd_C1[5];
                    MOSI_D1 <= MOSI_cmd_D1[5];
                    MOSI_A2 <= MOSI_cmd_A2[5];
                    MOSI_B2 <= MOSI_cmd_B2[5];
                    MOSI_C2 <= MOSI_cmd_C2[5];
                    MOSI_D2 <= MOSI_cmd_D2[5];
                    in4x_A1[102] <= MISO_A1; in4x_A2[102] <= MISO_A2;
                    in4x_B1[102] <= MISO_B1; in4x_B2[102] <= MISO_B2;
                    in4x_C1[102] <= MISO_C1; in4x_C2[102] <= MISO_C2;
                    in4x_D1[102] <= MISO_D1; in4x_D2[102] <= MISO_D2;            
                    main_state <= ms_clk27_b;
                end

                ms_clk27_b: begin
                    in4x_A1[103] <= MISO_A1; in4x_A2[103] <= MISO_A2;
                    in4x_B1[103] <= MISO_B1; in4x_B2[103] <= MISO_B2;
                    in4x_C1[103] <= MISO_C1; in4x_C2[103] <= MISO_C2;
                    in4x_D1[103] <= MISO_D1; in4x_D2[103] <= MISO_D2;            
                    main_state <= ms_clk27_c;
                end

                ms_clk27_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[104] <= MISO_A1; in4x_A2[104] <= MISO_A2;
                    in4x_B1[104] <= MISO_B1; in4x_B2[104] <= MISO_B2;
                    in4x_C1[104] <= MISO_C1; in4x_C2[104] <= MISO_C2;
                    in4x_D1[104] <= MISO_D1; in4x_D2[104] <= MISO_D2;               
                    main_state <= ms_clk27_d;
                end
                
                ms_clk27_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[105] <= MISO_A1; in4x_A2[105] <= MISO_A2;
                    in4x_B1[105] <= MISO_B1; in4x_B2[105] <= MISO_B2;
                    in4x_C1[105] <= MISO_C1; in4x_C2[105] <= MISO_C2;
                    in4x_D1[105] <= MISO_D1; in4x_D2[105] <= MISO_D2;            
                    main_state <= ms_clk28_a;
                end            

                ms_clk28_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[4];
                    MOSI_B1 <= MOSI_cmd_B1[4];
                    MOSI_C1 <= MOSI_cmd_C1[4];
                    MOSI_D1 <= MOSI_cmd_D1[4];
                    MOSI_A2 <= MOSI_cmd_A2[4];
                    MOSI_B2 <= MOSI_cmd_B2[4];
                    MOSI_C2 <= MOSI_cmd_C2[4];
                    MOSI_D2 <= MOSI_cmd_D2[4];
                    in4x_A1[106] <= MISO_A1; in4x_A2[106] <= MISO_A2;
                    in4x_B1[106] <= MISO_B1; in4x_B2[106] <= MISO_B2;
                    in4x_C1[106] <= MISO_C1; in4x_C2[106] <= MISO_C2;
                    in4x_D1[106] <= MISO_D1; in4x_D2[106] <= MISO_D2;            
                    main_state <= ms_clk28_b;
                end

                ms_clk28_b: begin
                    in4x_A1[107] <= MISO_A1; in4x_A2[107] <= MISO_A2;
                    in4x_B1[107] <= MISO_B1; in4x_B2[107] <= MISO_B2;
                    in4x_C1[107] <= MISO_C1; in4x_C2[107] <= MISO_C2;
                    in4x_D1[107] <= MISO_D1; in4x_D2[107] <= MISO_D2;            
                    main_state <= ms_clk28_c;
                end

                ms_clk28_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[108] <= MISO_A1; in4x_A2[108] <= MISO_A2;
                    in4x_B1[108] <= MISO_B1; in4x_B2[108] <= MISO_B2;
                    in4x_C1[108] <= MISO_C1; in4x_C2[108] <= MISO_C2;
                    in4x_D1[108] <= MISO_D1; in4x_D2[108] <= MISO_D2;               
                    main_state <= ms_clk28_d;
                end
                
                ms_clk28_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[109] <= MISO_A1; in4x_A2[109] <= MISO_A2;
                    in4x_B1[109] <= MISO_B1; in4x_B2[109] <= MISO_B2;
                    in4x_C1[109] <= MISO_C1; in4x_C2[109] <= MISO_C2;
                    in4x_D1[109] <= MISO_D1; in4x_D2[109] <= MISO_D2;            
                    main_state <= ms_clk29_a;
                end            

                ms_clk29_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[3];
                    MOSI_B1 <= MOSI_cmd_B1[3];
                    MOSI_C1 <= MOSI_cmd_C1[3];
                    MOSI_D1 <= MOSI_cmd_D1[3];
                    MOSI_A2 <= MOSI_cmd_A2[3];
                    MOSI_B2 <= MOSI_cmd_B2[3];
                    MOSI_C2 <= MOSI_cmd_C2[3];
                    MOSI_D2 <= MOSI_cmd_D2[3];
                    in4x_A1[110] <= MISO_A1; in4x_A2[110] <= MISO_A2;
                    in4x_B1[110] <= MISO_B1; in4x_B2[110] <= MISO_B2;
                    in4x_C1[110] <= MISO_C1; in4x_C2[110] <= MISO_C2;
                    in4x_D1[110] <= MISO_D1; in4x_D2[110] <= MISO_D2;            
                    main_state <= ms_clk29_b;
                end

                ms_clk29_b: begin
                    in4x_A1[111] <= MISO_A1; in4x_A2[111] <= MISO_A2;
                    in4x_B1[111] <= MISO_B1; in4x_B2[111] <= MISO_B2;
                    in4x_C1[111] <= MISO_C1; in4x_C2[111] <= MISO_C2;
                    in4x_D1[111] <= MISO_D1; in4x_D2[111] <= MISO_D2;            
                    main_state <= ms_clk29_c;
                end

                ms_clk29_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[112] <= MISO_A1; in4x_A2[112] <= MISO_A2;
                    in4x_B1[112] <= MISO_B1; in4x_B2[112] <= MISO_B2;
                    in4x_C1[112] <= MISO_C1; in4x_C2[112] <= MISO_C2;
                    in4x_D1[112] <= MISO_D1; in4x_D2[112] <= MISO_D2;               
                    main_state <= ms_clk29_d;
                end
                
                ms_clk29_d: begin
                    SCLK <= 1'b1;
                    in4x_A1[113] <= MISO_A1; in4x_A2[113] <= MISO_A2;
                    in4x_B1[113] <= MISO_B1; in4x_B2[113] <= MISO_B2;
                    in4x_C1[113] <= MISO_C1; in4x_C2[113] <= MISO_C2;
                    in4x_D1[113] <= MISO_D1; in4x_D2[113] <= MISO_D2;            
                    main_state <= ms_clk30_a;
                end            

                ms_clk30_a: begin
                    MOSI_A1 <= MOSI_cmd_A1[2];
                    MOSI_B1 <= MOSI_cmd_B1[2];
                    MOSI_C1 <= MOSI_cmd_C1[2];
                    MOSI_D1 <= MOSI_cmd_D1[2];
                    MOSI_A2 <= MOSI_cmd_A2[2];
                    MOSI_B2 <= MOSI_cmd_B2[2];
                    MOSI_C2 <= MOSI_cmd_C2[2];
                    MOSI_D2 <= MOSI_cmd_D2[2];
                    in4x_A1[114] <= MISO_A1; in4x_A2[114] <= MISO_A2;
                    in4x_B1[114] <= MISO_B1; in4x_B2[114] <= MISO_B2;
                    in4x_C1[114] <= MISO_C1; in4x_C2[114] <= MISO_C2;
                    in4x_D1[114] <= MISO_D1; in4x_D2[114] <= MISO_D2;            
                    main_state <= ms_clk30_b;
                end

                ms_clk30_b: begin
                    in4x_A1[115] <= MISO_A1; in4x_A2[115] <= MISO_A2;
                    in4x_B1[115] <= MISO_B1; in4x_B2[115] <= MISO_B2;
                    in4x_C1[115] <= MISO_C1; in4x_C2[115] <= MISO_C2;
                    in4x_D1[115] <= MISO_D1; in4x_D2[115] <= MISO_D2;            
                    main_state <= ms_clk30_c;
                end

                ms_clk30_c: begin
                    SCLK <= 1'b1;
                    in4x_A1[116] <= MISO_A1; in4x_A2[116] <= MISO_A2;
                    in4x_B1[116] <= MISO_B1; in4x_B2[116] <= MISO_B2;
                    in4x_C1[116] <= MISO_C1; in4x_C2[116] <= MISO_C2;
                    in4x_D1[116] <= MISO_D1; in4x_D2[116] <= MISO_D2;               
                    main_state <= ms_clk30_d;
                end
                
                ms_clk30_d: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_1;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[117] <= MISO_A1; in4x_A2[117] <= MISO_A2;
                    in4x_B1[117] <= MISO_B1; in4x_B2[117] <= MISO_B2;
                    in4x_C1[117] <= MISO_C1; in4x_C2[117] <= MISO_C2;
                    in4x_D1[117] <= MISO_D1; in4x_D2[117] <= MISO_D2;            
                    main_state <= ms_clk31_a;
                end

                ms_clk31_a: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_2;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[1];
                    MOSI_B1 <= MOSI_cmd_B1[1];
                    MOSI_C1 <= MOSI_cmd_C1[1];
                    MOSI_D1 <= MOSI_cmd_D1[1];
                    MOSI_A2 <= MOSI_cmd_A2[1];
                    MOSI_B2 <= MOSI_cmd_B2[1];
                    MOSI_C2 <= MOSI_cmd_C2[1];
                    MOSI_D2 <= MOSI_cmd_D2[1];
                    in4x_A1[118] <= MISO_A1; in4x_A2[118] <= MISO_A2;
                    in4x_B1[118] <= MISO_B1; in4x_B2[118] <= MISO_B2;
                    in4x_C1[118] <= MISO_C1; in4x_C2[118] <= MISO_C2;
                    in4x_D1[118] <= MISO_D1; in4x_D2[118] <= MISO_D2;            
                    main_state <= ms_clk31_b;
                end

                ms_clk31_b: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_3;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[119] <= MISO_A1; in4x_A2[119] <= MISO_A2;
                    in4x_B1[119] <= MISO_B1; in4x_B2[119] <= MISO_B2;
                    in4x_C1[119] <= MISO_C1; in4x_C2[119] <= MISO_C2;
                    in4x_D1[119] <= MISO_D1; in4x_D2[119] <= MISO_D2;            
                    main_state <= ms_clk31_c;
                end

                ms_clk31_c: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_4;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[120] <= MISO_A1; in4x_A2[120] <= MISO_A2;
                    in4x_B1[120] <= MISO_B1; in4x_B2[120] <= MISO_B2;
                    in4x_C1[120] <= MISO_C1; in4x_C2[120] <= MISO_C2;
                    in4x_D1[120] <= MISO_D1; in4x_D2[120] <= MISO_D2;               
                    main_state <= ms_clk31_d;
                end
                
                ms_clk31_d: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_5;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    SCLK <= 1'b1;
                    in4x_A1[121] <= MISO_A1; in4x_A2[121] <= MISO_A2;
                    in4x_B1[121] <= MISO_B1; in4x_B2[121] <= MISO_B2;
                    in4x_C1[121] <= MISO_C1; in4x_C2[121] <= MISO_C2;
                    in4x_D1[121] <= MISO_D1; in4x_D2[121] <= MISO_D2;            
                    main_state <= ms_clk32_a;
                end

                ms_clk32_a: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_6;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    MOSI_A1 <= MOSI_cmd_A1[0];
                    MOSI_B1 <= MOSI_cmd_B1[0];
                    MOSI_C1 <= MOSI_cmd_C1[0];
                    MOSI_D1 <= MOSI_cmd_D1[0];
                    MOSI_A2 <= MOSI_cmd_A2[0];
                    MOSI_B2 <= MOSI_cmd_B2[0];
                    MOSI_C2 <= MOSI_cmd_C2[0];
                    MOSI_D2 <= MOSI_cmd_D2[0];
                    in4x_A1[122] <= MISO_A1; in4x_A2[122] <= MISO_A2;
                    in4x_B1[122] <= MISO_B1; in4x_B2[122] <= MISO_B2;
                    in4x_C1[122] <= MISO_C1; in4x_C2[122] <= MISO_C2;
                    in4x_D1[122] <= MISO_D1; in4x_D2[122] <= MISO_D2;            
                    main_state <= ms_clk32_b;
                end

                ms_clk32_b: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_7;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                    end
                    in4x_A1[123] <= MISO_A1; in4x_A2[123] <= MISO_A2;
                    in4x_B1[123] <= MISO_B1; in4x_B2[123] <= MISO_B2;
                    in4x_C1[123] <= MISO_C1; in4x_C2[123] <= MISO_C2;
                    in4x_D1[123] <= MISO_D1; in4x_D2[123] <= MISO_D2;            
                    main_state <= ms_clk32_c;
                end

                ms_clk32_c: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= DAC_output_register_8;   // Write DAC output values
                        ACQ_FSM_data_valid <= 1'b1;
                        ADC_triggers[0] <= (data_stream_ADC_1 > ADC_threshold);
                        ADC_triggers[1] <= (data_stream_ADC_2 > ADC_threshold);
                        ADC_triggers[2] <= (data_stream_ADC_3 > ADC_threshold);
                        ADC_triggers[3] <= (data_stream_ADC_4 > ADC_threshold);
                        ADC_triggers[4] <= (data_stream_ADC_5 > ADC_threshold);
                        ADC_triggers[5] <= (data_stream_ADC_6 > ADC_threshold);
                        ADC_triggers[6] <= (data_stream_ADC_7 > ADC_threshold);
                        ADC_triggers[7] <= (data_stream_ADC_8 > ADC_threshold);                  
                    end
                    SCLK <= 1'b1;
                    in4x_A1[124] <= MISO_A1; in4x_A2[124] <= MISO_A2;
                    in4x_B1[124] <= MISO_B1; in4x_B2[124] <= MISO_B2;
                    in4x_C1[124] <= MISO_C1; in4x_C2[124] <= MISO_C2;
                    in4x_D1[124] <= MISO_D1; in4x_D2[124] <= MISO_D2;            
                    main_state <= ms_clk32_d;
                end

                ms_clk32_d: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_1;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               
 
                    SCLK <= 1'b1;
                    in4x_A1[125] <= MISO_A1; in4x_A2[125] <= MISO_A2;
                    in4x_B1[125] <= MISO_B1; in4x_B2[125] <= MISO_B2;
                    in4x_C1[125] <= MISO_C1; in4x_C2[125] <= MISO_C2;
                    in4x_D1[125] <= MISO_D1; in4x_D2[125] <= MISO_D2;            
                    main_state <= ms_clk33_a;
                end

                ms_clk33_a: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_2;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    MOSI_A1 <= 1'b0;
                    MOSI_B1 <= 1'b0;
                    MOSI_C1 <= 1'b0;
                    MOSI_D1 <= 1'b0;
                    MOSI_A2 <= 1'b0;
                    MOSI_B2 <= 1'b0;
                    MOSI_C2 <= 1'b0;
                    MOSI_D2 <= 1'b0;
                    in4x_A1[126] <= MISO_A1; in4x_A2[126] <= MISO_A2;
                    in4x_B1[126] <= MISO_B1; in4x_B2[126] <= MISO_B2;
                    in4x_C1[126] <= MISO_C1; in4x_C2[126] <= MISO_C2;
                    in4x_D1[126] <= MISO_D1; in4x_D2[126] <= MISO_D2;            
                    main_state <= ms_clk33_b;
                end
                
                ms_clk33_b: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_3;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    in4x_A1[127] <= MISO_A1; in4x_A2[127] <= MISO_A2;
                    in4x_B1[127] <= MISO_B1; in4x_B2[127] <= MISO_B2;
                    in4x_C1[127] <= MISO_C1; in4x_C2[127] <= MISO_C2;
                    in4x_D1[127] <= MISO_D1; in4x_D2[127] <= MISO_D2;            
                    main_state <= ms_cs_a;
                end
            
                ms_cs_a: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_4;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[128] <= MISO_A1; in4x_A2[128] <= MISO_A2;
                    in4x_B1[128] <= MISO_B1; in4x_B2[128] <= MISO_B2;
                    in4x_C1[128] <= MISO_C1; in4x_C2[128] <= MISO_C2;
                    in4x_D1[128] <= MISO_D1; in4x_D2[128] <= MISO_D2;            
                    main_state <= ms_cs_b;
                end
                
                ms_cs_b: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_5;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[129] <= MISO_A1; in4x_A2[129] <= MISO_A2;
                    in4x_B1[129] <= MISO_B1; in4x_B2[129] <= MISO_B2;
                    in4x_C1[129] <= MISO_C1; in4x_C2[129] <= MISO_C2;
                    in4x_D1[129] <= MISO_D1; in4x_D2[129] <= MISO_D2;            
                    main_state <= ms_cs_c;
                end
                
                ms_cs_c: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_6;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[130] <= MISO_A1; in4x_A2[130] <= MISO_A2;
                    in4x_B1[130] <= MISO_B1; in4x_B2[130] <= MISO_B2;
                    in4x_C1[130] <= MISO_C1; in4x_C2[130] <= MISO_C2;
                    in4x_D1[130] <= MISO_D1; in4x_D2[130] <= MISO_D2;            
                    main_state <= ms_cs_d;
                end
                
                ms_cs_d: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_7;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[131] <= MISO_A1; in4x_A2[131] <= MISO_A2;
                    in4x_B1[131] <= MISO_B1; in4x_B2[131] <= MISO_B2;
                    in4x_C1[131] <= MISO_C1; in4x_C2[131] <= MISO_C2;
                    in4x_D1[131] <= MISO_D1; in4x_D2[131] <= MISO_D2;            
                    main_state <= ms_cs_e;
                end
                
                ms_cs_e: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_ADC_8;   // Write evaluation-board ADC samples
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[132] <= MISO_A1; in4x_A2[132] <= MISO_A2;
                    in4x_B1[132] <= MISO_B1; in4x_B2[132] <= MISO_B2;
                    in4x_C1[132] <= MISO_C1; in4x_C2[132] <= MISO_C2;
                    in4x_D1[132] <= MISO_D1; in4x_D2[132] <= MISO_D2;            
                    main_state <= ms_cs_f;
                end
                
                ms_cs_f: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_TTL_in;   // Write TTL inputs
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    in4x_A1[133] <= MISO_A1; in4x_A2[133] <= MISO_A2;
                    in4x_B1[133] <= MISO_B1; in4x_B2[133] <= MISO_B2;
                    in4x_C1[133] <= MISO_C1; in4x_C2[133] <= MISO_C2;
                    in4x_D1[133] <= MISO_D1; in4x_D2[133] <= MISO_D2;            
                    main_state <= ms_cs_g;
                end
                
                ms_cs_g: begin
                    if (channel == 19 && ~shutdown) begin
                        ACQ_FSM_data_out <= data_stream_TTL_out;   // Write current value of TTL outputs so users can reconstruct exact timings
                        ACQ_FSM_data_valid <= 1'b1;
                    end               

                    CS_b <= 1'b1;
                    result_A1 <= in_A1; result_A2 <= in_A2;
                    result_B1 <= in_B1; result_B2 <= in_B2;
                    result_C1 <= in_C1; result_C2 <= in_C2;
                    result_D1 <= in_D1; result_D2 <= in_D2;
                    main_state <= ms_cs_h;
                end
            
                ms_cs_h: begin
                    if (~shutdown) begin
                        if (channel == 19) begin
                            if (aux_cmd_index_1 == max_aux_cmd_index_1) begin
                                aux_cmd_index_1 <= loop_aux_cmd_index_1;
                                max_aux_cmd_index_1 <= max_aux_cmd_index_1_in;
                            end else begin
                                aux_cmd_index_1 <= aux_cmd_index_1 + 1;
                            end
                            if (aux_cmd_index_2 == max_aux_cmd_index_2) begin
                                aux_cmd_index_2 <= loop_aux_cmd_index_2;
                                max_aux_cmd_index_2 <= max_aux_cmd_index_2_in;
                            end else begin
                                aux_cmd_index_2 <= aux_cmd_index_2 + 1;
                            end
                            if (aux_cmd_index_3 == max_aux_cmd_index_3) begin
                                aux_cmd_index_3 <= loop_aux_cmd_index_3;
                                max_aux_cmd_index_3 <= max_aux_cmd_index_3_in;
                            end else begin
                                aux_cmd_index_3 <= aux_cmd_index_3 + 1;
                            end
                            if (aux_cmd_index_4 == max_aux_cmd_index_4) begin
                                aux_cmd_index_4 <= loop_aux_cmd_index_4;
                                max_aux_cmd_index_4 <= max_aux_cmd_index_4_in;
                            end else begin
                                aux_cmd_index_4 <= aux_cmd_index_4 + 1;
                            end
                        end
                    end
                   
                    // Route selected samples to DAC outputs
                    if ((channel_MISO == DAC_channel_sel_1) && ~shutdown) begin
                        case (DAC_stream_sel_1)
                            0: DAC_pre_register_1 <= data_stream_1[31:16];
                            1: DAC_pre_register_1 <= data_stream_2[31:16];
                            2: DAC_pre_register_1 <= data_stream_3[31:16];
                            3: DAC_pre_register_1 <= data_stream_4[31:16];
                            4: DAC_pre_register_1 <= data_stream_5[31:16];
                            5: DAC_pre_register_1 <= data_stream_6[31:16];
                            6: DAC_pre_register_1 <= data_stream_7[31:16];
                            7: DAC_pre_register_1 <= data_stream_8[31:16];
                            8: DAC_pre_register_1 <= DAC_manual;
                            default: DAC_pre_register_1 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_2) && ~shutdown) begin
                        case (DAC_stream_sel_2)
                            0: DAC_pre_register_2 <= data_stream_1[31:16];
                            1: DAC_pre_register_2 <= data_stream_2[31:16];
                            2: DAC_pre_register_2 <= data_stream_3[31:16];
                            3: DAC_pre_register_2 <= data_stream_4[31:16];
                            4: DAC_pre_register_2 <= data_stream_5[31:16];
                            5: DAC_pre_register_2 <= data_stream_6[31:16];
                            6: DAC_pre_register_2 <= data_stream_7[31:16];
                            7: DAC_pre_register_2 <= data_stream_8[31:16];
                            8: DAC_pre_register_2 <= DAC_manual;
                            default: DAC_pre_register_2 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_3) && ~shutdown) begin
                        case (DAC_stream_sel_3)
                            0: DAC_pre_register_3 <= data_stream_1[31:16];
                            1: DAC_pre_register_3 <= data_stream_2[31:16];
                            2: DAC_pre_register_3 <= data_stream_3[31:16];
                            3: DAC_pre_register_3 <= data_stream_4[31:16];
                            4: DAC_pre_register_3 <= data_stream_5[31:16];
                            5: DAC_pre_register_3 <= data_stream_6[31:16];
                            6: DAC_pre_register_3 <= data_stream_7[31:16];
                            7: DAC_pre_register_3 <= data_stream_8[31:16];
                            8: DAC_pre_register_3 <= DAC_manual;
                            default: DAC_pre_register_3 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_4) && ~shutdown) begin
                        case (DAC_stream_sel_4)
                            0: DAC_pre_register_4 <= data_stream_1[31:16];
                            1: DAC_pre_register_4 <= data_stream_2[31:16];
                            2: DAC_pre_register_4 <= data_stream_3[31:16];
                            3: DAC_pre_register_4 <= data_stream_4[31:16];
                            4: DAC_pre_register_4 <= data_stream_5[31:16];
                            5: DAC_pre_register_4 <= data_stream_6[31:16];
                            6: DAC_pre_register_4 <= data_stream_7[31:16];
                            7: DAC_pre_register_4 <= data_stream_8[31:16];
                            8: DAC_pre_register_4 <= DAC_manual;
                            default: DAC_pre_register_4 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_5) && ~shutdown) begin
                        case (DAC_stream_sel_5)
                            0: DAC_pre_register_5 <= data_stream_1[31:16];
                            1: DAC_pre_register_5 <= data_stream_2[31:16];
                            2: DAC_pre_register_5 <= data_stream_3[31:16];
                            3: DAC_pre_register_5 <= data_stream_4[31:16];
                            4: DAC_pre_register_5 <= data_stream_5[31:16];
                            5: DAC_pre_register_5 <= data_stream_6[31:16];
                            6: DAC_pre_register_5 <= data_stream_7[31:16];
                            7: DAC_pre_register_5 <= data_stream_8[31:16];
                            8: DAC_pre_register_5 <= DAC_manual;
                            default: DAC_pre_register_5 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_6) && ~shutdown) begin
                        case (DAC_stream_sel_6)
                            0: DAC_pre_register_6 <= data_stream_1[31:16];
                            1: DAC_pre_register_6 <= data_stream_2[31:16];
                            2: DAC_pre_register_6 <= data_stream_3[31:16];
                            3: DAC_pre_register_6 <= data_stream_4[31:16];
                            4: DAC_pre_register_6 <= data_stream_5[31:16];
                            5: DAC_pre_register_6 <= data_stream_6[31:16];
                            6: DAC_pre_register_6 <= data_stream_7[31:16];
                            7: DAC_pre_register_6 <= data_stream_8[31:16];
                            8: DAC_pre_register_6 <= DAC_manual;
                            default: DAC_pre_register_6 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_7) && ~shutdown) begin
                        case (DAC_stream_sel_7)
                            0: DAC_pre_register_7 <= data_stream_1[31:16];
                            1: DAC_pre_register_7 <= data_stream_2[31:16];
                            2: DAC_pre_register_7 <= data_stream_3[31:16];
                            3: DAC_pre_register_7 <= data_stream_4[31:16];
                            4: DAC_pre_register_7 <= data_stream_5[31:16];
                            5: DAC_pre_register_7 <= data_stream_6[31:16];
                            6: DAC_pre_register_7 <= data_stream_7[31:16];
                            7: DAC_pre_register_7 <= data_stream_8[31:16];
                            8: DAC_pre_register_7 <= DAC_manual;
                            default: DAC_pre_register_7 <= 16'b0;
                        endcase
                    end
                    if ((channel_MISO == DAC_channel_sel_8) && ~shutdown) begin
                        case (DAC_stream_sel_8)
                            0: DAC_pre_register_8 <= data_stream_1[31:16];
                            1: DAC_pre_register_8 <= data_stream_2[31:16];
                            2: DAC_pre_register_8 <= data_stream_3[31:16];
                            3: DAC_pre_register_8 <= data_stream_4[31:16];
                            4: DAC_pre_register_8 <= data_stream_5[31:16];
                            5: DAC_pre_register_8 <= data_stream_6[31:16];
                            6: DAC_pre_register_8 <= data_stream_7[31:16];
                            7: DAC_pre_register_8 <= data_stream_8[31:16];
                            8: DAC_pre_register_8 <= DAC_manual;
                            default: DAC_pre_register_8 <= 16'b0;
                        endcase
                    end
                   
                    // Route selected sample to DAC software re-reference input
                    if (channel_MISO == DAC_reref_channel_sel && ~shutdown) begin
                        case (DAC_reref_stream_sel)
                            0: DAC_reref_pre_register <= data_stream_1;
                            1: DAC_reref_pre_register <= data_stream_2;
                            2: DAC_reref_pre_register <= data_stream_3;
                            3: DAC_reref_pre_register <= data_stream_4;
                            4: DAC_reref_pre_register <= data_stream_5;
                            5: DAC_reref_pre_register <= data_stream_6;
                            6: DAC_reref_pre_register <= data_stream_7;
                            7: DAC_reref_pre_register <= data_stream_8;
                        endcase
                    end
                   
                    if (channel == 0) begin
                        timestamp <= timestamp + 1;
                    end
                    CS_b <= 1'b1;         
                    main_state <= ms_cs_i;
                end
            
                ms_cs_i: begin
                    if (channel == 19) begin
                        channel <= 0;
                    end else begin
                        channel <= channel + 1;
                    end
                    if (channel_MISO == 19) begin
                        channel_MISO <= 0;
                    end else begin
                        channel_MISO <= channel_MISO + 1;
                    end
                    CS_b <= 1'b1;   
                   
                    if (channel == 19) begin
                        if (SPI_run_continuous) begin      // run continuously if SPI_run_continuous == 1
                            main_state <= ms_cs_j;
                        end else begin
                            if (shutdown) begin
                                shutdown <= 1'b0;
                                main_state <= ms_wait;
                            end else if (max_timestep == 32'b0) begin // stop with shutdown if max_timestep == 0
                                shutdown <= 1'b1;
                                main_state <= ms_cs_j;
                            end else if (timestamp == max_timestep) begin  // stop without shutdown if max_timestep reached
                                main_state <= ms_wait;
                            end else begin
                                main_state <= ms_cs_j;
                            end
                        end
                    end else begin
                        main_state <= ms_cs_j;
                    end
                end
                        
                default: begin
                    main_state <= ms_wait;
                end
            
            endcase
        end
    end



/*
 ######  ##    ## ######## ######  ##    ## ########     ####### #######  ######  ##    ## ####### ###    ##  ###### ####### ######  #######
##    ## ##    ##    ##    ##   ## ##    ##    ##        ##      ##      ##    ## ##    ## ##      ####   ## ##      ##      ##   ## ##
##    ## ##    ##    ##    ######  ##    ##    ##        ####### #####   ##    ## ##    ## #####   ## ##  ## ##      #####   ######  #######
##    ## ##    ##    ##    ##      ##    ##    ##             ## ##      ## ?????? ## ##    ## ##      ##  ## ## ##      ##      ##   ##      ##
 ######   ######     ##    ##       ######     ##        ####### #######  ######   ######  ####### ##   ####  ###### ####### ##   ## #######
                                                                             ??????
*/



   
    // Stimulation sequencers, digital output sequencer
    wire [31:0] triggers;
    assign triggers = { manual_triggers[7:0], ADC_triggers, TTL_in };
   
    stim_sequencer #(0) stim_sequencer_A1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_A1), .stim_pol(stim_pol_A1), .amp_settle(amp_settle_A1_pre), .charge_recov(charge_recov_A1),
        .amp_settle_changed(amp_settle_changed_A1), .reset_sequencer(reset_sequencers));

    stim_sequencer #(1) stim_sequencer_A2 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_A2), .stim_pol(stim_pol_A2), .amp_settle(amp_settle_A2_pre), .charge_recov(charge_recov_A2),
        .amp_settle_changed(amp_settle_changed_A2), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(2) stim_sequencer_B1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_B1), .stim_pol(stim_pol_B1), .amp_settle(amp_settle_B1_pre), .charge_recov(charge_recov_B1),
        .amp_settle_changed(amp_settle_changed_B1), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(3) stim_sequencer_B2 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_B2), .stim_pol(stim_pol_B2), .amp_settle(amp_settle_B2_pre), .charge_recov(charge_recov_B2),
        .amp_settle_changed(amp_settle_changed_B2), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(4) stim_sequencer_C1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_C1), .stim_pol(stim_pol_C1), .amp_settle(amp_settle_C1_pre), .charge_recov(charge_recov_C1),
        .amp_settle_changed(amp_settle_changed_C1), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(5) stim_sequencer_C2 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_C2), .stim_pol(stim_pol_C2), .amp_settle(amp_settle_C2_pre), .charge_recov(charge_recov_C2),
        .amp_settle_changed(amp_settle_changed_C2), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(6) stim_sequencer_D1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_D1), .stim_pol(stim_pol_D1), .amp_settle(amp_settle_D1_pre), .charge_recov(charge_recov_D1),
        .amp_settle_changed(amp_settle_changed_D1), .reset_sequencer(reset_sequencers));
      
    stim_sequencer #(7) stim_sequencer_D2 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .stim_on(stim_on_D2), .stim_pol(stim_pol_D2), .amp_settle(amp_settle_D2_pre), .charge_recov(charge_recov_D2),
        .amp_settle_changed(amp_settle_changed_D2), .reset_sequencer(reset_sequencers));
      
    wire [15:0] DAC_sequencer_1, DAC_sequencer_2, DAC_sequencer_3, DAC_sequencer_4;
    wire [15:0] DAC_sequencer_5, DAC_sequencer_6, DAC_sequencer_7, DAC_sequencer_8;
    wire DAC_sequencer_en_1, DAC_sequencer_en_2, DAC_sequencer_en_3, DAC_sequencer_en_4;
    wire DAC_sequencer_en_5, DAC_sequencer_en_6, DAC_sequencer_en_7, DAC_sequencer_en_8;
   
    analog_out_sequencer #(8) analog_out_sequencer_1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_1), .DAC_out(DAC_sequencer_1), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));
      
    analog_out_sequencer #(9) analog_out_sequencer_2 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_2), .DAC_out(DAC_sequencer_2), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(10) analog_out_sequencer_3 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_3), .DAC_out(DAC_sequencer_3), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(11) analog_out_sequencer_4 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_4), .DAC_out(DAC_sequencer_4), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(12) analog_out_sequencer_5 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_5), .DAC_out(DAC_sequencer_5), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(13) analog_out_sequencer_6 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_6), .DAC_out(DAC_sequencer_6), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(14) analog_out_sequencer_7 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_7), .DAC_out(DAC_sequencer_7), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

    analog_out_sequencer #(15) analog_out_sequencer_8 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .DAC_sequencer_en(DAC_sequencer_en_8), .DAC_out(DAC_sequencer_8), .shutdown(shutdown), 
        .reset_sequencer(reset_sequencers));

   
    wire [15:0] digout_sequencer, digout_sequencer_enabled, TTL_out_DAC_thresh;
   
    digout_sequencer #(16) digout_sequencer_1 (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .prog_channel(prog_channel), .prog_address(prog_address), .prog_module(prog_module), .prog_word(prog_word), .prog_trig(prog_trig),
        .triggers(triggers), .digout(digout_sequencer), .digout_enabled(digout_sequencer_enabled), .shutdown(shutdown),
        .reset_sequencer(reset_sequencers));
   
    assign TTL_out_DAC_thresh = { 8'b00000000, DAC_thresh_out };
   
    assign TTL_out_direct[0] = TTL_out_mode[0] ? TTL_out_DAC_thresh[0] : digout_sequencer[0];
    assign TTL_out_direct[1] = TTL_out_mode[1] ? TTL_out_DAC_thresh[1] : digout_sequencer[1];
    assign TTL_out_direct[2] = TTL_out_mode[2] ? TTL_out_DAC_thresh[2] : digout_sequencer[2];
    assign TTL_out_direct[3] = TTL_out_mode[3] ? TTL_out_DAC_thresh[3] : digout_sequencer[3];
    assign TTL_out_direct[4] = TTL_out_mode[4] ? TTL_out_DAC_thresh[4] : digout_sequencer[4];
    assign TTL_out_direct[5] = TTL_out_mode[5] ? TTL_out_DAC_thresh[5] : digout_sequencer[5];
    assign TTL_out_direct[6] = TTL_out_mode[6] ? TTL_out_DAC_thresh[6] : digout_sequencer[6];
    assign TTL_out_direct[7] = TTL_out_mode[7] ? TTL_out_DAC_thresh[7] : digout_sequencer[7];
    assign TTL_out_direct[15:8] = digout_sequencer[15:8];
   
    // assign TTL_out_direct = { CS_b_A, SCLK_A, MOSI1_A, MISO1_A, digout_sequencer[11:1], ACQ_FSM_data_valid };
    // assign TTL_out_direct = { DAC_SYNC, DAC_SCLK, DAC_DIN_1, DAC_DIN_2, digout_sequencer[11:0] };
    //   assign TTL_out_direct = { stim_on_A1[0], stim_pol_A1[0], amp_settle_A1[0], charge_recov_A1[0], 
    //                             stim_on_A1[1], stim_pol_A1[1], amp_settle_A1[1], charge_recov_A1[1], ADC_triggers[3:0], manual_triggers[3:0] }; 
   
    wire any_amp_settle_A, any_amp_settle_B, any_amp_settle_C, any_amp_settle_D;
    wire any_amp_settle;
   
    assign any_amp_settle_A = (amp_settle_A1_pre != 16'b0) || (amp_settle_A2_pre != 16'b0);
    assign any_amp_settle_B = (amp_settle_B1_pre != 16'b0) || (amp_settle_B2_pre != 16'b0);
    assign any_amp_settle_C = (amp_settle_C1_pre != 16'b0) || (amp_settle_C2_pre != 16'b0);
    assign any_amp_settle_D = (amp_settle_D1_pre != 16'b0) || (amp_settle_D2_pre != 16'b0);

    assign any_amp_settle = any_amp_settle_A || any_amp_settle_B || any_amp_settle_C || any_amp_settle_D;
   
    assign amp_settle_A1 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_A ? {16{any_amp_settle_A}} : amp_settle_A1_pre);
    assign amp_settle_A2 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_A ? {16{any_amp_settle_A}} : amp_settle_A2_pre);
    assign amp_settle_B1 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_B ? {16{any_amp_settle_B}} : amp_settle_B1_pre);
    assign amp_settle_B2 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_B ? {16{any_amp_settle_B}} : amp_settle_B2_pre);
    assign amp_settle_C1 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_C ? {16{any_amp_settle_C}} : amp_settle_C1_pre);
    assign amp_settle_C2 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_C ? {16{any_amp_settle_C}} : amp_settle_C2_pre);
    assign amp_settle_D1 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_D ? {16{any_amp_settle_D}} : amp_settle_D1_pre);
    assign amp_settle_D2 = settle_all_headstages ? {16{any_amp_settle}} : (settle_whole_headstage_D ? {16{any_amp_settle_D}} : amp_settle_D2_pre);

    // Input deserializer
      
    assign serial_CLK = serial_CLK_auto | serial_CLK_manual;
    assign serial_LOAD = serial_LOAD_auto & ~serial_LOAD_manual;
   
   
    digital_input_deserializer deserializer (.reset(reset), .dataclk(dataclk), .main_state(main_state), .channel(channel),
        .serial_in(TTL_in_serial), .serial_in_exp(TTL_in_serial_exp), .TTL_in_direct_1(TTL_in_direct_1), .TTL_in_direct_2(TTL_in_direct_2),
        .serial_CLK(serial_CLK_auto), .serial_LOAD(serial_LOAD_auto), .TTL_parallel(TTL_parallel), .TTL_parallel_exp(TTL_in));
   


/*
######   #####   ######      ######  ##    ## ######## ######  ##    ## ######## #######
##   ## ##   ## ##          ##    ## ##    ##    ##    ##   ## ##    ##    ##    ##
##   ## ####### ##          ##    ## ##    ##    ##    ######  ##    ##    ##    #######
##   ## ##   ## ##          ##    ## ##    ##    ##    ##      ##    ##    ##         ##
######  ##   ##  ######      ######   ######     ##    ##       ######     ##    #######
*/


    // Evaluation board 16-bit DAC outputs

    assign DAC_1_input_is_ref = (DAC_stream_sel_1 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_1 == DAC_reref_channel_sel);
    assign DAC_2_input_is_ref = (DAC_stream_sel_2 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_2 == DAC_reref_channel_sel);
    assign DAC_3_input_is_ref = (DAC_stream_sel_3 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_3 == DAC_reref_channel_sel);
    assign DAC_4_input_is_ref = (DAC_stream_sel_4 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_4 == DAC_reref_channel_sel);
    assign DAC_5_input_is_ref = (DAC_stream_sel_5 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_5 == DAC_reref_channel_sel);
    assign DAC_6_input_is_ref = (DAC_stream_sel_6 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_6 == DAC_reref_channel_sel);
    assign DAC_7_input_is_ref = (DAC_stream_sel_7 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_7 == DAC_reref_channel_sel);
    assign DAC_8_input_is_ref = (DAC_stream_sel_8 == { 1'b0, DAC_reref_stream_sel }) & (DAC_channel_sel_8 == DAC_reref_channel_sel);
   
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_1 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_1),
        .DAC_sequencer_in (DAC_sequencer_1),
        .use_sequencer (DAC_sequencer_en_1),
        .DAC_en          (DAC_en_1),
        .gain            (DAC_gain),
        .noise_suppress(DAC_noise_suppress),
        .DAC_SYNC       (DAC_SYNC),
        .DAC_SCLK       (DAC_SCLK),
        .DAC_DIN       (DAC_DIN_1),
        .DAC_thrsh     (DAC_thresh_1),
        .DAC_thrsh_pol (DAC_thresh_pol_1),
        .DAC_thrsh_out (DAC_thresh_out[0]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_1_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_1),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[0])
    );
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_2 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_2),
        .DAC_sequencer_in (DAC_sequencer_2),
        .use_sequencer (DAC_sequencer_en_2),
        .DAC_en          (DAC_en_2),
        .gain            (DAC_gain),
        .noise_suppress(DAC_noise_suppress),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_2),
        .DAC_thrsh     (DAC_thresh_2),
        .DAC_thrsh_pol (DAC_thresh_pol_2),
        .DAC_thrsh_out (DAC_thresh_out[1]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_2_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_2),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[1])
    );
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_3 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_3),
        .DAC_sequencer_in (DAC_sequencer_3),
        .use_sequencer (DAC_sequencer_en_3),
        .DAC_en          (DAC_en_3),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_3),
        .DAC_thrsh     (DAC_thresh_3),
        .DAC_thrsh_pol (DAC_thresh_pol_3),
        .DAC_thrsh_out (DAC_thresh_out[2]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_3_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_3),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[2])
    );
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_4 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_4),
        .DAC_sequencer_in (DAC_sequencer_4),
        .use_sequencer (DAC_sequencer_en_4),
        .DAC_en          (DAC_en_4),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_4),
        .DAC_thrsh     (DAC_thresh_4),
        .DAC_thrsh_pol (DAC_thresh_pol_4),
        .DAC_thrsh_out (DAC_thresh_out[3]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_4_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_4),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[3])
    );

    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_5 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_5),
        .DAC_sequencer_in (DAC_sequencer_5),
        .use_sequencer (DAC_sequencer_en_5),
        .DAC_en          (DAC_en_5),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_5),
        .DAC_thrsh     (DAC_thresh_5),
        .DAC_thrsh_pol (DAC_thresh_pol_5),
        .DAC_thrsh_out (DAC_thresh_out[4]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_5_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_5),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[4])
    );

    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_6 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_6),
        .DAC_sequencer_in (DAC_sequencer_6),
        .use_sequencer (DAC_sequencer_en_6),
        .DAC_en          (DAC_en_6),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_6),
        .DAC_thrsh     (DAC_thresh_6),
        .DAC_thrsh_pol (DAC_thresh_pol_6),
        .DAC_thrsh_out (DAC_thresh_out[5]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_6_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_6),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[5])
    );
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_7 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_7),
        .DAC_sequencer_in (DAC_sequencer_7),
        .use_sequencer (DAC_sequencer_en_7),
        .DAC_en          (DAC_en_7),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_7),
        .DAC_thrsh     (DAC_thresh_7),
        .DAC_thrsh_pol (DAC_thresh_pol_7),
        .DAC_thrsh_out (DAC_thresh_out[6]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_7_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_7),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[6])
    );
   
    DAC_output_scalable_HPF #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk9_d  (ms_clk9_d),
        .ms_clk18_c (ms_clk18_c),
        .ms_clk27_b (ms_clk27_b)
        )
        DAC_output_8 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .DAC_input       (DAC_register_8),
        .DAC_sequencer_in (DAC_sequencer_8),
        .use_sequencer (DAC_sequencer_en_8),
        .DAC_en          (DAC_en_8),
        .gain            (DAC_gain),
        .noise_suppress(0),
        .DAC_SYNC       (),
        .DAC_SCLK       (),
        .DAC_DIN       (DAC_DIN_8),
        .DAC_thrsh     (DAC_thresh_8),
        .DAC_thrsh_pol (DAC_thresh_pol_8),
        .DAC_thrsh_out (DAC_thresh_out[7]),
        .HPF_coefficient (HPF_coefficient),
        .HPF_en         (HPF_en),
        .software_reference_mode (DAC_reref_mode & ~DAC_8_input_is_ref),
        .software_reference (DAC_reref_register),
        .DAC_register   (DAC_output_register_8),
        .Trigger_bypass (CSTtrig_to_DAC),
        .Trigger_value  (CST_trig_stim[7])
    );
   


/*
 #####  ######   ######     ## ###    ## ######  ##    ## ######## #######
##   ## ##   ## ##          ## ####   ## ##   ## ##    ##    ##    ##
####### ##   ## ##          ## ## ##  ## ######  ##    ##    ##    #######
##   ## ##   ## ##          ## ##  ## ## ##      ##    ##    ##         ##
##   ## ######   ######     ## ##   #### ##       ######     ##    #######
*/


    // Evaluation board 16-bit ADC inputs

    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_1 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_1),
        .ADC_CS         (ADC_CS),
        .ADC_SCLK      (ADC_SCLK),
        .ADC_register   (data_stream_ADC_1)
    );

    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_2 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_2),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_2)
    );

    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_3 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_3),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_3)
    );
   
    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_4 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_4),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_4)
    );

    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_5 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_5),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_5)
    );
   
    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_6 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_6),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_6)
    );
   
    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_7 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_7),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_7)
    );
   
    ADC_input #(
        .ms_wait      (ms_wait),
        .ms_clk1_a    (ms_clk1_a),
        .ms_clk18_c   (ms_clk18_c)
        )
        ADC_inout_8 (
        .reset         (reset),
        .dataclk         (dataclk),
        .main_state      (main_state),
        .channel         (channel),
        .ADC_DOUT      (ADC_DOUT_8),
        .ADC_CS         (),
        .ADC_SCLK      (),
        .ADC_register   (data_stream_ADC_8)
    );



/*
###    ### ## #######  ######      ######  ##   ##  #####  ####### #######     ####### ####### ##      #######  ###### ########  ######  ######
####  #### ## ##      ##    ##     ##   ## ##   ## ##   ## ##      ##          ##      ##      ##      ##      ##         ##    ##    ## ##   ##
## #### ## ## ####### ##    ##     ######  ####### ####### ####### #####       ####### #####   ##      #####   ##         ##    ##    ## ######
##  ##  ## ##      ## ##    ##     ##      ##   ## ##   ##      ## ##               ## ##      ##      ##      ##         ##    ##    ## ##   ##
##      ## ## #######  ######      ##      ##   ## ##   ## ####### #######     ####### ####### ####### #######  ######    ##     ######  ##   ##
*/


    // MISO phase selectors (to compensate for headstage cable delays)

    MISO_phase_selector MISO_phase_selector_1 (
        .phase_select(delay_A), .MISO4x(in4x_A1), .MISO(in_A1));   

    MISO_phase_selector MISO_phase_selector_2 (
        .phase_select(delay_A), .MISO4x(in4x_A2), .MISO(in_A2));   

    MISO_phase_selector MISO_phase_selector_3 (
        .phase_select(delay_B), .MISO4x(in4x_B1), .MISO(in_B1));   

    MISO_phase_selector MISO_phase_selector_4 (
        .phase_select(delay_B), .MISO4x(in4x_B2), .MISO(in_B2));   
   
    MISO_phase_selector MISO_phase_selector_5 (
        .phase_select(delay_C), .MISO4x(in4x_C1), .MISO(in_C1));   

    MISO_phase_selector MISO_phase_selector_6 (
        .phase_select(delay_C), .MISO4x(in4x_C2), .MISO(in_C2));   
   
    MISO_phase_selector MISO_phase_selector_7 (
        .phase_select(delay_D), .MISO4x(in4x_D1), .MISO(in_D1));

    MISO_phase_selector MISO_phase_selector_8 (
        .phase_select(delay_D), .MISO4x(in4x_D2), .MISO(in_D2));   

    assign data_stream_1 = result_A1;
    assign data_stream_2 = result_A2;
    assign data_stream_3 = result_B1;
    assign data_stream_4 = result_B2;
    assign data_stream_5 = result_C1;
    assign data_stream_6 = result_C2;
    assign data_stream_7 = result_D1;
    assign data_stream_8 = result_D2;

   

/*
 ######  ##   ##     ##    ## ####### ######      ##     ##  ######
##    ## ##  ##      ##    ## ##      ##   ##     ##    ##  ##    ##
##    ## #####       ##    ## ####### ######      ##   ##   ##    ##
##    ## ##  ##      ##    ##      ## ##   ##     ##  ##    ##    ##
 ######  ##   ##      ######  ####### ######      ## ##      ######
*/


    // Opal Kelly USB I/O Host and Endpoint Modules
   
   
    okHost host (
        .hi_in(hi_in),
        .hi_out(hi_out),
        .hi_inout(hi_inout),
        .hi_aa(hi_aa),
        .ti_clk(ti_clk),
        .ok1(ok1), 
        .ok2(ok2)
        );
      
    wire [17*43-1:0]    ok2x;
    okWireOR # (.N(43)) wireOR (ok2, ok2x);

    okWireIn     wi00 (.ok1(ok1),                            .ep_addr(8'h00), .ep_dataout(ep00wirein));
    okWireIn     wi01 (.ok1(ok1),                            .ep_addr(8'h01), .ep_dataout(ep01wirein));
    okWireIn     wi02 (.ok1(ok1),                            .ep_addr(8'h02), .ep_dataout(ep02wirein));
    okWireIn     wi03 (.ok1(ok1),                            .ep_addr(8'h03), .ep_dataout(ep03wirein));
    okWireIn     wi04 (.ok1(ok1),                            .ep_addr(8'h04), .ep_dataout(ep04wirein));
    okWireIn     wi05 (.ok1(ok1),                            .ep_addr(8'h05), .ep_dataout(ep05wirein));
    okWireIn     wi06 (.ok1(ok1),                            .ep_addr(8'h06), .ep_dataout(ep06wirein));
    okWireIn     wi07 (.ok1(ok1),                            .ep_addr(8'h07), .ep_dataout(ep07wirein));
    okWireIn     wi08 (.ok1(ok1),                            .ep_addr(8'h08), .ep_dataout(ep08wirein));
    okWireIn     wi09 (.ok1(ok1),                            .ep_addr(8'h09), .ep_dataout(ep09wirein));
    okWireIn     wi0a (.ok1(ok1),                            .ep_addr(8'h0a), .ep_dataout(ep0awirein));
    okWireIn     wi0b (.ok1(ok1),                            .ep_addr(8'h0b), .ep_dataout(ep0bwirein));
    okWireIn     wi0c (.ok1(ok1),                            .ep_addr(8'h0c), .ep_dataout(ep0cwirein));
    okWireIn     wi0d (.ok1(ok1),                            .ep_addr(8'h0d), .ep_dataout(ep0dwirein));
    okWireIn     wi0e (.ok1(ok1),                            .ep_addr(8'h0e), .ep_dataout(ep0ewirein));
    okWireIn     wi0f (.ok1(ok1),                            .ep_addr(8'h0f), .ep_dataout(ep0fwirein));
    okWireIn     wi10 (.ok1(ok1),                            .ep_addr(8'h10), .ep_dataout(ep10wirein));
    okWireIn     wi11 (.ok1(ok1),                            .ep_addr(8'h11), .ep_dataout(ep11wirein));
    okWireIn     wi12 (.ok1(ok1),                            .ep_addr(8'h12), .ep_dataout(ep12wirein));
    okWireIn     wi13 (.ok1(ok1),                            .ep_addr(8'h13), .ep_dataout(ep13wirein));
    okWireIn     wi14 (.ok1(ok1),                            .ep_addr(8'h14), .ep_dataout(ep14wirein));
    okWireIn     wi15 (.ok1(ok1),                            .ep_addr(8'h15), .ep_dataout(ep15wirein));
    okWireIn     wi16 (.ok1(ok1),                            .ep_addr(8'h16), .ep_dataout(ep16wirein));
    okWireIn     wi17 (.ok1(ok1),                            .ep_addr(8'h17), .ep_dataout(ep17wirein));
    okWireIn     wi18 (.ok1(ok1),                            .ep_addr(8'h18), .ep_dataout(ep18wirein));
    okWireIn     wi19 (.ok1(ok1),                            .ep_addr(8'h19), .ep_dataout(ep19wirein));
    okWireIn     wi1a (.ok1(ok1),                            .ep_addr(8'h1a), .ep_dataout(ep1awirein));
    okWireIn     wi1b (.ok1(ok1),                            .ep_addr(8'h1b), .ep_dataout(ep1bwirein));
    okWireIn     wi1c (.ok1(ok1),                            .ep_addr(8'h1c), .ep_dataout(ep1cwirein));
    okWireIn     wi1d (.ok1(ok1),                            .ep_addr(8'h1d), .ep_dataout(ep1dwirein));
    okWireIn     wi1e (.ok1(ok1),                            .ep_addr(8'h1e), .ep_dataout(ep1ewirein));
    okWireIn     wi1f (.ok1(ok1),                            .ep_addr(8'h1f), .ep_dataout(ep1fwirein));
   
    okTriggerIn  ti40 (.ok1(ok1),                            .ep_addr(8'h40), .ep_clk(ti_clk),  .ep_trigger(ep40trigin));
    okTriggerIn  ti41 (.ok1(ok1),                            .ep_addr(8'h41), .ep_clk(dataclk), .ep_trigger(ep41trigin));
    okTriggerIn  ti42 (.ok1(ok1),                            .ep_addr(8'h42), .ep_clk(ti_clk),  .ep_trigger(ep42trigin));
    okTriggerIn  ti43 (.ok1(ok1),                            .ep_addr(8'h43), .ep_clk(ti_clk),  .ep_trigger(ep43trigin));
    okTriggerIn  ti44 (.ok1(ok1),                            .ep_addr(8'h44), .ep_clk(ti_clk),  .ep_trigger(ep44trigin));
    okTriggerIn  ti45 (.ok1(ok1),                            .ep_addr(8'h45), .ep_clk(ti_clk),  .ep_trigger(ep45trigin));
   
    okWireOut    wo20 (.ok1(ok1), .ok2(ok2x[ 0*17 +: 17 ]),  .ep_addr(8'h20), .ep_datain(ep20wireout));
    okWireOut    wo21 (.ok1(ok1), .ok2(ok2x[ 1*17 +: 17 ]),  .ep_addr(8'h21), .ep_datain(ep21wireout));
    okWireOut    wo22 (.ok1(ok1), .ok2(ok2x[ 2*17 +: 17 ]),  .ep_addr(8'h22), .ep_datain(ep22wireout));
    okWireOut    wo23 (.ok1(ok1), .ok2(ok2x[ 3*17 +: 17 ]),  .ep_addr(8'h23), .ep_datain(ep23wireout));
    okWireOut    wo24 (.ok1(ok1), .ok2(ok2x[ 4*17 +: 17 ]),  .ep_addr(8'h24), .ep_datain(ep24wireout));
    okWireOut    wo25 (.ok1(ok1), .ok2(ok2x[ 5*17 +: 17 ]),  .ep_addr(8'h25), .ep_datain(ep25wireout));
    okWireOut    wo26 (.ok1(ok1), .ok2(ok2x[ 6*17 +: 17 ]),  .ep_addr(8'h26), .ep_datain(ep26wireout));
    okWireOut    wo27 (.ok1(ok1), .ok2(ok2x[ 7*17 +: 17 ]),  .ep_addr(8'h27), .ep_datain(ep27wireout));
    okWireOut    wo28 (.ok1(ok1), .ok2(ok2x[ 8*17 +: 17 ]),  .ep_addr(8'h28), .ep_datain(ep28wireout));
    okWireOut    wo29 (.ok1(ok1), .ok2(ok2x[ 9*17 +: 17 ]),  .ep_addr(8'h29), .ep_datain(ep29wireout));
    okWireOut    wo2a (.ok1(ok1), .ok2(ok2x[ 10*17 +: 17 ]), .ep_addr(8'h2a), .ep_datain(ep2awireout));
    okWireOut    wo2b (.ok1(ok1), .ok2(ok2x[ 11*17 +: 17 ]), .ep_addr(8'h2b), .ep_datain(ep2bwireout));
    okWireOut    wo2c (.ok1(ok1), .ok2(ok2x[ 12*17 +: 17 ]), .ep_addr(8'h2c), .ep_datain(ep2cwireout));
    okWireOut    wo2d (.ok1(ok1), .ok2(ok2x[ 13*17 +: 17 ]), .ep_addr(8'h2d), .ep_datain(ep2dwireout));
    okWireOut    wo2e (.ok1(ok1), .ok2(ok2x[ 14*17 +: 17 ]), .ep_addr(8'h2e), .ep_datain(ep2ewireout));
    okWireOut    wo2f (.ok1(ok1), .ok2(ok2x[ 15*17 +: 17 ]), .ep_addr(8'h2f), .ep_datain(ep2fwireout));
    okWireOut    wo30 (.ok1(ok1), .ok2(ok2x[ 16*17 +: 17 ]), .ep_addr(8'h30), .ep_datain(ep30wireout));
    okWireOut    wo31 (.ok1(ok1), .ok2(ok2x[ 17*17 +: 17 ]), .ep_addr(8'h31), .ep_datain(ep31wireout));
    okWireOut    wo32 (.ok1(ok1), .ok2(ok2x[ 18*17 +: 17 ]), .ep_addr(8'h32), .ep_datain(ep32wireout));
    okWireOut    wo33 (.ok1(ok1), .ok2(ok2x[ 19*17 +: 17 ]), .ep_addr(8'h33), .ep_datain(ep33wireout));
    okWireOut    wo34 (.ok1(ok1), .ok2(ok2x[ 20*17 +: 17 ]), .ep_addr(8'h34), .ep_datain(ep34wireout));
    okWireOut    wo35 (.ok1(ok1), .ok2(ok2x[ 21*17 +: 17 ]), .ep_addr(8'h35), .ep_datain(ep35wireout));
    okWireOut    wo36 (.ok1(ok1), .ok2(ok2x[ 22*17 +: 17 ]), .ep_addr(8'h36), .ep_datain(ep36wireout));
    okWireOut    wo37 (.ok1(ok1), .ok2(ok2x[ 23*17 +: 17 ]), .ep_addr(8'h37), .ep_datain(ep37wireout));
    okWireOut    wo38 (.ok1(ok1), .ok2(ok2x[ 24*17 +: 17 ]), .ep_addr(8'h38), .ep_datain(ep38wireout));
    okWireOut    wo39 (.ok1(ok1), .ok2(ok2x[ 25*17 +: 17 ]), .ep_addr(8'h39), .ep_datain(ep39wireout));
    okWireOut    wo3a (.ok1(ok1), .ok2(ok2x[ 26*17 +: 17 ]), .ep_addr(8'h3a), .ep_datain(ep3awireout));
    okWireOut    wo3b (.ok1(ok1), .ok2(ok2x[ 27*17 +: 17 ]), .ep_addr(8'h3b), .ep_datain(ep3bwireout));
    okWireOut    wo3c (.ok1(ok1), .ok2(ok2x[ 28*17 +: 17 ]), .ep_addr(8'h3c), .ep_datain(ep3cwireout));
    okWireOut    wo3d (.ok1(ok1), .ok2(ok2x[ 29*17 +: 17 ]), .ep_addr(8'h3d), .ep_datain(ep3dwireout));
    okWireOut    wo3e (.ok1(ok1), .ok2(ok2x[ 30*17 +: 17 ]), .ep_addr(8'h3e), .ep_datain(ep3ewireout));
    okWireOut    wo3f (.ok1(ok1), .ok2(ok2x[ 31*17 +: 17 ]), .ep_addr(8'h3f), .ep_datain(ep3fwireout));
    
    okPipeIn     pi80 (.ok1(ok1), .ok2(ok2x[ 32*17 +: 17 ]), .ep_addr(8'h80), .ep_write(pipe_in_write_1_MSW), .ep_dataout(pipe_in_data_1_MSW));
    okPipeIn     pi81 (.ok1(ok1), .ok2(ok2x[ 33*17 +: 17 ]), .ep_addr(8'h81), .ep_write(pipe_in_write_1_LSW), .ep_dataout(pipe_in_data_1_LSW));
    okPipeIn     pi82 (.ok1(ok1), .ok2(ok2x[ 34*17 +: 17 ]), .ep_addr(8'h82), .ep_write(pipe_in_write_2_MSW), .ep_dataout(pipe_in_data_2_MSW));
    okPipeIn     pi83 (.ok1(ok1), .ok2(ok2x[ 35*17 +: 17 ]), .ep_addr(8'h83), .ep_write(pipe_in_write_2_LSW), .ep_dataout(pipe_in_data_2_LSW));
    okPipeIn     pi84 (.ok1(ok1), .ok2(ok2x[ 36*17 +: 17 ]), .ep_addr(8'h84), .ep_write(pipe_in_write_3_MSW), .ep_dataout(pipe_in_data_3_MSW));
    okPipeIn     pi85 (.ok1(ok1), .ok2(ok2x[ 37*17 +: 17 ]), .ep_addr(8'h85), .ep_write(pipe_in_write_3_LSW), .ep_dataout(pipe_in_data_3_LSW));
    okPipeIn     pi86 (.ok1(ok1), .ok2(ok2x[ 38*17 +: 17 ]), .ep_addr(8'h86), .ep_write(pipe_in_write_4_MSW), .ep_dataout(pipe_in_data_4_MSW));
    okPipeIn     pi87 (.ok1(ok1), .ok2(ok2x[ 39*17 +: 17 ]), .ep_addr(8'h87), .ep_write(pipe_in_write_4_LSW), .ep_dataout(pipe_in_data_4_LSW));
    okPipeIn     pi88 (.ok1(ok1), .ok2(ok2x[ 40*17 +: 17 ]), .ep_addr(8'h88), .ep_write(pipe_usb_to_cst_en),  .ep_dataout(pipe_usb_to_cst_data));    //YB: added for custom processing configuration

 
    okPipeOut    poa0 (.ok1(ok1), .ok2(ok2x[ 41*17 +: 17 ]), .ep_addr(8'ha0), .ep_read(USB_FIFO_read_from), .ep_datain(USB_FIFO_data_out));   
    okPipeOut    poa1 (.ok1(ok1), .ok2(ok2x[ 42*17 +: 17 ]), .ep_addr(8'ha1), .ep_read(pipe_cst_to_usb_read), .ep_datain(pipe_cst_to_usb_data));     //YB: added for custom processing configuration

 

endmodule



/*
 ######  ######  ###    ### ###    ###  #####  ###    ## ######      ####### ####### ##      #######  ###### ########     ######  ####### #######
##      ##    ## ####  #### ####  #### ##   ## ####   ## ##   ##     ##      ##      ##      ##      ##         ##        ##   ## ##      ##
##      ##    ## ## #### ## ## #### ## ####### ## ##  ## ##   ##     ####### #####   ##      #####   ##         ##        ##   ## #####   #####
##      ##    ## ##  ##  ## ##  ##  ## ##   ## ##  ## ## ##   ##          ## ##      ##      ##      ##         ##        ##   ## ##      ##
 ######  ######  ##      ## ##      ## ##   ## ##   #### ######      ####### ####### ####### #######  ######    ## ##     ######  ####### ## ##
*/



// This simple module creates MOSI commands.  If channel is between 0 and 15, the command is CONVERT(channel),
// and the LSB is set if DSP_settle = 1.  If channel is between 16 and 19, send stimulation commands.
module command_selector_stim (
    input wire [5:0]       channel,
    input wire             shutdown,
    input wire             DSP_settle,
    input wire             amp_settle_mode,
    input wire             charge_recov_mode,
    input wire [31:0]      aux_cmd,
    input wire             use_aux_cmd,
    input wire             DC_amp_convert,
    input wire             stim_en,
    input wire [15:0]      stim_on,
    input wire [15:0]      stim_pol,
    input wire [15:0]      charge_recov,
    input wire [15:0]      amp_settle,
    input wire             amp_settle_changed,
    output reg [31:0]      MOSI_cmd
    );

    wire [7:0] amp_settle_register, charge_recov_register, compliance_register, stim_on_register, stim_pol_register;
   
    assign compliance_register = 8'd40;
    assign stim_on_register    = 8'd42;
    assign stim_pol_register   = 8'd44;
   
    // If amp_settle_mode = 0, use amplifier low frequency cutoff select (Register 12)
    // If amp_settle_mode = 1, use amplifier fast settle (Register 10)
    assign amp_settle_register = amp_settle_mode ? 8'd10 : 8'd12;
    
    // If charge_recov_mode = 0, use current-limited charge recovery drivers (Register 48)
    // If charge_recov_mode = 1, use charge recovery switch (Register 46)
    assign charge_recov_register = charge_recov_mode ? 8'd46 : 8'd48;

    wire [31:0] aux_cmd1;
    assign aux_cmd1 = use_aux_cmd ? aux_cmd : { 2'b11, 2'b00, 4'b0000, 8'hff, 16'b0 };// if use_aux_cmd = 0, read from ROM register 255
      
    // If shutdown == 1, issue commands to turn off stimulation, amp settling, and charge recovery circuits.
    wire [15:0] stim_on_cmd, stim_pol_cmd, charge_recov_cmd, amp_settle_cmd;
    assign stim_on_cmd      = shutdown ? 16'b0 : stim_on;
    assign stim_pol_cmd     = shutdown ? 16'b0 : stim_pol;
    assign charge_recov_cmd = shutdown ? 16'b0 : charge_recov;
    assign amp_settle_cmd   = shutdown ? 16'b0 : amp_settle;
    
    // Opportunistically read from compliance monitor register and clear it.
    // If amp settle command hasn't changed, read from the compliance monitor (register 40) instead...
    wire [31:0] amp_settle_or_compliance_monitor_cmd;
    assign amp_settle_or_compliance_monitor_cmd = amp_settle_changed ?
        { 2'b10, 2'b00, 4'b0000, amp_settle_register, amp_settle_cmd } : { 2'b11, 2'b00, 4'b0000, compliance_register, 16'h0000 };
   
    // ...and if we read from it, set the M flag to clear the compliance monitor one command later.
    wire [31:0] charge_recov_or_compliance_monitor_cmd;
    assign charge_recov_or_compliance_monitor_cmd = amp_settle_changed ? 
        { 2'b10, 2'b10, 4'b0000, charge_recov_register, charge_recov_cmd } : { 2'b10, 2'b11, 4'b0000, charge_recov_register, charge_recov_cmd };
   
    always @(*) begin
        case (channel)
            0:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            1:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            2:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            3:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            4:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            5:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            6:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            7:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            8:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            9:       MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            10:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            11:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            12:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            13:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            14:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            15:      MOSI_cmd <= { 2'b00, 2'b00, DC_amp_convert, DSP_settle, 4'b0000, channel, 16'h0000 };
            16:      MOSI_cmd <= stim_en ? { 2'b10, 2'b00, 4'b0000, stim_on_register, stim_on_cmd  } : aux_cmd1; // turn stim on/off
            17:      MOSI_cmd <= stim_en ? { 2'b10, 2'b00, 4'b0000, stim_pol_register, stim_pol_cmd } : aux_cmd1; // set stim polarity
            18:      MOSI_cmd <= stim_en ? amp_settle_or_compliance_monitor_cmd : aux_cmd1; // set amp settle
            19:      MOSI_cmd <= stim_en ? charge_recov_or_compliance_monitor_cmd : aux_cmd1; // set charge recovery, trigger U flag
            default: MOSI_cmd <= 32'b0;
        endcase
    end   
   
endmodule
