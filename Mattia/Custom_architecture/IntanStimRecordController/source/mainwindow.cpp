//  ------------------------------------------------------------------------
//
//  This file is part of the Intan Technologies RHS2000 Interface
//  Version 1.05
//  Copyright (C) 2013-2018 Intan Technologies
//
//  ------------------------------------------------------------------------
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

#include <QWidget>
#include <qglobal.h>

#include <QFile>
#include <QTime>
#include <QSound>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "mainwindow.h"
#include "globalconstants.h"
#include "signalprocessor.h"
#include "waveplot.h"
#include "usbdatathread.h"
#include "datastreamfifo.h"
#include "signalsources.h"
#include "signalgroup.h"
#include "signalchannel.h"
#include "bandwidthdialog.h"
#include "impedancefreqdialog.h"
#include "renamechanneldialog.h"
#include "keyboardshortcutdialog.h"
#include "helpdialogchipfilters.h"
#include "helpdialogcomparators.h"
#include "helpdialogdacs.h"
#include "helpdialoghighpassfilter.h"
#include "helpdialognotchfilter.h"
#include "helpdialogfastsettle.h"
#include "helpdialogreference.h"
#include "helpdialogioexpander.h"
#include "spikescopedialog.h"
#include "triggerrecorddialog.h"
#include "setsaveformatdialog.h"
#include "cabledelaydialog.h"
#include "rhs2000evalboard.h"
#include "rhs2000registers.h"
#include "rhs2000datablock.h"
#include "okFrontPanelDLL.h"
#include "stimparamdialog.h"
#include "stimparameters.h"
#include "digoutdialog.h"
#include "anoutdialog.h"
#include "ampsettledialog.h"
#include "chargerecoverydialog.h"

// Main Window of RHS2000 USB interface application.

// Constructor.
MainWindow::MainWindow(int sampleRateIndex_, int stimStepIndex)
{
    sampleRateIndex = sampleRateIndex_;
    stimStep = (Rhs2000Registers::StimStepSize)(stimStepIndex + 1);

    synthMode = false;

    stimParamsHaveChanged = false;

    // Default reference source settings
    referenceSource.channel = 0;
    referenceSource.stream = 0;
    referenceSource.softwareMode = false;

    // Default amplifier bandwidth settings
    desiredLowerBandwidth = 1.0;
    desiredLowerSettleBandwidth = 1000.0;
    desiredUpperBandwidth = 7500.0;
    desiredDspCutoffFreq = 1.0;
    dspEnabled = true;

    useFastSettle = false;
    headstageGlobalSettle = false;
    chargeRecoveryMode = false;
    chargeRecoveryCurrentLimit = Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit10nA;
    chargeRecoveryTargetVoltage = 0.0;

    // Default electrode impedance measurement frequency
    desiredImpedanceFreq = 1000.0;

    actualImpedanceFreq = 0.0;
    impedanceFreqValid = false;

    // Set up vectors for 8 DACs on USB interface board
    dacEnabled.resize(8);
    dacEnabled.fill(false);
    dacSelectedChannel.resize(8);
    dacSelectedChannel.fill(0);

    cableLengthPortA = 1.0;
    cableLengthPortB = 1.0;
    cableLengthPortC = 1.0;
    cableLengthPortD = 1.0;

    chipId.resize(MAX_NUM_DATA_STREAMS);
    chipId.fill(-1);

    for (int i = 0; i < 16; ++i) {
        ttlOut[i] = 0;
    }
    evalBoardMode = 0;

    numSpiPorts = openInterfaceBoard(expanderBoardConnected);

    int maxPossibleDataStreams = 2 * numSpiPorts;
    int usbBufferSize = MAX_NUM_BLOCKS_TO_READ * 2 * Rhs2000DataBlock::calculateDataBlockSizeInWords(maxPossibleDataStreams);
    cout << "MainWindow: Allocating " << usbBufferSize / 1.0e6 << " MBytes for USB read buffer." << endl;
    usbReadBuffer = new unsigned char [usbBufferSize];
    const unsigned int numSeconds = 10;  // size of RAM buffer, in seconds, assuming maximum sampling rate of...
    const unsigned int maxSamplingRate = 40000; // in Samples/s
    unsigned int fifoBufferSize =
            numSeconds * maxSamplingRate * 2 * (Rhs2000DataBlock::calculateDataBlockSizeInWords(maxPossibleDataStreams) / SAMPLES_PER_DATA_BLOCK);
    usbStreamFifo = new DataStreamFifo(fifoBufferSize);
    if (!synthMode) {
        usbDataThread = new UsbDataThread(evalBoard, usbStreamFifo, this);
        connect(usbDataThread, SIGNAL(finished()), usbDataThread, SLOT(deleteLater()));
    } else {
        usbDataThread = nullptr;
    }

    // Set dialog pointers to null.
    spikeScopeDialog = nullptr;
    stimParamDialog = nullptr;
    digOutDialog = nullptr;
    anOutDialog = nullptr;
    keyboardShortcutDialog = nullptr;
    helpDialogChipFilters = nullptr;
    helpDialogComparators = nullptr;
    helpDialogDacs = nullptr;
    helpDialogHighpassFilter = nullptr;
    helpDialogNotchFilter = nullptr;
    helpDialogFastSettle = nullptr;
    helpDialogReference = nullptr;
    helpDialogIOExpander = nullptr;

    recordTriggerChannel = 0;
    recordTriggerPolarity = 0;
    recordTriggerBuffer = 1;
    postTriggerTime = 1;
    saveTriggerChannel = true;

    signalSources = new SignalSources(numSpiPorts);

    channelVisible.resize(MAX_NUM_DATA_STREAMS);
    for (int i = 0; i < MAX_NUM_DATA_STREAMS; ++i) {
        channelVisible[i].resize(CHANNELS_PER_STREAM);
        channelVisible[i].fill(false);
    }

    signalProcessor = new SignalProcessor();
    notchFilterFrequency = 60.0;
    notchFilterBandwidth = 10.0;
    notchFilterEnabled = false;
    signalProcessor->setNotchFilterEnabled(notchFilterEnabled);
    highpassFilterFrequency = 250.0;
    highpassFilterFrequency_CA = 250.0; // update GUI - SB - 05-04-2019
    highpassFilterEnabled = false;
    signalProcessor->setHighpassFilterEnabled(highpassFilterEnabled);

    running = false;
    recording = false;
    triggerSet = false;
    triggered = false;

    saveTtlOut = false;
    saveDcAmps = false;
    validFilename = false;


    wavePlot = new WavePlot(signalProcessor, signalSources, this, this);

    connect(wavePlot, SIGNAL(selectedChannelChanged(SignalChannel*)),
            this, SLOT(newSelectedChannel(SignalChannel*)));

    createActions();
    createMenus();
    createStatusBar();
    createLayout();

    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);
    stopButton->setEnabled(false);

    manualDelayEnabled.resize(MAX_NUM_SPI_PORTS);
    manualDelayEnabled.fill(false);
    manualDelay.resize(MAX_NUM_SPI_PORTS);
    manualDelay.fill(0);

    if (!synthMode) {
        initializeInterfaceBoard();
    }

    changeSampleRate(sampleRateIndex, true);

    scanPorts();
    setStatusBarReady();

    if (!synthMode) {
        setDacThreshold1(0);
        setDacThreshold2(0);
        setDacThreshold3(0);
        setDacThreshold4(0);
        setDacThreshold5(0);
        setDacThreshold6(0);
        setDacThreshold7(0);
        setDacThreshold8(0);

        evalBoard->enableDacHighpassFilter(false);
        evalBoard->setDacHighpassFilter(250.0);
    }

    // Default data file format.
    setSaveFormat(SaveFormatIntan);
    newSaveFilePeriodMinutes = 1;

    // Default settings for display scale combo boxes.
    yScaleComboBox->setCurrentIndex(3);
    yScaleDcAmpComboBox->setCurrentIndex(3);
    yScaleAdcComboBox->setCurrentIndex(3);
    tScaleComboBox->setCurrentIndex(4);

    changeTScale(tScaleComboBox->currentIndex());
    changeYScale(yScaleComboBox->currentIndex());
    changeYScaleDcAmp(yScaleDcAmpComboBox->currentIndex());
    changeYScaleAdc(yScaleAdcComboBox->currentIndex());

    // QSound cannot play wav files directly from Qt resource files, so
    // this is a workaround:  Copy the wav file from the resource file to
    // a temporary directory, then play it from there when needed.
    QFile fileTemp(QDir::tempPath() + "/triggerbeep.wav");
    if (fileTemp.open(QIODevice::ReadWrite))
    {
        QFile resourceFile(":/sounds/triggerbeep.wav");
        if(resourceFile.open(QIODevice::ReadOnly))
        {
            fileTemp.write(resourceFile.readAll());
            resourceFile.close();
        }
        fileTemp.close();
    }

    QFile fileTemp2(QDir::tempPath() + "/triggerendbeep.wav");
    if (fileTemp2.open(QIODevice::ReadWrite))
    {
        QFile resourceFile2(":/sounds/triggerendbeep.wav");
        if(resourceFile2.open(QIODevice::ReadOnly))
        {
            fileTemp2.write(resourceFile2.readAll());
            resourceFile2.close();
        }
        fileTemp2.close();
    }
    adjustSize();
}


MainWindow::~MainWindow()
{
    delete [] usbReadBuffer;
}

// Scan SPI Ports to identify all connected RHS2000 amplifier chips.
void MainWindow::scanPorts()
{
    runButton->setEnabled(false);
    recordButton->setEnabled(false);
    triggerButton->setEnabled(false);

    statusBar()->showMessage("Scanning ports...");

    // Scan SPI Ports.
    findConnectedAmplifiers();

    // Configure SignalProcessor object for the required number of data streams.
    if (!synthMode) {
        signalProcessor->allocateMemory(evalBoard->getNumEnabledDataStreams());
        setWindowTitle(tr("Intan Technologies Stimulation / Recording Controller"));
    } else {
        signalProcessor->allocateMemory(1);
        setWindowTitle(tr("Intan Technologies Stimulation / Recording Controller "
                          "(Demonstration Mode with Synthesized Biopotentials)"));
    }

    // Turn on appropriate (optional) LEDs for Ports A-D
    if (!synthMode) {
        int ledArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        for (int i = 0; i < 4; i++) {
            if (signalSources->signalPort[i].enabled) {
                ledArray[2 * i] = 1;
            }
        }
        evalBoard->setSpiLedDisplay(ledArray);
    }

    // Switch display to the first port that has an amplifier connected.
    if (signalSources->signalPort[0].enabled && numSpiPorts > 0) {
        wavePlot->initialize(0, numSpiPorts);
        changePort(0);
    } else if (signalSources->signalPort[1].enabled && numSpiPorts > 1) {
        wavePlot->initialize(1, numSpiPorts);
        changePort(1);
    } else if (signalSources->signalPort[2].enabled && numSpiPorts > 2) {
        wavePlot->initialize(2, numSpiPorts);
        changePort(2);
    } else if (signalSources->signalPort[3].enabled && numSpiPorts > 3) {
        wavePlot->initialize(3, numSpiPorts);
        changePort(3);
    } else {
        wavePlot->initialize(MAX_NUM_SPI_PORTS, numSpiPorts);
        changePort(MAX_NUM_SPI_PORTS);
        QMessageBox::information(this, tr("No RHS2000 Stim/Amp Chips Detected"),
                tr("No RHS2000 stim/amp chips are connected to the Intan Stimulation / Recording Controller."
                   "<p>Connect headstage modules and click 'Rescan Ports' under "
                   "the Configure tab."
                   "<p>You may record from Analog In and Digital In ports in the absence of headstage modules."));
    }

    wavePlot->setSampleRate(boardSampleRate);
    changeTScale(tScaleComboBox->currentIndex());
    changeYScale(yScaleComboBox->currentIndex());
    changeYScaleAdc(yScaleAdcComboBox->currentIndex());

    runButton->setEnabled(true);
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);

    statusBar()->clearMessage();
}

// Create GUI layout.  We create the user interface (UI) directly with code, so it
// is not necessary to use Qt Designer or any other special software to modify the
// UI.
void MainWindow::createLayout()
{
    int i;

    setWindowIcon(QIcon(":/images/Intan_Icon_32p_trans24.png"));

    runButton = new QPushButton(tr("&Run"));
    stopButton = new QPushButton(tr("&Stop"));
    recordButton = new QPushButton(tr("Record"));
    triggerButton = new QPushButton(tr("Trigger"));
    baseFilenameButton = new QPushButton(tr("Select Base Filename"));
    setSaveFormatButton = new QPushButton(tr("Select File Format"));

    changeBandwidthButton = new QPushButton(tr("Change Bandwidth"));
    renameChannelButton = new QPushButton(tr("Rename Channel"));
    enableChannelButton = new QPushButton(tr("Enable/Disable (Space)"));
    enableAllButton = new QPushButton(tr("Enable All on Port"));
    disableAllButton = new QPushButton(tr("Disable All on Port"));
    spikeScopeButton = new QPushButton(tr("Spike Scope"));

    helpDialogChipFiltersButton = new QToolButton();
    helpDialogChipFiltersButton->setText(tr("?"));
    helpDialogChipFiltersButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogChipFiltersButton, SIGNAL(clicked()), this, SLOT(chipFiltersHelp()));

    helpDialogComparatorsButton = new QToolButton();
    helpDialogComparatorsButton->setText(tr("?"));
    helpDialogComparatorsButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogComparatorsButton, SIGNAL(clicked()), this, SLOT(comparatorsHelp()));

    helpDialogDacsButton = new QToolButton();
    helpDialogDacsButton->setText(tr("?"));
    helpDialogDacsButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogDacsButton, SIGNAL(clicked()), this, SLOT(dacsHelp()));

    helpDialogHighpassFilterButton = new QToolButton();
    helpDialogHighpassFilterButton->setText(tr("?"));
    helpDialogHighpassFilterButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogHighpassFilterButton, SIGNAL(clicked()), this, SLOT(highpassFilterHelp()));

    helpDialogNotchFilterButton = new QToolButton();
    helpDialogNotchFilterButton->setText(tr("?"));
    helpDialogNotchFilterButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogNotchFilterButton, SIGNAL(clicked()), this, SLOT(notchFilterHelp()));

    helpDialogSettleButton = new QToolButton();
    helpDialogSettleButton->setText(tr("?"));
    helpDialogSettleButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogSettleButton, SIGNAL(clicked()), this, SLOT(fastSettleHelp()));

    helpDialogRefButton = new QToolButton();
    helpDialogRefButton->setText(tr("?"));
    helpDialogRefButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogRefButton, SIGNAL(clicked()), this, SLOT(referenceHelp()));

    helpDialogIOExpanderButton = new QToolButton();
    helpDialogIOExpanderButton->setText(tr("?"));
    helpDialogIOExpanderButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(helpDialogIOExpanderButton, SIGNAL(clicked()), this, SLOT(ioExpanderHelp()));

    displayPortAButton = new QRadioButton(signalSources->signalPort[0].name);
    displayPortBButton = new QRadioButton(signalSources->signalPort[1].name);
    displayPortCButton = new QRadioButton(signalSources->signalPort[2].name);
    displayPortDButton = new QRadioButton(signalSources->signalPort[3].name);
    displayAdcButton = new QRadioButton(signalSources->signalPort[numSpiPorts + 0].name);
    displayDacButton = new QRadioButton(signalSources->signalPort[numSpiPorts + 1].name);
    displayDigInButton = new QRadioButton(signalSources->signalPort[numSpiPorts + 2].name);
    displayDigOutButton = new QRadioButton(signalSources->signalPort[numSpiPorts + 3].name);

    QButtonGroup *displayButtonGroup = new QButtonGroup();
    displayButtonGroup->addButton(displayPortAButton, 0);
    displayButtonGroup->addButton(displayPortBButton, 1);
    displayButtonGroup->addButton(displayPortCButton, 2);
    displayButtonGroup->addButton(displayPortDButton, 3);
    displayButtonGroup->addButton(displayAdcButton, numSpiPorts + 0);
    displayButtonGroup->addButton(displayDacButton, numSpiPorts + 1);
    displayButtonGroup->addButton(displayDigInButton, numSpiPorts + 2);
    displayButtonGroup->addButton(displayDigOutButton, numSpiPorts + 3);

    QGroupBox *portGroupBox = new QGroupBox(tr("Ports"));
    QVBoxLayout *displayLayout = new QVBoxLayout;
    displayLayout->addWidget(displayPortAButton);
    displayLayout->addWidget(displayPortBButton);
    displayLayout->addWidget(displayPortCButton);
    displayLayout->addWidget(displayPortDButton);
    displayLayout->addWidget(displayAdcButton);
    displayLayout->addWidget(displayDacButton);
    displayLayout->addWidget(displayDigInButton);
    displayLayout->addWidget(displayDigOutButton);
    displayLayout->addStretch(1);
    portGroupBox->setLayout(displayLayout);

    QGroupBox *channelGroupBox = new QGroupBox(tr("Channels"));
    QVBoxLayout *channelLayout = new QVBoxLayout;
    channelLayout->addWidget(renameChannelButton);
    channelLayout->addWidget(enableChannelButton);
    channelLayout->addWidget(enableAllButton);
    channelLayout->addWidget(disableAllButton);
    channelLayout->addStretch(1);
    channelGroupBox->setLayout(channelLayout);

    refSelectButton = new QPushButton(tr("Use Selected Channel"));
    refHardwareRefButton = new QPushButton(tr("Use Hardware Reference"));
    refHardwareRefButton->setEnabled(false);

    connect(refSelectButton, SIGNAL(clicked()), this, SLOT(referenceSetSelectedChannel()));
    connect(refHardwareRefButton, SIGNAL(clicked()), this, SLOT(referenceSetHardware()));


    refTypeLabel = new QLabel(tr("Hardware Reference:"));
    refChannelLabel = new QLabel(tr("REF input on headstages"));

    QHBoxLayout *refButtonLayout1 = new QHBoxLayout;
    refButtonLayout1->addWidget(refSelectButton);
    refButtonLayout1->addWidget(helpDialogRefButton);

    QGroupBox *referenceGroupBox = new QGroupBox(tr("Reference Selection"));
    QVBoxLayout *referenceLayout = new QVBoxLayout;
    referenceLayout->addWidget(refTypeLabel);
    referenceLayout->addWidget(refChannelLabel);
    referenceLayout->addWidget(refHardwareRefButton);
    referenceLayout->addLayout(refButtonLayout1);
    referenceLayout->addStretch(1);
    referenceGroupBox->setLayout(referenceLayout);

    stimParamButton = new QPushButton(tr("Select Stimulation Parameters"));
    connect(stimParamButton, SIGNAL(clicked()), this, SLOT(stimParam()));

//    QGroupBox *stimGroupBox = new QGroupBox(tr("Stimulation Control"));
//    QVBoxLayout *stimLayout = new QVBoxLayout;
//    stimLayout->addWidget(stimParamButton);
//    stimLayout->addStretch(1);
//    stimGroupBox->setLayout(stimLayout);

    QVBoxLayout *portStimLayout = new QVBoxLayout;
    portStimLayout->addWidget(portGroupBox);
    portStimLayout->addWidget(stimParamButton);
    portStimLayout->addStretch(1);

    QVBoxLayout *channelReferenceLayout = new QVBoxLayout;
    channelReferenceLayout->addWidget(channelGroupBox);
    channelReferenceLayout->addWidget(referenceGroupBox);

    QHBoxLayout *portChannelLayout = new QHBoxLayout;
    portChannelLayout->addLayout(portStimLayout);
    portChannelLayout->addLayout(channelReferenceLayout);

    // Combo box for selecting number of frames displayed on screen.
    numFramesComboBox = new QComboBox();
    numFramesComboBox->addItem(tr("1"));
    numFramesComboBox->addItem(tr("2"));
    numFramesComboBox->addItem(tr("4"));
    numFramesComboBox->addItem(tr("8 (2 x 4)"));
    numFramesComboBox->addItem(tr("8 (1 x 8)"));
    numFramesComboBox->addItem(tr("16 (4 x 4)"));
    numFramesComboBox->addItem(tr("16 (2 x 8)"));
    numFramesComboBox->addItem(tr("16 (1 x 16)"));
    numFramesComboBox->addItem(tr("32 (4 x 8)"));
    numFramesComboBox->addItem(tr("32 (1 x 32)"));
    numFramesComboBox->setCurrentIndex(4);

    // Create list of DC amplifier voltage scales and associated combo box.
    yScaleList.append(50);
    yScaleList.append(100);
    yScaleList.append(200);
    yScaleList.append(500);
    yScaleList.append(1000);
    yScaleList.append(2000);
    yScaleList.append(5000);
    yScaleList.append(6400); // SB added this to display all possible values

    yScaleComboBox = new QComboBox();
    for (i = 0; i < yScaleList.size(); ++i) {
        yScaleComboBox->addItem("+/-" + QString::number(yScaleList[i]) +
                                " " + QSTRING_MU_SYMBOL + "V");
    }

    // Create list of DC amplifier board scales and associated combo box.
//    yScaleDcAmpList.append(1);
//    yScaleDcAmpList.append(2);
//    yScaleDcAmpList.append(5);
//    yScaleDcAmpList.append(10);
    yScaleDcAmpList.append(50); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(100); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(200); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(500); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(1000); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(2000); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(5000); // SB added this after conversion from DC to filtered
    yScaleDcAmpList.append(6400); // SB added this after conversion from DC to filtered

    yScaleDcAmpComboBox = new QComboBox();
    for (i = 0; i < yScaleDcAmpList.size(); ++i) {
        yScaleDcAmpComboBox->addItem("+/-" + QString::number(yScaleDcAmpList[i]) + " V");
    }

    // Create list of ADC voltage scales and associated combo box.
    yScaleAdcList.append(1);
    yScaleAdcList.append(2);
    yScaleAdcList.append(5);
    yScaleAdcList.append(10);

    yScaleAdcComboBox = new QComboBox();
    for (i = 0; i < yScaleAdcList.size(); ++i) {
        yScaleAdcComboBox->addItem("+/-" + QString::number(yScaleAdcList[i]) + " V");
    }

    // Create list of time scales and associated combo box.
    tScaleList.append(10);
    tScaleList.append(20);
    tScaleList.append(50);
    tScaleList.append(100);
    tScaleList.append(200);
    tScaleList.append(500);
    tScaleList.append(1000);
    tScaleList.append(2000);
    tScaleList.append(5000);

    tScaleComboBox = new QComboBox();
    for (i = 0; i < tScaleList.size(); ++i) {
        tScaleComboBox->addItem(QString::number(tScaleList[i]) + " ms");
    }

    // Create AC/DC amplifier combo box.
    ampComboBox = new QComboBox();
    ampComboBox->addItem("AC");
    ampComboBox->addItem("Filtered"); // SB modified, it was DC

    // Amplifier sample rate combo box.
