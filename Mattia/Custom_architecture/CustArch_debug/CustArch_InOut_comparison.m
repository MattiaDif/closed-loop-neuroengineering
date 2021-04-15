clear
close all
clc



%% Loading data
%input
load('fsm_out_chip10000000_nframe300.mat')
fsm_out_def_time = fsm_out_def_time.Data;

load('fsm_valid_chip10000000_nframe300.mat')
fsm_valid_def_time = fsm_valid_def_time.Data;


%output
load('custv2_out_chip10000000_nframe300.mat')
comp_data_out = comp_data_out.Data;

load('custv2_out_valid_chip10000000_nframe300.mat')
comp_data_out_valid = comp_data_out_valid.Data;



% indexing valid
idx_in = find(fsm_valid_def_time == 1);
idx_out = find(comp_data_out_valid == 1);


fsm_in = fsm_out_def_time(idx_in);
fsm_out = comp_data_out(idx_out);


figure
stem(fsm_in(1:length(fsm_out)),'r','LineWidth',1),hold on
stem(fsm_out,'b','LineWidth',1),hold off
title('Comparison custom architecture in/out')
legend('Input','Output')




