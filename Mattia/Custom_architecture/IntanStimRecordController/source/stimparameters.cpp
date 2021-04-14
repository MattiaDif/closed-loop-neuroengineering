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

#include "stimparameters.h"
#include "signalchannel.h"
#include <QXmlStreamWriter>

StimParameters::StimParameters()
{
    //default values
    stimShape = StimParameters::Biphasic;
    stimPolarity = StimParameters::NegativeFirst;
    firstPhaseDuration = 100.0;
    secondPhaseDuration = 100.0;
    interphaseDelay = 100.0;
    firstPhaseAmplitude = 0;
    secondPhaseAmplitude = 0;
    baselineVoltage = 0;

    enabled = false;
    triggerSource = StimParameters::DigitalIn1;
    triggerEdgeOrLevel = StimParameters::Edge;
    triggerHighOrLow = StimParameters::High;
    postTriggerDelay = 0;
    pulseOrTrain = StimParameters::SinglePulse;
    numberOfStimPulses = 2;
    pulseTrainPeriod = 10000.0;
    refractoryPeriod = 1000.0;

    preStimAmpSettle = 0;
    postStimAmpSettle = 1000.0;
    maintainAmpSettle = false;
    enableAmpSettle = true;

    postStimChargeRecovOn = 0;
    postStimChargeRecovOff = 0;
    enableChargeRecovery = false;
}

void StimParameters::copyStimParameters(StimParameters *source)
{
    stimShape = source->stimShape;
    stimPolarity = source->stimPolarity;
    firstPhaseDuration = source->firstPhaseDuration;
    secondPhaseDuration = source->secondPhaseDuration;
    interphaseDelay = source->interphaseDelay;
    firstPhaseAmplitude = source->firstPhaseAmplitude;
    secondPhaseAmplitude = source->secondPhaseAmplitude;
    baselineVoltage = source->baselineVoltage;

    enabled = source->enabled;
    triggerSource = source->triggerSource;
    triggerEdgeOrLevel = source->triggerEdgeOrLevel;
    triggerHighOrLow = source->triggerHighOrLow;
    postTriggerDelay = source->postTriggerDelay;
    pulseOrTrain = source->pulseOrTrain;
    numberOfStimPulses = source->numberOfStimPulses;
    pulseTrainPeriod = source->pulseTrainPeriod;
    refractoryPeriod = source->refractoryPeriod;

    preStimAmpSettle = source->preStimAmpSettle;
    postStimAmpSettle = source->postStimAmpSettle;
    maintainAmpSettle = source->maintainAmpSettle;
    enableAmpSettle = source->enableAmpSettle;

    postStimChargeRecovOn = source->postStimChargeRecovOn;
    postStimChargeRecovOff = source->postStimChargeRecovOff;
    enableChargeRecovery = source->enableChargeRecovery;
}

void StimParameters::pasteStimParameters(StimParameters *destination)
{
    destination->stimShape = stimShape;
    destination->stimPolarity = stimPolarity;
    destination->firstPhaseDuration = firstPhaseDuration;
    destination->secondPhaseDuration = secondPhaseDuration;
    destination->interphaseDelay = interphaseDelay;
    destination->firstPhaseAmplitude = firstPhaseAmplitude;
    destination->secondPhaseAmplitude = secondPhaseAmplitude;
    destination->baselineVoltage = baselineVoltage;

    destination->enabled = enabled;
    destination->triggerSource = triggerSource;
    destination->triggerEdgeOrLevel = triggerEdgeOrLevel;
    destination->triggerHighOrLow = triggerHighOrLow;
    destination->postTriggerDelay = postTriggerDelay;
    destination->pulseOrTrain = pulseOrTrain;
    destination->numberOfStimPulses = numberOfStimPulses;
    destination->pulseTrainPeriod = pulseTrainPeriod;
    destination->refractoryPeriod = refractoryPeriod;

    destination->preStimAmpSettle = preStimAmpSettle;
    destination->postStimAmpSettle = postStimAmpSettle;
    destination->maintainAmpSettle = maintainAmpSettle;
    destination->enableAmpSettle = enableAmpSettle;

    destination->postStimChargeRecovOn = postStimChargeRecovOn;
    destination->postStimChargeRecovOff = postStimChargeRecovOff;
    destination->enableChargeRecovery = enableChargeRecovery;
}

void StimParameters::writeXml(QXmlStreamWriter &xml, SignalType signalType)
{
    // No stimulation parameters are associated with Analog In or Digital In channels.
    if (signalType == BoardAdcSignal || signalType == BoardDigInSignal) return;

    xml.writeTextElement("enabled", QString::number((int)enabled));
    xml.writeTextElement("triggerSource", QString::number((int)triggerSource));
    xml.writeTextElement("triggerEdgeOrLevel", QString::number((int)triggerEdgeOrLevel));
    xml.writeTextElement("triggerHighOrLow", QString::number((int)triggerHighOrLow));
    xml.writeTextElement("postTriggerDelay", QString::number(postTriggerDelay));
    xml.writeTextElement("pulseOrTrain", QString::number((int)pulseOrTrain));
    xml.writeTextElement("numberOfStimPulses", QString::number(numberOfStimPulses));
    xml.writeTextElement("pulseTrainPeriod", QString::number(pulseTrainPeriod));
    xml.writeTextElement("refractoryPeriod", QString::number(refractoryPeriod));

    if (signalType == AmplifierSignal || signalType == BoardDacSignal) {
        xml.writeTextElement("stimShape", QString::number((int)stimShape));
        xml.writeTextElement("stimPolarity", QString::number((int)stimPolarity));
    }

    xml.writeTextElement("firstPhaseDuration", QString::number(firstPhaseDuration));

    if (signalType == AmplifierSignal || signalType == BoardDacSignal) {
        xml.writeTextElement("secondPhaseDuration", QString::number(secondPhaseDuration));
        xml.writeTextElement("interphaseDelay", QString::number(interphaseDelay));
        xml.writeTextElement("firstPhaseAmplitude", QString::number(firstPhaseAmplitude));
        xml.writeTextElement("secondPhaseAmplitude", QString::number(secondPhaseAmplitude));
    }

    if (signalType == BoardDacSignal) {
        xml.writeTextElement("baselineVoltage", QString::number(baselineVoltage));
    }

    if (signalType == AmplifierSignal) {
        xml.writeTextElement("enableAmpSettle", QString::number((int)enableAmpSettle));
        xml.writeTextElement("preStimAmpSettle", QString::number(preStimAmpSettle));
        xml.writeTextElement("postStimAmpSettle", QString::number(postStimAmpSettle));
        xml.writeTextElement("maintainAmpSettle", QString::number((int)maintainAmpSettle));

        xml.writeTextElement("enableChargeRecovery", QString::number((int)enableChargeRecovery));
        xml.writeTextElement("postStimChargeRecovOn", QString::number(postStimChargeRecovOn));
        xml.writeTextElement("postStimChargeRecovOff", QString::number(postStimChargeRecovOff));
    }
}