//    sampleRateComboBox = new QComboBox();
/*
    sampleRateComboBox->addItem("1.00 kS/s");
    sampleRateComboBox->addItem("1.25 kS/s");
    sampleRateComboBox->addItem("1.50 kS/s");
    sampleRateComboBox->addItem("2.00 kS/s");
    sampleRateComboBox->addItem("2.50 kS/s");
    sampleRateComboBox->addItem("3.00 kS/s");
    sampleRateComboBox->addItem("3.33 kS/s");
    sampleRateComboBox->addItem("4.00 kS/s");
    sampleRateComboBox->addItem("5.00 kS/s");
    sampleRateComboBox->addItem("6.25 kS/s");
    sampleRateComboBox->addItem("8.00 kS/s");
    sampleRateComboBox->addItem("10.0 kS/s");
    sampleRateComboBox->addItem("12.5 kS/s");
    sampleRateComboBox->addItem("15.0 kS/s");
*/
//    sampleRateComboBox->addItem("20.0 kS/s");
//    sampleRateComboBox->addItem("25.0 kS/s");
//    sampleRateComboBox->addItem("30.0 kS/s");
//    sampleRateComboBox->addItem("40.0 kS/s");
//    sampleRateComboBox->setCurrentIndex(3);

    // Notch filter combo box.
    notchFilterComboBox = new QComboBox();
    notchFilterComboBox->addItem(tr("Disabled"));
    notchFilterComboBox->addItem("50 Hz");
    notchFilterComboBox->addItem("60 Hz");
    notchFilterComboBox->setCurrentIndex(0);

    connect(runButton, SIGNAL(clicked()), this, SLOT(runInterfaceBoard()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopInterfaceBoard()));
    connect(recordButton, SIGNAL(clicked()), this, SLOT(recordInterfaceBoard()));
    connect(triggerButton, SIGNAL(clicked()), this, SLOT(triggerRecordInterfaceBoard()));
    connect(baseFilenameButton, SIGNAL(clicked()), this, SLOT(selectBaseFilenameSlot()));
    connect(changeBandwidthButton, SIGNAL(clicked()), this, SLOT(changeBandwidth()));
    connect(renameChannelButton, SIGNAL(clicked()), this, SLOT(renameChannel()));
    connect(enableChannelButton, SIGNAL(clicked()), this, SLOT(toggleChannelEnable()));
    connect(enableAllButton, SIGNAL(clicked()), this, SLOT(enableAllChannels()));
    connect(disableAllButton, SIGNAL(clicked()), this, SLOT(disableAllChannels()));
    connect(setSaveFormatButton, SIGNAL(clicked()), this, SLOT(setSaveFormatDialog()));

    connect(numFramesComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeNumFrames(int)));
    connect(yScaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeYScale(int)));
    connect(yScaleDcAmpComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeYScaleDcAmp(int)));
    connect(yScaleAdcComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeYScaleAdc(int)));
    connect(tScaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeTScale(int)));
    connect(ampComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeAmpType(int)));
    connect(notchFilterComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeNotchFilter(int)));
    connect(displayButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(changePort(int)));

    dacGainSlider = new QSlider(Qt::Horizontal);
    dacNoiseSuppressSlider = new QSlider(Qt::Horizontal);

    dacGainSlider->setRange(0, 7);
    dacGainSlider->setValue(0);
    dacNoiseSuppressSlider->setRange(0, 64);
    dacNoiseSuppressSlider->setValue(0);

    dacGainLabel = new QLabel();
    dacNoiseSuppressLabel = new QLabel();
    setDacGainLabel(0);
    setDacNoiseSuppressLabel(0);

    connect(dacGainSlider, SIGNAL(valueChanged(int)),
            this, SLOT(changeDacGain(int)));
    connect(dacNoiseSuppressSlider, SIGNAL(valueChanged(int)),
            this, SLOT(changeDacNoiseSuppress(int)));

    fifoLagLabel = new QLabel(tr("0 ms"));
    fifoLagLabel->setStyleSheet("color: green");
    fifoLagLabel->setFixedWidth(fontMetrics().width("9999 ms"));

    fifoFullLabel = new QLabel(tr("0%"));
    fifoFullLabel->setStyleSheet("color: black");
    fifoFullLabel->setFixedWidth(fontMetrics().width("99%"));

    bufferFullLabel = new QLabel(tr("0%"));
    bufferFullLabel->setStyleSheet("color: black");
    bufferFullLabel->setFixedWidth(fontMetrics().width("99%"));

    cpuWarningLabel = new QLabel("CPU limit");
    cpuWarningLabel->setStyleSheet("color: red");
    cpuWarningLabel->hide();

    QHBoxLayout *runStopLayout = new QHBoxLayout;
    runStopLayout->addWidget(runButton);
    runStopLayout->addWidget(stopButton);
    runStopLayout->addStretch(1);
    runStopLayout->addWidget(new QLabel(tr("HW buffer:")));
    runStopLayout->addWidget(fifoLagLabel);
    runStopLayout->addWidget(fifoFullLabel);
    runStopLayout->addWidget(new QLabel(tr("SW buffer:")));
    runStopLayout->addWidget(bufferFullLabel);

    QHBoxLayout *recordLayout = new QHBoxLayout;
    recordLayout->addWidget(recordButton);
    recordLayout->addWidget(triggerButton);
    recordLayout->addWidget(setSaveFormatButton);
    recordLayout->addStretch(1);
    recordLayout->addWidget(cpuWarningLabel);

    saveFilenameLineEdit = new QLineEdit();
    saveFilenameLineEdit->setEnabled(false);

    QHBoxLayout *filenameSelectLayout = new QHBoxLayout;
    filenameSelectLayout->addWidget(baseFilenameButton);
    filenameSelectLayout->addWidget(new QLabel(tr("(Date and time stamp will be added)")));
    filenameSelectLayout->addStretch(1);

    QHBoxLayout *baseFilenameLayout = new QHBoxLayout;
    baseFilenameLayout->addWidget(new QLabel(tr("Base Filename")));
    baseFilenameLayout->addWidget(saveFilenameLineEdit);

    voltageScaleLabel = new QLabel(tr("Voltage Scale (+/-)"));
    ampTypeLabel = new QLabel(tr("Amp:"));

    QLabel *ioExpanderLabel = new QLabel();
    if (expanderBoardConnected) {
        ioExpanderLabel->setText(tr("I/O Expander Detected"));
        ioExpanderLabel->setStyleSheet("color: darkGreen");
    } else {
        ioExpanderLabel->setText(tr("No I/O Expander Detected"));
        ioExpanderLabel->setStyleSheet("color: darkRed");
    }

    QHBoxLayout *numWaveformsLayout = new QHBoxLayout();
    numWaveformsLayout->addWidget(voltageScaleLabel);
    numWaveformsLayout->addWidget(yScaleComboBox);
    numWaveformsLayout->addWidget(yScaleDcAmpComboBox);
    numWaveformsLayout->addWidget(yScaleAdcComboBox);
    numWaveformsLayout->addWidget(ampTypeLabel);
    numWaveformsLayout->addWidget(ampComboBox);
    numWaveformsLayout->addStretch(1);
    numWaveformsLayout->addWidget(ioExpanderLabel);
    if (!expanderBoardConnected) {
        numWaveformsLayout->addWidget(helpDialogIOExpanderButton);
    }

    yScaleDcAmpComboBox->hide();
    yScaleAdcComboBox->hide();

    connect(spikeScopeButton, SIGNAL(clicked()),
            this, SLOT(spikeScope()));

    QHBoxLayout *scaleLayout = new QHBoxLayout();
    scaleLayout->addWidget(new QLabel(tr("Time Scale (</>)")));
    scaleLayout->addWidget(tScaleComboBox);
    scaleLayout->addStretch(1);
    scaleLayout->addWidget(new QLabel(tr("Waveforms ([/])")));
    scaleLayout->addWidget(numFramesComboBox);
    scaleLayout->addStretch(1);
    scaleLayout->addWidget(spikeScopeButton);

    QVBoxLayout *displayOrderLayout = new QVBoxLayout();
    displayOrderLayout->addLayout(numWaveformsLayout);
    displayOrderLayout->addLayout(scaleLayout);
    displayOrderLayout->addStretch(1);

    QVBoxLayout *leftLayout1 = new QVBoxLayout;
    leftLayout1->addLayout(runStopLayout);
    leftLayout1->addLayout(recordLayout);
    leftLayout1->addLayout(filenameSelectLayout);
    leftLayout1->addLayout(baseFilenameLayout);
    leftLayout1->addLayout(portChannelLayout);
    leftLayout1->addLayout(displayOrderLayout);

    QFrame *groupBox1 = new QFrame();
    groupBox1->setLayout(leftLayout1);

    QFrame *frameTab1 = new QFrame();
    QFrame *frameTab2 = new QFrame();
    QFrame *frameTab3 = new QFrame();
    QFrame *frameTab4 = new QFrame();
    QFrame *frameTab5 = new QFrame();
    QFrame *frameTab6 = new QFrame(); // update GUI - SB - 05-04-2019

    impedanceFreqSelectButton = new QPushButton(tr("Select Impedance Test Frequency"));
    runImpedanceTestButton = new QPushButton(tr("Run Impedance Measurement"));
    runImpedanceTestButton->setEnabled(false);

    connect(impedanceFreqSelectButton, SIGNAL(clicked()),
            this, SLOT(changeImpedanceFrequency()));
    connect(runImpedanceTestButton, SIGNAL(clicked()),
            this, SLOT(runImpedanceMeasurement()));

    showImpedanceCheckBox = new QCheckBox(tr("Show Last Measured Electrode Impedances"));
    connect(showImpedanceCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(showImpedances(bool)));

    saveImpedancesButton = new QPushButton(tr("Save Impedance Measurements in CSV Format"));
    saveImpedancesButton->setEnabled(false);
    connect(saveImpedancesButton, SIGNAL(clicked()),
            this, SLOT(saveImpedances()));

    QHBoxLayout *impedanceFreqSelectLayout = new QHBoxLayout();
    impedanceFreqSelectLayout->addWidget(impedanceFreqSelectButton);
    impedanceFreqSelectLayout->addStretch(1);

    QHBoxLayout *runImpedanceTestLayout = new QHBoxLayout();
    runImpedanceTestLayout->addWidget(runImpedanceTestButton);
    runImpedanceTestLayout->addStretch(1);

    QHBoxLayout *saveImpedancesLayout = new QHBoxLayout();
    saveImpedancesLayout->addWidget(saveImpedancesButton);
    saveImpedancesLayout->addStretch(1);

    desiredImpedanceFreqLabel = new QLabel(tr("Desired Impedance Test Frequency: 1000 Hz"));
    actualImpedanceFreqLabel = new QLabel(tr("Actual Impedance Test Frequency: -"));

    QVBoxLayout *impedanceLayout = new QVBoxLayout();
    impedanceLayout->addLayout(impedanceFreqSelectLayout);
    impedanceLayout->addWidget(desiredImpedanceFreqLabel);
    impedanceLayout->addWidget(actualImpedanceFreqLabel);
    impedanceLayout->addLayout(runImpedanceTestLayout);
    impedanceLayout->addWidget(showImpedanceCheckBox);
    impedanceLayout->addLayout(saveImpedancesLayout);
    impedanceLayout->addWidget(new QLabel(tr("(Impedance measurements are also saved with data.)")));
    impedanceLayout->addStretch(1);

    frameTab2->setLayout(impedanceLayout);

    QHBoxLayout *dacGainLayout = new QHBoxLayout;
    dacGainLayout->addWidget(new QLabel(tr("Electrode to ANALOG OUT Total Gain")));
    dacGainLayout->addWidget(dacGainSlider);
    dacGainLayout->addWidget(dacGainLabel);
    dacGainLayout->addStretch(1);

    QHBoxLayout *dacNoiseSuppressLayout = new QHBoxLayout;
    dacNoiseSuppressLayout->addWidget(new QLabel(tr("Audio Noise Slicer (ANALOG OUT 1,2)")));
    dacNoiseSuppressLayout->addWidget(dacNoiseSuppressSlider);
    dacNoiseSuppressLayout->addWidget(dacNoiseSuppressLabel);
    dacNoiseSuppressLayout->addStretch(1);

    dacButton1 = new QRadioButton("");
    dacButton2 = new QRadioButton("");
    dacButton3 = new QRadioButton("");
    dacButton4 = new QRadioButton("");
    dacButton5 = new QRadioButton("");
    dacButton6 = new QRadioButton("");
    dacButton7 = new QRadioButton("");
    dacButton8 = new QRadioButton("");

    for (i = 0; i < 8; ++i) {
        setDacChannelLabel(i, "n/a", "n/a");
    }

    dacButtonGroup = new QButtonGroup;
    dacButtonGroup->addButton(dacButton1, 0);
    dacButtonGroup->addButton(dacButton2, 1);
    dacButtonGroup->addButton(dacButton3, 2);
    dacButtonGroup->addButton(dacButton4, 3);
    dacButtonGroup->addButton(dacButton5, 4);
    dacButtonGroup->addButton(dacButton6, 5);
    dacButtonGroup->addButton(dacButton7, 6);
    dacButtonGroup->addButton(dacButton8, 7);
    dacButton1->setChecked(true);

    dacEnableCheckBox = new QCheckBox(tr("Analog Port Enabled"));
    dacLockToSelectedBox = new QCheckBox(tr("Lock ANALOG OUT 1 to Selected"));
    dacSetButton = new QPushButton(tr("Set to Selected"));


    connect(dacEnableCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(dacEnable(bool)));
    connect(dacSetButton, SIGNAL(clicked()),
            this, SLOT(dacSetChannel()));
    connect(dacButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(dacSelected(int)));

    QHBoxLayout *dacControlLayout = new QHBoxLayout;
    dacControlLayout->addWidget(dacEnableCheckBox);
    dacControlLayout->addWidget(dacSetButton);
    dacControlLayout->addStretch(1);
    dacControlLayout->addWidget(dacLockToSelectedBox);

    QHBoxLayout *dacHeadingLayout = new QHBoxLayout;
    dacHeadingLayout->addWidget(new QLabel("<b><u>ANALOG OUT Channel</u></b>"));
    dacHeadingLayout->addWidget(helpDialogDacsButton);
    dacHeadingLayout->addStretch(1);
    dacHeadingLayout->addWidget(new QLabel("<b><u>DIGITAL OUT Threshold</u></b>"));
    dacHeadingLayout->addWidget(helpDialogComparatorsButton);

    dac1ThresholdSpinBox = new QSpinBox();
    dac1ThresholdSpinBox->setRange(-6400, 6400);
    dac1ThresholdSpinBox->setSingleStep(5);
    dac1ThresholdSpinBox->setValue(0);

    dac2ThresholdSpinBox = new QSpinBox();
    dac2ThresholdSpinBox->setRange(-6400, 6400);
    dac2ThresholdSpinBox->setSingleStep(5);
    dac2ThresholdSpinBox->setValue(0);

    dac3ThresholdSpinBox = new QSpinBox();
    dac3ThresholdSpinBox->setRange(-6400, 6400);
    dac3ThresholdSpinBox->setSingleStep(5);
    dac3ThresholdSpinBox->setValue(0);

    dac4ThresholdSpinBox = new QSpinBox();
    dac4ThresholdSpinBox->setRange(-6400, 6400);
    dac4ThresholdSpinBox->setSingleStep(5);
    dac4ThresholdSpinBox->setValue(0);

    dac5ThresholdSpinBox = new QSpinBox();
    dac5ThresholdSpinBox->setRange(-6400, 6400);
    dac5ThresholdSpinBox->setSingleStep(5);
    dac5ThresholdSpinBox->setValue(0);

    dac6ThresholdSpinBox = new QSpinBox();
    dac6ThresholdSpinBox->setRange(-6400, 6400);
    dac6ThresholdSpinBox->setSingleStep(5);
    dac6ThresholdSpinBox->setValue(0);

    dac7ThresholdSpinBox = new QSpinBox();
    dac7ThresholdSpinBox->setRange(-6400, 6400);
    dac7ThresholdSpinBox->setSingleStep(5);
    dac7ThresholdSpinBox->setValue(0);

    dac8ThresholdSpinBox = new QSpinBox();
    dac8ThresholdSpinBox->setRange(-6400, 6400);
    dac8ThresholdSpinBox->setSingleStep(5);
    dac8ThresholdSpinBox->setValue(0);

    dac1ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac2ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac3ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac4ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac5ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac6ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac7ThresholdEnableCheckBox = new QCheckBox(tr("enable"));
    dac8ThresholdEnableCheckBox = new QCheckBox(tr("enable"));

    QHBoxLayout *dac1Layout = new QHBoxLayout;
    dac1Layout->addWidget(dacButton1);
    dac1Layout->addStretch(1);
    dac1Layout->addWidget(dac1ThresholdSpinBox);
    dac1Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac1Layout->addWidget(dac1ThresholdEnableCheckBox);

    QHBoxLayout *dac2Layout = new QHBoxLayout;
    dac2Layout->addWidget(dacButton2);
    dac2Layout->addStretch(1);
    dac2Layout->addWidget(dac2ThresholdSpinBox);
    dac2Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac2Layout->addWidget(dac2ThresholdEnableCheckBox);

    QHBoxLayout *dac3Layout = new QHBoxLayout;
    dac3Layout->addWidget(dacButton3);
    dac3Layout->addStretch(1);
    dac3Layout->addWidget(dac3ThresholdSpinBox);
    dac3Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac3Layout->addWidget(dac3ThresholdEnableCheckBox);

    QHBoxLayout *dac4Layout = new QHBoxLayout;
    dac4Layout->addWidget(dacButton4);
    dac4Layout->addStretch(1);
    dac4Layout->addWidget(dac4ThresholdSpinBox);
    dac4Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac4Layout->addWidget(dac4ThresholdEnableCheckBox);

    QHBoxLayout *dac5Layout = new QHBoxLayout;
    dac5Layout->addWidget(dacButton5);
    dac5Layout->addStretch(1);
    dac5Layout->addWidget(dac5ThresholdSpinBox);
    dac5Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac5Layout->addWidget(dac5ThresholdEnableCheckBox);

    QHBoxLayout *dac6Layout = new QHBoxLayout;
    dac6Layout->addWidget(dacButton6);
    dac6Layout->addStretch(1);
    dac6Layout->addWidget(dac6ThresholdSpinBox);
    dac6Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac6Layout->addWidget(dac6ThresholdEnableCheckBox);

    QHBoxLayout *dac7Layout = new QHBoxLayout;
    dac7Layout->addWidget(dacButton7);
    dac7Layout->addStretch(1);
    dac7Layout->addWidget(dac7ThresholdSpinBox);
    dac7Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac7Layout->addWidget(dac7ThresholdEnableCheckBox);

    QHBoxLayout *dac8Layout = new QHBoxLayout;
    dac8Layout->addWidget(dacButton8);
    dac8Layout->addStretch(1);
    dac8Layout->addWidget(dac8ThresholdSpinBox);
    dac8Layout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    dac8Layout->addWidget(dac8ThresholdEnableCheckBox);

    connect(dac1ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold1(int)));
    connect(dac2ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold2(int)));
    connect(dac3ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold3(int)));
    connect(dac4ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold4(int)));
    connect(dac5ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold5(int)));
    connect(dac6ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold6(int)));
    connect(dac7ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold7(int)));
    connect(dac8ThresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setDacThreshold8(int)));

    connect(dac1ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac2ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac3ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac4ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac5ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac6ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac7ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));
    connect(dac8ThresholdEnableCheckBox, SIGNAL(clicked()), this, SLOT(dacThresholdEnable()));

    QVBoxLayout *dacMainLayout = new QVBoxLayout;
    dacMainLayout->addLayout(dacGainLayout);
    dacMainLayout->addLayout(dacNoiseSuppressLayout);
    dacMainLayout->addLayout(dacControlLayout);
    dacMainLayout->addLayout(dacHeadingLayout);
    dacMainLayout->addLayout(dac1Layout);
    dacMainLayout->addLayout(dac2Layout);
    dacMainLayout->addLayout(dac3Layout);
    dacMainLayout->addLayout(dac4Layout);
    dacMainLayout->addLayout(dac5Layout);
    dacMainLayout->addLayout(dac6Layout);
    dacMainLayout->addLayout(dac7Layout);
    dacMainLayout->addLayout(dac8Layout);
    dacMainLayout->addStretch(1);
    // dacMainLayout->addWidget(dacLockToSelectedBox);

    frameTab3->setLayout(dacMainLayout);

    QVBoxLayout *configLayout =  new QVBoxLayout;
    scanButton = new QPushButton(tr("Rescan Ports"));
    setCableDelayButton = new QPushButton(tr("Manual"));

    QHBoxLayout *scanLayout = new QHBoxLayout;
    scanLayout->addWidget(scanButton);
    scanLayout->addStretch(1);
    scanLayout->addWidget(setCableDelayButton);

    QGroupBox *scanGroupBox = new QGroupBox(tr("Connected RHS2000 Headstages"));
    scanGroupBox->setLayout(scanLayout);

    QHBoxLayout *configTopLayout = new QHBoxLayout;
    configTopLayout->addWidget(scanGroupBox);
    configTopLayout->addStretch(1);

    note1LineEdit = new QLineEdit();
    note2LineEdit = new QLineEdit();
    note3LineEdit = new QLineEdit();
    note1LineEdit->setMaxLength(255);   // Note: default maxLength of a QLineEdit is 32767
    note2LineEdit->setMaxLength(255);
    note3LineEdit->setMaxLength(255);

    QVBoxLayout *notesLayout = new QVBoxLayout;
    notesLayout->addWidget(new QLabel(tr("The following text will be appended to saved data files.")));
    notesLayout->addWidget(new QLabel("Note 1:"));
    notesLayout->addWidget(note1LineEdit);
    notesLayout->addWidget(new QLabel("Note 2:"));
    notesLayout->addWidget(note2LineEdit);
    notesLayout->addWidget(new QLabel("Note 3:"));
    notesLayout->addWidget(note3LineEdit);
    notesLayout->addStretch(1);

    QGroupBox *notesGroupBox = new QGroupBox(tr("Notes"));
    notesGroupBox->setLayout(notesLayout);

    configLayout->addLayout(configTopLayout);
    configLayout->addWidget(notesGroupBox);
    configLayout->addStretch(1);

    frameTab4->setLayout(configLayout);

    connect(scanButton, SIGNAL(clicked()),
            this, SLOT(scanPorts()));
    connect(setCableDelayButton, SIGNAL(clicked()),
            this, SLOT(manualCableDelayControl()));

    displayMarkerCheckBox = new QCheckBox(tr("Show Marker:"));
    displayMarkerSpinBox = new QSpinBox();
    displayMarkerSpinBox->setRange(1, 16);
    displayMarkerSpinBox->setSingleStep(1);
    displayMarkerSpinBox->setValue(1);

    QVBoxLayout *markerLayout = new QVBoxLayout;

    QHBoxLayout *markerLayout1 = new QHBoxLayout;
    markerLayout1->addWidget(displayMarkerCheckBox);
    markerLayout1->addWidget(new QLabel(tr("DIGITAL IN")));
    markerLayout1->addWidget(displayMarkerSpinBox);
    markerLayout1->addStretch(1);

    displayTriggerCheckBox = new QCheckBox(tr("Trigger Display on Marker"));

    markerLayout->addLayout(markerLayout1);
    markerLayout->addWidget(displayTriggerCheckBox) ;

    QGroupBox *markerGroupBox = new QGroupBox(tr("Digital Marker"));
    markerGroupBox->setLayout(markerLayout);

    plotPointsCheckBox = new QCheckBox(tr("Plot Points Only"));
    connect(plotPointsCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(plotPointsMode(bool)));

    v0AxisLineCheckBox = new QCheckBox(tr("Display V = 0 Axis in Amplifier Plots"));
    v0AxisLineCheckBox->setChecked(false);

    QVBoxLayout *displayParamsLayout =  new QVBoxLayout;
    displayParamsLayout->addWidget(markerGroupBox);
    displayParamsLayout->addWidget(v0AxisLineCheckBox);
    displayParamsLayout->addWidget(plotPointsCheckBox);
    displayParamsLayout->addStretch(1);

    frameTab5->setLayout(displayParamsLayout);

// start - SB - 05-04-2019
    cst_en_CheckBox = new QCheckBox(tr("Cst_en"));
    cst_trig_CheckBox = new QCheckBox(tr("Cst_trig"));

    connect(cst_en_CheckBox, SIGNAL(clicked()), this, SLOT(custom_architecture_Enable()));
    connect(cst_trig_CheckBox, SIGNAL(clicked()), this, SLOT(custom_architecture_trig_Enable()));

    QVBoxLayout *custom_en_Layout = new QVBoxLayout;

    QHBoxLayout *cust_en_Layout1 = new QHBoxLayout;
    cust_en_Layout1->addWidget(cst_en_CheckBox);
    cust_en_Layout1->addWidget(cst_trig_CheckBox);
    cust_en_Layout1->addStretch(1);

    custom_en_Layout->addLayout(cust_en_Layout1);

    QGroupBox *customGroupBox = new QGroupBox(tr("Custom enable"));
    customGroupBox->setLayout(custom_en_Layout);

    highpassFilterLineEdit_CA = new QLineEdit(QString::number(highpassFilterFrequency_CA, 'f', 0));
    highpassFilterLineEdit_CA->setValidator(new QDoubleValidator(0.01, 9999.99, 2, this));
    connect(highpassFilterLineEdit_CA, SIGNAL(textChanged(const QString &)),
            this, SLOT(highpassFilterLineEditChanged_CA()));

    QVBoxLayout *cust_ParamsLayout =  new QVBoxLayout;
    cust_ParamsLayout->addWidget(customGroupBox);
    cust_ParamsLayout->addWidget(highpassFilterLineEdit_CA);
    cust_ParamsLayout->addWidget(new QLabel(tr("Hz")));
    cust_ParamsLayout->addStretch(1);
    cust_ParamsLayout->addStretch(1);

    frameTab6->setLayout(cust_ParamsLayout);
// stop update GUI - SB - 05-04-2019

    QTabWidget *tabWidget1 = new QTabWidget();
    tabWidget1->addTab(frameTab1, tr("Bandwidth"));
    tabWidget1->addTab(frameTab2, tr("Impedance"));
    tabWidget1->addTab(frameTab3, tr("Analog Out/Audio"));
    tabWidget1->addTab(frameTab4, tr("Configure"));
    tabWidget1->addTab(frameTab5, tr("Display"));
    tabWidget1->addTab(frameTab6, tr("Custom_arch")); // - SB - 05-04-2019

    dspCutoffFreqLabel = new QLabel("0.00 Hz");
    lowerBandwidthLabel = new QLabel("0.00 Hz");
    lowerSettleBandwidthLabel = new QLabel("0.00 Hz");
    upperBandwidthLabel = new QLabel("0.00 kHz");

    QHBoxLayout *sampleRateLayout = new QHBoxLayout;
    sampleRateLayout->addWidget(new QLabel(tr("Amplifier Sampling Rate:")));
    sampleRateLayout->addWidget(new QLabel(sampleRateText(sampleRateIndex)));
    sampleRateLayout->addStretch(1);

    QHBoxLayout *changeBandwidthLayout = new QHBoxLayout;
    changeBandwidthLayout->addWidget(changeBandwidthButton);
    changeBandwidthLayout->addStretch(1);
    changeBandwidthLayout->addWidget(helpDialogChipFiltersButton);

    QVBoxLayout *bandwidthLayout = new QVBoxLayout;
    bandwidthLayout->addWidget(dspCutoffFreqLabel);
    bandwidthLayout->addWidget(lowerBandwidthLabel);
