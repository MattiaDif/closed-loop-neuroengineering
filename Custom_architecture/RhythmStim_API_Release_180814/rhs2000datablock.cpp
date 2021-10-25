//----------------------------------------------------------------------------------
// rhs2000datablock.cpp
//
// Intan Technoloies RHS2000 Interface API
// Rhs2000DataBlock Class
// Version 1.01 (28 March 2017)
//
// Copyright (c) 2013-2017 Intan Technologies LLC
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any applications that
// use Intan Technologies integrated circuits, and to alter it and redistribute it
// freely.
//
// See http://www.intantech.com for documentation and product information.
//----------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "rhs2000datablock.h"

using namespace std;

// This class creates a data structure storing SAMPLES_PER_DATA_BLOCK data frames
// from a Rhythm FPGA interface controlling up to eight RHS2116 chips.

// Constructor.  Allocates memory for data block.
Rhs2000DataBlock::Rhs2000DataBlock(int numDataStreams)
{
    numDataStreamsStored = numDataStreams;
	allocateUIntArray1D(timeStamp, SAMPLES_PER_DATA_BLOCK);
    // allocateIntArray3D(amplifierData, numDataStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK);
    amplifierDataFast = new int [numDataStreams * CHANNELS_PER_STREAM * SAMPLES_PER_DATA_BLOCK];
    allocateIntArray3D(dcAmplifierData, numDataStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK);
	allocateIntArray3D(auxiliaryData, numDataStreams, 4, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray3D(complianceLimit, numDataStreams, CHANNELS_PER_STREAM, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(stimOn, numDataStreams, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(stimPol, numDataStreams, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(ampSettle, numDataStreams, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(chargeRecov, numDataStreams, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(boardDacData, 8, SAMPLES_PER_DATA_BLOCK);
    allocateIntArray2D(boardAdcData, 8, SAMPLES_PER_DATA_BLOCK);
	allocateIntArray1D(ttlIn, SAMPLES_PER_DATA_BLOCK);
	allocateIntArray1D(ttlOut, SAMPLES_PER_DATA_BLOCK);
}

Rhs2000DataBlock::~Rhs2000DataBlock()
{
    delete [] amplifierDataFast;
}

// Copy constructor
Rhs2000DataBlock::Rhs2000DataBlock(const Rhs2000DataBlock &obj)
{
    int size = obj.numDataStreamsStored * CHANNELS_PER_STREAM * SAMPLES_PER_DATA_BLOCK;
    amplifierDataFast = new int [size];
    for (int i = 0; i < size; ++i) {
        amplifierDataFast[i] = obj.amplifierDataFast[i];
    }

    timeStamp = obj.timeStamp;
    // amplifierData = obj.amplifierData;
    dcAmplifierData = obj.dcAmplifierData; // TODO: convert dcAmplifierData to an int[], too, for speed, if necessary
    auxiliaryData = obj.auxiliaryData;
    complianceLimit = obj.complianceLimit;
    stimOn = obj.stimOn;
    stimPol = obj.stimPol;
    ampSettle = obj.ampSettle;
    chargeRecov = obj.chargeRecov;
    boardDacData = obj.boardDacData;
    boardAdcData = obj.boardAdcData;
    ttlIn = obj.ttlIn;
    ttlOut = obj.ttlOut;
    numDataStreamsStored = obj.numDataStreamsStored;
}

// Allocates memory for a 1-D array of integers.
void Rhs2000DataBlock::allocateIntArray1D(vector<int> &array1D, int xSize)
{
	array1D.resize(xSize);
}

// Allocates memory for a 1-D array of unsigned integers.
void Rhs2000DataBlock::allocateUIntArray1D(vector<unsigned int> &array1D, int xSize)
{
	array1D.resize(xSize);
}

// Allocates memory for a 2-D array of integers.
void Rhs2000DataBlock::allocateIntArray2D(vector<vector<int> > & array2D, int xSize, int ySize)
{
	int i;

	array2D.resize(xSize);
	for (i = 0; i < xSize; ++i)
		array2D[i].resize(ySize);
}

// Allocates memory for a 3-D array of integers.
void Rhs2000DataBlock::allocateIntArray3D(vector<vector<vector<int> > > &array3D, int xSize, int ySize, int zSize)
{
	int i, j;

	array3D.resize(xSize);
	for (i = 0; i < xSize; ++i) {
		array3D[i].resize(ySize);

		for (j = 0; j < ySize; ++j) {
			array3D[i][j].resize(zSize);
		}
	}
}

// Returns the number of samples in a USB data block.
unsigned int Rhs2000DataBlock::getSamplesPerDataBlock()
{
	return SAMPLES_PER_DATA_BLOCK;
}

// Returns the number of 16-bit words in a USB data block with numDataStreams data streams enabled.
unsigned int Rhs2000DataBlock::calculateDataBlockSizeInWords(int numDataStreams)
{
    return SAMPLES_PER_DATA_BLOCK * (4 + 2 + numDataStreams * (2 * (CHANNELS_PER_STREAM + 4) + 4) + 8 + 8 + 2);
    // 4 = magic number; 2 = time stamp; 20 = (16 amp channels + 4 aux commands, each 32 bit results);
    // 4 = stim control params; 8 = DACs; 8 = ADCs; 2 = TTL in/out
}

// Check first 64 bits of USB header against the fixed Rhythm "magic number" to verify data sync.
bool Rhs2000DataBlock::checkUsbHeader(unsigned char usbBuffer[], int index)
{
	unsigned long long x1, x2, x3, x4, x5, x6, x7, x8;
	unsigned long long header;

	x1 = usbBuffer[index];
	x2 = usbBuffer[index + 1];
	x3 = usbBuffer[index + 2];
	x4 = usbBuffer[index + 3];
	x5 = usbBuffer[index + 4];
	x6 = usbBuffer[index + 5];
	x7 = usbBuffer[index + 6];
	x8 = usbBuffer[index + 7];

	header = (x8 << 56) + (x7 << 48) + (x6 << 40) + (x5 << 32) + (x4 << 24) + (x3 << 16) + (x2 << 8) + (x1 << 0);

	return (header == RHS2000_HEADER_MAGIC_NUMBER);
}

// Read 32-bit time stamp from USB data frame.
unsigned int Rhs2000DataBlock::convertUsbTimeStamp(unsigned char usbBuffer[], int index)
{
	unsigned int x1, x2, x3, x4;
	x1 = usbBuffer[index];
	x2 = usbBuffer[index + 1];
	x3 = usbBuffer[index + 2];
	x4 = usbBuffer[index + 3];

	return (x4 << 24) + (x3 << 16) + (x2 << 8) + (x1 << 0);
}

// Convert two USB bytes into 16-bit word.
int Rhs2000DataBlock::convertUsbWord(unsigned char usbBuffer[], int index)
{
	unsigned int x1, x2, result;

	x1 = (unsigned int)usbBuffer[index];
	x2 = (unsigned int)usbBuffer[index + 1];

	result = (x2 << 8) | (x1 << 0);

	return (int)result;
}

// Fill data block with raw data from USB input buffer.
void Rhs2000DataBlock::fillFromUsbBuffer(unsigned char usbBuffer[], int blockIndex, int numDataStreams)
{
    int index, t, channel, stream, i, highWord;

    int ampIndex = 0;
	index = blockIndex * 2 * calculateDataBlockSizeInWords(numDataStreams);
	for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
		if (!checkUsbHeader(usbBuffer, index)) {
            cerr << "Error in Rhs2000EvalBoard::readDataBlock: Incorrect header." << endl;
		}
		index += 8;
		timeStamp[t] = convertUsbTimeStamp(usbBuffer, index);
		index += 4;

        // Read auxiliary command 1-3 results (see below for auxiliary command 0 results)
		for (channel = 1; channel < 4; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
                auxiliaryData[stream][channel][t] = convertUsbWord(usbBuffer, index);
				index += 2;
                if (channel == 2) {
                    highWord = convertUsbWord(usbBuffer,index); // The top 16 bits will be either all 1's (results of a WRITE command)
                                                                // or all 0's (results of a READ command)
                    if (highWord == 0) {  // update compliance limit only if a 'read' command was executed, denoting a read from Register 40
                        for (int ch = 0; ch < CHANNELS_PER_STREAM; ++ch) {
                            complianceLimit[stream][ch][t] = (auxiliaryData[stream][2][t] & (1 << ch)) ? 1 : 0;
                        }
                    } else {
                        for (int ch = 0; ch < CHANNELS_PER_STREAM; ++ch) {
                            complianceLimit[stream][ch][t] = 0;  // if Register 40 was not read, assume no compliance limit violations
                        }
                    }
                }
                index += 2;
			}
		}

		// Read amplifier channels 0-15
		for (channel = 0; channel < 16; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
                dcAmplifierData[stream][channel][t] = convertUsbWord(usbBuffer, index);  // lower 16 bits (10 bits, actually) contain DC amplifier results
				index += 2;
                // amplifierData[stream][channel][t] = convertUsbWord(usbBuffer, index);    // top 16 bits contain AC amplifier results
                amplifierDataFast[ampIndex++] = convertUsbWord(usbBuffer, index);
				index += 2;
			}
		}

        // Read auxiliary command 0 results (see above for auxiliary command 1-3 results)
		for (channel = 0; channel < 1; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
				auxiliaryData[stream][channel][t] = convertUsbWord(usbBuffer, index);
				index += 2;
				index += 2; // We are skipping the top 16 bits here since they will typically be either all 1's (results of a WRITE command)
							// or all 0's (results of a READ command).
			}
		}

        // Read stimulation control parameters
        for (stream = 0; stream < numDataStreams; ++stream) {
            stimOn[stream][t] = convertUsbWord(usbBuffer, index);
            index += 2;
        }

        for (stream = 0; stream < numDataStreams; ++stream) {
            stimPol[stream][t] = convertUsbWord(usbBuffer, index);
            index += 2;
        }

        for (stream = 0; stream < numDataStreams; ++stream) {
            ampSettle[stream][t] = convertUsbWord(usbBuffer, index);
            index += 2;
        }

        for (stream = 0; stream < numDataStreams; ++stream) {
            chargeRecov[stream][t] = convertUsbWord(usbBuffer, index);
            index += 2;
        }

        // Read from DACs
        for (i = 0; i < 8; ++i) {
            boardDacData[i][t] = convertUsbWord(usbBuffer, index);
            index += 2;
        }

        // Read from ADCs
		for (i = 0; i < 8; ++i) {
			boardAdcData[i][t] = convertUsbWord(usbBuffer, index);
			index += 2;
		}

		// Read TTL input and output values
		ttlIn[t] = convertUsbWord(usbBuffer, index);
		index += 2;

		ttlOut[t] = convertUsbWord(usbBuffer, index);
		index += 2;
	}
}

// Print the contents of RHS2116 registers from a selected USB data stream (0-7)
// to the console.
void Rhs2000DataBlock::print(int stream) const
{
	const int RomOffset = 57; // 56
	const int RamOffset = 62; // 61
	const int auxSlot = 0;

//	cout << endl;
//	cout << "Raw RHS 2000 Data Block contents:" << endl;
//	for (int i = 0; i < 128; i++) {
//		cout << "  Data postion " << i << ": " << auxiliaryData[stream][auxSlot][i] << endl;
//	}

	cout << endl;
	cout << "RHS 2000 Data Block contents:" << endl;
	cout << "  ROM contents:" << endl;
	cout << "    Company Name:          " <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 4] & 0xff00) >> 8) <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 4] & 0x00ff) >> 0) <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 3] & 0xff00) >> 8) <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 3] & 0x00ff) >> 0) <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 2] & 0xff00) >> 8) <<
		(char)((auxiliaryData[stream][auxSlot][RomOffset + 2] & 0x00ff) >> 0) << endl;
	cout << "    Intan Chip ID:         " << auxiliaryData[stream][auxSlot][RomOffset + 0] << endl;
	cout << "    Number of Amps:        " << (auxiliaryData[stream][auxSlot][RomOffset + 1] & 0x00ff) << endl;
	cout << "    Die Revision:          " << ((auxiliaryData[stream][auxSlot][RomOffset + 1] & 0xff00) >> 8) << endl;

	cout << "  RAM contents:" << endl;
	cout << "    ADC buffer bias:       " << ((auxiliaryData[stream][auxSlot][RamOffset + 0] & 0x0fc0) >> 6) << endl;
	cout << "    MUX bias:              " << ((auxiliaryData[stream][auxSlot][RamOffset + 0] & 0x003f) >> 0) << endl;
	cout << "    digoutOD:              " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x1000) >> 12) << endl;
	cout << "    digout2:               " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0800) >> 11) << endl;
	cout << "    digout2 HiZ:           " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0400) >> 10) << endl;
	cout << "    digout1:               " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0200) >> 9) << endl;
	cout << "    digout1 HiZ:           " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0100) >> 8) << endl;
	cout << "    weak MISO:             " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0080) >> 7) << endl;
	cout << "    twoscomp:              " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0040) >> 6) << endl;
	cout << "    absmode:               " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0020) >> 5) << endl;
	cout << "    DSPen:                 " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x0010) >> 4) << endl;
	cout << "    DSP cutoff freq:       " << ((auxiliaryData[stream][auxSlot][RamOffset + 1] & 0x000f) >> 0) << endl;
	cout << "    Zcheck select:         " << ((auxiliaryData[stream][auxSlot][RamOffset + 2] & 0x3f00) >> 8) << endl;
	cout << "    Zcheck DAC power:      " << ((auxiliaryData[stream][auxSlot][RamOffset + 2] & 0x0040) >> 6) << endl;
	cout << "    Zcheck load:           " << ((auxiliaryData[stream][auxSlot][RamOffset + 2] & 0x0020) >> 5) << endl;
	cout << "    Zcheck scale:          " << ((auxiliaryData[stream][auxSlot][RamOffset + 2] & 0x0018) >> 3) << endl;
	cout << "    Zcheck en:             " << ((auxiliaryData[stream][auxSlot][RamOffset + 2] & 0x0001) >> 0) << endl;
	cout << "    Zcheck DAC:            " << ((auxiliaryData[stream][auxSlot][RamOffset + 3] & 0x00ff) >> 0) << endl;

	int rH1Dac1 = (auxiliaryData[stream][auxSlot][RamOffset + 4] & 0x003f) >> 0;
	int rH1Dac2 = (auxiliaryData[stream][auxSlot][RamOffset + 4] & 0x07c0) >> 6;
	int rH2Dac1 = (auxiliaryData[stream][auxSlot][RamOffset + 5] & 0x003f) >> 0;
	int rH2Dac2 = (auxiliaryData[stream][auxSlot][RamOffset + 5] & 0x07c0) >> 6;
	int rLDac1A = (auxiliaryData[stream][auxSlot][RamOffset + 6] & 0x007f) >> 0;
	int rLDac2A = (auxiliaryData[stream][auxSlot][RamOffset + 6] & 0x1f8f) >> 7;
	int rLDac3A = (auxiliaryData[stream][auxSlot][RamOffset + 6] & 0x2000) >> 13;
	int rLDac1B = (auxiliaryData[stream][auxSlot][RamOffset + 7] & 0x007f) >> 0;
	int rLDac2B = (auxiliaryData[stream][auxSlot][RamOffset + 7] & 0x1f8f) >> 7;
	int rLDac3B = (auxiliaryData[stream][auxSlot][RamOffset + 7] & 0x2000) >> 13;

	double rH1 = 2630.0 + rH1Dac2 * 30800.0 + rH1Dac1 * 590.0;
	double rH2 = 8200.0 + rH2Dac2 * 38400.0 + rH2Dac1 * 730.0;
	double rLA = 3300.0 + rLDac3A * 3000000.0 + rLDac2A * 15400.0 + rLDac1A * 190.0;
	double rLB = 3300.0 + rLDac3B * 3000000.0 + rLDac2B * 15400.0 + rLDac1B * 190.0;

	cout << fixed << setprecision(2);

	cout << "    RH1 DAC1, DAC2:        " << rH1Dac1 << " " << rH1Dac2 << " = " << (rH1 / 1000) <<
		" kOhm" << endl;
	cout << "    RH2 DAC1, DAC2:        " << rH2Dac1 << " " << rH2Dac2 << " = " << (rH2 / 1000) <<
		" kOhm" << endl;
	cout << "    RL_A DAC1, DAC2, DAC3: " << rLDac1A << " " << rLDac2A << " " << rLDac3A << " = " <<
		(rLA / 1000) << " kOhm" << endl;
	cout << "    RL_B DAC1, DAC2, DAC3: " << rLDac1B << " " << rLDac2B << " " << rLDac3B << " = " <<
		(rLB / 1000) << " kOhm" << endl;

	cout << "    amp power[15:0]:       " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 8] & 0x0001) >> 0) << endl;

	cout << "    amp fast settle[15:0]: " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 9] & 0x0001) >> 0) << endl;

	cout << "    amp fL select[15:0]:   " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 10] & 0x0001) >> 0) << endl;

	cout << "    stim enable A (43690): " << ((auxiliaryData[stream][auxSlot][RamOffset + 11] & 0xffff) >> 0) << endl;
	cout << "    stim enable B (255):   " << ((auxiliaryData[stream][auxSlot][RamOffset + 12] & 0xffff) >> 0) << endl;
	cout << "    stim step size 1,2,3:  " << ((auxiliaryData[stream][auxSlot][RamOffset + 13] & 0x007f) >> 0) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 13] & 0x1f80) >> 7) << " " << 
		((auxiliaryData[stream][auxSlot][RamOffset + 13] & 0x6000) >> 13) << endl;
	cout << "    stim Pbias:            " << ((auxiliaryData[stream][auxSlot][RamOffset + 14] & 0x00f0) >> 4) << endl;
	cout << "    stim Nbias:            " << ((auxiliaryData[stream][auxSlot][RamOffset + 14] & 0x000f) >> 0) << endl;
	cout << "    charge recovery DAC:   " << ((auxiliaryData[stream][auxSlot][RamOffset + 15] & 0x00ff) >> 0) << endl;
	cout << "    current limit 1,2,3:   " << ((auxiliaryData[stream][auxSlot][RamOffset + 16] & 0x007f) >> 0) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 16] & 0x1f80) >> 7) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 16] & 0x6000) >> 13) << endl;

	cout << "    DC amp power[15:0]:    " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 17] & 0x0001) >> 0) << endl;

	cout << "    compliance mon[15:0]:  " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 18] & 0x0001) >> 0) << endl;

	cout << "    stim on[15:0]:         " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 19] & 0x0001) >> 0) << endl;

	cout << "    stim pol[15:0]:        " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 20] & 0x0001) >> 0) << endl;

	cout << "    charge recov sw[15:0]: " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 21] & 0x0001) >> 0) << endl;

	cout << "    CL recov en[15:0]:     " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x8000) >> 15) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x4000) >> 14) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x2000) >> 13) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x1000) >> 12) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0800) >> 11) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0400) >> 10) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0200) >> 9) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0100) >> 8) << " " <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0080) >> 7) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0040) >> 6) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0020) >> 5) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0010) >> 4) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0008) >> 3) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0004) >> 2) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0002) >> 1) <<
		((auxiliaryData[stream][auxSlot][RamOffset + 22] & 0x0001) >> 0) << endl;

	cout << "    fault current detect:  " << ((auxiliaryData[stream][auxSlot][RamOffset + 23] & 0xffff) >> 0) << endl;
	
	for (int channel = 0; channel < 16; channel++) {
		cout << "    stim magnitude/trim neg/pos[" << channel << "]: " <<
			((auxiliaryData[stream][auxSlot][RamOffset + 24 + channel] & 0x00ff) >> 0) << " " <<
			((auxiliaryData[stream][auxSlot][RamOffset + 24 + channel] & 0xff00) >> 8) << "   " <<
			((auxiliaryData[stream][auxSlot][RamOffset + 40 + channel] & 0x00ff) >> 0) << " " <<
			((auxiliaryData[stream][auxSlot][RamOffset + 40 + channel] & 0xff00) >> 8) << " " << endl;
	}
	
	cout << endl;

	cout << setprecision(6);
	cout.unsetf(ios::floatfield);
	cout << endl;
}

