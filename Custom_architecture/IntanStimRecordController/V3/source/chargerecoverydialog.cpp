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

#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

#include "chargerecoverydialog.h"
#include "globalconstants.h"

// Charge recovery dialog.
// This dialog allows users to select desired values for charge recovery parameters.

ChargeRecoveryDialog::ChargeRecoveryDialog(bool chargeRecoveryMode,
                                           Rhs2000Registers::ChargeRecoveryCurrentLimit currentLimit,
                                           double targetVoltage, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *targetVoltageLayout = new QVBoxLayout();
    QHBoxLayout *targetVoltageSelectLayout = new QHBoxLayout();

    targetVoltageRangeLabel = new QLabel(tr("Target Voltage Range: -1.225 V to +1.215 V."));

    targetVoltageLineEdit = new QLineEdit(QString::number(targetVoltage, 'f', 3));
    targetVoltageLineEdit->setValidator(new QDoubleValidator(-1.225, 1.215, 3, this));
    connect(targetVoltageLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onLineEditTextChanged()));

    targetVoltageSelectLayout->addWidget(new QLabel(tr("Target Voltage for Current-Limited Charge Recovery")));
    targetVoltageSelectLayout->addWidget(targetVoltageLineEdit);
    targetVoltageSelectLayout->addWidget(new QLabel(tr("V")));
    targetVoltageSelectLayout->addStretch();

    targetVoltageLayout->addLayout(targetVoltageSelectLayout);
    targetVoltageLayout->addWidget(targetVoltageRangeLabel);

    // Current limit combo box.
    currentLimitComboBox = new QComboBox();
    currentLimitComboBox->addItem("1 nA");
    currentLimitComboBox->addItem("2 nA");
    currentLimitComboBox->addItem("5 nA");
    currentLimitComboBox->addItem("10 nA");
    currentLimitComboBox->addItem("20 nA");
    currentLimitComboBox->addItem("50 nA");
    currentLimitComboBox->addItem("100 nA");
    currentLimitComboBox->addItem("200 nA");
    currentLimitComboBox->addItem("500 nA");
    currentLimitComboBox->addItem("1 " + QSTRING_MU_SYMBOL + "A");
    currentLimitComboBox->setCurrentIndex(((int) currentLimit) - 1);

    QHBoxLayout *currentLimitLayout = new QHBoxLayout;
    currentLimitLayout->addWidget(new QLabel(tr("Charge Recovery Current Limit")));
    currentLimitLayout->addWidget(currentLimitComboBox);
    currentLimitLayout->addStretch();

    // Charge recovery mode combo box.
    chargeRecoveryModeComboBox = new QComboBox();
    chargeRecoveryModeComboBox->addItem(tr("Current-Limited Charge Recovery Circuit"));
    chargeRecoveryModeComboBox->addItem(tr("Charge Recovery Switch"));
    chargeRecoveryModeComboBox->setCurrentIndex(0);

    connect(chargeRecoveryModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setChargeRecoveryMode(int)));
    chargeRecoveryModeComboBox->setCurrentIndex(chargeRecoveryMode ? 1 : 0);

    QHBoxLayout *chargeRecoveryModeLayout = new QHBoxLayout;
    chargeRecoveryModeLayout->addWidget(new QLabel(tr("Charge Recovery Mode")));
    chargeRecoveryModeLayout->addWidget(chargeRecoveryModeComboBox);
    chargeRecoveryModeLayout->addStretch();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(chargeRecoveryModeLayout);
    mainLayout->addLayout(targetVoltageLayout);
    mainLayout->addLayout(currentLimitLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Select Charge Recovery Parameters"));

    onLineEditTextChanged();
}

// Check the validity of requested frequencies.
void ChargeRecoveryDialog::onLineEditTextChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                (targetVoltageLineEdit->hasAcceptableInput()));

    if (!targetVoltageLineEdit->hasAcceptableInput()) {
        targetVoltageRangeLabel->setStyleSheet("color: red");
    } else {
        targetVoltageRangeLabel->setStyleSheet("");
    }
}

// Change charge recovery mode
void ChargeRecoveryDialog::setChargeRecoveryMode(int index)
{
    if (index == 1) {
        targetVoltageLineEdit->setEnabled(false);
        currentLimitComboBox->setEnabled(false);
    } else {
        targetVoltageLineEdit->setEnabled(true);
        currentLimitComboBox->setEnabled(true);
    }
}

Rhs2000Registers::ChargeRecoveryCurrentLimit ChargeRecoveryDialog::getCurrentLimit()
{
    switch (currentLimitComboBox->currentIndex()) {
    case 0:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit1nA;
    case 1:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit2nA;
    case 2:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit5nA;
    case 3:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit10nA;
    case 4:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit20nA;
    case 5:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit50nA;
    case 6:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit100nA;
    case 7:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit200nA;
    case 8:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit500nA;
    case 9:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimit1uA;
    default:
        return Rhs2000Registers::ChargeRecoveryCurrentLimit::CurrentLimitMax;
    }
}
