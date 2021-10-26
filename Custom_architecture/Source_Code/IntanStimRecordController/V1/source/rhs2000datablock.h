//----------------------------------------------------------------------------------
// rhs2000datablock.h
//
// Intan Technoloies RHS2000 Interface API
// Rhs2000DataBlock Class Header File
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

#ifndef RHS2000DATABLOCK_H
#define RHS2000DATABLOCK_H

#define SAMPLES_PER_DATA_BLOCK 128
#define CHANNELS_PER_STREAM 16
#define RHS2000_HEADER_MAGIC_NUMBER 0x8d542c8a49712f0b

using namespace std;

class Rhs2000EvalBoard;

class Rhs2000DataBlock
{
public:
	Rhs2000DataBlock(int numDataStreams);
    ~Rhs2000DataBlock();
    Rhs2000DataBlock(const Rhs2000DataBlock &obj); // copy constructor

	vector<unsigned int> timeStamp;
    int* amplifierDataFast;
    // vector<vector<vector<int> > > amplifierData;
	vector<vector<vector<int> > > dcAmplifierData;
	vector<vector<vector<int> > > auxiliaryData;
    vector<vector<vector<int> > > complianceLimit;
    vector<vector<int> > stimOn;
    vector<vector<int> > stimPol;
    vector<vector<int> > ampSettle;
    vector<vector<int> > chargeRecov;
    vector<vector<int> > boardDacData;
    vector<vector<int> > boardAdcData;
	vector<int> ttlIn;
	vector<int> ttlOut;
	
	static unsigned int calculateDataBlockSizeInWords(int numDataStreams);
	static unsigned int getSamplesPerDataBlock();
	void fillFromUsbBuffer(unsigned char usbBuffer[], int blockIndex, int numDataStreams);
	void print(int stream) const;
	void write(ofstream &saveOut, int numDataStreams) const;
	void writeToVector(vector<int> &dataOut) const;
    bool checkUsbHeader(unsigned char usbBuffer[], int index);
    inline int fastIndex(int stream, int channel, int t) const;

private:
	void allocateIntArray3D(vector<vector<vector<int> > > &array3D, int xSize, int ySize, int zSize);
	void allocateIntArray2D(vector<vector<int> > &array2D, int xSize, int ySize);
	void allocateIntArray1D(vector<int> &array1D, int xSize);
	void allocateUIntArray1D(vector<unsigned int> &array1D, int xSize);

	void writeWordLittleEndian(ofstream &outputStream, int dataWord) const;

    int numDataStreamsStored;
	unsigned int convertUsbTimeStamp(unsigned char usbBuffer[], int index);
	int convertUsbWord(unsigned char usbBuffer[], int index);
};

#endif // RHS2000DATABLOCK_H
