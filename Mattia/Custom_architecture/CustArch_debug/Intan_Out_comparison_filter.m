clear 
close all
clc



%% Change current folder
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
curr_folder = pwd;
path_index = strfind(curr_folder,'closed-loop-neuroscience');

project_path = curr_folder(1:path_index+length('closed-loop-neuroscience')-1);
addpath(genpath(project_path)); %adding to the Matlab path all the project folder including all the subfolders



%% INTAN STUFF
%% Loading file
[file, path, filterindex] = ...
    uigetfile('*.rhs', 'Select an RHS2000 Data File', 'MultiSelect', 'off');

if (file == 0)
    return;
end

read_Intan_RHS2000_file([path,file])

ac_data_cust = amplifier_data;
dc_data_cust = dc_amplifier_data;


power_spect_ch = 4; %performig the PSD of this channel
fs = 1/t(2); %sampling frequency (Hz)



%% Conversion in uint 16 according to Intan datasheet
ac_data_cust_uint16 = (ac_data_cust/(0.195))+32768;
dc_data_cust_uint16 = (dc_data_cust/(-19.23*10^-3))+512;

time = t;

w = hamming(128);
noverlap = length(w)/2;
NFFT = 1024;


%PSD
%pwelch needs single or double data
AC_raw_double = double(ac_data_cust_uint16);
AC_filtered_double = double(dc_data_cust_uint16);


[pxx,f] = pwelch(AC_raw_double(power_spect_ch,:)-mean(AC_raw_double(power_spect_ch,:)),w,noverlap,NFFT,fs);
[pxx_f,f_f] = pwelch(AC_filtered_double(power_spect_ch,:)-mean(AC_filtered_double(power_spect_ch,:)),w,noverlap,NFFT,fs);


figure
plot(f,pxx/max(pxx),'r'),hold on
plot(f_f,pxx_f/max(pxx),'b')
hold off
title(['Normalized PSD - ch: ',num2str(power_spect_ch)])
legend('Raw','Filtered')



%% SIMULINK STUFF
%% Simulation parameters
mdl_name = "fixed_point_filter_CustArch_v19";
sim_type = 'normal'; %simulation speed
sim_stop_time = num2str(size(ac_data_cust,2)/fs);   %s
mdl=convertStringsToChars(mdl_name);
load_system(mdl);
set_param(mdl, 'SimulationMode', sim_type)
set_param(mdl,'StartTime','0','StopTime',sim_stop_time)
flag_progress = 'off';  %show simulation progress or not


%run simulation
for curr_sim = 1:size(ac_data_cust,1)
    
    signal.signal = [time' ac_data_cust(curr_sim,:)'];
    in = Simulink.SimulationInput(mdl);
    
    out = sim(in,'ShowProgress', flag_progress);
    
    %take the same number of samples of the Intan acquisition (sometimes
    %discrepance between sym_stop_time due to rounding)
    ac_data_cust_uint16_sim(curr_sim,:) = out.logsout{1}.Values.Data(1:length(dc_data_cust_uint16));
    ac_data_cust_uint16_sim = double(ac_data_cust_uint16_sim);
    dc_data_cust_uint16_sim(curr_sim,:) = out.logsout{2}.Values.Data(1:length(dc_data_cust_uint16));
    dc_data_cust_uint16_sim = double(dc_data_cust_uint16_sim);
       
    
    residuals = abs(dc_data_cust_uint16(curr_sim,:)-dc_data_cust_uint16_sim(curr_sim,:));
    
    figure
    ax(1)=subplot(2,2,1);
    stairs(time*1000,(ac_data_cust_uint16_sim(curr_sim,:)),'r','LineWidth',1),hold on
    stairs(time*1000,(dc_data_cust_uint16_sim(curr_sim,:)),'b','LineWidth',1),hold on
    title(['Simulink Raw vs Filtered signal - ch: ',num2str(curr_sim)])
%     xlabel('Time (ms)')
    ylabel('Amplitude (uint16)')
    set(gca,'FontSize',14)
    legend('Raw','Filtered')
    hold off
    grid on
    grid minor
    
    ax(2)=subplot(2,2,3);
    stairs(time*1000,(ac_data_cust_uint16(curr_sim,:)),'r','LineWidth',1),hold on
    stairs(time*1000,(dc_data_cust_uint16(curr_sim,:)),'k','LineWidth',1),hold on
    title(['FPGA Raw vs Filtered signal - ch: ',num2str(curr_sim)])
    xlabel('Time (ms)')
    ylabel('Amplitude (uint16)')
    set(gca,'FontSize',14)
    legend('Raw','Filtered')
    hold off
    grid on
    grid minor
    
    ax(3)=subplot(2,2,2);
    stairs(time*1000,(dc_data_cust_uint16_sim(curr_sim,:)),'b','LineWidth',1),hold on
    stairs(time*1000,(dc_data_cust_uint16(curr_sim,:)),'k','LineWidth',1),hold on
    title(['Simulink Filtered vs FPGA Filtered signal - ch: ',num2str(curr_sim)])
%     xlabel('Time (ms)')
%     ylabel('Amplitude (uint16)')
    set(gca,'FontSize',14)
    legend('Simulink','FPGA')
    hold off
    grid on
    grid minor
    
    ax(4)=subplot(2,2,4);
    stairs(time*1000,residuals,'g','LineWidth',1)
    title(['Residuals (FPGA - Simulink outputs) - ch: ',num2str(curr_sim)])
    xlabel('Time (ms)')
%     ylabel('Amplitude (uint16)')
    set(gca,'FontSize',14)
    hold off
    grid on
    grid minor
    linkaxes(ax,'xy')
    
end

    

