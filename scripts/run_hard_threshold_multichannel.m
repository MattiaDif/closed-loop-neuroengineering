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
load('data\rec')
cd('models')

%% running a parallel simulation with different thresholds
th_sweep=-1:-1:-100; %% sweeping  thresholds
numSims = length(th_sweep);

mdl='hard_threshold_fixed_multichannel';
load_system(mdl);
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/Compare_to_threshold'],'DialogParameters')
%%
for curr_sim = numSims:-1:1
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/Compare_to_threshold'], 'const', num2str(th_sweep(curr_sim)));
end
out = parsim(in, 'ShowProgress', 'on');
% cd ..
% cd('sim_output')
% save('sim_out_32','out','-v7.3') % huge files
%% getting the number of samples below threshold
n_el=size(rec,1)-1;
sum_1=zeros(numSims,n_el);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts = simOut.logsout.get('signal_below_th').Values;
        sum_1(curr_sim,:)=sum(ts.Data);
end
figure
plot(th_sweep,sum_1)
xlabel('threshold')
ylabel('# of samples below threshold')
title('# of samples below threshold vs threshold')

%% getting the number of edges detected below threshold 
n_ground_truth=sum(spike_train(2:end,:),2);
n_edges=zeros(numSims,n_el);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts(curr_sim) = simOut.logsout.get('edge_detected').Values;
        n_edges(curr_sim,:)=sum(ts(curr_sim).Data);
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
match_timestamps_per_sim=cell(numSims,n_el);
for curr_el=1:n_el
    for curr_sim = 1:numSims
        match_timestamps_per_sim{curr_sim,curr_el}=find(ts(curr_sim).Data(:,curr_el));
    end
end
ts_ground_truth=cell(10,1);
for curr_cell=1:10
    ts_ground_truth{curr_cell,1}=find(spike_train(curr_cell+1,:));
end
n_matched_spikes=zeros(numSims,n_el,10);
for curr_cell=1:10
    for curr_el=1:n_el
        for curr_sim = 1:numSims
            ts_curr_sim=match_timestamps_per_sim{curr_sim,curr_el};
            n_spikes_curr_sim=length(ts_curr_sim);
            for curr_spike=1:n_spikes_curr_sim
                if min(abs(ts_curr_sim(curr_spike)-ts_ground_truth{curr_cell,1})) <= tol
                    n_matched_spikes(curr_sim,curr_el,curr_cell)=n_matched_spikes(curr_sim,curr_el,curr_cell)+1;
                end
            end
        end
    end
end
%% plot matched spikes (true positive) problem is that there's no 
%% refractory and thus it's possible for 2 edges to be so close they are counted twice as matched spikes
figure
for curr_cell=1:10
    subplot(2,5,curr_cell)
    plot(th_sweep,n_matched_spikes(:,:,curr_cell))
    hold on
    plot([th_sweep(1) th_sweep(end)],[n_ground_truth(curr_cell) n_ground_truth(curr_cell)],'-.')
    xlabel('threshold')
    ylabel('# of matched spikes')
    title(['matched spikes, curr cell: ' num2str(curr_cell)])
end

%% get spikes above refractory period (true positives)
ts_ref=timeseries;
n_above_refractory=zeros(numSims,n_el);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts_ref(curr_sim,:) = simOut.logsout.get('spike_above_refractory').Values;
        n_above_refractory(curr_sim,:)=sum(ts_ref(curr_sim,:).Data);
end
match_ref_timestamps_per_sim=cell(numSims,n_el);
for curr_el=1:n_el
    for curr_sim = 1:numSims
        match_ref_timestamps_per_sim{curr_sim,n_el}=find(ts_ref(curr_sim).Data(:,curr_el));
    end
end

n_false_positive=zeros(numSims,n_el,10);
n_matched_ref_spikes=zeros(numSims,n_el,10);
for curr_cell=1:10
    for curr_el=1:n_el
        for curr_sim = 1:numSims
            ts_curr_sim=match_ref_timestamps_per_sim{curr_sim,curr_el};
            n_spikes_curr_sim=length(ts_curr_sim);
            for curr_spike=1:n_spikes_curr_sim
                if min(abs(ts_curr_sim(curr_spike)-ts_ground_truth{curr_cell,1})) <= tol
                    n_matched_ref_spikes(curr_sim,curr_el,curr_cell)=n_matched_ref_spikes(curr_sim,curr_el,curr_cell)+1;
                else
                    n_false_positive(curr_sim,curr_el,curr_cell)=n_false_positive(curr_sim,curr_el,curr_cell)+1;
                end
            end
        end
    end
end

%% plot
for curr_cell=1:10
    figure
    subplot(1,2,1)
    plot(th_sweep,n_matched_ref_spikes(:,:,curr_cell))
    hold on
    plot([th_sweep(1) th_sweep(end)],[n_ground_truth(curr_cell) n_ground_truth(curr_cell)],'-.')
    xlabel('threshold')
    ylabel('# True positives')
    title('matched spikes (above refractory period) per threshold')
    legend({'simulations','ground truth'},'Location','Best')
    
    subplot(1,2,2)
    plot(th_sweep,n_false_positive(:,:,curr_cell))
    title('False positive')
    xlabel('threshold')
    ylabel('# False positives')
    curr_lim=ylim;
    ylim([-10 curr_lim(2)])
end






