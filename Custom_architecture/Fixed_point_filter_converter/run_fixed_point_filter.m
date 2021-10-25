%% This script load the parameters necessary for the fixed-point conversion of the filters

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


%% Simulation parameters
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency


%% Data loading
filename = 'C:\File\IIT - Neuroengineering\Progetto MathWorks\Dataset\Intan\CustArch_v10_210526_110108_ch4';

signal = load([filename,'.mat']);



%% Fixed-point conversion 
% model = 'fixed_point_filter';
% sub = 'fixed_point_filter/digital_filter';
% 
% %make the option object
% options = fxpOptimizationOptions();
% options.MaxTime = 15*60;    %15 minutes of search
% options.MaxIterations = 100;    %100 iterations
% 
% %set tolerances
% addTolerance(options, [model '/filter_output'], 1, 'RelTol', 1e-2);
% addTolerance(options, [model '/filter_output'], 1, 'AbsTol', 5e-3);
% 
% %run optimization
% result = fxpopt(model, sub, options);
% 
% %explore best fixed-point design
% bestSolution = explore(result);








