clear
close all
clc



%% Loading data
%input
load('fsm_out.mat')
fsm_out_def_time = fsm_out_def_time.Data;

load('fsm_valid.mat')
fsm_valid_def_time = fsm_valid_def_time.Data;


%output
load('comp_data_out.mat')
comp_data_out = comp_data_out.Data;

load('comp_data_out_valid.mat')
comp_data_out_valid = comp_data_out_valid.Data;



% indexing valid
idx_in = find(fsm_valid_def_time == 1);
idx_out = find(comp_data_out_valid == 1);


fsm_in = fsm_out_def_time(idx_in);
fsm_out = comp_data_out(idx_out);


figure
plot(fsm_in),hold on
plot(fsm_out),hold off
title('Comparison custom architecture in/out')
legend('Input','Output')