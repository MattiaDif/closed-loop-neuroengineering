%% goal of the script: run multiple times the hard threshold model
clear
close all
clc

%% Change the current folder to the folder of this m-file.
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
cd ..
load('data\spike_train')
cd('models')

%% running a parallel simulation with different thresholds
th_sweep=-1:-1:-100; %% sweeping  thresholds
numSims = length(th_sweep);

mdl='hard_threshold_fixed';
load_system(mdl);
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/Compare_to_threshold'],'DialogParameters')
%%
for curr_sim = numSims:-1:1
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/Compare_to_threshold'], 'const', num2str(th_sweep(curr_sim)));
end
out = parsim(in, 'ShowProgress', 'on');

%% getting the number of samples below threshold
ts=timeseries;
sum_1=zeros(numSims,1);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts(curr_sim,:) = simOut.logsout.get('signal_below_th').Values;
        sum_1(curr_sim,1)=sum(ts(curr_sim,:).Data);
end
figure
plot(th_sweep,sum_1)
xlabel('threshold')
ylabel('# of samples below threshold')
title('# of samples below threshold vs threshold')

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
plot(th_sweep,n_edges)
hold on
plot([th_sweep(1) th_sweep(end)],[n_ground_truth n_ground_truth],'-.')
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
plot(th_sweep,n_matched_spikes)
hold on
plot([th_sweep(1) th_sweep(end)],[n_ground_truth n_ground_truth],'-.')
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
plot(th_sweep,n_matched_ref_spikes)
hold on
plot([th_sweep(1) th_sweep(end)],[n_ground_truth n_ground_truth],'-.')
xlabel('threshold')
ylabel('# True positives')
title('matched spikes (above refractory period) per threshold')
legend({'simulations','ground truth'},'Location','Best')
ylim([-1 n_ground_truth+5])
subplot(1,2,2)
plot(th_sweep,n_false_positive)
title('False positive')
xlabel('threshold')
ylabel('# False positives')
curr_lim=ylim;
ylim([-10 curr_lim(2)])






