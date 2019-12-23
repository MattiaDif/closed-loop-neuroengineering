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
threshold=50;
fs=32e3;
w_pre=1;
w_post=1;
DMdpolar='negative';
PLP_ms=1;
RP_ms=2;
% function [data_out,dthresh]=SpikeDetectionArray_vOrig1(data_in,th_type,th_factor,fs,w_pre,w_post,DMdpolar,PLP,RP)
%% Parameters: Can be changed
peakDuration_samples    =   floor(PLP_ms*1e-3*fs);   % Pulse lifetime period in samples
refrTime_samples        =   floor(RP_ms*1e-3*fs);   % Refractory time in samples
stim_artifacts  =   -1;                 % No Analog Raw Data folder
interpolation   =   1;
w_pre           =   floor(w_pre*1e-3*fs);
w_post          =   floor(w_post*1e-3*fs);
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
spikesTime  = 1 + spkTimeStamps( spkTimeStamps > 0 ); % +1 added to accomodate for zero- (c) or one-based (matlab) array indexing
dthresh= []; %DiffThr( spkTimeStamps > 0 );
spikesValue = spkValues( spkTimeStamps > 0 );
%% this is to exclude spikes too close to the start and end
spikesValue(spikesTime<=w_pre+1 | spikesTime>=length(data_in)-w_post-2)=[];
spikesTime(spikesTime<=w_pre+1 | spikesTime>=length(data_in)-w_post-2)=[];
nspk = length(spikesTime);

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
mdl='PTSD_single_channel_local_min_max';
load_system(mdl);
BlockPaths = find_system(mdl,'Type','Block')
% BlockDialogParameters = get_param([mdl '/within_PLP'],'DialogParameters')
%%
for curr_sim = numSims:-1:1
    in(curr_sim) = Simulink.SimulationInput(mdl);
%     in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/within_PLP'], 'const', num2str(PLP_sweep(curr_sim)));
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/differential_threshold'], 'const', num2str(threshold));
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/check_refractory/Enabling_condition'], 'const', num2str(RP_ms/1e3));
end
% out = parsim(in, 'ShowProgress', 'on');
out = sim(in, 'ShowProgress', 'on');

%% getting the number of samples below threshold
edge_detected=timeseries;
peak_to_peak_above_th=timeseries;
spike_above_refractory=timeseries;
ts_above_ref=cell(numSims,1);

sum_spikes_above_ref=zeros(numSims,1);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        edge_detected(curr_sim,:) = simOut.logsout.get('edge_detected').Values;
        peak_to_peak_above_th(curr_sim,:) = simOut.logsout.get('peak_to_peak_above_th').Values;
        spike_above_refractory(curr_sim,:) = simOut.logsout.get('spike_above_refractory').Values;
        
        sum_spikes_above_ref(curr_sim,1)=sum(spike_above_refractory(curr_sim,:).Data);
        ts_above_ref{curr_sim,1}=find(spike_above_refractory(curr_sim,:).Data);
end
%% plot comparison
for curr_sim = 1:numSims
    figure
    plot(ts_above_ref{curr_sim,1}./fs,ones(sum_spikes_above_ref(curr_sim,1),1),'bo')
    hold on
    plot(spikesTime./fs,ones(nspk,1),'r.')
    title(['comparison cpp ' num2str(nspk) ' vs Simulink ' num2str(length(ts_above_ref{curr_sim,1}))])
    legend({'Simulink','cpp'})
end
%%
figure
if nspk<length(ts_above_ref{curr_sim,1})
    plot(ts_above_ref{curr_sim,1}(1:nspk)/fs,ts_above_ref{curr_sim,1}(1:nspk)-spikesTime)
else
    plot(ts_above_ref{curr_sim,1}/fs,ts_above_ref{curr_sim,1}-spikesTime(1:sum_spikes_above_ref(curr_sim,1)))
end
ylabel('difference in samples')
title('Simulink timestamps - cpp timestamps')

%% getting the number of edges detected below threshold 
n_ground_truth=sum(spike_train(2,:));
ts=timeseries;
n_edges=zeros(numSims,1);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts(curr_sim,:) = simOut.logsout.get('edge_detected').Values;
        n_edges(curr_sim,1)=sum(ts(curr_sim,:).Data);
end
figure
plot(PLP_sweep,n_edges)
hold on
plot([PLP_sweep(1) PLP_sweep(end)],[n_ground_truth n_ground_truth],'-.')
xlabel('threshold')
ylabel('# of edges below threshold detected')
title('simulation (edge det) vs ground truth')
legend({'simulations','ground truth'})

