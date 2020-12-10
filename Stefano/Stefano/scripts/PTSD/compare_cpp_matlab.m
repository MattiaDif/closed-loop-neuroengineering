clear
clc
close all
%% Change the current folder to the folder of this m-file.
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
cd ..
cd ..
load('data\rec_single_ch.mat')

%%
data_in=rec_single_ch(2,:);
thresh=100;
fs=32e3;
w_pre=1;
w_post=1;
DMdpolar='negative';
PLP=1;
RP=2;
% function [data_out,dthresh]=SpikeDetectionArray_vOrig1(data_in,th_type,th_factor,fs,w_pre,w_post,DMdpolar,PLP,RP)
%% Parameters: Can be changed
peakDuration    =   floor(PLP*1e-3*fs);   % Pulse lifetime period in samples
refrTime        =   floor(RP*1e-3*fs);   % Refractory time in samples
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
[spkValues, spkTimeStamps] = SpikeDetection_PTSD_core(double(data_in)', thresh, peakDuration, refrTime, alignFlag);
spikesTime  = 1 + spkTimeStamps( spkTimeStamps > 0 ); % +1 added to accomodate for zero- (c) or one-based (matlab) array indexing
dthresh= []; %DiffThr( spkTimeStamps > 0 );
spikesValue = spkValues( spkTimeStamps > 0 );
%% this is to exclude spikes too close to the start and end
spikesValue(spikesTime<=w_pre+1 | spikesTime>=length(data_in)-w_post-2)=[];
spikesTime(spikesTime<=w_pre+1 | spikesTime>=length(data_in)-w_post-2)=[];
nspk = length(spikesTime);

%% plot results
figure
plot(data_in)
hold on
% plot(spikesTime,spikesValue,'r*') % spike value is the absolute of the difference of two peaks
plot(spikesTime,data_in(spikesTime),'r*')

%% compare with the matlab version
[spkValues_matlab, spkTimeStamps_matlab] = SpikeDetection_PTSD_core_matlab(double(data_in)', thresh, peakDuration, refrTime, alignFlag);
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