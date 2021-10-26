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

#ifndef CHARGERECOVERYDIALOG_H
#define CHARGERECOVERYDIALOG_H

#include <QDialog>
#include "rhs2000registers.h"

class QDialogButtonBox;
class QLineEdit;
class QLabel;
class QComboBox;

class ChargeRecoveryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChargeRecoveryDialog(bool chargeRecoveryMode,
                                  Rhs2000Registers::ChargeRecoveryCurrentLimit currentLimit,
                                  double targetVoltage, QWidget *parent);

    QLineEdit *targetVoltageLineEdit;
    QComboBox *currentLimitComboBox;
    QComboBox *chargeRecoveryModeComboBox;
    QDialogButtonBox *buttonBox;

    Rhs2000Registers::ChargeRecoveryCurrentLimit getCurrentLimit();

signals:
    
public slots:

private slots:
    void onLineEditTextChanged();
    void setChargeRecoveryMode(int index);

private:
    QLabel *targetVoltageRangeLabel;
};

#endif // CHARGERECOVERYDIALOG_H
