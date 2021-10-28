//////////////////////////////////////////////////////////////////////////////////
// Company: 		 IMS lab - Univ. Bordeaux - Bordeaux INP
// 
// Design Name: 	 RHD2000 Rhythm USB3 Interface computation complement
// Module Name:    CUST_ARCH 
// Project Name:   Stimulation controller
// Target Devices: 
// Tool versions: 
// Description:    
//
// Dependencies: 
//
// Additional Comments: The number of channels managed by this module cannot be changed
//                      because it is closely related to the existing hardware it runs
//                      on. Submodules should be configurable in terms of channel number
//
// How to improve:
//       computation actually waits until all channels have been received, we could start
//       earlier to reduce computation latency...
//
//
// TODO:
//      - data_from_source and read_source are X'ed. might be a consequence of point (2)
//        otherwise, check these...
//              => might also come from the filter module
// FIXED:
//      - there are 8 DAC/ADC values instead of 1
//      - samples have a 3 cycle shift !!!
//      - totally rebuilt the way output packets were produced
//      - added an output debug-specific process to identify data flows
//      
//
//////////////////////////////////////////////////////////////////////////////////

module CUST_ARCH #(
    parameter CLKFREQ_MHZ           = 100,      // system frequency in MHz
    parameter TRIGGER_NUM           = 8,        // number of trigger channels
    parameter RESP_FIFO_SIZE        = 10,       // size of the FIFO that contains instruction responses
    parameter BOARD_ID              = 16'h0000, // board ID from the main project
    parameter BOARD_VERSION         = 16'h0000, // board version from the main project
    parameter HEADER_MAGIC_NUMBER   = 64'h0     // data stream magic number
	)
	(
    input wire                      clk,
    input wire                      reset,
    input wire                      FIFO_write_to,
    input wire  [15           :0]   FIFO_data_in,
    output reg                      comp_data_out_valid,
    output reg  [15           :0]   comp_data_out,
    output reg  [TRIGGER_NUM-1:0]   trig_stim,
    output reg                      computing,
    input wire  [15           :0]   pipe_in_data,
    input wire                      pipe_in_en,
    output reg  [15           :0]   pipe_out_data,
    input wire                      pipe_out_read,
    input wire  [15           :0]   cfg,
    input wire  [ 7           :0]   data_stream_en,
    output wire [15           :0]   status
   );





/*
######  #######  ###### ##       #####  ######   #####  ######## ##  ######  ###    ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ####   ## ##
##   ## #####   ##      ##      ####### ######  #######    ##    ## ##    ## ## ##  ## #######
##   ## ##      ##      ##      ##   ## ##   ## ##   ##    ##    ## ##    ## ##  ## ##      ##
######  #######  ###### ####### ##   ## ##   ## ##   ##    ##    ##  ######  ##   #### #######
*/

    localparam LOC_RESP_FIFO_SIZE = (RESP_FIFO_SIZE <1024) ? RESP_FIFO_SIZE : 1024;
    localparam VERSION_MAJOR      = 16'h0001;
    localparam VERSION_MINOR      = 16'h0003;


    reg  [ 2:0]  source_device;      // the spi device that provided the data
    wire [ 3:0]  inc_source_device;  // the spi device that is expected to provide next data
    reg  [ 4:0]  device_output;      // the device channel
    wire [ 7:0]  source_channel;     // concatenation of the two values above to get a unique identifier
    reg  [ 3:0]  next_source_incr;   // this reg will be added to source_device when required to switch to
                                     // the next device. All 8 devices are not necessarily connected
    reg  [ 7:0]  chan_en_rollmap;    // a map of the next 8 device presence (relative to current device)
    reg  [31:0]  r_timestamp;        // the timestamp of data being received 
    //reg  [31:0]  s_timestamp;        // the timestamp of data to send
    reg  [ 2:0]  converter_cnt;      // the counter for ADC and DAC samples
    
    integer      main_state;         // this reg will contain the actual state of incoming data


    // the following states for the machine identify the data that is expected on next write
    localparam  state_idle           =  0,  // only used for output_state
                state_magic_number_0 =  1,
                state_magic_number_1 =  2,
                state_magic_number_2 =  3,
                state_magic_number_3 =  4,
                state_timestamp_0    =  5,
                state_timestamp_1    =  6,
                state_DC_sample      =  7,  // data_stream_en dependant, only 10 lower bits meaningful
                state_chan_sample    =  8,  // data_stream_en dependant
                state_stim_on_off    =  9,  // data_stream_en dependant
                state_stim_pol       = 10,  // data_stream_en dependant
                state_amp_settle     = 11,  // data_stream_en dependant
                state_charge_recov   = 12,  // data_stream_en dependant
                state_DAC            = 13,
                state_ADC            = 14,
                state_TTL_in         = 15,
                state_TTL_out        = 16,
                state_processing     = 17;  // only used for output_state

    reg [ 8:0]  input_write_addr;       // the address at which we will store data
    reg [15:0]  input_write_data;       // the data that will be stored
    reg         input_write_enable;     // the write order input data
    reg [ 8:0]  input_read_addr;        // the address at which we will read original data
    reg [15:0]  input_read_data;        // the data that has been retrieved from memory
    // we actually use two memories because we will need two independant read accesses
    // technically, we do not need to get two big memories (input_storage_c could only contain sample data)
    // but FPGA primitives will be the same (16kbit BRAM), so there is no need to add complexity in order to
    // save no resources...
    reg [15:0]  input_storage_p[0:511];   // this memory is used to store data from the input, to send it back 
    reg [15:0]  input_storage_c[0:511];   // this memory is used to store data from the input, as a source for computation
    
    
    // these signals are use to build the packets in output
    reg [ 6:0]  output_write_addr;       // the address at which we store data fro computation 						// sb increased to fit 160 values
    reg [15:0]  output_write_data;       // the data to output as acquired signal
    reg         output_write_enable;     // the write order for output data
    reg [ 6:0]  output_read_addr;        // the address at which we will read resulting samples 					// sb increased to fit 160 values
    reg [15:0]  output_read_data;        // the data that has been retrieved from memory
    reg [15:0]  output_storage[0:127];   // this memory is used to store data from the computing algorithm 		//sb it was 127. now 159

    integer     output_state;            // this stores the state to build the output packet
    reg [ 1:0]  output_fsm_subsampler;   // this is used to make output_state change each 4 clock edge
                                         // useful to get time to retrieve data from memories
    reg  [ 7:0]  output_en_rollmap;      // a map of the next 8 device presence (relative to current device)
    reg  [ 2:0]  output_device_ID;       // the device we are adressing
    reg  [ 4:0]  output_device_channel;  // the device channel
    wire [ 7:0]  output_channel;         // concatenation of the two values above to get a unique identifier

    // computation related registers
    reg  [ 1:0] next_cmd_word_num;
    reg  [15:0] cmd_words[2:0];


    reg [15:0] filter_coeff;



    reg  [7:0] trigg_basetime;                  // the counter that provides system microseconds for trigger duration
                                                // 8 bits is more than required today because we run at 100MHz, this is
                                                // just to anticipate higher frequencies.
    reg [15:0] trig_counter[0:TRIGGER_NUM-1];   // used to decount the number of microseconds for trigger
    reg [15:0] trig_length;                     // the time to decount in microseconds ()
    reg [15:0] trig_source [0:TRIGGER_NUM-1];   // what is the event source to use for each trigger ?


    reg [TRIGGER_NUM-1:0] trig_enable;          // should the corresponding trigger be active ?

