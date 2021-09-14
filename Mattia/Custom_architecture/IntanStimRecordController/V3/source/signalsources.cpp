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
#include <QString>
#include <QDataStream>
#include <QXmlStreamWriter>
#include <iostream>

#include "signalchannel.h"
#include "signalgroup.h"
#include "signalsources.h"
#include "stimparameters.h"

using namespace std;

// Data structure containing descriptions of all signal sources acquired
// from the USB interface board.

SignalSources::SignalSources(int numPorts)
{
    int i;

    // We have up to eight SPI ports, 8 DAC outputs, 8 ADC inputs, and 16 digital inputs and outputs.
    signalPort.resize(numPorts + 4);

    for (i = 0; i < numPorts; i++) {
        signalPort[i].name = "Port " + QString(QChar(65 + i));
        signalPort[i].prefix = QString(QChar(65 + i));
        signalPort[i].enabled = false;
    }
    signalPort[numPorts + 0].name = "Analog In Ports";
    signalPort[numPorts + 1].name = "Analog Out Ports";
    signalPort[numPorts + 2].name = "Digital In Ports";
    signalPort[numPorts + 3].name = "Digital Out Ports";

    signalPort[numPorts + 0].prefix = "ANALOG-IN";
    signalPort[numPorts + 1].prefix = "ANALOG-OUT";
    signalPort[numPorts + 2].prefix = "DIGITAL-IN";
    signalPort[numPorts + 3].prefix = "DIGITAL-OUT";

    signalPort[numPorts + 0].enabled = true;   // Board analog inputs are always enabled
    signalPort[numPorts + 1].enabled = true;
    signalPort[numPorts + 2].enabled = true;   // Board digital inputs are always enabled
    signalPort[numPorts + 3].enabled = true;

    // Add 8 board analog input signals
    for (int channel = 0; channel < 8; ++channel) {
        signalPort[numPorts + 0].addBoardAdcChannel(channel);
        signalPort[numPorts + 0].channel[channel].enabled = false;
    }
    // Add 8 board analog output signals
    for (int channel = 0; channel < 8; ++channel) {
        signalPort[numPorts + 1].addBoardDacChannel(channel);
        signalPort[numPorts + 1].channel[channel].enabled = true;
    }
    // Add 16 board digital input signals
    for (int channel = 0; channel < 16; ++channel) {
        signalPort[numPorts + 2].addBoardDigInChannel(channel);
        signalPort[numPorts + 2].channel[channel].enabled = false;
    }
    // Add 16 board digital output signals
    for (int channel = 0; channel < 16; ++channel) {
        signalPort[numPorts + 3].addBoardDigOutChannel(channel);
        signalPort[numPorts + 3].channel[channel].enabled = true;
    }
    // Amplifier channels on SPI ports A-D are added later, if amplifier
    // boards are found to be connected to these ports.
}

// Return a pointer to a SignalChannel with a particular nativeName (e.g., "A-02").
SignalChannel* SignalSources::findChannelFromName(const QString &nativeName)
{
    SignalChannel *channel;

    for (int i = 0; i < signalPort.size(); ++i) {
        for (int j = 0; j < signalPort[i].numChannels(); ++j) {
            if (signalPort[i].channel[j].nativeChannelName == nativeName) {
                channel = &signalPort[i].channel[j];
                return channel;
            }
        }
    }

    return nullptr;
}

// Return a pointer to a SignalChannel corresponding to a particular USB interface
// data stream and chip channel number.
SignalChannel* SignalSources::findAmplifierChannel(int boardStream, int chipChannel)
{
    SignalChannel *channel;

    for (int i = 0; i < signalPort.size(); ++i) {
        for (int j = 0; j < signalPort[i].numChannels(); ++j) {
            if (signalPort[i].channel[j].signalType == AmplifierSignal &&
                    signalPort[i].channel[j].boardStream == boardStream &&
                    signalPort[i].channel[j].chipChannel == chipChannel) {
                channel = &signalPort[i].channel[j];
                return channel;
            }
        }
    }

    return nullptr;
}

// Stream all signal sources out to binary data stream.
QDataStream& operator<<(QDataStream &outStream, const SignalSources &a)
{
    outStream << (qint16) a.signalPort.size();
    for (int i = 0; i < a.signalPort.size(); ++i) {
        outStream << a.signalPort[i];
    }
    return outStream;
}

// Stream all signal sources in from binary data stream.
QDataStream& operator>>(QDataStream &inStream, SignalSources &a)
{
    qint16 tempQint16;
    int nGroups;

    inStream >> tempQint16;
    nGroups = (int) tempQint16;
    a.signalPort.resize(nGroups);

    for (int i = 0; i < nGroups; ++i) {
        inStream >> a.signalPort[i];
    }

    return inStream;
}

// Write all stimulation parameters to XML data stream.
void SignalSources::writeToXml(QXmlStreamWriter &xml)
{
    for (int i = 0; i < signalPort.size(); ++i) {
        signalPort[i].writeToXml(xml);
    }
}

void SignalSources::disableAllStim()
{
    for (int i = 0; i < signalPort.size(); ++i) {
        signalPort[i].disableAllStim();
    }
}


