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

#ifndef STIMPARAMDIALOG_H
#define STIMPARAMDIALOG_H

#include <QDialog>

#include "stimparameters.h"
#include "globalconstants.h"

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
class CurrentSpinBox;
class StimFigure;

class StimParamDialog : public QDialog
{
    Q_OBJECT

public:
    StimParamDialog(StimParameters *parameters, QString nativeChannelName, QString customChannelName, double timestep_us, double currentstep_uA, QWidget *parent = 0);
    ~StimParamDialog();
    void loadParameters(StimParameters* parameters);

public slots:
    void accept();
    void notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus);

private:
    QDialogButtonBox *buttonBox;

    StimFigure *stimFigure;

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
    CurrentSpinBox *firstPhaseAmplitude;
    QLabel *firstPhaseAmplitudeLabel;
    CurrentSpinBox *secondPhaseAmplitude;
    QLabel *secondPhaseAmplitudeLabel;
    QLabel *totalPositiveChargeLabel;
    QLabel *totalNegativeChargeLabel;
    QLabel *chargeBalanceLabel;

    /*
    QLabel *displayTimeStepLabel;
    QLabel *displayCurrentStepLabel;
    */

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

    QGroupBox *ampSettle;
    TimeSpinBox *preStimAmpSettle;
    QLabel *preStimAmpSettleLabel;
    TimeSpinBox *postStimAmpSettle;
    QLabel *postStimAmpSettleLabel;
    QCheckBox *maintainAmpSettle;
    QCheckBox *enableAmpSettle;

    QGroupBox *chargeRecovery;
    TimeSpinBox *postStimChargeRecovOn;
    QLabel *postStimChargeRecovOnLabel;
    TimeSpinBox *postStimChargeRecovOff;
    QLabel *postStimChargeRecovOffLabel;
    QCheckBox *enableChargeRecovery;

    double timestep, currentstep;

private slots:
    void enableWidgets();
    void calculateCharge();
    void calculatePulseTrainFrequency();
    void constrainPostTriggerDelay();
    void constrainPostStimChargeRecovery();
    void constrainRefractoryPeriod();
    void constrainPulseTrainPeriod();
    void roundTimeInputs();
    void roundCurrentInputs();

signals:
    //signal that is emitted when the DoubleSpinBoxes that control charge have been changed
    void chargeChanged();

    //signal that is emitted when the DoubleSpinBoxes or ComboBox that control the minimum pulse train period have been changed
    void minimumPeriodChanged();

    //signals that are emitted when various widgets that control the Stimulation Parameters are selected
    void highlightFirstPhaseDuration(bool highlight);
    void highlightSecondPhaseDuration(bool highlight);
    void highlightInterphaseDelay(bool highlight);
    void highlightFirstPhaseAmplitude(bool highlight);
    void highlightSecondPhaseAmplitude(bool highlight);
    void highlightPostTriggerDelay(bool highlight);
    void highlightPulseTrainPeriod(bool highlight);
    void highlightRefractoryPeriod(bool highlight);
    void highlightPreStimAmpSettle(bool highlight);
    void highlightPostStimAmpSettle(bool highlight);
    void highlightPostStimChargeRecovOn(bool highlight);
    void highlightPostStimChargeRecovOff(bool highlight);
};

#endif // STIMPARAMDIALOG_H
