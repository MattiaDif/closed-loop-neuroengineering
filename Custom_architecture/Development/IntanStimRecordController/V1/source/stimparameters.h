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

#ifndef STIMPARAMETERS_H
#define STIMPARAMETERS_H

#include "signalchannel.h"

class QXmlStreamWriter;

class StimParameters
{
public:

    enum StimShapeValues {
        Biphasic = 0,
        BiphasicWithInterphaseDelay = 1,
        Triphasic = 2,
        Monophasic = 3
    };

    enum StimPolarityValues {
        NegativeFirst = 0,
        PositiveFirst = 1
    };

    enum TriggerSources {
        DigitalIn1 = 0, DigitalIn2 = 1, DigitalIn3 = 2, DigitalIn4 = 3, DigitalIn5 = 4, DigitalIn6 = 5, DigitalIn7 = 6, DigitalIn8 = 7,
        DigitalIn9 = 8, DigitalIn10 = 9, DigitalIn11 = 10, DigitalIn12 = 11, DigitalIn13 = 12, DigitalIn14 = 13, DigitalIn15 = 14, DigitalIn16 = 15,
        AnalogIn1 = 16, AnalogIn2 = 17, AnalogIn3 = 18, AnalogIn4 = 19, AnalogIn5 = 20, AnalogIn6 = 21, AnalogIn7 = 22, AnalogIn8 = 23,
        KeyPress1 = 24, KeyPress2 = 25, KeyPress3 = 26, KeyPress4 = 27, KeyPress5 = 28, KeyPress6 = 29, KeyPress7 = 30, KeyPress8 = 31
    };

    enum TriggerEdgeOrLevels {
        Edge = 0,
        Level = 1
    };

    enum TriggerHighOrLows {
        High = 0,
        Low = 1
    };

    enum PulseOrTrainValues {
        SinglePulse = 0,
        PulseTrain = 1
    };


    StimParameters();
    void copyStimParameters(StimParameters *source);
    void pasteStimParameters(StimParameters *destination);
    void writeXml(QXmlStreamWriter &xml, SignalType signalType);

    StimShapeValues stimShape;
    StimPolarityValues stimPolarity;
    double firstPhaseDuration;
    double secondPhaseDuration;
    double interphaseDelay;
    double firstPhaseAmplitude;
    double secondPhaseAmplitude;
    double baselineVoltage;

    TriggerSources triggerSource;
    TriggerEdgeOrLevels triggerEdgeOrLevel;
    TriggerHighOrLows triggerHighOrLow;
    bool enabled;
    double postTriggerDelay;
    PulseOrTrainValues pulseOrTrain;
    int numberOfStimPulses;
    double pulseTrainPeriod;
    double refractoryPeriod;

    double preStimAmpSettle;
    double postStimAmpSettle;
    bool maintainAmpSettle;
    bool enableAmpSettle;

    double postStimChargeRecovOn;
    double postStimChargeRecovOff;
    bool enableChargeRecovery;
};

#endif // STIMPARAMETERS_H

