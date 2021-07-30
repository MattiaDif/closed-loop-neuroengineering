//------------------------------------------------------------------------------
//
//  Intan Technologies RHX Data Acquisition Software
//  Version 3.0.3
//
//  Copyright (c) 2020-2021 Intan Technologies
//
//  This file is part of the Intan Technologies RHX Data Acquisition Software.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  This software is provided 'as-is', without any express or implied warranty.
//  In no event will the authors be held liable for any damages arising from
//  the use of this software.
//
//  See <http://www.intantech.com> for documentation and product information.
//
//------------------------------------------------------------------------------

#ifndef TRIGGERRECORDDIALOG_H
#define TRIGGERRECORDDIALOG_H

#include <QDialog>
#include "systemstate.h"

class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QSpinBox;

class TriggerRecordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TriggerRecordDialog(SystemState* state_, QWidget *parent);

    void updateFromState();

    QString getTriggerSave();
    QString getTriggerInput();
    QString getTriggerPolarity();
    QString getRecordBuffer();
    QString getPostTriggerBufferSeconds();

private:
    SystemState* state;

    QCheckBox *triggerSaveCheckBox;

    QComboBox *digitalInputComboBox;
    QComboBox *triggerPolarityComboBox;
    QSpinBox *recordBufferSpinBox;
    QSpinBox *postTriggerSpinBox;
    QDialogButtonBox *buttonBox;

    int digitalInput;
    int triggerPolarity;

private slots:
    void setDigitalInput(int index);
    void setTriggerPolarity(int index);
};

#endif // TRIGGERRECORDDIALOG_H
