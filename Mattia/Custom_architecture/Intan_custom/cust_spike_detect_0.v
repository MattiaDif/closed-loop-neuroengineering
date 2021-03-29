//////////////////////////////////////////////////////////////////////////////////
// Company: 		 IMS lab - Univ. Bordeaux - Bordeaux INP
// 
// Design Name: 	 RHD2000 Rhythm USB3 Interface computation complement
// Module Name:    CUST_spike_detect_0 
// Project Name:   Stimulation controller
// Target Devices: 
// Tool versions: 
// Description:    
//
// Dependencies: 
//
// 
// This detection technique is the result of 4 tests on the waveform data.
// Each test is of the kind "above or below threshold" for a given time.
// It is then possible to select on which set of samples the test should apply
//
// Instead of storing all samples then apply the tests on the final stream,
// the module only stores the result of the tests in process and updates these
// partial results at each new sample. e.g. :
//      if we state that samples from 0 to 40 should be negative:
//      - a new negative sample will no imply particular action
//      - a new positive sample will mark tests from 0 to 40 as False
//
// The history stored is 288 bits, but it is only possible to address the 256
// more recent tests. The remaining bits might be useful to memorise extra 
// values like a refractory period counter. They are stored in consecutive memory
// addresses.
//
//
//////////////////////////////////////////////////////////////////////////////////

module CUST_spike_detect_0 #(
	parameter CHANNELS 	   = 128,
    parameter CHANNELS_PW2 = 7
	)
	(
	input  wire                      clk,
	input  wire                      reset,
    input  wire             [15 : 0] chan_in_sample,
    input  wire [CHANNELS_PW2-1 : 0] chan_in_num,
    input  wire                      chan_in_valid,
    output wire                      chan_in_read,
    
    output reg  [CHANNELS_PW2-1 : 0] chan_out_num,
    output wire                      chan_out_valid,
    input  wire                      chan_out_read,
    
    input  wire             [15 : 0] threshold_0,
    input  wire             [7  : 0] comp_start_0,
    input  wire             [7  : 0] comp_stop_0,
    input  wire                      above_0,
    input  wire                      enable_0,
    input  wire             [15 : 0] threshold_1,
    input  wire             [7  : 0] comp_start_1,
    input  wire             [7  : 0] comp_stop_1,
    input  wire                      above_1,
    input  wire                      enable_1,
    input  wire             [15 : 0] threshold_2,
    input  wire             [7  : 0] comp_start_2,
    input  wire             [7  : 0] comp_stop_2,
    input  wire                      above_2,
    input  wire                      enable_2,
    input  wire             [15 : 0] threshold_3,
    input  wire             [7  : 0] comp_start_3,
    input  wire             [7  : 0] comp_stop_3,
    input  wire                      above_3,
    input  wire                      enable_3,
    input  wire             [7  : 0] final_decision_sample
   );




