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
pipe_in_version = 3;   %version of pipe_in_debug

chip = num2str(data_stream_enable);
frame = num2str(n_dataframe);

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
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\pipe_in_v',num2str(pipe_in_version),...
        '_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
comp_data_out = comp_data_out.Data;

load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\pipe_in_v',num2str(pipe_in_version),...
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













