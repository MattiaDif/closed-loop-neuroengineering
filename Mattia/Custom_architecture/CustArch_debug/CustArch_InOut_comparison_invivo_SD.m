clear
close all
clc



data_stream_enable = [1 1 1 1 1 1 1 1]; %data stream enable rollmap
n_chip = sum(data_stream_enable); % number of data stream enabled in the simulated data
n_dataframe = 300;
cust_version = 12;   %version of custom architecture

chip = num2str(data_stream_enable);
frame = num2str(n_dataframe);

fs = 25000; %sampling frequency (Hz)


%% Loading data
%raw input
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\invivo_raw_fsm_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])

% load('raw_fsm_valid_chip10000000_nframe300.mat')


%input
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\invivo_fsm_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
fsm_out_def_time = fsm_out_def_time.Data;

load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\In\invivo_fsm_valid_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
fsm_valid_def_time = fsm_valid_def_time.Data;


%output
load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\invivo_custv',num2str(cust_version),...
        '_out_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
comp_data_out = comp_data_out.Data;

load(['C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\In_Out_Simulink\Out\invivo_custv',num2str(cust_version),...
        '_out_valid_chip',chip(find(~isspace(chip))),'_nframe',frame,'.mat'])
    
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
% n_element = round(length(fsm_out)/16,0)*16;
% fsm_out = fsm_out(1:n_element);
% fsm_in = fsm_in(1:n_element);

AC_raw = (fsm_out(raw_idx_in_AC(find(raw_idx_in_AC <= length(fsm_out)))));
AC_filtered = (fsm_out(raw_idx_in_DC(find(raw_idx_in_DC <= length(fsm_out)))));

n_element = round(length(AC_filtered)/(16*n_chip),0)*(16*n_chip)-(16*n_chip);

AC_raw = reshape(AC_raw(1:n_element),16*n_chip,[]);
AC_filtered = reshape(AC_filtered(1:n_element),16*n_chip,[]);

time = (0:length(AC_raw(1,:))-1);

%% SD check
for i = 1:n_chip*16
    
    figure
%     subplot(2,1,1)
    plot(time,AC_raw(i,:),'b','LineWidth',1),hold on
%     title('Raw data')
%     ylabel('Amplitude')
%     axis([0 length(time) 32000 33200])
%     set(gca,'FontSize',14)
%     subplot(2,1,2)
    plot(time(find(AC_filtered(i,:))),AC_raw(i,find(AC_filtered(i,:))),'ro')
    title('Filtered data vs Spikes')
    xlabel('Time (Samples)')
    ylabel('Amplitude')
%     axis([0 length(time) 32000 33200])
    set(gca,'FontSize',14)
    legend('Filtered','Spikes')
    
end


%% Interspike interval
for i = 1:size(AC_filtered,1)
    
    ISI{i,:} = diff(find(AC_filtered(i,:)));
    min_ISI(i) = min(ISI{i,:});
    max_ISI(i) = max(ISI{i,:});
    
end

figure
stem(min_ISI,'b','LineWidth',1),hold on
stem(max_ISI,'r','LineWidth',1)
title('min vs max ISI')
ylabel('Samples')
xlabel('Channel number')
legend('Min','Max')
set(gca,'FontSize',14)
hold off







