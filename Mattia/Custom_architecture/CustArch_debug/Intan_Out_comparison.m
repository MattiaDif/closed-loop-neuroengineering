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



%% Conversion in uint 16 according to Intan datasheet
ac_data_cust_uint16 = (ac_data_cust/(0.195))+32768;
dc_data_cust_uint16 = (dc_data_cust/(-19.23*10^-3))+512;

% ac_data_orig_uint16 = (ac_data_orig/(0.195*10^-6))+32768;
% dc_data_orig_uint16 = (dc_data_orig/(-19.23*10^-3))+512;


for i = 1:size(ac_data_cust,1)
    
    figure
    ax(1)=subplot(2,1,1);
    title('AC vs DC data custom architecture')
    plot(ac_data_cust_uint16(i,:))
    ax(2)=subplot(2,1,2);
    plot(dc_data_cust_uint16(i,:))
    linkaxes(ax,'x')
    
end
