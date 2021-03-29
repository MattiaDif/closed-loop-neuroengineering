//////////////////////////////////////////////////////////////////////////////////
// Company: 		 IMS lab - Univ. Bordeaux - Bordeaux INP
// 
// Design Name: 	 RHD2000 Rhythm USB3 Interface computation complement
// Module Name:    CUST_HP_filter
// Project Name:   Stimulation controller
// Target Devices: 
// Tool versions: 
// Description:    
//
// Dependencies: 
//
// Revision:       0.1.1 (August 8th 2018)
// Additional Comments: this module is bogus if using less than 5 channels at
//                      maximal speed (not sure it is necessary to fix this)
//
//
//////////////////////////////////////////////////////////////////////////////////

module CUST_HP_filter #(
	parameter CHANNELS 	   = 128, // sb was 128 now 160
    parameter CHANNELS_PW2 = 7		// sb was 7 now 8
	)
	(
	input  wire                      clk,
	input  wire                      reset,
    input  wire             [15 : 0] chan_in_sample,
    input  wire [CHANNELS_PW2-1 : 0] chan_in_num,
    input  wire                      chan_in_valid,
    output wire                      chan_in_read, //  tells if new sample is read by next module    
    output wire             [15 : 0] chan_out_sample,
    output wire [CHANNELS_PW2-1 : 0] chan_out_num,
    output wire                      chan_out_valid,
    input  wire                      chan_out_read, // now is always 1 as assigned by spike detection module
    
    input  wire             [15 : 0] coeff
   );




/*
######  #######  ###### ##       #####  ######   #####  ######## ##  ######  ###    ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ####   ## ##
##   ## #####   ##      ##      ####### ######  #######    ##    ## ##    ## ## ##  ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ##  ## ##      ##
######  #######  ###### ####### ##   ## ##   ## ##   ##    ##    ##  ######  ##   #### #######
*/


    reg [31:0]  prev_sample_storage[0:CHANNELS-1]; // this memory is used to store data from the input
    reg [CHANNELS_PW2-1 : 0] reset_chan;     // used to reset the filters
    

    reg [CHANNELS_PW2-1 : 0] stage0_number;  // the number of the channel being processed
    reg             [15 : 0] stage0_sample;  // the value of the channel
    reg                      stage0_valid;   // True if data is meaningful

    reg [CHANNELS_PW2-1 : 0] stage1_number;  // the number of the channel being processed
    reg             [15 : 0] stage1_sample;  // the value of the channel
    reg             [31 : 0] stage1_oldval;  // the value of Y(n-1)
    reg                      stage1_valid;   // True if data is meaningful
    
    reg [CHANNELS_PW2-1 : 0] stage2_number;  // the number of the channel being processed
    reg signed      [17 : 0] stage2_multin;  // sample - oldvalue with overflow tests
    reg             [31 : 0] stage2_oldval;  // the value of Y(n-1)
    reg                      stage2_valid;   // True if data is meaningful
    
    reg [CHANNELS_PW2-1 : 0] stage3_number;  // the number of the channel being processed
    reg signed      [35 : 0] stage3_multout; // (sample - oldvalue)*coeff
    reg             [31 : 0] stage3_oldval;  // the value of Y(n-1)
    reg                      stage3_valid;   // True if data is meaningful

    reg [CHANNELS_PW2-1 : 0] stage4_number;  // the number of the channel being processed
    reg             [31 : 0] stage4_newval;  // the value of Y(n-1)
    reg                      stage4_valid;   // True if data is meaningful


    wire            [18:0]   difference;
    wire            [35:0]   multiplier_out;
    wire signed     [17:0]   coeff_multin;






