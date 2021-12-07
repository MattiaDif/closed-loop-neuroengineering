# A reference architecture for closed-loop systems in the neurotechnology field

This project comes from a collaboration between the Rehab Technologies Lab at the Italian Institute of Technologies (Genoa) and the MathWorks® company.

The goal is to provide a rapid-prototyping, model-based design architecture to speed up the implementation of closed-loop systems for neuroengineering application on FPGAs. The repo consists in a customized Simulink library to develop personalized spike detection algorithms for the Intan RHS Stim/Recording System (https://intantech.com/RHS_system.html). This is reached by means of the following process:

1. Build a reference Simulink model to process neural data;
2. Take advantage of the Fixed-Point Designer and HDL code generator provided by MathWorks to implement the model on FPGA;
3. Implementation and test on commercially available systems such as the Inatn RHS Stim/Recording System.

Retracing these steps, the user will be driven through the development process of a closed-loop system starting from the model design until the hardware immplementation.

<br />

<p>
    <img src="https://github.com/StefanoBuccelli/closed-loop-neuroscience/blob/master/Images/cust_arch_example.png" width=100% height=100%>
    <figcaption  align = "center"> <b>Fig.1 - Simulink library application example</b></figcaption>
</p>

<br />

## Content Overview

1. Custom_architecture contains all the file/scripts/models and examples to develop from scratch a closed-loop system by exploiting a model-based design approach.
2. Docs contains all the Intan RHS documentation useful for a better understanding of how the whole system works.
3. Spike_Detection_models contains all the single- and multi-channel spike detection models developed in Simulink with data to test how they function.


## Required Softwares

1. Matlab version R2020a or later
2. MathWorks HDL Coder™
3. MathWorks Fixed-Point Designer
4. Xilinx ISE Design Suite


## Installation

### Repo cloning

To clone this repo open your terminal and run:

`git clone https://github.com/StefanoBuccelli/closed-loop-neuroscience.git`

To start using all the repo functionalities avoiding potential missing file issues, please run the installer.m script to add to your Matlab path all the project.

### Getting Started Tutorial

If you just want to have a look at the Simulink library functionalities and how to implement your model on FPGA, please download the example application here: https://it.mathworks.com/matlabcentral/fileexchange/101238-iit-genoa-intan-hdl-blocks


## Documentation (WORK IN PROGRESS)

You can find the closed-loop-neuroscience detailed documentation here: https://mattia-di-florio.gitbook.io/closed-loop-neuroscience-1/.


## Core Team
The following people have contributed to the current state of the project (28/10/2021). Specifically:
- Development: Stefano Buccelli [1], Mattia Di Florio [1],[3].
- Conceptualization/Supervision: Vijay Iyer [2], Akshay Rajhans [2], Kiran Kintali [2], Stefano Buccelli [1], Michela Chiappalone [1],[3].

For any questions, please reach via email Mattia Di Florio (di.florio.mattia@gmail.com) or Stefano Buccelli (stefano.buccelli@iit.it) or write an issue!

1. Rehab Technologies IIT-INAIL Lab, Istituto Italiano di Tecnologia, Via Morego 30, 16163 Genova, Italy
2. The MathWorks, Inc.
3. Dept. Informatics, Bioengineering, Robotics and Systems Engineering, University of Genoa, 16145 Genova, Italy



