%% This script load the parameters necessary for the fixed-point conversion of the hard threshold model

clear all
close all
clc


project_path = 'C:\GitHub\closed-loop-neuroscience';    %set the path according to the repository location
addpath(genpath(project_path)); %adding to the Matlab path all the project folder including all the subfolders


% %%%%%%%%% CHANGE THE noise_level VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
% noise_level = 10;   %10, 20, 30
% %%%%%%%%% CHANGE THE ch VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
% ch = 'ch10';

%%%%%%%%% CHANGE THE mdl_name VARIABLE ACCORDING TO THE SIMULINK MODEL %%%%%%%%%
mdl_name = "fixed_sch_Sample_HardThreshold";


%% Simulation parameters
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
th=32408; % sweeping  thresholds, from -70 µV --> (-70*10^-6*192*(2^16-1))/(2*1.225) + 32768 and quantized using a voltage step size of 0.195 µV
sim_type = 'normal'; %simulation speed
sim_stop_time = '10';   %s


%% Performance analysis parameters
w_len = fs/1000;  %samples --> 1ms
peak_diff = 15; %samples --> max spike position distance between recording and ground truth
spiketrain = 3; %ground_truth selected for performance evaluation
%peak_diff --> tolerance

%% Data loading
filename = 'monotrode_test_20';

signal = load([filename,'.mat']);
ground = load([filename,'_gt.mat']);









