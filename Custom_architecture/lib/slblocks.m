function blkStruct = slblocks
% This function specifies that the library 'mylib'
% should appear in the Library Browser with the 
% name 'My Library'

    Browser.Library = 'IIT_Genoa_Intan_HDL_blocks';
    % 'mylib' is the name of the library

    Browser.Name = 'IIT Genoa Intan HDL blocks';
    % 'My Library' is the library name that appears
    % in the Library Browser

    blkStruct.Browser = Browser;