%% check matched spikes 
fs=32e3; % WARNING
tol=round(0.0005*fs); %tolerance for two spikes to be matched
match_timestamps_per_sim=cell(numSims,1);
for curr_sim = 1:numSims
        match_timestamps_per_sim{curr_sim,1}=find(ts(curr_sim).Data);
end
ts_ground_truth=find(spike_train(2,:));
n_matched_spikes=zeros(numSims,1);
for curr_sim = 1:numSims
    ts_curr_sim=match_timestamps_per_sim{curr_sim,1};
    n_spikes_curr_sim=length(ts_curr_sim);
    for curr_spike=1:n_spikes_curr_sim
        if min(abs(ts_curr_sim(curr_spike)-ts_ground_truth)) <= tol
            n_matched_spikes(curr_sim,1)=n_matched_spikes(curr_sim,1)+1;
        end
    end
end
%% plot matched spikes (true positive) problem is that there's no 
%% refractory and thus it's possible for 2 edges to be so close they are counted twice as matched spikes
figure
plot(PLP_sweep,n_matched_spikes)
hold on
plot([PLP_sweep(1) PLP_sweep(end)],[n_ground_truth n_ground_truth],'-.')
xlabel('threshold')
ylabel('# of matched spikes')
title('matched spikes per threshold')
legend({'simulations','ground truth'})


%% get spikes above refractory period (true positives)
ts_ref=timeseries;
n_above_refractory=zeros(numSims,1);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts_ref(curr_sim,:) = simOut.logsout.get('spike_above_refractory').Values;
        n_above_refractory(curr_sim,1)=sum(ts_ref(curr_sim,:).Data);
end
match_ref_timestamps_per_sim=cell(numSims,1);
for curr_sim = 1:numSims
        match_ref_timestamps_per_sim{curr_sim,1}=find(ts_ref(curr_sim).Data);
end
ts_ground_truth=find(spike_train(2,:));
n_false_positive=zeros(numSims,1);
n_matched_ref_spikes=zeros(numSims,1);
for curr_sim = 1:numSims
    ts_curr_sim=match_ref_timestamps_per_sim{curr_sim,1};
    n_spikes_curr_sim=length(ts_curr_sim);
    for curr_spike=1:n_spikes_curr_sim
        if min(abs(ts_curr_sim(curr_spike)-ts_ground_truth)) <= tol
            n_matched_ref_spikes(curr_sim,1)=n_matched_ref_spikes(curr_sim,1)+1;
        else
            n_false_positive(curr_sim,1)=n_false_positive(curr_sim,1)+1;
        end
    end
end
figure
subplot(1,2,1)
plot(PLP_sweep,n_matched_ref_spikes)
hold on
plot([PLP_sweep(1) PLP_sweep(end)],[n_ground_truth n_ground_truth],'-.')
xlabel('threshold')
ylabel('# True positives')
title('matched spikes (above refractory period) per threshold')
legend({'simulations','ground truth'},'Location','Best')
ylim([-1 n_ground_truth+5])
subplot(1,2,2)
plot(PLP_sweep,n_false_positive)
title('False positive')
xlabel('threshold')
ylabel('# False positives')
curr_lim=ylim;
ylim([-10 curr_lim(2)])

%% compare with the simulink version
[spkValues_matlab, spkTimeStamps_matlab] = SpikeDetection_PTSD_core_matlab(double(data_in)', threshold, peakDuration_samples, refrTime_samples, alignFlag);
spikesTime_matlab  = spkTimeStamps_matlab(spkTimeStamps_matlab > 0);
spikesValue_matlab = spkValues_matlab( spkTimeStamps_matlab > 0 );
%% this is to exclude spikes too close to the start and end
spikesValue_matlab(spikesTime_matlab<=w_pre+1 | spikesTime_matlab>=length(data_in)-w_post-2)=[];
spikesTime_matlab(spikesTime_matlab<=w_pre+1 | spikesTime_matlab>=length(data_in)-w_post-2)=[];
nspk_matlab = length(spikesTime_matlab);

%% compare the two versions 
figure
plot(spikesTime_matlab,spikesValue_matlab,'r.')
hold on
plot(spikesTime,spikesValue,'bO')
title('comparing cpp and matlab')
legend({'from matlab','from cpp'})
disp(['sum(difference (matlab - cpp)) = ' num2str(sum(spikesValue_matlab-spikesValue))])




