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

#include "helpdialogreference.h"

// Selectable reference help dialog.

HelpDialogReference::HelpDialogReference(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Reference Selection"));

    QLabel *label1 = new QLabel(tr("Each channel on an RHS2000 chip amplifies an electrode signal with respect to a "
                                   "reference potential connected to the REF input of the chip.  This <b>hardware reference</b> "
                                   "is often connected to a local ground near the recording site to reduce pickup of common-mode "
                                   "signals such as AC line noise and movement artifacts."));
    label1->setWordWrap(true);

    QLabel *label2 = new QLabel(tr("In some cases, it is desriable to perform an additional digital subtraction, "
                                   "using one amplifier channel with no localized activity to re-reference all "
                                   "the other amplifier signals.  "
                                   "Any selected amplifier channel may be used as a <b>digital reference</b>.  "
                                   "This channel will be subtracted from all other amplifier channels in real time.  This "
                                   "affects data saved to disk as well as signals routed to the ANALOG OUT ports "
                                   "and DIGITAL OUT threshold units."));
    label2->setWordWrap(true);

    QLabel *label3 = new QLabel(tr("The identity of the digital reference channel is saved in the header of the "
                                   "data file, and this channel may be added back to saved data if desired to reconstruct the "
                                   "original signals prior to digital re-referencing."));
    label3->setWordWrap(true);

    QLabel *label4 = new QLabel(tr("<b>Note:</b> If the background noise on all channels is approximately Gaussian, "
                                   "this digital reference subtraction will increase the background noise by 41% "
                                   "(a factor of the square root of two)."));
    label4->setWordWrap(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label1);
    mainLayout->addWidget(label2);
    mainLayout->addWidget(label3);
    mainLayout->addWidget(label4);

    setLayout(mainLayout);
}
