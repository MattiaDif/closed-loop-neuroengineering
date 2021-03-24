%% This replicates the output of the RHS FSM sent to the host computer

% Main State Machine Description

% The heart of the RhythmStim code, in main.v, is a finite state machine that cycles through a pattern of 140 repeating states. These 140 states execute a single SPI cycle. 
% (Refer to the RHS2116 datasheet for more information on the SPI communication protocol.) Each SCLK period consists of four states labeled ms_clkX_a through ms_clkX_d, 
% where X advances from 1 to 32. After 32 complete SCLK cycles, there are an additional two states (ms_clk33_a and ms_clk33_b) to create a delay between the falling edge of 
% the last SCLK pulse and the rising edge of CS. The CS signal is held high for 10 states labeled ms_cs_a through ms_cs_j. After all 140 states have completed, the variable channel 
% is incremented. If channel exceeds 19, it is reset to 0. This variable tracks the repeating series of 20 commands send on the MOSI line (16 CONVERT commands for each amplifier 
% channel and four auxiliary commands).
% Data is transferred to the FIFO (and thence to the USB interface) during particular states of this main state machine. One 16-bit word can be transferred to the FIFO during each state.

% • The 64-bit header magic number is sent during states ms_clk1_b through ms_clk2_a only when channel = 0.
% • The timestamp is sent during states ms_clk2_b and ms_clk2_c only when channel = 0.
% • Data streams 1-8 are sent during states ms_clk2_d through ms_clk6_c.
% • The state of all on-chip stimulators (including stimulator on/off, polarity, amplifier settle on/off, and charge recovery on/off) are sent during states ms_clk6_d through ms_clk14_c.
% • States ms_clk14_d through ms_clk30_c could be used to send more data streams in the future.
% • States ms_clk30_d through ms_clk32_c are used to send data from the eight optional DACs only when channel = 19.
% • States ms_clk32_d through ms_cs_e are used to send data from the eight optional ADCs only when channel = 19.
% • State ms_cs_f is used to send the 16 TTL inputs only when channel = 19.
% • State ms_cs_g is used to send the current value of the 16 TTL outputs only when channel = 19.

% in the Intan platform all data are communicated using to UINT16 datatype



clear
clc


