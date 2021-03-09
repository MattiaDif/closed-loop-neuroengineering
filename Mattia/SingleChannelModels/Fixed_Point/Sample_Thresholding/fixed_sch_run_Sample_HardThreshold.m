clear all
close all
clc


%%%%%%%%% CHANGE THE noise_level VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
noise_level = 10;   %10, 20, 30
%%%%%%%%% CHANGE THE ch VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
ch = 'ch10';
%%%%%%%%% CHANGE THE mdl_name VARIABLE ACCORDING TO THE SIMULINK MODEL %%%%%%%%%
mdl_name = "Sample_HardThreshold";


result_flag = 0;    %1 --> save results, 0 --> not save


%% Simulation parameters
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
th=32408; % sweeping  thresholds, from -70 µV --> (-70*10^-6*192*(2^16-1))/(2*1.225) + 32768 and quantized using a voltage step size of 0.195 µV
sim_type = 'rapid'; %simulation speed
sim_stop_time = '10';   %s


%% Performance analysis parameters
w_len = fs/1000;  %samples --> 1ms
peak_diff = 15; %samples --> max spike position distance between recording and ground truth
spiketrain = 3; %ground_truth selected for performance evaluation
%peak_diff --> tolerance

%% Data loading
%worskpace saving --> sim parameters saving
if result_flag == 1
    save(['C:/File/IIT - Neuroengineering/Progetto MathWorks/Data/MEArec/ResultTable/sim_par_',convertStringsToChars(mdl_name),'_',num2str(noise_level),'.mat'])
end

filename = [ch,'_neuronexus32_recording_',num2str(noise_level)];

signal = load([filename,'.mat']);
ground = load([filename,'_gt.mat']);

load(['sim_results_',num2str(noise_level),'.mat']);


%% Simulation with different thresholds
numSims = length(th);   %number of simulation depending on number of thresholds

%Simulation parameters
mdl=convertStringsToChars(mdl_name);
load_system(mdl);
set_param(mdl, 'SimulationMode', sim_type)
set_param(mdl,'StartTime','0','StopTime',sim_stop_time)
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/threshold'],'DialogParameters')

%Input setting
for curr_sim = 1:numSims
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/threshold'], 'const', num2str(th(curr_sim)));
end

%Simulation running
out = sim(in,'ShowProgress', 'on');


%% Get simulation output
for curr_sim = 1:numSims
    
    simOut = out(curr_sim);
    float_filtered_recording_ts(curr_sim,:) = simOut.logsout.get('float_filtered_recording').Values;
    float_sample_above_th_ts(curr_sim,:) = simOut.logsout.get('float_sample_above_th').Values;
    float_spikes_ts(curr_sim,:) = simOut.logsout.get('float_spikes').Values;
    float_interspike_ts(curr_sim,:) = simOut.logsout.get('float_interspike').Values;
    
    float_filtered_recording(curr_sim,:) = float_filtered_recording_ts(curr_sim).Data;
    float_sample_above_th(curr_sim,:) = float_sample_above_th_ts(curr_sim).Data;
    float_spikes(curr_sim,:) = float_spikes_ts(curr_sim).Data;
    float_interspike(curr_sim,:) = float_interspike_ts(curr_sim).Data;
    
    
    fixed_filtered_recording_ts(curr_sim,:) = simOut.logsout.get('fixed_filtered_recording').Values;
    fixed_sample_above_th_ts(curr_sim,:) = simOut.logsout.get('fixed_sample_above_th').Values;
    fixed_spikes_ts(curr_sim,:) = simOut.logsout.get('fixed_spikes').Values;
    fixed_interspike_ts(curr_sim,:) = simOut.logsout.get('fixed_interspike').Values;
    
    fixed_filtered_recording(curr_sim,:) = fixed_filtered_recording_ts(curr_sim).Data;
    fixed_sample_above_th(curr_sim,:) = fixed_sample_above_th_ts(curr_sim).Data;
    fixed_spikes(curr_sim,:) = fixed_spikes_ts(curr_sim).Data;
    fixed_interspike(curr_sim,:) = fixed_interspike_ts(curr_sim).Data;
        
end


t_axis = (0:length(float_filtered_recording)-1)/fs;

figure
ax(1)=subplot(2,1,1);
stairs(t_axis,float_filtered_recording,'b','LineWidth',1),hold on
stairs(t_axis,fixed_filtered_recording,'r','LineWidth',1),hold off
title('Filtered recording - uint16')
set(gca,'FontSize',14)
legend('Floating','Fixed')

ax(2)=subplot(2,1,2);
stairs(t_axis,float_filtered_recording-fixed_filtered_recording,'k','LineWidth',1)
title('Difference')
xlabel('Time (s)')
set(gca,'FontSize',14)

linkaxes(ax,'x')



max_diff = max(abs(float_filtered_recording-fixed_filtered_recording));








