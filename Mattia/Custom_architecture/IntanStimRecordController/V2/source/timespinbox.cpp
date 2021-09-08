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

#include "timespinbox.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

/* Time Spin Box is a widget containing a double spin box, customized to display time values from nA to mA */

/* Constructor */
TimeSpinBox::TimeSpinBox(double timestep_us, QWidget *parent) :
    QWidget(parent)
{
    //allow other functions to access currentstep: loadValue(), scaleUnits(), and round()
    timestep = timestep_us;

    //create new double spin box, and set its default units to microseconds
    doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setSingleStep(timestep_us);
    doubleSpinBox->setDecimals(1);
    doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "s");

    //when the Current Spin Box's value is changed, emit a signal and scale units
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(sendSignalValueMicros(double)));
    connect(this, SIGNAL(trueValueChanged(double)), this, SLOT(scaleUnits(double)));

    //when the Double Spin Box within Time Spin Box's value is changed or editing has finished, emit a complementary signal from the Time Spin Box
    connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(doubleSpinBox, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));

    //set the Time Spin Box to contain the Double Spin Box widget
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(doubleSpinBox);
    setLayout(mainLayout);
}

/* Pass-through public member function that allows external setting of the range of the Double Spin Box */
void TimeSpinBox::setRange(double minimum, double maximum)
{
    doubleSpinBox->setRange(minimum, maximum);
}

/* Public member function that allows external access to the true value (in microseconds) of the Double Spin Box */
double TimeSpinBox::getTrueValue()
{
    return value_us;
}

/* Public member function that allows external setting of the true minimum (in microseconds) of the Double Spin Box */
void TimeSpinBox::setTrueMinimum(double min_us)
{
    if (getTrueValue() >= 1000)
    {
        if (min_us >= 1000)
        {
            doubleSpinBox->setMinimum(min_us/1000);
        }
        else
        {
            //do nothing: the constrained box is in milliseconds, and the minimum box is in microseconds
            //the constrained box will switch to milliseconds before the minimum is reached
            doubleSpinBox->setMinimum(min_us/1000);
        }
    }

    else if (getTrueValue() < 1000)
    {
        doubleSpinBox->setMinimum(min_us);
    }
}

/* Pass-through public member function that allows external access to the pointer referencing the Double Spin Box */
QWidget* TimeSpinBox::pointer()
{
    return doubleSpinBox;
}

/* Pass-through public member slot that allows external setting of the value of the Double Spin Box */
void TimeSpinBox::setValue(double val)
{
    doubleSpinBox->setValue(val);
}

/* Public member slot that allows the Time Spin Box to be initialized when first loaded. Once loaded, values should be changed through setValue() */
void TimeSpinBox::loadValue(double val)
{
    //block signals so that the initialization does not activate the constrain() functions
    blockSignals(true);

    //if the time value should be in ms, but it isn't, change the maximum, suffix, decimal precision, value, and single step to reflect this
    if (val >= 1000 && doubleSpinBox->suffix() != " ms")
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() / 1000);
        doubleSpinBox->setSuffix(" ms");
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setValue(val / 1000);
        doubleSpinBox->setSingleStep(timestep / 1000);
    }

    //if the time value should be in ms, and it is, only change the value to reflect this
    else if (val >= 1000 && doubleSpinBox->suffix() == " ms")
    {
        doubleSpinBox->setValue(val / 1000);
    }

    //if the time value should be in microseconds, but it isn't, change the suffix, decimal precision, value, and single step to reflect this
    if ( val < 1000 && doubleSpinBox->suffix() == " ms")
    {
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "s");
        doubleSpinBox->setDecimals(1);
        doubleSpinBox->setValue(val);
        doubleSpinBox->setSingleStep(timestep);
    }

    //if the time value should be in microseconds, and it is, only change the value to reflect this
    else if (val < 1000 && doubleSpinBox->suffix() != " ms")
    {
        doubleSpinBox->setValue(val);
    }

    //enable signals so that further user changes can active the constrain() functions
    blockSignals(false);

    //set the private member value_us to hold the time value of the Double Spin box in microseconds
    value_us = val;
}

/* Public member slot that scales the units of the Time Spin Box to reflect the value it holds */
void TimeSpinBox::scaleUnits(double val_us)
{
    //store the original minimum of the Double Spin box so that it can be referenced with satisfactory precision
    double storedMinimum = doubleSpinBox->minimum();

    //block signals so that the scaling does not activate the constrain() functions
    blockSignals(true);

    //examine the value (in microseconds) to determine the correct units, and examine the suffix to determine the current units.
    //when they don't match, change the range, suffix, displayed value, decimal precision, and single step to reflect the correct units

    //us to ms
    if (val_us >= 1000 && doubleSpinBox->suffix() != " ms")
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() / 1000);
        doubleSpinBox->setMinimum(storedMinimum / 1000);
        doubleSpinBox->setSuffix(" ms");
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setValue(val_us / 1000);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep()/1000);
    }

    //ms to us
    if (val_us < 1000 && doubleSpinBox->suffix() == " ms")
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() * 1000);
        doubleSpinBox->setMinimum(storedMinimum * 1000);
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "s");
        doubleSpinBox->setValue(val_us);
        doubleSpinBox->setDecimals(1);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() * 1000);
    }

    //enable signals so that further user changes can activate the constrain() functions
    blockSignals(false);
}

/* Public member slot that sets value_us and emits a signal with the value of the Double Spin Box in microseconds */
void TimeSpinBox::sendSignalValueMicros(double val)
{
    //if units are us, units are consistent
    if (doubleSpinBox->suffix() != " ms")
    {
        value_us = val;
    }

    //if units are ms, value in us is value in ms multiplied by 1000
    else
    {
        value_us = val * 1000;
    }

    //emit a signal, with a parameter containing the value of the Double Spin Box in us
    emit trueValueChanged(value_us);
}

/* Public member slot that snaps the value of the Double Spin Box to its closest multiple of time step */
void TimeSpinBox::round()
{
    //use modulo to find the difference between the current value and the closest multiple of time step
    //due to errors in floating point modulo, coduct modulo in the integer domain, and then bring back to double
    long valueint = (long) ((this->getTrueValue() * 1000) + 0.5);
    long timestepint = (long) (timestep * 1000);
    int modint = valueint % timestepint;
    double mod = modint / 1000.0;

    //if the modulo is not zero, the current value is not a multiple of time step
    if (mod != 0)
    {
        //if the modulo is less than half of the time step, the value should round down
        if (mod < timestep/2)
        {
            //if the current units are microseconds, the difference doesn't need to be scaled
            if (this->getTrueValue() < 1000)
                doubleSpinBox->setValue(doubleSpinBox->value() - mod);

            //if the current units are milliseconds, the difference should be scaled to milliseconds as well
            else
                doubleSpinBox->setValue(doubleSpinBox->value() - mod/1000);
        }

        //if the modulo is greater than or equal to half of the time step, the value should round up
        else
        {
            //if the current units are microseconds, the difference doesn't need to be scaled
            if (this->getTrueValue() < 1000)
                doubleSpinBox->setValue(doubleSpinBox->value() + (timestep - mod));

            //if the current units are milliseconds, the difference should be scaled to milliseconds as well
            else
                doubleSpinBox->setValue(doubleSpinBox->value() + (timestep - mod)/1000);
        }
    }
}
