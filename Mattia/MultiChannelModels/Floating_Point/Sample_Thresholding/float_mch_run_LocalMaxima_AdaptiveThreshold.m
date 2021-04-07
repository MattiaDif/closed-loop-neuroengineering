clear all
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


% %%%%%%%%% CHANGE THE noise_level VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
% noise_level = 10;   %10, 20, 30
% %%%%%%%%% CHANGE THE ch VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
% ch = 'ch7';

% %%%%%%%%% CHANGE THE mdl_name VARIABLE ACCORDING TO THE SIMULINK MODEL %%%%%%%%%
mdl_name = "float_sch_LocalMaxima_AdaptiveThreshold";


%% Simulation parameters
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
feature_buffer = fs;    %feature buffer length
feature_gain = [5];   %%adaptive threshold gain
sim_type = 'normal'; %simulation speed
sim_stop_time = '5';   %s


%% Performance analysis parameters
w_len = fs/1000;  %samples --> 1ms
peak_diff = 15; %samples --> max spike position distance between recording and ground truth
spiketrain = 1; %ground_truth selected for performance evaluation
%peak_diff --> tolerance


%% Data loading
%worskpace saving --> sim parameters saving
filename = 'tetrode_test_20';

signal = load([filename,'.mat']);
ground = load([filename,'_gt.mat']);

