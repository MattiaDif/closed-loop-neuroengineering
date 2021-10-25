//  ------------------------------------------------------------------------
//
//  This file is part of the Intan Technologies RHS2000 Interface
//  Version 1.01
//  Copyright (C) 2013-2017 Intan Technologies
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

#include <QVector>
#include <QFile>
#include <QDataStream>
#include <queue>
#include <qmath.h>
#include <iostream>
#include <QElapsedTimer>

#include "mainwindow.h"
#include "signalprocessor.h"
#include "globalconstants.h"
#include "randomnumber.h"
#include "signalsources.h"
#include "signalgroup.h"
#include "signalchannel.h"
#include "rhs2000datablock.h"
#include "stimparameters.h"

using namespace std;

// This class stores and processes short segments of waveform data
// acquired from the USB interface board.  The primary purpose of the
// class is to read from a queue of Rhs2000DataBlock objects and scale
// this raw data appropriately to generate wavefrom vectors with units
// of volts or microvolts.
//
// The class is also capable of applying a notch filter to amplifier
// waveform data, measuring the amplitude of a particular frequency
// component (useful in the electrode impedance measurements), and
// generating synthetic neural or ECG data for demonstration purposes.

// Constructor.
SignalProcessor::SignalProcessor()
{
    // Notch filter initial parameters.
    notchFilterEnabled = false;
    a1 = 0.0;
    a2 = 0.0;
    b0 = 0.0;
    b1 = 0.0;
    b2 = 0.0;

    // Highpass filter initial parameters.
    highpassFilterEnabled = false;
    aHpf = 0.0;
    bHpf = 0.0;

    // Set up random number generator in case we are asked to generate synthetic data.
    random = new RandomNumber();
    random->setGaussianAccuracy(6);

    // Other synthetic waveform variables.
    tPulse = 0.0;
    synthTimeStamp = 0;

    amplifierPreFilterFast = nullptr;
}

SignalProcessor::~SignalProcessor()
{
    delete [] amplifierPreFilterFast;
}

inline int SignalProcessor::fastIndex(int stream, int channel, int t) const
{
    return ((t * numDataStreams * CHANNELS_PER_STREAM) + (channel * numDataStreams) + stream);
}

