Project structure:
-Model divided between SINGLE and MULTI-channel MODALITIES
-MODALITIES subdivided by spike detection FAMILY: sample thresholding (spike detected if it is above a threshold), energy operator (operator computation to enhance the high frequency content), template matching (spike detection based on the similarity between a waveform and a template)
-FAMILIES subdivided by model data type: FIXED- and FLOATING-POINT

sch --> single channel
mch --> multi-channel
fixed --> fixed-point model
float --> floating point model

How to commit:
commit every time with a brief description of what you have done only when you have completed a specific change. Example, if you need to update the label of a specific simulink block in different models, modify it in all your models then commit with, for example, "label block xxx update"