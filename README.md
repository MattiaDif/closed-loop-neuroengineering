# A reference architecture for closed-loop systems in the neurotechnology field

The goal of this project is to provide a rapid-prototyping, model-based design architecture to speed up the implementation of closed-loop systems for neuroengineering application on FPGAs. This is reached by means of the following process:

1.  Build a reference Simulink model to process neural data;
2.  Take advantage of the Fixed-Point Designer and HDL coder to implement the model on FPGA;
3.	Implementation and test on commercially available systems such as the Inatn RHS Stim/Recording System.

Retracing these steps, the user will be driven through the development process of a closed-loop system starting from the model design until the hardware immplementation.


## Installation

To clone this repo open your terminal and run:

`git clone https://github.com/StefanoBuccelli/closed-loop-neuroscience.git`


## Repository content

The repo consists in a hierarchical folder structure as a function of the spike detection model MODALITY, FAMILY and DATA TYPE:

1.  Model divided between **single-** and **multi-channel** MODALITIES;
2.  MODALITIES subdivided by spike detection FAMILY: **sample thresholding** (spike detected if it is above a threshold), **energy operator** (operator computation to enhance the high frequency content), **template matching** (spike detection based on the similarity between a waveform and a template);
3.  FAMILIES subdivided by model DATA TYPE: **fixed-** and **floating-point**.

![Fig. 1: Current model developed](Mattia/Images/model_summary.PNG)  
_**Figure 1:** This image summarizes the working spike detection models developed in both single- and multi- channel modes, highlighting the candidates for the next FPGA implementation._






## Documentation


### References