//    bandwidthLayout->addWidget(lowerSettleBandwidthLabel);
    bandwidthLayout->addWidget(upperBandwidthLabel);
    bandwidthLayout->addLayout(changeBandwidthLayout);

    QGroupBox *bandwidthGroupBox = new QGroupBox(tr("Amplifier Hardware Bandwidth"));
    bandwidthGroupBox->setLayout(bandwidthLayout);

    highpassFilterCheckBox = new QCheckBox(tr("Software/DAC High-Pass Filter"));
    connect(highpassFilterCheckBox, SIGNAL(clicked(bool)),
           this, SLOT(enableHighpassFilter(bool)));
    highpassFilterLineEdit = new QLineEdit(QString::number(highpassFilterFrequency, 'f', 0));
    highpassFilterLineEdit->setValidator(new QDoubleValidator(0.01, 9999.99, 2, this));
    connect(highpassFilterLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(highpassFilterLineEditChanged()));

    QHBoxLayout *highpassFilterLayout = new QHBoxLayout;
    highpassFilterLayout->addWidget(highpassFilterCheckBox);
    highpassFilterLayout->addWidget(highpassFilterLineEdit);
    highpassFilterLayout->addWidget(new QLabel(tr("Hz")));
    highpassFilterLayout->addStretch(1);
    highpassFilterLayout->addWidget(helpDialogHighpassFilterButton);

    QHBoxLayout *notchFilterLayout = new QHBoxLayout;
    notchFilterLayout->addWidget(new QLabel(tr("Notch Filter Setting")));
    notchFilterLayout->addWidget(notchFilterComboBox);
    notchFilterLayout->addStretch(1);
    notchFilterLayout->addWidget(helpDialogNotchFilterButton);

    QVBoxLayout *offchipFilterLayout = new QVBoxLayout;
    offchipFilterLayout->addLayout(highpassFilterLayout);
    offchipFilterLayout->addLayout(notchFilterLayout);

    QGroupBox *notchFilterGroupBox = new QGroupBox(tr("Software Filters"));
    notchFilterGroupBox->setLayout(offchipFilterLayout);

    QVBoxLayout *freqLayout = new QVBoxLayout;
    freqLayout->addLayout(sampleRateLayout);
    freqLayout->addWidget(bandwidthGroupBox);
    freqLayout->addWidget(notchFilterGroupBox);
    freqLayout->addStretch(1);

    frameTab1->setLayout(freqLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(groupBox1);
    leftLayout->addWidget(tabWidget1);
    leftLayout->addStretch(1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(wavePlot);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    QRect screenRect = QApplication::desktop()->screenGeometry();
    setCentralWidget(mainWidget);
    adjustSize();

    //If the screen height has less than 100 pixels to spare at the current size of mainWidget, or
    // if the screen width has less than 100 pixels to spare, put mainWidget inside a QScrollArea

    if ((screenRect.height() < mainWidget->height() + 100) ||
            (screenRect.width() < mainWidget->width() + 100)) {
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidget(mainWidget);
        setCentralWidget(scrollArea);

        if (screenRect.height() < mainWidget->height() + 100)
            setMinimumHeight(screenRect.height() - 100);
        else
            setMinimumHeight(mainWidget->height() + 50);

        if (screenRect.width() < mainWidget->width() + 100)
            setMinimumWidth(screenRect.width() - 100);
        else
            setMinimumWidth(mainWidget->width() + 50);
    }

    wavePlot->setFocus();
}

// Create QActions linking menu items to functions.
void MainWindow::createActions()
{
    loadSettingsAction = new QAction(tr("Load General Settings"), this);
    loadSettingsAction->setShortcut(tr("Ctrl+O"));
    connect(loadSettingsAction, SIGNAL(triggered()),
            this, SLOT(loadSettings()));

    saveSettingsAction = new QAction(tr("Save General Settings"), this);
    saveSettingsAction->setShortcut(tr("Ctrl+S"));
    connect(saveSettingsAction, SIGNAL(triggered()),
            this, SLOT(saveSettings()));

    loadStimSettingsAction = new QAction(tr("Load Stimulation Settings"), this);
    connect(loadStimSettingsAction, SIGNAL(triggered()),
            this, SLOT(loadStimSettings()));

    saveStimSettingsAction = new QAction(tr("Save Stimulation Settings"), this);
    connect(saveStimSettingsAction, SIGNAL(triggered()),
            this, SLOT(saveStimSettings()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    originalOrderAction =
            new QAction(tr("Restore Original Channel Order"), this);
    connect(originalOrderAction, SIGNAL(triggered()),
            this, SLOT(restoreOriginalChannelOrder()));

    alphaOrderAction =
            new QAction(tr("Order Channels Alphabetically"), this);
    connect(alphaOrderAction, SIGNAL(triggered()),
            this, SLOT(alphabetizeChannels()));

    aboutAction =
            new QAction(tr("&About Intan Stim/Recording Controller..."), this);
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    keyboardHelpAction =
            new QAction(tr("&Keyboard Shortcuts..."), this);
    keyboardHelpAction->setShortcut(tr("F1"));
    connect(keyboardHelpAction, SIGNAL(triggered()),
            this, SLOT(keyboardShortcutsHelp()));

    intanWebsiteAction =
            new QAction(tr("Visit Intan Website..."), this);
    connect(intanWebsiteAction, SIGNAL(triggered()),
            this, SLOT(openIntanWebsite()));

    renameChannelAction =
            new QAction(tr("Rename Channel"), this);
    renameChannelAction->setShortcut(tr("Ctrl+R"));
    connect(renameChannelAction, SIGNAL(triggered()),
            this, SLOT(renameChannel()));

    toggleChannelEnableAction =
            new QAction(tr("Enable/Disable Channel"), this);
    toggleChannelEnableAction->setShortcut(tr("Space"));
    connect(toggleChannelEnableAction, SIGNAL(triggered()),
            this, SLOT(toggleChannelEnable()));

    enableAllChannelsAction =
            new QAction(tr("Enable all Channels on Port"), this);
    connect(enableAllChannelsAction, SIGNAL(triggered()),
            this, SLOT(enableAllChannels()));

    disableAllChannelsAction =
            new QAction(tr("Disable all Channels on Port"), this);
    connect(disableAllChannelsAction, SIGNAL(triggered()),
            this, SLOT(disableAllChannels()));

    copyStimParametersAction =
            new QAction(tr("Copy Stimulation Parameters"), this);
    copyStimParametersAction->setShortcut(QKeySequence::Copy);
    connect(copyStimParametersAction, SIGNAL(triggered()),
            this, SLOT(copyStimParameters()));

    pasteStimParametersAction =
            new QAction(tr("Paste Stimulation Parameters"), this);
    pasteStimParametersAction->setShortcut(QKeySequence::Paste);
    connect(pasteStimParametersAction, SIGNAL(triggered()),
            this, SLOT(pasteStimParameters()));

    ampSettleSettingsAction =
            new QAction(tr("Amplifier Settle Settings"), this);
    connect(ampSettleSettingsAction, SIGNAL(triggered()),
            this, SLOT(ampSettleSettings()));

    chargeRecoverySettingsAction =
            new QAction(tr("Charge Recovery Settings"), this);
    connect(chargeRecoverySettingsAction, SIGNAL(triggered()),
            this, SLOT(chargeRecoverySettings()));
}

// Create pull-down menus.
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadSettingsAction);
    fileMenu->addAction(saveSettingsAction);
    fileMenu->addAction(loadStimSettingsAction);
    fileMenu->addAction(saveStimSettingsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyStimParametersAction);
    editMenu->addAction(pasteStimParametersAction);

    stimMenu = menuBar()->addMenu(tr("&Stimulation"));
    stimMenu->addAction(ampSettleSettingsAction);
    stimMenu->addAction(chargeRecoverySettingsAction);

    channelMenu = menuBar()->addMenu(tr("&Channels"));
    channelMenu->addAction(renameChannelAction);
    channelMenu->addAction(toggleChannelEnableAction);
    channelMenu->addAction(enableAllChannelsAction);
    channelMenu->addAction(disableAllChannelsAction);
    channelMenu->addSeparator();
    channelMenu->addAction(originalOrderAction);
    channelMenu->addAction(alphaOrderAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(keyboardHelpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(intanWebsiteAction);
    helpMenu->addAction(aboutAction);
}

// Create status bar at bottom of the main window.
void MainWindow::createStatusBar()
{
    statusBarLabel = new QLabel(tr(""));
    statusBar()->addWidget(statusBarLabel, 1);
    statusBar()->setSizeGripEnabled(false);  // fixed window size
}

// Display "About" message box.
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Intan Technologies Stimulation / Recording Controller"),
            tr("<h2>Intan Technologies Stimulation / Recording Controller</h2>"
               "<p>Version 1.05"
               "<p>Copyright &copy; 2017-2018 Intan Technologies"
               "<p>This application controls the "
               "128 Channel Stimulation / Recording Controller "
               "from Intan Technologies.  The C++/Qt source code "
               "for this application is freely available from Intan Technologies. "
               "For more information visit <i>http://www.intantech.com</i>."
               "<p>This program is free software: you can redistribute it and/or modify "
               "it under the terms of the GNU Lesser General Public License as published "
               "by the Free Software Foundation, either version 3 of the License, or "
               "(at your option) any later version."
               "<p>This program is distributed in the hope that it will be useful, "
               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
               "GNU Lesser General Public License for more details."
               "<p>You should have received a copy of the GNU Lesser General Public License "
               "along with this program.  If not, see <i>http://www.gnu.org/licenses/</i>."));
}

// Display keyboard shortcut window.
void MainWindow::keyboardShortcutsHelp()
{
    if (!keyboardShortcutDialog) {
        keyboardShortcutDialog = new KeyboardShortcutDialog(this);
    }
    keyboardShortcutDialog->show();
    keyboardShortcutDialog->raise();
    keyboardShortcutDialog->activateWindow();
    wavePlot->setFocus();
}

// Display on-chip filters help menu.
void MainWindow::chipFiltersHelp()
{
    if (!helpDialogChipFilters) {
        helpDialogChipFilters = new HelpDialogChipFilters(this);
    }
    helpDialogChipFilters->show();
    helpDialogChipFilters->raise();
    helpDialogChipFilters->activateWindow();
    wavePlot->setFocus();
}

// Display comparators help menu.
void MainWindow::comparatorsHelp()
{
    if (!helpDialogComparators) {
        helpDialogComparators = new HelpDialogComparators(this);
    }
    helpDialogComparators->show();
    helpDialogComparators->raise();
    helpDialogComparators->activateWindow();
    wavePlot->setFocus();
}

// Display DACs help menu.
void MainWindow::dacsHelp()
{
    if (!helpDialogDacs) {
        helpDialogDacs = new HelpDialogDacs(this);
    }
    helpDialogDacs->show();
    helpDialogDacs->raise();
    helpDialogDacs->activateWindow();
    wavePlot->setFocus();
}

// Display software/DAC high-pass filter help menu.
void MainWindow::highpassFilterHelp()
{
    if (!helpDialogHighpassFilter) {
        helpDialogHighpassFilter = new HelpDialogHighpassFilter(this);
    }
    helpDialogHighpassFilter->show();
    helpDialogHighpassFilter->raise();
    helpDialogHighpassFilter->activateWindow();
    wavePlot->setFocus();
}

// Display software notch filter help menu.
void MainWindow::notchFilterHelp()
{
    if (!helpDialogNotchFilter) {
        helpDialogNotchFilter = new HelpDialogNotchFilter(this);
    }
    helpDialogNotchFilter->show();
    helpDialogNotchFilter->raise();
    helpDialogNotchFilter->activateWindow();
    wavePlot->setFocus();
}

// Display fast settle help menu.
void MainWindow::fastSettleHelp()
{
    if (!helpDialogFastSettle) {
        helpDialogFastSettle = new HelpDialogFastSettle(this);
    }
    helpDialogFastSettle->show();
    helpDialogFastSettle->raise();
    helpDialogFastSettle->activateWindow();
    wavePlot->setFocus();
}

// Display I/O expander help menu.
void MainWindow::ioExpanderHelp()
{
    if (!helpDialogIOExpander) {
        helpDialogIOExpander = new HelpDialogIOExpander(this);
    }
    helpDialogIOExpander->show();
    helpDialogIOExpander->raise();
    helpDialogIOExpander->activateWindow();
    wavePlot->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Perform any clean-up here before application closes.
    if (running) {
        stopInterfaceBoard(); // stop SPI communication before we exit
    }
    if (!synthMode) {
        usbDataThread->close();
        usbDataThread->wait();

        int ledArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        evalBoard->setSpiLedDisplay(ledArray);
        // evalBoard->setAllDacsToZero();  // Need to run board for this to take effect.
        evalBoard->resetFpga();  // Hard reset
    }

    event->accept();
}

// Change the number of waveform frames displayed on the screen.
void MainWindow::changeNumFrames(int index)
{
    wavePlot->setNumFrames(numFramesComboBox->currentIndex());
    numFramesComboBox->setCurrentIndex(index);
    wavePlot->setFocus();
}

// Change voltage scale of AC amplifier waveform plots.
void MainWindow::changeYScale(int index)
{
    wavePlot->setYScale(yScaleList[index]);
    wavePlot->setFocus();
}

// Change voltage scale of DC amplifier waveform plots.
void MainWindow::changeYScaleDcAmp(int index)
{
    wavePlot->setYScaleDcAmp(yScaleDcAmpList[index]);
    wavePlot->setFocus();
}

// Change voltage scale of ADC plots.
void MainWindow::changeYScaleAdc(int index)
{
    wavePlot->setYScaleAdc(yScaleAdcList[index]);
    wavePlot->setFocus();
}

// Switch between AC and DC amps.
void MainWindow::changeAmpType(int index)
{
    if (index == 0) {
        yScaleComboBox->show();
        yScaleDcAmpComboBox->hide();
    } else {
        yScaleComboBox->hide();
        yScaleDcAmpComboBox->show();
    }
    wavePlot->setPlotDc(index == 1);
    wavePlot->setFocus();
}

// Change time scale of waveform plots.
void MainWindow::changeTScale(int index)
{
    wavePlot->setTScale(tScaleList[index]);
    wavePlot->setFocus();
}

// Launch amplifier bandwidth selection dialog and set new bandwidth.
void MainWindow::changeBandwidth()
{
    BandwidthDialog bandwidthDialog(desiredLowerBandwidth, desiredUpperBandwidth,
                                    desiredDspCutoffFreq, dspEnabled, boardSampleRate, this);
    if (bandwidthDialog.exec()) {
        desiredDspCutoffFreq = bandwidthDialog.dspFreqLineEdit->text().toDouble();
        desiredLowerBandwidth = bandwidthDialog.lowFreqLineEdit->text().toDouble();
        desiredUpperBandwidth = bandwidthDialog.highFreqLineEdit->text().toDouble();
        dspEnabled = bandwidthDialog.dspEnableCheckBox->isChecked();
        changeSampleRate(sampleRateIndex, false); // this function sets new amp bandwidth
    }
    wavePlot->setFocus();
}

// Launch electrode impedance measurement frequency selection dialog.
void MainWindow::changeImpedanceFrequency()
{
    ImpedanceFreqDialog impedanceFreqDialog(desiredImpedanceFreq, actualLowerBandwidth, actualUpperBandwidth,
                                            actualDspCutoffFreq, dspEnabled, boardSampleRate, this);
    if (impedanceFreqDialog.exec()) {
        desiredImpedanceFreq = impedanceFreqDialog.impedanceFreqLineEdit->text().toDouble();
        updateImpedanceFrequency();
    }
    wavePlot->setFocus();
}

// Update electrode impedance measurement frequency, after checking that
// requested test frequency lies within acceptable ranges based on the
// amplifier bandwidth and the sampling rate.  See impedancefreqdialog.cpp
// for more information.
void MainWindow::updateImpedanceFrequency()
{
    int impedancePeriod;
    double lowerBandwidthLimit, upperBandwidthLimit;

    upperBandwidthLimit = actualUpperBandwidth / 1.5;
    lowerBandwidthLimit = actualLowerBandwidth * 1.5;
    if (dspEnabled) {
        if (actualDspCutoffFreq > actualLowerBandwidth) {
            lowerBandwidthLimit = actualDspCutoffFreq * 1.5;
        }
    }

    if (desiredImpedanceFreq > 0.0) {
        desiredImpedanceFreqLabel->setText("Desired Impedance Test Frequency: " +
                                           QString::number(desiredImpedanceFreq, 'f', 0) +
                                           " Hz");
        impedancePeriod = qRound(boardSampleRate / desiredImpedanceFreq);
        if (impedancePeriod >= 4 && impedancePeriod <= 1024 &&
                desiredImpedanceFreq >= lowerBandwidthLimit &&
                desiredImpedanceFreq <= upperBandwidthLimit) {
            actualImpedanceFreq = boardSampleRate / impedancePeriod;
            impedanceFreqValid = true;
        } else {
            actualImpedanceFreq = 0.0;
            impedanceFreqValid = false;
        }
    } else {
        desiredImpedanceFreqLabel->setText("Desired Impedance Test Frequency: -");
        actualImpedanceFreq = 0.0;
        impedanceFreqValid = false;
    }
    if (impedanceFreqValid) {
        actualImpedanceFreqLabel->setText("Actual Impedance Test Frequency: " +
                                          QString::number(actualImpedanceFreq, 'f', 1) +
                                          " Hz");
    } else {
        actualImpedanceFreqLabel->setText("Actual Impedance Test Frequency: -");
    }
    runImpedanceTestButton->setEnabled(impedanceFreqValid);
}

// Rename selected channel.
void MainWindow::renameChannel()
{
    QString newChannelName;

    RenameChannelDialog renameChannelDialog(wavePlot->getNativeChannelName(),
                                            wavePlot->getChannelName(), this);
    if (renameChannelDialog.exec()) {
        newChannelName = renameChannelDialog.nameLineEdit->text();
        wavePlot->setChannelName(newChannelName);
        wavePlot->refreshScreen();
    }
    wavePlot->setFocus();
}

void MainWindow::sortChannelsByName()
{
    wavePlot->sortChannelsByName();
    wavePlot->refreshScreen();
    wavePlot->setFocus();
}

void MainWindow::sortChannelsByNumber()
{
    wavePlot->sortChannelsByNumber();
    wavePlot->refreshScreen();
    wavePlot->setFocus();
}

void MainWindow::restoreOriginalChannelOrder()
{
    wavePlot->sortChannelsByNumber();
    wavePlot->refreshScreen();
    wavePlot->setFocus();
}

void MainWindow::alphabetizeChannels()
{
    wavePlot->sortChannelsByName();
    wavePlot->refreshScreen();
    wavePlot->setFocus();
}

void MainWindow::toggleChannelEnable()
{
    wavePlot->toggleSelectedChannelEnable();
    wavePlot->setFocus();
}

void MainWindow::enableAllChannels()
{
    wavePlot->enableAllChannels();
    wavePlot->setFocus();
}

void MainWindow::disableAllChannels()
{
    wavePlot->disableAllChannels();
    wavePlot->setFocus();
}

void MainWindow::changePort(int port)
{
    wavePlot->setPort(port);

    if (port == numSpiPorts + 0) {  // Analog In Ports
        voltageScaleLabel->show();
        yScaleComboBox->hide();
        yScaleDcAmpComboBox->hide();
        yScaleAdcComboBox->show();
        ampComboBox->hide();
        ampTypeLabel->hide();
        stimParamButton->setEnabled(false);
    } else if (port == numSpiPorts + 1) {  // Analog Out Ports
        voltageScaleLabel->show();
        yScaleComboBox->hide();
        yScaleDcAmpComboBox->hide();
        yScaleAdcComboBox->show();
        ampComboBox->hide();
        ampTypeLabel->hide();
        stimParamButton->setEnabled(true);
    } else if (port == numSpiPorts + 2) {  // Digital In Ports
        voltageScaleLabel->hide();
        yScaleComboBox->hide();
        yScaleDcAmpComboBox->hide();
        yScaleAdcComboBox->hide();
        ampComboBox->hide();
        ampTypeLabel->hide();
        stimParamButton->setEnabled(false);
    } else if (port == numSpiPorts + 3) {  // Digital Out Ports
        voltageScaleLabel->hide();
        yScaleComboBox->hide();
        yScaleDcAmpComboBox->hide();
        yScaleAdcComboBox->hide();
        ampComboBox->hide();
        ampTypeLabel->hide();
        stimParamButton->setEnabled(true);
    } else {  // SPI Ports
        voltageScaleLabel->show();
        if (ampComboBox->currentIndex() == 0) {
            yScaleComboBox->show();
        } else {
            yScaleDcAmpComboBox->show();
        }
        yScaleAdcComboBox->hide();
        ampComboBox->show();
        ampTypeLabel->show();
        stimParamButton->setEnabled(true);
    }

    wavePlot->setFocus();
}

void MainWindow::changeDacGain(int index)
{
    if (!synthMode)
        evalBoard->setDacGain(index);
    setDacGainLabel(index);
    wavePlot->setFocus();
}

void MainWindow::setDacGainLabel(int gain)
{
    double trueGain = 1600.0 * pow(2.0, gain);
    dacGainLabel->setText(QString::number(trueGain  / 1000.0) + " mV/" + QSTRING_MU_SYMBOL + "V");
}

void MainWindow::changeDacNoiseSuppress(int index)
{
    if (!synthMode)
        evalBoard->setAudioNoiseSuppress(index);
    setDacNoiseSuppressLabel(index);
    wavePlot->setFocus();
}

void MainWindow::setDacNoiseSuppressLabel(int noiseSuppress)
{
    dacNoiseSuppressLabel->setText("+/-" +
                                   QString::number(3.12 * noiseSuppress, 'f', 0) +
                                   " " + QSTRING_MU_SYMBOL + "V");
}

// Enable or disable DAC on USB interface board.
void MainWindow::dacEnable(bool enable)
{
    int dacChannel = dacButtonGroup->checkedId();

    dacEnabled[dacChannel] = enable;
    if (!synthMode)
        evalBoard->enableDac(dacChannel, enable);
    if (dacSelectedChannel[dacChannel]) {
        setDacChannelLabel(dacChannel, dacSelectedChannel[dacChannel]->customChannelName,
                           dacSelectedChannel[dacChannel]->nativeChannelName);
    } else {
        setDacChannelLabel(dacChannel, "n/a", "n/a");
    }
    wavePlot->setFocus();
}

// Route selected amplifier channel to selected DAC.
void MainWindow::dacSetChannel()
{
    int dacChannel = dacButtonGroup->checkedId();
    SignalChannel *selectedChannel = wavePlot->selectedChannel();
    if (selectedChannel->signalType == AmplifierSignal) {
        // If DAC is not yet enabled, enable it.
        if (!dacEnabled[dacChannel]) {
            dacEnableCheckBox->setChecked(true);
            dacEnable(true);
        }
        // Set DAC to selected channel and label it accordingly.
        dacSelectedChannel[dacChannel] = selectedChannel;
        if (!synthMode) {
            evalBoard->selectDacDataStream(dacChannel, selectedChannel->commandStream);
            evalBoard->selectDacDataChannel(dacChannel, selectedChannel->chipChannel);
        }
        setDacChannelLabel(dacChannel, selectedChannel->customChannelName,
                           selectedChannel->nativeChannelName);
    }
    wavePlot->setFocus();
}

void MainWindow::dacSelected(int dacChannel)
{
    dacEnableCheckBox->setChecked(dacEnabled[dacChannel]);
    wavePlot->setFocus();
}

// Label DAC selection button in GUI with selected amplifier channel.
void MainWindow::setDacChannelLabel(int dacChannel, QString channel,
                                    QString name)
{
    QString text = "ANALOG OUT " + QString::number(dacChannel + 1);
    if (dacChannel == 0) text += " (Audio L)";
    if (dacChannel == 1) text += " (Audio R)";
    text += ": ";
    if (dacEnabled[dacChannel] == false) {
        text += "off";
    } else {
        text += name + " (" + channel + ")";
    }

    switch (dacChannel) {
    case 0:
        dacButton1->setText(text);
        break;
    case 1:
        dacButton2->setText(text);
        break;
    case 2:
        dacButton3->setText(text);
        break;
    case 3:
        dacButton4->setText(text);
        break;
    case 4:
        dacButton5->setText(text);
        break;
    case 5:
        dacButton6->setText(text);
        break;
    case 6:
        dacButton7->setText(text);
        break;
    case 7:
        dacButton8->setText(text);
        break;
    }
}

// Change notch filter settings.
void MainWindow::changeNotchFilter(int notchFilterIndex)
{
    switch (notchFilterIndex) {
    case 0:
        notchFilterEnabled = false;
        break;
    case 1:
        notchFilterFrequency = 50.0;
        notchFilterEnabled = true;
        break;
    case 2:
        notchFilterFrequency = 60.0;
        notchFilterEnabled = true;
        break;
    }
    signalProcessor->setNotchFilter(notchFilterFrequency, notchFilterBandwidth, boardSampleRate);
    signalProcessor->setNotchFilterEnabled(notchFilterEnabled);
    wavePlot->setFocus();
}

// Enable/disable software/FPGA high-pass filter.
void MainWindow::enableHighpassFilter(bool enable)
{
    highpassFilterEnabled = enable;
    signalProcessor->setHighpassFilterEnabled(enable);
    if (!synthMode) {
        evalBoard->enableDacHighpassFilter(enable);
    }
    wavePlot->setFocus();
}

// Update software/FPGA high-pass filter when LineEdit changes.
void MainWindow::highpassFilterLineEditChanged()
{
    setHighpassFilterCutoff(highpassFilterLineEdit->text().toDouble());
}


// Update software/FPGA high-pass filter cutoff frequency.
void MainWindow::setHighpassFilterCutoff(double cutoff)
{
    highpassFilterFrequency = cutoff;
    signalProcessor->setHighpassFilter(cutoff , boardSampleRate);
    if (!synthMode) {
        evalBoard->setDacHighpassFilter(cutoff);
    }
}

// start update GUI - SB - 05-04-2019
// Update software/FPGA high-pass filter when LineEdit changes.
void MainWindow::highpassFilterLineEditChanged_CA()
{
    setHighpassFilterCutoff_CA(highpassFilterLineEdit_CA->text().toDouble());
}

// Update software/FPGA high-pass filter cutoff frequency.
void MainWindow::setHighpassFilterCutoff_CA(double cutoff)
{
    highpassFilterFrequency_CA = cutoff;
    // signalProcessor->setHighpassFilter(cutoff , boardSampleRate);
    if (!synthMode) {
        evalBoard->setCA_HighpassFilter(cutoff);
    }
}

// stop update GUI - SB - 05-04-2019

// Change RHS2000 interface board amplifier sample rate.
void MainWindow::changeSampleRate(int sampleRateIndex, bool updateStimParams)
{
    Rhs2000EvalBoard::AmplifierSampleRate sampleRate =
            Rhs2000EvalBoard::SampleRate20000Hz;

    // Note: numUsbBlocksToRead is set to give an approximate frame rate of
    // 30 Hz for most sampling rates.

    switch (sampleRateIndex) {
    case 0:
        sampleRate = Rhs2000EvalBoard::SampleRate20000Hz;
        boardSampleRate = 20000.0;
        numUsbBlocksToRead = 5;
        break;
    case 1:
        sampleRate = Rhs2000EvalBoard::SampleRate25000Hz;
        boardSampleRate = 25000.0;
        numUsbBlocksToRead = 6;
        break;
    case 2:
        sampleRate = Rhs2000EvalBoard::SampleRate30000Hz;
        boardSampleRate = 30000.0;
        numUsbBlocksToRead = MAX_NUM_BLOCKS_TO_READ;
        break;
    case 3:
        sampleRate = Rhs2000EvalBoard::SampleRate40000Hz;
        boardSampleRate = 40000.0;
        numUsbBlocksToRead = MAX_NUM_BLOCKS_TO_READ;
        break;
    }

    wavePlot->setNumUsbBlocksToPlot(numUsbBlocksToRead);

    // Set up an RHS2000 register object using this sample rate to
    // optimize MUX-related register settings.
    Rhs2000Registers chipRegisters(boardSampleRate, stimStep);

    int commandSequenceLength;
    vector<unsigned int> commandList;

    if (!synthMode) {
        evalBoard->setSampleRate(sampleRate);

        // Now that we have set our sampling rate, we can set the MISO sampling delay
        // which is dependent on the sample rate.
        if (manualDelayEnabled[0]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortA, manualDelay[0]);
        } else {
            evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortA, cableLengthPortA);
        }
        if (manualDelayEnabled[1]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortB, manualDelay[1]);
        } else {
            evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortB, cableLengthPortB);
        }
        if (manualDelayEnabled[2]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortC, manualDelay[2]);
        } else {
            evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortC, cableLengthPortC);
        }
        if (manualDelayEnabled[3]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortD, manualDelay[3]);
        } else {
            evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortD, cableLengthPortD);
        }

        // Create command lists to be uploaded to auxiliary command slots.

        // Create a command list for the AuxCmd1 slot.  This command sequence will create a 250 Hz,
        // zero-amplitude sine wave (i.e., a flatline).  We will change this when we want to perform
        // impedance testing.
        // commandSequenceLength = chipRegisters.createCommandListZcheckDac(commandList, 250.0, 0.0);

        chipRegisters.setDigOutLow(Rhs2000Registers::DigOut::DigOut1);   // Take auxiliary output out of HiZ mode.
        chipRegisters.setDigOutLow(Rhs2000Registers::DigOut::DigOut2);   // Take auxiliary output out of HiZ mode.
        chipRegisters.setDigOutLow(Rhs2000Registers::DigOut::DigOutOD);   // Take auxiliary output out of HiZ mode.

        commandSequenceLength = chipRegisters.createCommandListRegisterConfig(commandList, updateStimParams);
        evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
        evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);
        // evalBoard->printCommandList(commandList); // optionally, print command list

        // Next, fill the other three command slots with dummy commands.
        chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 255));
        evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd2);
        chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 254));
        evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd3);
        chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 253));
        evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd4);

    }
    // Before generating register configuration command sequences, set amplifier
    // bandwidth paramters.

    actualDspCutoffFreq = chipRegisters.setDspCutoffFreq(desiredDspCutoffFreq);
    actualLowerBandwidth = chipRegisters.setLowerBandwidth(desiredLowerBandwidth, 0);
    actualLowerSettleBandwidth = chipRegisters.setLowerBandwidth(desiredLowerSettleBandwidth, 1);
    actualUpperBandwidth = chipRegisters.setUpperBandwidth(desiredUpperBandwidth);
    chipRegisters.enableDsp(dspEnabled);

    if (dspEnabled) {
        dspCutoffFreqLabel->setText("Desired/Actual DSP Cutoff: " +
                                    QString::number(desiredDspCutoffFreq, 'f', 2) + " Hz / " +
                                    QString::number(actualDspCutoffFreq, 'f', 2) + " Hz");
    } else {
        dspCutoffFreqLabel->setText("Desired/Actual DSP Cutoff: DSP disabled");
    }

    lowerBandwidthLabel->setText("Desired/Actual Lower Bandwidth: " +
                                 QString::number(desiredLowerBandwidth, 'f', 2) + " Hz / " +
                                 QString::number(actualLowerBandwidth, 'f', 2) + " Hz");
    lowerSettleBandwidthLabel->setText("Desired/Actual Lower Amp Settle Bandwidth: " +
                                 QString::number(desiredLowerSettleBandwidth, 'f', 0) + " Hz / " +
                                 QString::number(actualLowerSettleBandwidth, 'f', 0) + " Hz");
    upperBandwidthLabel->setText("Desired/Actual Upper Bandwidth: " +
                                 QString::number(desiredUpperBandwidth / 1000.0, 'f', 2) + " kHz / " +
                                 QString::number(actualUpperBandwidth / 1000.0, 'f', 2) + " kHz");

    if (!synthMode) {
        commandSequenceLength = chipRegisters.createCommandListRegisterConfig(commandList, updateStimParams);
        // Upload version with no ADC calibration to AuxCmd1 RAM Bank.
        evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
        evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);

        // Run system once for changes to take effect
        evalBoard->setContinuousRunMode(false);
        evalBoard->setMaxTimeStep(SAMPLES_PER_DATA_BLOCK);

        // Start SPI interface.
        evalBoard->run();

        // Wait for the 128-sample run to complete.
        while (evalBoard->isRunning()) {
            qApp->processEvents();
        }

        evalBoard->flush();
    }

    wavePlot->setSampleRate(boardSampleRate);

    signalProcessor->setNotchFilter(notchFilterFrequency, notchFilterBandwidth, boardSampleRate);
    signalProcessor->setHighpassFilter(highpassFilterFrequency, boardSampleRate);

    if (!synthMode) {
        evalBoard->setDacHighpassFilter(highpassFilterFrequency);
    }

    if (spikeScopeDialog) {
        spikeScopeDialog->setSampleRate(boardSampleRate);
    }

    impedanceFreqValid = false;
    updateImpedanceFrequency();

    wavePlot->setFocus();
}

