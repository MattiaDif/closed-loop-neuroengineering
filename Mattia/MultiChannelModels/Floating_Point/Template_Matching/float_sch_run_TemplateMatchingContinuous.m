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
% noise_level = 30;   %10, 20, 30
% %%%%%%%%% CHANGE THE ch VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
% ch = 'ch7';

%%%%%%%%% CHANGE THE mdl_name VARIABLE ACCORDING TO THE SIMULINK MODEL %%%%%%%%%
mdl_name = "float_sch_TemplateMatchingContinuous";


filename = 'tetrode_recording_20';
load([filename,'_waveforms_mean.mat']);

%% Simulation parameters
load([filename,'.mat']);
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
template = double(mean_waveform{1, 1})';  %template extracted from MEArec dataset
% template2 = double(mean_waveform{1, 2}(:,str2num(ch(3:end))))';
% template3 = double(mean_waveform{1, 3}(:,str2num(ch(3:end))))';
buffer_rec = length(template);    %buffer length
buffer_overlap = buffer_rec - 1;    %buffer overlap
score = [3600];
sim_type = 'normal'; %simulation speed
sim_stop_time = '5';   %s


%% Performance analysis parameters
w_len = fs/1000;  %samples --> 1ms
peak_diff = 65; %samples --> max spike position distance between recording and ground truth
spiketrain = 1; %ground_truth selected for performance evaluation


%% Data loading
filename = 'tetrode_test_20';

signal = load([filename,'.mat']);
ground = load([filename,'_gt.mat']);

