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

#include "ampsettledialog.h"

// Amp settle dialog.
// This dialog allows users to select desired values for amplifier settle parameters.

AmpSettleDialog::AmpSettleDialog(double lowerSettleBandwidth, bool useFastSettle, bool headstageGlobalSettle,
                                 QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *lowFreqSettleLayout = new QVBoxLayout();
    QHBoxLayout *lowFreqSettleSelectLayout = new QHBoxLayout();

    settleHeadstageCheckBox = new QCheckBox(tr("Headstage Global Amp Settle"));
    settleHeadstageCheckBox->setChecked(headstageGlobalSettle);

    lowRangeSettleLabel = new QLabel(tr("Lower Bandwidth for Amp Settle Range: 0.1 Hz to 1000 Hz."));

    lowFreqSettleLineEdit = new QLineEdit(QString::number(lowerSettleBandwidth, 'f', 2));
    lowFreqSettleLineEdit->setValidator(new QDoubleValidator(0.1, 1000.0, 3, this));
    connect(lowFreqSettleLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(onLineEditTextChanged()));

    lowFreqSettleSelectLayout->addWidget(new QLabel(tr("Amplifier Lower Bandwidth for Amp Settle")));
    lowFreqSettleSelectLayout->addWidget(lowFreqSettleLineEdit);
    lowFreqSettleSelectLayout->addWidget(new QLabel(tr("Hz")));
    lowFreqSettleSelectLayout->addStretch();

    lowFreqSettleLayout->addLayout(lowFreqSettleSelectLayout);
    lowFreqSettleLayout->addWidget(lowRangeSettleLabel);

    // Amplifier settle mode combo box.
    ampSettleModeComboBox = new QComboBox();
    ampSettleModeComboBox->addItem(tr("Switch Lower Bandwidth"));
    ampSettleModeComboBox->addItem(tr("Traditional Fast Settle"));
    ampSettleModeComboBox->setCurrentIndex(0);

    connect(ampSettleModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAmpSettleMode(int)));
    ampSettleModeComboBox->setCurrentIndex(useFastSettle ? 1 : 0);

    QHBoxLayout *ampSettleModeLayout = new QHBoxLayout;
    ampSettleModeLayout->addWidget(new QLabel(tr("Amp Settle Mode")));
    ampSettleModeLayout->addWidget(ampSettleModeComboBox);
    ampSettleModeLayout->addStretch();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(ampSettleModeLayout);
    mainLayout->addLayout(lowFreqSettleLayout);
    mainLayout->addWidget(settleHeadstageCheckBox);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Select Amplifier Settle Parameters"));

    onLineEditTextChanged();
}

// Check the validity of requested frequencies.
void AmpSettleDialog::onLineEditTextChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                (lowFreqSettleLineEdit->hasAcceptableInput()));

    if (!lowFreqSettleLineEdit->hasAcceptableInput()) {
        lowRangeSettleLabel->setStyleSheet("color: red");
    } else {
        lowRangeSettleLabel->setStyleSheet("");
    }
}

// Change amp settle mode
void AmpSettleDialog::setAmpSettleMode(int index)
{
    if (index == 1) {
        lowFreqSettleLineEdit->setEnabled(false);
    } else {
        lowFreqSettleLineEdit->setEnabled(true);
    }
}