// Allocate memory to store waveform data.
void SignalProcessor::allocateMemory(int numStreams)
{
    delete [] amplifierPreFilterFast;

    numDataStreams = numStreams;

    // Allocate vector memory for waveforms from USB interface board and notch filter.
    // allocateDoubleArray3D(amplifierPreFilter, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * maxNumBlocks);
    amplifierPreFilterFast = new double [numStreams * CHANNELS_PER_STREAM * SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS];
    allocateDoubleArray3D(amplifierPostFilter, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateDoubleArray2D(highpassFilterState, numStreams, CHANNELS_PER_STREAM);
    allocateDoubleArray3D(prevAmplifierPreFilter, numStreams, CHANNELS_PER_STREAM, 2);
    allocateDoubleArray3D(prevAmplifierPostFilter, numStreams, CHANNELS_PER_STREAM, 2);
    allocateDoubleArray3D(dcAmplifier, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray3D(complianceLimit, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray3D(stimOn, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray3D(stimPol, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray3D(ampSettle, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray3D(chargeRecov, numStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateDoubleArray2D(boardDac, 8, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateDoubleArray2D(boardAdc, 8, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray2D(boardDigIn, 16, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);
    allocateIntArray2D(boardDigOut, 16, SAMPLES_PER_DATA_BLOCK * MAX_NUM_BLOCKS);

    // Initialize vector memory used in notch filter state.
    fillZerosDoubleArray3D(amplifierPostFilter);
    fillZerosDoubleArray3D(prevAmplifierPreFilter);
    fillZerosDoubleArray3D(prevAmplifierPostFilter);
    fillZerosDoubleArray2D(highpassFilterState);

    // Allocate vector memory for generating synthetic neural and ECG waveforms.
    allocateDoubleArray3D(synthSpikeAmplitude, numStreams, CHANNELS_PER_STREAM, 2);
    allocateDoubleArray3D(synthSpikeDuration, numStreams, CHANNELS_PER_STREAM, 2);
    allocateDoubleArray2D(synthRelativeSpikeRate, numStreams, CHANNELS_PER_STREAM);
    allocateDoubleArray2D(synthEcgAmplitude, numStreams, CHANNELS_PER_STREAM);

    // Assign random parameters for synthetic waveforms.
    for (int stream = 0; stream < numStreams; ++stream) {
        for (int channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
            synthEcgAmplitude[stream][channel] =
                    random->randomUniform(0.5, 3.0);
            for (int spikeNum = 0; spikeNum < 2; ++spikeNum) {
                synthSpikeAmplitude[stream][channel][spikeNum] =
                        random->randomUniform(-400.0, 100.0);
                synthSpikeDuration[stream][channel][spikeNum] =
                        random->randomUniform(0.3, 1.7);
                synthRelativeSpikeRate[stream][channel] =
                        random->randomUniform(0.1, 5.0);
            }
        }
    }
}

// Allocates memory for a 3-D array of doubles.
void SignalProcessor::allocateDoubleArray3D(QVector<QVector<QVector<double> > > &array3D,
                                            int xSize, int ySize, int zSize)
{
    int i, j;

    if (xSize == 0) return;
    array3D.resize(xSize);
    for (i = 0; i < xSize; ++i) {
        array3D[i].resize(ySize);
        for (j = 0; j < ySize; ++j) {
            array3D[i][j].resize(zSize);
        }
    }
}

// Allocates memory for a 3-D array of ints.
void SignalProcessor::allocateIntArray3D(QVector<QVector<QVector<int> > > &array3D,
                                         int xSize, int ySize, int zSize)
{
    int i, j;

    if (xSize == 0) return;
    array3D.resize(xSize);
    for (i = 0; i < xSize; ++i) {
        array3D[i].resize(ySize);
        for (j = 0; j < ySize; ++j) {
            array3D[i][j].resize(zSize);
        }
    }
}

// Allocates memory for a 2-D array of doubles.
void SignalProcessor::allocateDoubleArray2D(QVector<QVector<double> > &array2D,
                                            int xSize, int ySize)
{
    int i;

    if (xSize == 0) return;
    array2D.resize(xSize);
    for (i = 0; i < xSize; ++i) {
        array2D[i].resize(ySize);
    }
}

// Allocates memory for a 2-D array of integers.
void SignalProcessor::allocateIntArray2D(QVector<QVector<int> > &array2D,
                                         int xSize, int ySize)
{
    int i;

    array2D.resize(xSize);
    for (i = 0; i < xSize; ++i) {
        array2D[i].resize(ySize);
    }
}

// Allocates memory for a 1-D array of doubles.
void SignalProcessor::allocateDoubleArray1D(QVector<double> &array1D,
                                            int xSize)
{
    array1D.resize(xSize);
}

// Fill a 3-D array of doubles with zero.
void SignalProcessor::fillZerosDoubleArray3D(
        QVector<QVector<QVector<double> > > &array3D)
{
    int x, y;
    int xSize = array3D.size();

    if (xSize == 0) return;

    int ySize = array3D[0].size();

    for (x = 0; x < xSize; ++x) {
        for (y = 0; y < ySize; ++y) {
            array3D[x][y].fill(0.0);
        }
    }
}

// Fill a 2-D array of doubles with zero.
void SignalProcessor::fillZerosDoubleArray2D(
        QVector<QVector<double> > &array2D)
{
    int x;
    int xSize = array2D.size();

    if (xSize == 0) return;

    for (x = 0; x < xSize; ++x) {
        array2D[x].fill(0.0);
    }
}

// Creates lists (vectors, actually) of all enabled waveforms to expedite
// save-to-disk operations.
void SignalProcessor::createSaveList(SignalSources *signalSources, bool addTriggerChannel, int triggerChannel, double stimStepSize)
{
    int port, index;
    SignalChannel *currentChannel;

    synthTimeStamp = 0;  // for synthetic data mode

    saveListAmplifier.clear();
    saveListBoardAdc.clear();
    saveListBoardDac.clear();
    saveListBoardDigitalIn.clear();
    saveListBoardDigitalOut.clear();

    posStimAmplitudeList.clear();
    negStimAmplitudeList.clear();

    saveListBoardDigIn = false;

    for (port = 0; port < signalSources->signalPort.size(); ++port) {
        for (index = 0; index < signalSources->signalPort[port].numChannels(); ++index) {
            currentChannel = signalSources->signalPort[port].channelByNativeOrder(index);
            // Maybe add this channel if it is the trigger channel.
            if (addTriggerChannel) {
                if (triggerChannel > 15 && currentChannel->signalType == BoardAdcSignal) {
                    if (currentChannel->nativeChannelNumber == triggerChannel - 16) {
                        currentChannel->enabled = true;
                    }
                } else if (triggerChannel < 16 && currentChannel->signalType == BoardDigInSignal) {
                    if (currentChannel->nativeChannelNumber == triggerChannel) {
                        currentChannel->enabled = true;
                    }
                }
            }
            // Add all enabled channels to their appropriate save list.
            if (currentChannel->enabled) {
                switch (currentChannel->signalType) {
                case AmplifierSignal:
                    saveListAmplifier.append(currentChannel);
                    if (currentChannel->stimParameters->stimPolarity == StimParameters::NegativeFirst) {
                        negStimAmplitudeList.append((int)(currentChannel->stimParameters->firstPhaseAmplitude / stimStepSize + 0.5));
                        posStimAmplitudeList.append((int)(currentChannel->stimParameters->secondPhaseAmplitude / stimStepSize + 0.5));
                    } else {
                        posStimAmplitudeList.append((int)(currentChannel->stimParameters->firstPhaseAmplitude / stimStepSize + 0.5));
                        negStimAmplitudeList.append((int)(currentChannel->stimParameters->secondPhaseAmplitude / stimStepSize + 0.5));
                    }
                    break;
                case AuxInputSignal:
                    // error; aux inputs not present in RHS2000
                    break;
                case SupplyVoltageSignal:
                    // error; supply signals not present in RHS2000
                    break;
                case BoardAdcSignal:
                    saveListBoardAdc.append(currentChannel);
                    break;
                case BoardDacSignal:
                    saveListBoardDac.append(currentChannel);
                    break;
                case BoardDigInSignal:
                    saveListBoardDigIn = true;
                    saveListBoardDigitalIn.append(currentChannel);
                    break;
                case BoardDigOutSignal:
                    saveListBoardDigitalOut.append(currentChannel);
                    break;
                }
            }
        }
    }
}

// Create filename (appended to the specified path) for timestamp data.
void SignalProcessor::createTimestampFilename(QString path)
{
    timestampFileName = path + "/" + "time" + ".dat";
}

// Create filename (appended to the specified path) for data files in
// "One File Per Signal Type" format.
void SignalProcessor::createSignalTypeFilenames(QString path)
{
    amplifierFileName = path + "/" + "amplifier" + ".dat";
    dcAmplifierFileName = path + "/" + "dcamplifier" + ".dat";
    stimFileName = path + "/" + "stim" + ".dat";
    adcInputFileName = path + "/" + "analogin" + ".dat";
    dacOutputFileName = path + "/" + "analogout" + ".dat";
    digitalInputFileName = path + "/" + "digitalin" + ".dat";
    digitalOutputFileName = path + "/" + "digitalout" + ".dat";
}

// Open timestamp save file.
void SignalProcessor::openTimestampFile()
{
    timestampFile = new QFile(timestampFileName);

    if (!timestampFile->open(QIODevice::WriteOnly)) {
        cerr << "Cannot open file for writing: " <<
                qPrintable(timestampFile->errorString()) << endl;
    }

    timestampStream = new QDataStream(timestampFile);
    timestampStream->setVersion(QDataStream::Qt_4_8);

    // Set to little endian mode for compatibilty with MATLAB,
    // which is little endian on all platforms
    timestampStream->setByteOrder(QDataStream::LittleEndian);

    // Write 4-byte floating-point numbers (instead of the default 8-byte numbers)
    // to save disk space.
    timestampStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
}

// Open data files for "One File Per Signal Type" format.
void SignalProcessor::openSignalTypeFiles(bool saveTtlOut, bool saveDcAmps)
{
    amplifierFile = nullptr;
    dcAmplifierFile = nullptr;
    stimFile = nullptr;
    adcInputFile = nullptr;
    digitalInputFile = nullptr;
    digitalOutputFile = nullptr;

    amplifierStream = nullptr;
    dcAmplifierStream = nullptr;
    stimStream = nullptr;
    adcInputStream = nullptr;
    digitalInputStream = nullptr;
    digitalOutputStream = nullptr;

    if (saveListAmplifier.size() > 0) {
        amplifierFile = new QFile(amplifierFileName);
        if (!amplifierFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(amplifierFile->errorString()) << endl;
        amplifierStream = new QDataStream(amplifierFile);
        amplifierStream->setVersion(QDataStream::Qt_4_8);
        amplifierStream->setByteOrder(QDataStream::LittleEndian);
        amplifierStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveListAmplifier.size() > 0 && saveDcAmps) {
        dcAmplifierFile = new QFile(dcAmplifierFileName);
        if (!dcAmplifierFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(dcAmplifierFile->errorString()) << endl;
        dcAmplifierStream = new QDataStream(dcAmplifierFile);
        dcAmplifierStream->setVersion(QDataStream::Qt_4_8);
        dcAmplifierStream->setByteOrder(QDataStream::LittleEndian);
        dcAmplifierStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveListAmplifier.size() > 0) {
        stimFile = new QFile(stimFileName);
        if (!stimFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(stimFile->errorString()) << endl;
        stimStream = new QDataStream(stimFile);
        stimStream->setVersion(QDataStream::Qt_4_8);
        stimStream->setByteOrder(QDataStream::LittleEndian);
        stimStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveListBoardAdc.size() > 0) {
        adcInputFile = new QFile(adcInputFileName);
        if (!adcInputFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(adcInputFile->errorString()) << endl;
        adcInputStream = new QDataStream(adcInputFile);
        adcInputStream->setVersion(QDataStream::Qt_4_8);
        adcInputStream->setByteOrder(QDataStream::LittleEndian);
        adcInputStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveListBoardDac.size() > 0) {
        dacOutputFile = new QFile(dacOutputFileName);
        if (!dacOutputFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(dacOutputFile->errorString()) << endl;
        dacOutputStream = new QDataStream(dacOutputFile);
        dacOutputStream->setVersion(QDataStream::Qt_4_8);
        dacOutputStream->setByteOrder(QDataStream::LittleEndian);
        dacOutputStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveListBoardDigitalIn.size() > 0) {
        digitalInputFile = new QFile(digitalInputFileName);
        if (!digitalInputFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(digitalInputFile->errorString()) << endl;
        digitalInputStream = new QDataStream(digitalInputFile);
        digitalInputStream->setVersion(QDataStream::Qt_4_8);
        digitalInputStream->setByteOrder(QDataStream::LittleEndian);
        digitalInputStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
    if (saveTtlOut) {
        digitalOutputFile = new QFile(digitalOutputFileName);
        if (!digitalOutputFile->open(QIODevice::WriteOnly))
            cerr << "Cannot open file for writing: " <<
                    qPrintable(digitalOutputFile->errorString()) << endl;
        digitalOutputStream = new QDataStream(digitalOutputFile);
        digitalOutputStream->setVersion(QDataStream::Qt_4_8);
        digitalOutputStream->setByteOrder(QDataStream::LittleEndian);
        digitalOutputStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    }
}

// Close timestamp save file.
void SignalProcessor::closeTimestampFile()
{
    timestampFile->close();
    delete timestampStream;
    delete timestampFile;
}

// Close data files for "One File Per Signal Type" format.
void SignalProcessor::closeSignalTypeFiles()
{
    if (amplifierFile) {
        amplifierFile->close();
        delete amplifierStream;
        delete amplifierFile;
    }
    if (dcAmplifierFile) {
        dcAmplifierFile->close();
        delete dcAmplifierStream;
        delete dcAmplifierFile;
    }
    if (stimFile) {
        stimFile->close();
        delete stimStream;
        delete stimFile;
    }
    if (adcInputFile) {
        adcInputFile->close();
        delete adcInputStream;
        delete adcInputFile;
    }
    if (digitalInputFile) {
        digitalInputFile->close();
        delete digitalInputStream;
        delete digitalInputFile;
    }
    if (digitalOutputFile) {
        digitalOutputFile->close();
        delete digitalOutputStream;
        delete digitalOutputFile;
    }
}

// Create filenames (appended to the specified path) for each waveform.
void SignalProcessor::createFilenames(SignalSources *signalSources, QString path)
{
    int port, index;
    SignalChannel *currentChannel;

    for (port = 0; port < signalSources->signalPort.size(); ++port) {
        for (index = 0; index < signalSources->signalPort[port].numChannels(); ++index) {
            currentChannel = signalSources->signalPort[port].channelByNativeOrder(index);
            // Only create filenames for enabled channels.
            if (currentChannel->enabled) {
                switch (currentChannel->signalType) {
                case AmplifierSignal:
                    currentChannel->saveFileName =
                            path + "/" + "amp-" + currentChannel->nativeChannelName + ".dat";
                    currentChannel->dcSaveFileName =
                            path + "/" + "dc-" + currentChannel->nativeChannelName + ".dat";
                    currentChannel->stimSaveFileName =
                            path + "/" + "stim-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case AuxInputSignal:
                    currentChannel->saveFileName =
                            path + "/" + "aux-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case SupplyVoltageSignal:
                    currentChannel->saveFileName =
                            path + "/" + "vdd-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case BoardAdcSignal:
                    currentChannel->saveFileName =
                            path + "/" + "board-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case BoardDacSignal:
                    currentChannel->saveFileName =
                            path + "/" + "board-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case BoardDigInSignal:
                    currentChannel->saveFileName =
                            path + "/" + "board-" + currentChannel->nativeChannelName + ".dat";
                    break;
                case BoardDigOutSignal:
                    currentChannel->saveFileName =
                            path + "/" + "board-" + currentChannel->nativeChannelName + ".dat";
                }
            }
        }
    }
}

// Open individual save data files for all enabled waveforms.
void SignalProcessor::openSaveFiles(SignalSources *signalSources, bool saveDcAmps)
{
    int port, index;
    SignalChannel *currentChannel;

    for (port = 0; port < signalSources->signalPort.size(); ++port) {
        for (index = 0; index < signalSources->signalPort[port].numChannels(); ++index) {
            currentChannel = signalSources->signalPort[port].channelByNativeOrder(index);
            // Only open files for enabled channels.
            if (currentChannel->enabled) {
                currentChannel->saveFile = new QFile(currentChannel->saveFileName);

                if (!currentChannel->saveFile->open(QIODevice::WriteOnly)) {
                    cerr << "Cannot open file for writing: " <<
                            qPrintable(currentChannel->saveFile->errorString()) << endl;
                }

                currentChannel->saveStream = new QDataStream(currentChannel->saveFile);
                currentChannel->saveStream->setVersion(QDataStream::Qt_4_8);

                // Set to little endian mode for compatibilty with MATLAB,
                // which is little endian on all platforms
                currentChannel->saveStream->setByteOrder(QDataStream::LittleEndian);

                // Write 4-byte floating-point numbers (instead of the default 8-byte numbers)
                // to save disk space.
                currentChannel->saveStream->setFloatingPointPrecision(QDataStream::SinglePrecision);

                if (saveDcAmps && currentChannel->signalType == AmplifierSignal) {
                    currentChannel->dcSaveFile = new QFile(currentChannel->dcSaveFileName);
                    if (!currentChannel->dcSaveFile->open(QIODevice::WriteOnly)) {
                        cerr << "Cannot open file for writing: " <<
                                qPrintable(currentChannel->dcSaveFile->errorString()) << endl;
                    }
                    currentChannel->dcSaveStream = new QDataStream(currentChannel->dcSaveFile);
                    currentChannel->dcSaveStream->setVersion(QDataStream::Qt_4_8);
                    currentChannel->dcSaveStream->setByteOrder(QDataStream::LittleEndian);
                    currentChannel->dcSaveStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
                }

                if (currentChannel->signalType == AmplifierSignal && currentChannel->stimParameters->enabled == true) {
                    currentChannel->stimSaveFile = new QFile(currentChannel->stimSaveFileName);
                    if (!currentChannel->stimSaveFile->open(QIODevice::WriteOnly)) {
                        cerr << "Cannot open file for writing: " <<
                                qPrintable(currentChannel->stimSaveFile->errorString()) << endl;
                    }
                    currentChannel->stimSaveStream = new QDataStream(currentChannel->stimSaveFile);
                    currentChannel->stimSaveStream->setVersion(QDataStream::Qt_4_8);
                    currentChannel->stimSaveStream->setByteOrder(QDataStream::LittleEndian);
                    currentChannel->stimSaveStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
                }
            }
        }
    }
}

// Close individual save data files for all enabled waveforms.
void SignalProcessor::closeSaveFiles(SignalSources *signalSources, bool saveDcAmps)
{
    int port, index;
    SignalChannel *currentChannel;

    for (port = 0; port < signalSources->signalPort.size(); ++port) {
        for (index = 0; index < signalSources->signalPort[port].numChannels(); ++index) {
            currentChannel = signalSources->signalPort[port].channelByNativeOrder(index);
            // Only close files for enabled channels.
            if (currentChannel->enabled) {
                currentChannel->saveFile->close();
                delete currentChannel->saveStream;
                delete currentChannel->saveFile;
                if (saveDcAmps && currentChannel->signalType == AmplifierSignal) {
                    currentChannel->dcSaveFile->close();
                    delete currentChannel->dcSaveStream;
                    delete currentChannel->dcSaveFile;
                }
                if (currentChannel->signalType == AmplifierSignal && currentChannel->stimParameters->enabled == true) {
                    currentChannel->stimSaveFile->close();
                    delete currentChannel->stimSaveStream;
                    delete currentChannel->stimSaveFile;
                }
            }
        }
    }
}

// Reads numBlocks blocks of raw USB data stored in a queue of Rhs2000DataBlock
// objects, loads this data into this SignalProcessor object, scaling the raw
// data to generate waveforms with units of volts or microvolts.
//
// If lookForTrigger is true, this function looks for a trigger on digital input
// triggerChannel with triggerPolarity.  If trigger is found, triggerTimeIndex
// returns timestamp of trigger point.  Otherwise, triggerTimeIndex returns -1,
// indicating no trigger was found.
//
// Returns number of bytes written to binary datastream out if saveToDisk == true.
int SignalProcessor::loadAmplifierData(queue<Rhs2000DataBlock> &dataQueue,
                                       int numBlocks, bool lookForTrigger, int triggerChannel,
                                       int triggerPolarity, int &triggerTimeIndex, bool addToBuffer, queue<Rhs2000DataBlock> &bufferQueue,
                                       bool saveToDisk, QDataStream &out, SaveFormat format,
                                       bool saveTtlOut, bool saveDcAmps, int timestampOffset, ReferenceSource referenceSource)
{
    int block, t, channel, stream, i;
    int indexDcAmp = 0;
    int indexCompliance = 0;
    int indexStimOn = 0;
    int indexStimPol = 0;
    int indexAmpSettle = 0;
    int indexChargeRecov = 0;
    int indexAdc = 0;
    int indexDig = 0;
    int numWordsWritten = 0;

    int bufferIndex, stimOnLocal, stimPolLocal, stimAmpLocal, ampSettleLocal, chargeRecovLocal, complianceLimitLocal;
    qint16 tempQint16;
    quint16 tempQuint16;
    qint32 tempQint32;

    bool triggerFound = false;
    const double AnalogTriggerThreshold = 1.65;

    QElapsedTimer timer;

    timer.start();

    for (i = 0; i < saveListAmplifier.size(); ++i) {
        bufferArrayIndex[i] = 0;
        bufferArrayIndexDc[i] = 0;
    }

    if (lookForTrigger) {
        triggerTimeIndex = -1;
    }

    double* pIndex = amplifierPreFilterFast;
    for (block = 0; block < numBlocks; ++block) {

        // Load and scale RHS2000 amplifier waveforms
        // (sampled at amplifier sampling rate)

        // Old, slow method:
//        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
//            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
//                for (stream = 0; stream < numDataStreams; ++stream) {
//                    // Amplifier waveform units = microvolts
//                    amplifierPreFilter[stream][channel][indexAmp] = 0.195 *
//                           (dataQueue.front().amplifierData[stream][channel][t] - 32768);
//                }
//            }
//            ++indexAmp;
//        }

        // New, faster method:
        int* pIndex2 = dataQueue.front().amplifierDataFast;
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                for (stream = 0; stream < numDataStreams; ++stream) {
                    // Amplifier waveform units = microvolts
                    (*pIndex++) =  0.195 * ((*pIndex2++) - 32768); // units = uV
                }
            }
        }

        // Optional software re-referencing
        if (referenceSource.softwareMode) {
            pIndex -= SAMPLES_PER_DATA_BLOCK * CHANNELS_PER_STREAM * numDataStreams;  // jump back to beginning of data block
            // Point to user-selected software reference channel
            double* refIndex = pIndex + referenceSource.channel * numDataStreams + referenceSource.stream;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    for (stream = 0; stream < numDataStreams; ++stream) {
                        if (pIndex != refIndex) {
                            *pIndex -=  *refIndex;  // subtract reference channel
                        }
                        pIndex++;
                    }
                }
                refIndex += (CHANNELS_PER_STREAM * numDataStreams);
            }
        }

        // Load DC amplifier waveforms
        // (sampled at amplifier sampling rate)
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                for (stream = 0; stream < numDataStreams; ++stream) {
//                    dcAmplifier[stream][channel][indexDcAmp] = -0.01923 *
//                           (dataQueue.front().dcAmplifierData[stream][channel][t] - 512); // units = V   SB commented and changed with next conversion
                    dcAmplifier[stream][channel][indexDcAmp] = 0.195 *
                           (dataQueue.front().dcAmplifierData[stream][channel][t] - 32768); // units = V
                }
            }
            ++indexDcAmp;
        }

        // Load compliance limit markers
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                for (stream = 0; stream < numDataStreams; ++stream) {
                    complianceLimit[stream][channel][indexCompliance] = dataQueue.front().complianceLimit[stream][channel][t];
                }
            }
            ++indexCompliance;
        }

        // Load stim control parameters
        // (sampled at amplifier sampling rate)
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    stimOn[stream][channel][indexStimOn] = (dataQueue.front().stimOn[stream][t] & (1 << channel)) != 0;
                }
            }
            ++indexStimOn;
        }
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    stimPol[stream][channel][indexStimPol] = (dataQueue.front().stimPol[stream][t] & (1 << channel)) != 0;
                }
            }
            ++indexStimPol;
        }
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    ampSettle[stream][channel][indexAmpSettle] = (dataQueue.front().ampSettle[stream][t] & (1 << channel)) != 0;
                }
            }
            ++indexAmpSettle;
        }
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    chargeRecov[stream][channel][indexChargeRecov] = (dataQueue.front().chargeRecov[stream][t] & (1 << channel)) != 0;
                }
            }
            ++indexChargeRecov;
        }

        // Load and scale USB interface board ADC waveforms
        // (sampled at amplifier sampling rate)
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < 8; ++channel) {
                // DAC waveform units = volts
                boardDac[channel][indexAdc] =
                        0.0003125 * (dataQueue.front().boardDacData[channel][t] - 32768);
                // ADC waveform units = volts
                boardAdc[channel][indexAdc] =
                        0.0003125 * (dataQueue.front().boardAdcData[channel][t] - 32768);
            }
            if (lookForTrigger && !triggerFound && triggerChannel >= 16) {
                if (triggerPolarity) {
                    // Trigger on logic low
                    if (boardAdc[triggerChannel - 16][indexAdc] < AnalogTriggerThreshold) {
                        triggerTimeIndex = dataQueue.front().timeStamp[t];
                        triggerFound = true;
                    }
                } else {
                    // Trigger on logic high
                    if (boardAdc[triggerChannel - 16][indexAdc] >= AnalogTriggerThreshold) {
                        triggerTimeIndex = dataQueue.front().timeStamp[t];
                        triggerFound = true;
                    }
                }
            }
            ++indexAdc;
        }

        // Load USB interface board digital input and output waveforms
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < 16; ++channel) {
                boardDigIn[channel][indexDig] =
                        (dataQueue.front().ttlIn[t] & (1 << channel)) != 0;
                boardDigOut[channel][indexDig] =
                        (dataQueue.front().ttlOut[t] & (1 << channel)) != 0;
                }
            if (lookForTrigger && !triggerFound && triggerChannel < 16) {
                if (triggerPolarity) {
                    // Trigger on logic low
                    if (boardDigIn[triggerChannel][indexDig] == 0) {
                        triggerTimeIndex = dataQueue.front().timeStamp[t];
                        triggerFound = true;
                    }
                } else {
                    // Trigger on logic high
                    if (boardDigIn[triggerChannel][indexDig] == 1) {
                        triggerTimeIndex = dataQueue.front().timeStamp[t];
                        triggerFound = true;
                    }
                }
            }
            ++indexDig;
        }

        // Optionally send binary data to binary output stream
        if (saveToDisk) {
            switch (format) {
            case SaveFormatIntan:
                // Save timestamp data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQint32 = ((qint32) dataQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                    dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint32 in little-endian format
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);     // Stream out all data at once to speed writing
                numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

                // Save amplifier data
                bufferIndex = 0;
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            // dataQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                            dataQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;

                // Save dc amplifier data
                if (saveDcAmps) {
                    bufferIndex = 0;
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            tempQuint16 = (quint16) dataQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                            dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                            dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                        }
                    }
                    out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
                }

                // Save stimulation data
                bufferIndex = 0;
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        stimOnLocal = (dataQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        stimPolLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                        stimAmpLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                        ampSettleLocal = (dataQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        chargeRecovLocal = (dataQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        complianceLimitLocal = dataQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        tempQuint16 = (quint16)(
                                (complianceLimitLocal ? (1 << 15) : 0) +
                                (chargeRecovLocal ? (1 << 14) : 0) +
                                (ampSettleLocal ? (1 << 13) : 0) +
                                ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                                (stimOnLocal * stimAmpLocal));
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;

                // Save board ADC data
                bufferIndex = 0;
                for (i = 0; i < saveListBoardAdc.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListBoardAdc.size() * SAMPLES_PER_DATA_BLOCK;

                // Save board DAC data
                bufferIndex = 0;
                for (i = 0; i < saveListBoardDac.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListBoardDac.size() * SAMPLES_PER_DATA_BLOCK;

                // Save board digital input data
                if (saveListBoardDigIn) {
                    // If ANY digital inputs are enabled, we save ALL 16 channels, since
                    // we are writing 16-bit chunks of data.
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) dataQueue.front().ttlIn[t];
                        ++numWordsWritten;
                    }
                }

                // Save board digital output data, if saveTtlOut = true
                if (saveTtlOut) {
                    // Save all 16 channels, since we are writing 16-bit chunks of data.
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) dataQueue.front().ttlOut[t];
                        ++numWordsWritten;
                    }
                }

                break;

            case SaveFormatFilePerSignalType:
                // Save timestamp data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQint32 = ((qint32) dataQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                    dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint 32 in little-endian format
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
                }
                timestampStream->writeRawData(dataStreamBuffer, bufferIndex);     // Stream out all data at once to speed writing
                numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

                // Save amplifier data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        tempQint16 = (qint16)
                            // (dataQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t] - 32768);
                                (dataQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)] - 32768);
                        dataStreamBuffer[bufferIndex++] = tempQint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                if (bufferIndex > 0) {
                    amplifierStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
                }

                // Save DC amplifier data
                if (saveDcAmps) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        for (i = 0; i < saveListAmplifier.size(); ++i) {
                            tempQuint16 = (quint16) dataQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                            dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                            dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                        }
                    }
                    if (bufferIndex > 0) {
                        dcAmplifierStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                        numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
                    }
                }

                // Save stimulation data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        stimOnLocal = (dataQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        stimPolLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                        stimAmpLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                        ampSettleLocal = (dataQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        chargeRecovLocal = (dataQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        complianceLimitLocal = dataQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        tempQuint16 = (quint16)(
                                (complianceLimitLocal ? (1 << 15) : 0) +
                                (chargeRecovLocal ? (1 << 14) : 0) +
                                (ampSettleLocal ? (1 << 13) : 0) +
                                ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                                (stimOnLocal * stimAmpLocal));
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                if (bufferIndex > 0) {
                    stimStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
                }

                // Save board ADC data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    for (i = 0; i < saveListBoardAdc.size(); ++i) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                if (bufferIndex > 0) {
                    adcInputStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += saveListBoardAdc.size() * SAMPLES_PER_DATA_BLOCK;
                }

                // Save board DAC data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    for (i = 0; i < saveListBoardDac.size(); ++i) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                if (bufferIndex > 0) {
                    dacOutputStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += saveListBoardDac.size() * SAMPLES_PER_DATA_BLOCK;
                }

                // Save board digital input data
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    if (saveListBoardDigIn) {
                        // If ANY digital inputs are enabled, we save ALL 16 channels, since
                        // we are writing 16-bit chunks of data.
                        *(digitalInputStream) << (quint16) dataQueue.front().ttlIn[t];
                        ++numWordsWritten;
                    }
                }

                // Save board digital output data, if saveTtlOut = true
                if (saveTtlOut) {
                    // Save all 16 channels, since we are writing 16-bit chunks of data.
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(digitalOutputStream) << (quint16) dataQueue.front().ttlOut[t];
                        ++numWordsWritten;
                    }
                }

                break;

            case SaveFormatFilePerChannel:
                // Save timestamp data
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQint32 = ((qint32) dataQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                    dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint 32 in little-endian format
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                    dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
                }
                timestampStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

                // Save amplifier data to dataStreamBufferArray; In in effort to increase write speed we will
                // collect amplifier data from all data blocks and then write all data at the end of this function.
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQint16 = (qint16)
                            // (dataQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t] - 32768);
                            (dataQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)] - 32768);
                        dataStreamBufferArray[i][bufferArrayIndex[i]++] = tempQint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                        dataStreamBufferArray[i][bufferArrayIndex[i]++] = (tempQint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }

                // Save DC amplifier data
                if (saveDcAmps) {
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            tempQuint16 = (quint16)dataQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                            dataStreamBufferArrayDc[i][bufferArrayIndexDc[i]++] = tempQuint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                            dataStreamBufferArrayDc[i][bufferArrayIndexDc[i]++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                        }
                        numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                    }
                }

                // Save stimulation data
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    if (saveListAmplifier[i]->stimParameters->enabled == true) {
                        bufferIndex = 0;
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            stimOnLocal = (dataQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                            stimPolLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                            stimAmpLocal = (dataQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                            ampSettleLocal = (dataQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                            chargeRecovLocal = (dataQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                            complianceLimitLocal = dataQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                            tempQuint16 = (quint16)(
                                    (complianceLimitLocal ? (1 << 15) : 0) +
                                    (chargeRecovLocal ? (1 << 14) : 0) +
                                    (ampSettleLocal ? (1 << 13) : 0) +
                                    ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                                    (stimOnLocal * stimAmpLocal));
                            dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                            dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                        }
                        saveListAmplifier.at(i)->stimSaveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                        numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                    }
                }

                // Save board ADC data
                for (i = 0; i < saveListBoardAdc.size(); ++i) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                    saveListBoardAdc.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }

                // Save board DAC data
                for (i = 0; i < saveListBoardDac.size(); ++i) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            dataQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                    saveListBoardDac.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }

                // Save board digital input data
                for (i = 0; i < saveListBoardDigitalIn.size(); ++i) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            ((dataQueue.front().ttlIn[t] & (1 << saveListBoardDigitalIn.at(i)->nativeChannelNumber)) != 0);
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = 0;  // (MSB of individual digital input will always be zero)
                    }
                    saveListBoardDigitalIn.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }

                // Save board digital output data, if saveTtlOut = true
                if (saveTtlOut) {
                    for (i = 0; i < 16; ++i) {
                        bufferIndex = 0;
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            tempQuint16 = (quint16)
                                ((dataQueue.front().ttlOut[t] & (1 << i)) != 0);
                            dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = 0;  // (MSB of individual digital input will always be zero)
                        }
                        saveListBoardDigitalOut.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                        numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                    }
                }

                break;
            }
        }
        if (addToBuffer) {
            bufferQueue.push(dataQueue.front());
        }

        // We are done with this Rhs2000DataBlock object; remove it from dataQueue
        dataQueue.pop();
    }

    // If we are operating on the "One File Per Channel" format, we have saved all amplifier data from
    // multiple data blocks in dataStreamBufferArray.  Now we write it all at once, for each channel.
    if (saveToDisk && format == SaveFormatFilePerChannel) {
        for (i = 0; i < saveListAmplifier.size(); ++i) {
            saveListAmplifier.at(i)->saveStream->writeRawData(dataStreamBufferArray[i], bufferArrayIndex[i]);    // Stream out all amplifier data at once to speed writing
        }
        if (saveDcAmps) {
            for (i = 0; i < saveListAmplifier.size(); ++i) {
                saveListAmplifier.at(i)->dcSaveStream->writeRawData(dataStreamBufferArrayDc[i], bufferArrayIndexDc[i]);    // Stream out all DC amplifier data at once to speed writing
            }
        }
    }

    // Return total number of bytes written to binary output stream
    return (2 * numWordsWritten);
}

