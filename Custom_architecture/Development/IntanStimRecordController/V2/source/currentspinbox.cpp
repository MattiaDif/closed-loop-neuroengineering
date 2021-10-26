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

#include "currentspinbox.h"
#include <QtGui>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#endif

/* Current Spin Box is a widget containing a double spin box, customized to display current values from nA to mA */

/* Constructor */
CurrentSpinBox::CurrentSpinBox(double currentstep_uA, QWidget *parent) :
    QWidget(parent)
{
    //allow other functions to access currentstep: loadValue(), scaleUnits(), and round()
    currentstep = currentstep_uA;

    //create new double spin box, and set its default units to microamps
    doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setSingleStep(currentstep_uA);
    doubleSpinBox->setDecimals(2);
    doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "A");

    //when the Current Spin Box's value is changed, emit a signal and scale units
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(sendSignalValueMicroA(double)));
    connect(this, SIGNAL(trueValueChanged(double)), this, SLOT(scaleUnits(double)));

    //when the Double Spin Box within Current Spin Box's value is changed or editing has finished, emit a complementary signal from the Current Spin Box
    connect(doubleSpinBox,  SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(doubleSpinBox, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));

    //set the Current Spin Box to contain the Double Spin Box widget
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(doubleSpinBox);
    setLayout(mainLayout);
}

/* Pass-through public member function that allows external setting of the range of the Double Spin Box */
void CurrentSpinBox::setRange(double minimum, double maximum)
{
    doubleSpinBox->setRange(minimum, maximum);
}

/* Public member function that allows external access to the true value (in microamps) of the Double Spin Box */
double CurrentSpinBox::getTrueValue()
{
    return value_uA;
}

/* Public member function that allows external setting of the true minimum (in microamps) of the Double Spin Box */
void CurrentSpinBox::setTrueMinimum(double min_uA)
{
    //if the Current Spin Box's units are milliamps, set its minimum in milliamps (microamps / 1000)
    if (getTrueValue() >= 1000)
        doubleSpinBox->setMinimum(min_uA/1000);

    //if the Current Spin Box's units are microamps, set its minimum in microamps
    else if (getTrueValue() < 1000 && getTrueValue() >= 1)
        doubleSpinBox->setMinimum(min_uA);

    //if the Current Spin Box's units are nanoamps, set its minimum in nanoamps (microamps * 1000)
    else
        doubleSpinBox->setMinimum(min_uA * 1000);
}

/* Pass-through public member function that allows external access to the pointer referencing the Double Spin Box */
QWidget* CurrentSpinBox::pointer()
{
    return doubleSpinBox;
}

/* Pass-through public member slot that allows external setting of the value of the Double Spin Box */
void CurrentSpinBox::setValue(double val)
{
    doubleSpinBox->setValue(val);
}

/* Public member slot that allows the Current Spin Box to be initialized when first loaded. Once loaded, values should be changed through setValue() */
void CurrentSpinBox::loadValue(double val)
{
    //if the current step is in microamps, display the value to be loaded in microamps
    if (val == 0 && currentstep >= 1)
    {
        doubleSpinBox->setMaximum(255 * currentstep);
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "A");
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setValue(val);
        doubleSpinBox->setSingleStep(currentstep);
    }

    //if the value to be loaded is less than 1 microamp, display the initial value in nanoamps
    else if (val < 1)
    {
        doubleSpinBox->setMaximum((255 * currentstep) * 1000);
        doubleSpinBox->setSuffix(" nA");
        doubleSpinBox->setDecimals(0);
        doubleSpinBox->setValue(val * 1000);
        doubleSpinBox->setSingleStep(currentstep * 1000);
    }

    //if the value to be loaded is 1 milliamp or greater, display the initial value in milliamps
    else if (val >= 1000)
    {
        doubleSpinBox->setMaximum((255 * currentstep) / 1000);
        doubleSpinBox->setSuffix(" mA");
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setValue(val / 1000);
        doubleSpinBox->setSingleStep(currentstep / 1000);
    }

    //if the value to be loaded is within the range of 1 to 1000 microamps, display the initial value in microamps
    else
    {
        doubleSpinBox->setMaximum(255 * currentstep);
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "A");
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setValue(val);
        doubleSpinBox->setSingleStep(currentstep);
    }

    //set the private member value_uA to hold the current value of the Double Spin Box in microamps
    value_uA = val;
}

