%% Change current folder
if(~isdeployed)
    cd(fileparts(which(mfilename)));
end
curr_folder = pwd;
%path_index = strfind(curr_folder,'closed-loop-neuroengineering');

%project_path = curr_folder(1:path_index+length('closed-loop-neuroengineering')-1);
addpath(genpath(curr_folder)); %adding to the Matlab path all the project folder including all the subfolders