function [spkValues,spkTimeStamps] =  SpikeDetection_PTSD_core_matlab(data,thresh, peakDuration, refrTime, alignmentFlag)
% This is the matlab counterpart of the cpp core of the PTSD. It is exactly
% the same. SB

overlap=5;

nSamples=length(data);
newIndex = 1;
indexPeak = 1;
spkValues=zeros(size(data));
spkTimeStamps=zeros(size(data));
%% cycle for each data value
for index = 2: nSamples-1
    if (index < newIndex)
        continue; % jump to the new position for scanning data
    end
    
    %% if there is a peak, i.e. a relative max (or min)
    if ((abs(data(index)) > abs(data(index-1))) && (abs(data(index)) >= abs(data(index+1))))
        sTimePeak  = index;       % collect the start peak time
        sValuePeak = data(index); % collect the start peak value
        
        %% control on the end of the array
        if ((index + peakDuration) > nSamples)
            interval = nSamples - index;
        else
            interval = peakDuration;
        end

        %% If start peak value is positive, search for a minimum within the interval of possible peak duration
        if (sValuePeak > 0)
            % Initialize value and time for the ending peak
            eTimePeak = index + 1;
            eValuePeak = sValuePeak;
            % Find the minimum within the interval
            for i = (index + 1):(index + interval)
                if (data(i) < eValuePeak)
                    eTimePeak = i;
                    eValuePeak = data(i);
                end
            end
            % Maximaze finding a new max inside the interval if there is
            for i = (index + 1): eTimePeak-1
                if (data(i) > sValuePeak)
                    
                    sTimePeak = i;
                    sValuePeak = data(i);
                end
            end
            % When the min is found at the end of the interval check if signal continues to decrease
            if ((eTimePeak == (index + interval)) && ((index + interval + overlap) < nSamples))
                for i = (eTimePeak + 1): (index + interval + overlap)
                    if (data(i) < eValuePeak)
                        eTimePeak = i;
                        eValuePeak = data(i);
                    end
                end
            end
        else % if instead it is negative, search for a maximum
           
            %% Initialize value and time for the ending peak
            eTimePeak = index + 1;
            eValuePeak = sValuePeak;
            % Find the maximum within the interval
            for i = (index + 1): (index + interval)
                if (data(i) > eValuePeak)
                    eTimePeak = i;
                    eValuePeak = data(i);
                end
            end
            % Maximaze finding a new min inside the interval if there is
            for i = (index + 1): eTimePeak-1
                if (data(i) < sValuePeak)
                    sTimePeak = i;
                    sValuePeak = data(i);
                end
            end
            % When the max is found at the end of the interval check if signal continues to raise
            if ((eTimePeak == (index + interval)) && ((index + interval + overlap) < nSamples))
                for i = (eTimePeak + 1): (index + interval + overlap)
                    if (data(i) > eValuePeak)
                        eTimePeak = i;
                        eValuePeak = data(i);
                    end
                end
            end
        end
        
        %% The difference overtake the threshold and a spike is found
        if (abs((sValuePeak - eValuePeak)) >= thresh ) % necessary to put parentheses for C syntax

            spkValues(indexPeak) = abs( (sValuePeak - eValuePeak) ); % value is assumed to be the difference
            if (alignmentFlag == 0)
                %% With the following code the timestamp is assigned to the higher peak
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                if (abs(sValuePeak) > abs(eValuePeak))
                    spkTimeStamps(indexPeak) = sTimePeak;
                else
                    spkTimeStamps(indexPeak) = eTimePeak;
                end
            else
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %% With the following code the timestamp is assigned to the negative peak
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                if (sValuePeak < eValuePeak)
                    spkTimeStamps(indexPeak) = sTimePeak;
                else
                    spkTimeStamps(indexPeak) = eTimePeak;
                end
            end
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            %% Set the newIndex
            if (((spkTimeStamps(indexPeak) + refrTime) > eTimePeak) && ((spkTimeStamps(indexPeak) + refrTime) < nSamples))
                newIndex = spkTimeStamps(indexPeak) + refrTime;
            else
                newIndex = eTimePeak + 1;
            end
            
            %% increase index to fill spikes arrays
            indexPeak = indexPeak + 1;
        end
    end
end

