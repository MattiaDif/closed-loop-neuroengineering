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

#ifndef DIGOUTDIALOG_H
#define DIGOUTDIALOG_H

#include <QDialog>

#include "stimparameters.h"
#include "globalconstants.h"

class QDialogButtonBox;
class QWidget;
class QComboBox;
class QSpinBox;
class QLabel;
class QGroupBox;
class QCheckBox;
class StimParameters;
class TimeSpinBox;
class DigFigure;

class DigOutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DigOutDialog(StimParameters *parameter, QString nativeChannelName, QString customChannelName, double timestep_us, QWidget *parent = 0);
    void loadParameters(StimParameters* parameters);

public slots:
    void accept();
    void notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus);

private:
    QDialogButtonBox *buttonBox;

    DigFigure *digFigure;

    StimParameters *parameters;

    QGroupBox *trigger;
    QCheckBox *enablePulse;
    QComboBox *triggerSource;
    QLabel *triggerSourceLabel;
    QComboBox *triggerEdgeOrLevel;
    QLabel *triggerEdgeOrLevelLabel;
    QComboBox *triggerHighOrLow;
    QLabel *triggerHighOrLowLabel;
    TimeSpinBox *postTriggerDelay;
    QLabel *postTriggerDelayLabel;

    QGroupBox *pulse;
    TimeSpinBox *pulseDuration;
    QLabel *pulseDurationLabel;
    QComboBox *pulseRepetition;
    QLabel *pulseRepetitionLabel;
    QSpinBox *numPulses;
    QLabel *numPulsesLabel;
    TimeSpinBox *pulseTrainPeriod;
    QLabel *pulseTrainPeriodLabel;
    QLabel *pulseTrainFrequencyLabel;
    TimeSpinBox *refractoryPeriod;
    QLabel *refractoryPeriodLabel;

    double timestep;

private slots:
    void enableWidgets();
    void roundTimeInputs();
    void calculatePulseTrainFrequency();
    void constrainPulseTrainPeriod();

signals:
    //signals that are emitted when various widgets that control the Digital Output Parameters are selected
    void highlightFirstPhaseDuration(bool highlight);
    void highlightPostTriggerDelay(bool highlight);
    void highlightPulseTrainPeriod(bool highlight);
    void highlightRefractoryPeriod(bool highlight);

};

#endif // DIGOUTDIALOG_H
