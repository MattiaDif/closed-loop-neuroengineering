%% This replicates the output of RHS chip when only one data stream (the first) is enabled.
clear
clc
fs=30e3;
m=matfile('C:\Users\BuccelliLab\Desktop\Chronics_paper\Sync Artifact Vid Data\extracted_data.mat');
data_16chs_1s=m.amplifier_data(1:16,1:fs);
%%

%% don't forget to modify the filter in Verilog with this number:
% fc=300;
% b = 1.0 - exp(-2.0 * 3.1415926535897 * fc / fs); 
% filterCoefficient = floor(65536.0 * b + 0.5);

%% put in unsigned integer
amplifier_u16=zeros(size(data_16chs_1s));
for curr_ch=1:size(data_16chs_1s,1)
    amplifier_u16(curr_ch,:)=round(32768+data_16chs_1s(curr_ch,:)/0.195);
end

load('C:\Users\BuccelliLab\Desktop\tests_fpga\normal.mat')
amplifier_u16=a;
dc_ampl_u16=b;
amplifier_u16_2=fliplr(a);
dc_ampl_u16_2=fliplr(b);

%% create the data flow
header_magic_number='1000110101010100001011001000101001001001011100010010111100001011'; % 64'h8d542c8a49712f0b
n_samples=20;
all_data_b16=char(zeros(6*n_samples+16*20*n_samples+8*4*n_samples+18*n_samples,16)); % 
all_valid=char(zeros(size(all_data_b16,1),1));
features=[];
count=1;
for curr_sample=1:n_samples
    for curr_ch=1:20
        if curr_ch==1
            all_data_b16(count,:)=header_magic_number(49:64);
            all_data_b16(count+1,:)=header_magic_number(33:48);
            all_data_b16(count+2,:)=header_magic_number(17:32);
            all_data_b16(count+3,:)=header_magic_number(1:16);
            all_data_b16(count+4,:)=char(dec2bin(curr_sample,16));
            all_data_b16(count+5,:)=char(dec2bin(0,16));
            all_valid(count:count+5,1)='1';
            count=count+6;
        end
        %% in reality, due to the pipeline first 3 inputs will be results from aux commands then 16 amplifier and then the result to command aux 0
        if curr_ch<17
            % headstage 1
            all_data_b16(count,:)=char(dec2bin(dc_ampl_u16(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+1,:)=char(dec2bin(amplifier_u16(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 2
            all_data_b16(count+2,:)=char(dec2bin(dc_ampl_u16_2(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+3,:)=char(dec2bin(amplifier_u16_2(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 3
            all_data_b16(count+4,:)=char(dec2bin(dc_ampl_u16(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+5,:)=char(dec2bin(amplifier_u16(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 4
            all_data_b16(count+6,:)=char(dec2bin(dc_ampl_u16_2(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+7,:)=char(dec2bin(amplifier_u16_2(curr_ch,curr_sample),16)); % ok if up to 16
            
            % headstage 5
            all_data_b16(count+8,:)=char(dec2bin(dc_ampl_u16(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+9,:)=char(dec2bin(amplifier_u16(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 6
            all_data_b16(count+10,:)=char(dec2bin(dc_ampl_u16_2(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+11,:)=char(dec2bin(amplifier_u16_2(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 7
            all_data_b16(count+12,:)=char(dec2bin(dc_ampl_u16(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+13,:)=char(dec2bin(amplifier_u16(curr_ch,curr_sample),16)); % ok if up to 16
            % headstage 8
            all_data_b16(count+14,:)=char(dec2bin(dc_ampl_u16_2(curr_ch,curr_sample),16)); % DC low gain let's put 16 bits
            all_data_b16(count+15,:)=char(dec2bin(amplifier_u16_2(curr_ch,curr_sample),16)); % ok if up to 16
            
        else
            % headstage 1
            all_data_b16(count,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+1,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            % headstage 2
            all_data_b16(count+2,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+3,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
             % headstage 3
            all_data_b16(count+4,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+5,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            % headstage 4
            all_data_b16(count+6,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+7,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            
             % headstage 5
            all_data_b16(count+8,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count++9,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            % headstage 6
            all_data_b16(count+10,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+11,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
             % headstage 7
            all_data_b16(count+12,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+13,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            % headstage 8
            all_data_b16(count+14,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            all_data_b16(count+15,:)=char(dec2bin(4,16));% simulating the 4 aux commands responses (but they are not in this order in reality.. need to figure out how the Verilog or C++ code manage this discrepancy)
            
        end
        all_valid(count:count+15,1)='1';
        count=count+16;
        %% add zeros meaning positive clocks with no data to write. Let's increse from 14 to 36
        for curr_count=count:count+138
            all_data_b16(curr_count,:)=char(dec2bin(32,16));%
        end
        all_valid(count:count+138,1)='0';
        count=count+139;
        %% then we need to put all the other stuff
        if curr_ch==20
            for curr_count=count:count+49
                all_data_b16(curr_count,:)=char(dec2bin(16+curr_count-count,16)); % simulating all the other values as 16
            end
            all_valid(count:count+7,1)='1';             % Stim On-OFF
%             all_valid(count+2:count+7,1)='0';
            all_valid(count+8:count+15,1)='1';           % Stim Pol
%             all_valid(count+10:count+15,1)='0';
            all_valid(count+16:count+23,1)='1';          % Amp settle
%             all_valid(count+18:count+23,1)='0';
            all_valid(count+24:count+31,1)='1';          % Charge recovery
%             all_valid(count+26:count+31,1)='0';
            all_valid(count+32:count+49,1)='1'; % DAC ADC
            count=count+50;
        end
%         %% here we add zeros just to have the time to perform the simulation (output data is slower that retrieving it, of course)
%          if curr_ch==20
%              for curr_count=count:count+999
%                  all_data_b16(curr_count,:)=char(dec2bin(0,16)); % simulating all the other values as 0
%                  all_valid(curr_count,1)='0';
%              end
%              count=count+1000;
%          end
    end
end
%%
all_valid_u16=logical(str2num(all_valid));
all_data_u16=bin2dec(all_data_b16);
all_valid_u16_16_time_8=timeseries(all_valid_u16);
all_data_u16_time_8=timeseries(uint16(all_data_u16));
save('C:\Users\BuccelliLab\Desktop\Chronics_paper\Sync Artifact Vid Data\all_valid_u16_time_8','all_valid_u16_16_time_8')
save('C:\Users\BuccelliLab\Desktop\Chronics_paper\Sync Artifact Vid Data\all_data_u16_time_8','all_data_u16_time_8')