//sb
	 reg [ 1:0]  input_fsm_subsampler;   

    // ************************************
    // * Parameters for spike detection 0 *
    // ************************************
    reg [15 : 0] spkdet_0_threshold_0;
    reg [15 : 0] spkdet_0_startstop_0;
    reg          spkdet_0_above_0;
    reg          spkdet_0_enable_0;
    reg [15 : 0] spkdet_0_threshold_1;
    reg [15 : 0] spkdet_0_startstop_1;
    reg          spkdet_0_above_1;
    reg          spkdet_0_enable_1;
    reg [15 : 0] spkdet_0_threshold_2;
    reg [15 : 0] spkdet_0_startstop_2;
    reg          spkdet_0_above_2;
    reg          spkdet_0_enable_2;
    reg [15 : 0] spkdet_0_threshold_3;
    reg [15 : 0] spkdet_0_startstop_3;
    reg          spkdet_0_above_3;
    reg          spkdet_0_enable_3;
    reg [ 7 : 0] spkdet_0_finalreport;



    // response fifo registers
    reg [15:0]  pipe_out_FIFO[0:LOC_RESP_FIFO_SIZE-2];  // this memory is used to store responses
                                                        // to commands. its size is one cell less
                                                        // than requesed because there is an output
                                                        // reg.
    reg [ 9:0]  write_addr;                             // the address to write the new data
    reg [ 9:0]  read_addr;                              // the address where to read the next data
    reg [ 9:0]  pipe_out_data_size;                     // the number of elements in the FIFO
    
    reg [15:0]  pipe_out_FIFO_data;                     // the data to write to FIFO
    reg         pipe_out_FIFO_write;                    // the write order to FIFO
    reg         pipe_out_valid;                         // true when the output is ready for read
    
    reg [15:0]  last_err;                               // the last error detected by the system

    // internal loop counter
    integer index;
	 
	 // sb adding 
	 reg [ 9:0]  input_addr_AC ; 								  // to split the concatenation

