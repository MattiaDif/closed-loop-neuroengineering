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

#ifndef ANOUTDIALOG_H
#define ANOUTDIALOG_H

#include <QDialog>

#include "stimparameters.h"

class QDialogButtonBox;
class QWidget;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class QLabel;
class QGroupBox;
class QCheckBox;
class StimParameters;
class TimeSpinBox;
class VoltageSpinBox;
class AnOutFigure;

class AnOutDialog : public QDialog
{
    Q_OBJECT

public:
    AnOutDialog(StimParameters *parameters, QString nativeChannelName, QString customChannelName, double timestep_us, QWidget *parent = 0);
    ~AnOutDialog();
    void loadParameters(StimParameters* parameters);

public slots:
    void accept();
    void notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus);

private:
    QDialogButtonBox *buttonBox;

    AnOutFigure *anOutFigure;

    StimParameters *parameters;
    QGroupBox *stimWaveForm;
    QComboBox *stimShape;
    QLabel *stimShapeLabel;
    QComboBox *stimPolarity;
    QLabel *stimPolarityLabel;
    TimeSpinBox *firstPhaseDuration;
    QLabel *firstPhaseDurationLabel;
    TimeSpinBox *secondPhaseDuration;
    QLabel *secondPhaseDurationLabel;
    TimeSpinBox *interphaseDelay;
    QLabel *interphaseDelayLabel;
    VoltageSpinBox *firstPhaseAmplitude;
    QLabel *firstPhaseAmplitudeLabel;
    VoltageSpinBox *secondPhaseAmplitude;
    QLabel *secondPhaseAmplitudeLabel;
    VoltageSpinBox *baselineVoltage;
    QLabel *baselineVoltageLabel;

    QGroupBox *trigger;
    QCheckBox *enableStim;
    QComboBox *triggerSource;
    QLabel *triggerSourceLabel;
    QComboBox *triggerEdgeOrLevel;
    QLabel *triggerEdgeOrLevelLabel;
    QComboBox *triggerHighOrLow;
    QLabel *triggerHighOrLowLabel;
    TimeSpinBox *postTriggerDelay;
    QLabel *postTriggerDelayLabel;

    QGroupBox *pulseTrain;
    QComboBox *pulseOrTrain;
    QLabel *pulseOrTrainLabel;
    QSpinBox *numberOfStimPulses;
    QLabel *numberOfStimPulsesLabel;
    TimeSpinBox *pulseTrainPeriod;
    QLabel *pulseTrainPeriodLabel;
    QLabel *pulseTrainFrequencyLabel;
    TimeSpinBox *refractoryPeriod;
    QLabel *refractoryPeriodLabel;

    double timestep, currentstep;

private slots:
    void enableWidgets();
    void calculatePulseTrainFrequency();
    void constrainPulseTrainPeriod();
    void roundTimeInputs();
    void roundVoltageInputs();
    void updateMonophasicAndPositive();

signals:
    //signal that is emitted when the DoubleSpinBoxes or ComboBox that control the minimum pulse train period have been changed
    void minimumPeriodChanged();

    //signals that are emitted when various widgets that control the Stimulation Parameters are selected
    void monophasicAndPositiveSignalChanged(bool logicValue);
    void highlightBaselineVoltage(bool highlight);
    void highlightFirstPhaseDuration(bool highlight);
    void highlightSecondPhaseDuration(bool highlight);
    void highlightInterphaseDelay(bool highlight);
    void highlightFirstPhaseAmplitude(bool highlight);
    void highlightSecondPhaseAmplitude(bool highlight);
    void highlightPostTriggerDelay(bool highlight);
    void highlightPulseTrainPeriod(bool highlight);
    void highlightRefractoryPeriod(bool highlight);
};

#endif // ANOUTDIALOG_H
