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

#include "voltagespinbox.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

VoltageSpinBox::VoltageSpinBox(QWidget *parent) :
    QWidget(parent)
{
    //allow other functions to access voltagestep: loadValue() and round()
    voltagestep = 0.01;

    //create new double spin box, and set its units
    doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setSingleStep(voltagestep);
    doubleSpinBox->setDecimals(2);
    doubleSpinBox->setSuffix(" V");

    //when the Voltage Spin Box's value is changed, emit a signal
    connect(doubleSpinBox, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));

    //set the Voltage Spin Box to contain the Double Spin Box widget
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(doubleSpinBox);
    setLayout(mainLayout);
}

/* Pass-through public member function that allows external setting of the range of the Double Spin Box */
void VoltageSpinBox::setRange(double minimum, double maximum)
{
    doubleSpinBox->setRange(minimum, maximum);
}

/* Public member function that allows external access to the value of the Double Spin Box */
double VoltageSpinBox::getValue()
{
    return doubleSpinBox->value();
}

/* Pass-through public member function that allows external access to the pointer referencing the Double Spin Box */
QWidget* VoltageSpinBox::pointer()
{
    return doubleSpinBox;
}

/* Pass-through public member slot that allows external setting of the value of the Double Spin box */
void VoltageSpinBox::setValue(double val)
{
    doubleSpinBox->setValue(val);
}

/* Public member slot that snaps the value of the Double Spin Box to its closest multiple of voltage step */
void VoltageSpinBox::round()
{
    int sign = (getValue() >= 0 ? 1 : -1);
    //use modulo to find the difference between the current value and the closest multiple of voltage step
    //due to errors in floating point modulo, conduct modulo in the integer domain, and then bring back to double
    long valueint = (long) ((this->getValue() * 1000) + sign * 0.5);
    long voltagestepint = (long) (voltagestep * 1000);
    int modint = (sign * valueint) % voltagestepint;
    double mod = modint / 1000.0;

    //if the modulo is not zero, the current value is not a multiple of voltage step
    if (mod != 0)
    {
        //if the modulo is less than half of the voltage step, the value should round down
        if (mod < voltagestep/2)
        {
            doubleSpinBox->setValue(doubleSpinBox->value() - sign * mod);
        }
        else
        {
            doubleSpinBox->setValue(doubleSpinBox->value() + sign * (voltagestep - mod));
        }
    }
}
