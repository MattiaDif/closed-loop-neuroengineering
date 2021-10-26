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

#include <iostream>
#include "usbdatathread.h"
#include "rhs2000datablock.h"

using namespace std;

UsbDataThread::UsbDataThread(Rhs2000EvalBoard* board_, DataStreamFifo* usbFifo_, QObject *parent) :
    QThread(parent),
    board(board_),
    usbFifo(usbFifo_)
{
    keepGoing = false;
    running = false;
    stopThread = false;
    numUsbBlocksToRead = 1;
    unsigned int bufferSize = BUFFER_SIZE_IN_BLOCKS * 2 * Rhs2000DataBlock::calculateDataBlockSizeInWords(MAX_NUM_DATA_STREAMS);
    cout << "UsbDataThread: Allocating " << bufferSize / 1.0e6 << " MBytes for USB buffer." << endl;
    usbBuffer = new unsigned char [bufferSize];
}

UsbDataThread::~UsbDataThread()
{
    delete [] usbBuffer;
}

void UsbDataThread::run()
{
    while (!stopThread) {
        if (keepGoing) {
            running = true;
            long numBytesRead;
            board->setStimCmdMode(true);
            board->setContinuousRunMode(true);
            board->setMaxTimeStep(0);
            board->run();
            while (keepGoing && !stopThread) {
                numBytesRead = board->readDataBlocksRaw(numUsbBlocksToRead, usbBuffer);
                if (numBytesRead > 0) {
                    if (!usbFifo->writeToBuffer(usbBuffer, (unsigned int)numBytesRead)) {
                        cerr << "UsbDataThread: USB buffer overrun!" << endl;
                    }
                } else {
                    usleep(100);  // wait 100 microseconds
                }
            }
            board->setContinuousRunMode(false);
            board->setStimCmdMode(false);
            board->setMaxTimeStep(0);
            board->flush();  // Flush USB FIFO on XEM6310
            running = false;
        } else {
            usleep(100);
        }
    }
}

void UsbDataThread::startRunning()
{
    keepGoing = true;
}

void UsbDataThread::stopRunning()
{
    keepGoing = false;
}

void UsbDataThread::close()
{
    keepGoing = false;
    stopThread = true;
}

bool UsbDataThread::isRunning() const
{
    return running;
}

void UsbDataThread::setNumUsbBlocksToRead(int numUsbBlocksToRead_)
{
    if (numUsbBlocksToRead_ > BUFFER_SIZE_IN_BLOCKS) {
        cerr << "UsbDataThread::setNumUsbBlocksToRead: Buffer is too small to read " << numUsbBlocksToRead_ <<
                " blocks.  Increase BUFFER_SIZE_IN_BLOCKS." << endl;
    }
    numUsbBlocksToRead = numUsbBlocksToRead_;
}