// Attempt to open a USB interface board connected to a USB port.  Returns number of SPI ports, and
// indicates whether I/O expander board is connected.
int MainWindow::openInterfaceBoard(bool& expanderBoardDetected)
{
    evalBoard = new Rhs2000EvalBoard;
    expanderBoardDetected = false;

    // Open Opal Kelly XEM6310 board.
    int errorCode = evalBoard->open();

    // cerr << "In MainWindow::openInterfaceBoard.  errorCode = " << errorCode << "\n";

    if (errorCode < 1) {
        QMessageBox::StandardButton r;
        if (errorCode == -1) {
            r = QMessageBox::question(this, tr("Cannot load Opal Kelly FrontPanel DLL"),
                                  tr("Opal Kelly USB drivers not installed.  "
                                     "Click OK to run application with synthesized biopotential data for "
                                     "demonstration purposes."
                                     "<p>To use the Intan Stimulation / Recording Controller, click Cancel, load the correct "
                                     "Opal Kelly drivers, then restart the application."
                                     "<p>Visit http://www.intantech.com for more information."),
                                  QMessageBox::Ok | QMessageBox::Cancel);
        } else {
            r = QMessageBox::question(this, tr("Intan Stimulation / Recording Controller Not Found"),
                                  tr("Intan Technologies Stimulation / Recording Controller not found on any USB port.  "
                                     "Click OK to run application with synthesized neural data for "
                                     "demonstration purposes."
                                     "<p>To use the Intan Stimulation / Recording Controller, click Cancel, connect the device "
                                     "to a USB port, then restart the application."
                                     "<p>Visit http://www.intantech.com for more information."),
                                  QMessageBox::Ok | QMessageBox::Cancel);
        }
        if (r == QMessageBox::Ok) {
            QMessageBox::information(this, tr("Synthesized Data Mode"),
                                     tr("The software will generate synthetic neural spikes for "
                                        "demonstration purposes."
                                        "<p>In demonstration mode, the audio output will not work since this "
                                        "requires the line out signal from the interface board.  Also, electrode "
                                        "impedance testing is disabled in this mode.  Stimulation settings can be "
                                        "configured, but stimulation pulses are not emulated."),
                                     QMessageBox::Ok);
            synthMode = true;
            delete evalBoard;
            evalBoard = 0;
            return 4;
        } else {
            exit(EXIT_FAILURE); // abort application
        }
    }

    // Load Rhythm FPGA configuration bitfile (provided by Intan Technologies).
    string bitfilename =
            QString(QCoreApplication::applicationDirPath() + "/main.bit").toStdString();

    if (!evalBoard->uploadFpgaBitfile(bitfilename)) {
        QMessageBox::critical(this, tr("Hardware Configuration File Upload Error"),
                              tr("Cannot upload configuration file to Intan Stimulation / Recording Controller.  Make sure file main.bit "
                                 "is in the same directory as the executable file."));
        exit(EXIT_FAILURE); // abort application
    }

    evalBoard->resetBoard();
    // Read 4-bit board mode.
    evalBoardMode = evalBoard->getBoardMode();

    if (evalBoardMode != RHS_BOARD_MODE) {
        QMessageBox::critical(this, tr("Incompatible Controller Error"),
                              tr("The device connected to the USB port is not an Intan Stimulation / Recording Controller."));
        exit(EXIT_FAILURE); // abort application
    }

    evalBoard->readDigitalInManual(expanderBoardDetected);
    evalBoard->readDigitalInExpManual();
    return 4;
}

// Initialize a USB interface board connected to a USB port.
void MainWindow::initializeInterfaceBoard()
{
    // Initialize interface board.
    evalBoard->initialize();

    evalBoard->enableDcAmpConvert(true);
    evalBoard->setExtraStates(0);

    // Set sample rate and upload all auxiliary SPI command sequences.
    changeSampleRate(sampleRateIndex, true);

    // Since our longest command sequence is 128 commands, we run the SPI
    // interface for 128 samples.
    evalBoard->setMaxTimeStep(SAMPLES_PER_DATA_BLOCK);
    evalBoard->setContinuousRunMode(false);

    // Start SPI interface.
    evalBoard->run();

    // Wait for the 128-sample run to complete.
    while (evalBoard->isRunning()) {
        qApp->processEvents();
    }

    // Read the resulting single data block from the USB interface.
    Rhs2000DataBlock *dataBlock = new Rhs2000DataBlock(evalBoard->getNumEnabledDataStreams());
    evalBoard->readDataBlock(dataBlock);

    // We don't need to do anything with this data block; it was used to configure
    // the RHS2000 amplifier chips and to run ADC calibration.
    delete dataBlock;

    // Set default configuration for all eight DACs on interface board.
    evalBoard->enableDac(0, false);
    evalBoard->enableDac(1, false);
    evalBoard->enableDac(2, false);
    evalBoard->enableDac(3, false);
    evalBoard->enableDac(4, false);
    evalBoard->enableDac(5, false);
    evalBoard->enableDac(6, false);
    evalBoard->enableDac(7, false);
    evalBoard->selectDacDataStream(0, 8);   // Initially point DACs to DacManual1 input
    evalBoard->selectDacDataStream(1, 8);
    evalBoard->selectDacDataStream(2, 8);
    evalBoard->selectDacDataStream(3, 8);
    evalBoard->selectDacDataStream(4, 8);
    evalBoard->selectDacDataStream(5, 8);
    evalBoard->selectDacDataStream(6, 8);
    evalBoard->selectDacDataStream(7, 8);
    evalBoard->selectDacDataChannel(0, 0);
    evalBoard->selectDacDataChannel(1, 1);
    evalBoard->selectDacDataChannel(2, 0);
    evalBoard->selectDacDataChannel(3, 0);
    evalBoard->selectDacDataChannel(4, 0);
    evalBoard->selectDacDataChannel(5, 0);
    evalBoard->selectDacDataChannel(6, 0);
    evalBoard->selectDacDataChannel(7, 0);
    evalBoard->setDacManual(32768);
    evalBoard->setDacGain(0);
    evalBoard->setAudioNoiseSuppress(0);

    evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortA, 0.0);
    evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortB, 0.0);
    evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortC, 0.0);
    evalBoard->setCableLengthMeters(Rhs2000EvalBoard::PortD, 0.0);
}

// Scan SPI Ports A-D to find all connected RHS2000 amplifier chips.
// Read the chip ID from on-chip ROM register 63 to determine the number
// of amplifier channels on each port.  This process is repeated at all
// possible MISO delays in the FPGA, and the cable length on each port
// is inferred from this.
void MainWindow::findConnectedAmplifiers()
{
    int delay, stream, id, i, channel, port;
    // int auxName, vddName;
    int numChannelsOnPort[MAX_NUM_SPI_PORTS] = {0, 0, 0, 0};
    QVector<int> portIndex, portIndexOld, chipIdOld, commandStream;

    portIndex.resize(MAX_NUM_DATA_STREAMS);
    portIndexOld.resize(MAX_NUM_DATA_STREAMS);
    chipIdOld.resize(MAX_NUM_DATA_STREAMS);
    commandStream.resize(MAX_NUM_DATA_STREAMS);

    chipId.fill(-1);
    chipIdOld.fill(-1);
    portIndexOld.fill(-1);
    portIndex.fill(-1);
    commandStream.fill(-1);

    if (!synthMode) {
        // Set sampling rate to highest value for maximum temporal resolution.
//        changeSampleRate(sampleRateComboBox->count() - 1);

        portIndexOld[0]  = 0;
        portIndexOld[1]  = 0;
        portIndexOld[2]  = 1;
        portIndexOld[3]  = 1;
        portIndexOld[4]  = 2;
        portIndexOld[5]  = 2;
        portIndexOld[6]  = 3;
        portIndexOld[7]  = 3;

        // Enable all data streams
        for (i = 0; i < MAX_NUM_DATA_STREAMS; i++) {
            evalBoard->enableDataStream(i, true);
        }

//        evalBoard->selectAuxCommandBank(Rhs2000EvalBoard::PortA,
//                                        Rhs2000EvalBoard::AuxCmd3, 0);
//        evalBoard->selectAuxCommandBank(Rhs2000EvalBoard::PortB,
//                                        Rhs2000EvalBoard::AuxCmd3, 0);
//        evalBoard->selectAuxCommandBank(Rhs2000EvalBoard::PortC,
//                                        Rhs2000EvalBoard::AuxCmd3, 0);
//        evalBoard->selectAuxCommandBank(Rhs2000EvalBoard::PortD,
//                                        Rhs2000EvalBoard::AuxCmd3, 0);

        // Since our longest command sequence is 128 commands, we run the SPI
        // interface for 128 samples.
        evalBoard->setMaxTimeStep(SAMPLES_PER_DATA_BLOCK);
        evalBoard->setContinuousRunMode(false);

        Rhs2000DataBlock *dataBlock =
                new Rhs2000DataBlock(evalBoard->getNumEnabledDataStreams());

        QVector<int> sumGoodDelays(MAX_NUM_DATA_STREAMS, 0);
        QVector<int> indexFirstGoodDelay(MAX_NUM_DATA_STREAMS, -1);
        QVector<int> indexSecondGoodDelay(MAX_NUM_DATA_STREAMS, -1);

        // Run SPI command sequence at all 16 possible FPGA MISO delay settings
        // to find optimum delay for each SPI interface cable.
        for (delay = 0; delay < 16; ++delay) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortA, delay);
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortB, delay);
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortC, delay);
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortD, delay);

            // Start SPI interface.
            evalBoard->run();

            // Wait for the 128-sample run to complete.
            while (evalBoard->isRunning()) {
                qApp->processEvents();
            }

            // Read the resulting single data block from the USB interface.
            evalBoard->readDataBlock(dataBlock);

            // Read the Intan chip ID number from each RHS2000 chip found.
            // Record delay settings that yield good communication with the chip.
            for (stream = 0; stream < MAX_NUM_DATA_STREAMS; ++stream) {
                id = deviceId(dataBlock, stream);

                if (id == CHIP_ID_RHS2116) {
                    // cout << "Delay: " << delay << " on stream " << stream << " is good." << endl;
                    sumGoodDelays[stream] = sumGoodDelays[stream] + 1;
                    if (indexFirstGoodDelay[stream] == -1) {
                        indexFirstGoodDelay[stream] = delay;
                        chipIdOld[stream] = id;
                    } else if (indexSecondGoodDelay[stream] == -1) {
                        indexSecondGoodDelay[stream] = delay;
                        chipIdOld[stream] = id;
                    }
                }
            }
        }

        // Set cable delay settings that yield good communication with each
        // RHS2000 chip.
        QVector<int> optimumDelay(MAX_NUM_DATA_STREAMS, 0);
        for (stream = 0; stream < MAX_NUM_DATA_STREAMS; ++stream) {
            if (sumGoodDelays[stream] == 1 || sumGoodDelays[stream] == 2) {
                optimumDelay[stream] = indexFirstGoodDelay[stream];
            } else if (sumGoodDelays[stream] > 2) {
                optimumDelay[stream] = indexSecondGoodDelay[stream];
            }
        }

        evalBoard->setCableDelay(Rhs2000EvalBoard::PortA,
                                 qMax(optimumDelay[0], optimumDelay[1]));
        evalBoard->setCableDelay(Rhs2000EvalBoard::PortB,
                                 qMax(optimumDelay[2], optimumDelay[3]));
        evalBoard->setCableDelay(Rhs2000EvalBoard::PortC,
                                 qMax(optimumDelay[4], optimumDelay[5]));
        evalBoard->setCableDelay(Rhs2000EvalBoard::PortD,
                                 qMax(optimumDelay[6], optimumDelay[7]));

        cout << "Port A cable delay: " << qMax(optimumDelay[0], optimumDelay[1]) << endl;
        cout << "Port B cable delay: " << qMax(optimumDelay[2], optimumDelay[3]) << endl;
        cout << "Port C cable delay: " << qMax(optimumDelay[4], optimumDelay[5]) << endl;
        cout << "Port D cable delay: " << qMax(optimumDelay[6], optimumDelay[7]) << endl;

        cableLengthPortA =
                evalBoard->estimateCableLengthMeters(qMax(optimumDelay[0], optimumDelay[1]));
        cableLengthPortB =
                evalBoard->estimateCableLengthMeters(qMax(optimumDelay[2], optimumDelay[3]));
        cableLengthPortC =
                evalBoard->estimateCableLengthMeters(qMax(optimumDelay[4], optimumDelay[5]));
        cableLengthPortD =
                evalBoard->estimateCableLengthMeters(qMax(optimumDelay[6], optimumDelay[7]));

        delete dataBlock;

    } else {
        // If we are running with synthetic data (i.e., no interface board), just assume
        // that one RHS2116 is plugged into Port A.
        chipIdOld[0] = CHIP_ID_RHS2116;
        portIndexOld[0] = 0;
    }

    // Now that we know which RHS2000 amplifier chips are plugged into each SPI port,
    // add up the total number of amplifier channels on each port and calcualate the number
    // of data streams necessary to convey this data over the USB interface.
    int numStreamsRequired = 0;
    for (stream = 0; stream < MAX_NUM_DATA_STREAMS; ++stream) {
        if (chipIdOld[stream] == CHIP_ID_RHS2116) {
            numStreamsRequired++;
            if (numStreamsRequired <= MAX_NUM_DATA_STREAMS) {
                numChannelsOnPort[portIndexOld[stream]] += 16;
            }
        }
    }

    // Reconfigure USB data streams in consecutive order to accommodate all connected chips.
    stream = 0;
    for (int oldStream = 0; oldStream < MAX_NUM_DATA_STREAMS; ++oldStream) {
        if ((chipIdOld[oldStream] == CHIP_ID_RHS2116) && (stream < MAX_NUM_DATA_STREAMS)) {
            chipId[stream] = CHIP_ID_RHS2116;
            portIndex[stream] = portIndexOld[oldStream];
            if (!synthMode) {
                evalBoard->enableDataStream(oldStream, true);
            }
            commandStream[stream] = oldStream;
            stream++ ;
        } else {
            if (!synthMode) {
                evalBoard->enableDataStream(oldStream, false);
            }
        }
    }

    // Add channel descriptions to the SignalSources object to create a list of all waveforms.
    for (port = 0; port < numSpiPorts; ++port) {
        if (numChannelsOnPort[port] == 0) {
            signalSources->signalPort[port].channel.clear();
            signalSources->signalPort[port].enabled = false;
        } else if (signalSources->signalPort[port].numAmplifierChannels() !=
                   numChannelsOnPort[port]) {  // if number of channels on port has changed...
            signalSources->signalPort[port].channel.clear();  // ...clear existing channels...
            // ...and create new ones.
            channel = 0;
            // Create amplifier channels for each chip.
            for (stream = 0; stream < MAX_NUM_DATA_STREAMS; ++stream) {
                if (portIndex[stream] == port) {
                    if (chipId[stream] == CHIP_ID_RHS2116) {
                        for (i = 0; i < 16; ++i) {
                            signalSources->signalPort[port].addAmplifierChannel(channel, i, stream);
                            signalSources->signalPort[port].channel[channel].commandStream = commandStream[stream];
                            channel++;
                        }
                    }
                }
            }
        } else {    // If number of channels on port has not changed, don't create new channels (since this
                    // would clear all user-defined channel names.  But we must update the data stream indices
                    // on the port.
            channel = 0;
            // Update stream indices for amplifier channels.
            for (stream = 0; stream < MAX_NUM_DATA_STREAMS; ++stream) {
                if (portIndex[stream] == port) {
                    if (chipId[stream] == CHIP_ID_RHS2116) {
                        for (i = channel; i < channel + 16; ++i) {
                            signalSources->signalPort[port].channel[i].boardStream = stream;
                        }
                        channel += 16;
                    }
                }
            }
        }
    }

    // Update SPI Port radio buttons in GUI

    if (signalSources->signalPort[0].numAmplifierChannels() == 0) {
        signalSources->signalPort[0].enabled = false;
        displayPortAButton->setEnabled(false);
        displayPortAButton->setText(signalSources->signalPort[0].name);
    } else {
        signalSources->signalPort[0].enabled = true;
        displayPortAButton->setEnabled(true);
        displayPortAButton->setText(signalSources->signalPort[0].name +
          QString(" (") + QString::number(signalSources->signalPort[0].numAmplifierChannels()) +
          QString(" channels)"));
    }

    if (signalSources->signalPort[1].numAmplifierChannels() == 0) {
        signalSources->signalPort[1].enabled = false;
        displayPortBButton->setEnabled(false);
        displayPortBButton->setText(signalSources->signalPort[1].name);
    } else {
        signalSources->signalPort[1].enabled = true;
        displayPortBButton->setEnabled(true);
        displayPortBButton->setText(signalSources->signalPort[1].name +
          " (" + QString::number(signalSources->signalPort[1].numAmplifierChannels()) +
          " channels)");
    }

    if (signalSources->signalPort[2].numAmplifierChannels() == 0) {
        signalSources->signalPort[2].enabled = false;
        displayPortCButton->setEnabled(false);
        displayPortCButton->setText(signalSources->signalPort[2].name);
    } else {
        signalSources->signalPort[2].enabled = true;
        displayPortCButton->setEnabled(true);
        displayPortCButton->setText(signalSources->signalPort[2].name +
          " (" + QString::number(signalSources->signalPort[2].numAmplifierChannels()) +
          " channels)");
    }

    if (signalSources->signalPort[3].numAmplifierChannels() == 0) {
        signalSources->signalPort[3].enabled = false;
        displayPortDButton->setEnabled(false);
        displayPortDButton->setText(signalSources->signalPort[3].name);
    } else {
        signalSources->signalPort[3].enabled = true;
        displayPortDButton->setEnabled(true);
        displayPortDButton->setText(signalSources->signalPort[3].name +
          " (" + QString::number(signalSources->signalPort[3].numAmplifierChannels()) +
          " channels)");
    }

    if (signalSources->signalPort[0].numAmplifierChannels() > 0) {
        displayPortAButton->setChecked(true);
    } else if (signalSources->signalPort[1].numAmplifierChannels() > 0) {
        displayPortBButton->setChecked(true);
    } else if (signalSources->signalPort[2].numAmplifierChannels() > 0) {
        displayPortCButton->setChecked(true);
    } else if (signalSources->signalPort[3].numAmplifierChannels() > 0) {
        displayPortDButton->setChecked(true);
    } else {
        displayAdcButton->setChecked(true);
    }

    // Return sample rate to original user-selected value.
//    changeSampleRate(sampleRateComboBox->currentIndex());

//    signalSources->signalPort[0].print();
//    signalSources->signalPort[1].print();
//    signalSources->signalPort[2].print();
//    signalSources->signalPort[3].print();

}

// Return the Intan chip ID stored in ROM register 255.  If the data is invalid
// (due to a SPI communication channel with the wrong delay or a chip not present)
// then return -1.
int MainWindow::deviceId(Rhs2000DataBlock *dataBlock, int stream)
{
    bool intanChipPresent;

    // First, check ROM registers 251-253 to verify that they hold 'INTAN'.
    // This is just used to verify that we are getting good data over the SPI
    // communication channel.
    intanChipPresent = ((char) ((dataBlock->auxiliaryData[stream][0][61] & 0xff00) >> 8) == 'I' &&
                        (char) ((dataBlock->auxiliaryData[stream][0][61] & 0x00ff) >> 0) == 'N' &&
                        (char) ((dataBlock->auxiliaryData[stream][0][60] & 0xff00) >> 8) == 'T' &&
                        (char) ((dataBlock->auxiliaryData[stream][0][60] & 0x00ff) >> 0) == 'A' &&
                        (char) ((dataBlock->auxiliaryData[stream][0][59] & 0xff00) >> 8) == 'N' &&
                        (char) ((dataBlock->auxiliaryData[stream][0][59] & 0x00ff) >> 0) == 0);

    if (!intanChipPresent) {
        return -1;
    } else {
        return dataBlock->auxiliaryData[stream][0][57]; // chip ID (Register 255)
    }
}