/*
 ######  ######  ###    ### ######  ##    ## ########  #####  ######## ##  ######  ###    ##
##      ##    ## ####  #### ##   ## ##    ##    ##    ##   ##    ##    ## ##    ## ####   ##
##      ##    ## ## #### ## ######  ##    ##    ##    #######    ##    ## ##    ## ## ##  ##
##      ##    ## ##  ##  ## ##      ##    ##    ##    ##   ##    ##    ## ##    ## ##  ## ##
 ######  ######  ##      ## ##       ######     ##    ##   ##    ##    ##  ######  ##   ####
*/


    assign chan_in_read = (!stage1_valid || !stage2_valid || chan_out_read) && (reset_chan==0);
    
    always @(posedge clk) begin
        if      (reset)                  reset_chan <= 1;
        else if (reset_chan == CHANNELS) reset_chan <= 0;
        else if (reset_chan != 0)        reset_chan <= reset_chan + 1; 
    end



    // this stage is not performing any computation, i is just used to cut an
    // eventual critical path, and make it possible to use BRAMs for
    // internal memory

    always @(posedge clk) begin
        if (reset)
            stage0_valid <= 1'b0;
        else if (!stage0_valid || !stage1_valid || !stage2_valid || chan_out_read) begin
            // see syage 1 for explanations
            stage0_number  <= chan_in_num;
            stage0_valid   <= chan_in_valid;
            stage0_sample  <= chan_in_sample;
        end
    end



    // first stage of the pipeline...
    // we record data arriving, and we retreive the y(n-1) value from the local memory...

    always @(posedge clk) begin
        if (reset)
            stage1_valid <= 1'b0;
        else if (!stage1_valid || !stage2_valid || chan_out_read) begin
            // we only perform this computing if the stage1 or stage2 are free (stage?_valid==0)
            // or if stage2 is read (chan_out_read == 1). if data is read at stage 2, the
            // pipeline data will shift on stage ahead freeing stage 1. If stage 2 is free,
            // stage 1 will progress to stage 2 and stage 1 will be free....
            
            stage1_number  <= stage0_number;
            stage1_oldval  <= prev_sample_storage[stage0_number];
            stage1_valid   <= stage0_valid;
            stage1_sample  <= { ~stage0_sample[15], stage0_sample[14:0]};
                // original data was unsigned centered on 0x8000,
                // we switch to signed centered on 0
        end
    end



    assign difference = { stage1_sample, 2'b00 } - stage1_oldval[31:14];

    // second stage of the pipeline...
    // data arrives in these regs two clock cycles after entering the module
    // we compute the difference and overflow checks
    always @(posedge clk) begin
        if (reset)
            stage2_valid <= 1'b0;
        else if (!stage2_valid || chan_out_read) begin
        // we only perform this computing if the next stage is free (stage2_valid==0)
        // or if stage2 is read (chan_out_read == 1, it means that the stage may
        // accept new data for the next clock cycle)
        
            stage2_number  <= stage1_number;
            stage2_valid   <= stage1_valid;
            stage2_oldval  <= stage1_oldval;

            if      ( stage1_sample[15] && ~stage1_oldval[31] && ~difference[17])       stage2_multin <= 18'h20000; // negative overflow
            else if (~stage1_sample[15] &&  stage1_oldval[31] &&  difference[17])       stage2_multin <= 18'h1FFFF; // positive overflow
            else                                                                        stage2_multin <= difference;
        end
    end


    assign chan_out_sample = {~stage2_multin[17], stage2_multin[16:2]}; // we revert the output as original format
    assign chan_out_num    = stage2_number;
    assign chan_out_valid  = stage2_valid;

    // beyond this point, there is no need to manage free or empty stages because there
    // is no more blocking step.



    assign coeff_multin={1'b0, coeff, 1'b0};

    // third stage of the pipeline...
    // we just retreive the output of the multiplier, we only do this in this clock
    // period but multipliers are complex device that require some time to compute...
    // if doing something else, it might slow down the critical path (and the whole system)
    always @(posedge clk) begin
        if (chan_out_read && stage2_valid) begin
        // we only propagate the next steps if the data has actually been read,
        // this may spare some electrical power and costs very few hardware resource
            stage3_number  <= stage2_number;
            stage3_oldval  <= stage2_oldval;
            //stage3_multout <= multiplier_out;
            stage3_multout <= stage2_multin*coeff_multin;
        end
        stage3_valid   <= stage2_valid & chan_out_read; 
    end

    // fourth stage of the pipeline...
    // the final operation is the subtraction to get the new value to store in memory
    // this operation is rather fast, but it is not possible to do it asychronously
    // because output register stage4_newval is memory input and is likely to be
    // absorbed by the RAM module (if not, it might not be possible to implement
    // the memory in RAM block which is slow and resource-consumming)
    always @(posedge clk) begin
        if (reset) begin
            stage4_number <= 0;
            stage4_valid  <= 1'b1;
            stage4_newval <= 32'h0;
        end else  if (reset_chan != 0) begin
            stage4_number <= reset_chan;
            stage4_valid  <= 1'b1;
            stage4_newval <= 32'h0;
        end else begin
            stage4_number <= stage3_number;
            stage4_valid  <= stage3_valid;
            stage4_newval <= stage3_oldval + stage3_multout[34:3];
        end
    end

    


    // store the result in memory
    // that's all folks !!
    always @(posedge clk) begin
        if (stage4_valid) prev_sample_storage[stage4_number] <= stage4_newval;
    end



endmodule