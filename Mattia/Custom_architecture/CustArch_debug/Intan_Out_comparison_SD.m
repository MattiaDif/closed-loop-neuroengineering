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


%% Loading file
[file, path, filterindex] = ...
    uigetfile('*.rhs', 'Select an RHS2000 Data File', 'MultiSelect', 'off');

if (file == 0)
    return;
end

read_Intan_RHS2000_file([path,file])

ac_data_cust = amplifier_data;
dc_data_cust = dc_amplifier_data;


power_spect_ch = 1; %performig the PSD of this channel
fs = 1/t(2); %sampling frequency (Hz)



%% Conversion in uint 16 according to Intan datasheet
ac_data_cust_uint16 = (ac_data_cust/(0.195))+32768;
dc_data_cust_uint16 = (dc_data_cust/(0.195))+32768;

time = t;

% ac_data_orig_uint16 = (ac_data_orig/(0.195*10^-6))+32768;
% dc_data_orig_uint16 = (dc_data_orig/(-19.23*10^-3))+512;



%% SD check
for i = 1:size(ac_data_cust,1)
    
    figure
    plot(time*1000,ac_data_cust_uint16(i,:),'b','LineWidth',1), hold on
    plot(time(find(dc_data_cust_uint16(i,:))-1)*1000,ac_data_cust_uint16(i,find(dc_data_cust_uint16(i,:))-1),'r*','LineWidth',1)
    title(['Filtered data + SD - ch: ',num2str(i)])
    ylabel('Amplitude (uint16)')
    xlabel('Time (ms)')
    set(gca,'FontSize',14)
    legend('Filtered','Spikes')
    hold off
    
end


%% Interspike interval
for i = 1:size(dc_data_cust_uint16,1)
    
    ISI{i,:} = diff(find(dc_data_cust_uint16(i,:)));
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