/* Public member slot that scales the units of the Current Spin Box to reflect the value it holds */
void CurrentSpinBox::scaleUnits(double val_uA)
{
    //examine the value (in microamps) to determine the correct units, and examine the suffix to determine the current units.
    //when they don't match, change the range, suffix, displayed value, decimal precision, and single step to reflect the correct units

    //mA to uA
    if ((val_uA < 1000) && (val_uA >= 1) && (doubleSpinBox->suffix() == (" mA")))
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() * 1000);
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "A");
        doubleSpinBox->setValue(val_uA);
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() * 1000);
    }

    //mA to nA
    if ((val_uA < 1) && (doubleSpinBox->suffix() == " mA") && currentstep < .1)
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() * 1000000);
        doubleSpinBox->setSuffix(" nA");
        doubleSpinBox->setValue(val_uA * 1000);
        doubleSpinBox->setDecimals(0);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() * 1000000);
    }

    //uA to mA
    if ((val_uA >= 1000) && (doubleSpinBox->suffix() == (" " + QSTRING_MU_SYMBOL + "A")))
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() / 1000);
        doubleSpinBox->setSuffix(" mA");
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setValue(val_uA / 1000);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() / 1000);
    }

    //uA to nA
    if ((val_uA < 1) && (doubleSpinBox->suffix() == (" " + QSTRING_MU_SYMBOL + "A")) && currentstep < .1)
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() * 1000);
        doubleSpinBox->setSuffix(" nA");
        doubleSpinBox->setValue(val_uA * 1000);
        doubleSpinBox->setDecimals(0);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() * 1000);
    }

    //nA to mA
    if ((val_uA >= 1000) && (doubleSpinBox->suffix() == " nA"))
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() / 1000000);
        doubleSpinBox->setSuffix(" mA");
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setValue(val_uA / 1000);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() / 1000000);
    }

    //nA to uA
    if ((val_uA < 1000) && (val_uA >= 1) && (doubleSpinBox->suffix() == (" nA")))
    {
        doubleSpinBox->setMaximum(doubleSpinBox->maximum() / 1000);
        doubleSpinBox->setSuffix(" " + QSTRING_MU_SYMBOL + "A");
        doubleSpinBox->setValue(val_uA);
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setSingleStep(doubleSpinBox->singleStep() / 1000);
    }
}

/* Public member slot that sets value_uA and emits a signal with the value of the Double Spin Box in microamps */
void CurrentSpinBox::sendSignalValueMicroA(double val)
{
    //if units are nA, value in uA is value in nA divided by 1000
    if (doubleSpinBox->suffix() == " nA")
    {
        value_uA = val / 1000;
    }

    //if units are uA, units are consistent
    else if (doubleSpinBox->suffix() == " " + QSTRING_MU_SYMBOL + "A")
    {
        value_uA = val;
    }

    //if units are mA, value in uA is value in mA multiplied by 1000
    else if (doubleSpinBox->suffix() == " mA")
    {
        value_uA = val * 1000;
    }

    //emit a signal, with a parameter containing the value of the Double Spin Box in uA
    emit trueValueChanged(value_uA);
}

/* Public member slot that snaps the value of the Double Spin Box to its closest multiple of current step */
void CurrentSpinBox::round()
{
    //use modulo to find the difference between the current value and the closest multiple of current step
    //due to errors in floating point modulo, conduct modulo in the integer domain, and then bring back to double
    long valueint = (long) ((this->getTrueValue() * 1000) + 0.5);
    long currentstepint = (long) (currentstep * 1000);
    int modint = valueint % currentstepint;
    double mod = modint / 1000.0;

    //if the modulo is not zero, the current value is not a multiple of current step
    if (mod != 0)
    {
        //if the modulo is less than half of the current step, the value should round down
        if (mod < currentstep/2)
        {
            //if the current units are milliamps, the difference should be scaled to milliamps as well
            if (this->getTrueValue() >= 1000)
                doubleSpinBox->setValue(doubleSpinBox->value() - mod / 1000);

            //if the current units are microamps, the difference doesn't need to be scaled
            else if (this->getTrueValue() < 1000 && this->getTrueValue() >= 1)
                doubleSpinBox->setValue(doubleSpinBox->value() - mod);

            //if the current units are nanoamps, the difference should be scaled to nanoamps as well
            else
                doubleSpinBox->setValue(doubleSpinBox->value() - mod * 1000);
        }

        //if the modulo is greater than or equal to half of the current step, the value should round up
        else
        {
            //if the current units are milliamps, the difference should be scaled to milliamps as well
            if (this->getTrueValue() >= 1000)
                doubleSpinBox->setValue(doubleSpinBox->value() + (currentstep - mod) / 1000);

            //if the current units are microamps, the difference doesn't need to be scaled
            else if (this->getTrueValue() < 1000 && this->getTrueValue() >= 1)
                doubleSpinBox->setValue(doubleSpinBox->value() + (currentstep - mod));

            //if the current units are nanoamps, the difference should be scaled to nanoamps as well
            else
                doubleSpinBox->setValue(doubleSpinBox->value() + (currentstep - mod) * 1000);
        }
    }
}
