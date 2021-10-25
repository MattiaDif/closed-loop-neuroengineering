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
#include <iostream>

#include "helpdialogioexpander.h"

using namespace std;

// I/O expander help dialog.

HelpDialogIOExpander::HelpDialogIOExpander(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Intan I/O Expander"));

    QPixmap imageFront;
    imageFront.load(":/images/front_panel_expander_small.jpg", "JPG");
    QLabel *imageFrontLabel = new QLabel();
    imageFrontLabel->setPixmap(imageFront);

    QPixmap imageRear;
    imageRear.load(":/images/back_panel_expander_small.jpg", "JPG");
    QLabel *imageRearLabel = new QLabel();
    imageRearLabel->setPixmap(imageRear);

    QLabel *label1 = new QLabel(tr("The optional <a href=\"http://www.intantech.com/IO_expander.html\">Intan I/O Expander</a> "
                                   "connects to the Intan Stimulation / Recording Controller "
                                   "to provide additional analog and digital input and output ports.  The I/O Expander adds six "
                                   "additional Analog In and Analog Out ports (with +/-10V range and 16-bit resolution) and 14 "
                                   "additional Digital In and Digital Out ports."));
    label1->setWordWrap(true);
    label1->setTextFormat(Qt::RichText);
    label1->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label1->setOpenExternalLinks(true);

    QLabel *label2 = new QLabel(tr("The I/O Expander is powered from the Intan Stimulation / Recording Controller over a supplied interface "
                                   "cable.  The small unit may be mounted in a standard 19\" rack or operated from a bench "
                                   "top."));
    label2->setWordWrap(true);

    QLabel *label3 = new QLabel(tr("<b>Front view</b>"));
    label3->setAlignment(Qt::AlignCenter);

    QLabel *label4 = new QLabel(tr("<b>Rear view</b>"));
    label4->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label1);
    mainLayout->addWidget(label2);
    mainLayout->addWidget(imageFrontLabel);
    mainLayout->addWidget(label3);
    mainLayout->addWidget(imageRearLabel);
    mainLayout->addWidget(label4);

    setLayout(mainLayout);
}
