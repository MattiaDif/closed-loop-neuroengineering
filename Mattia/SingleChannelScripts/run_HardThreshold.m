clear all
close all
clc


%% Data loading and processing
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
w_len = fs/1000;  %samples --> 1ms
peak_diff = 10; %samples --> max spike position distance between EC and ground truth


%% Simulation with different thresholds
th=-[25:12.5:75]; % sweeping  thresholds
numSims = length(th);   %number of simulation depending on number of thresholds


%Simulation parameters
mdl='HardThreshold';
load_system(mdl);
set_param(mdl, 'SimulationMode', 'rapid')
set_param(mdl,'StartTime','0','StopTime','120')
BlockPaths = find_system(mdl,'Type','Block')
BlockDialogParameters = get_param([mdl '/Threshold'],'DialogParameters')

%Input setting
for curr_sim = 1:numSims
    in(curr_sim) = Simulink.SimulationInput(mdl);
    in(curr_sim) = setBlockParameter(in(curr_sim), [mdl '/Threshold'], 'const', num2str(th(curr_sim)));
end

%Simulation running
out = sim(in,'ShowProgress', 'on');


%% Get simulation output
for curr_sim = 1:numSims
    
    simOut = out(curr_sim);
    edge_ts(curr_sim,:) = simOut.logsout.get('Edge').Values;
    EC_ts(curr_sim,:) = simOut.logsout.get('EC').Values;
%     microEC_f_abs_ts(curr_sim,:) = simOut.logsout.get('microEC_f_abs').Values;
%     microEC_f_ts(curr_sim,:) = simOut.logsout.get('microEC_f').Values;
%     EC_f_ts(curr_sim,:) = simOut.logsout.get('EC_f').Values;
    interspike_ts(curr_sim,:) = simOut.logsout.get('interspike').Values;
    spikes_ts(curr_sim,:) = simOut.logsout.get('spikes').Values;
    EC_above_ts(curr_sim,:) = simOut.logsout.get('EC_above').Values;
    ground_point_ts(curr_sim,:) = simOut.logsout.get('ground_point').Values;

    edge(curr_sim,:) = edge_ts(curr_sim).Data;
    EC(curr_sim,:) = EC_ts(curr_sim).Data;
%     microEC_f_abs(curr_sim,:) = microEC_f_abs_ts(curr_sim).Data;
%     microEC_f(curr_sim,:) = microEC_f_ts(curr_sim).Data;
%     EC_f(curr_sim,:) = EC_f_ts(curr_sim).Data;
    interspike(curr_sim,:) = interspike_ts(curr_sim).Data;
    spikes(curr_sim,:) = spikes_ts(curr_sim).Data;
    EC_above(curr_sim,:) = EC_above_ts(curr_sim).Data;
    ground_point(curr_sim,:) = ground_point_ts(curr_sim).Data;



    % Performance evaluation
    P(curr_sim) = sum(round(ground_point(curr_sim,:)));    %P    %round due to some quantization error (some samples were e-11 instead of 0)
    NDS(curr_sim) = sum(round(spikes(curr_sim,:)));  %NDS

    spikes_locks{curr_sim,:} = find(round(spikes(curr_sim,:)));    %samples
    ground_locks{curr_sim,:} = find(round(ground_point(curr_sim,:))); %samples
    cont = 0;
    for i=1:length(ground_locks{curr_sim,:})
        for j=1:length(spikes_locks{curr_sim,:})
            if  abs(spikes_locks{curr_sim}(j)-ground_locks{curr_sim}(i)) <= peak_diff
                    TP(curr_sim) = cont + 1;    %TP
                    cont = cont + 1;
            end
        end
    end

    FN(curr_sim) = P(curr_sim) - TP(curr_sim);
    FP(curr_sim) = NDS(curr_sim) - TP(curr_sim);
    
    N(curr_sim) = ((length(EC(curr_sim,:)))-P(curr_sim)*w_len)/w_len;
    
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
    MCC(curr_sim) = (TP(curr_sim)*TN(curr_sim))/sqrt((TP(curr_sim)+FP(curr_sim))*(TP(curr_sim)+FN(curr_sim))*(TN(curr_sim)+FP(curr_sim))*(TN(curr_sim)+FN(curr_sim)));

    FPrate(curr_sim) = FP(curr_sim)/N(curr_sim);
    TPrate(curr_sim) = TP(curr_sim)/P(curr_sim);
        
end


%% ROC, confusion matrix, AUC

FPrate = [1 FPrate 0];
TPrate = [1 TPrate 0];

figure
plot((FPrate),TPrate,'r','LineWidth',2)
xlabel('FP rate')
ylabel('TP rate')
title('Hard Threshold ROC')
set(gca,'FontSize',14)
axis([0 1 0 1])

AUC = trapz(FPrate,TPrate);










