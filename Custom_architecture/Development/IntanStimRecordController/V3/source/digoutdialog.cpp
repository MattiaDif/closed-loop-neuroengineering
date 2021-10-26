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

#include "digoutdialog.h"
#include "timespinbox.h"
#include "digfigure.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

DigOutDialog::DigOutDialog(StimParameters *parameter, QString nativeChannelName, QString customChannelName, double timestep_us, QWidget *parent) :
    QDialog(parent)
{
    //set the parameter and timestep objects, so that they can be accessed by other functions in digoutdialog.cpp
    parameters = parameter;
    timestep = timestep_us;

    //create a new DigFigure
    digFigure = new DigFigure(parameters, this);

    //create trigger information widgets
    trigger = new QGroupBox(tr("Trigger"));

    enablePulse = new QCheckBox(tr("Enable"));

    triggerSourceLabel = new QLabel(tr("Trigger Source: "));
    triggerSource = new QComboBox();
    QStringList triggerSources;
    triggerSources << "DIGITAL IN 1" << "DIGITAL IN 2" << "DIGITAL IN 3" << "DIGITAL IN 4" << "DIGITAL IN 5"
                   << "DIGITAL IN 6" << "DIGITAL IN 7" << "DIGITAL IN 8" << "DIGITAL IN 9" << "DIGITAL IN 10"
                   << "DIGITAL IN 11" << "DIGITAL IN 12" << "DIGITAL IN 13" << "DIGITAL IN 14" << "DIGITAL IN 15"
                   << "DIGITAL IN 16" << "ANALOG IN 1" << "ANALOG IN 2" << "ANALOG IN 3" << "ANALOG IN 4"
                   << "ANALOG IN 5" << "ANALOG IN 6" << "ANALOG IN 7" << "ANALOG IN 8" << "KEYPRESS: 1"
                   << "KEYPRESS: 2" << "KEYPRESS: 3" << "KEYPRESS: 4" << "KEYPRESS: 5" << "KEYPRESS: 6"
                   << "KEYPRESS: 7" << "KEYPRESS: 8";
    triggerSource->addItems(triggerSources);

    triggerEdgeOrLevelLabel = new QLabel(tr(" "));
    triggerEdgeOrLevel = new QComboBox();
    QStringList triggerEdgeOrLevels;
    triggerEdgeOrLevels << "Edge Triggered" << "Level Triggered";
    triggerEdgeOrLevel->addItems(triggerEdgeOrLevels);

    triggerHighOrLowLabel = new QLabel(tr(" "));
    triggerHighOrLow = new QComboBox();
    QStringList triggerHighOrLows;
    triggerHighOrLows << "Trigger on High" << "Trigger on Low";
    triggerHighOrLow->addItems(triggerHighOrLows);

    postTriggerDelay = new TimeSpinBox(timestep_us, this);
    postTriggerDelay->setRange(0, 500000);
    postTriggerDelayLabel = new QLabel(tr("Post Trigger Delay: "));

    //create pulse waveform widgets
    pulse = new QGroupBox(tr("Pulse"));

    pulseDuration = new TimeSpinBox(timestep_us, this);
    pulseDuration->setRange(0, 100000);
    pulseDurationLabel = new QLabel(tr("Pulse Duration (D1): "));

    pulseRepetition = new QComboBox();
    QStringList pulseRepetitions;
    pulseRepetitions << "Single Pulse" << "Pulse Train";
    pulseRepetition->addItems(pulseRepetitions);
    pulseRepetitionLabel = new QLabel(tr("Pulse Repetition: "));

    numPulses = new QSpinBox();
    numPulses->setRange(2, 256);
    numPulsesLabel = new QLabel(tr("Number of Pulses: "));

    pulseTrainPeriod = new TimeSpinBox(timestep_us);
    pulseTrainPeriod->setRange(0, 1000000);
    pulseTrainPeriodLabel = new QLabel(tr("Pulse Train Period: "));

    pulseTrainFrequencyLabel = new QLabel(tr("Pulse Train Frequency: -- Hz"));

    refractoryPeriod = new TimeSpinBox(timestep_us);
    refractoryPeriod->setRange(0, 1000000);
    refractoryPeriodLabel = new QLabel(tr("Refractory Period: "));

    //create and connect 'OK' and 'Cancel' buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    //connect internal signals and slots (for rounding and graying out)
    connect(enablePulse, SIGNAL(stateChanged(int)), this, SLOT(enableWidgets()));
    connect(pulseRepetition, SIGNAL(currentIndexChanged(int)), this, SLOT(enableWidgets()));
    connect(postTriggerDelay, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(pulseDuration, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(pulseTrainPeriod, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(refractoryPeriod, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(pulseTrainPeriod, SIGNAL(valueChanged(double)), this, SLOT(calculatePulseTrainFrequency()));
    connect(pulseDuration, SIGNAL(valueChanged(double)), this, SLOT(constrainPulseTrainPeriod()));
    connect(enablePulse, SIGNAL(toggled(bool)), digFigure, SLOT(highlightStimTrace(bool)));

    //connect signals to stimFigure's non-highlight slots
    connect(pulseRepetition, SIGNAL(currentIndexChanged(int)), digFigure, SLOT(updatePulseOrTrain(int)));

    //connect signals to digFigure's highlight slots
    connect(this, SIGNAL(highlightFirstPhaseDuration(bool)), digFigure, SLOT(highlightFirstPhaseDuration(bool)));
    connect(this, SIGNAL(highlightPostTriggerDelay(bool)), digFigure, SLOT(highlightPostTriggerDelay(bool)));
    connect(this, SIGNAL(highlightPulseTrainPeriod(bool)), digFigure, SLOT(highlightPulseTrainPeriod(bool)));
    connect(this, SIGNAL(highlightRefractoryPeriod(bool)), digFigure, SLOT(highlightRefractoryPeriod(bool)));

    //update widgets' states to be different than the default states, so state changes are detected properly
    enablePulse->setChecked(true);
    pulseRepetition->setCurrentIndex(StimParameters::SinglePulse);

    //update dialog's state based on structParameters
    loadParameters(parameters);

    //trigger information widgets' layout
    QHBoxLayout *enablePulseRow = new QHBoxLayout;
    enablePulseRow->addWidget(enablePulse);

    QHBoxLayout *triggerSourceRow = new QHBoxLayout;
    triggerSourceRow->addWidget(triggerSourceLabel);
    triggerSourceRow->addStretch();
    triggerSourceRow->addWidget(triggerSource);

    QHBoxLayout *triggerEdgeOrLevelRow = new QHBoxLayout;
    triggerEdgeOrLevelRow->addWidget(triggerEdgeOrLevelLabel);
    triggerEdgeOrLevelRow->addWidget(triggerEdgeOrLevel);

    QHBoxLayout *triggerHighOrLowRow = new QHBoxLayout;
    triggerHighOrLowRow->addWidget(triggerHighOrLowLabel);
    triggerHighOrLowRow->addWidget(triggerHighOrLow);

    QHBoxLayout *postTriggerDelayRow = new QHBoxLayout;
    postTriggerDelayRow->addWidget(postTriggerDelayLabel);
    postTriggerDelayRow->addStretch();
    postTriggerDelayRow->addWidget(postTriggerDelay);

    QVBoxLayout *triggerLayout = new QVBoxLayout;
    triggerLayout->addLayout(enablePulseRow);
    triggerLayout->addLayout(triggerSourceRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(triggerEdgeOrLevelRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(triggerHighOrLowRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(postTriggerDelayRow);
    trigger->setLayout(triggerLayout);

    //pulse train information widgets' layout
    QHBoxLayout *pulseDurationRow = new QHBoxLayout;
    pulseDurationRow->addWidget(pulseDurationLabel);
    pulseDurationRow->addStretch();
    pulseDurationRow->addWidget(pulseDuration);

    QHBoxLayout *pulseRepetitionRow = new QHBoxLayout;
    pulseRepetitionRow->addWidget(pulseRepetitionLabel);
    pulseRepetitionRow->addStretch();
    pulseRepetitionRow->addWidget(pulseRepetition);

    QHBoxLayout *numPulsesRow = new QHBoxLayout;
    numPulsesRow->addWidget(numPulsesLabel);
    numPulsesRow->addStretch();
    numPulsesRow->addWidget(numPulses);

    QHBoxLayout *pulseTrainPeriodRow = new QHBoxLayout;
    pulseTrainPeriodRow->addWidget(pulseTrainPeriodLabel);
    pulseTrainPeriodRow->addStretch();
    pulseTrainPeriodRow->addWidget(pulseTrainPeriod);

    QHBoxLayout *pulseTrainFrequencyRow = new QHBoxLayout;
    pulseTrainFrequencyRow->addWidget(pulseTrainFrequencyLabel);

    QHBoxLayout *refractoryPeriodRow = new QHBoxLayout;
    refractoryPeriodRow->addWidget(refractoryPeriodLabel);
    refractoryPeriodRow->addStretch();
    refractoryPeriodRow->addWidget(refractoryPeriod);

    QVBoxLayout *pulseLayout = new QVBoxLayout;
    pulseLayout->addLayout(pulseDurationRow);
    pulseLayout->addLayout(pulseRepetitionRow);
    pulseLayout->addLayout(numPulsesRow);
    pulseLayout->addLayout(pulseTrainPeriodRow);
    pulseLayout->addLayout(pulseTrainFrequencyRow);
    pulseLayout->addLayout(refractoryPeriodRow);
    pulse->setLayout(pulseLayout);

    //first Column
    QVBoxLayout *firstColumn = new QVBoxLayout;
    firstColumn->addWidget(trigger);
    firstColumn->addStretch();

    //second Column
    QVBoxLayout *secondColumn = new QVBoxLayout;
    secondColumn->addWidget(pulse);

    //final Row
    QHBoxLayout *finalRow = new QHBoxLayout;
    finalRow->addWidget(buttonBox);

    //main Layout
    QHBoxLayout *columns = new QHBoxLayout;
    columns->addLayout(firstColumn);
    columns->addLayout(secondColumn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(digFigure);
    mainLayout->addLayout(columns);
    mainLayout->addLayout(finalRow);
    setLayout(mainLayout);

    setWindowTitle("Digital Output Parameters: " + nativeChannelName + " (" + customChannelName + ")");
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}


void DigOutDialog::loadParameters(StimParameters *parameters)
{
    //set zero values initially to bring all ranges to microseconds, so values can be set properly
    postTriggerDelay->setValue(0);
    pulseDuration->setValue(0);
    pulseTrainPeriod->setValue(0);
    refractoryPeriod->setValue(0);

    //load parameters
    enablePulse->setChecked(parameters->enabled);
    triggerSource->setCurrentIndex(parameters->triggerSource);
    triggerEdgeOrLevel->setCurrentIndex(parameters->triggerEdgeOrLevel);
    triggerHighOrLow->setCurrentIndex(parameters->triggerHighOrLow);
    postTriggerDelay->loadValue(parameters->postTriggerDelay);
    pulseDuration->loadValue(parameters->firstPhaseDuration);
    pulseRepetition->setCurrentIndex(parameters->pulseOrTrain);
    numPulses->setValue(parameters->numberOfStimPulses);
    pulseTrainPeriod->loadValue(parameters->pulseTrainPeriod);
    refractoryPeriod->loadValue(parameters->refractoryPeriod);

    //calculate frequency so that the first label that is displayed corresponds to the loaded parameters
    calculatePulseTrainFrequency();

    //constrain pulse period so that when first loaded, the widget has a proper minimum
    constrainPulseTrainPeriod();
}

/* Public slot that saves the values from the dialog box widgets into the parameters object, and closes the window */
void DigOutDialog::accept()
{
    //save the values of the parameters from the dialog box into the object
    parameters->enabled = enablePulse->isChecked();
    parameters->triggerSource = (StimParameters::TriggerSources) triggerSource->currentIndex();
    parameters->triggerEdgeOrLevel = (StimParameters::TriggerEdgeOrLevels) triggerEdgeOrLevel->currentIndex();
    parameters->triggerHighOrLow = (StimParameters::TriggerHighOrLows) triggerHighOrLow->currentIndex();
    parameters->postTriggerDelay = postTriggerDelay->getTrueValue();
    parameters->firstPhaseDuration = pulseDuration->getTrueValue();
    parameters->pulseOrTrain = (StimParameters::PulseOrTrainValues) pulseRepetition->currentIndex();
    parameters->numberOfStimPulses = numPulses->value();
    parameters->pulseTrainPeriod = pulseTrainPeriod->getTrueValue();
    parameters->refractoryPeriod = refractoryPeriod->getTrueValue();

    done(Accepted);
}


void DigOutDialog::notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus)
{
//emit signals when a widget loses focus
    if (lostFocus == pulseDuration->pointer()) {
        emit highlightFirstPhaseDuration(false);
    }
    if (lostFocus == postTriggerDelay->pointer()) {
        emit highlightPostTriggerDelay(false);
    }
    if (lostFocus == pulseTrainPeriod->pointer()) {
        emit highlightPulseTrainPeriod(false);
    }
    if (lostFocus == refractoryPeriod->pointer()) {
        emit highlightRefractoryPeriod(false);
    }

    //emit signals when a widget gains focus
    if (gainedFocus == pulseDuration->pointer()) {
        emit highlightFirstPhaseDuration(true);
    }
    if (gainedFocus == postTriggerDelay->pointer()) {
        emit highlightPostTriggerDelay(true);
    }
    if (gainedFocus == pulseTrainPeriod->pointer()) {
        emit highlightPulseTrainPeriod(true);
    }
    if (gainedFocus == refractoryPeriod->pointer()) {
        emit highlightRefractoryPeriod(true);
    }
}


/* Private slot that calculates the frequency of the pulse train, given the user-selected period */
void DigOutDialog::calculatePulseTrainFrequency()
{
    double frequency;

    if (pulseTrainPeriod->getTrueValue() == 0)
        pulseTrainFrequencyLabel->setText(tr("Pulse Train Frequency: -"));

    else if (pulseTrainPeriod->getTrueValue() < 1000)
    {
        frequency = 1 / pulseTrainPeriod->getTrueValue();
        pulseTrainFrequencyLabel->setText("Pulse Train Frequency: " + QString::number(frequency * 1000, 'f', 2) + " kHz");
    }

    else
    {
        frequency = 1 / pulseTrainPeriod->getTrueValue();
        pulseTrainFrequencyLabel->setText("Pulse Train Frequency: " + QString::number(frequency * 1000000, 'f', 2) + " Hz");
    }
}


/* Private slot that rounds all TimeSpinBox values to the nearest integer multiple of timestep_us when the user has finished editing them */
void DigOutDialog::roundTimeInputs()
{
    /* for all time inputs, if they are not divisible by timestep_us, round them to the nearest multiple of timestep_us */

    //postTriggerDelay
    postTriggerDelay->round();

    //pulseDuration
    pulseDuration->round();

    //pulseTrainPeriod
    pulseTrainPeriod->round();

    //refractoryPeriod
    refractoryPeriod->round();

}

/* Private slot that looks at each widget individually and the state of its control widgets to see if it should be enabled or disabled */
void DigOutDialog::enableWidgets()
{
    //boolean conditional statements reflect if each widget should be enabled

    /* Trigger Group Box */
    triggerSourceLabel->setEnabled(enablePulse->isChecked());
    triggerSource->setEnabled(enablePulse->isChecked());
    triggerEdgeOrLevel->setEnabled(enablePulse->isChecked());
    triggerHighOrLow->setEnabled(enablePulse->isChecked());
    postTriggerDelayLabel->setEnabled(enablePulse->isChecked());
    postTriggerDelay->setEnabled(enablePulse->isChecked());

    /* Pulse Group Box */
    pulseDurationLabel->setEnabled(enablePulse->isChecked());
    pulseDuration->setEnabled(enablePulse->isChecked());
    pulseRepetitionLabel->setEnabled(enablePulse->isChecked());
    pulseRepetition->setEnabled(enablePulse->isChecked());
    numPulsesLabel->setEnabled(enablePulse->isChecked() && pulseRepetition->currentIndex() == StimParameters::PulseTrain);
    numPulses->setEnabled(enablePulse->isChecked() && pulseRepetition->currentIndex() == StimParameters::PulseTrain);
    pulseTrainPeriodLabel->setEnabled(enablePulse->isChecked() && pulseRepetition->currentIndex() == StimParameters::PulseTrain);
    pulseTrainPeriod->setEnabled(enablePulse->isChecked() && pulseRepetition->currentIndex() == StimParameters::PulseTrain);
    pulseTrainFrequencyLabel->setEnabled(enablePulse->isChecked() && pulseRepetition->currentIndex() == StimParameters::PulseTrain);
    refractoryPeriodLabel->setEnabled(enablePulse->isChecked());
    refractoryPeriod->setEnabled(enablePulse->isChecked());
}


/* Private slot that constrains pulseTrainPeriod's lowest possible value to the current value of pulseDuration */
void DigOutDialog::constrainPulseTrainPeriod()
{
    pulseTrainPeriod->setTrueMinimum(pulseDuration->getTrueValue());
}
