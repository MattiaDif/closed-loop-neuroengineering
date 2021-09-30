clear all
clc

n_ch = 128;

rng(0,'twister');

a = 38000;
b = 50000;
r = (b-a).*rand(n_ch,1) + a;

r_range = [min(r) max(r)]

refractory = 300;    %samples

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