// Save to entire contents of the buffer queue to disk, and empty the queue in the process.
// Returns number of bytes written to binary datastream out.
int SignalProcessor::saveBufferedData(queue<Rhs2000DataBlock> &bufferQueue, QDataStream &out, SaveFormat format,
                                      bool saveTtlOut, bool saveDcAmps, int timestampOffset)
{
    int t, i;
    int numWordsWritten = 0;

    int bufferIndex, stimOnLocal, stimPolLocal, stimAmpLocal, ampSettleLocal, chargeRecovLocal, complianceLimitLocal;
    qint16 tempQint16;
    quint16 tempQuint16;
    qint32 tempQint32;

    switch (format) {
    case SaveFormatIntan:
        while (bufferQueue.empty() == false) {
            // Save timestamp data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                tempQint32 = ((qint32) bufferQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint 32 in little-endian format
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
            }
            out.writeRawData(dataStreamBuffer, bufferIndex);     // Stream out all data at once to speed writing
            numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

            // Save amplifier data
            bufferIndex = 0;
            for (i = 0; i < saveListAmplifier.size(); ++i) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        // bufferQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        bufferQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
            numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;

            // Save dc amplifier data
            if (saveDcAmps) {
                bufferIndex = 0;
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16) bufferQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                }
                out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save stimulation data
            bufferIndex = 0;
            for (i = 0; i < saveListAmplifier.size(); ++i) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    stimOnLocal = (bufferQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    stimPolLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                    stimAmpLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                    ampSettleLocal = (bufferQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    chargeRecovLocal = (bufferQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    complianceLimitLocal = bufferQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                    tempQuint16 = (quint16)(
                            (complianceLimitLocal ? (1 << 15) : 0) +
                            (chargeRecovLocal ? (1 << 14) : 0) +
                            (ampSettleLocal ? (1 << 13) : 0) +
                            ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                            (stimOnLocal * stimAmpLocal));
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
            numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;

            // Save board ADC data
            bufferIndex = 0;
            for (i = 0; i < saveListBoardAdc.size(); ++i) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
            numWordsWritten += saveListBoardAdc.size() * SAMPLES_PER_DATA_BLOCK;

            // Save board DAC data
            bufferIndex = 0;
            for (i = 0; i < saveListBoardDac.size(); ++i) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            out.writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
            numWordsWritten += saveListBoardDac.size() * SAMPLES_PER_DATA_BLOCK;

            // Save board digital input data
            if (saveListBoardDigIn) {
                // If ANY digital inputs are enabled, we save ALL 16 channels, since
                // we are writing 16-bit chunks of data.
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    out << (quint16) bufferQueue.front().ttlIn[t];
                    ++numWordsWritten;
                }
            }

            // Save board digital output data, is saveTtlOut = true
            if (saveTtlOut) {
                // We save all 16 channels, since we are writing 16-bit chunks of data.
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    out << (quint16) bufferQueue.front().ttlOut[t];
                    ++numWordsWritten;
                }
            }
            // We are done with this Rhs2000DataBlock object; remove it from bufferQueue
            bufferQueue.pop();

        }
        break;
    case SaveFormatFilePerSignalType:
        while (bufferQueue.empty() == false) {

            // Save timestamp data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                tempQint32 = ((qint32) bufferQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint 32 in little-endian format
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
            }
            timestampStream->writeRawData(dataStreamBuffer, bufferIndex);     // Stream out all data at once to speed writing
            numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

            // Save amplifier data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    tempQint16 = (qint16)
                        // (bufferQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t] - 32768);
                            (bufferQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)] - 32768);
                    dataStreamBuffer[bufferIndex++] = tempQint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            if (bufferIndex > 0) {
                amplifierStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save DC amplifier data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    tempQuint16 = (quint16) bufferQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            if (bufferIndex > 0) {
                dcAmplifierStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save stimulation data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    stimOnLocal = (bufferQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    stimPolLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                    stimAmpLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                    ampSettleLocal = (bufferQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    chargeRecovLocal = (bufferQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                    complianceLimitLocal = bufferQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                    tempQuint16 = (quint16)(
                            (complianceLimitLocal ? (1 << 15) : 0) +
                            (chargeRecovLocal ? (1 << 14) : 0) +
                            (ampSettleLocal ? (1 << 13) : 0) +
                            ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                            (stimOnLocal * stimAmpLocal));
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            if (bufferIndex > 0) {
                stimStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListAmplifier.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save board ADC data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (i = 0; i < saveListBoardAdc.size(); ++i) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            if (bufferIndex > 0) {
                adcInputStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListBoardAdc.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save board DAC data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                for (i = 0; i < saveListBoardDac.size(); ++i) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
            }
            if (bufferIndex > 0) {
                dacOutputStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += saveListBoardDac.size() * SAMPLES_PER_DATA_BLOCK;
            }

            // Save board digital input data
            if (saveListBoardDigIn) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    // If ANY digital inputs are enabled, we save ALL 16 channels, since
                    // we are writing 16-bit chunks of data.
                    *(digitalInputStream) << (quint16) bufferQueue.front().ttlIn[t];
                    ++numWordsWritten;
                }
            }

            // Save board digital output data, if saveTtlOut = true
            if (saveTtlOut) {
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    // We save all 16 channels, since we are writing 16-bit chunks of data.
                    *(digitalOutputStream) << (quint16) bufferQueue.front().ttlOut[t];
                    ++numWordsWritten;
                }
            }

            // We are done with this Rhs2000DataBlock object; remove it from bufferQueue
            bufferQueue.pop();
        }
        break;

    case SaveFormatFilePerChannel:
        while (bufferQueue.empty() == false) {
            // Save timestamp data
            bufferIndex = 0;
            for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                tempQint32 = ((qint32) bufferQueue.front().timeStamp[t]) - ((qint32) timestampOffset);
                dataStreamBuffer[bufferIndex++] = tempQint32 & 0x000000ff;          // Save qint 32 in little-endian format
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x0000ff00) >> 8;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0x00ff0000) >> 16;
                dataStreamBuffer[bufferIndex++] = (tempQint32 & 0xff000000) >> 24;
            }
            timestampStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
            numWordsWritten += 2 * SAMPLES_PER_DATA_BLOCK;

            // Save amplifier data
            for (i = 0; i < saveListAmplifier.size(); ++i) {
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQint16 = (qint16)
                        // (bufferQueue.front().amplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t] - 32768);
                            (bufferQueue.front().amplifierDataFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, t)] - 32768);
                    dataStreamBuffer[bufferIndex++] = tempQint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQint16 & 0xff00) >> 8;  // (MSByte last)
                }
                saveListAmplifier.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += SAMPLES_PER_DATA_BLOCK;
            }

            // Save DC amplifier data
            if (saveDcAmps) {
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16) bufferQueue.front().dcAmplifierData[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save qint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                    saveListAmplifier.at(i)->dcSaveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }
            }

            // Save stimulation data
            for (i = 0; i < saveListAmplifier.size(); ++i) {
                if (saveListAmplifier[i]->stimParameters->enabled == true) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        stimOnLocal = (bufferQueue.front().stimOn[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        stimPolLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 0 : 1; // 0 = pos, 1 = neg
                        stimAmpLocal = (bufferQueue.front().stimPol[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? posStimAmplitudeList.at(i) : negStimAmplitudeList.at(i);
                        ampSettleLocal = (bufferQueue.front().ampSettle[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        chargeRecovLocal = (bufferQueue.front().chargeRecov[saveListAmplifier.at(i)->boardStream][t] & (1 << saveListAmplifier.at(i)->chipChannel)) ? 1 : 0;
                        complianceLimitLocal = bufferQueue.front().complianceLimit[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][t];
                        tempQuint16 = (quint16)(
                                (complianceLimitLocal ? (1 << 15) : 0) +
                                (chargeRecovLocal ? (1 << 14) : 0) +
                                (ampSettleLocal ? (1 << 13) : 0) +
                                ((stimOnLocal * stimPolLocal) ? (1 << 8) : 0) +
                                (stimOnLocal * stimAmpLocal));
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                    }
                    saveListAmplifier.at(i)->stimSaveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }
            }

            // Save board ADC data
            for (i = 0; i < saveListBoardAdc.size(); ++i) {
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardAdcData[saveListBoardAdc.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
                saveListBoardAdc.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += SAMPLES_PER_DATA_BLOCK;
            }

            // Save board DAC data
            for (i = 0; i < saveListBoardDac.size(); ++i) {
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        bufferQueue.front().boardDacData[saveListBoardDac.at(i)->nativeChannelNumber][t];
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = (tempQuint16 & 0xff00) >> 8;  // (MSByte last)
                }
                saveListBoardDac.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += SAMPLES_PER_DATA_BLOCK;
            }

            // Save board digital input data
            for (i = 0; i < saveListBoardDigitalIn.size(); ++i) {
                bufferIndex = 0;
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    tempQuint16 = (quint16)
                        ((bufferQueue.front().ttlIn[t] & (1 << saveListBoardDigitalIn.at(i)->nativeChannelNumber)) != 0);
                    dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                    dataStreamBuffer[bufferIndex++] = 0;  // (MSB of individual digital input will always be zero)
                }
                saveListBoardDigitalIn.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                numWordsWritten += SAMPLES_PER_DATA_BLOCK;
            }

            // Save board digital output data, if saveTtlOut = true
            if (saveTtlOut) {
                for (i = 0; i < 16; ++i) {
                    bufferIndex = 0;
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        tempQuint16 = (quint16)
                            ((bufferQueue.front().ttlOut[t] & (1 << i)) != 0);
                        dataStreamBuffer[bufferIndex++] = tempQuint16 & 0x00ff;         // Save quint16 in little-endian format (LSByte first)
                        dataStreamBuffer[bufferIndex++] = 0;  // (MSB of individual digital input will always be zero)
                    }
                    saveListBoardDigitalOut.at(i)->saveStream->writeRawData(dataStreamBuffer, bufferIndex);    // Stream out all data at once to speed writing
                    numWordsWritten += SAMPLES_PER_DATA_BLOCK;
                }
            }

            // We are done with this Rhs2000DataBlock object; remove it from bufferQueue
            bufferQueue.pop();
        }
        break;
    }

    // Return total number of bytes written to binary output stream
    return (2 * numWordsWritten);
}

