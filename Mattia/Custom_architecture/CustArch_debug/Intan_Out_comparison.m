clear 
close all
clc



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
dc_data_cust_uint16 = (dc_data_cust/(-19.23*10^-3))+512;

time = t;

% ac_data_orig_uint16 = (ac_data_orig/(0.195*10^-6))+32768;
% dc_data_orig_uint16 = (dc_data_orig/(-19.23*10^-3))+512;



%% Load simulation
% mdl_name = ["Filter_",;




for i = 1:size(ac_data_cust,1)
    
    figure
    ax(1)=subplot(3,1,1);
    stairs(time,ac_data_cust_uint16(i,:),'r','LineWidth',1)
    title(['Raw data - ch: ',num2str(i)])
    ylabel('Amplitude')
    set(gca,'FontSize',14)
    
    ax(2)=subplot(3,1,2);
    stairs(time,dc_data_cust_uint16(i,:),'b','LineWidth',1)
    title(['Filtered data - ch: ',num2str(i)])
    ylabel('Amplitude')
    set(gca,'FontSize',14)
    
    ax(3)=subplot(3,1,3);
    stairs(time,ac_data_cust_uint16(i,:),'r','LineWidth',1),hold on
    stairs(time,dc_data_cust_uint16(i,:),'b','LineWidth',1),hold on
    title('Comparison')
    xlabel('Time (Samples)')
    ylabel('Amplitude')
    set(gca,'FontSize',14)
    legend('Raw','Filtered')
    
    linkaxes(ax,'x')
    hold off
    
end


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
plot(f,pxx,'r'),hold on
plot(f_f,pxx_f,'b')
hold off
title(['PSD - ch: ',num2str(power_spect_ch)])
legend('Raw','Filtered')