/*
######  #######  ###### ##       #####  ######   #####  ######## ##  ######  ###    ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ####   ## ##
##   ## #####   ##      ##      ####### ######  #######    ##    ## ##    ## ## ##  ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ##  ## ##      ##
######  #######  ###### ####### ##   ## ##   ## ##   ##    ##    ##  ######  ##   #### #######
*/


    reg [35:0]  chan_history[0:8*CHANNELS-1];       // this memory is used to store history of 
                                                    // previous tests, 288 bits store whether
                                                    // the last 288 samples have fulfilled the
                                                    // tests so far. 
                                                    
    reg [CHANNELS_PW2+2 : 0]  RAM_address_A;        // these two addresses are used for memory
    reg [CHANNELS_PW2+2 : 0]  RAM_address_B;        // exchanges. 
    reg                       RAM_write;            // should we write on the different addresses ?
    reg [            35 : 0]  RAM_in_A;             // data to write in port A
    reg [            35 : 0]  RAM_in_B;             // data to write in port B
    reg [            35 : 0]  RAM_out_A;            // data read from port A
    reg [            35 : 0]  RAM_out_B;            // data read from port B

    reg [CHANNELS_PW2-1 : 0]  processed_channel;    // the channel we are processing
    reg [            15 : 0]  new_sample;           // the new sample to take into account
    
    reg [           288 : 0]  test_history;         // the results of the tests so far, the upper
                                                    // the number, the older the test

    reg [             3 : 0]  threshold_pass;       // which thresholds does the current sample pass ?
    reg                       output_full;          // the output of the module has been read
    reg                       spike_detected;       // true when a spike has actually be detected

    integer                   main_state;           // the state of the module


    // the following states for the machine identify the data that is expected on next write
    localparam  state_idle           =  0,  // this is the default state, nothing happens here
                state_resetting      =  1,  // the state machine acknowledges the reset input
                                            // if the system is not in reset anymore, it is
                                            // erasing the content of the memory
                
                state_mem_read_0     =  8,  // reading the history of the channel
                state_mem_read_1     =  9,  //    ""
                state_mem_read_2     = 10,  //    ""
                state_mem_read_3     = 11,  //    ""
                state_wait_mem_lat_0 = 12,  // waiting for read latency to propagate
                                            // we also compute threshold_pass
                state_wait_mem_lat_1 = 13,  // waiting for read latency to propagate

                state_apply_test_0   = 16,  // the result of the test is applied
                state_apply_test_1   = 17,  //    ""
                state_apply_test_2   = 18,  //    ""
                state_apply_test_3   = 19,  //    ""
                
                state_mem_write_0    = 20,  // we write back the history of the channel to memory
                state_mem_write_1    = 21,  //       ""
                state_mem_write_2    = 22,  //       ""
                state_mem_write_3    = 23;  //       ""

    // internal loop counter
    integer index;


/*
####### ########  #####  ######## #######     ###    ###  #####   ###### ##   ## ## ###    ## #######
##         ##    ##   ##    ##    ##          ####  #### ##   ## ##      ##   ## ## ####   ## ##
#######    ##    #######    ##    #####       ## #### ## ####### ##      ####### ## ## ##  ## #####
     ##    ##    ##   ##    ##    ##          ##  ##  ## ##   ## ##      ##   ## ## ##  ## ## ##
#######    ##    ##   ##    ##    #######     ##      ## ##   ##  ###### ##   ## ## ##   #### #######
*/





    always @(posedge clk) begin
        if (reset) begin
            main_state <= state_resetting;
        end else begin
            case (main_state)
                state_resetting      : if ((~RAM_address_B)==0) main_state <= state_idle;
                state_idle           : if (chan_in_valid)       main_state <= state_mem_read_0;
                state_mem_read_0     :                          main_state <= state_mem_read_1;
                state_mem_read_1     :                          main_state <= state_mem_read_2;
                state_mem_read_2     :                          main_state <= state_mem_read_3;
                state_mem_read_3     :                          main_state <= state_wait_mem_lat_0;
                state_wait_mem_lat_0 :                          main_state <= state_wait_mem_lat_1;
                state_wait_mem_lat_1 :                          main_state <= state_apply_test_0;
                state_apply_test_0   :                          main_state <= state_apply_test_1;
                state_apply_test_1   :                          main_state <= state_apply_test_2;
                state_apply_test_2   :                          main_state <= state_apply_test_3;
                state_apply_test_3   :                          main_state <= state_mem_write_0;
                state_mem_write_0    :                          main_state <= state_mem_write_1;
                state_mem_write_1    : if (!spike_detected || !output_full)
                                                                main_state <= state_mem_write_2;
                state_mem_write_2    :                          main_state <= state_mem_write_3;
                state_mem_write_3    :                          main_state <= state_idle;
                default              : begin end
            endcase
        end
    end

//    assign chan_in_read = (main_state == state_idle); SB for debug only
		assign chan_in_read = 1;



