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

#include "datastreamfifo.h"
#include <iostream>
#include <QSemaphore>
#include <QThread>

using namespace std;

DataStreamFifo::DataStreamFifo(unsigned int bufferSize_)
{
    bufferSize = bufferSize_;
    cout << "DataStreamFifo: Allocating " << bufferSize / 1.0e6 << " MBytes for FIFO buffer." << endl;
    buffer = new (nothrow) unsigned char [bufferSize];
    if (buffer == nullptr) {
        cerr << "Error: DataStreamFifo constructor could not allocate " << bufferSize << " bytes of memory." << endl;
    }
    resetBuffer();
}

DataStreamFifo::~DataStreamFifo()
{
    delete [] buffer;
}

bool DataStreamFifo::writeToBuffer(unsigned char* dataSource, unsigned int numBytes)
{
    if (freeBytes.tryAcquire(numBytes)) {
        for (unsigned int i = 0; i < numBytes; ++i) {
            buffer[bufferWriteIndex++] = dataSource[i];
            if (bufferWriteIndex >= bufferSize) {
                bufferWriteIndex = 0;
            }
        }

        usedBytes.release(numBytes);
        return true;
    } else {
        return false;  // buffer overrun error
    }
}

bool DataStreamFifo::dataAvailable(unsigned int numBytes) const
{
    if ((unsigned int)(usedBytes.available()) >= numBytes) {
        return true;
    } else {
        return false;
    }
}

int DataStreamFifo::indexDistance() const
{
    return (bufferWriteIndex - bufferReadIndex) % bufferSize;
}

unsigned int DataStreamFifo::bytesAvailable() const
{
    return (unsigned int)usedBytes.available();
}

double DataStreamFifo::percentFull() const
{
    return 100.0 * ((double)usedBytes.available() / (double)bufferSize);
}

bool DataStreamFifo::readFromBuffer(unsigned char *dataSink, unsigned int numBytes)
{
    if (!usedBytes.tryAcquire(numBytes)) {
        return false;  // not enough data available in buffer
    }
    for (unsigned int i = 0; i < numBytes; ++i) {
        dataSink[i] = buffer[bufferReadIndex++];
        if (bufferReadIndex >= bufferSize) {
            bufferReadIndex = 0;
        }
    }

    freeBytes.release(numBytes);
    return true;
}

void DataStreamFifo::resetBuffer()
{
    freeBytes.acquire(freeBytes.available());
    usedBytes.acquire(usedBytes.available());
    bufferWriteIndex = 0;
    bufferReadIndex = 0;
    freeBytes.release(bufferSize);
}
