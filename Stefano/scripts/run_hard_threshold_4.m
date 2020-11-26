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
% th_sweep=-1:-1:-100; %% sweeping  thresholds
th_sweep=-1; %% sweeping  thresholds
numSims = length(th_sweep);

mdl='hard_threshold_fixed_4';
load_system(mdl);
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/Compare_to_threshold'],'DialogParameters')
%%
for curr_sim = numSims:-1:1
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/Compare_to_threshold'], 'const', num2str(th_sweep(curr_sim)));
end
% out = parsim(in, 'ShowProgress', 'on');
out = sim(in, 'ShowProgress', 'on');
%% getting the number of edges detected below threshold 
n_ground_truth=sum(spike_train(2,:));

%% get spikes above refractory period (true positives)
ts_ref=timeseries;
above_th_ref=timeseries;
edge=timeseries;
n_above_refractory=zeros(numSims,1);
for curr_sim = 1:numSims
        simOut = out(curr_sim);
        ts_ref(curr_sim,:) = simOut.logsout.get('spike_above_refractory').Values;
        above_th_ref(curr_sim,:) = simOut.logsout.get('above_th').Values;
        edge(curr_sim,:) = simOut.logsout.get('edge').Values;
        n_above_refractory(curr_sim,1)=sum(ts_ref(curr_sim,:).Data);
end

%% compare spike above ref with findpeaks
load('data\rec_single_ch.mat')
%%
ts_ref_d=ts_ref(1).Data;
ts_ref_t=ts_ref.Time;
above=ts_ref_d>0;
above_th_ref_d=above_th_ref(1).Data;
edge_d=edge(1).Data;
%%
% figure
% plot(ts_ref_t(above),ones(sum(above)),'o')
% hold on
% plot(rec_single_ch(1,:),rec_single_ch(2,:)<=-1)

%% just to confirm that simulink is doing it's job correctly
n_spk_simulink=sum(above);
ts_simulink=find(above);
[pk,loc]=findpeaks(double(ts_ref_d),'MinPeakHeight',0,'MinPeakDistance',32); %input is spike above refractory
[pk_2,loc_2]=findpeaks(double(edge_d),'MinPeakHeight',0,'MinPeakDistance',32); %input is above threshold from simulink
loc_2(loc_2<=32)=[];
above_th_matlab=rec_single_ch(2,:)<=th_sweep;
[pk_3,loc_3]=findpeaks(double(above_th_matlab'),'MinPeakHeight',0,'MinPeakDistance',32); %input is above threshold from matlab
loc_3(loc_3<=32)=[];
[pk_4,loc_4]=findpeaks(double(-rec_single_ch(2,:)),'MinPeakHeight',1,'MinPeakDistance',32); %input is raw signal from matlab is different
[pk_5,loc_5]=findpeaks(double(diff(above_th_matlab')),'MinPeakHeight',0,'MinPeakDistance',32); %input is above threshold from matlab
loc_5(loc_5<=32)=[];
%% plot to understand that findpeaks when flat gives only the first sample, while in simulink if a flat is longer than 1ms, it returns a spike after a ms
figure
histogram(diff(find(above)),'BinWidth',1)
hold on
histogram(diff(loc_3),'BinWidth',1)
legend({'hard threshold simulink','find peaks using above th from matlab'})
xlabel('ISI [samples]')

%% findpeaks works the same way
% figure
% plot(loc_2-loc_3)

%% final understanding
figure
plot(loc-loc_2)
title('difference from simulink and from findpeaks in matlab from edge')