/*
###    ###  #####  ## ###    ##     ####### ####### ###    ###
####  #### ##   ## ## ####   ##     ##      ##      ####  ####
## #### ## ####### ## ## ##  ##     #####   ####### ## #### ##
##  ##  ## ##   ## ## ##  ## ##     ##           ## ##  ##  ##
##      ## ##   ## ## ##   ####     ##      ####### ##      ##


input channel and device counting is also part of the FSM. They are managed just
after main_state
*/




    always @(posedge clk) begin
        if (reset) begin
            main_state <= state_magic_number_0;
        end else begin
            case (main_state)
                state_magic_number_0: begin
                    if (FIFO_write_to) begin
                        // if the next test is wrong, then we did not receive the
                        // expected data, therefore we stay in the same state
                        if (FIFO_data_in == HEADER_MAGIC_NUMBER[15: 0])
                            main_state <= state_magic_number_1;
                    end
                end

                state_magic_number_1: begin
                    if (FIFO_write_to) begin
                        if (FIFO_data_in == HEADER_MAGIC_NUMBER[31:16]) 
                            main_state <= state_magic_number_2;
                        else
                            // here, the received data does not correspond to the expected magic number...
                            // so we should not go further and start again from the beginning
                            main_state <= state_magic_number_0;
                    end
                end

                state_magic_number_2: begin
                    if (FIFO_write_to) begin
                        if (FIFO_data_in == HEADER_MAGIC_NUMBER[47:32])
                            main_state <= state_magic_number_3;
                        else
                            // here, the received data does not correspond to the expected magic number...
                            // so we should not go further and start again from the beginning
                            main_state <= state_magic_number_0;
                    end
                end

                state_magic_number_3: begin
                    if (FIFO_write_to) begin
                        if (FIFO_data_in == HEADER_MAGIC_NUMBER[63:48])
                            main_state <= state_timestamp_0;
                        else
                            // here, the received data does not correspond to the expected magic number...
                            // so we should not go further and start again from the beginning
                            main_state <= state_magic_number_0;
                    end
                end
                
                // so far, we passed the magic number test, so we trust incoming data...

                state_timestamp_0: begin if (FIFO_write_to)                           main_state <= state_timestamp_1;    end
                state_timestamp_1: begin if (FIFO_write_to)                           main_state <= state_DC_sample;      end
                state_DC_sample:   begin if (FIFO_write_to)                           main_state <= state_chan_sample;    end

                state_chan_sample: begin if (FIFO_write_to) begin
                            if (inc_source_device[3] && device_output == 5'd19  )     main_state <= state_stim_on_off; // sb needs to be 19
                            else                                                      main_state <= state_DC_sample;      end
                         end
                
                state_stim_on_off:  begin if (FIFO_write_to && inc_source_device[3])  main_state <= state_stim_pol;       end
                state_stim_pol:     begin if (FIFO_write_to && inc_source_device[3])  main_state <= state_amp_settle;     end
                state_amp_settle:   begin if (FIFO_write_to && inc_source_device[3])  main_state <= state_charge_recov;   end
                state_charge_recov: begin if (FIFO_write_to && inc_source_device[3])  main_state <= state_DAC;            end                
                state_DAC:          begin if (FIFO_write_to && converter_cnt==3'b111) main_state <= state_ADC;            end
                state_ADC:          begin if (FIFO_write_to && converter_cnt==3'b111) main_state <= state_TTL_in;         end
                state_TTL_in:       begin if (FIFO_write_to)                          main_state <= state_TTL_out;        end
                state_TTL_out:      begin if (FIFO_write_to)                          main_state <= state_magic_number_0; end
                default:            begin                                             main_state <= state_magic_number_0; end
            
            endcase

        end
    end


    // the chan_en_rollmap makes rotations on the data_stream_en value. Therefore, even if we have no clear
    // view of the actual device number, we know if the next devices are used or not. Doing this, we know
    // how we should increase source_device. This increment is stored in next_source_incr.
    // the operation is performed in advance because one clock cycle latency is necessary to propagate to
    // the actual increment of source_device. On the late states the value still evolves, but we do not use
    // source_device any more, so it does not matter
    // this structure is usefull to accept sample at a higher rate.

    always@(posedge clk) begin
        if (main_state==state_magic_number_3) chan_en_rollmap  <= data_stream_en;
        else if (FIFO_write_to && ((main_state != state_chan_sample && main_state != state_timestamp_1) || (inc_source_device[3] && device_output == 5'd19))) begin
            if (chan_en_rollmap[6:0]==7'b0000000) begin
                chan_en_rollmap  <= chan_en_rollmap;        // this line is actually useless, but the case must be managed to avoid to fall in other cases...
            end else if (chan_en_rollmap[5:0]==6'b000000) begin
                chan_en_rollmap  <= {7'b1000000, chan_en_rollmap[7]};
            end else if (chan_en_rollmap[4:0]==5'b00000)  begin
                chan_en_rollmap  <= {6'b100000,  chan_en_rollmap[7:6]};
            end else if (chan_en_rollmap[3:0]==4'b0000)   begin
                chan_en_rollmap  <= {5'b10000,   chan_en_rollmap[7:5]};
            end else if (chan_en_rollmap[2:0]==3'b000)    begin
                chan_en_rollmap  <= {4'b1000,    chan_en_rollmap[7:4]};
            end else if (chan_en_rollmap[1:0]==2'b00)     begin
                chan_en_rollmap  <= {3'b100,     chan_en_rollmap[7:3]};
            end else if (chan_en_rollmap[0]  ==1'b0)      begin
                chan_en_rollmap  <= {2'b10,      chan_en_rollmap[7:2]};
            end else begin
                chan_en_rollmap  <= {1'b1,       chan_en_rollmap[7:1]};
            end
        end
    end



    always@(posedge clk) begin
        if (reset) begin
            next_source_incr <= 4'h0;
        end else begin
            case (main_state)
                state_magic_number_0, state_magic_number_1, state_magic_number_2 :begin // : begin
                    next_source_incr <= 4'h0;
                end // sb commented this...

                state_DC_sample : begin end // sb what's this?
                
                default :
                    if (FIFO_write_to) begin
                        if (chan_en_rollmap[6:0]==7'b0000000) begin
                            next_source_incr <= 4'h8;
                        end else if (chan_en_rollmap[5:0]==6'b000000) begin
                            next_source_incr <= 4'h7;
                        end else if (chan_en_rollmap[4:0]==5'b00000)  begin
                            next_source_incr <= 4'h6;
                        end else if (chan_en_rollmap[3:0]==4'b0000)   begin
                            next_source_incr <= 4'h5;
                        end else if (chan_en_rollmap[2:0]==3'b000)    begin
                            next_source_incr <= 4'h4;
                        end else if (chan_en_rollmap[1:0]==2'b00)     begin
                            next_source_incr <= 4'h3;
                        end else if (chan_en_rollmap[0]  ==1'b0)      begin
                            next_source_incr <= 4'h2;
                        end else begin
                            next_source_incr <= 4'h1;
                        end
                    end
            endcase
        end
    end


    // we identify channels by a concatenation of device input and the source device
    // so : channel 0 is source 0 of device 0
    //      channel 1 is source 0 of device 1
    //      ...
    assign source_channel = {device_output, source_device};

    always @(posedge clk) begin
        if (reset) begin
            source_device <= 3'b111; //sb it was 111
            device_output <= 5'b00000;
        end else begin
            if (main_state == state_timestamp_0) begin
                source_device <= 3'b111; //sb it was 111
                device_output <= 5'b00000;  // recently added, might bring new issues... we anticipate the 3 aux data before samples //sb was 10000
            end else if ((main_state ==  state_chan_sample || main_state ==  state_magic_number_3) && FIFO_write_to) begin // sb it was state_chan_sample main_state ==  state_chan_sample || main_state ==  state_magic_number_3
                // source identification is initialized 1 data in advance, and state_magic_number_3 is considered making counting progress
                // this is quite useful to identify the first sample without specific circuitry. otherwise we start with device 0, which
                // is not correct is device 0 is not connected.
                source_device <= inc_source_device[2:0];
                if (inc_source_device[3]) begin
                    if (device_output < 5'd19) begin
                        device_output <= device_output + 1;
                    end else begin
                        device_output <= 5'b00000;
                    end
                end
            end else if (main_state != state_DC_sample && FIFO_write_to) begin
                source_device <= inc_source_device[2:0];
            end
        end
    end

    assign inc_source_device = source_device + next_source_incr;


    // this counter handles identification of DAC and ADC.
    // we are not actually using source_device to avoid a complex incrementation scheme
    always @(posedge clk) begin
        if (reset) begin
            converter_cnt <= 3'b000;
        end else if (main_state == state_charge_recov) begin // this case is theoritically useless
            converter_cnt <= 3'b000;                         // but it ensures robustness (just in case)
        end else if (((main_state == state_DAC) || (main_state == state_ADC)) && (FIFO_write_to)) begin
            converter_cnt <= converter_cnt + 1;
        end
    end




/*
## ###    ## ######  ##    ## ########     ####### ########  ######  ######   #####   ######  #######
## ####   ## ##   ## ##    ##    ##        ##         ##    ##    ## ##   ## ##   ## ##       ##
## ## ##  ## ######  ##    ##    ##        #######    ##    ##    ## ######  ####### ##   ### #####
## ##  ## ## ##      ##    ##    ##             ##    ##    ##    ## ##   ## ##   ## ##    ## ##
## ##   #### ##       ######     ##        #######    ##     ######  ##   ## ##   ##  ######  #######

The next lines will store incoming data to the appropriate areas
*/


    always @(posedge clk) begin
        if (reset) begin
            r_timestamp <= 32'h00000000;
//            S_timestamp <= 32'h00000000;
        end else begin
            if (main_state==state_timestamp_0 && FIFO_write_to) begin
                r_timestamp[15: 0] <= FIFO_data_in;
            end
            if (main_state==state_timestamp_1 && FIFO_write_to) begin
                r_timestamp[31:16] <= FIFO_data_in;
            end
//            if (main_state==state_magic_number_3 && FIFO_write_to) begin
//                s_timestamp <= r_timestamp;
//            end            
        end
    end



    always @(posedge clk) begin
        if (reset) begin
            input_write_data   <= 16'h0000;
            input_write_addr   <=  9'h000;
            input_write_enable <=  1'b0;
        end else begin
            input_write_data   <= FIFO_data_in;
            case (main_state)
                state_DC_sample:    begin   input_write_addr   <= {source_channel, 1'b0};
                                            input_write_enable <= FIFO_write_to;          end
                state_chan_sample:  begin   input_write_addr   <= {source_channel, 1'b1};
                                            input_write_enable <= FIFO_write_to;          end
                state_stim_on_off:  begin   input_write_addr   <= {6'b101000, source_device};   // data stored from addresses 140 to 147
                                            input_write_enable <= FIFO_write_to;          end
                state_stim_pol:     begin   input_write_addr   <= {6'b101001, source_device};   // data stored from addresses 148 to 14F
                                            input_write_enable <= FIFO_write_to;          end
                state_amp_settle:   begin   input_write_addr   <= {6'b101010, source_device};   // data stored from addresses 150 to 157
                                            input_write_enable <= FIFO_write_to;          end
                state_charge_recov: begin   input_write_addr   <= {6'b101011, source_device};   // data stored from addresses 158 to 15F
                                            input_write_enable <= FIFO_write_to;          end
                state_DAC:          begin   input_write_addr   <= {6'b101100, converter_cnt};   // data stored from addresses 160 to 167
                                            input_write_enable <= FIFO_write_to;          end
                state_ADC:          begin   input_write_addr   <= {6'b101101, converter_cnt};   // data stored from addresses 168 to 16F
                                            input_write_enable <= FIFO_write_to;          end
                state_TTL_in:       begin   input_write_addr   <= 9'b101110000;                 // data stored at 170
                                            input_write_enable <= FIFO_write_to;          end
                state_TTL_out:      begin   input_write_addr   <= 9'b101110001;                 // data stored at 171
                                            input_write_enable <= FIFO_write_to;          end
                default:            begin   input_write_enable <= 1'b0;                   end
            endcase
        end
    end




    always @(posedge clk) begin
        if (input_write_enable) begin // sb was input_write_enable
            input_storage_c[input_write_addr] <= input_write_data;
            input_storage_p[input_write_addr] <= input_write_data;
        end
        input_read_data <= input_storage_p[input_read_addr];
    end




/*
######  ##    ## ## ##      ######  ## ###    ##  ######      ###    ## ####### ##     ##      ######  ##    ## ######## ######  ##    ## ########
##   ## ##    ## ## ##      ##   ## ## ####   ## ##           ####   ## ##      ##     ##     ##    ## ##    ##    ##    ##   ## ##    ##    ##
######  ##    ## ## ##      ##   ## ## ## ##  ## ##   ###     ## ##  ## #####   ##  #  ##     ##    ## ##    ##    ##    ######  ##    ##    ##
##   ## ##    ## ## ##      ##   ## ## ##  ## ## ##    ##     ##  ## ## ##      ## ### ##     ##    ## ##    ##    ##    ##      ##    ##    ##
######   ######  ## ####### ######  ## ##   ####  ######      ##   #### #######  ### ###       ######   ######     ##    ##       ######     ##

To build the new packet in output, we can take our time :)
contrary to the input where we must always be ready to receive new data, we output new data when we want (so when we are ready)
then we take several clock cycles to send a single data, each of them corresponding to a specific command
    - determine address at which we need to retrieve data       output_fsm_subsampler = 00
    - let the memory make affectation to its output             output_fsm_subsampler = 10
    - actually use the output                                   output_fsm_subsampler = 11

should an extra clock period be required, it should still be possible to use the value output_fsm_subsampler = 01    
*/

 
    always @(posedge clk) begin
        computing           <= 1'b0; // sb: apparently useless so far.
    end
    
    assign status[ 9: 0] = pipe_out_data_size; // sb: apparently useless so far.
    assign status[15:10] = 6'h00;					// sb: apparently useless so far.
    

    always @(posedge clk) begin
        if (reset) begin
            output_state <= state_idle;
        end else begin
            case (output_state)
                state_idle           : if (output_write_enable && output_write_addr>0)      output_state <= state_processing;       // no need to be precise on the transition here
                state_processing     : if (output_write_addr==0)                            output_state <= state_magic_number_0;   // processing is over, starting new packet
                state_magic_number_0 :                                                      output_state <= state_magic_number_1;
                state_magic_number_1 :                                                      output_state <= state_magic_number_2;
                state_magic_number_2 :                                                      output_state <= state_magic_number_3;
                state_magic_number_3 :                                                      output_state <= state_timestamp_0;
                state_timestamp_0    :                                                      output_state <= state_timestamp_1;
                state_timestamp_1    :                                                      output_state <= state_DC_sample;
                state_DC_sample      : if (output_fsm_subsampler == 2'b11)                  output_state <= state_chan_sample;
                state_chan_sample    : if (output_fsm_subsampler == 2'b11) begin
                                            if (output_channel == 8'b10011111)              output_state <= state_stim_on_off; // sb it was output_channel == 8'b10000111 equal 135 but it should be 159: 10011111 //sb last put 127 (meaning it's the last 
                                            else                                            output_state <= state_DC_sample;
                                       end
                state_stim_on_off    : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_stim_pol;
                state_stim_pol       : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_amp_settle;
                state_amp_settle     : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_charge_recov;
                state_charge_recov   : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_DAC;
                state_DAC            : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_ADC;
                state_ADC            : if (output_fsm_subsampler == 2'b11 && output_device_ID == 3'b111)    output_state <= state_TTL_in;
                state_TTL_in         : if (output_fsm_subsampler == 2'b11)                  output_state <= state_TTL_out;
                state_TTL_out        : if (output_fsm_subsampler == 2'b11)                  output_state <= state_idle;
                default              : output_state <= state_idle;
            endcase
        end
    end

    // we don't always need to subsample the output...
    // only when we need to get data from memory (we might get some simpler way to code it, but I am not sure that it as efficient in terms of hardware resources)
    always @(posedge clk) begin
        if (reset) begin
            output_fsm_subsampler <= 2'b00; //sb: it was 00
        end else if (output_state == state_DC_sample || output_state == state_chan_sample || output_state == state_stim_on_off  ||
                     output_state == state_stim_pol  || output_state == state_amp_settle  || output_state == state_charge_recov ||
                     output_state == state_DAC       || output_state == state_ADC         || output_state == state_TTL_in       ||
                     output_state == state_TTL_out) begin
            case (output_fsm_subsampler)
//                2'b00   : output_fsm_subsampler <= 2'b10;
//                2'b10   : output_fsm_subsampler <= 2'b11;
//                default : output_fsm_subsampler <= 2'b00; // sb commented these lines
						2'b00   : output_fsm_subsampler <= 2'b01;
						2'b01   : output_fsm_subsampler <= 2'b10;
						2'b10   : output_fsm_subsampler <= 2'b11;
						default : output_fsm_subsampler <= 2'b00;
            endcase
            //output_fsm_subsampler <= output_fsm_subsampler + 1; // not used to avoid loosing time in value '01'
        end
    end

    // we do not need to always be ready, so we just roll output_en_rollmap, if device is not not connected, we will simply do nothing.
    // same thing for device/channel identifier... we pass through all values, output_en_rollmap[0] will tell if it is necessary to
    // output data.
    always @(posedge clk) begin
        if (reset) begin
            output_en_rollmap     <= 8'd0;
            output_device_ID      <= 3'b000;
            output_device_channel <= 5'b00000; // was b10001 by SB
        end else begin
//				if (output_state == state_magic_number_0 || output_state == state_magic_number_1 || output_state == state_magic_number_2  ||
//                output_state == state_magic_number_3 || output_state == state_timestamp_0    || output_state == state_timestamp_1 ) begin
            if (output_state == state_timestamp_1) begin // sb commented this
                output_en_rollmap <= data_stream_en;
                output_device_ID      <= 3'b000;
                output_device_channel <= 5'b00000;  // 10001 start value is 17 to get the 3 aux data for each device
                                                    // no need to anticipate a shift as what is done in input. // sb starts from 136
            end else begin
                //if (output_fsm_subsampler == 2'b11 && output_state == state_chan_sample ||  output_state == state_stim_on_off  ||  output_state == state_stim_pol  || output_state == state_amp_settle  || output_state == state_charge_recov  ) begin
					   if (output_fsm_subsampler == 2'b11 && output_state != state_DC_sample) begin
                    // when in state_DC_sample, we do not increment the identifier, nor roll the output device, because we expect another
                    // data on the same identifier
                    output_en_rollmap <= {output_en_rollmap[0], output_en_rollmap[7:1]};
                    output_device_ID  <= output_device_ID + 1;
                    if (output_device_ID == 3'b111) begin
                        if (output_device_channel < 5'd19)  output_device_channel <= output_device_channel + 1;
                        else                                output_device_channel <= 5'd0;
                    end
                end
            end
        end
    end

    assign output_channel = {output_device_channel, output_device_ID};


    // here is the data we actually output
    always @(posedge clk) begin
        case (output_state)
            state_magic_number_0: comp_data_out <= HEADER_MAGIC_NUMBER[15: 0];
            state_magic_number_1: comp_data_out <= HEADER_MAGIC_NUMBER[31:16];
            state_magic_number_2: comp_data_out <= HEADER_MAGIC_NUMBER[47:32];
            state_magic_number_3: comp_data_out <= HEADER_MAGIC_NUMBER[63:48];
            state_timestamp_0   : comp_data_out <= r_timestamp[15: 0];
            state_timestamp_1   : comp_data_out <= r_timestamp[31:16];
            state_DC_sample     : comp_data_out <= input_read_data;
//				state_DC_sample     : comp_data_out <= 6'h0003;
            state_chan_sample   : if (output_channel[7]) begin comp_data_out <= input_read_data;  end // sb: condition was (output_channel[7])
                                                   else begin comp_data_out <= output_read_data; end
//            state_chan_sample   : comp_data_out <= output_storage[output_read_addr]; // modified by sb .. get always output_read_data // sb  output_storage[output_read_addr]
//				state_chan_sample   : comp_data_out <= 6'h0005; // modified by sb .. get always output_read_data
            state_stim_on_off   : comp_data_out <= input_read_data;
            state_stim_pol      : comp_data_out <= input_read_data;
            state_amp_settle    : comp_data_out <= input_read_data;
            state_charge_recov  : comp_data_out <= input_read_data;
            state_DAC           : comp_data_out <= input_read_data;
            state_ADC           : comp_data_out <= input_read_data;
            state_TTL_in        : comp_data_out <= input_read_data;
            state_TTL_out       : comp_data_out <= input_read_data;
            default             : comp_data_out <= 16'h0005;
        endcase
    end
     

/*    // here is a debug output data scheme to actually check if everything performs normally
    always @(posedge clk) begin
        case (output_state)
            state_magic_number_0: comp_data_out <= HEADER_MAGIC_NUMBER[15: 0];
            state_magic_number_1: comp_data_out <= HEADER_MAGIC_NUMBER[31:16];
            state_magic_number_2: comp_data_out <= HEADER_MAGIC_NUMBER[47:32];
            state_magic_number_3: comp_data_out <= HEADER_MAGIC_NUMBER[63:48];
            state_timestamp_0   : comp_data_out <= r_timestamp[15: 0];
            state_timestamp_1   : comp_data_out <= r_timestamp[31:16];
            state_DC_sample     : comp_data_out <= {1'b0, output_device_ID, 3'b000, output_device_channel, 4'h0};
            state_chan_sample   : comp_data_out <= {1'b0, output_device_ID, 3'b000, output_device_channel, 4'h1};
            state_stim_on_off   : comp_data_out <= {1'b0, output_device_ID, 12'h002};
            state_stim_pol      : comp_data_out <= {1'b0, output_device_ID, 12'h003};
            state_amp_settle    : comp_data_out <= {1'b0, output_device_ID, 12'h004};
            state_charge_recov  : comp_data_out <= {1'b0, output_device_ID, 12'h005};
            state_DAC           : comp_data_out <= {5'b0, output_device_ID,  8'h006};
            state_ADC           : comp_data_out <= {5'b0, output_device_ID,  8'h007};
            state_TTL_in        : comp_data_out <= 16'h0008;
            state_TTL_out       : comp_data_out <= 16'h0009;
            default             : comp_data_out <= 16'h0000;
        endcase
    end
*/

    always @(posedge clk) begin
        if (reset) begin
            comp_data_out_valid <= 1'b0;
        end else begin
            if (output_state == state_DC_sample || output_state == state_chan_sample || output_state == state_stim_on_off  ||
                output_state == state_stim_pol  || output_state == state_amp_settle  || output_state == state_charge_recov) begin
                // in all these states, we only output data if : we are in the last period of the state (output_fsm_subsampler == 2b'11)
                // and the corresponding device is connected (output_en_rollmap[0] == 1)
                if (output_fsm_subsampler == 2'b11)
                    comp_data_out_valid <= output_en_rollmap[0];
                else
                    comp_data_out_valid <= 1'b0;
            end else begin
                if (output_state == state_DAC || output_state == state_ADC || output_state == state_TTL_in || output_state == state_TTL_out) begin
                    // in these states we do not rely on a device that could be connected or not... we just have to care about when we should 
                    // activate the output strobe
                    if (output_fsm_subsampler == 2'b11)
                        comp_data_out_valid <= 1'b1;
                    else
                        comp_data_out_valid <= 1'b0;                   
                end else begin
                    if (output_state == state_magic_number_0 || output_state == state_magic_number_1 || output_state == state_magic_number_2  ||
                         output_state == state_magic_number_3 || output_state == state_timestamp_0    || output_state == state_timestamp_1 ) begin
                        // in these states, we do not need to retrieve data from memories, they are immediately available so we do not
                        // subsample. we always have to output somehting so : always 1.
                        comp_data_out_valid <= 1'b1;
                    end else begin
                        comp_data_out_valid <= 1'b0;
                    end
                end
            end
        end
    end

// the address at which we read data. if we want output_read_addr to be ready when output_fsm_subsampler = 10,
    // output_channel must be meaningful when output_fsm_subsampler = 01
    always @(posedge clk) begin
        if (reset) begin
            output_read_addr <= 7'h00; // sb it was 7'h00; now 8
        end else begin
            case (output_state)
                state_DC_sample     : output_read_addr <= output_channel[6:0]; // sb: limiting the address to output_channel[6:0]; mean only 128 positions 
                state_chan_sample   : output_read_addr <= output_channel[6:0];
                default             : output_read_addr <= 7'h00; 				// sb it was 7'h00; now 8
            endcase 
        end
    end

    // the address at which we read data. if we want output_read_addr to be ready when output_fsm_subsampler = 10,
    // output_channel must be meaningful when output_fsm_subsampler = 01
    always @(posedge clk) begin
        if (reset) begin
            input_read_addr <= 9'h000;
        end else begin
            case (output_state)
                state_DC_sample     : input_read_addr <= {output_channel, 1'b0};  // sb it was output_read_data
                state_chan_sample   : input_read_addr <= {output_channel, 1'b1};  // we do not use this one. it's only here to make it easier to modify // sb it was output_read_data
                state_stim_on_off   : input_read_addr <= {6'b101000, output_device_ID};
                state_stim_pol      : input_read_addr <= {6'b101001, output_device_ID};
                state_amp_settle    : input_read_addr <= {6'b101010, output_device_ID};
                state_charge_recov  : input_read_addr <= {6'b101011, output_device_ID};
                state_DAC           : input_read_addr <= {6'b101100, output_device_ID};
                state_ADC           : input_read_addr <= {6'b101101, output_device_ID};
                state_TTL_in        : input_read_addr <= 9'h170;
                state_TTL_out       : input_read_addr <= 9'h171;
                default             : input_read_addr <= 9'h000;
            endcase 
        end
    end


     
     // these signals determine the module used to output data
     // update them to change the stream to output to the computer
     // current configuration is to listen to the HP filter
    always @(posedge clk) begin
        output_write_data   <= data_from_filter;
        output_write_enable <= valid_from_filter && read_from_filter;
        output_write_addr   <= chan_from_filter;
    end
    
    // the memory that contain data from the module listened to. if we want output_read_data to be meaningful when output_fsm_subsampler = 11,
    // output_read_addr must be meaningful when output_fsm_subsampler = 10,
    always @(posedge clk) begin
        if (output_write_enable) begin
            output_storage[output_write_addr] <= output_write_data;
        end
        output_read_data    <= output_storage[output_read_addr];
    end
    
    




/*
 ######  ######  ###    ### ######  ##    ## ########  #####  ######## ##  ######  ###    ##     #######  ######  ##    ## ######   ###### #######
##      ##    ## ####  #### ##   ## ##    ##    ##    ##   ##    ##    ## ##    ## ####   ##     ##      ##    ## ##    ## ##   ## ##      ##
##      ##    ## ## #### ## ######  ##    ##    ##    #######    ##    ## ##    ## ## ##  ##     ####### ##    ## ##    ## ######  ##      #####
##      ##    ## ##  ##  ## ##      ##    ##    ##    ##   ##    ##    ## ##    ## ##  ## ##          ## ##    ## ##    ## ##   ## ##      ##
 ######  ######  ##      ## ##       ######     ##    ##   ##    ##    ##  ######  ##   ####     #######  ######   ######  ##   ##  ###### #######

This is where computation actually starts... as soon as sample data are received and stored,

*/

    reg [ 6:0]  input_read_chan;        // the channel from which we read the data to process in memory // sb was 6:0
          

    reg [15:0]   data_from_source;      // the data that will be store
    reg  [6:0]   chan_from_source;      // the channel number to be output at the same time as the sample value		// sb was 6:0
    reg         valid_from_source;      // true when data_from_source is output
    wire              read_source;      // tells if new sample is read by next module


    // this state machine is necessary to avoid launching data read twice in the same
    // sample row... (yes, this may happen quite easily if processing is nicely pipelined)
    reg [1:0]   reading_FSM;
    localparam idle            = 0,
               loading_samples = 1,
               reading_samples = 2;
               
    
    always @(posedge clk) begin
        if       (reset)                                  reading_FSM     <= idle;
        else if (main_state == state_timestamp_1)         reading_FSM     <= loading_samples;
        else if (device_output == 5'd19)                  reading_FSM     <= reading_samples;	//sb: was 16
            // we consider that all meaningful samples are received when device_output is 16
            // because data samples 16 to 19 correspond to extra command transfers
				
				// sb: nope! there's a delay and so you need to get all 20. 
        else if (input_read_chan == 7'h7F && read_source) reading_FSM     <= idle; // sb it was (input_read_chan == 7'h7F && read_source)
            // the sample of the last channel is being read
    end



    always @(posedge clk) begin
        if (reset || reading_FSM == idle ) begin
            data_from_source    <= 16'h0000;
            chan_from_source    <=  7'h00; // sb was 7
            valid_from_source   <=  1'b0;
            input_read_chan     <=  7'h00; // sb was 7
				input_addr_AC       <=  9'd1;
        end else if ((read_source || !valid_from_source) && reading_FSM == reading_samples) begin
				if (input_fsm_subsampler==2'b00) begin
						input_addr_AC <= {1'b0, input_read_chan, 1'b1};
						data_from_source    <= input_storage_c[input_addr_AC];  // we only read samples coming from regular channels // sb it was input_storage_c[{1'b0, input_read_chan, 1'b1}]; 
						valid_from_source   <= 1'b1;
						chan_from_source    <= input_read_chan;
				// end else if (input_fsm_subsampler==2'b01) begin
						input_read_chan     <= input_read_chan + 1;
				end
        end
    end
	 
	 //sb

       // we don't always need to subsample the output...
    // only when we need to get data from memory (we might get some simpler way to code it, but I am not sure that it as efficient in terms of hardware resources)
    always @(posedge clk) begin
        if (reset) begin
            input_fsm_subsampler <= 2'b00; //sb: it was 00
        end else begin
            case (input_fsm_subsampler)
						//2'b00   : input_fsm_subsampler <= 2'b01;
						//2'b01   : input_fsm_subsampler <= 2'b00;
						default : input_fsm_subsampler <= 2'b00;
            endcase
        end
    end
//    
	 // sb

/*
 ######  ######  ###    ### ######  ##    ## ########  #####  ######## ##  ######  ###    ##     ###    ###  ######  ######  ##    ## ##      ####### #######
##      ##    ## ####  #### ##   ## ##    ##    ##    ##   ##    ##    ## ##    ## ####   ##     ####  #### ##    ## ##   ## ##    ## ##      ##      ##
##      ##    ## ## #### ## ######  ##    ##    ##    #######    ##    ## ##    ## ## ##  ##     ## #### ## ##    ## ##   ## ##    ## ##      #####   #######
##      ##    ## ##  ##  ## ##      ##    ##    ##    ##   ##    ##    ## ##    ## ##  ## ##     ##  ##  ## ##    ## ##   ## ##    ## ##      ##           ##
 ######  ######  ##      ## ##       ######     ##    ##   ##    ##    ##  ######  ##   ####     ##      ##  ######  ######   ######  ####### ####### #######


 - each data sample goes with the ID of its channel to avoid messing up data
 - flow protocol:
    * each module outputs its data as soon as they are available and tells with
      data_valid if it is available
    * the data receiver tells if it accepts the data with the data_read I/O.
    * if data in ouptut is not accepted by the receiver module, the emitter
      holds the data and data_valid bit until data is accepted by the receiver
    * data are actually exchanged when both data_ready and read_chan are set. Both
      emitter and receiver may transfer new data on the following clock cycle
      
 - for events, data actually is the timestamp of the event.

*/
    
        
    //always @(posedge clk) trig_stim = data_from_filter; // this is for debug only

    wire [15:0]   data_from_filter;
    wire  [6:0]   chan_from_filter;  // sb was 6:0
    wire         valid_from_filter;
    wire          read_from_filter;

    wire [15:0] tstamp_from_spkdetect0;
    wire  [6:0]   chan_from_spkdetect0; // sb was 6:0
    wire         valid_from_spkdetect0;
    wire          read_from_spkdetect0;


    CUST_HP_filter  #(
        .CHANNELS                   (128), // sb 160 was 128
        .CHANNELS_PW2               (7)	 // sb 8 was 7
        )
    filter_0
        (
        .clk                        (clk),
        .reset                      (reset),
        
        .chan_in_sample             (data_from_source),
        .chan_in_num                (chan_from_source),
        .chan_in_valid              (valid_from_source),
        .chan_in_read               (read_source),
        
        .chan_out_sample            (data_from_filter),
        .chan_out_num               (chan_from_filter),
        .chan_out_valid             (valid_from_filter),
        .chan_out_read              (read_from_filter),

        .coeff                      (filter_coeff)
        );
        


    CUST_spike_detect_0 #(
	    .CHANNELS 	            (128), // sb 160 was 128
        .CHANNELS_PW2           (7)	 // sb 8 was 7
	    )
    spk_detect_0
	    (
	    .clk                    (clk),
	    .reset                  (reset),
        .chan_in_sample         (data_from_filter),
        .chan_in_num            (chan_from_filter),
        .chan_in_valid          (valid_from_filter),
        .chan_in_read           (read_from_filter),
    
        .chan_out_num           (chan_from_spkdetect0),
        .chan_out_valid         (valid_from_spkdetect0),
        .chan_out_read          (read_from_spkdetect0),
    
        .threshold_0            (spkdet_0_threshold_0),
        .comp_start_0           (spkdet_0_startstop_0[ 7:0]),
        .comp_stop_0            (spkdet_0_startstop_0[15:8]),
        .above_0                (spkdet_0_above_0),
        .enable_0               (spkdet_0_enable_0),
        .threshold_1            (spkdet_0_threshold_1),
        .comp_start_1           (spkdet_0_startstop_1[ 7:0]),
        .comp_stop_1            (spkdet_0_startstop_1[15:8]),
        .above_1                (spkdet_0_above_1),
        .enable_1               (spkdet_0_enable_1),
        .threshold_2            (spkdet_0_threshold_2),
        .comp_start_2           (spkdet_0_startstop_2[ 7:0]),
        .comp_stop_2            (spkdet_0_startstop_2[15:8]),
        .above_2                (spkdet_0_above_2),
        .enable_2               (spkdet_0_enable_2),
        .threshold_3            (spkdet_0_threshold_3),
        .comp_start_3           (spkdet_0_startstop_3[ 7:0]),
        .comp_stop_3            (spkdet_0_startstop_3[15:8]),
        .above_3                (spkdet_0_above_3),
        .enable_3               (spkdet_0_enable_3),
        .final_decision_sample  (spkdet_0_finalreport)
        );

        assign read_from_spkdetect0 = 1'b1;





/*
######## ######  ##  ######   ######  ####### ######      ######  ##    ## ##      ####### ####### #######
   ##    ##   ## ## ##       ##       ##      ##   ##     ##   ## ##    ## ##      ##      ##      ##
   ##    ######  ## ##   ### ##   ### #####   ######      ######  ##    ## ##      ####### #####   #######
   ##    ##   ## ## ##    ## ##    ## ##      ##   ##     ##      ##    ## ##           ## ##           ##
   ##    ##   ## ##  ######   ######  ####### ##   ##     ##       ######  ####### ####### ####### #######
*/




    // the basetime that counts microseconds
    always @(posedge clk) begin
        if (reset)
            trigg_basetime <= 0;
        else if (trigg_basetime >= CLKFREQ_MHZ)
            trigg_basetime <= 0;
        else
            trigg_basetime <= trigg_basetime + 1;
    end

    // the trigger counters...
    // these counters are set to the pulse length if activated, and decreased each microsecond.
    // the resulting length is actually between trig_length-1 and trig_length microseconds
    // depending of when the counter is activated vs trigg_basetime
    always @(posedge clk) begin
        for (index=0; index<TRIGGER_NUM; index=index+1) begin
            if (reset)
                trig_counter[index] <= 0;
            else if (next_cmd_word_num==2'b01 && cmd_words[0]==16'h0222 && pipe_in_en && pipe_in_data[index])
                trig_counter[index] <= trig_length;
            // here we should add the test to trigg according to the selected event
            else if (trigg_basetime == 0 && trig_counter[index]>0 )
                trig_counter[index] <= trig_counter[index] - 1;
        end
    end

    // the trigger outputs, they are active as far as the corresponding counter is different from 0
    always @(posedge clk) begin
        for (index=0; index<TRIGGER_NUM; index=index+1) begin
            if (reset)
                trig_stim[index] <= 1'b0;
            else if (trig_counter[index]>=0)
                trig_stim[index] <= trig_enable[index];
            else
                trig_stim[index] <= 1'b0;
        end
    end




/*
######   #####  ######   #####  ###    ### ####### ######## ####### ######      ###    ###  #####  ###    ##  #####   ######  ####### ###    ### ####### ###    ## ########
##   ## ##   ## ##   ## ##   ## ####  #### ##         ##    ##      ##   ##     ####  #### ##   ## ####   ## ##   ## ##       ##      ####  #### ##      ####   ##    ##
######  ####### ######  ####### ## #### ## #####      ##    #####   ######      ## #### ## ####### ## ##  ## ####### ##   ### #####   ## #### ## #####   ## ##  ##    ##
##      ##   ## ##   ## ##   ## ##  ##  ## ##         ##    ##      ##   ##     ##  ##  ## ##   ## ##  ## ## ##   ## ##    ## ##      ##  ##  ## ##      ##  ## ##    ##
##      ##   ## ##   ## ##   ## ##      ## #######    ##    ####### ##   ##     ##      ## ##   ## ##   #### ##   ##  ######  ####### ##      ## ####### ##   ####    ##


*/

    always @(posedge clk) begin
        if (reset)
            next_cmd_word_num <= 2'b00;
        else if (pipe_in_en) begin
            case(next_cmd_word_num)
                2'b00 : begin
                            if (pipe_in_data[15:8]!=8'h01) begin
                                next_cmd_word_num <= 2'b01;
                                cmd_words[0]      <= pipe_in_data;
                            end
                        end
                2'b01 : begin
                            if (cmd_words[0][15:8]!=8'h02) begin
                                next_cmd_word_num <= 2'b10;
                                cmd_words[1]      <= pipe_in_data;
                            end else begin
                                next_cmd_word_num <= 2'b00;
                            end
                        end
                2'b10 : begin
                            if (cmd_words[0][15:8]!=8'h03) begin
                                next_cmd_word_num <= 2'b11;
                                cmd_words[2]      <= pipe_in_data;
                            end else begin
                                next_cmd_word_num <= 2'b00;
                            end
                        end
                default :       next_cmd_word_num <= 2'b00;                    
            endcase
        end
    end


    // HPF1 filter coefficient
    always @(posedge clk) begin
        if (reset)
            filter_coeff <= 16'h0001;  // sb for debugging
        else if (cmd_words[0]==16'h0200 && next_cmd_word_num==2'b01 && pipe_in_en)
            filter_coeff <= pipe_in_data;
    end



    // event source per trigger
    always @(posedge clk) begin
        for (index=0; index<TRIGGER_NUM; index=index+1) begin
            if (reset)
                trig_source[index] <= 16'h0000;
            else if (cmd_words[0][15:4]==12'h021 && cmd_words[0][3:0]==index && next_cmd_word_num==2'b01 && pipe_in_en)
                trig_source[index] <= pipe_in_data;
        end
    end


    // the length of the triggers
    always @(posedge clk) begin
        if (reset)
            trig_length <= 16'h0000;
        else if (cmd_words[0]==16'h0220 && next_cmd_word_num==2'b01 && pipe_in_en)
            trig_length <= pipe_in_data;
    end

    // enable triggers (or not)
    always @(posedge clk) begin
        if (reset)
            trig_enable <= 0;
        else if (cmd_words[0]==16'h0221 && next_cmd_word_num==2'b01 && pipe_in_en)
            trig_enable <= pipe_in_data[TRIGGER_NUM-1:0];
    end


    // force trigg instruction is directly managed at the trigger management...
    
    
    // parameters for spike detector 0
    always @(posedge clk) begin
        if (reset)                                                                  spkdet_0_threshold_0 <= 0;
        else if (cmd_words[0]==16'h0230 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_threshold_0 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_startstop_0 <= 0;
        else if (cmd_words[0]==16'h0231 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_startstop_0 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_threshold_1 <= 0;
        else if (cmd_words[0]==16'h0232 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_threshold_1 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_startstop_1 <= 0;
        else if (cmd_words[0]==16'h0233 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_startstop_1 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_threshold_2 <= 0;
        else if (cmd_words[0]==16'h0234 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_threshold_2 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_startstop_2 <= 0;
        else if (cmd_words[0]==16'h0235 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_startstop_2 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_threshold_3 <= 0;
        else if (cmd_words[0]==16'h0236 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_threshold_3 <= pipe_in_data;

        if (reset)                                                                  spkdet_0_startstop_3 <= 0;
        else if (cmd_words[0]==16'h0237 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_startstop_3 <= pipe_in_data;

        if (reset) begin
                spkdet_0_above_0    <= 0;
                spkdet_0_enable_0   <= 0;
                spkdet_0_above_1    <= 0;
                spkdet_0_enable_1   <= 0;
                spkdet_0_above_2    <= 0;
                spkdet_0_enable_2   <= 0;
                spkdet_0_above_3    <= 0;
                spkdet_0_enable_3   <= 0;
            end
        else if (cmd_words[0]==16'h0238 && next_cmd_word_num==2'b01 && pipe_in_en) begin
                spkdet_0_above_0    <= pipe_in_data[0];
                spkdet_0_enable_0   <= pipe_in_data[1];
                spkdet_0_above_1    <= pipe_in_data[2];
                spkdet_0_enable_1   <= pipe_in_data[3];
                spkdet_0_above_2    <= pipe_in_data[4];
                spkdet_0_enable_2   <= pipe_in_data[5];
                spkdet_0_above_3    <= pipe_in_data[6];
                spkdet_0_enable_3   <= pipe_in_data[7];
            end

        if (reset)                                                                  spkdet_0_finalreport <= 0;
        else if (cmd_words[0]==16'h0239 && next_cmd_word_num==2'b01 && pipe_in_en)  spkdet_0_finalreport <= pipe_in_data[7:0];
    end
    
    
    
    

    


/*
 ###### ###    ### ######      ######  ####### ####### ######   ######  ###    ## ####### ####### #######
##      ####  #### ##   ##     ##   ## ##      ##      ##   ## ##    ## ####   ## ##      ##      ##
##      ## #### ## ##   ##     ######  #####   ####### ######  ##    ## ## ##  ## ####### #####   #######
##      ##  ##  ## ##   ##     ##   ## ##           ## ##      ##    ## ##  ## ##      ## ##           ##
 ###### ##      ## ######      ##   ## ####### ####### ##       ######  ##   #### ####### ####### #######
*/


    always @(posedge clk) begin
        if (reset)
            pipe_out_FIFO_write <= 1'b0;
        else if (next_cmd_word_num==2'b00 && pipe_in_en && pipe_in_data==16'h0100)
            begin
                pipe_out_FIFO_write <= 1'b1;
                pipe_out_FIFO_data  <= VERSION_MAJOR;
            end
        else if (next_cmd_word_num==2'b00 && pipe_in_en && pipe_in_data==16'h0101)
            begin
                pipe_out_FIFO_write <= 1'b1;
                pipe_out_FIFO_data  <= VERSION_MINOR;
            end
        else if (next_cmd_word_num==2'b00 && pipe_in_en && pipe_in_data==16'h0102)
            begin
                pipe_out_FIFO_write <= 1'b1;
                pipe_out_FIFO_data  <= last_err;
            end
        else
            pipe_out_FIFO_write <= 1'b0;
    end


    // error management...
    always @(posedge clk) begin
        if (reset)
            if ((BOARD_ID==16'd800) && (BOARD_VERSION==16'd1))
                last_err <= 0;
            else
                last_err <= 8;                      // system version mismatch
        else if (pipe_out_FIFO_write && (pipe_out_data_size==LOC_RESP_FIFO_SIZE))
            last_err <= 2;                          // no space in FIFO to store responses
        else if (pipe_out_read && (pipe_out_data_size==0))
            last_err <= 4;                          // reading empty FIFO
        else if (pipe_in_en && next_cmd_word_num==2'b00)
            case(pipe_in_data)
                16'h0000,
                16'h0100, 16'h0101, 
                16'h0200:   begin end
                
                16'h0102:   last_err <= 16'h0000;   // reading the error clears it
                default :   last_err <= 16'h0001;   // unknown command
            endcase
            
    end


/*
######  ####### ####### ######   ######  ###    ## ####### #######     ####### ## #######  ######
##   ## ##      ##      ##   ## ##    ## ####   ## ##      ##          ##      ## ##      ##    ##
######  #####   ####### ######  ##    ## ## ##  ## ####### #####       #####   ## #####   ##    ##
##   ## ##           ## ##      ##    ## ##  ## ##      ## ##          ##      ## ##      ##    ##
##   ## ####### ####### ##       ######  ##   #### ####### #######     ##      ## ##       ######


The FIFO code is actually wrong because the FIFO reports having one element
befor the element is actually available on the output (1 clock cycle delay)
It does not really matter since the USB master will require much more than
one clock cycle to consult the FIFO status and decide to read its content

The fix is easy, we just need to change status from wire to reg, but it takes
flip-flops for nothing...

*/


    

    always @(posedge clk) begin
        if (reset)
            pipe_out_valid <= 0;
        else if (~pipe_out_valid || pipe_out_read)
            begin
                pipe_out_data  <= pipe_out_FIFO[read_addr];
                if (pipe_out_read)
                    pipe_out_valid <= pipe_out_data_size > 1;
                else
                    pipe_out_valid <= pipe_out_data_size > 0;
            end;
        if (reset)
            read_addr <= 0;
        else if ((pipe_out_read && pipe_out_data_size>1) || ((~pipe_out_valid) && (pipe_out_data_size!=0)))
            if (read_addr>=LOC_RESP_FIFO_SIZE-2)
                read_addr <= 0;
            else
                read_addr <= read_addr +1;
    end

    always @(posedge clk) begin
        if (pipe_out_FIFO_write && (pipe_out_data_size!=(LOC_RESP_FIFO_SIZE-1)))
            pipe_out_FIFO[write_addr] <= pipe_out_FIFO_data;
            
        if (reset)
            write_addr <= 0;
        else if (pipe_out_FIFO_write && (pipe_out_data_size!=(LOC_RESP_FIFO_SIZE-1)))
            if (write_addr>=LOC_RESP_FIFO_SIZE-2)
                write_addr <= 0;
            else
                write_addr <= write_addr + 1;
    end

    always @(posedge clk) begin
        if (reset)
            pipe_out_data_size <= 0;
        else if (pipe_out_read && ~pipe_out_FIFO_write && pipe_out_data_size!=0)
            pipe_out_data_size <= pipe_out_data_size - 1;
        else if (pipe_out_FIFO_write && ~pipe_out_read && pipe_out_data_size!=(LOC_RESP_FIFO_SIZE-1))
            pipe_out_data_size <= pipe_out_data_size + 1;
        else if (pipe_out_FIFO_write && pipe_out_read && pipe_out_data_size==0)
            pipe_out_data_size <= pipe_out_data_size + 1;
    end



endmodule