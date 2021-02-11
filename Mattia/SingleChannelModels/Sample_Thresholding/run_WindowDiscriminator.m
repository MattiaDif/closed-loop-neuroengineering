clear all
close all
clc


%%%%%%%%% CHANGE THE noise_level VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
noise_level = 10;   %10, 20, 30
%%%%%%%%% CHANGE THE ch VARIABLE ACCORDING TO THE SIMULATION RECORDING %%%%%%%%%
ch = 'ch7';
%%%%%%%%% CHANGE THE mdl_name VARIABLE ACCORDING TO THE SIMULINK MODEL %%%%%%%%%
mdl_name = "WindowDiscriminator";


result_flag = 0;    %1 --> save results, 0 --> not save


%% Simulation parameters
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period

% inclusione exclusion amplitude thresholds
th1 = [-39.975]; th2 = [-148.98]; th3 = [-24.96]; th4 = [13.065];
th5 = [-11]; th6 = [-11]; th7 = [-11]; th8 = [-11];

DAC_stop_max = 15;  % if DAC_sto_max = 15 --> spike detected

% inclusion exclusion temporal thresholds
start1 = 0; stop1 = 1;
start2 = 1; stop2 = 6;
start3 = 4; stop3 = 11;
start4 = 3; stop4 = 15;
start5 = 3; stop5 = 2;
start6 = 3; stop6 = 2;
start7 = 3; stop7 = 2;
start8 = 3; stop8 = 2;

%DAC_type
DAC_type1 = 0; DAC_type2 = 1; DAC_type3 = 1; DAC_type4 = 1;
DAC_type5 = 0; DAC_type6 = 0; DAC_type7 = 0; DAC_type8 = 0;

%DAC_enable
DAC_enable1 = 1; DAC_enable2 = 1; DAC_enable3 = 1; DAC_enable4 = 1;
DAC_enable5 = 0; DAC_enable6 = 0; DAC_enable7 = 0; DAC_enable8 = 0;

sim_type = 'normal'; %simulation speed
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
numSims = 1;   %number of simulation depending on number of thresholds

%Simulation parameters
mdl=convertStringsToChars(mdl_name);
load_system(mdl);
set_param(mdl, 'SimulationMode', sim_type)
set_param(mdl,'StartTime','0','StopTime',sim_stop_time)
BlockPaths = find_system(mdl,'Type','Block')
% BlockDialogParameters = get_param([mdl '/threshold'],'DialogParameters')

%Input setting
for curr_sim = 1:numSims
    in(curr_sim) = Simulink.SimulationInput(mdl);
end

%Simulation running
out = sim(in,'ShowProgress', 'on');


%% Get simulation output
for curr_sim = 1:numSims
    
    simOut = out(curr_sim);
    ground_truth_ts(curr_sim,:) = simOut.logsout.get('ground_truth').Values;
    recording_ts(curr_sim,:) = simOut.logsout.get('recording').Values;
    DAC_fsm_counter_max_ts(curr_sim,:) = simOut.logsout.get('DAC_fsm_counter==max').Values;
    spikes_ts(curr_sim,:) = simOut.logsout.get('spikes').Values;
    interspike_ts(curr_sim,:) = simOut.logsout.get('interspike').Values;

    recording(curr_sim,:) = recording_ts(curr_sim).Data;
    DAC_fsm_counter_max(curr_sim,:) = DAC_fsm_counter_max_ts(curr_sim).Data;
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
        locks_diff = abs(ground_locks{curr_sim,:}(i) - ground_locks{curr_sim,:});
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
    
    save(['C:/File/IIT - Neuroengineering/Progetto MathWorks/Data/MEArec/ResultTable/sim_results_',num2str(noise_level),'.mat'],'results_table'); 

end










