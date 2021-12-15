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

## REPO CONTENT OVERVIEW

1. Custom_architecture contains all the file/scripts/models and examples to develop from scratch a closed-loop system by exploiting a model-based design approach.
2. Spike_Detection_models contains all the single- and multi-channel spike detection models developed in Simulink with data to test how they function.
3. Videoguide contains a couple of tutorial videos about model design and FPGA implementation.

For a deeper description about the repo content, please refer to this link: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/c/FE53jT3DUmAwsrZJRfZq/closed-loop-neuroscience-repo.

## INSTALLATION

### Required Software

1. Matlab version R2020a or later
2. MathWorks HDL Coder™
3. MathWorks Fixed-Point Designer
4. Xilinx ISE Design Suite
5. Windows 10 Pro / Enterprise


### Repo cloning

To clone this repo open your terminal and run:

`git clone https://github.com/StefanoBuccelli/closed-loop-neuroscience.git`

To start using all the repo functionalities avoiding potential missing file issues, please run the installer.m script to add to your Matlab path all the project.

## GETTING STARTED TUTORIAL

If you just want to have a look at the Simulink library functionalities and how to implement your model on FPGA, please download the example application here: [MISSING LINK].

A supplemental explanation about how to use the repo is provided at this link: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/getting-started-tutorial.


## BACKGROUND

Backstage of the project: brief introduction of the leading concept and how the main goals have been achieved. 

### Synthetic Neuronal dataset

A synthetic neuronal dataset was built to test the spike detection model developed in Simulink by exploiting MEArec (https://github.com/alejoe91/MEArec). Synthetic data have been used in order to have a total control on the position and number of spikes in the signal.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/1.-synthetic-neuronal-dataset.

### Spike detection models

Single-channel and multi-channel spike detection models has been developed in Simulink to investigate their feasibility in a real-time environment. The algorithms are subdivided among 3 main categories according to the spike detection methods found in literature:

1. Sample Thresholding: a spike is detected if the sample overcomes a threshold.
2. Energy Operator: non-linear energy operator (NEO) computation to enhance the high frequency content. A spike is detected if the NEO sample overcomes a threshold.
3. Template Matching: spike detection based on the similarity between a waveform and a template. A spike is detected if the similarity metric is greater than a set value.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/2.-spike-detection-models.

### Fixed-point architecture

During modelling, the default setting allows to design in floating-point architecture. Thinking about the next FPGA implementation, the models have been converted in fixed-point architecture according the FPGA specs.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/3.-fixed-point-architecture.

### Custom Architecture

The Custom Architecture (CA) consists in an original Intan RHS Stim/Recording System Verilog code modification in order to hold the Verilog code generated by the Simulink model. Starting from the experience on the Simulink environment, the concept is to focus on it for the enhancement of the Intan system, in particular, taking advantage of the MathWorks HDL Coder™ which allows to convert a Simulink model in a language that can be synthetized for the FPGA.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/4.-custom-architecture.

### Custom Architecture on FPGA

After the modelling of the custom architecture, the next step has been its implementation on FPGA. In this step, the MathWorks HDL Coder™ is required to generate the Verilog code to be included in the custom Verilog code.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/5.-custom-architecture-on-fpga.

### GUI Customization

To provide custom communication between the Host PC and the FPGA a personalization of the original Intan GUI has been developed. The two main modification at the C++/Qt code regards the possibility to enable or not the CA from the user interface and adding an action to load custom parameters for the spike detection.

Further details at: https://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/6.-gui-customization.

### Simulink Library

Aiming at simplifying the development process of closed-loop system reducing the entry barrier for a non-expert, a custom Simulink library with ready-to-use blocks has been developed. Thus, the user can design his own algorithms for the Intan RHS system just connecting the already tested and preconfigured Simulink blocks.

Further details at: hhttps://app.gitbook.com/s/wHExZxdCVJjuZ6FQM5Eq/background/7.-simulink-library.

## DOCUMENTATION (WORK IN PROGRESS)

You can find the closed-loop-neuroscience detailed documentation here: https://mattia-di-florio.gitbook.io/closed-loop-neuroscience-1/.


## CORE TEAM
The following people have contributed to the current state of the project (28/10/2021). Specifically:
- Development: Stefano Buccelli [1], Mattia Di Florio [1],[3], Yannick Bornat [4].
- Conceptualization/Supervision: Vijay Iyer [2], Akshay Rajhans [2], Kiran Kintali [2], Stefano Buccelli [1], Michela Chiappalone [1],[3].

For any questions, please reach via email Mattia Di Florio (di.florio.mattia@gmail.com) or Stefano Buccelli (stefano.buccelli@iit.it) or write an issue!

1. Rehab Technologies IIT-INAIL Lab, Istituto Italiano di Tecnologia, Via Morego 30, 12 16163 Genova, Italy
2. The MathWorks, Inc.
3.  Department of Informatics, Bioengineering, Robotics, System Engineering (DIBRIS), 20 University of Genova, Via all’Opera Pia 13, 16145, Genova, Italy
4.  Laboratoire de l’Intégration du Matériau au Système (IMS), University of Bordeaux, 18 Bordeaux INP, CNRS UMR 5218, 351 Cours de la Libération, 33405 Talence           Cedex, France