// Write a 16-bit dataWord to an outputStream in "little endian" format (i.e., least significant
// byte first).  We must do this explicitly for cross-platform consistency.  For example, Windows
// is a little-endian OS, while Mac OS X and Linux can be little-endian or big-endian depending on
// the processor running the operating system.
//
// (See "Endianness" article in Wikipedia for more information.)
void Rhs2000DataBlock::writeWordLittleEndian(ofstream &outputStream, int dataWord) const
{
	unsigned short msb, lsb;

	lsb = ((unsigned short)dataWord) & 0x00ff;
	msb = (((unsigned short)dataWord) & 0xff00) >> 8;

	outputStream << (unsigned char)lsb;
	outputStream << (unsigned char)msb;
}

// Write contents of data block to a binary output stream (saveOut) in little endian format.
void Rhs2000DataBlock::write(ofstream &saveOut, int numDataStreams) const
{
	int t, channel, stream, i;

	for (t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
		writeWordLittleEndian(saveOut, timeStamp[t]);
		for (channel = 0; channel < 16; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
                writeWordLittleEndian(saveOut, amplifierDataFast[fastIndex(stream, channel, t)]);
			}
		}
		for (channel = 0; channel < 16; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
				writeWordLittleEndian(saveOut, dcAmplifierData[stream][channel][t]);
			}
		}
		for (channel = 0; channel < 4; ++channel) {
			for (stream = 0; stream < numDataStreams; ++stream) {
				writeWordLittleEndian(saveOut, auxiliaryData[stream][channel][t]);
			}
		}
        for (i = 0; i < 8; ++i) {
            writeWordLittleEndian(saveOut, boardDacData[i][t]);
        }
        for (i = 0; i < 8; ++i) {
			writeWordLittleEndian(saveOut, boardAdcData[i][t]);
		}
		writeWordLittleEndian(saveOut, ttlIn[t]);
		writeWordLittleEndian(saveOut, ttlOut[t]);
	}
}

// Write ADC sample data from of data block from RHS2116 to a vector.
void Rhs2000DataBlock::writeToVector(vector<int> &dataOut) const
{
	for (int t = 0; t < SAMPLES_PER_DATA_BLOCK; ++t) {
        dataOut.push_back(amplifierDataFast[fastIndex(0, 0, t)]);
	}
}

// Inline function to address data stored in amplifierDataFast[] array
inline int Rhs2000DataBlock::fastIndex(int stream, int channel, int t) const
{
    return ((t * numDataStreamsStored * CHANNELS_PER_STREAM) + (channel * numDataStreamsStored) + stream);
}
