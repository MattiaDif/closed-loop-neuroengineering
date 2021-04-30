clear
close all
clc



n_chip = 1; % number of data stream enabled in the data simulated

%% Loading data
%raw input
load('raw_fsm_out_chip10000000_nframe300.mat')

% load('raw_fsm_valid_chip10000000_nframe300.mat')


%input
load('fsm_out_chip10000000_nframe300.mat')
fsm_out_def_time = fsm_out_def_time.Data;

load('fsm_valid_chip10000000_nframe300.mat')
fsm_valid_def_time = fsm_valid_def_time.Data;


%output
load('custv5_out_chip10000000_nframe300.mat')
comp_data_out = comp_data_out.Data;

load('custv5_out_valid_chip10000000_nframe300.mat')
comp_data_out_valid = comp_data_out_valid.Data;



% indexing 
idx_in = find(fsm_valid_def_time == 1);
idx_out = find(comp_data_out_valid == 1);


raw_fsm_in = fsm_out_def(idx_in);
raw_idx_in_DC = find(raw_fsm_in == 10000);
raw_idx_in_AC = find(raw_fsm_in == 20000);


%I/O comparison
fsm_in = fsm_out_def_time(idx_in);
fsm_out = comp_data_out(idx_out);


figure
stem(fsm_in(1:length(fsm_out)),'r','LineWidth',1),hold on
stem(fsm_out,'b','LineWidth',1),hold off
title('Comparison custom architecture in/out')
legend('Input','Output')


%AC/DC filter comparison
AC_raw = reshape(fsm_out(raw_idx_in_AC(find(raw_idx_in_AC <= length(fsm_out)))),16*n_chip,[]);
AC_filtered = reshape(fsm_out(raw_idx_in_DC(find(raw_idx_in_DC <= length(fsm_out)))),16*n_chip,[]);


figure
plot(AC_raw(1,:),'r','LineWidth',1),hold on
plot(AC_filtered(1,:),'b','LineWidth',1),hold off
title('AC raw data vs AC filtered data')
legend('Raw','Filtered')




