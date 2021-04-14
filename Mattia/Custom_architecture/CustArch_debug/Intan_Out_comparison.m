clear 
close all
clc



%% Loading file
path = 'C:\GitHub\closed-loop-neuroscience\Mattia\Custom_architecture\CustArch_debug\';
files = dir([path,'*.rhs']);

for i=1:length(files)
    read_Intan_RHS2000_file([path,files(i).name])
    
    if strfind(files(i).name,'custom')
        ac_data_cust = amplifier_data;
        dc_data_cust = dc_amplifier_data;
    else
        ac_data_orig = amplifier_data;
        dc_data_orig = dc_amplifier_data;
    end
    
end


%% Conversion in uint 16 according to Intan datasheet
ac_data_cust_uint16 = (ac_data_cust/(0.195*10^-6))+32768;
dc_data_cust_uint16 = (dc_data_cust/(-19.23*10^-3))+512;

ac_data_orig_uint16 = (ac_data_orig/(0.195*10^-6))+32768;
dc_data_orig_uint16 = (dc_data_orig/(-19.23*10^-3))+512;


for i = 1:size(ac_data_cust,1)
    
    figure
    ax(1)=subplot(2,1,1);
    title('AC vs DC data custom architecture')
    plot(ac_data_cust_uint16(i,:))
    ax(2)=subplot(2,1,2);
    plot(dc_data_cust_uint16(i,:))
    linkaxes(ax,'x')
    
end
