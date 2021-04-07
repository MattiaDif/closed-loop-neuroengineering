clear all
close all
clc


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


result_flag = 0;    %1 --> save results, 0 --> not save

filename = 'monotrode_recording_20';
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
%worskpace saving --> sim parameters saving
if result_flag == 1
    save(['C:/File/IIT - Neuroengineering/Progetto MathWorks/Data/MEArec/ResultTable/sim_par_',convertStringsToChars(mdl_name),'_',num2str(noise_level),'.mat'])
end

filename = 'monotrode_test_20';

signal = load([filename,'.mat']);
ground = load([filename,'_gt.mat']);

% load(['sim_results_',num2str(noise_level),'.mat']);


%% Simulation with different thresholds
numSims = length(score);   %number of simulation depending on number of thresholds

%Simulation parameters
mdl=convertStringsToChars(mdl_name);
load_system(mdl);
set_param(mdl, 'SimulationMode', sim_type)
set_param(mdl,'StartTime','0','StopTime',sim_stop_time)
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/template matching/score_th'],'DialogParameters')
% BlockDialogParameters = get_param([mdl '/template matching2/score_th'],'DialogParameters')
% BlockDialogParameters = get_param([mdl '/template matching2/score_th'],'DialogParameters')


%Input setting
for curr_sim = 1:numSims
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/template matching/score_th'], 'const', num2str(score(curr_sim)));
%     in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/template matching2/score_th'], 'const', num2str(score(curr_sim)));
%     in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/template matching3/score_th'], 'const', num2str(score(curr_sim)));
end

%Simulation running
out = sim(in,'ShowProgress', 'on');


%% Get simulation output
for curr_sim = 1:numSims
    
    simOut = out(curr_sim);
    ground_truth_ts(curr_sim,:) = simOut.logsout.get('ground_truth').Values;
    recording_ts(curr_sim,:) = simOut.logsout.get('recording').Values;
    spikes_ts(curr_sim,:) = simOut.logsout.get('spikes').Values;
    interspike_ts(curr_sim,:) = simOut.logsout.get('interspike').Values;
    
    recording(curr_sim,:) = recording_ts(curr_sim).Data;
    spikes(curr_sim,:) = spikes_ts(curr_sim).Data;
    interspike(curr_sim,:) = interspike_ts(curr_sim).Data;
    
    ground_truth(curr_sim,:) = zeros(1,size(recording,2));
    for train = 1:spiketrain
        ground_truth(curr_sim,:) = ground_truth(curr_sim,:) + ground_truth_ts(curr_sim).Data(:,train)';
    end

    

    % Performance evaluation
    P(curr_sim) = sum(round(ground_truth(curr_sim,:)));    %P    %round due to some quantization error (some samples were e-11 instead of 0)
    NDS(curr_sim) = sum(round(spikes(curr_sim,:)));  %NDS

    spikes_locks{curr_sim,:} = find(round(spikes(curr_sim,:)));    %samples
    ground_locks{curr_sim,:} = find(round(ground_truth(curr_sim,:))); %samples
    
    TP(curr_sim) = 0;
    for i=1:length(spikes_locks{curr_sim,:})
        locks_diff = [];
        TP_temp = [];
        locks_diff = abs(spikes_locks{curr_sim,:}(i) - ground_locks{curr_sim,:});
        TP_temp = find(locks_diff <= peak_diff);
        if isempty(TP_temp)
            TP(curr_sim) = TP(curr_sim);
        else
            TP(curr_sim) = TP(curr_sim) + 1;
        end
    end


    FN(curr_sim) = P(curr_sim) - TP(curr_sim);
    FP(curr_sim) = NDS(curr_sim) - TP(curr_sim);
    
    N(curr_sim) = ((length(recording(curr_sim,:)))-P(curr_sim)*w_len)/w_len;
    
    TN(curr_sim) = N(curr_sim) - FP(curr_sim);
    accuracy(curr_sim) = (TP(curr_sim) + TN(curr_sim))/(P(curr_sim)+N(curr_sim));
    perf(curr_sim) = TP(curr_sim)/(FP(curr_sim)+FN(curr_sim));
    eff(curr_sim) = perf(curr_sim)/(perf(curr_sim)+1);
    sens(curr_sim) = TP(curr_sim)/(TP(curr_sim)+FN(curr_sim));
    spec(curr_sim) = TN(curr_sim)/N(curr_sim);
    prec(curr_sim) = TP(curr_sim)/(TP(curr_sim)+FP(curr_sim));
    NPV(curr_sim) = TN(curr_sim)/(TN(curr_sim)+FN(curr_sim));
    FNR(curr_sim) = FN(curr_sim)/(FN(curr_sim)+TP(curr_sim));
    FPR(curr_sim) = FP(curr_sim)/(FP(curr_sim)+TP(curr_sim));
    F1score(curr_sim) = 2*TP(curr_sim)/(2*TP(curr_sim)+FN(curr_sim)+FP(curr_sim));
    MCC(curr_sim) = (TP(curr_sim)*TN(curr_sim)-FP(curr_sim)*FN(curr_sim))/...
                    sqrt((TP(curr_sim)+FP(curr_sim))*(TP(curr_sim)+FN(curr_sim))*(TN(curr_sim)+FP(curr_sim))*(TN(curr_sim)+FN(curr_sim)));

    FPrate(curr_sim) = FP(curr_sim)/N(curr_sim);
    TPrate(curr_sim) = TP(curr_sim)/P(curr_sim);
        
end


%% ROC, confusion matrix, AUC
FPrate = [1 FPrate 0];
TPrate = [1 TPrate 0];

figure
plot(flip(FPrate),flip(TPrate),'r','LineWidth',2)
xlabel('FP rate')
ylabel('TP rate')
title('Hard Threshold ROC')
set(gca,'FontSize',14)
axis([0 1 0 1])

AUC = -trapz(FPrate,TPrate);



%% Result saving
if result_flag == 1
    results_table{"P",mdl_name} = num2cell(P,2);
    results_table{"NDS",mdl_name} = num2cell(NDS,2);
    results_table{"TP",mdl_name} = num2cell(TP,2);
    results_table{"FN",mdl_name} = num2cell(FN,2);
    results_table{"FP",mdl_name} = num2cell(FP,2);
    results_table{"N",mdl_name} = num2cell(N,2);
    results_table{"TN",mdl_name} = num2cell(TN,2);
    results_table{"accuracy",mdl_name} = num2cell(accuracy,2);
    results_table{"perf",mdl_name} = num2cell(perf,2);
    results_table{"eff",mdl_name} = num2cell(eff,2);
    results_table{"sens",mdl_name} = num2cell(sens,2);
    results_table{"spec",mdl_name} = num2cell(spec,2);
    results_table{"prec",mdl_name} = num2cell(prec,2);
    results_table{"NPV",mdl_name} = num2cell(NPV,2);
    results_table{"FNR",mdl_name} = num2cell(FNR,2);
    results_table{"FPR",mdl_name} = num2cell(FPR,2);
    results_table{"F1score",mdl_name} = num2cell(F1score,2);
    results_table{"MCC",mdl_name} = num2cell(MCC,2);
    results_table{"FPrate",mdl_name} = num2cell(FPrate,2);
    results_table{"TPrate",mdl_name} = num2cell(TPrate,2);
    results_table{"AUC",mdl_name} = num2cell(AUC,2);
    
    save(['C:/File/IIT - Neuroengineering/Progetto MathWorks/Data/MEArec/ResultTable/sim_results_',num2str(noise_level),'.mat'],'results_table');   %std noise: 10

end










