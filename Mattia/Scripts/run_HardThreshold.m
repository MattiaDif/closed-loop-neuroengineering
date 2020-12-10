clear
close all
clc


%% Data loading and processing
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency


%% Simulation with different thresholds
th=[100]; % sweeping  thresholds
numSims = length(th);   %number of simulation depending on number of thresholds

%Simulation parameters
mdl='HardThreshold';
load_system(mdl);
set_param(mdl, 'SimulationMode', 'normal')
set_param(mdl,'StartTime','0','StopTime','10')
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
        above_th(curr_sim,:) = simOut.logsout.get('above_threshold').Values;
        ciao(curr_sim,:) = simOut.logsout.get('Extracellular time since last spike above threshold AND refractory').Values;
end

% above_th=above_th(1).Data;