// Start recording data from USB interface board to disk.
void MainWindow::recordInterfaceBoard()
{
    // Create list of enabled channels that will be saved to disk.
    signalProcessor->createSaveList(signalSources, false, 0, Rhs2000Registers::stimStepSizeToDouble(stimStep) /  1.0e-6);

    startNewSaveFile(saveFormat);

    // Write save file header information.
    writeSaveFileHeader(*saveStream, *infoStream, saveFormat);

    // Disable some GUI buttons while recording is in progress.
    enableChannelButton->setEnabled(false);
    enableAllButton->setEnabled(false);
    disableAllButton->setEnabled(false);
//    sampleRateComboBox->setEnabled(false);
    // recordFileSpinBox->setEnabled(false);
    setSaveFormatButton->setEnabled(false);

    recording = true;
    triggerSet = false;
    triggered = false;
    runInterfaceBoard();
}

// Wait for user-defined trigger to start recording data from USB interface board to disk.
void MainWindow::triggerRecordInterfaceBoard()
{
    TriggerRecordDialog triggerRecordDialog(recordTriggerChannel, recordTriggerPolarity,
                                            recordTriggerBuffer, postTriggerTime,
                                            saveTriggerChannel, this);

    if (triggerRecordDialog.exec()) {
        recordTriggerChannel = triggerRecordDialog.digitalInput;
        recordTriggerPolarity = triggerRecordDialog.triggerPolarity;
        recordTriggerBuffer = triggerRecordDialog.recordBuffer;
        postTriggerTime = triggerRecordDialog.postTriggerTime;
        saveTriggerChannel = (triggerRecordDialog.saveTriggerChannelCheckBox->checkState() == Qt::Checked);

        // Create list of enabled channels that will be saved to disk.
        signalProcessor->createSaveList(signalSources, saveTriggerChannel, recordTriggerChannel, Rhs2000Registers::stimStepSizeToDouble(stimStep) /  1.0e-6);

        // Disable some GUI buttons while recording is in progress.
        enableChannelButton->setEnabled(false);
        enableAllButton->setEnabled(false);
        disableAllButton->setEnabled(false);
//        sampleRateComboBox->setEnabled(false);
        // recordFileSpinBox->setEnabled(false);
        setSaveFormatButton->setEnabled(false);

        recording = false;
        triggerSet = true;
        triggered = false;
        runInterfaceBoard();
    }

    wavePlot->setFocus();
}

// Write header to data save file, containing information on
// recording settings, amplifier parameters, and signal sources.
void MainWindow::writeSaveFileHeader(QDataStream &outStream, QDataStream &infoStream,
                                     SaveFormat format)
{
    for (int i = 0; i < 16; ++i) {
        signalSources->signalPort[numSpiPorts + 3].channel[i].enabled = saveTtlOut;
    }

    switch (format) {
    case SaveFormatIntan:
        outStream << (quint32) DATA_FILE_MAGIC_NUMBER;
        outStream << (qint16) DATA_FILE_MAIN_VERSION_NUMBER;
        outStream << (qint16) DATA_FILE_SECONDARY_VERSION_NUMBER;

        outStream << boardSampleRate;

        outStream << (qint16) dspEnabled;
        outStream << actualDspCutoffFreq;
        outStream << actualLowerBandwidth;
        outStream << actualLowerSettleBandwidth;
        outStream << actualUpperBandwidth;

        outStream << desiredDspCutoffFreq;
        outStream << desiredLowerBandwidth;
        outStream << desiredLowerSettleBandwidth;
        outStream << desiredUpperBandwidth;

        outStream << (qint16) notchFilterComboBox->currentIndex();

        outStream << desiredImpedanceFreq;
        outStream << actualImpedanceFreq;

        outStream << (qint16) useFastSettle;
        outStream << (qint16) chargeRecoveryMode;

        outStream << Rhs2000Registers::stimStepSizeToDouble(stimStep);
        outStream << Rhs2000Registers::chargeRecoveryCurrentLimitToDouble(chargeRecoveryCurrentLimit);
        outStream << chargeRecoveryTargetVoltage;

        outStream << note1LineEdit->text();
        outStream << note2LineEdit->text();
        outStream << note3LineEdit->text();

        outStream << (qint16) saveDcAmps;

        outStream << (qint16) evalBoardMode;

        if (referenceSource.softwareMode) {
            outStream << signalSources->findAmplifierChannel(referenceSource.stream, referenceSource.channel)->nativeChannelName;
        } else {
            outStream << QString("n/a");
        }

        outStream << *signalSources;

        break;

    case SaveFormatFilePerSignalType:
    case SaveFormatFilePerChannel:
        infoStream << (quint32) DATA_FILE_MAGIC_NUMBER;
        infoStream << (qint16) DATA_FILE_MAIN_VERSION_NUMBER;
        infoStream << (qint16) DATA_FILE_SECONDARY_VERSION_NUMBER;

        infoStream << boardSampleRate;

        infoStream << (qint16) dspEnabled;
        infoStream << actualDspCutoffFreq;
        infoStream << actualLowerBandwidth;
        infoStream << actualLowerSettleBandwidth;
        infoStream << actualUpperBandwidth;

        infoStream << desiredDspCutoffFreq;
        infoStream << desiredLowerBandwidth;
        infoStream << desiredLowerSettleBandwidth;
        infoStream << desiredUpperBandwidth;

        infoStream << (qint16) notchFilterComboBox->currentIndex();

        infoStream << desiredImpedanceFreq;
        infoStream << actualImpedanceFreq;

        infoStream << (qint16) useFastSettle;
        infoStream << (qint16) chargeRecoveryMode;

        infoStream << Rhs2000Registers::stimStepSizeToDouble(stimStep);
        infoStream << Rhs2000Registers::chargeRecoveryCurrentLimitToDouble(chargeRecoveryCurrentLimit);
        infoStream << chargeRecoveryTargetVoltage;

        infoStream << note1LineEdit->text();
        infoStream << note2LineEdit->text();
        infoStream << note3LineEdit->text();

        infoStream << (qint16) 0;

        infoStream << (qint16) evalBoardMode;

        if (referenceSource.softwareMode) {
            infoStream << signalSources->findAmplifierChannel(referenceSource.stream, referenceSource.channel)->nativeChannelName;
        } else {
            infoStream << QString("n/a");
        }

        infoStream << *signalSources;

        break;
    }
}

// Start SPI communication to all connected RHS2000 amplifiers and stream
// waveform data over USB port.
void MainWindow::runInterfaceBoard()
{
    bool newDataReady = false;
    int triggerIndex;
    QTime timer;
    int timestampOffset = 0;
    unsigned int preTriggerBufferQueueLength = 0;
    queue<Rhs2000DataBlock> bufferQueue;
    Rhs2000DataBlock* dataBlock = nullptr;
    static int fifoNearlyFull = 0;
    static int triggerEndCounter = 0;
    int triggerEndThreshold;
    bool hasBeenUpdated = false;
    int index;
    unsigned int sample;

    triggerEndThreshold = qCeil(postTriggerTime * boardSampleRate / (numUsbBlocksToRead * SAMPLES_PER_DATA_BLOCK)) - 1;

    if (triggerSet) {
        preTriggerBufferQueueLength = numUsbBlocksToRead *
                (qCeil(recordTriggerBuffer / (numUsbBlocksToRead * Rhs2000DataBlock::getSamplesPerDataBlock() / boardSampleRate)) + 1);
    }

    QSound triggerBeep(QDir::tempPath() + "/triggerbeep.wav");
    QSound triggerEndBeep(QDir::tempPath() + "/triggerendbeep.wav");

    running = true;
    wavePlot->setFocus();

    // Enable stop button on GUI while running
    stopButton->setEnabled(true);

    // Disable various buttons on GUI while running
    runButton->setEnabled(false);
    recordButton->setEnabled(false);
    triggerButton->setEnabled(false);

    baseFilenameButton->setEnabled(false);
    renameChannelButton->setEnabled(false);
    changeBandwidthButton->setEnabled(false);
    impedanceFreqSelectButton->setEnabled(false);
    runImpedanceTestButton->setEnabled(false);
    scanButton->setEnabled(false);
    setCableDelayButton->setEnabled(false);
    setSaveFormatButton->setEnabled(false);
    stimParamButton->setEnabled(false);
    ampSettleSettingsAction->setEnabled(false);
    chargeRecoverySettingsAction->setEnabled(false);

    unsigned int dataBlockSize;
    unsigned int numBytesToRead;

    if (synthMode) {
        dataBlockSize = Rhs2000DataBlock::calculateDataBlockSizeInWords(1);
    } else {
        dataBlock = new Rhs2000DataBlock(evalBoard->getNumEnabledDataStreams());
        dataBlockSize = Rhs2000DataBlock::calculateDataBlockSizeInWords(
                    evalBoard->getNumEnabledDataStreams());
    }
    unsigned int sampleSizeInBytes = 2 * dataBlockSize / SAMPLES_PER_DATA_BLOCK;

    unsigned int wordsInFifo;
    double fifoPercentageFull, fifoCapacity, samplePeriod, latency;
    long long totalBytesWritten = 0;
    double totalRecordTimeSeconds = 0.0;
    double totalElapsedRecordTimeSeconds = 0.0;
    double recordTimeIncrementSeconds = numUsbBlocksToRead *
            Rhs2000DataBlock::getSamplesPerDataBlock() / boardSampleRate;

    // Calculate the number of bytes per minute that we will be saving to disk
    // if recording data (excluding headers).
    double bytesPerMinute = Rhs2000DataBlock::getSamplesPerDataBlock() *
            ((double) signalProcessor->bytesPerBlock(saveFormat, saveTtlOut) /
             (double) Rhs2000DataBlock::getSamplesPerDataBlock()) * boardSampleRate;

    samplePeriod = 1.0 / boardSampleRate;
    fifoCapacity = Rhs2000EvalBoard::fifoCapacityInWords();

    if (recording) {
        setStatusBarRecording(bytesPerMinute, totalElapsedRecordTimeSeconds);
    } else if (triggerSet) {
        setStatusBarWaitForTrigger();
    } else {
        setStatusBarRunning();
    }

    if (!synthMode) {
        usbDataThread->start();
        usbDataThread->startRunning();
    } else {
        timer.start();
    }

    // Timers to measure performance
    QElapsedTimer readTimer, loopTimer, processingTimer;
    int extraCycles = 0;
    readTimer.start();
    loopTimer.start();
    processingTimer.start();
    qint64 readTime, loopTime, processingTime, idleTime;
    double dutyCycle, cpuFree;

    while (running) {
        if (recording) {
            setStatusBarRecording(bytesPerMinute, totalElapsedRecordTimeSeconds);
        }

        // If we are running in demo mode, use a timer to periodically generate more synthetic
        // data.  If not, wait for a certain amount of data to be ready from the USB interface board.
        if (synthMode) {
            newDataReady = (timer.elapsed() >=
                            ((int) (1000.0 * SAMPLES_PER_DATA_BLOCK * (double) numUsbBlocksToRead / boardSampleRate)));
        } else {
            readTimer.restart();
            usbDataThread->setNumUsbBlocksToRead(numUsbBlocksToRead);
            numBytesToRead = numUsbBlocksToRead * 2 * Rhs2000DataBlock::calculateDataBlockSizeInWords(evalBoard->getNumEnabledDataStreams());
            newDataReady = usbStreamFifo->readFromBuffer(usbReadBuffer, numBytesToRead);

            if (newDataReady) {
                bufferFullLabel->setText(QString::number(usbStreamFifo->percentFull(), 'f', 0) + "%");
                if (usbStreamFifo->percentFull() > 75.0) {
                    bufferFullLabel->setStyleSheet("color: red");
                } else {
                    bufferFullLabel->setStyleSheet("color: black");
                }

                if (extraCycles == 0) {
                    cpuWarningLabel->show();
                } else {
                    cpuWarningLabel->hide();
                }
                extraCycles = 0;

                // USB data error checking

                /*
                // Spoof USB glitches by dropping bytes (for debugging purposes only)
                static int glitchCounter = 0;
                if (glitchCounter++ == 50) {
                    // Introduce USB 'glitch'
                    glitchCounter = 0;
                    cout << "USB GLITCH!" << endl;

                    unsigned int numWordsToSwallow = 35;
                    unsigned int glitchPosition = 1000;

                    // Throw away some words from the USB buffer...
                    for (unsigned int i = glitchPosition; i < numBytesToRead - 2 * numWordsToSwallow; ++i) {
                        usbReadBuffer[i] = usbReadBuffer[i + 2 * numWordsToSwallow];
                    }
                    // ...and fill the buffer back up from the USB port.

                    while (usbStreamFifo->bytesAvailable() < 2 * numWordsToSwallow) {    // ...wait for data word to become available...
                        qApp->processEvents();  // Stay responsive to GUI events during this loop
                    }

                    // ...and read N more words (2N more bytes) from USB read buffer, and append it to the end.
                    usbStreamFifo->readFromBuffer(&usbReadBuffer[numBytesToRead - 2 * numWordsToSwallow], 2 * numWordsToSwallow);
                }
                */

                // Look for proper 'magic number' header in all data blocks to check for USB glitches

                index = 0;
                for (sample = 0; sample < numUsbBlocksToRead * SAMPLES_PER_DATA_BLOCK; ++sample) {
                    if (!(dataBlock->checkUsbHeader(usbReadBuffer, index))) {
                        if (sample > 0) {
                            // If we have a bad data sample header on any sample but the first, we shouldn't trust
                            // the integrity of the prior sample, since it is likely contains a "hole" where missing
                            // USB data should be.  Jump back one sample and try to fix that one.
                            sample--;
                            index -= sampleSizeInBytes;
                        }

                        // Search for correct header throughout the sample.
                        int lag = sampleSizeInBytes / 2;
                        for (unsigned int i = 1; i < sampleSizeInBytes / 2; ++i) {
                            if (dataBlock->checkUsbHeader(usbReadBuffer, index + 2 * i)) {
                                lag = i;
                                break;
                            }
                        }
                        // Realign data and read additional words from the USB to refill buffer.

                        unsigned int numBytes = 2 * lag;
                        // Shift all data beyond error point back by N words (2N bytes)...
                        for (unsigned int i = index; i < numBytesToRead - numBytes; i += 2) {
                            usbReadBuffer[i] = usbReadBuffer[i + numBytes];
                            usbReadBuffer[i + 1] = usbReadBuffer[i + numBytes + 1];
                        }

                        while (usbStreamFifo->bytesAvailable() < numBytes) {    // ...wait for data word to become available...
                            qApp->processEvents();  // Stay responsive to GUI events during this loop
                        }

                        // ...and read N more words (2N more bytes) from USB read buffer, and append it to the end.
                        usbStreamFifo->readFromBuffer(&usbReadBuffer[numBytesToRead - numBytes], numBytes);
                    }
                    index += sampleSizeInBytes;
                }

                /*
                // Re-check USB headers (for debugging purposes only)
                index = 0;
                for (sample = 0; sample < numUsbBlocksToRead * SAMPLES_PER_DATA_BLOCK; ++sample) {
                    if (!(dataBlock->checkUsbHeader(usbReadBuffer, index))) {
                        cerr << "Unfixed header error at sample " << sample << endl;
                    }
                    index += sampleSizeInBytes;
                }
                */

                // End of USB error checking

                for (unsigned int j = 0; j < numUsbBlocksToRead; ++j) {
                    dataBlock->fillFromUsbBuffer(usbReadBuffer, j, evalBoard->getNumEnabledDataStreams());
                    dataQueue.push(*dataBlock);
                }

                readTime = readTimer.restart();
                loopTime = loopTimer.restart();
                idleTime = loopTime - readTime - processingTime;
                dutyCycle = 100.0 * ((double)readTime) / ((double)loopTime);
                cpuFree = 100.0 * ((double)idleTime) / ((double)loopTime);
                // cout << "Loop: " << loopTime << "ms.  Processing: " << processingTime << " ms.  USB: " << readTime << " ms " <<
                //         (int)dutyCycle << "%.  " << "CPU idle: " << (int)cpuFree << "%" << endl;
            } else {
                extraCycles++;
            }
        }

        // If new data is ready, then read it.
        if (newDataReady) {
            processingTimer.start();

            if (synthMode) {
                timer.start();  // restart timer
                fifoPercentageFull = 0.0;

                // Generate synthetic data
                totalBytesWritten +=
                        signalProcessor->loadSyntheticData(numUsbBlocksToRead,
                                                           boardSampleRate, recording,
                                                           *saveStream, saveFormat, saveTtlOut, saveDcAmps, referenceSource);
            } else {
                // Check the number of words stored in the Opal Kelly USB interface FIFO.
                wordsInFifo = evalBoard->getLastNumWordsInFifo(hasBeenUpdated);
                if (hasBeenUpdated) {
                    latency = 1000.0 * Rhs2000DataBlock::getSamplesPerDataBlock() *
                            (wordsInFifo / dataBlockSize) * samplePeriod;

                    fifoPercentageFull = 100.0 * wordsInFifo / fifoCapacity;

                    // Alert the user if the number of words in the FIFO is getting to be significant
                    // or nearing FIFO capacity.

                    fifoLagLabel->setText(QString::number(latency, 'f', 0) + " ms");
                    if (latency > 200.0) {
                        fifoLagLabel->setStyleSheet("color: red");
                    } else {
                        fifoLagLabel->setStyleSheet("color: green");
                    }

                    fifoFullLabel->setText(QString::number(fifoPercentageFull, 'f', 0) + "%");
                    if (fifoPercentageFull > 75.0) {
                        fifoFullLabel->setStyleSheet("color: red");
                    } else {
                        fifoFullLabel->setStyleSheet("color: black");
                    }
                }

                // Read waveform data from USB interface board.
                totalBytesWritten +=
                        signalProcessor->loadAmplifierData(dataQueue, (int) numUsbBlocksToRead,
                                                           (triggerSet | triggered), recordTriggerChannel,
                                                           (triggered ? (1 - recordTriggerPolarity) : recordTriggerPolarity),
                                                           triggerIndex, triggerSet, bufferQueue,
                                                           recording, *saveStream, saveFormat,
                                                           saveTtlOut, saveDcAmps, timestampOffset, referenceSource);

                while (bufferQueue.size() > preTriggerBufferQueueLength) {
                    bufferQueue.pop();
                }

                if (triggerSet && (triggerIndex != -1)) {
                    triggerSet = false;
                    triggered = true;
                    recording = true;
                    timestampOffset = triggerIndex;

                    // Play trigger sound
                    triggerBeep.play();

                    startNewSaveFile(saveFormat);

                    // Write save file header information.
                    writeSaveFileHeader(*saveStream, *infoStream, saveFormat);

                    setStatusBarRecording(bytesPerMinute, totalElapsedRecordTimeSeconds);

                    totalRecordTimeSeconds = bufferQueue.size() * Rhs2000DataBlock::getSamplesPerDataBlock() / boardSampleRate;
                    totalElapsedRecordTimeSeconds = totalRecordTimeSeconds;

                    // Write contents of pre-trigger buffer to file.
                    totalBytesWritten += signalProcessor->saveBufferedData(bufferQueue, *saveStream, saveFormat,
                                                                           saveTtlOut, saveDcAmps, timestampOffset);
                } else if (triggered && (triggerIndex != -1)) { // Episodic triggered recording
                    triggerEndCounter++;
                    if (triggerEndCounter > triggerEndThreshold) {
                                                    // Keep recording for the specified number of seconds after the trigger has
                                                    // been de-asserted.
                        triggerEndCounter = 0;
                        triggerSet = true;          // Enable trigger again for true episodic recording.
                        triggered = false;
                        recording = false;
                        closeSaveFile(saveFormat);
                        totalRecordTimeSeconds = 0.0;
                        totalElapsedRecordTimeSeconds = 0.0;

                        setStatusBarWaitForTrigger();

                        // Play trigger end sound
                        triggerEndBeep.play();
                    }
                } else if (triggered) {
                    triggerEndCounter = 0;          // Ignore brief (< 1 second) trigger-off events.
                }
            }

            // Apply notch filter to amplifier data.
            signalProcessor->filterData(numUsbBlocksToRead, channelVisible);

            // Trigger WavePlot widget to display new waveform data.
            wavePlot->passFilteredData();

            // Trigger Spike Scope to update with new waveform data.
            if (spikeScopeDialog) {
                spikeScopeDialog->updateWaveform(numUsbBlocksToRead);
            }

            // If we are recording in Intan format and our data file has reached its specified
            // maximum length (e.g., 1 minute), close the current data file and open a new one.

            if (recording) {
                totalRecordTimeSeconds += recordTimeIncrementSeconds;
                totalElapsedRecordTimeSeconds += recordTimeIncrementSeconds;

                if (saveFormat == SaveFormatIntan) {
                    if (totalRecordTimeSeconds >= (60 * newSaveFilePeriodMinutes)) {
                        closeSaveFile(saveFormat);
                        startNewSaveFile(saveFormat);

                        // Write save file header information.
                        writeSaveFileHeader(*saveStream, *infoStream, saveFormat);

                        setStatusBarRecording(bytesPerMinute, totalElapsedRecordTimeSeconds);

                        totalRecordTimeSeconds = 0.0;
                    }
                }
            }

            // If the USB interface FIFO (on the FPGA board) exceeds 95% full, halt
            // data acquisition and display a warning message.
            if (fifoPercentageFull > 95.0 && hasBeenUpdated) {
                fifoNearlyFull++;   // We must see the FIFO >95% full three times in a row to eliminate the possiblity
                                    // of a USB glitch causing recording to stop.
                if (fifoNearlyFull > 2) {
                    running = false;

                    // Stop data acquisition
                    if (!synthMode) {
                        evalBoard->setContinuousRunMode(false);
                        evalBoard->setMaxTimeStep(0);
                    }

                    if (recording) {
                        closeSaveFile(saveFormat);
                        recording = false;
                        triggerSet = false;
                        triggered = false;
                    }

                    QMessageBox::critical(this, tr("USB Buffer Overrun Error"),
                                          tr("Recording was stopped because the USB FIFO buffer on the interface "
                                             "board reached maximum capacity.  This happens when the host computer "
                                             "cannot keep up with the data streaming from the interface board."
                                             "<p>Try lowering the sample rate, disabling the notch filter, or reducing "
                                             "the number of waveforms on the screen to reduce CPU load."));
                }
            } else if (hasBeenUpdated) {
                fifoNearlyFull = 0;
            }

            processingTime = processingTimer.elapsed();
        }
        qApp->processEvents();  // Stay responsive to GUI events during this loop
    }

    // Stop data acquisition (when running == false)
    if (!synthMode) {
        usbDataThread->stopRunning();
        while (usbDataThread->isRunning()) {  // Important!  Must wait for usbDataThread to fully stop before we reset usbStreamFifo buffer!
            qApp->processEvents();  // Stay responsive to GUI events during this loop
        }
        usbStreamFifo->resetBuffer();
        evalBoard->resetSequencers();   // reset sequencers
    }

    // Close save file, if recording.
    if (recording) {
        closeSaveFile(saveFormat);
        recording = false;
    }

    // Reset trigger
    triggerSet = false;
    triggered = false;

    totalRecordTimeSeconds = 0.0;
    totalElapsedRecordTimeSeconds = 0.0;
    setStatusBarReady();

    // Enable/disable various GUI buttons.

    runButton->setEnabled(true);
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);
    stopButton->setEnabled(false);

    baseFilenameButton->setEnabled(true);
    renameChannelButton->setEnabled(true);
    changeBandwidthButton->setEnabled(true);
    impedanceFreqSelectButton->setEnabled(true);
    runImpedanceTestButton->setEnabled(impedanceFreqValid);
    scanButton->setEnabled(true);
    setCableDelayButton->setEnabled(true);

    enableChannelButton->setEnabled(true);
    enableAllButton->setEnabled(true);
    disableAllButton->setEnabled(true);
//    sampleRateComboBox->setEnabled(true);
    setSaveFormatButton->setEnabled(true);
    stimParamButton->setEnabled(!(displayDigInButton->isChecked()));
    ampSettleSettingsAction->setEnabled(true);
    chargeRecoverySettingsAction->setEnabled(true);

    while (bufferQueue.size() > 0) {
        bufferQueue.pop();
    }

    delete dataBlock;
}

// Stop SPI data acquisition.
void MainWindow::stopInterfaceBoard()
{
    running = false;
    wavePlot->setFocus();
}

void MainWindow::selectBaseFilenameSlot()
{
    selectBaseFilename(saveFormat);
}

// Display file dialog to allow user to select base filename for data
// save files.
void MainWindow::selectBaseFilename(SaveFormat format)
{
    QString newFileName;

    switch (format) {
    case SaveFormatIntan:
        newFileName = QFileDialog::getSaveFileName(this,
                                                tr("Select Base Filename"), ".",
                                                tr("Intan Data Files (*.rhs)"));
        break;

    case SaveFormatFilePerSignalType:
        newFileName = QFileDialog::getSaveFileName(this,
                                                tr("Select Base Filename"), ".",
                                                tr("Intan Data Files (*.rhs)"));
        break;
    case SaveFormatFilePerChannel:
        newFileName = QFileDialog::getSaveFileName(this,
                                                tr("Select Base Filename"), ".",
                                                tr("Intan Data Files (*.rhs)"));
        break;
    }

    if (!newFileName.isEmpty()) {
        saveBaseFileName = newFileName;
        QFileInfo newFileInfo(newFileName);
        saveFilenameLineEdit->setText(newFileInfo.baseName());
    }
    validFilename = !saveBaseFileName.isEmpty();
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);
    wavePlot->setFocus();
}

