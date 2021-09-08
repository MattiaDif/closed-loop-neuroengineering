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
#include <qmath.h>
#include <iostream>

#include "anoutfigure.h"
#include "stimparameters.h"

using namespace std;

/* Constructor that calls the pseudo-constructor common to all children of AbstractFigure, and sets the local variables */
AnOutFigure::AnOutFigure(StimParameters *stimParameters, QWidget *parent) :
    AbstractFigure()
{
    generalSetup(stimParameters, parent);

    //set locals
    localStimShape = parameters->stimShape;
    localStimPolarity = parameters->stimPolarity;
    localPulseOrTrain = parameters->pulseOrTrain;
}

/* Public function that draws the figure as an Analog Output pulse */
void AnOutFigure::uniqueRedraw(QPainter &painter)
{
    y0 = height() / 2;
    xStimBegin = 1.3 * xStimBegin;

    oldColor = painter.pen().color();

    if (highlitBaselineVoltage)
        painter.setPen(durationLabelColorHL);

    painter.drawLine(x0, y0, xStimBegin, y0);

    painter.drawText(x0 + 3, y0 + fontMetrics().height(), "BASELINE VOLTAGE");

    // draw stimulation pulse
    int xStimEnd = x0 + 0.3 * xLength;
    int yMaxAmplitude = 0.37 * height();
    int xStimBegin2 = 0;
    int xStimEnd2 = 0;

    drawHorizontalArrow(painter, x0, xStimBegin, y0 - yMaxAmplitude - ArrowSize - 3,
                        highlitPostTriggerDelay ? durationLabelColorHL : durationLabelColor, "DELAY", true);

    painter.setPen(oldColor);
    drawStimPulse(painter, xStimBegin, xStimEnd, y0, yMaxAmplitude);

    if (highlitBaselineVoltage)
        painter.setPen(durationLabelColorHL);

    if (localPulseOrTrain == StimParameters::SinglePulse) {
        painter.drawLine(xStimEnd, y0, xEnd, y0);
    } else {
        painter.drawLine(xStimEnd, y0, x0 + 0.35 * xLength, y0);
        drawDashedHorizontalLine(painter, x0 + 0.35 * xLength, x0 + 0.45 * xLength, y0, (highlitStimTrace ? currentColorHL : currentColor), 6);
        xStimBegin2 = x0 + 0.5 * xLength + 0.3 * xStimBegin;
        xStimEnd2 = x0 + 0.7 * xLength;
        painter.drawLine(x0 + 0.45 * xLength, y0, xStimBegin2, y0);

        painter.setPen(oldColor);
        drawStimPulse(painter, xStimBegin2, xStimEnd2, y0, yMaxAmplitude);

        if (highlitBaselineVoltage)
            painter.setPen(durationLabelColorHL);

        painter.drawLine(xStimEnd2, y0, xEnd, y0);
        if (localMonophasicAndPositive)
        {
            drawHorizontalArrow(painter, xStimBegin, xStimBegin2, y0 + 3 * (ArrowSize + 3),
                                highlitPulseTrainPeriod ? durationLabelColorHL : durationLabelColor, "PULSE PERIOD", false);
        }

        else
        {
            drawHorizontalArrow(painter, xStimBegin, xStimBegin2, y0 + yMaxAmplitude + ArrowSize + 3,
                                highlitPulseTrainPeriod ? durationLabelColorHL : durationLabelColor, "PULSE PERIOD", false);
        }
    }
    int xStimEndFinal = (localPulseOrTrain == StimParameters::SinglePulse) ? xStimEnd : xStimEnd2;
    drawHorizontalArrow(painter, xStimEndFinal, xEnd, y0 - yMaxAmplitude - ArrowSize - 3,
                        highlitRefractoryPeriod ? durationLabelColorHL : durationLabelColor, "REFRACTORY", true);

    // draw trigger marker
    painter.setPen(Qt::darkBlue);
    painter.drawText(stimFrame.left(), fontMetrics().height() + 1, triggerLabel);
    painter.drawLine(x0, stimFrame.top(), x0, frame.bottom() - 5);

    // draw end marker
    painter.drawText(stimFrame.right() - endLabelWidth, fontMetrics().height() + 1, endLabel);
    painter.drawLine(xEnd, stimFrame.top(), xEnd, frame.bottom() - 5);
}

/* Private function that determines the size of the figure */
QSize AnOutFigure::minimumSizeHint() const
{
    return QSize(figureXSize, figureYSize);
}

/* Private function that determines the size of the figure */
QSize AnOutFigure::sizeHint() const
{
    return QSize(figureXSize, figureYSize);
}

/* Public slot that is called when Second Phase gains or loses focus */
void AnOutFigure::highlightSecondPhaseDuration(bool highlight)
{
    highlitSecondPhaseDuration = highlight;
    generalRedraw();
}

/* Public slot that is called when Interphase Delay gains or loses focus */
void AnOutFigure::highlightInterphaseDelay(bool highlight)
{
    highlitInterphaseDelay = highlight;
    generalRedraw();
}

/* Public slot that is called when First Phase Amplitude gains or loses focus */
void AnOutFigure::highlightFirstPhaseAmplitude(bool highlight)
{
    highlitFirstPhaseAmplitude = highlight;
    generalRedraw();
}

/* Public slot that is called when Second Phase Amplitude gains or loses focus */
void AnOutFigure::highlightSecondPhaseAmplitude(bool highlight)
{
    highlitSecondPhaseAmplitude = highlight;
    generalRedraw();
}

/* Public slot that is called when Baseline Voltage gains or loses focus */
void AnOutFigure::highlightBaselineVoltage(bool highlight)
{
    highlitBaselineVoltage = highlight;
    generalRedraw();
}


/* Public slot that updates the monophasicAndPositive variable and redraws the figure when stimShape or stimPolarity changes */
void AnOutFigure::updateMonophasicAndPositive(bool logicValue)
{
    localMonophasicAndPositive = logicValue;
    generalRedraw();
}
