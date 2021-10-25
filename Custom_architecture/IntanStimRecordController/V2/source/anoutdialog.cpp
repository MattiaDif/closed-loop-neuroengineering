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

#include "anoutdialog.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

/* Analog Out Dialog is a dialog box that allows the user to view and change Analog Output parameters */

#include "timespinbox.h"
#include "voltagespinbox.h"
#include "anoutfigure.h"
#include <cmath>

AnOutDialog::AnOutDialog(StimParameters *parameter, QString nativeChannelName, QString customChannelName, double timestep_us, QWidget *parent)
    : QDialog(parent)
{
    //set the parameter, timestep objects, so that they can be accessed by other functions in anoutdialog.cpp
    parameters = parameter;
    timestep = timestep_us;

    //create a new AnOutFigure
    anOutFigure = new AnOutFigure(parameters, this);

    //create stimulation waveform widgets
    stimWaveForm = new QGroupBox(tr("Stimulation Waveform"));

    stimShapeLabel = new QLabel(tr("Stimulation Shape: "));
    stimShape = new QComboBox();
    QStringList stimShapes;
    stimShapes << "Biphasic" << "Biphasic with Delay" << "Triphasic" << "Monophasic";
    stimShape->addItems(stimShapes);

    stimPolarityLabel = new QLabel(tr("Stimulation Polarity: "));
    stimPolarity = new QComboBox();
    stimPolarity->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    QStringList stimPolarities;
    stimPolarities << "Negative Voltage First" << "Positive Voltage First";
    stimPolarity->addItems(stimPolarities);

    firstPhaseDurationLabel = new QLabel(tr("First Phase Duration (D1): "));
    firstPhaseDuration = new TimeSpinBox(timestep_us);
    firstPhaseDuration->setRange(0, 5000);

    secondPhaseDurationLabel = new QLabel(tr("Second Phase Duration (D2): "));
    secondPhaseDuration = new TimeSpinBox(timestep_us);
    secondPhaseDuration->setRange(0, 5000);

    interphaseDelayLabel = new QLabel(tr("Interphase Delay (DP): "));
    interphaseDelay = new TimeSpinBox(timestep_us);
    interphaseDelay->setRange(0, 5000);

    firstPhaseAmplitudeLabel = new QLabel(tr("First Phase Amplitude (A1): "));
    firstPhaseAmplitude = new VoltageSpinBox();
    firstPhaseAmplitude->setRange(0, 10.24);

    secondPhaseAmplitudeLabel = new QLabel(tr("Second Phase Amplitude (A2): "));
    secondPhaseAmplitude = new VoltageSpinBox();
    secondPhaseAmplitude->setRange(0, 10.24);

    baselineVoltageLabel = new QLabel(tr("Baseline Voltage: "));
    baselineVoltage = new VoltageSpinBox();
    baselineVoltage->setRange(-10.24, 10.24);

    //create trigger information widgets
    trigger = new QGroupBox(tr("Trigger"));

    enableStim = new QCheckBox(tr("Enable"));

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

    //triggerEdgeOrLevelLabel = new QLabel(tr("Stimulation is: "));
    triggerEdgeOrLevelLabel = new QLabel(tr(" "));
    triggerEdgeOrLevel = new QComboBox();
    QStringList triggerEdgeOrLevels;
    triggerEdgeOrLevels << "Edge Triggered" << "Level Triggered";
    triggerEdgeOrLevel->addItems(triggerEdgeOrLevels);

    //triggerHighOrLowLabel = new QLabel(tr("Stimulation will: "));
    triggerHighOrLowLabel = new QLabel(tr(" "));
    triggerHighOrLow = new QComboBox();
    QStringList triggerHighOrLows;
    triggerHighOrLows << "Trigger on High" << "Trigger on Low";
    triggerHighOrLow->addItems(triggerHighOrLows);

    postTriggerDelayLabel = new QLabel(tr("Post Trigger Delay: "));
    postTriggerDelay = new TimeSpinBox(timestep_us);
    postTriggerDelay->setRange(0, 500000);

    //create pulse train information widgets
    pulseTrain = new QGroupBox(tr("Pulse Train"));

    pulseOrTrainLabel = new QLabel(tr("Pulse Repetition: "));
    pulseOrTrain = new QComboBox();
    QStringList pulseOrTrains;
    pulseOrTrains << "Single Stim Pulse" << "Stim Pulse Train";
    pulseOrTrain->addItems(pulseOrTrains);

    numberOfStimPulsesLabel = new QLabel(tr("Number of Stim Pulses"));
    numberOfStimPulses = new QSpinBox();
    numberOfStimPulses->setRange(2, 256);

    pulseTrainPeriodLabel = new QLabel(tr("Pulse Train Period: "));
    pulseTrainPeriod = new TimeSpinBox(timestep_us);
    pulseTrainPeriod->setRange(0, 1000000);

    pulseTrainFrequencyLabel = new QLabel();

    refractoryPeriodLabel = new QLabel(tr("Post-Stim Refractory Period: "));
    refractoryPeriod = new TimeSpinBox(timestep_us);
    refractoryPeriod->setRange(0, 1000000);

    //create and connect 'OK' and 'Cancel' buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    //connect internal signals and slots
    connect(enableStim, SIGNAL(stateChanged(int)), this, SLOT(enableWidgets()));
    connect(pulseOrTrain, SIGNAL(currentIndexChanged(int)), this, SLOT(enableWidgets()));
    connect(stimShape, SIGNAL(currentIndexChanged(int)), this, SLOT(enableWidgets()));

    connect(stimShape, SIGNAL(currentIndexChanged(int)), this, SIGNAL(minimumPeriodChanged()));
    connect(firstPhaseDuration, SIGNAL(valueChanged(double)), this, SIGNAL(minimumPeriodChanged()));
    connect(secondPhaseDuration, SIGNAL(valueChanged(double)), this, SIGNAL(minimumPeriodChanged()));

    connect(pulseTrainPeriod, SIGNAL(valueChanged(double)), this, SLOT(calculatePulseTrainFrequency()));
    connect(this, SIGNAL(minimumPeriodChanged()), this, SLOT(constrainPulseTrainPeriod()));
    connect(firstPhaseDuration, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(secondPhaseDuration, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(interphaseDelay, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(interphaseDelay, SIGNAL(valueChanged(double)), this, SIGNAL(minimumPeriodChanged()));
    connect(postTriggerDelay, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(pulseTrainPeriod, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(refractoryPeriod, SIGNAL(editingFinished()), this, SLOT(roundTimeInputs()));
    connect(firstPhaseAmplitude, SIGNAL(editingFinished()), this, SLOT(roundVoltageInputs()));
    connect(secondPhaseAmplitude, SIGNAL(editingFinished()), this, SLOT(roundVoltageInputs()));
    connect(baselineVoltage, SIGNAL(editingFinished()), this, SLOT(roundVoltageInputs()));
    connect(stimShape, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMonophasicAndPositive()));
    connect(stimPolarity, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMonophasicAndPositive()));
    connect(enableStim, SIGNAL(toggled(bool)), anOutFigure, SLOT(highlightStimTrace(bool)));

    //connect signals to anOutFigure's non-highlight slots
    connect(stimShape, SIGNAL(currentIndexChanged(int)), anOutFigure, SLOT(updateStimShape(int)));
    connect(stimPolarity, SIGNAL(currentIndexChanged(int)), anOutFigure, SLOT(updateStimPolarity(int)));
    connect(pulseOrTrain, SIGNAL(currentIndexChanged(int)), anOutFigure, SLOT(updatePulseOrTrain(int)));
    connect(this, SIGNAL(monophasicAndPositiveSignalChanged(bool)), anOutFigure, SLOT(updateMonophasicAndPositive(bool)));

    //connect signals to anOutFigure's highlight slots
    connect(this, SIGNAL(highlightBaselineVoltage(bool)), anOutFigure, SLOT(highlightBaselineVoltage(bool)));
    connect(this, SIGNAL(highlightFirstPhaseDuration(bool)), anOutFigure, SLOT(highlightFirstPhaseDuration(bool)));
    connect(this, SIGNAL(highlightSecondPhaseDuration(bool)), anOutFigure, SLOT(highlightSecondPhaseDuration(bool)));
    connect(this, SIGNAL(highlightInterphaseDelay(bool)), anOutFigure, SLOT(highlightInterphaseDelay(bool)));
    connect(this, SIGNAL(highlightFirstPhaseAmplitude(bool)), anOutFigure, SLOT(highlightFirstPhaseAmplitude(bool)));
    connect(this, SIGNAL(highlightSecondPhaseAmplitude(bool)), anOutFigure, SLOT(highlightSecondPhaseAmplitude(bool)));
    connect(this, SIGNAL(highlightPostTriggerDelay(bool)), anOutFigure, SLOT(highlightPostTriggerDelay(bool)));
    connect(this, SIGNAL(highlightPulseTrainPeriod(bool)), anOutFigure, SLOT(highlightPulseTrainPeriod(bool)));
    connect(this, SIGNAL(highlightRefractoryPeriod(bool)), anOutFigure, SLOT(highlightRefractoryPeriod(bool)));

    //update widgets' states to be different than the default states, so state changes are detected properly

    stimShape->setCurrentIndex(StimParameters::Triphasic);
    stimPolarity->setCurrentIndex(StimParameters::PositiveFirst);
    firstPhaseDuration->setValue(1);
    secondPhaseDuration->setValue(1);
    interphaseDelay->setValue(1);
    firstPhaseAmplitude->setValue(1);
    secondPhaseAmplitude->setValue(1);
    baselineVoltage->setValue(1);
    enableStim->setChecked(true);
    triggerSource->setCurrentIndex(StimParameters::AnalogIn1);
    triggerEdgeOrLevel->setCurrentIndex(StimParameters::Level);
    triggerHighOrLow->setCurrentIndex(StimParameters::Low);
    postTriggerDelay->setValue(2);
    pulseOrTrain->setCurrentIndex(StimParameters::SinglePulse);
    numberOfStimPulses->setValue(2);
    pulseTrainPeriod->setValue(1);
    refractoryPeriod->setValue(1);

    //update dialog's state based on structParameters
    loadParameters(parameters);

    //stimulation waveform widgets' layout
    QHBoxLayout *stimShapeRow = new QHBoxLayout;
    stimShapeRow->addWidget(stimShapeLabel);
    stimShapeRow->addStretch();
    stimShapeRow->addWidget(stimShape);

    QHBoxLayout *stimPolarityRow = new QHBoxLayout;
    stimPolarityRow->addWidget(stimPolarityLabel);
    stimPolarityRow->addStretch();
    stimPolarityRow->addWidget(stimPolarity);

    QHBoxLayout *firstPhaseDurationRow = new QHBoxLayout;
    firstPhaseDurationRow->addWidget(firstPhaseDurationLabel);
    firstPhaseDurationRow->addStretch();
    firstPhaseDurationRow->addWidget(firstPhaseDuration);

    QHBoxLayout *secondPhaseDurationRow = new QHBoxLayout;
    secondPhaseDurationRow->addWidget(secondPhaseDurationLabel);
    secondPhaseDurationRow->addStretch();
    secondPhaseDurationRow->addWidget(secondPhaseDuration);

    QHBoxLayout *interphaseDelayRow = new QHBoxLayout;
    interphaseDelayRow->addWidget(interphaseDelayLabel);
    interphaseDelayRow->addStretch();
    interphaseDelayRow->addWidget(interphaseDelay);

    QHBoxLayout *firstPhaseAmplitudeRow = new QHBoxLayout;
    firstPhaseAmplitudeRow->addWidget(firstPhaseAmplitudeLabel);
    firstPhaseAmplitudeRow->addStretch();
    firstPhaseAmplitudeRow->addWidget(firstPhaseAmplitude);

    QHBoxLayout *secondPhaseAmplitudeRow = new QHBoxLayout;
    secondPhaseAmplitudeRow->addWidget(secondPhaseAmplitudeLabel);
    secondPhaseAmplitudeRow->addStretch();
    secondPhaseAmplitudeRow->addWidget(secondPhaseAmplitude);

    QHBoxLayout *baselineVoltageRow = new QHBoxLayout;
    baselineVoltageRow->addWidget(baselineVoltageLabel);
    baselineVoltageRow->addStretch();
    baselineVoltageRow->addWidget(baselineVoltage);

    QVBoxLayout *stimWaveFormLayout = new QVBoxLayout;
    stimWaveFormLayout->addLayout(stimShapeRow);
    stimWaveFormLayout->addLayout(stimPolarityRow);
    stimWaveFormLayout->addLayout(firstPhaseDurationRow);
    stimWaveFormLayout->addLayout(secondPhaseDurationRow);
    stimWaveFormLayout->addLayout(interphaseDelayRow);
    stimWaveFormLayout->addLayout(firstPhaseAmplitudeRow);
    stimWaveFormLayout->addLayout(secondPhaseAmplitudeRow);
    stimWaveFormLayout->addLayout(baselineVoltageRow);
    stimWaveFormLayout->addStretch();
    stimWaveForm->setLayout(stimWaveFormLayout);

    //trigger information widgets' layout
    QHBoxLayout *enableStimRow = new QHBoxLayout;
    enableStimRow->addWidget(enableStim);

    QHBoxLayout *triggerSourceRow = new QHBoxLayout;
    triggerSourceRow->addWidget(triggerSourceLabel);
    triggerSourceRow->addStretch();
    triggerSourceRow->addWidget(triggerSource);

    QHBoxLayout *triggerEdgeOrLevelRow = new QHBoxLayout;
    triggerEdgeOrLevelRow->addWidget(triggerEdgeOrLevelLabel);
    triggerEdgeOrLevelRow->addStretch();
    triggerEdgeOrLevelRow->addWidget(triggerEdgeOrLevel);

    QHBoxLayout *triggerHighOrLowRow = new QHBoxLayout;
    triggerHighOrLowRow->addWidget(triggerHighOrLowLabel);
    triggerHighOrLowRow->addStretch();
    triggerHighOrLowRow->addWidget(triggerHighOrLow);

    QHBoxLayout *postTriggerDelayRow = new QHBoxLayout;
    postTriggerDelayRow->addWidget(postTriggerDelayLabel);
    postTriggerDelayRow->addStretch();
    postTriggerDelayRow->addWidget(postTriggerDelay);

    QVBoxLayout *triggerLayout = new QVBoxLayout;
    triggerLayout->addLayout(enableStimRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(triggerSourceRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(triggerEdgeOrLevelRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(triggerHighOrLowRow);
    triggerLayout->addStretch();
    triggerLayout->addLayout(postTriggerDelayRow);
    trigger->setLayout(triggerLayout);

    //pulse train information widgets' layout
    QHBoxLayout *pulseOrTrainRow = new QHBoxLayout;
    pulseOrTrainRow->addWidget(pulseOrTrainLabel);
    pulseOrTrainRow->addStretch();
    pulseOrTrainRow->addWidget(pulseOrTrain);

    QHBoxLayout *numberOfStimPulsesRow = new QHBoxLayout;
    numberOfStimPulsesRow->addWidget(numberOfStimPulsesLabel);
    numberOfStimPulsesRow->addStretch();
    numberOfStimPulsesRow->addWidget(numberOfStimPulses);

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

    QVBoxLayout *pulseTrainLayout = new QVBoxLayout;
    pulseTrainLayout->addLayout(pulseOrTrainRow);
    pulseTrainLayout->addLayout(numberOfStimPulsesRow);
    pulseTrainLayout->addLayout(pulseTrainPeriodRow);
    pulseTrainLayout->addLayout(pulseTrainFrequencyRow);
    pulseTrainLayout->addLayout(refractoryPeriodRow);
    pulseTrain->setLayout(pulseTrainLayout);

    //first Column
    QVBoxLayout *firstColumn = new QVBoxLayout;
    firstColumn->addWidget(trigger);
    firstColumn->addStretch();

    //second Column
    QVBoxLayout *secondColumn = new QVBoxLayout;
    secondColumn->addWidget(pulseTrain);
    secondColumn->addStretch();

    //third Column
    QVBoxLayout *thirdColumn = new QVBoxLayout;
    thirdColumn->addWidget(stimWaveForm);
    thirdColumn->addStretch();

    //final Row
    QHBoxLayout *finalRow = new QHBoxLayout;
    finalRow->addWidget(buttonBox);

    //main Layout
    QHBoxLayout *columns = new QHBoxLayout;
    columns->addLayout(firstColumn);
    columns->addLayout(secondColumn);
    columns->addLayout(thirdColumn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(anOutFigure);
    mainLayout->addLayout(columns);
    mainLayout->addStretch();
    mainLayout->addLayout(finalRow);
    setLayout(mainLayout);

    setWindowTitle("Analog Output Parameters: " + nativeChannelName + " (" + customChannelName + ")");
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

AnOutDialog::~AnOutDialog()
{

}

/* Public member function that loads the widgets in StimParamDialog with the values from StimParameters */
void AnOutDialog::loadParameters(StimParameters *parameters)
{
    //set zero values initially to bring all ranges to microseconds, so values can be set properly
    firstPhaseDuration->setValue(0);
    secondPhaseDuration->setValue(0);
    interphaseDelay->setValue(0);
    postTriggerDelay->setValue(0);
    pulseTrainPeriod->setValue(0);
    refractoryPeriod->setValue(0);
    firstPhaseAmplitude->setValue(0);
    secondPhaseAmplitude->setValue(0);
    baselineVoltage->setValue(0);

    //load parameters that affect the ranges of other parameters first:

    //load the rest of the parameters
    stimShape->setCurrentIndex(parameters->stimShape);
    stimPolarity->setCurrentIndex(parameters->stimPolarity);
    firstPhaseDuration->loadValue(parameters->firstPhaseDuration);
    secondPhaseDuration->loadValue(parameters->secondPhaseDuration);
    interphaseDelay->loadValue(parameters->interphaseDelay);
    firstPhaseAmplitude->setValue(parameters->firstPhaseAmplitude);
    secondPhaseAmplitude->setValue(parameters->secondPhaseAmplitude);
    baselineVoltage->setValue(parameters->baselineVoltage);
    enableStim->setChecked(parameters->enabled);
    triggerSource->setCurrentIndex(parameters->triggerSource);
    triggerEdgeOrLevel->setCurrentIndex(parameters->triggerEdgeOrLevel);
    triggerHighOrLow->setCurrentIndex(parameters->triggerHighOrLow);
    postTriggerDelay->loadValue(parameters->postTriggerDelay);
    pulseOrTrain->setCurrentIndex(parameters->pulseOrTrain);
    numberOfStimPulses->setValue(parameters->numberOfStimPulses);
    pulseTrainPeriod->loadValue(parameters->pulseTrainPeriod);
    refractoryPeriod->loadValue(parameters->refractoryPeriod);

    //constrain time periods so that the first values the user sees are valid
    constrainPulseTrainPeriod();

    //calculate frequency so that the first labels that are displayed correspond to the loaded parameters
    calculatePulseTrainFrequency();
}

/* Public slot that saves the values from the dialog box widgets into the parameters object, and closes the window */
void AnOutDialog::accept()
{
    //save the values of the parameters from the dialog box into the object
    parameters->stimShape = (StimParameters::StimShapeValues) stimShape->currentIndex();
    parameters->stimPolarity = (StimParameters::StimPolarityValues) stimPolarity->currentIndex();
    parameters->firstPhaseDuration = firstPhaseDuration->getTrueValue();
    parameters->secondPhaseDuration = secondPhaseDuration->getTrueValue();
    parameters->interphaseDelay = interphaseDelay->getTrueValue();
    parameters->firstPhaseAmplitude = firstPhaseAmplitude->getValue();
    parameters->secondPhaseAmplitude = secondPhaseAmplitude->getValue();
    parameters->baselineVoltage = baselineVoltage->getValue();
    parameters->enabled = enableStim->isChecked();
    parameters->triggerSource = (StimParameters::TriggerSources) triggerSource->currentIndex();
    parameters->triggerEdgeOrLevel = (StimParameters::TriggerEdgeOrLevels) triggerEdgeOrLevel->currentIndex();
    parameters->triggerHighOrLow = (StimParameters::TriggerHighOrLows) triggerHighOrLow->currentIndex();
    parameters->postTriggerDelay = postTriggerDelay->getTrueValue();
    parameters->pulseOrTrain = (StimParameters::PulseOrTrainValues) pulseOrTrain->currentIndex();
    parameters->numberOfStimPulses = numberOfStimPulses->value();
    parameters->pulseTrainPeriod = pulseTrainPeriod->getTrueValue();
    parameters->refractoryPeriod = refractoryPeriod->getTrueValue();

    //close the window
    done(Accepted);
}


/* Public slot that emits signals when widgets that can be highlighted gain or lose focus */
void AnOutDialog::notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus)
{
    //emit signals when a widget loses focus
    if (lostFocus == firstPhaseDuration->pointer()) {
        emit highlightFirstPhaseDuration(false);
    }
    if (lostFocus == secondPhaseDuration->pointer()) {
        emit highlightSecondPhaseDuration(false);
    }
    if (lostFocus == interphaseDelay->pointer()) {
        emit highlightInterphaseDelay(false);
    }
    if (lostFocus == firstPhaseAmplitude->pointer()) {
        emit highlightFirstPhaseAmplitude(false);
    }
    if (lostFocus == secondPhaseAmplitude->pointer()) {
        emit highlightSecondPhaseAmplitude(false);
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
    if (lostFocus == baselineVoltage->pointer()) {
        emit highlightBaselineVoltage(false);
    }

    //emit signals when a widget gains focus
    if (gainedFocus == firstPhaseDuration->pointer()) {
        emit highlightFirstPhaseDuration(true);
    }
    if (gainedFocus == secondPhaseDuration->pointer()) {
        emit highlightSecondPhaseDuration(true);
    }
    if (gainedFocus == interphaseDelay->pointer()) {
        emit highlightInterphaseDelay(true);
    }
    if (gainedFocus == firstPhaseAmplitude->pointer()) {
        emit highlightFirstPhaseAmplitude(true);
    }
    if (gainedFocus == secondPhaseAmplitude->pointer()) {
        emit highlightSecondPhaseAmplitude(true);
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
    if (gainedFocus == baselineVoltage->pointer()) {
        emit highlightBaselineVoltage(true);
    }
}

/* Private slot that looks at each widget individually and the state of its control widgets to see if it should be enabled or disabled */
void AnOutDialog::enableWidgets()
{
    //boolean conditional statements reflect if each widget should be enabled

    /* Trigger Group Box */
    triggerSourceLabel->setEnabled(enableStim->isChecked());
    triggerSource->setEnabled(enableStim->isChecked());
    triggerEdgeOrLevel->setEnabled(enableStim->isChecked());
    triggerHighOrLow->setEnabled(enableStim->isChecked());
    postTriggerDelayLabel->setEnabled(enableStim->isChecked());
    postTriggerDelay->setEnabled(enableStim->isChecked());

    /* Pulse Train Group Box */
    pulseOrTrainLabel->setEnabled(enableStim->isChecked());
    pulseOrTrain->setEnabled(enableStim->isChecked());
    numberOfStimPulsesLabel->setEnabled(enableStim->isChecked() && pulseOrTrain->currentIndex() == StimParameters::PulseTrain);
    numberOfStimPulses->setEnabled(enableStim->isChecked() && pulseOrTrain->currentIndex() == StimParameters::PulseTrain);
    pulseTrainPeriodLabel->setEnabled(enableStim->isChecked() && pulseOrTrain->currentIndex() == StimParameters::PulseTrain);
    pulseTrainPeriod->setEnabled(enableStim->isChecked() && pulseOrTrain->currentIndex() == StimParameters::PulseTrain);
    pulseTrainFrequencyLabel->setEnabled(enableStim->isChecked() && pulseOrTrain->currentIndex() == StimParameters::PulseTrain);
    refractoryPeriodLabel->setEnabled(enableStim->isChecked());
    refractoryPeriod->setEnabled(enableStim->isChecked());

    /* Stimulation Waveform */
    stimShapeLabel->setEnabled(enableStim->isChecked());
    stimShape->setEnabled(enableStim->isChecked());
    stimPolarityLabel->setEnabled(enableStim->isChecked());
    stimPolarity->setEnabled(enableStim->isChecked());
    if (stimShape->currentIndex() == StimParameters::Monophasic)
    {
        stimPolarity->setItemText(StimParameters::NegativeFirst, "Negative Voltage");
        stimPolarity->setItemText(StimParameters::PositiveFirst, "Positive Voltage");
    }
    else
    {
        stimPolarity->setItemText(StimParameters::NegativeFirst, "Negative Voltage First");
        stimPolarity->setItemText(StimParameters::PositiveFirst, "Positive Voltage First");
    }
    firstPhaseDurationLabel->setEnabled(enableStim->isChecked());
    firstPhaseDuration->setEnabled(enableStim->isChecked());
    secondPhaseDurationLabel->setEnabled(enableStim->isChecked() && stimShape->currentIndex() != StimParameters::Monophasic);
    secondPhaseDuration->setEnabled(enableStim->isChecked() && stimShape->currentIndex() != StimParameters::Monophasic);
    interphaseDelayLabel->setEnabled(enableStim->isChecked() && stimShape->currentIndex() == StimParameters::BiphasicWithInterphaseDelay);
    interphaseDelay->setEnabled(enableStim->isChecked() && stimShape->currentIndex() == StimParameters::BiphasicWithInterphaseDelay);
    firstPhaseAmplitudeLabel->setEnabled(enableStim->isChecked());
    firstPhaseAmplitude->setEnabled(enableStim->isChecked());
    secondPhaseAmplitudeLabel->setEnabled(enableStim->isChecked() && stimShape->currentIndex() != StimParameters::Monophasic);
    secondPhaseAmplitude->setEnabled(enableStim->isChecked() && stimShape->currentIndex() != StimParameters::Monophasic);
    baselineVoltageLabel->setEnabled(enableStim->isChecked());
    baselineVoltage->setEnabled(enableStim->isChecked());

    /* Reset Text for First Phase Labels */
    if (stimShape->currentIndex() != StimParameters::Triphasic)
    {
        firstPhaseDurationLabel->setText(tr("First Phase Duration (D1): "));
        firstPhaseAmplitudeLabel->setText(tr("First Phase Amplitude (A1): "));
    }
    else
    {
        firstPhaseDurationLabel->setText(tr("First/Third Phase Duration (D1): "));
        firstPhaseAmplitudeLabel->setText(tr("First/Third Phase Amplitude (A1): "));
    }
}

/* Private slot that calculates the frequency of the pulse train, given the user-selected period */
void AnOutDialog::calculatePulseTrainFrequency()
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

/* Private slot that constrains pulseTrainPeriod's lowest possible value to the sum of the durations of active phases */
void AnOutDialog::constrainPulseTrainPeriod()
{
    double minimum;
    //if biphasic
    if (stimShape->currentIndex() == StimParameters::Biphasic)
    {
        //minimum equals D1 + D2
        minimum = firstPhaseDuration->getTrueValue() + secondPhaseDuration->getTrueValue();
    }

    //if biphasic with interphase delay
    else if (stimShape->currentIndex() == StimParameters::BiphasicWithInterphaseDelay)
    {
        //minimum equals D1 + D2 + D_int
        minimum = firstPhaseDuration->getTrueValue() + secondPhaseDuration->getTrueValue() + interphaseDelay->getTrueValue();
    }

    //if triphasic
    else if (stimShape->currentIndex() == StimParameters::Triphasic)
    {
        //minimum equals 2*D1 + D2
        minimum = (2 * firstPhaseDuration->getTrueValue()) + secondPhaseDuration->getTrueValue();
    }

    pulseTrainPeriod->setTrueMinimum(minimum);
}


/* Private slot that rounds all TimeSpinBox values to the nearest integer multiple of timestep_us when the user has finished editing them */
void AnOutDialog::roundTimeInputs()
{
    /* for all time inputs, if they are not divisible by timestep_us, round them to the nearest multiple of timestep_us */

    //firstPhaseDuration
    firstPhaseDuration->round();

    //secondPhaseDuration
    secondPhaseDuration->round();

    //interphaseDelay
    interphaseDelay->round();

    //postTriggerDelay
    postTriggerDelay->round();

    //pulseTrainPeriod
    pulseTrainPeriod->round();

    //refractoryPeriod
    refractoryPeriod->round();
}

/* Private slot that rounds all VoltageSpinBox values to the nearest integer multiple of timestep_us when the user has finished editing them */
void AnOutDialog::roundVoltageInputs()
{
    /* for all voltage inputs, if they are not divisible by voltagestep, round them to the nearest multiple of voltagestep */

    //firstPhaseAmplitude
    firstPhaseAmplitude->round();

    //secondPhaseAmplitude
    secondPhaseAmplitude->round();

    //baselineVoltage
    baselineVoltage->round();
}

void AnOutDialog::updateMonophasicAndPositive()
{
    emit monophasicAndPositiveSignalChanged(stimShape->currentIndex() == StimParameters::Monophasic && stimPolarity->currentIndex() == StimParameters::PositiveFirst);
}