// Open Intan Technologies website in the default browser.
void MainWindow::openIntanWebsite()
{
    QDesktopServices::openUrl(QUrl("http://www.intantech.com", QUrl::TolerantMode));
}

void MainWindow::setNumWaveformsComboBox(int index)
{
    numFramesComboBox->setCurrentIndex(index);
}

// Open Spike Scope dialog and initialize it.
void MainWindow::spikeScope()
{
    if (!spikeScopeDialog) {
        spikeScopeDialog = new SpikeScopeDialog(signalProcessor, signalSources,
                                                wavePlot->selectedChannel(), this);
        // add any 'connect' statements here
    }

    spikeScopeDialog->show();
    spikeScopeDialog->raise();
    spikeScopeDialog->activateWindow();
    spikeScopeDialog->setYScale(yScaleComboBox->currentIndex());
    spikeScopeDialog->setSampleRate(boardSampleRate);
    wavePlot->setFocus();
}

// Change selected channel on Spike Scope when user selects a new channel.
void MainWindow::newSelectedChannel(SignalChannel* newChannel)
{
    if (spikeScopeDialog) {
        spikeScopeDialog->setNewChannel(newChannel);
    }

    if (dacLockToSelectedBox->isChecked()) {
        if (newChannel->signalType == AmplifierSignal) {
            // Set DAC 1 to selected channel and label it accordingly.
            dacSelectedChannel[0] = newChannel;
            if (!synthMode) {
                evalBoard->selectDacDataStream(0, newChannel->boardStream);
                evalBoard->selectDacDataChannel(0, newChannel->chipChannel);
            }
            setDacChannelLabel(0, newChannel->customChannelName,
                               newChannel->nativeChannelName);
        }
    }
}

// Load application settings from *.isf (Intan Settings File) file.
void MainWindow::loadSettings()
{
    if (stimParamsHaveChanged) {
        QMessageBox::StandardButton r;
        r = QMessageBox::warning(this, tr("Stimulation Settings Will Be Lost"),
                              tr("Loading general settings will erase all stimulation settings.  "
                                 "Any important stimulation settings should be saved first and reloaded after general "
                                 "settings are loaded.<p>Do you want to continue?"),
                              QMessageBox::Ok | QMessageBox::Cancel);

        if (r != QMessageBox::Ok) return;
    }

    QString loadSettingsFileName;
    loadSettingsFileName = QFileDialog::getOpenFileName(this,
                                                        tr("Select Settings Filename"), ".",
                                                        tr("Intan Settings Files (*.isf)"));
    if (loadSettingsFileName.isEmpty()) {
        wavePlot->setFocus();
        return;
    }

    QFile settingsFile(loadSettingsFileName);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        cerr << "Can't open settings file " <<
                loadSettingsFileName.toStdString() << endl;
        wavePlot->setFocus();
        return;
    }

    // Load settings
    QDataStream inStream(&settingsFile);
    inStream.setVersion(QDataStream::Qt_4_8);
    inStream.setByteOrder(QDataStream::LittleEndian);
    inStream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    qint16 tempQint16;
    quint32 tempQuint32;
    qint32 tempQint32;
    int versionMain, versionSecondary;

    inStream >> tempQuint32;
    if (tempQuint32 != SETTINGS_FILE_MAGIC_NUMBER) {
        QMessageBox::critical(this, tr("Cannot Parse Settings File"),
                              tr("Selected file is not a valid settings file."));
        settingsFile.close();
        wavePlot->setFocus();
        return;
    }

    statusBar()->showMessage("Restoring settings from file...");

    inStream >> tempQint16;
    versionMain = tempQint16;
    inStream >> tempQint16;
    versionSecondary = tempQint16;

    // Eventually check version number here for compatibility issues.

    inStream >> tempQint16;
    if (tempQint16 != numSpiPorts) {
        QMessageBox::critical(this, tr("Cannot Parse Settings File"),
                              tr("Selected file was saved on an unknown system."));
        settingsFile.close();

        wavePlot->refreshScreen();
        statusBar()->clearMessage();
        wavePlot->setFocus();
        return;
    }

    inStream >> *signalSources;

//    inStream >> tempQint16;
//    sampleRateComboBox->setCurrentIndex(tempQint16);
    inStream >> tempQint16;
    yScaleComboBox->setCurrentIndex(tempQint16);
    inStream >> tempQint16;
    yScaleDcAmpComboBox->setCurrentIndex(tempQint16);
    inStream >> tempQint16;
    yScaleAdcComboBox->setCurrentIndex(tempQint16);
    inStream >> tempQint16;
    tScaleComboBox->setCurrentIndex(tempQint16);

    scanPorts();

    inStream >> tempQint16;
    notchFilterComboBox->setCurrentIndex(tempQint16);
    changeNotchFilter(notchFilterComboBox->currentIndex());

    inStream >> saveBaseFileName;
    QFileInfo fileInfo(saveBaseFileName);
    validFilename = !saveBaseFileName.isEmpty();
    if (validFilename) {
        saveFilenameLineEdit->setText(fileInfo.baseName());
    }
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);

    inStream >> tempQint16;
    // recordFileSpinBox->setValue(tempQint16);
    newSaveFilePeriodMinutes = tempQint16;

    inStream >> tempQint16;
    dspEnabled = (bool) tempQint16;
    inStream >> desiredDspCutoffFreq;
    inStream >> desiredLowerBandwidth;
    inStream >> desiredLowerSettleBandwidth;
    inStream >> desiredUpperBandwidth;

    inStream >> desiredImpedanceFreq;
    inStream >> actualImpedanceFreq;
    inStream >> tempQint16;
    impedanceFreqValid = (bool) tempQint16;

    // This will update bandwidth settings on RHS2000 chips and
    // the GUI bandwidth display:
    changeSampleRate(sampleRateIndex, false);

    inStream >> tempQint16;
    dacGainSlider->setValue(tempQint16);
    changeDacGain(tempQint16);

    inStream >> tempQint16;
    dacNoiseSuppressSlider->setValue(tempQint16);
    changeDacNoiseSuppress(tempQint16);

    QVector<QString> dacNamesTemp;
    dacNamesTemp.resize(8);

    for (int i = 0; i < 8; ++i) {
        inStream >> tempQint16;
        dacEnabled[i] = (bool) tempQint16;
        inStream >> dacNamesTemp[i];
        dacSelectedChannel[i] =
                signalSources->findChannelFromName(dacNamesTemp[i]);
        if (dacSelectedChannel[i] == 0) {
            dacEnabled[i] = false;
        }
        if (dacEnabled[i]) {
            setDacChannelLabel(i, dacSelectedChannel[i]->customChannelName,
                               dacSelectedChannel[i]->nativeChannelName);
        } else {
            setDacChannelLabel(i, "n/a", "n/a");
        }
        if (!synthMode) {
            evalBoard->enableDac(i, dacEnabled[i]);
            if (dacEnabled[i]) {
                evalBoard->selectDacDataStream(i, dacSelectedChannel[i]->boardStream);
                evalBoard->selectDacDataChannel(i, dacSelectedChannel[i]->chipChannel);
            } else {
                evalBoard->selectDacDataStream(i, 0);
                evalBoard->selectDacDataChannel(i, 0);
            }
        }
    }
    dacButton1->setChecked(true);
    dacEnableCheckBox->setChecked(dacEnabled[0]);

    inStream >> tempQint16;
    plotPointsCheckBox->setChecked((bool) tempQint16);
    plotPointsMode((bool) tempQint16);

    QString noteText;
    inStream >> noteText;
    note1LineEdit->setText(noteText);
    inStream >> noteText;
    note2LineEdit->setText(noteText);
    inStream >> noteText;
    note3LineEdit->setText(noteText);

    // Ports are saved in reverse order.
    for (int port = numSpiPorts + 1; port >= 0; --port) {
        inStream >> tempQint16;
        if (signalSources->signalPort[port].enabled) {
            wavePlot->setNumFrames(tempQint16, port);
        }
        inStream >> tempQint16;
        if (signalSources->signalPort[port].enabled) {
            wavePlot->setTopLeftFrame(tempQint16, port);
        }
    }

    inStream >> tempQint16;
    recordTriggerChannel = tempQint16;
    inStream >> tempQint16;
    recordTriggerPolarity = tempQint16;
    inStream >> tempQint16;
    recordTriggerBuffer = tempQint16;

    inStream >> tempQint16;
    setSaveFormat((SaveFormat) tempQint16);

    inStream >> tempQint16;
    dacLockToSelectedBox->setChecked((bool) tempQint16);

    inStream >> tempQint32;
    dac1ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold1(tempQint32);
    inStream >> tempQint32;
    dac2ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold2(tempQint32);
    inStream >> tempQint32;
    dac3ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold3(tempQint32);
    inStream >> tempQint32;
    dac4ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold4(tempQint32);
    inStream >> tempQint32;
    dac5ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold5(tempQint32);
    inStream >> tempQint32;
    dac6ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold6(tempQint32);
    inStream >> tempQint32;
    dac7ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold7(tempQint32);
    inStream >> tempQint32;
    dac8ThresholdSpinBox->setValue(tempQint32);
    setDacThreshold8(tempQint32);

    inStream >> tempQint16;
    dac1ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac2ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac3ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac4ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac5ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac6ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac7ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    inStream >> tempQint16;
    dac8ThresholdEnableCheckBox->setChecked((bool) tempQint16);
    dacThresholdEnable();

    inStream >> tempQint16;
    saveTtlOut = (bool) tempQint16;

    inStream >> tempQint16;
    saveDcAmps = (bool) tempQint16;

    inStream >> tempQint16;
    enableHighpassFilter((bool) tempQint16);
    highpassFilterCheckBox->setChecked(highpassFilterEnabled);

    inStream >> highpassFilterFrequency;
    highpassFilterLineEdit->setText(QString::number(highpassFilterFrequency, 'f', 2));
    setHighpassFilterCutoff(highpassFilterFrequency);

    for (int i = 0; i < MAX_NUM_SPI_PORTS; ++i) {
        inStream >> tempQint16;
        manualDelayEnabled[i] = (bool) tempQint16;
        inStream >> tempQint16;
        manualDelay[i] = (int) tempQint16;
    }

    if (!synthMode) {
        if (manualDelayEnabled[0]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortA, manualDelay[0]);
        }
        if (manualDelayEnabled[1]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortB, manualDelay[1]);
        }
        if (manualDelayEnabled[2]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortC, manualDelay[2]);
        }
        if (manualDelayEnabled[3]) {
            evalBoard->setCableDelay(Rhs2000EvalBoard::PortD, manualDelay[3]);
        }
    }

    inStream >> tempQint16;
    postTriggerTime = tempQint16;
    inStream >> tempQint16;
    saveTriggerChannel = (bool) tempQint16;

    inStream >> tempQint16;
    referenceSource.softwareMode = (bool) tempQint16;
    inStream >> tempQint16;
    referenceSource.stream = tempQint16;
    inStream >> tempQint16;
    referenceSource.channel = tempQint16;

    referenceSetChannel();

    inStream >> tempQint16;
    displayMarkerCheckBox->setChecked((bool) tempQint16);

    inStream >> tempQint16;
    displayMarkerSpinBox->setValue(tempQint16 + 1);

    inStream >> tempQint16;
    displayTriggerCheckBox->setChecked((bool) tempQint16);

    inStream >> tempQint16;
    v0AxisLineCheckBox->setChecked((bool) tempQint16);

    inStream >> tempQint16;
    headstageGlobalSettle = (bool) tempQint16;

    inStream >> tempQint16;
    useFastSettle = (bool) tempQint16;

    evalBoard->setAmpSettleMode(useFastSettle);
    evalBoard->setGlobalSettlePolicy(headstageGlobalSettle, headstageGlobalSettle,
                                     headstageGlobalSettle, headstageGlobalSettle, false);

    inStream >> tempQint16;
    chargeRecoveryMode = (bool) tempQint16;

    inStream >> tempQint16;
    chargeRecoveryCurrentLimit = (Rhs2000Registers::ChargeRecoveryCurrentLimit) tempQint16;

    inStream >> chargeRecoveryTargetVoltage;

    setChargeRecoveryParameters(chargeRecoveryMode, chargeRecoveryCurrentLimit, chargeRecoveryTargetVoltage);

    settingsFile.close();

    wavePlot->refreshScreen();
    statusBar()->clearMessage();
    wavePlot->setFocus();
}

// Save application settings to *.isf (Intan Settings File) file.
void MainWindow::saveSettings()
{
    QString saveSettingsFileName;
    saveSettingsFileName = QFileDialog::getSaveFileName(this,
                                            tr("Select Settings Filename"), ".",
                                            tr("Intan Settings Files (*.isf)"));

    if (saveSettingsFileName.isEmpty()) {
        wavePlot->setFocus();
        return;
    }

    QFile settingsFile(saveSettingsFileName);
    if (!settingsFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Cannot Save Settings File"),
                              tr("Cannot open new settings file for writing."));
        wavePlot->setFocus();
        return;
    }

    statusBar()->showMessage("Saving settings to file...");

    // Save settings
    QDataStream outStream(&settingsFile);
    outStream.setVersion(QDataStream::Qt_4_8);
    outStream.setByteOrder(QDataStream::LittleEndian);
    outStream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    outStream << (quint32) SETTINGS_FILE_MAGIC_NUMBER;
    outStream << (qint16) SETTINGS_FILE_MAIN_VERSION_NUMBER;
    outStream << (qint16) SETTINGS_FILE_SECONDARY_VERSION_NUMBER;

    outStream << (qint16) numSpiPorts;

    outStream << *signalSources;

//    outStream << (qint16) sampleRateComboBox->currentIndex();
    outStream << (qint16) yScaleComboBox->currentIndex();
    outStream << (qint16) yScaleDcAmpComboBox->currentIndex();
    outStream << (qint16) yScaleAdcComboBox->currentIndex();
    outStream << (qint16) tScaleComboBox->currentIndex();
    outStream << (qint16) notchFilterComboBox->currentIndex();
    outStream << saveBaseFileName;
    outStream << (qint16) newSaveFilePeriodMinutes;
    outStream << (qint16) dspEnabled;
    outStream << desiredDspCutoffFreq;
    outStream << desiredLowerBandwidth;
    outStream << desiredLowerSettleBandwidth;
    outStream << desiredUpperBandwidth;
    outStream << desiredImpedanceFreq;
    outStream << actualImpedanceFreq;
    outStream << (qint16) impedanceFreqValid;
    outStream << (qint16) dacGainSlider->value();
    outStream << (qint16) dacNoiseSuppressSlider->value();
    for (int i = 0; i < 8; ++i) {
        outStream << (qint16) dacEnabled[i];
        if (dacSelectedChannel[i]) {
            outStream << dacSelectedChannel[i]->nativeChannelName;
        } else {
            outStream << QString("");
        }
    }
    outStream << (qint16) plotPointsCheckBox->isChecked();
    outStream << note1LineEdit->text();
    outStream << note2LineEdit->text();
    outStream << note3LineEdit->text();

    // We need to save the ports in reverse order to make things
    // work out correctly when we load them again.
    for (int port = numSpiPorts + 1; port >= 0; --port) {
        outStream << (qint16) wavePlot->getNumFramesIndex(port);
        outStream << (qint16) wavePlot->getTopLeftFrame(port);
    }

    outStream << (qint16) recordTriggerChannel;
    outStream << (qint16) recordTriggerPolarity;
    outStream << (qint16) recordTriggerBuffer;

    outStream << (qint16) saveFormat;
    outStream << (qint16) dacLockToSelectedBox->isChecked();

    outStream << (qint32) dac1ThresholdSpinBox->value();
    outStream << (qint32) dac2ThresholdSpinBox->value();
    outStream << (qint32) dac3ThresholdSpinBox->value();
    outStream << (qint32) dac4ThresholdSpinBox->value();
    outStream << (qint32) dac5ThresholdSpinBox->value();
    outStream << (qint32) dac6ThresholdSpinBox->value();
    outStream << (qint32) dac7ThresholdSpinBox->value();
    outStream << (qint32) dac8ThresholdSpinBox->value();

    outStream << (qint16) dac1ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac2ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac3ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac4ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac5ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac6ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac7ThresholdEnableCheckBox->isChecked();
    outStream << (qint16) dac8ThresholdEnableCheckBox->isChecked();

    outStream << (qint16) saveTtlOut;
    outStream << (qint16) saveDcAmps;

    outStream << (qint16) highpassFilterEnabled;
    outStream << highpassFilterFrequency;

    for (int i = 0; i < MAX_NUM_SPI_PORTS; ++i) {
        outStream << (qint16) manualDelayEnabled[i];
        outStream << (qint16) manualDelay[i];
    }

    outStream << (qint16) postTriggerTime;
    outStream << (qint16) saveTriggerChannel;

    outStream << (qint16) referenceSource.softwareMode;
    outStream << (qint16) referenceSource.stream;
    outStream << (qint16) referenceSource.channel;

    outStream << (qint16) markerMode();
    outStream << (qint16) markerChannel();
    outStream << (qint16) resetXOnMarker();

    outStream << (qint16) showV0Axis();

    outStream << (qint16) headstageGlobalSettle;
    outStream << (qint16) useFastSettle;

    outStream << (qint16) chargeRecoveryMode;
    outStream << (qint16) chargeRecoveryCurrentLimit;
    outStream << chargeRecoveryTargetVoltage;

    settingsFile.close();

    statusBar()->clearMessage();
    wavePlot->setFocus();
}

// Enable or disable the display of electrode impedances.
void MainWindow::showImpedances(bool enabled)
{
    wavePlot->setImpedanceLabels(enabled);
    wavePlot->setFocus();
}

// Execute an electrode impedance measurement procedure.
void MainWindow::runImpedanceMeasurement()
{
    // We can't really measure impedances in demo mode, so just return.
    if (synthMode) {
        showImpedanceCheckBox->setChecked(true);
        showImpedances(true);
        wavePlot->setFocus();
        return;
    }

    Rhs2000Registers chipRegisters(boardSampleRate, stimStep);
    int commandSequenceLength, stream, channel, capRange;
    double cSeries;
    vector<unsigned int> commandList;
    int triggerIndex;                       // dummy reference variable; not used
    queue<Rhs2000DataBlock> bufferQueue;    // dummy reference variable; not used

    // Disable DACs
    for (int i = 0; i < 8; i++) {
        evalBoard->enableDac(i, false);
    }

    statusBar()->showMessage("Measuring electrode impedances...");

    // Create a progress bar to let user know how long this will take.
    QProgressDialog progress("Measuring Electrode Impedances", "Abort", 0, 50, this);
    progress.setWindowTitle("Progress");
    progress.setMinimumDuration(0);
    progress.setModal(true);
    progress.setValue(0);

    // Create a command list for the AuxCmd1 slot.
    commandSequenceLength =
            chipRegisters.createCommandListZcheckDac(commandList, actualImpedanceFreq, 128.0);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);

    progress.setValue(1);

    // Select number of periods to measure impedance over
    int numPeriods = qRound(0.020 * actualImpedanceFreq); // Test each channel for at least 20 msec...
    if (numPeriods < 5) numPeriods = 5; // ...but always measure across no fewer than 5 complete periods
    double period = boardSampleRate / actualImpedanceFreq;
    int numBlocks = qCeil((numPeriods + 2.0) * period / SAMPLES_PER_DATA_BLOCK);  // + 2 periods to give time to settle initially
    if (numBlocks < 2) numBlocks = 2;   // need first block for command to switch channels to take effect.

    actualDspCutoffFreq = chipRegisters.setDspCutoffFreq(desiredDspCutoffFreq);
    actualLowerBandwidth = chipRegisters.setLowerBandwidth(desiredLowerBandwidth, 0);
    actualUpperBandwidth = chipRegisters.setUpperBandwidth(desiredUpperBandwidth);
    chipRegisters.enableDsp(dspEnabled);
    chipRegisters.enableZcheck(true);
    commandSequenceLength = chipRegisters.createCommandListRegisterConfig(commandList, false);
    // Upload version with no ADC calibration to AuxCmd3 RAM.
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd3);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd3, 0, commandSequenceLength - 1);

    evalBoard->setContinuousRunMode(false);
    evalBoard->setMaxTimeStep(SAMPLES_PER_DATA_BLOCK * numBlocks);

    // Create matrices of doubles of size (numStreams x 16 x 3) to store complex amplitudes
    // of all amplifier channels (16 on each data stream) at three different Cseries values.
    QVector<QVector<QVector<double> > > measuredMagnitude;
    QVector<QVector<QVector<double> > > measuredPhase;
    measuredMagnitude.resize(evalBoard->getNumEnabledDataStreams());
    measuredPhase.resize(evalBoard->getNumEnabledDataStreams());
    for (int i = 0; i < evalBoard->getNumEnabledDataStreams(); ++i) {
        measuredMagnitude[i].resize(16);
        measuredPhase[i].resize(16);
        for (int j = 0; j < 16; ++j) {
            measuredMagnitude[i][j].resize(3);
            measuredPhase[i][j].resize(3);
        }
    }

    // We execute three complete electrode impedance measurements: one each with
    // Cseries set to 0.1 pF, 1 pF, and 10 pF.  Then we select the best measurement
    // for each channel so that we achieve a wide impedance measurement range.
    for (capRange = 0; capRange < 3; ++ capRange) {
        switch (capRange) {
        case 0:
            chipRegisters.setZcheckScale(Rhs2000Registers::ZcheckCs100fF);
            cSeries = 0.1e-12;
            break;
        case 1:
            chipRegisters.setZcheckScale(Rhs2000Registers::ZcheckCs1pF);
            cSeries = 1.0e-12;
            break;
        case 2:
            chipRegisters.setZcheckScale(Rhs2000Registers::ZcheckCs10pF);
            cSeries = 10.0e-12;
            break;
        }

        // Check all 16 channels across all active data streams.
        for (channel = 0; channel < 16; ++channel) {

            progress.setValue(16 * capRange + channel + 2);
            if (progress.wasCanceled()) {
                evalBoard->setContinuousRunMode(false);
                evalBoard->setMaxTimeStep(0);
                evalBoard->flush();
                statusBar()->clearMessage();
                wavePlot->setFocus();
                return;
            }

            chipRegisters.setZcheckChannel(channel);
            commandSequenceLength =
                    chipRegisters.createCommandListRegisterConfig(commandList, false);
            // Upload version with no ADC calibration to AuxCmd3 RAM Bank.
            evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd3);

            evalBoard->run();
            while (evalBoard->isRunning() ) {
                qApp->processEvents();
            }
            evalBoard->readDataBlocks(numBlocks, dataQueue);

            signalProcessor->loadAmplifierData(dataQueue, numBlocks, false, 0, 0, triggerIndex, false, bufferQueue,
                                               false, *saveStream, saveFormat, false, false, 0, ReferenceSource{0, 0, false});
            for (stream = 0; stream < evalBoard->getNumEnabledDataStreams(); ++stream) {
                signalProcessor->measureComplexAmplitude(measuredMagnitude, measuredPhase,
                                                    capRange, stream, channel,  numBlocks, boardSampleRate,
                                                    actualImpedanceFreq, numPeriods);
            }

        }
    }

    SignalChannel *signalChannel;
    double distance, minDistance, current, Cseries;
    double impedanceMagnitude, impedancePhase;

    const double bestAmplitude = 250.0;  // we favor voltage readings that are closest to 250 uV: not too large,
                                         // and not too small.
    const double dacVoltageAmplitude = 128 * (1.225 / 256);  // this assumes the DAC amplitude was set to 128
    const double parasiticCapacitance = 10.0e-12;  // 10 pF: an estimate of on-chip parasitic capacitance,
                                                   // including effective amplifier input capacitance.
    double relativeFreq = actualImpedanceFreq / boardSampleRate;

    int bestAmplitudeIndex;
    for (stream = 0; stream < evalBoard->getNumEnabledDataStreams(); ++stream) {
        for (channel = 0; channel < 16; ++channel) {
            signalChannel = signalSources->findAmplifierChannel(stream, channel);
            if (signalChannel) {
                minDistance = 9.9e99;  // ridiculously large number
                for (capRange = 0; capRange < 3; ++capRange) {
                    // Find the measured amplitude that is closest to bestAmplitude on a logarithmic scale
                    distance = qAbs(qLn(measuredMagnitude[stream][channel][capRange] / bestAmplitude));
                    if (distance < minDistance) {
                        bestAmplitudeIndex = capRange;
                        minDistance = distance;
                    }
                }
                switch (bestAmplitudeIndex) {
                case 0:
                    Cseries = 0.1e-12;
                    break;
                case 1:
                    Cseries = 1.0e-12;
                    break;
                case 2:
                    Cseries = 10.0e-12;
                    break;
                }

                // Calculate current amplitude produced by on-chip voltage DAC
                current = TWO_PI * actualImpedanceFreq * dacVoltageAmplitude * Cseries;

                // Calculate impedance magnitude from calculated current and measured voltage.
                impedanceMagnitude = 1.0e-6 * (measuredMagnitude[stream][channel][bestAmplitudeIndex] / current) *
                        (18.0 * relativeFreq * relativeFreq + 1.0);

                // Calculate impedance phase, with small correction factor accounting for the
                // 3-command SPI pipeline delay.
                impedancePhase = measuredPhase[stream][channel][bestAmplitudeIndex] + (360.0 * (3.0 / period));

                // Factor out on-chip parasitic capacitance from impedance measurement.
                factorOutParallelCapacitance(impedanceMagnitude, impedancePhase, actualImpedanceFreq,
                                             parasiticCapacitance);

                // Perform empirical resistance correction to improve accuarcy at sample rates below
                // 15 kS/s.
                empiricalResistanceCorrection(impedanceMagnitude, impedancePhase,
                                              boardSampleRate);

                signalChannel->electrodeImpedanceMagnitude = impedanceMagnitude;
                signalChannel->electrodeImpedancePhase = impedancePhase;
            }
        }
    }

    evalBoard->setContinuousRunMode(false);
    evalBoard->setMaxTimeStep(0);
    evalBoard->flush();

    // Switch back to flatline
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, SAMPLES_PER_DATA_BLOCK - 1);

    progress.setValue(progress.maximum());

    // Re-enable DACs
    for (int i = 0; i < 8; i++) {
        evalBoard->enableDac(i, dacEnabled[i]);
    }

    saveImpedancesButton->setEnabled(true);
    statusBar()->clearMessage();
    showImpedanceCheckBox->setChecked(true);
    showImpedances(true);
    wavePlot->setFocus();
}

