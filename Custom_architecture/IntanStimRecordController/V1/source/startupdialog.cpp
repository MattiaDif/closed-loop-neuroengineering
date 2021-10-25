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

#include "startupdialog.h"
#include "globalconstants.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

StartUpDialog::StartUpDialog(int *sampleRateIndex_, int *stimStepIndex_, QWidget *parent) :
    QDialog(parent)
{
    sampleRateIndex = sampleRateIndex_;
    stimStepIndex = stimStepIndex_;

    QGroupBox *sampleRateGroupBox = new QGroupBox(tr("Amplifier Sampling Rate / Stimulation Time Resolution"));

    // Amplifier sample rate combo box.
    sampleRateComboBox = new QComboBox();

    sampleRateComboBox->addItem("20.0 kS/s sampling rate (50 " + QSTRING_MU_SYMBOL + "s stimulation time resolution)");
    sampleRateComboBox->addItem("25.0 kS/s sampling rate (40 " + QSTRING_MU_SYMBOL + "s stimulation time resolution)");
    sampleRateComboBox->addItem("30.0 kS/s sampling rate (33.3 " + QSTRING_MU_SYMBOL + "s stimulation time resolution)");
//    sampleRateComboBox->addItem("40.0 kS/s sampling rate (25 " + QSTRING_MU_SYMBOL + "s stimulation time resolution)");
    sampleRateComboBox->setCurrentIndex(2);

    QHBoxLayout *sampleRateLayout1 = new QHBoxLayout();

    sampleRateLayout1->addWidget(new QLabel(tr("Amplifier Sampling Rate")));
    sampleRateLayout1->addWidget(sampleRateComboBox);
    sampleRateLayout1->addStretch(1);

    QVBoxLayout *sampleRateLayout = new QVBoxLayout();
    sampleRateLayout->addLayout(sampleRateLayout1);

    sampleRateGroupBox->setLayout(sampleRateLayout);

    QGroupBox *stimStepGroupBox = new QGroupBox(tr("Stimulation Range / Step Size"));

    stimStepComboBox = new QComboBox();
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "2.55 " + QSTRING_MU_SYMBOL + "A range (10 nA step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "5.10 " + QSTRING_MU_SYMBOL + "A range (20 nA step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "12.75 " + QSTRING_MU_SYMBOL + "A range (50 nA step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "25.5 " + QSTRING_MU_SYMBOL + "A range (0.1 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "51.0 " + QSTRING_MU_SYMBOL + "A range (0.2 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "127.5 " + QSTRING_MU_SYMBOL + "A range (0.5 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "255 " + QSTRING_MU_SYMBOL + "A range (1 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "510 " + QSTRING_MU_SYMBOL + "A range (2 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "1.275 mA range (5 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->addItem(QSTRING_PLUSMINUS_SYMBOL + "2.550 mA range (10 " + QSTRING_MU_SYMBOL + "A step size)");
    stimStepComboBox->setCurrentIndex(5);

    QHBoxLayout *stimStepLayout1 = new QHBoxLayout();

    stimStepLayout1->addWidget(new QLabel(tr("Stimulation Range")));
    stimStepLayout1->addWidget(stimStepComboBox);
    stimStepLayout1->addStretch(1);

    QVBoxLayout *stimStepLayout = new QVBoxLayout();
    stimStepLayout->addLayout(stimStepLayout1);

    stimStepGroupBox->setLayout(stimStepLayout);

    // Create and connect 'OK' button
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    QHBoxLayout *buttonBoxRow = new QHBoxLayout();
    buttonBoxRow->addStretch();
    buttonBoxRow->addWidget(buttonBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sampleRateGroupBox);
    mainLayout->addWidget(stimStepGroupBox);
    mainLayout->addLayout(buttonBoxRow);
    setLayout(mainLayout);

    setWindowTitle(tr("Intan Technologies Stimulation / Recording Controller Configuration"));
//    setFixedHeight(sizeHint().height());
//    setFixedWidth(sizeHint().width());
}

/* Public slot that saves the values from the dialog box and closes the window */
void StartUpDialog::accept()
{
    *sampleRateIndex = sampleRateComboBox->currentIndex();
    *stimStepIndex = stimStepComboBox->currentIndex();

    done(Accepted);
}