%% Initial conditions
%MIN 1, MAX 8
n_chip = 8; %how much chip you want to simulate (example: if you want to simulate an acquisition using a headstage 16 channels (1 chip) write 1;
            %if you want to simulate an acquisition using two headstages 32 channels (4 chip) write 4
n_dataframe = 100;  %how much dataframe simulate
on_off = 1; %stimulation on/off, 1 --> on, 0 --> not
polarity = 1; %polarity, 1 --> on, 0 --> not
settle = 1; %amplifier settle, 1 --> on, 0 --> not
recovery = 1; %charge recovery, 1 --> on, 0 --> not


%% Data flow replica
dc_data = ones(10000000,1)*535;
ac_data = ones(10000000,1)*7535;

fsm_out = char(dec2bin(zeros(140*20,1),16)); %140 --> states of the FSM, constant, 20 --> 16 channels + 4 aux commands
fsm_valid = zeros(140*20,1);

magic_number = '1000110101010100001011001000101001001001011100010010111100001011'; % 64'h8d542c8a49712f0b


% 64999 --> dummy value ms_cs_j, ms_clk1_a
% magic number correnspond to the real one

% 70000 --> dummy number timestamp

% 10000 --> dummy number DC
% 20000 --> dummy number AC

% 4 --> dummy number aux commands low bits
% 8 --> dummy number aux commands high bits

% 16 --> dummy value additional parameters

% 18 --> dummy value ADC, DAC, TTL

% 64999 --> dummy value ms_cs_h, ms_cs_i

% 50 --> dummy value for not informative data


tic

count = 1;
for curr_ch = 1:20    %16 channels + 4 aux commands per chip
    if curr_ch == 1

        fsm_out(count,:) = char(dec2bin(64999,16)); %ms_cs_j
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(64999,16)); %ms_clk1_a
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(magic_number(49:64)); %ms_clk2_a --> magic number start
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(magic_number(33:48));
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(magic_number(17:32));
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(magic_number(1:16));
        fsm_valid(count) = 1;
        count = count + 1;

        sample = dec2bin(70000,32);

        fsm_out(count,:) = char(sample(17:32));   %ms_clk2_b --> timestamp start
        fsm_valid(count) = 1;
        count = count + 1;

        fsm_out(count,:) = char(sample(1:16));
        fsm_valid(count) = 1;
        count = count + 1;

    else

        fsm_out(count,:) = char(dec2bin(50,16)); %ms_cs_j
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16)); %ms_clk1_a
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16)); %ms_clk2_a --> magic number start
        fsm_valid(count) =0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16));
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16));
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16));
        fsm_valid(count) = 0;
        count = count + 1;

        sample = dec2bin(50,16);

        fsm_out(count,:) = char(sample);   %ms_clk2_b --> timestamp start
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(sample);
        fsm_valid(count) = 0;
        count = count + 1;

    end


    %ms_clk2_d --> channel data start
    if curr_ch <= 16
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(10000,16));    % DC low gain amplifier sample
            fsm_valid(count) = 1;
            count = count + 1;

            fsm_out(count,:) = char(dec2bin(20000,16));    % AC high gain amplifier sample
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

                fsm_out(count,:) = char(dec2bin(50,16));    % DC low gain amplifier sample
                fsm_valid(count) = 0;
                count = count + 1;

                fsm_out(count,:) = char(dec2bin(50,16));    % AC high gain amplifier sample
                fsm_valid(count) = 0;
                count = count + 1;

            end
        end
    end


    if curr_ch > 16
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(4,16));    % aux low
            fsm_valid(count) = 1;
            count = count + 1;

            fsm_out(count,:) = char(dec2bin(8,16));    % aux high
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

                fsm_out(count,:) = char(dec2bin(50,16));    % aux low
                fsm_valid(count) = 0;
                count = count + 1;

                fsm_out(count,:) = char(dec2bin(50,16));    % aux high
                fsm_valid(count) = 0;
                count = count + 1;

            end
        end
    end


    %ms_clk6_d --> additional parameters start
    %on/off
    if curr_ch == 20 && on_off == 1
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(16,16));    % on/off
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

            fsm_out(count,:) = char(dec2bin(50,16));    % on/off
            fsm_valid(count) = 0;
            count = count + 1;

            end
        end
    end


    if curr_ch == 20 && on_off == 0 || curr_ch < 20
        for i = 1:8

            fsm_out(count,:) = char(dec2bin(50,16));    % on/off
            fsm_valid(count) = 0;
            count = count + 1;

        end
    end


    %polarity
    if curr_ch == 20 && polarity == 1
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(16,16));    % polarity
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

            fsm_out(count,:) = char(dec2bin(50,16));    % polarity
            fsm_valid(count) = 0;
            count = count + 1;

            end
        end
    end


    if curr_ch == 20 && polarity == 0 || curr_ch < 20
        for i = 1:8

            fsm_out(count,:) = char(dec2bin(50,16));    % polarity
            fsm_valid(count) = 0;
            count = count + 1;

        end
    end


    %settle
    if curr_ch == 20 && settle == 1
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(16,16));    % settle
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

            fsm_out(count,:) = char(dec2bin(50,16));    % settle
            fsm_valid(count) = 0;
            count = count + 1;

            end
        end
    end


    if curr_ch == 20 && settle == 0 || curr_ch < 20
        for i = 1:8

            fsm_out(count,:) = char(dec2bin(50,16));    % settle
            fsm_valid(count) = 0;
            count = count + 1;

        end
    end


    %recovery
    if curr_ch == 20 && recovery == 1
        for i = 1:n_chip

            fsm_out(count,:) = char(dec2bin(16,16));    % recovery
            fsm_valid(count) = 1;
            count = count + 1;

        end

        if n_chip < 8
            for j = 1:8-n_chip

            fsm_out(count,:) = char(dec2bin(50,16));    % recovery
            fsm_valid(count) = 0;
            count = count + 1;

            end
        end
    end


    if curr_ch == 20 && recovery == 0 || curr_ch < 20
        for i = 1:8

            fsm_out(count,:) = char(dec2bin(50,16));    % recovery
            fsm_valid(count) = 0;
            count = count + 1;

        end
    end


    %ms_clk14_d --> unused states
    for i = 1:64    %64 --> number of unused states
        fsm_out(count,:) = char(dec2bin(50,16));    % unused
        fsm_valid(count) = 0;
        count = count + 1;
    end


    %ms_clk30_d --> DAC, ADC, TTL I/O
    if curr_ch == 20
        for i = 1:18 %--> 8 DAC + 8 ADC + 1 TTL inp + 1 TTL out

            fsm_out(count,:) = char(dec2bin(18,16));    % DAC, ADC, TTL I/O
            fsm_valid(count) = 1;
            count = count + 1; 

        end
    else
         for i = 1:18 %--> 8 DAC + 8 ADC + 1 TTL inp + 1 TTL out

            fsm_out(count,:) = char(dec2bin(50,16));    % DAC, ADC, TTL I/O
            fsm_valid(count) = 0;
            count = count + 1; 

        end
    end


    %ms_cs_h, %ms_cs_i
    if curr_ch == 20

        fsm_out(count,:) = char(dec2bin(64999,16)); %ms_cs_h
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(64999,16)); %ms_cs_i
        fsm_valid(count) = 0;
        count = count + 1; 

    else

        fsm_out(count,:) = char(dec2bin(50,16));    %ms_cs_h
        fsm_valid(count) = 0;
        count = count + 1;

        fsm_out(count,:) = char(dec2bin(50,16));    %ms_cs_i
        fsm_valid(count) = 0;
        count = count + 1;

    end
end


%conversion to uint16
fsm_out = uint16(bin2dec(fsm_out));
fsm_valid = uint16(fsm_valid);

%replicate the previous sequence n_samples time
fsm_out_def = repmat(fsm_out,n_dataframe,1);
fsm_valid_def = repmat(fsm_valid,n_dataframe,1);

toc


%% let's replicate dummy values with real values
dc_locks = find(fsm_out_def == 10000);  %DC low gain
fsm_out_def(dc_locks) = dc_data(1:n_dataframe*n_chip*16);


ac_locks = find(fsm_out_def == 20000);  %AC high gain
fsm_out_def(ac_locks) = ac_data(1:n_dataframe*n_chip*16);


timestamp = char(dec2bin(1:n_dataframe,32));    %timestamp
timestamp_low = bin2dec(timestamp(:,17:32));
timestamp_high = bin2dec(timestamp(:,1:16));
time_low_locks = find(fsm_out_def == 4464);
time_high_locks = find(fsm_out_def == 1);
fsm_out_def(time_low_locks) = timestamp_low;
fsm_out_def(time_high_locks) = timestamp_high;



% save('C:\Users\BuccelliLab\Desktop\Chronics_paper\Sync Artifact Vid Data\all_valid_u16_time_8','all_valid_u16_16_time_8')
% save('C:\Users\BuccelliLab\Desktop\Chronics_paper\Sync Artifact Vid Data\all_data_u16_time_8','all_data_u16_time_8')