// Given a measured complex impedance that is the result of an electrode impedance in parallel
// with a parasitic capacitance (i.e., due to the amplifier input capacitance and other
// capacitances associated with the chip bondpads), this function factors out the effect of the
// parasitic capacitance to return the acutal electrode impedance.
void MainWindow::factorOutParallelCapacitance(double &impedanceMagnitude, double &impedancePhase,
                                              double frequency, double parasiticCapacitance)
{
    // First, convert from polar coordinates to rectangular coordinates.
    double measuredR = impedanceMagnitude * qCos(DEGREES_TO_RADIANS * impedancePhase);
    double measuredX = impedanceMagnitude * qSin(DEGREES_TO_RADIANS * impedancePhase);

    double capTerm = TWO_PI * frequency * parasiticCapacitance;
    double xTerm = capTerm * (measuredR * measuredR + measuredX * measuredX);
    double denominator = capTerm * xTerm + 2 * capTerm * measuredX + 1;
    double trueR = measuredR / denominator;
    double trueX = (measuredX + xTerm) / denominator;

    // Now, convert from rectangular coordinates back to polar coordinates.
    impedanceMagnitude = qSqrt(trueR * trueR + trueX * trueX);
    impedancePhase = RADIANS_TO_DEGREES * qAtan2(trueX, trueR);
}

// This is a purely empirical function to correct observed errors in the real component
// of measured electrode impedances at sampling rates below 15 kS/s.  At low sampling rates,
// it is difficult to approximate a smooth sine wave with the on-chip voltage DAC and 10 kHz
// 2-pole lowpass filter.  This function attempts to somewhat correct for this, but a better
// solution is to always run impedance measurements at 20 kS/s, where they seem to be most
// accurate.
void MainWindow::empiricalResistanceCorrection(double &impedanceMagnitude, double &impedancePhase,
                                               double boardSampleRate)
{
    // First, convert from polar coordinates to rectangular coordinates.
    double impedanceR = impedanceMagnitude * qCos(DEGREES_TO_RADIANS * impedancePhase);
    double impedanceX = impedanceMagnitude * qSin(DEGREES_TO_RADIANS * impedancePhase);

    // Emprically derived correction factor (i.e., no physical basis for this equation).
    impedanceR /= 10.0 * qExp(-boardSampleRate / 2500.0) * qCos(TWO_PI * boardSampleRate / 15000.0) + 1.0;

    // Now, convert from rectangular coordinates back to polar coordinates.
    impedanceMagnitude = qSqrt(impedanceR * impedanceR + impedanceX * impedanceX);
    impedancePhase = RADIANS_TO_DEGREES * qAtan2(impedanceX, impedanceR);
}

// Save measured electrode impedances in CSV (Comma Separated Values) text file.
void MainWindow::saveImpedances()
{
    double equivalentR, equivalentC;

    QString csvFileName;
    csvFileName = QFileDialog::getSaveFileName(this,
                                            tr("Save Impedance Data As"), ".",
                                            tr("CSV (Comma delimited) (*.csv)"));

    if (!csvFileName.isEmpty()) {
        QFile csvFile(csvFileName);

        if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            cerr << "Cannot open CSV file for writing: " <<
                    qPrintable(csvFile.errorString()) << endl;
        }
        QTextStream out(&csvFile);

        out << "Channel Number,Channel Name,Port,Enabled,";
        out << "Impedance Magnitude at " << actualImpedanceFreq << " Hz (ohms),";
        out << "Impedance Phase at " << actualImpedanceFreq << " Hz (degrees),";
        out << "Series RC equivalent R (Ohms),";
        out << "Series RC equivalent C (Farads)" << endl;

        SignalChannel *signalChannel;
        for (int stream = 0; stream < evalBoard->getNumEnabledDataStreams(); ++stream) {
            for (int channel = 0; channel < 32; ++channel) {
                signalChannel = signalSources->findAmplifierChannel(stream, channel);
                if (signalChannel != nullptr) {
                    equivalentR = signalChannel->electrodeImpedanceMagnitude *
                        qCos(DEGREES_TO_RADIANS * signalChannel->electrodeImpedancePhase);
                    equivalentC = 1.0 / (TWO_PI * actualImpedanceFreq * signalChannel->electrodeImpedanceMagnitude *
                        -1.0 * qSin(DEGREES_TO_RADIANS * signalChannel->electrodeImpedancePhase));

                    out.setRealNumberNotation(QTextStream::ScientificNotation);
                    out.setRealNumberPrecision(2);

                    out << signalChannel->nativeChannelName << ",";
                    out << signalChannel->customChannelName << ",";
                    out << signalChannel->signalGroup->name << ",";
                    out << signalChannel->enabled << ",";
                    out << signalChannel->electrodeImpedanceMagnitude << ",";

                    out.setRealNumberNotation(QTextStream::FixedNotation);
                    out.setRealNumberPrecision(0);

                    out << signalChannel->electrodeImpedancePhase << ",";

                    out.setRealNumberNotation(QTextStream::ScientificNotation);
                    out.setRealNumberPrecision(2);

                    out << equivalentR << ",";
                    out << equivalentC << endl;
                }
            }
        }

        csvFile.close();
    }
    wavePlot->setFocus();
}

void MainWindow::plotPointsMode(bool enabled)
{
    wavePlot->setPointPlotMode(enabled);
    wavePlot->setFocus();
}

void MainWindow::setStatusBarReady()
{
    if (!synthMode) {
        statusBarLabel->setText("Ready.");
    } else {
        statusBarLabel->setText("No Intan Stimulation / Recording Controller hardware connected.  Ready to run with synthesized data.");
    }
}

void MainWindow::setStatusBarRunning()
{
    if (!synthMode) {
        statusBarLabel->setText("Running.");
    } else {
        statusBarLabel->setText("Running with synthesized data.");
    }
}

void MainWindow::setStatusBarRecording(double bytesPerMinute, double totalElapsedRecordTimeSeconds)
{
    QTime recordTime(0, 0, 0, 0);
    QString timeString = recordTime.addSecs((int)totalElapsedRecordTimeSeconds).toString("hh:mm:ss");

    if (!synthMode) {
        statusBarLabel->setText("<b>" + timeString + "</b>  Saving data to file " + saveFileName + ".  (" +
                                QString::number(bytesPerMinute / (1024.0 * 1024.0), 'f', 1) +
                                " MB/minute.  File size may be reduced by disabling unused inputs.)");
    } else {
        statusBarLabel->setText("<b>" + timeString + "</b>  Saving synthesized data to file " + saveFileName + ".  (" +
                                QString::number(bytesPerMinute / (1024.0 * 1024.0), 'f', 1) +
                                " MB/minute.  File size may be reduced by disabling unused inputs.)");
    }
}

void MainWindow::setStatusBarWaitForTrigger()
{
    if (recordTriggerPolarity == 0) {
        statusBarLabel->setText("Waiting for logic high trigger on digital input " +
                                QString::number(recordTriggerChannel + 1) + "...");
    } else {
        statusBarLabel->setText("Waiting for logic low trigger on digital input " +
                                QString::number(recordTriggerChannel + 1) + "...");
    }
}

// Set the format of the saved data file.
void MainWindow::setSaveFormat(SaveFormat format)
{
    saveFormat = format;
}

// Create and open a new save file for data (saveFile), and create a new
// data stream (saveStream) for writing to the file.
void MainWindow::startNewSaveFile(SaveFormat format)
{
    QFileInfo fileInfo(saveBaseFileName);
    QDateTime dateTime = QDateTime::currentDateTime();

    if (format == SaveFormatIntan) {
        // Add time and date stamp to base filename.
        saveFileName = fileInfo.path();
        saveFileName += "/";
        saveFileName += fileInfo.baseName();
        saveFileName += "_";
        saveFileName += dateTime.toString("yyMMdd");    // date stamp
        saveFileName += "_";
        saveFileName += dateTime.toString("HHmmss");    // time stamp
        saveFileName += ".rhs";

        saveFile = new QFile(saveFileName);

        if (!saveFile->open(QIODevice::WriteOnly)) {
            cerr << "Cannot open file for writing: " <<
                    qPrintable(saveFile->errorString()) << endl;
        }

        saveStream = new QDataStream(saveFile);
        saveStream->setVersion(QDataStream::Qt_4_8);

        // Set to little endian mode for compatibilty with MATLAB,
        // which is little endian on all platforms
        saveStream->setByteOrder(QDataStream::LittleEndian);

        // Write 4-byte floating-point numbers (instead of the default 8-byte numbers)
        // to save disk space.
        saveStream->setFloatingPointPrecision(QDataStream::SinglePrecision);

    } else if (format == SaveFormatFilePerSignalType) {
        // Create 'save file' name for status bar display.
        saveFileName = fileInfo.path();
        saveFileName += "/";
        saveFileName += fileInfo.baseName();
        saveFileName += "_";
        saveFileName += dateTime.toString("yyMMdd");    // date stamp
        saveFileName += "_";
        saveFileName += dateTime.toString("HHmmss");    // time stamp

        // Create subdirectory for data, timestamp, and info files.
        QString subdirName;
        subdirName = fileInfo.baseName();
        subdirName += "_";
        subdirName += dateTime.toString("yyMMdd");    // date stamp
        subdirName += "_";
        subdirName += dateTime.toString("HHmmss");    // time stamp

        QDir dir(fileInfo.path());
        dir.mkdir(subdirName);

        QDir subdir(fileInfo.path() + "/" + subdirName);

        infoFileName = subdir.path() + "/" + "info.rhs";

        signalProcessor->createTimestampFilename(subdir.path());
        signalProcessor->openTimestampFile();

        signalProcessor->createSignalTypeFilenames(subdir.path());
        signalProcessor->openSignalTypeFiles(saveTtlOut, saveDcAmps);

        infoFile = new QFile(infoFileName);

        if (!infoFile->open(QIODevice::WriteOnly)) {
            cerr << "Cannot open file for writing: " <<
                    qPrintable(infoFile->errorString()) << endl;
        }

        infoStream = new QDataStream(infoFile);
        infoStream->setVersion(QDataStream::Qt_4_8);

        // Set to little endian mode for compatibilty with MATLAB,
        // which is little endian on all platforms
        infoStream->setByteOrder(QDataStream::LittleEndian);

        // Write 4-byte floating-point numbers (instead of the default 8-byte numbers)
        // to save disk space.
        infoStream->setFloatingPointPrecision(QDataStream::SinglePrecision);

    } else if (format == SaveFormatFilePerChannel) {
        // Create 'save file' name for status bar display.
        saveFileName = fileInfo.path();
        saveFileName += "/";
        saveFileName += fileInfo.baseName();
        saveFileName += "_";
        saveFileName += dateTime.toString("yyMMdd");    // date stamp
        saveFileName += "_";
        saveFileName += dateTime.toString("HHmmss");    // time stamp

        // Create subdirectory for individual channel data files.
        QString subdirName;
        subdirName = fileInfo.baseName();
        subdirName += "_";
        subdirName += dateTime.toString("yyMMdd");    // date stamp
        subdirName += "_";
        subdirName += dateTime.toString("HHmmss");    // time stamp

        QDir dir(fileInfo.path());
        dir.mkdir(subdirName);

        QDir subdir(fileInfo.path() + "/" + subdirName);

        // Create filename for each channel.
        signalProcessor->createTimestampFilename(subdir.path());
        signalProcessor->createFilenames(signalSources, subdir.path());

        // Open save files and output streams.
        signalProcessor->openTimestampFile();
        signalProcessor->openSaveFiles(signalSources, saveDcAmps);

        // Create info file.
        infoFileName = subdir.path() + "/" + "info.rhs";

        infoFile = new QFile(infoFileName);

        if (!infoFile->open(QIODevice::WriteOnly)) {
            cerr << "Cannot open file for writing: " <<
                    qPrintable(infoFile->errorString()) << endl;
        }

        infoStream = new QDataStream(infoFile);
        infoStream->setVersion(QDataStream::Qt_4_8);

        // Set to little endian mode for compatibilty with MATLAB,
        // which is little endian on all platforms
        infoStream->setByteOrder(QDataStream::LittleEndian);

        // Write 4-byte floating-point numbers (instead of the default 8-byte numbers)
        // to save disk space.
        infoStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
}

void MainWindow::closeSaveFile(SaveFormat format) {
    switch (format) {
    case SaveFormatIntan:
        saveFile->close();
        delete saveStream;
        delete saveFile;
        break;

    case SaveFormatFilePerSignalType:
        signalProcessor->closeTimestampFile();
        signalProcessor->closeSignalTypeFiles();
        infoFile->close();
        delete infoStream;
        delete infoFile;
        break;

    case SaveFormatFilePerChannel:
        signalProcessor->closeTimestampFile();
        signalProcessor->closeSaveFiles(signalSources, saveDcAmps);
        infoFile->close();
        delete infoStream;
        delete infoFile;
        break;
    }
}

// Launch save file format selection dialog.
void MainWindow::setSaveFormatDialog()
{
    SetSaveFormatDialog saveFormatDialog(saveFormat, saveTtlOut, saveDcAmps, newSaveFilePeriodMinutes, this);

    if (saveFormatDialog.exec()) {
        saveFormat = (SaveFormat) saveFormatDialog.buttonGroup->checkedId();
        saveTtlOut = (saveFormatDialog.saveTtlOutCheckBox->checkState() == Qt::Checked);
        saveDcAmps = (saveFormatDialog.saveDcAmpsCheckBox->checkState() == Qt::Checked);
        newSaveFilePeriodMinutes = saveFormatDialog.recordTimeSpinBox->value();

        setSaveFormat(saveFormat);
    }
    wavePlot->setFocus();
}

void MainWindow::setDacThreshold1(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(0, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold2(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(1, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold3(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(2, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold4(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(3, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold5(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(4, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold6(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(5, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold7(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(6, threshLevel, threshold >= 0);
}

void MainWindow::setDacThreshold8(int threshold)
{
    int threshLevel = qRound((double) threshold / 0.195) + 32768;
    if (!synthMode) evalBoard->setDacThreshold(7, threshLevel, threshold >= 0);
}

void MainWindow::dacThresholdEnable()
{
    if (!synthMode) {
        evalBoard->setTtlOutMode(dac1ThresholdEnableCheckBox->isChecked(), dac2ThresholdEnableCheckBox->isChecked(),
                                 dac3ThresholdEnableCheckBox->isChecked(), dac4ThresholdEnableCheckBox->isChecked(),
                                 dac5ThresholdEnableCheckBox->isChecked(), dac6ThresholdEnableCheckBox->isChecked(),
                                 dac7ThresholdEnableCheckBox->isChecked(), dac8ThresholdEnableCheckBox->isChecked());
    }
}

// start update GUI - SB - 05-04-2019
void MainWindow::custom_architecture_Enable()
{
    if (!synthMode) {
        evalBoard->custom_architecture_Enable(cst_en_CheckBox->isChecked());
    }
}

void MainWindow::custom_architecture_trig_Enable()
{
    if (!synthMode) {
        evalBoard->custom_architecture_trig_Enable(cst_trig_CheckBox->isChecked());
    }
}

// stop update GUI - SB - 05-04-2019

int MainWindow::getEvalBoardMode()
{
    return evalBoardMode;
}

// Launch manual cable delay configuration dialog.
void MainWindow::manualCableDelayControl()
{
    runButton->setEnabled(false);
    recordButton->setEnabled(false);
    triggerButton->setEnabled(false);

    vector<int> currentDelays;
    currentDelays.resize(MAX_NUM_SPI_PORTS, 0);
    if (!synthMode) {
        evalBoard->getCableDelay(currentDelays);
    }

    CableDelayDialog manualCableDelayDialog(manualDelayEnabled, currentDelays, this);
    if (manualCableDelayDialog.exec()) {
        manualDelayEnabled[0] = manualCableDelayDialog.manualPortACheckBox->isChecked();
        manualDelayEnabled[1] = manualCableDelayDialog.manualPortBCheckBox->isChecked();
        manualDelayEnabled[2] = manualCableDelayDialog.manualPortCCheckBox->isChecked();
        manualDelayEnabled[3] = manualCableDelayDialog.manualPortDCheckBox->isChecked();
        if (manualDelayEnabled[0]) {
            manualDelay[0] = manualCableDelayDialog.delayPortASpinBox->value();
            if (!synthMode) {
                evalBoard->setCableDelay(Rhs2000EvalBoard::PortA, manualDelay[0]);
            }
        }
        if (manualDelayEnabled[1]) {
            manualDelay[1] = manualCableDelayDialog.delayPortBSpinBox->value();
            if (!synthMode) {
                evalBoard->setCableDelay(Rhs2000EvalBoard::PortB, manualDelay[1]);
            }
        }
        if (manualDelayEnabled[2]) {
            manualDelay[2] = manualCableDelayDialog.delayPortCSpinBox->value();
            if (!synthMode) {
                evalBoard->setCableDelay(Rhs2000EvalBoard::PortC, manualDelay[2]);
            }
            }
        if (manualDelayEnabled[3]) {
            manualDelay[3] = manualCableDelayDialog.delayPortDSpinBox->value();
            if (!synthMode) {
                evalBoard->setCableDelay(Rhs2000EvalBoard::PortD, manualDelay[3]);
            }
        }
    }
    scanPorts();

    runButton->setEnabled(true);
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);

    wavePlot->setFocus();
}

bool MainWindow::isRecording()
{
    return recording;
}

// Select current channels as software reference
void MainWindow::referenceSetSelectedChannel()
{
    SignalChannel *selectedChannel = wavePlot->selectedChannel();
    if (selectedChannel->signalType == AmplifierSignal) {
        referenceSource.channel = selectedChannel->chipChannel;
        referenceSource.stream = selectedChannel->boardStream;
        referenceSource.softwareMode = true;
        referenceSetChannel();
    }
}

// Select hardware reference mode (disable software re-referencing)
void MainWindow::referenceSetHardware()
{
    referenceSource.softwareMode = false;
    referenceSetChannel();
}

void MainWindow::referenceSetChannel()
{
    if (referenceSource.softwareMode) {
        if (!synthMode) {
            evalBoard->setDacRerefSource(referenceSource.stream, referenceSource.channel);
            evalBoard->enableDacReref(true);
        }
        refTypeLabel->setText(tr("Digital Reference:"));
        refChannelLabel->setText("Channel " + signalSources->findAmplifierChannel(referenceSource.stream, referenceSource.channel)->customChannelName);
        refHardwareRefButton->setEnabled(true);
    } else {
        if (!synthMode) {
            evalBoard->enableDacReref(false);
        }
        refTypeLabel->setText(tr("Hardware Reference:"));
        refChannelLabel->setText(tr("REF input on headstages"));
        refHardwareRefButton->setEnabled(false);
    }
    wavePlot->setFocus();
}

void MainWindow::referenceHelp()
{
    if (!helpDialogReference) {
        helpDialogReference = new HelpDialogReference(this);
    }
    helpDialogReference->show();
    helpDialogReference->raise();
    helpDialogReference->activateWindow();
    wavePlot->setFocus();
}

ReferenceSource MainWindow::getReferenceSource()
{
    return referenceSource;
}

bool MainWindow::markerMode()
{
    return displayMarkerCheckBox->isChecked();
}

bool MainWindow::resetXOnMarker()
{
    return displayTriggerCheckBox->isChecked();
}

int MainWindow::markerChannel()
{
    return displayMarkerSpinBox->value() - 1;
}

bool MainWindow::showV0Axis()
{
    return v0AxisLineCheckBox->isChecked();
}

void MainWindow::setManualStimTrigger(int trigger, bool triggerOn)
{
    if (!synthMode) {
        evalBoard->setManualStimTrigger(trigger, triggerOn);
    }
}

void MainWindow::stimParam()
{
    SignalChannel* selectedChannel = wavePlot->selectedChannel();
    StimParameters* parameters = selectedChannel->stimParameters;
    double timestep_us = 1.0e6 / boardSampleRate;
    double currentstep_uA = Rhs2000Registers::stimStepSizeToDouble(stimStep) / 1.0e-6;

    if (displayDigOutButton->isChecked()) {  // Digital out channel
        // if DigOutDialog object already exists, disconnect it and destroy it first so a new object is created
        // for the new parameters.
        if (digOutDialog) {
            disconnect(this, 0, digOutDialog, 0); // disconnect all signals and slots between MainWindow and DigOutDialog
            delete digOutDialog;
            digOutDialog = nullptr;
        }

        digOutDialog = new DigOutDialog(parameters, selectedChannel->nativeChannelName, selectedChannel->customChannelName, timestep_us, this);
        connect(this, SIGNAL(focusChanged(QWidget*,QWidget*)), digOutDialog, SLOT(notifyFocusChanged(QWidget*,QWidget*)));

        if (digOutDialog->exec() == QDialog::Accepted)
        {
            stimParamsHaveChanged = true;
            setDigitalOutSequenceParameters(evalBoard, timestep_us, selectedChannel->nativeChannelNumber, parameters);
        }
    } else if (displayDacButton->isChecked()) {
        // if AnOutDialog object already exists, disconnect it and destroy it first so a new object is created
        // for the new parameters.
        if (anOutDialog) {
            disconnect(this, 0, anOutDialog, 0); // disconnect all signals and slots between MainWindow and AnOutDialog
            delete anOutDialog;
            anOutDialog = nullptr;
        }

        anOutDialog = new AnOutDialog(parameters, selectedChannel->nativeChannelName, selectedChannel->customChannelName, timestep_us, this);
        connect(this, SIGNAL(focusChanged(QWidget*,QWidget*)), anOutDialog, SLOT(notifyFocusChanged(QWidget*,QWidget*)));

        if (anOutDialog->exec() == QDialog::Accepted)
        {
            stimParamsHaveChanged = true;
            setAnalogOutSequenceParameters(evalBoard, timestep_us, selectedChannel->nativeChannelNumber, parameters);
        }
    } else {    // Amplifier channel
        // if StimParamDialog object already exists, disconnect it and destroy it first so a new object is created
        // for the new parameters.
        if (stimParamDialog) {
            disconnect(this, 0, stimParamDialog, 0); // disconnect all signals and slots between MainWindow and StimParamDialog
            delete stimParamDialog;
            stimParamDialog = nullptr;
        }
        stimParamDialog = new StimParamDialog(parameters, selectedChannel->nativeChannelName, selectedChannel->customChannelName,
                                              timestep_us, currentstep_uA, this);
        connect(this, SIGNAL(focusChanged(QWidget*,QWidget*)), stimParamDialog, SLOT(notifyFocusChanged(QWidget*,QWidget*)));

        if (stimParamDialog->exec() == QDialog::Accepted) {
            stimParamsHaveChanged = true;
            setStimSequenceParameters(evalBoard, timestep_us, currentstep_uA, selectedChannel->commandStream,
                                      selectedChannel->chipChannel, parameters);
        }
    }

    wavePlot->refreshScreen();
    wavePlot->setFocus();
}

void MainWindow::setDigitalOutSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, int channel, StimParameters *parameters)
{
    if (synthMode) return;

    int stream = 16;
    const int NEVER = 65535;

    int numOfPulses = (parameters->pulseOrTrain == StimParameters::SinglePulse) ? 1 : parameters->numberOfStimPulses;

    evalBoard->configureStimTrigger(stream, channel, (int) parameters->triggerSource,
                                    parameters->enabled,
                                    (parameters->triggerEdgeOrLevel == StimParameters::Edge),
                                    (parameters->triggerHighOrLow == StimParameters::Low));
    evalBoard->configureStimPulses(stream, channel, numOfPulses, (Rhs2000EvalBoard::StimShape)(parameters->stimShape),
                                   (parameters->stimPolarity == StimParameters::NegativeFirst));

    int postTriggerDelay = (int)(parameters->postTriggerDelay / timestep_us + 0.5);
    int firstPhaseDuration = (int)(parameters->firstPhaseDuration / timestep_us + 0.5);
    int refractoryPeriod = (int)(parameters->refractoryPeriod / timestep_us + 0.5);
    int pulseTrainPeriod = (int)(parameters->pulseTrainPeriod / timestep_us + 0.5);

    int eventStartStim = postTriggerDelay;
    int eventEndStim = eventStartStim + firstPhaseDuration;
    int eventEnd = eventEndStim + refractoryPeriod;
    int eventRepeatStim;

    if (parameters->pulseOrTrain == StimParameters::PulseTrain) {
        eventRepeatStim = eventStartStim + pulseTrainPeriod;
    } else {
        eventRepeatStim = NEVER;
    }

    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventStartStim, eventStartStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventEndStim, eventEndStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventRepeatStim, eventRepeatStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventEnd, eventEnd);
}

void MainWindow::setAnalogOutSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, int channel, StimParameters *parameters)
{
    if (synthMode) return;

    int stream = 8 + channel;
    const int NEVER = 65535;

    int numOfPulses = (parameters->pulseOrTrain == StimParameters::SinglePulse) ? 1 : parameters->numberOfStimPulses;

    evalBoard->configureStimTrigger(stream, 0, (int) parameters->triggerSource,
                                    parameters->enabled,
                                    (parameters->triggerEdgeOrLevel == StimParameters::Edge),
                                    (parameters->triggerHighOrLow == StimParameters::Low));
    evalBoard->configureStimPulses(stream, 0, numOfPulses, (Rhs2000EvalBoard::StimShape)(parameters->stimShape),
                                   (parameters->stimPolarity == StimParameters::NegativeFirst));

    int postTriggerDelay = (int)(parameters->postTriggerDelay / timestep_us + 0.5);
    int firstPhaseDuration = (int)(parameters->firstPhaseDuration / timestep_us + 0.5);
    int secondPhaseDuration = (int)(parameters->secondPhaseDuration / timestep_us + 0.5);
    int interphaseDelay = (int)(parameters->interphaseDelay / timestep_us + 0.5);
    int refractoryPeriod = (int)(parameters->refractoryPeriod / timestep_us + 0.5);
    int pulseTrainPeriod = (int)(parameters->pulseTrainPeriod / timestep_us + 0.5);

    int eventStartStim;
    int eventStimPhase2;
    int eventStimPhase3;
    int eventEndStim;
    int eventEnd;
    int eventRepeatStim;

    switch (parameters->stimShape) {
    case StimParameters::Biphasic:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = NEVER;
        eventEndStim = eventStimPhase2 + secondPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    case StimParameters::BiphasicWithInterphaseDelay:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = eventStimPhase2 + interphaseDelay;
        eventEndStim = eventStimPhase3 + secondPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    case StimParameters::Triphasic:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = eventStimPhase2 + secondPhaseDuration;
        eventEndStim = eventStimPhase3 + firstPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    case StimParameters::Monophasic:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = NEVER;
        eventStimPhase3 = NEVER;
        eventEndStim = eventStartStim + firstPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    }

    if (parameters->pulseOrTrain == StimParameters::PulseTrain) {
        eventRepeatStim = eventStartStim + pulseTrainPeriod;
    } else {
        eventRepeatStim = NEVER;
    }

    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventStartStim, eventStartStim);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventStimPhase2, eventStimPhase2);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventStimPhase3, eventStimPhase3);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventEndStim, eventEndStim);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventRepeatStim, eventRepeatStim);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::EventEnd, eventEnd);

    int dacBaseline, dacPositive, dacNegative;
    const double dacLsb = (2 * 10.24) / 65536;
    const int dacMid = 32768;
    dacBaseline = dacMid + (int)(parameters->baselineVoltage / dacLsb);

    if (parameters->stimShape == StimParameters::Monophasic) {
        if (parameters->stimPolarity == StimParameters::NegativeFirst) {
            dacPositive = dacBaseline;
            dacNegative = dacBaseline + (int)(-1.0 * parameters->firstPhaseAmplitude / dacLsb);
        } else {
            dacPositive = dacBaseline + (int)(parameters->firstPhaseAmplitude / dacLsb);
            dacNegative = dacBaseline;
        }
    } else {
        dacPositive = dacBaseline + (int)((parameters->stimPolarity == StimParameters::NegativeFirst ?
                                               parameters->secondPhaseAmplitude : parameters->firstPhaseAmplitude) / dacLsb);
        dacNegative = dacBaseline + (int)(-1.0 * (parameters->stimPolarity == StimParameters::NegativeFirst ?
                                                      parameters->firstPhaseAmplitude : parameters->secondPhaseAmplitude) / dacLsb);
    }

    if (dacBaseline < 0) dacBaseline = 0;
    if (dacPositive < 0) dacPositive = 0;
    if (dacNegative < 0) dacNegative = 0;
    if (dacBaseline > 65535) dacBaseline = 65535;
    if (dacPositive > 65535) dacPositive = 65535;
    if (dacNegative > 65535) dacNegative = 65535;

    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::DacBaseline, dacBaseline);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::DacPositive, dacPositive);
    evalBoard->programStimReg(stream, 0, Rhs2000EvalBoard::DacNegative, dacNegative);
}

void MainWindow::setStimSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, double currentstep_uA, int stream, int channel, StimParameters *parameters)
{
    if (synthMode) return;

    const int NEVER = 65535;

    int numOfPulses = (parameters->pulseOrTrain == StimParameters::SinglePulse) ? 1 : parameters->numberOfStimPulses;

    evalBoard->configureStimTrigger(stream, channel, (int) parameters->triggerSource,
                                    parameters->enabled,
                                    (parameters->triggerEdgeOrLevel == StimParameters::Edge),
                                    (parameters->triggerHighOrLow == StimParameters::Low));
    evalBoard->configureStimPulses(stream, channel, numOfPulses, (Rhs2000EvalBoard::StimShape)(parameters->stimShape),
                                   (parameters->stimPolarity == StimParameters::NegativeFirst));

    int preStimAmpSettle = (int)(parameters->preStimAmpSettle / timestep_us + 0.5);
    int postStimAmpSettle = (int)(parameters->postStimAmpSettle / timestep_us + 0.5);
    int postTriggerDelay = (int)(parameters->postTriggerDelay / timestep_us + 0.5);
    int firstPhaseDuration = (int)(parameters->firstPhaseDuration / timestep_us + 0.5);
    int secondPhaseDuration = (int)(parameters->secondPhaseDuration / timestep_us + 0.5);
    int interphaseDelay = (int)(parameters->interphaseDelay / timestep_us + 0.5);
    int refractoryPeriod = (int)(parameters->refractoryPeriod / timestep_us + 0.5);
    int postStimChargeRecovOn = (int)(parameters->postStimChargeRecovOn / timestep_us + 0.5);
    int postStimChargeRecovOff = (int)(parameters->postStimChargeRecovOff / timestep_us + 0.5);
    int pulseTrainPeriod = (int)(parameters->pulseTrainPeriod / timestep_us + 0.5);

    int eventStartStim;
    int eventStimPhase2;
    int eventStimPhase3;
    int eventEndStim;
    int eventEnd;
    int eventRepeatStim;
    int eventAmpSettleOn;
    int eventAmpSettleOff;
    int eventAmpSettleOnRepeat;
    int eventAmpSettleOffRepeat;
    int eventChargeRecovOn;
    int eventChargeRecovOff;

    switch (parameters->stimShape) {
    case StimParameters::Biphasic:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = NEVER;
        eventEndStim = eventStimPhase2 + secondPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    case StimParameters::BiphasicWithInterphaseDelay:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = eventStimPhase2 + interphaseDelay;
        eventEndStim = eventStimPhase3 + secondPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    case StimParameters::Triphasic:
        eventStartStim = postTriggerDelay;
        eventStimPhase2 = eventStartStim + firstPhaseDuration;
        eventStimPhase3 = eventStimPhase2 + secondPhaseDuration;
        eventEndStim = eventStimPhase3 + firstPhaseDuration;
        eventEnd = eventEndStim + refractoryPeriod;
        break;
    }

    if (parameters->pulseOrTrain == StimParameters::PulseTrain) {
        eventRepeatStim = eventStartStim + pulseTrainPeriod;
    } else {
        eventRepeatStim = NEVER;
    }

    if (parameters->enableAmpSettle) {
        eventAmpSettleOn = eventStartStim - preStimAmpSettle;
        eventAmpSettleOff = eventEndStim + postStimAmpSettle;
        if (parameters->maintainAmpSettle) {
            eventAmpSettleOnRepeat = NEVER;
            eventAmpSettleOffRepeat = NEVER;
        } else {
            eventAmpSettleOnRepeat = eventRepeatStim - preStimAmpSettle;
            eventAmpSettleOffRepeat = postStimAmpSettle;
        }
    } else {
        eventAmpSettleOn = NEVER;
        eventAmpSettleOff = 0;
        eventAmpSettleOnRepeat = NEVER;
        eventAmpSettleOffRepeat = NEVER;
    }

    if (parameters->enableChargeRecovery) {
        eventChargeRecovOn = eventEndStim + postStimChargeRecovOn;
        eventChargeRecovOff = eventEndStim + postStimChargeRecovOff;
    } else {
        eventChargeRecovOn = NEVER;
        eventChargeRecovOff = 0;
    }

    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventAmpSettleOn, eventAmpSettleOn);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventStartStim, eventStartStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventStimPhase2, eventStimPhase2);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventStimPhase3, eventStimPhase3);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventEndStim, eventEndStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventRepeatStim, eventRepeatStim);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventAmpSettleOff, eventAmpSettleOff);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventChargeRecovOn, eventChargeRecovOn);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventChargeRecovOff, eventChargeRecovOff);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventAmpSettleOnRepeat, eventAmpSettleOnRepeat);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventAmpSettleOffRepeat, eventAmpSettleOffRepeat);
    evalBoard->programStimReg(stream, channel, Rhs2000EvalBoard::EventEnd, eventEnd);

    evalBoard->enableAuxCommandsOnOneStream(stream);

    Rhs2000Registers chipRegisters(boardSampleRate, stimStep);
    int commandSequenceLength;
    vector<unsigned int> commandList;

    int firstPhaseAmplitude = (int)(parameters->firstPhaseAmplitude / currentstep_uA + 0.5);
    int secondPhaseAmplitude = (int)(parameters->secondPhaseAmplitude / currentstep_uA + 0.5);
    int posMag = (parameters->stimPolarity == StimParameters::PositiveFirst) ? firstPhaseAmplitude : secondPhaseAmplitude;
    int negMag = (parameters->stimPolarity == StimParameters::NegativeFirst) ? firstPhaseAmplitude : secondPhaseAmplitude;

    commandSequenceLength = chipRegisters.createCommandListSetStimMagnitudes(commandList, channel, posMag, 0, negMag, 0);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);

    chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 255));
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd2);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd3);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd4);

    evalBoard->setMaxTimeStep(commandSequenceLength);
    evalBoard->setContinuousRunMode(false);
    evalBoard->setStimCmdMode(false);
    evalBoard->enableAuxCommandsOnOneStream(stream);

    evalBoard->run();
    while (evalBoard->isRunning() ) {
        qApp->processEvents();
    }

    commandSequenceLength = chipRegisters.createCommandListRegisterRead(commandList);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);
    evalBoard->run();
    while (evalBoard->isRunning() ) {
        qApp->processEvents();
    }

    Rhs2000DataBlock* dataBlock = new Rhs2000DataBlock(evalBoard->getNumEnabledDataStreams());;
    evalBoard->readDataBlock(dataBlock);
    evalBoard->readDataBlock(dataBlock);

    // chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 255));
    // evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);

    commandSequenceLength = chipRegisters.createCommandListRegisterConfig(commandList, true);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);

    evalBoard->enableAuxCommandsOnAllStreams();
}

