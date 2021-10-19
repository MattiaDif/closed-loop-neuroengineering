clear all
clc


%% Change current folder
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
curr_folder = pwd;
path_index = strfind(curr_folder,'closed-loop-neuroscience');

project_path = curr_folder(1:path_index+length('closed-loop-neuroscience')-1);
addpath(genpath(project_path)); %adding to the Matlab path all the project folder including all the subfolders


n_ch = 128;

rng(0,'twister');

a = 38000;
b = 50000;
r = (b-a).*rand(n_ch,1) + a;

r_range = [min(r) max(r)]

refractory = 30;    %samples

config_file = zeros(n_ch,1);
for i=1 : 1 : n_ch
    
    config_file((i-1)*2+1) = (i-1)*2;
    config_file((i)*2) = r(i);
    
end

config_file = uint16([config_file; 512; refractory]);


config_file_txt = fopen(['config_file.txt'], 'w');

for i = 1:length(config_file)

    fprintf(config_file_txt,'%d \n',config_file(i));

end

parameter_array_uint16 = uint16(config_file);
parameter_array_time = timeseries(parameter_array_uint16);

save('config_file','parameter_array_time','-v7.3')