// This function behaves similarly to loadAmplifierData, but generates
// synthetic neural or ECG data for demonstration purposes when there is
// no USB interface board present.
// Returns number of bytes written to binary datastream out if saveToDisk == true.
int SignalProcessor::loadSyntheticData(int numBlocks, double sampleRate,
                                       bool saveToDisk, QDataStream &out,
                                       SaveFormat format, bool saveTtlOut, bool saveDcAmps,
                                       ReferenceSource& referenceSource)
{
    int block, t, channel, stream, i;
    int indexCompliance = 0;
    int indexAdc = 0;
    int indexDig = 0;
    int numWordsWritten = 0;

    double tStepMsec = 1000.0 / sampleRate;
    double spikeDelay, ecgValue;
    bool spikePresent;
    int spikeNum;

    // If the sample rate is 5 kS/s or higher, generate synthetic neural data;
    // otherwise, generate synthetic ECG data.
    if (sampleRate > 4999.9) {
        // Generate synthetic neural data.
        for (block = 0; block < numBlocks; ++block) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    spikePresent = false;
                    spikeNum = 0;
                    if (random->randomUniform() < synthRelativeSpikeRate.at(stream).at(channel) * tStepMsec) {
                        spikePresent = true;
                        spikeDelay = random->randomUniform(0.0, 0.3);  // add some random time jitter
                        if (random->randomUniform() < 0.3) spikeNum = 1;  // choose between one of two spike types
                    }
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        // Create realistic background Gaussian noise of 2.4 uVrms (would be more in cortex)
                        //amplifierPreFilter[stream][channel][SAMPLES_PER_DATA_BLOCK * block + t] =
                        amplifierPreFilterFast[fastIndex(stream, channel, SAMPLES_PER_DATA_BLOCK * block + t)] =
                                2.4 * random->randomGaussian();
                        if (spikePresent) {
                            // Create synthetic spike
                            if (t * tStepMsec > spikeDelay &&
                                t * tStepMsec < synthSpikeDuration[stream][channel][spikeNum] + spikeDelay) {
                                //amplifierPreFilter[stream][channel][SAMPLES_PER_DATA_BLOCK * block + t] +=
                                amplifierPreFilterFast[fastIndex(stream, channel, SAMPLES_PER_DATA_BLOCK * block + t)] +=
                                        synthSpikeAmplitude.at(stream).at(channel).at(spikeNum) *
                                        qExp(-2.0 * (t * tStepMsec - spikeDelay)) *
                                        qSin(TWO_PI * (t * tStepMsec - spikeDelay) /
                                             synthSpikeDuration.at(stream).at(channel).at(spikeNum));
                            }
                        }
                    }
                }
            }
        }
    } else {
        // Generate synthetic ECG data.
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK * numBlocks; ++t) {
            // Piece together half sine waves to model QRS complex, P wave, and T wave
            if (tPulse < 80.0) {
                ecgValue = 40.0 * qSin(TWO_PI * tPulse / 160.0); // P wave
            } else if (tPulse > 100.0 && tPulse < 120.0) {
                ecgValue = -250.0 * qSin(TWO_PI * (tPulse - 100.0) / 40.0); // Q
            } else if (tPulse > 120.0 && tPulse < 180.0) {
                ecgValue = 1000.0 * qSin(TWO_PI * (tPulse - 120.0) / 120.0); // R
            } else if (tPulse > 180.0 && tPulse < 260.0) {
                ecgValue = -120.0 * qSin(TWO_PI * (tPulse - 180.0) / 160.0); // S
            } else if (tPulse > 340.0 && tPulse < 400.0) {
                ecgValue = 60.0 * qSin(TWO_PI * (tPulse - 340.0) / 120.0); // T wave
            } else {
                ecgValue = 0.0;
            }
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    // Multiply basic ECG waveform by channel-specific amplitude, and
                    // add 2.4 uVrms noise.
                    // amplifierPreFilter[stream][channel][t] =
                    amplifierPreFilterFast[fastIndex(stream, channel, t)] =
                            synthEcgAmplitude[stream][channel] * ecgValue +
                            2.4 * random->randomGaussian();
                }
            }
            tPulse += tStepMsec;
        }
    }

    if (referenceSource.softwareMode) {
        for (stream = 0; stream < numDataStreams; ++stream) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                if (!(stream == referenceSource.stream && channel == referenceSource.channel)) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK * numBlocks; ++t) {
                        amplifierPreFilterFast[fastIndex(stream, channel, t)] -=
                                amplifierPreFilterFast[fastIndex(referenceSource.stream, referenceSource.channel, t)];
                    }
                }
            }
        }
    }

    // Repeat ECG waveform with regular period.
    if (tPulse > 840.0) tPulse = 0.0;

    for (block = 0; block < numBlocks; ++block) {
        // Generate synthetic compliance limit data.
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (stream = 0; stream < numDataStreams; ++stream) {
                for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                    complianceLimit[stream][channel][indexCompliance] = 0;
                }
            }
            ++indexCompliance;
        }

        // Generate synthetic USB interface board ADC data.
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < 8; ++channel) {
                boardDac[channel][indexAdc] = 0.0;
                boardAdc[channel][indexAdc] = 0.0;
            }
            ++indexAdc;
        }

        // Generate synthetic USB interface board digital I/O data.
        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
            for (channel = 0; channel < 16; ++channel) {
                boardDigIn[channel][indexDig] = 0;
                boardDigOut[channel][indexDig] = 0;
            }
            ++indexDig;
        }
    }

    // Optionally send binary data to binary output stream
    if (saveToDisk) {
        switch (format) {
        case SaveFormatIntan:
            for (block = 0; block < numBlocks; ++block) {
                // Save timestamp data
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    out << (qint32) (synthTimeStamp++);
                    numWordsWritten += 2;
                }
                // Save amplifier data
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16)
                               // ((amplifierPreFilter[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][SAMPLES_PER_DATA_BLOCK * block + t] / 0.195) + 32768);
                               ((amplifierPreFilterFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, SAMPLES_PER_DATA_BLOCK * block + t)] / 0.195) + 32768);
                        ++numWordsWritten;
                    }
                }
                // Save dc amplifier data
                if (saveDcAmps) {
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            out << (quint16) 512;
                            ++numWordsWritten;
                        }
                    }
                }
                // Save stimulation data
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16)0;
                        numWordsWritten ++;
                    }
                }
                // Save board ADC data
                for (i = 0; i < saveListBoardAdc.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board DAC data
                for (i = 0; i < saveListBoardDac.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board digital input data
                if (saveListBoardDigIn) {
                    // If ANY digital inputs are enabled, we save ALL 16 channels, since
                    // we are writing 16-bit chunks of data.
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board digital output data
                if (saveTtlOut) {
                    // We save all 16 channels, since we are writing 16-bit chunks of data.
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        out << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
            }

            break;

        case SaveFormatFilePerSignalType:
            for (block = 0; block < numBlocks; ++block) {
                // Save timestamp data
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    *(timestampStream) << (qint32) (synthTimeStamp++);
                    numWordsWritten += 2;
                }

                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    // Save amplifier data
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        *(amplifierStream) << (qint16)
                               // (amplifierPreFilter[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][SAMPLES_PER_DATA_BLOCK * block + t] / 0.195);
                               (amplifierPreFilterFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, SAMPLES_PER_DATA_BLOCK * block + t)] / 0.195);
                        ++numWordsWritten;
                    }

                    // Save DC amplifier data
                    if (saveDcAmps) {
                        for (i = 0; i < saveListAmplifier.size(); ++i) {
                            *(dcAmplifierStream) << (quint16) 512;
                            ++numWordsWritten;
                        }
                    }

                    // Save board ADC data
                    for (i = 0; i < saveListBoardAdc.size(); ++i) {
                        *(adcInputStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                    // Save board DAC data
                    for (i = 0; i < saveListBoardDac.size(); ++i) {
                        *(dacOutputStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                    // Save board digital input data
                    if (saveListBoardDigIn) {
                        *(digitalInputStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                    // Save board digital output data
                    if (saveTtlOut) {
                        *(digitalOutputStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
            }
            break;

        case SaveFormatFilePerChannel:
            for (block = 0; block < numBlocks; ++block) {
                // Save timestamp data
                for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                    *(timestampStream) << (qint32) (synthTimeStamp++);
                    numWordsWritten += 2;
                }

                // Save amplifier data
                for (i = 0; i < saveListAmplifier.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(saveListAmplifier.at(i)->saveStream) << (qint16)
                            // (amplifierPreFilter[saveListAmplifier.at(i)->boardStream][saveListAmplifier.at(i)->chipChannel][SAMPLES_PER_DATA_BLOCK * block + t] / 0.195);
                            (amplifierPreFilterFast[fastIndex(saveListAmplifier.at(i)->boardStream, saveListAmplifier.at(i)->chipChannel, SAMPLES_PER_DATA_BLOCK * block + t)] / 0.195);
                        ++numWordsWritten;
                    }
                }

                // Save DC amplifier data
                if (saveDcAmps) {
                    for (i = 0; i < saveListAmplifier.size(); ++i) {
                        for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                            *(saveListAmplifier.at(i)->dcSaveStream) << (quint16) 512;
                            ++numWordsWritten;
                        }
                    }
                }

                // Save board ADC data
                for (i = 0; i < saveListBoardAdc.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(saveListBoardAdc.at(i)->saveStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board DAC data
                for (i = 0; i < saveListBoardDac.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(saveListBoardDac.at(i)->saveStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board digital input data
                for (i = 0; i < saveListBoardDigitalIn.size(); ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(saveListBoardDigitalIn.at(i)->saveStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
                // Save board digital output data
                for (i = 0; i < 16; ++i) {
                    for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
                        *(saveListBoardDigitalOut.at(i)->saveStream) << (quint16) 0;
                        ++numWordsWritten;
                    }
                }
            }
            break;
        }
    }

    // Return total number of bytes written to binary output stream
    return (2 * numWordsWritten);
}

// Returns the total number of bytes saved to disk per data block.
int SignalProcessor::bytesPerBlock(SaveFormat saveFormat, bool saveTtlOut)
{
    int bytes = 0;
    bytes += 4 * SAMPLES_PER_DATA_BLOCK;  // timestamps
    bytes += 2 * SAMPLES_PER_DATA_BLOCK * saveListAmplifier.size();
    bytes += 2 * SAMPLES_PER_DATA_BLOCK * saveListBoardAdc.size();
    bytes += 2 * SAMPLES_PER_DATA_BLOCK * saveListBoardDac.size();
    if (saveFormat == SaveFormatIntan || saveFormat == SaveFormatFilePerSignalType) {
        if (saveListBoardDigIn) {
            bytes += 2 * SAMPLES_PER_DATA_BLOCK;
        }
    } else {
        bytes += 2 * SAMPLES_PER_DATA_BLOCK * saveListBoardDigitalIn.size();
    }
    if (saveTtlOut) {
        if (saveFormat == SaveFormatIntan || saveFormat == SaveFormatFilePerSignalType) {
            bytes += 2 * SAMPLES_PER_DATA_BLOCK;
        } else {
            bytes += 2 * SAMPLES_PER_DATA_BLOCK * 16;
        }
    }
    return bytes;
}

// Set notch filter parameters.  All filter parameters are given in Hz (or
// in Samples/s).  A bandwidth of 10 Hz is recommended for 50 or 60 Hz notch
// filters.  Narrower bandwidths will produce extended ringing in the time
// domain in response to large transients.
void SignalProcessor::setNotchFilter(double notchFreq, double bandwidth,
                                     double sampleFreq)
{
    double d;

    d = exp(-PI * bandwidth / sampleFreq);

    // Calculate biquad IIR filter coefficients.
    a1 = -(1.0 + d * d) * cos(2.0 * PI * notchFreq / sampleFreq);
    a2 = d * d;
    b0 = (1 + d * d) / 2.0;
    b1 = a1;
    b2 = b0;
}

// Enables or disables amplifier waveform notch filter.
void SignalProcessor::setNotchFilterEnabled(bool enable)
{
    notchFilterEnabled = enable;
}

// Set highpass filter parameters.  All filter parameters are given in Hz (or
// in Samples/s).
void SignalProcessor::setHighpassFilter(double cutoffFreq, double sampleFreq)
{
    aHpf = exp(-1.0 * TWO_PI * cutoffFreq / sampleFreq);
    bHpf = 1.0 - aHpf;
}

// Enables or disables amplifier waveform highpass filter.
void SignalProcessor::setHighpassFilterEnabled(bool enable)
{
    highpassFilterEnabled = enable;
}

// Runs notch filter only on amplifier channels that are visible
// on the display (according to channelVisible).
void SignalProcessor::filterData(int numBlocks,
                                 const QVector<QVector<bool> > &channelVisible)
{
    int t, channel, stream;
    int length = SAMPLES_PER_DATA_BLOCK * numBlocks;

    // Note: Throughout this function, and elsewhere in this source code, we access
    // multidimensional 'QVector' containers using the at() function, so instead of
    // writing:
    //            x = my3dQVector[i][j][k];
    // we write:
    //            x = my3dQVector.at(i).at(j).at(k);
    //
    // Web research suggests that using at() instead of [] to access the contents
    // of a multidimensional QVector results in faster code, and some preliminary
    // experiments at Intan bear this out.  Web research also suggests that the
    // opposite could be true of C++ STL (non-Qt) 'vector' containers, so some
    // experimentation may be needed to optimize the runtime performance of code.

    if (notchFilterEnabled) {
        for (stream = 0; stream < numDataStreams; ++stream) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                if (channelVisible.at(stream).at(channel)) {
                    // Execute biquad IIR notch filter.  The filter "looks backwards" two timesteps,
                    // so we must use the prevAmplifierPreFilter and prevAmplifierPostFilter
                    // variables to store the last two samples from the previous block of
                    // waveform data so that the filter works smoothly across the "seams".
                    amplifierPostFilter[stream][channel][0] =
                            b2 * prevAmplifierPreFilter.at(stream).at(channel).at(0) +
                            b1 * prevAmplifierPreFilter.at(stream).at(channel).at(1) +
                            // b0 * amplifierPreFilter.at(stream).at(channel).at(0) -
                            b0 * amplifierPreFilterFast[fastIndex(stream, channel, 0)] -
                            a2 * prevAmplifierPostFilter.at(stream).at(channel).at(0) -
                            a1 * prevAmplifierPostFilter.at(stream).at(channel).at(1);
                    amplifierPostFilter[stream][channel][1] =
                            b2 * prevAmplifierPreFilter.at(stream).at(channel).at(1) +
                            // b1 * amplifierPreFilter.at(stream).at(channel).at(0) +
                            b1 * amplifierPreFilterFast[fastIndex(stream, channel, 0)] +
                            // b0 * amplifierPreFilter.at(stream).at(channel).at(1) -
                            b0 * amplifierPreFilterFast[fastIndex(stream, channel, 1)] -
                            a2 * prevAmplifierPostFilter.at(stream).at(channel).at(1) -
                            a1 * amplifierPostFilter.at(stream).at(channel).at(0);
                    for (t = 2; t < length; ++t) {
                        amplifierPostFilter[stream][channel][t] =
                                // b2 * amplifierPreFilter.at(stream).at(channel).at(t - 2) +
                                b2 * amplifierPreFilterFast[fastIndex(stream, channel, t - 2)] +
                                // b1 * amplifierPreFilter.at(stream).at(channel).at(t - 1) +
                                b1 * amplifierPreFilterFast[fastIndex(stream, channel, t - 1)] +
                                // b0 * amplifierPreFilter.at(stream).at(channel).at(t) -
                                b0 * amplifierPreFilterFast[fastIndex(stream, channel, t)] -
                                a2 * amplifierPostFilter.at(stream).at(channel).at(t - 2) -
                                a1 * amplifierPostFilter.at(stream).at(channel).at(t - 1);
                    }
                }
            }
        }
    } else {
        // If the notch filter is disabled, simply copy the data without filtering.
        for (stream = 0; stream < numDataStreams; ++stream) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                if (channelVisible.at(stream).at(channel)) {
                    for (t = 0; t < length; ++t) {
                        amplifierPostFilter[stream][channel][t] =
                                // amplifierPreFilter.at(stream).at(channel).at(t);
                                amplifierPreFilterFast[fastIndex(stream, channel, t)];
                    }
                }
            }
        }
    }

    // Save the last two data points from each waveform to use in successive IIR filter
    // calculations.
    for (stream = 0; stream < numDataStreams; ++stream) {
        for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
            prevAmplifierPreFilter[stream][channel][0] =
                    // amplifierPreFilter.at(stream).at(channel).at(length - 2);
                    amplifierPreFilterFast[fastIndex(stream, channel, length - 2)];
            prevAmplifierPreFilter[stream][channel][1] =
                    // amplifierPreFilter.at(stream).at(channel).at(length - 1);
                    amplifierPreFilterFast[fastIndex(stream, channel, length - 1)];
            prevAmplifierPostFilter[stream][channel][0] =
                    amplifierPostFilter.at(stream).at(channel).at(length - 2);
            prevAmplifierPostFilter[stream][channel][1] =
                    amplifierPostFilter.at(stream).at(channel).at(length - 1);
        }
    }

    // Apply first-order high-pass filter, if selected
    if (highpassFilterEnabled) {
        double temp;
        for (stream = 0; stream < numDataStreams; ++stream) {
            for (channel = 0; channel < CHANNELS_PER_STREAM; ++channel) {
                if (channelVisible.at(stream).at(channel)) {
                    for (t = 0; t < length; ++t) {
                        temp = amplifierPostFilter[stream][channel][t];
                        amplifierPostFilter[stream][channel][t] -= highpassFilterState[stream][channel];
                        highpassFilterState[stream][channel] =
                                aHpf * highpassFilterState[stream][channel] + bHpf * temp;
                    }
                }
            }
        }
    }

}

// Return the magnitude and phase (in degrees) of a selected frequency component (in Hz)
// for a selected amplifier channel on the selected USB data stream.
void SignalProcessor::measureComplexAmplitude(QVector<QVector<QVector<double> > > &measuredMagnitude,
                                              QVector<QVector<QVector<double> > > &measuredPhase,
                                              int capIndex, int stream, int chipChannel, int numBlocks,
                                              double sampleRate, double frequency, int numPeriods)
{
    int period = qRound(sampleRate / frequency);
    int startIndex = 0;
    int endIndex = startIndex + numPeriods * period - 1;

    // Move the measurement window to the end of the waveform to ignore start-up transient.
    while (endIndex < SAMPLES_PER_DATA_BLOCK * numBlocks - period) {
        startIndex += period;
        endIndex += period;
    }

    double iComponent, qComponent;

    // Measure real (iComponent) and imaginary (qComponent) amplitude of frequency component.
    amplitudeOfFreqComponent(iComponent, qComponent, amplifierPreFilterFast, stream, chipChannel,
                             startIndex, endIndex, sampleRate, frequency);
    // Calculate magnitude and phase from real (I) and imaginary (Q) components.
    measuredMagnitude[stream][chipChannel][capIndex] =
            qSqrt(iComponent * iComponent + qComponent * qComponent);
    measuredPhase[stream][chipChannel][capIndex] =
            RADIANS_TO_DEGREES *qAtan2(qComponent, iComponent);
}

// Returns the real and imaginary amplitudes of a selected frequency component in the vector
// data, between a start index and end index.
void SignalProcessor::amplitudeOfFreqComponent(double &realComponent, double &imagComponent,
                                               double* data, int stream, int channel, int startIndex,
                                               int endIndex, double sampleRate, double frequency)
{
    int length = endIndex - startIndex + 1;
    const double k = TWO_PI * frequency / sampleRate;  // precalculate for speed

    // Perform correlation with sine and cosine waveforms.
    double meanI = 0.0;
    double meanQ = 0.0;
    for (int t = startIndex; t <= endIndex; ++t) {
        meanI += data[fastIndex(stream, channel, t)] * qCos(k * t);
        meanQ += data[fastIndex(stream, channel, t)] * -1.0 * qSin(k * t);
    }
    meanI /= (double) length;
    meanQ /= (double) length;

    realComponent = 2.0 * meanI;
    imagComponent = 2.0 * meanQ;
}