void MainWindow::notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus)
{
    emit focusChanged(lostFocus, gainedFocus);
}

QString MainWindow::sampleRateText(int index)
{
    switch (index) {
    case 0:
        return QString("20.0 kS/s");
    case 1:
        return QString("25.0 kS/s");
    case 2:
        return QString("30.0 kS/s");
    case 3:
        return QString("40.0 kS/s");
    default:
        return QString("n/a");
    }
}

// Save stimulation settings to *.xml file.
void MainWindow::saveStimSettings()
{
    QString saveStimSettingsFileName;
    saveStimSettingsFileName = QFileDialog::getSaveFileName(this,
                                            tr("Select Stimulation Parameters Filename"), ".",
                                            tr("Intan Stim Parameters Files (*.xml)"));

    if (saveStimSettingsFileName.isEmpty()) {
        wavePlot->setFocus();
        return;
    }

    QFile stimSettingsFile(saveStimSettingsFileName);
    if (!stimSettingsFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Cannot Save Stimulation Parameters File"),
                              tr("Cannot open new stimulation parameters file for writing."));
        wavePlot->setFocus();
        return;
    }

    statusBar()->showMessage("Saving stimulation parameters to file...");

    QXmlStreamWriter xml(&stimSettingsFile);
    xml.setAutoFormatting(true);

    // Save stimulation parameters
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xstim>");
    xml.writeStartElement("xstim");
    xml.writeAttribute("version", "1.0");

    signalSources->writeToXml(xml);

    // Close file
    xml.writeEndDocument();
    stimSettingsFile.close();

    statusBar()->clearMessage();
    wavePlot->setFocus();
}

// Load stimulation settings from *.xml file
void MainWindow::loadStimSettings()
{
    double timestep_us = 1.0e6 / boardSampleRate;
    double currentstep_uA = Rhs2000Registers::stimStepSizeToDouble(stimStep) / 1.0e-6;

    QString loadStimSettingsFileName;
    loadStimSettingsFileName = QFileDialog::getOpenFileName(this,
                                                        tr("Select Stimulation Parameters Filename"), ".",
                                                        tr("Intan Stim Parameters Files (*.xml)"));
    if (loadStimSettingsFileName.isEmpty()) {
        wavePlot->setFocus();
        return;
    }

    QFile stimSettingsFile(loadStimSettingsFileName);
    if (!stimSettingsFile.open(QIODevice::ReadOnly)) {
        cerr << "Can't open stim parameters file " <<
                loadStimSettingsFileName.toStdString() << endl;
        wavePlot->setFocus();
        return;
    }

    runButton->setEnabled(false);
    recordButton->setEnabled(false);
    triggerButton->setEnabled(false);

    baseFilenameButton->setEnabled(false);
    renameChannelButton->setEnabled(false);
    changeBandwidthButton->setEnabled(false);
    impedanceFreqSelectButton->setEnabled(false);
    runImpedanceTestButton->setEnabled(false);
    scanButton->setEnabled(false);
    setCableDelayButton->setEnabled(false);

    enableChannelButton->setEnabled(false);
    enableAllButton->setEnabled(false);
    disableAllButton->setEnabled(false);
//    sampleRateComboBox->setEnabled(false);
    setSaveFormatButton->setEnabled(false);
    stimParamButton->setEnabled(false);
    ampSettleSettingsAction->setEnabled(false);
    chargeRecoverySettingsAction->setEnabled(false);

    statusBar()->showMessage("Loading stimulation settings...");

    stimParamsHaveChanged = true;
    QXmlStreamReader xml(&stimSettingsFile);

    signalSources->disableAllStim();    // disable any stimulation protocols first

    // Load settings
    if (xml.readNextStartElement()) {
        if (xml.name() == "xstim" && xml.attributes().value("version") == "1.0") {

            QString channelName;
            SignalChannel *channel;
            bool doneWithChannel;

            while (xml.readNextStartElement()) {
                if (xml.name() == "channel") {
                    channelName = xml.attributes().value("name").toString();
                    channel = signalSources->findChannelFromName(channelName);
                    if (channel == nullptr) {
                        cout << "Error: channel not found: " << channelName.toStdString() << endl;
                    } else {
                        doneWithChannel = false;
                        while (!doneWithChannel) {
                            if (xml.readNextStartElement()) {
                                if (xml.name() == "enabled") {
                                    channel->stimParameters->enabled = (bool)xml.readElementText().toInt();
                                } else if (xml.name() == "triggerSource") {
                                    channel->stimParameters->triggerSource = (StimParameters::TriggerSources)xml.readElementText().toInt();
                                } else if (xml.name() == "triggerEdgeOrLevel") {
                                    channel->stimParameters->triggerEdgeOrLevel = (StimParameters::TriggerEdgeOrLevels)xml.readElementText().toInt();
                                } else if (xml.name() == "triggerHighOrLow") {
                                    channel->stimParameters->triggerHighOrLow = (StimParameters::TriggerHighOrLows)xml.readElementText().toInt();
                                } else if (xml.name() == "postTriggerDelay") {
                                    channel->stimParameters->postTriggerDelay = xml.readElementText().toDouble();
                                } else if (xml.name() == "pulseOrTrain") {
                                    channel->stimParameters->pulseOrTrain = (StimParameters::PulseOrTrainValues)xml.readElementText().toInt();
                                } else if (xml.name() == "numberOfStimPulses") {
                                    channel->stimParameters->numberOfStimPulses = xml.readElementText().toInt();
                                } else if (xml.name() == "pulseTrainPeriod") {
                                    channel->stimParameters->pulseTrainPeriod = xml.readElementText().toDouble();
                                } else if (xml.name() == "refractoryPeriod") {
                                    channel->stimParameters->refractoryPeriod = xml.readElementText().toDouble();
                                } else if (xml.name() == "stimShape") {
                                    channel->stimParameters->stimShape = (StimParameters::StimShapeValues)xml.readElementText().toInt();
                                } else if (xml.name() == "stimPolarity") {
                                    channel->stimParameters->stimPolarity = (StimParameters::StimPolarityValues)xml.readElementText().toInt();
                                } else if (xml.name() == "firstPhaseDuration") {
                                    channel->stimParameters->firstPhaseDuration = xml.readElementText().toDouble();
                                } else if (xml.name() == "secondPhaseDuration") {
                                    channel->stimParameters->secondPhaseDuration = xml.readElementText().toDouble();
                                } else if (xml.name() == "interphaseDelay") {
                                    channel->stimParameters->interphaseDelay = xml.readElementText().toDouble();
                                } else if (xml.name() == "firstPhaseAmplitude") {
                                    channel->stimParameters->firstPhaseAmplitude = xml.readElementText().toDouble();
                                } else if (xml.name() == "secondPhaseAmplitude") {
                                    channel->stimParameters->secondPhaseAmplitude = xml.readElementText().toDouble();
                                } else if (xml.name() == "baselineVoltage") {
                                    channel->stimParameters->baselineVoltage = xml.readElementText().toDouble();
                                } else if (xml.name() == "preStimAmpSettle") {
                                    channel->stimParameters->preStimAmpSettle = xml.readElementText().toDouble();
                                } else if (xml.name() == "postStimAmpSettle") {
                                    channel->stimParameters->postStimAmpSettle = xml.readElementText().toDouble();
                                } else if (xml.name() == "maintainAmpSettle") {
                                    channel->stimParameters->maintainAmpSettle = (bool)xml.readElementText().toInt();
                                } else if (xml.name() == "enableAmpSettle") {
                                    channel->stimParameters->enableAmpSettle = (bool)xml.readElementText().toInt();
                                } else if (xml.name() == "postStimChargeRecovOn") {
                                    channel->stimParameters->postStimChargeRecovOn = xml.readElementText().toDouble();
                                } else if (xml.name() == "postStimChargeRecovOff") {
                                    channel->stimParameters->postStimChargeRecovOff = xml.readElementText().toDouble();
                                } else if (xml.name() == "enableChargeRecovery") {
                                    channel->stimParameters->enableChargeRecovery = (bool)xml.readElementText().toInt();
                                } else {
                                    cout << "Error: unrecognized XML name: " << xml.name().toString().toStdString() << endl;
                                }
                            } else if (xml.isEndElement()) {
                                if (xml.name() == "channel") {
                                    doneWithChannel = true;
                                    switch (channel->signalType) {
                                    case AmplifierSignal:
                                        setStimSequenceParameters(evalBoard, timestep_us, currentstep_uA, channel->commandStream, channel->chipChannel, channel->stimParameters);
                                        break;
                                    case BoardDacSignal:
                                        setAnalogOutSequenceParameters(evalBoard, timestep_us, channel->nativeChannelNumber, channel->stimParameters);
                                        break;
                                    case BoardDigOutSignal:
                                        setDigitalOutSequenceParameters(evalBoard, timestep_us, channel->nativeChannelNumber, channel->stimParameters);
                                        break;
                                    default:
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        else {
            xml.raiseError(QObject::tr("The file is not an XSTIM version 1.0 file."));
        }
    }

    // Close file
    stimSettingsFile.close();

    runButton->setEnabled(true);
    recordButton->setEnabled(validFilename);
    triggerButton->setEnabled(validFilename);

    baseFilenameButton->setEnabled(true);
    renameChannelButton->setEnabled(true);
    changeBandwidthButton->setEnabled(true);
    impedanceFreqSelectButton->setEnabled(true);
    runImpedanceTestButton->setEnabled(impedanceFreqValid);
    scanButton->setEnabled(true);
    setCableDelayButton->setEnabled(true);

    enableChannelButton->setEnabled(true);
    enableAllButton->setEnabled(true);
    disableAllButton->setEnabled(true);
//    sampleRateComboBox->setEnabled(true);
    setSaveFormatButton->setEnabled(true);
    stimParamButton->setEnabled(!(displayDigInButton->isChecked()));
    ampSettleSettingsAction->setEnabled(true);
    chargeRecoverySettingsAction->setEnabled(true);

    wavePlot->refreshScreen();
    statusBar()->clearMessage();
    wavePlot->setFocus();
}

void MainWindow::copyStimParameters()
{
    copiedStimParameters.copyStimParameters(wavePlot->selectedChannel()->stimParameters);
    statusBar()->showMessage(tr("Stimulation parameters copied from selected channel."), 1500);
}

void MainWindow::pasteStimParameters()
{
    if (running) return;

    double timestep_us = 1.0e6 / boardSampleRate;
    double currentstep_uA = Rhs2000Registers::stimStepSizeToDouble(stimStep) / 1.0e-6;

    copiedStimParameters.pasteStimParameters(wavePlot->selectedChannel()->stimParameters);

    if (!synthMode) {
        switch (wavePlot->selectedChannel()->signalType) {
        case AmplifierSignal:
            setStimSequenceParameters(evalBoard, timestep_us, currentstep_uA, wavePlot->selectedChannel()->commandStream,
                                      wavePlot->selectedChannel()->chipChannel, wavePlot->selectedChannel()->stimParameters);
            break;
        case BoardDacSignal:
            setAnalogOutSequenceParameters(evalBoard, timestep_us, wavePlot->selectedChannel()->nativeChannelNumber,
                                           wavePlot->selectedChannel()->stimParameters);
            break;
        case BoardDigOutSignal:
            setDigitalOutSequenceParameters(evalBoard, timestep_us, wavePlot->selectedChannel()->nativeChannelNumber,
                                            wavePlot->selectedChannel()->stimParameters);
            break;
        default:
            break;
        }
    }

    statusBar()->showMessage(tr("Stimulation parameters pasted to selected channel."), 1500);
    wavePlot->refreshScreen();
}

// Launch amp settle settings dialog and apply new settings
void MainWindow::ampSettleSettings()
{
    AmpSettleDialog ampSettleDialog(desiredLowerSettleBandwidth, useFastSettle, headstageGlobalSettle, this);
    if (ampSettleDialog.exec()) {
        // get updated settings
        desiredLowerSettleBandwidth = ampSettleDialog.lowFreqSettleLineEdit->text().toDouble();
        useFastSettle = (ampSettleDialog.ampSettleModeComboBox->currentIndex() == 1);
        headstageGlobalSettle = ampSettleDialog.settleHeadstageCheckBox->isChecked();

        // update values in hardware
        evalBoard->setAmpSettleMode(useFastSettle);
        evalBoard->setGlobalSettlePolicy(headstageGlobalSettle, headstageGlobalSettle,
                                         headstageGlobalSettle, headstageGlobalSettle, false);
        changeSampleRate(sampleRateIndex, false); // this function sets new amp bandwidth
    }
    wavePlot->setFocus();
}

// Launch charge recovery settings dialog and apply new settings
void MainWindow::chargeRecoverySettings()
{
    ChargeRecoveryDialog chargeRecoveryDialog(chargeRecoveryMode, chargeRecoveryCurrentLimit,
                                              chargeRecoveryTargetVoltage, this);
    if (chargeRecoveryDialog.exec()) {
        // get updated settings
        chargeRecoveryMode = (chargeRecoveryDialog.chargeRecoveryModeComboBox->currentIndex() == 1);
        chargeRecoveryCurrentLimit = chargeRecoveryDialog.getCurrentLimit();
        chargeRecoveryTargetVoltage = chargeRecoveryDialog.targetVoltageLineEdit->text().toDouble();

        // update values in hardware
        setChargeRecoveryParameters(chargeRecoveryMode, chargeRecoveryCurrentLimit, chargeRecoveryTargetVoltage);
    }
    wavePlot->setFocus();
}

// Update charge recovery parameters on all headstages
void MainWindow::setChargeRecoveryParameters(bool mode,
                                             Rhs2000Registers::ChargeRecoveryCurrentLimit currentLimit,
                                             double targetVoltage)
{
    evalBoard->setChargeRecoveryMode(mode);
    evalBoard->enableAuxCommandsOnAllStreams();

    Rhs2000Registers chipRegisters(boardSampleRate, stimStep);
    int commandSequenceLength;
    vector<unsigned int> commandList;

    commandSequenceLength = chipRegisters.createCommandListConfigChargeRecovery(commandList, currentLimit, targetVoltage);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);

    chipRegisters.createCommandListDummy(commandList, 8192, chipRegisters.createRhs2000Command(Rhs2000Registers::Rhs2000CommandRegRead, 255));
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd2);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd3);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd4);

    evalBoard->setMaxTimeStep(commandSequenceLength);
    evalBoard->setContinuousRunMode(false);
    evalBoard->setStimCmdMode(false);

    evalBoard->run();
    while (evalBoard->isRunning() ) {
        qApp->processEvents();
    }

    commandSequenceLength = chipRegisters.createCommandListRegisterRead(commandList);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);
    evalBoard->run();
    while (evalBoard->isRunning() ) {
        qApp->processEvents();
    }

    Rhs2000DataBlock* dataBlock = new Rhs2000DataBlock(evalBoard->getNumEnabledDataStreams());;
    evalBoard->readDataBlock(dataBlock);
    evalBoard->readDataBlock(dataBlock);

    commandSequenceLength = chipRegisters.createCommandListRegisterConfig(commandList, true);
    evalBoard->uploadCommandList(commandList, Rhs2000EvalBoard::AuxCmd1);
    evalBoard->selectAuxCommandLength(Rhs2000EvalBoard::AuxCmd1, 0, commandSequenceLength - 1);
}
