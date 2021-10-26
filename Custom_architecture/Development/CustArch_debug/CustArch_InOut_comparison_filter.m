clear
close all
clc


%% Change current folder
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
curr_folder = pwd;
path_index = strfind(curr_folder,'closed-loop-neuroscience');

project_path = curr_folder(1:path_index+length('closed-loop-neuroscience')-1);
addpath(genpath(project_path)); %adding to the Matlab path all the project folder including all the subfolders


%% Parameter setting
data_stream_enable = [1 0 0 0 0 0 0 0]; %data stream enable rollmap
n_chip = sum(data_stream_enable); % number of data stream enabled in the simulated data
n_dataframe = 300;
cust_version = 5;   %version of custom architecture

chip = num2str(data_stream_enable);
frame = num2str(n_dataframe);

power_spect_ch = 10; %performig the PSD of this channel
fs = 30000; %sampling frequency (Hz)


%% Loading data
%raw input
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\raw_fsm_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])

% load('raw_fsm_valid_chip10000000_nframe300.mat')


%input
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\fsm_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
fsm_out_def_time = fsm_out_def_time.Data;

load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\fsm_valid_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
fsm_valid_def_time = fsm_valid_def_time.Data;


%output
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\custv',num2str(cust_version),...
        '_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
comp_data_out = comp_data_out.Data;

load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\custv',num2str(cust_version),...
        '_out_valid_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
    
comp_data_out_valid = comp_data_out_valid.Data;



% indexing 
idx_in = find(fsm_valid_def_time == 1);
idx_out = find(comp_data_out_valid == 1);


raw_fsm_in = fsm_out_def(idx_in);
raw_idx_in_DC = find(raw_fsm_in == 10000);
raw_idx_in_AC = find(raw_fsm_in == 20000);


%I/O comparison
fsm_in = fsm_out_def_time(idx_in);
fsm_out = comp_data_out(idx_out);


figure
stem(fsm_in(1:length(fsm_out)),'r','LineWidth',1),hold on
stem(fsm_out,'b','LineWidth',1),hold off
title('Comparison custom architecture in/out')
legend('Input','Output')


%AC/DC filter comparison
% n_element = round(length(fsm_out)/16,0)*16;
% fsm_out = fsm_out(1:n_element);
% fsm_in = fsm_in(1:n_element);

AC_raw = (fsm_out(raw_idx_in_AC(find(raw_idx_in_AC <= length(fsm_out)))));
AC_filtered = (fsm_out(raw_idx_in_DC(find(raw_idx_in_DC <= length(fsm_out)))));

n_element = round(length(AC_filtered)/(16*n_chip),0)*(16*n_chip)-(16*n_chip);

AC_raw = reshape(AC_raw(1:n_element),16*n_chip,[]);
AC_filtered = reshape(AC_filtered(1:n_element),16*n_chip,[]);

time = (0:length(AC_raw(1,:))-1);

%raw vs filtered data
for i = 1:n_chip*16
    
    figure
%     subplot(2,1,1)
    plot(time,AC_raw(i,:),'r','LineWidth',1),hold on
%     title('Raw data')
%     ylabel('Amplitude')
%     axis([0 length(time) 32000 33200])
%     set(gca,'FontSize',14)
%     subplot(2,1,2)
    plot(time,AC_filtered(i,:),'b','LineWidth',1)
    title('High-gain data (Raw) data vs Low-gain data (Filtered)')
    xlabel('Time (Samples)')
    ylabel('Amplitude')
%     axis([0 length(time) 32000 33200])
    set(gca,'FontSize',14)
    legend('Raw','Filtered')
    
end

w = hamming(128);
noverlap = length(w)/2;
NFFT = 1024;

AC_raw_double = (double(AC_raw) - 32768)*0.195;
AC_filtered_double = (double(AC_filtered) - 32768)*0.195;


%PSD
%pwelch needs single or double data
[pxx,f] = pwelch(AC_raw_double(power_spect_ch,:)-mean(AC_raw_double(power_spect_ch,:)),w,noverlap,NFFT,fs);
[pxx_f,f_f] = pwelch(AC_filtered_double(power_spect_ch,:)-mean(AC_filtered_double(power_spect_ch,:)),w,noverlap,NFFT,fs);


figure
plot(f,pxx,'r'),hold on
plot(f_f,pxx_f,'b')
hold off
title('PSD')
legend('Raw','Filtered')
% axis([0 127 0 1]);










