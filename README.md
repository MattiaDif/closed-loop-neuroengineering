# A reference architecture for closed-loop systems in the neurotechnology field

The goal of this project is to provide a rapid-prototyping, model-based design architecture to speed up the implementation of closed-loop systems for neuroengineering application on FPGAs. The repo consists in a customized Simulink library to develop personalized spike detection algorithms for the Intan RHS Stim/Recording System (https://intantech.com/RHS_system.html). This is reached by means of the following process:

1. Build a reference Simulink model to process neural data;
2. Take advantage of the Fixed-Point Designer and HDL code generator provided by MathWorks to implement the model on FPGA;
3. Implementation and test on commercially available systems such as the Inatn RHS Stim/Recording System.

Retracing these steps, the user will be driven through the development process of a closed-loop system starting from the model design until the hardware immplementation.


## Required Software

1. Matlab version R2020a or later
2. MathWorks HDL Coder™
3. MathWorks Fixed-Point Designer
4. Xilinx ISE Design Suite


## Installation

To clone this repo open your terminal and run:

`git clone https://github.com/StefanoBuccelli/closed-loop-neuroscience.git`


## Documentation

You can find the closed-loop-neuroscience detailed documentation here:


## References


## Acknowledgements
