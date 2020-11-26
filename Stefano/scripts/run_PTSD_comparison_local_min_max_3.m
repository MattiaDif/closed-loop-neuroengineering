%% goal of the script: run multiple times the hard threshold model
clear
close all
clc

%% Change the current folder to the folder of this m-file.
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
cd ..
load('data\spike_train_single_ch.mat')
load('data\rec_single_ch.mat')
cd('models')

%%
data_in=rec_single_ch(2,:);
threshold=40;
fs=32e3;
w_pre=2;
w_post=2;
DMdpolar='negative';
PLP_ms=1;
RP_ms=2;
% function [data_out,dthresh]=SpikeDetectionArray_vOrig1(data_in,th_type,th_factor,fs,w_pre,w_post,DMdpolar,PLP,RP)
%% Parameters: Can be changed
peakDuration_samples    =   round(PLP_ms*1e-3*fs);   % Pulse lifetime period in samples
refrTime_samples        =   round(RP_ms*1e-3*fs);   % Refractory time in samples
stim_artifacts  =   -1;                 % No Analog Raw Data folder
interpolation   =   1;
w_pre           =   round(w_pre*1e-3*fs);
w_post          =   round(w_post*1e-3*fs);
ls              =   w_pre+w_post;
art_thresh_elec =   350;
art_dist        =   1/35*fs;               % Maximum Stimulation frequency

%%
if strcmp(DMdpolar,'negative')          % Alignment flag for detection
    alignFlag       =   1;
elseif  strcmp(DMdpolar,'highest')
    alignFlag       =   0;
else
    %    fprintf('Default search polarity is negative');
    alignFlag       =   1;
end

%% compile mex if that's the first time
% mex SpikeDetection_PTSD_core.cpp;  %Compile mex

%%
[spkValues, spkTimeStamps] = SpikeDetection_PTSD_core(double(data_in)', threshold, peakDuration_samples, refrTime_samples, alignFlag);
spikesTime_cpp  = 1 + spkTimeStamps( spkTimeStamps > 0 ); % +1 added to accomodate for zero- (c) or one-based (matlab) array indexing
dthresh= []; %DiffThr( spkTimeStamps > 0 );
spikesValue = spkValues( spkTimeStamps > 0 );
%% this is to exclude spikes too close to the start and end
spikesValue(spikesTime_cpp<=w_pre+1 | spikesTime_cpp>=length(data_in)-w_post-2)=[];
spikesTime_cpp(spikesTime_cpp<=w_pre+1 | spikesTime_cpp>=length(data_in)-w_post-2)=[];
nspk = length(spikesTime_cpp);

%% plot results
% figure
% plot(data_in)
% hold on
% % plot(spikesTime,spikesValue,'r*') % spike value is the absolute of the difference of two peaks
% plot(spikesTime,data_in(spikesTime),'r*')

%% running a simulation of the Simulink model with the same parameters
PLP_sweep=peakDuration_samples; %% sweeping  thresholds
numSims = length(PLP_sweep);

% mdl='PTSD_single_channel_buffer_2';
% mdl='PTSD_single_channel_local_min_max_2';
mdl='PTSD_single_channel_local_min_max_9';
load_system(mdl);
BlockPaths = find_system(mdl,'Type','Block')
% BlockDialogParameters = get_param([mdl '/within_PLP'],'DialogParameters')
%%
for curr_sim = numSims:-1:1
    in(curr_sim) = Simulink.SimulationInput(mdl);
%     in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/within_PLP'], 'const', num2str(PLP_sweep(curr_sim)));
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/differential_threshold'], 'const', num2str(threshold));
%     in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/check_refractory_1/Enabling_condition'], 'const', num2str((RP_ms)/1e3));
end
% out = parsim(in, 'ShowProgress', 'on');
out = sim(in, 'ShowProgress', 'off');

%% getting the number of samples below threshold
edge_detected=timeseries;
peak_to_peak_above_th=timeseries;
spike_above_refractory=timeseries;
ts_above_ref=cell(numSims,1);
aligned_min=timeseries;
sum_spikes_above_ref=zeros(numSims,1);

for curr_sim = 1:numSims
        simOut = out(curr_sim);
        edge_detected(curr_sim,:) = simOut.logsout.get('edge_detected').Values;
        peak_to_peak_above_th(curr_sim,:) = simOut.logsout.get('peak_to_peak_above_th').Values;
        spike_above_refractory(curr_sim,:) = simOut.logsout.get('spike_above_refractory').Values;
        aligned_min(curr_sim,:) = simOut.logsout.get('aligned_min').Values;
        ts_above_ref{curr_sim,1}=find(squeeze(spike_above_refractory(curr_sim,:).Data));
end
ts_above_ref{1,1}(ts_above_ref{1,1}<=w_pre+1 | ts_above_ref{1,1}>=length(data_in)-w_post-2)=[];

%% align values
values_align=squeeze(aligned_min.Data(1,1,ts_above_ref{1,1}));

%% spikes above ref
figure
ts_above_ref_aligned=ts_above_ref{1,1}-39+values_align;
plot(ts_above_ref_aligned./fs,ones(length(ts_above_ref{1,1})),'bo')
hold on
plot(spikesTime_cpp./fs,ones(nspk,1),'r.')
title(['comparison cpp ' num2str(nspk) ' vs Simulink ' num2str(length(ts_above_ref{curr_sim,1}))])

%% check distance
figure
plot(diff(ts_above_ref_aligned),'b')
hold on
plot(diff(spikesTime_cpp),'r')
plot([0 length(ts_above_ref{1,1})],[64 64])
ylabel('ISI [samples]')
legend({'simulink','cpp',['RP= ' num2str((RP_ms)/1e3) ' ms']})
xlabel('spike number')

%% figure plot differences cpp - simulink
figure
if length(spikesTime_cpp)>length(ts_above_ref_aligned)
    plot(spikesTime_cpp(1:length(ts_above_ref_aligned))./fs,spikesTime_cpp(1:length(ts_above_ref_aligned))-ts_above_ref_aligned)
else
    plot(spikesTime_cpp./fs,spikesTime_cpp-ts_above_ref_aligned(1:length(spikesTime_cpp)))
end
ylabel('samples')
title('difference in samples cpp - simulink')
xlabel('time [s]')

%% this was to check the behavior
% time=simOut.logsout.get('time').Values;
% great_th=simOut.logsout.get('great_eq').Values;
% comparison_ms=simOut.logsout.get('comparison_ms').Values;
% 
% figure
% h(1)=subplot(3,1,1);
% plot(time,'.')
% hold on
% plot(spike_above_refractory(1,:),'.')
% 
% h(2)=subplot(3,1,2);
% plot(great_th)
% hold on
% plot(time.Time,time.Data>=2*1e-3)
% 
% h(3)=subplot(3,1,3);
% plot(comparison_ms)
% linkaxes(h,'x')
