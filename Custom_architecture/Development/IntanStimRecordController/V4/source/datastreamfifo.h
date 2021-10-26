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

#ifndef DATASTREAMFIFO_H
#define DATASTREAMFIFO_H

#include <QSemaphore>

class QSemaphore;

class DataStreamFifo
{
public:
    DataStreamFifo(unsigned int bufferSize_);
    ~DataStreamFifo();

    bool writeToBuffer(unsigned char* dataSource, unsigned int numBytes);
    bool dataAvailable(unsigned int numBytes) const;
    bool readFromBuffer(unsigned char *dataSink, unsigned int numBytes);
    void resetBuffer();
    unsigned int bytesAvailable() const;
    double percentFull() const;
    int indexDistance() const;

private:
    unsigned char* buffer;
    int bufferSize;
    QSemaphore freeBytes;
    QSemaphore usedBytes;
    int bufferWriteIndex;
    int bufferReadIndex;
};

#endif // DATASTREAMFIFO_H