/*
###    ### ####### ###    ###  ######  ######  ##    ##
####  #### ##      ####  #### ##    ## ##   ##  ##  ##
## #### ## #####   ## #### ## ##    ## ######    ####
##  ##  ## ##      ##  ##  ## ##    ## ##   ##    ##
##      ## ####### ##      ##  ######  ##   ##    ##
*/

    always @(posedge clk) begin
        if (reset) begin
            RAM_address_A <= 0;
            RAM_address_B <= 1;
            RAM_write     <= 1'b0;
        end else begin
            case (main_state)
                state_resetting      : begin
                                           RAM_write     <= 1'b1;
                                           if (!RAM_write) begin
                                                RAM_address_A <= RAM_address_A + 2;
                                                RAM_address_B <= RAM_address_B + 2;
                                           end
                                       end
                state_mem_read_0     : begin
                                            RAM_address_A <= {processed_channel, 3'b000};
                                            RAM_address_A <= {processed_channel, 3'b001};
                                            RAM_write     <= 1'b0;
                                       end
                state_mem_read_1     : begin
                                            RAM_address_A <= {processed_channel, 3'b010};
                                            RAM_address_A <= {processed_channel, 3'b011};
                                            RAM_write     <= 1'b0;
                                       end
                state_mem_read_2     : begin
                                            RAM_address_A <= {processed_channel, 3'b100};
                                            RAM_address_A <= {processed_channel, 3'b101};
                                            RAM_write     <= 1'b0;
                                       end
                state_mem_read_3     : begin
                                            RAM_address_A <= {processed_channel, 3'b110};
                                            RAM_address_A <= {processed_channel, 3'b111};
                                            RAM_write     <= 1'b0;
                                       end
                state_mem_write_0    : begin
                                            RAM_address_A <= {processed_channel, 3'b000};
                                            RAM_address_A <= {processed_channel, 3'b001};
                                            RAM_write     <= 1'b1;
                                       end
                state_mem_write_1    : begin
                                            RAM_address_A <= {processed_channel, 3'b010};
                                            RAM_address_A <= {processed_channel, 3'b011};
                                            RAM_write     <= 1'b1;
                                       end
                state_mem_write_2    : begin
                                            RAM_address_A <= {processed_channel, 3'b100};
                                            RAM_address_A <= {processed_channel, 3'b101};
                                            RAM_write     <= 1'b1;
                                       end
                state_mem_write_3    : begin
                                            RAM_address_A <= {processed_channel, 3'b110};
                                            RAM_address_A <= {processed_channel, 3'b111};
                                            RAM_write     <= 1'b1;
                                       end
                default              : RAM_write     <= 1'b0;
            endcase
        end
    end


    always @(posedge clk) begin
        if (RAM_write) begin
            chan_history[RAM_address_A] <= RAM_in_A;
            chan_history[RAM_address_B] <= RAM_in_B;
        end
        RAM_out_A <= chan_history[RAM_address_A];
        RAM_out_B <= chan_history[RAM_address_B];
    end




/*
 ######  ######  ###    ### ######  ##    ## ########  #####  ######## ##  ######  ###    ##
##      ##    ## ####  #### ##   ## ##    ##    ##    ##   ##    ##    ## ##    ## ####   ##
##      ##    ## ## #### ## ######  ##    ##    ##    #######    ##    ## ##    ## ## ##  ##
##      ##    ## ##  ##  ## ##      ##    ##    ##    ##   ##    ##    ## ##    ## ##  ## ##
 ######  ######  ##      ## ##       ######     ##    ##   ##    ##    ##  ######  ##   ####
*/

    // this part to store the information about the channel and sample being processed
    always @(posedge clk) begin
        if (reset) begin
            processed_channel <= 0;
            new_sample        <= 0;
        end else if (main_state == state_idle && chan_in_valid) begin
            processed_channel <= chan_in_num;
            new_sample        <= chan_in_sample;
        end
    end
    
    // does our sample fit the different tests ?
    always @(posedge clk) begin
        if (main_state == state_wait_mem_lat_0) begin   // we only compute this once
            if (((new_sample > threshold_0) ^ above_0) && (new_sample != threshold_0)) threshold_pass[0] <= 1'b0;
            else                                                                       threshold_pass[0] <= 1'b1;

            if (((new_sample > threshold_1) ^ above_1) && (new_sample != threshold_1)) threshold_pass[1] <= 1'b0;
            else                                                                       threshold_pass[1] <= 1'b1;

            if (((new_sample > threshold_2) ^ above_2) && (new_sample != threshold_2)) threshold_pass[2] <= 1'b0;
            else                                                                       threshold_pass[2] <= 1'b1;

            if (((new_sample > threshold_3) ^ above_3) && (new_sample != threshold_3)) threshold_pass[3] <= 1'b0;
            else                                                                       threshold_pass[3] <= 1'b1;
        end
    end
    
    
    



    always @(posedge clk) begin
        if (reset) test_history <= 0;
        else 
            case (main_state)
                state_mem_read_2 : begin
                                    test_history[      0] <= 1'b1;      // no test has cleared the new
                                                                        // sample (yet)
                                    test_history[ 36:  1] <= RAM_out_A;
                                    test_history[ 72: 37] <= RAM_out_B;
                                   end
                state_mem_read_3 : begin
                                    test_history[108: 73] <= RAM_out_A;
                                    test_history[144:109] <= RAM_out_B;
                                   end
                state_wait_mem_lat_0 : begin
                                    test_history[180:145] <= RAM_out_A;
                                    test_history[216:181] <= RAM_out_B;
                                   end
                state_wait_mem_lat_1 : begin
                                    test_history[252:217] <= RAM_out_A;
                                    test_history[288:253] <= RAM_out_B;
                                   end
                state_apply_test_0 : begin
                                        if (! threshold_pass[0])
                                            for (index=0; index<=288; index=index+1)
                                                if ((index >= comp_start_0) && (comp_stop_0 >= index))
                                                    test_history[index] <= 1'b0;
                                   end
                state_apply_test_1 : begin
                                        if (! threshold_pass[1])
                                            for (index=0; index<=288; index=index+1)
                                                if ((index >= comp_start_1) && (comp_stop_1 >= index))
                                                    test_history[index] <= 1'b0;
                                   end
                state_apply_test_2 : begin
                                        if (! threshold_pass[0])
                                            for (index=0; index<=288; index=index+1)
                                                if ((index >= comp_start_2) && (comp_stop_2 >= index))
                                                    test_history[index] <= 1'b0;
                                   end
                state_apply_test_3 : begin
                                        if (! threshold_pass[1])
                                            for (index=0; index<=288; index=index+1)
                                                if ((index >= comp_start_3) && (comp_stop_3 >= index))
                                                    test_history[index] <= 1'b0;
                                   end
                default : begin end
            endcase
    end




    always @(posedge clk) begin
        case (main_state)
            state_mem_write_0 : begin
                                    RAM_in_A <= test_history[ 35:  0];
                                    RAM_in_B <= test_history[ 71: 36];
                                end
            state_mem_write_1 : begin
                                    RAM_in_A <= test_history[107: 72];
                                    RAM_in_B <= test_history[143:108];
                                end
            state_mem_write_2 : begin
                                    RAM_in_A <= test_history[179:144];
                                    RAM_in_B <= test_history[215:180];
                                end
            state_mem_write_3 : begin
                                    RAM_in_A <= test_history[251:216];
                                    RAM_in_B <= test_history[287:252];
                                end
            default : begin end
        endcase
    end


/*
 ######  ##    ## ######## ######  ##    ## ########
##    ## ##    ##    ##    ##   ## ##    ##    ##
##    ## ##    ##    ##    ######  ##    ##    ##
##    ## ##    ##    ##    ##      ##    ##    ##
 ######   ######     ##    ##       ######     ##
*/


    always @(posedge clk) begin
        if (reset)                                spike_detected <= 1'b0;
        else if (main_state == state_mem_write_0) spike_detected <= test_history[final_decision_sample];
    end

    always @(posedge clk) begin
        if (reset) begin
            chan_out_num <= 0;
            output_full  <= 1'b0;
        end else if (output_full && chan_out_read) begin
            // the next module is reading the event ...
            // there is no problem to shadow the next test, because if output_full
            // is true, spike_detected is False or the state machine is stalled
            // before reaching state_mem_write_2
            output_full  <= 1'b0;            
        end else if (main_state == state_mem_write_2 && spike_detected) begin
            // a new event is output
            chan_out_num <= processed_channel;
            output_full  <= 1'b1;
        end
    end

    assign chan_out_valid = output_full;




endmodule