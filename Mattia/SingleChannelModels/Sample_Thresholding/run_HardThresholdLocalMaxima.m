clear all
close all
clc


%% Data loading and processing
fs = 30000; %Hz - sampling frequency
fn = fs/2;  %Hz - Nyquist frequency
refractory = 10^-3; %refractory period
w_len = fs/1000;  %samples --> 1ms
peak_diff = 10; %samples --> max spike position distance between EC and ground truth
feature_gain = 7;
th=-200;