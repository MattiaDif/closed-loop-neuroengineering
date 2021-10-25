//----------------------------------------------------------------------------------
// rhs2000evalboard.cpp
//
// Intan Technoloies RHS2000 Interface API
// Rhs2000EvalBoard Class
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
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <mutex>

#include "rhs2000evalboard.h"
#include "rhs2000datablock.h"
#include "rhs2000registers.h"

#include "okFrontPanelDLL.h"

using namespace std;

// This class provides access to and control of the Opal Kelly XEM6010 USB/FPGA
// interface board running the Rhythm interface Verilog code.

// Constructor.  Set sampling rate variable to 30.0 kS/s/channel (FPGA default).
Rhs2000EvalBoard::Rhs2000EvalBoard()
{
	int i;
    usbBufferSize = MAX_NUM_BLOCKS * 2 * Rhs2000DataBlock::calculateDataBlockSizeInWords(MAX_NUM_DATA_STREAMS);
    cout << "Rhs2000EvalBoard: Allocating " << usbBufferSize / 1.0e6 << " MBytes for USB buffer." << endl;
    usbBuffer = new unsigned char [usbBufferSize];
	sampleRate = SampleRate30000Hz; // Rhythm FPGA boots up with 30.0 kS/s/channel sampling rate
	numDataStreams = 0;

	for (i = 0; i < MAX_NUM_DATA_STREAMS; ++i) {
		dataStreamEnabled[i] = 0;
	}

    cableDelay.resize(MAX_NUM_SPI_PORTS, -1);
    lastNumWordsInFifo = 0;
    numWordsHasBeenUpdated = false;
}

Rhs2000EvalBoard::~Rhs2000EvalBoard()
{
    delete [] usbBuffer;
}

// Find an Opal Kelly XEM6010-LX45 board attached to a USB port and open it.
// Returns 1 if successful, -1 if FrontPanel cannot be loaded, and -2 if XEM6010 can't be found.
int Rhs2000EvalBoard::open()
{
    lock_guard<mutex> lockOk(okMutex);
	char dll_date[32], dll_time[32];
	string serialNumber = "";
	int i, nDevices;

    cout << "---- Intan Technologies ---- Rhythm Stim RHS2000 Controller v1.0 ----" << endl << endl;
	if (okFrontPanelDLL_LoadLib(NULL) == false) {
		cerr << "FrontPanel DLL could not be loaded.  " <<
			"Make sure this DLL is in the application start directory." << endl;
		return -1;
	}
	okFrontPanelDLL_GetVersion(dll_date, dll_time);
	cout << endl << "FrontPanel DLL loaded.  Built: " << dll_date << "  " << dll_time << endl;

	dev = new okCFrontPanel;

	cout << endl << "Scanning USB for Opal Kelly devices..." << endl << endl;
	nDevices = dev->GetDeviceCount();
	cout << "Found " << nDevices << " Opal Kelly device" << ((nDevices == 1) ? "" : "s") <<
		" connected:" << endl;
	for (i = 0; i < nDevices; ++i) {
		cout << "  Device #" << i + 1 << ": Opal Kelly " <<
			opalKellyModelName(dev->GetDeviceListModel(i)).c_str() <<
			" with serial number " << dev->GetDeviceListSerial(i).c_str() << endl;
	}
	cout << endl;

	// Find first device in list of type XEM6010LX45.
	for (i = 0; i < nDevices; ++i) {
		if (dev->GetDeviceListModel(i) == OK_PRODUCT_XEM6010LX45) {
			serialNumber = dev->GetDeviceListSerial(i);
			break;
		}
	}

    if (serialNumber == "") {
        cerr << "No XEM6010-LX45 Opal Kelly board found." << endl;
        return -2;
    }

	// Attempt to open device.
	if (dev->OpenBySerial(serialNumber) != okCFrontPanel::NoError) {
		delete dev;
		cerr << "Device could not be opened.  Is one connected?" << endl;
		return -2;
	}

	// Configure the on-board PLL appropriately.
	dev->LoadDefaultPLLConfiguration();

	// Get some general information about the XEM.
	cout << "FPGA system clock: " << getSystemClockFreq() << " MHz" << endl; // Should indicate 100 MHz
	cout << "Opal Kelly device firmware version: " << dev->GetDeviceMajorVersion() << "." <<
		dev->GetDeviceMinorVersion() << endl;
	cout << "Opal Kelly device serial number: " << dev->GetSerialNumber().c_str() << endl;
	cout << "Opal Kelly device ID string: " << dev->GetDeviceID().c_str() << endl << endl;

	return 1;
}

// Uploads the configuration file (bitfile) to the FPGA.  Returns true if successful.
bool Rhs2000EvalBoard::uploadFpgaBitfile(string filename)
{
    lock_guard<mutex> lockOk(okMutex);
	okCFrontPanel::ErrorCode errorCode = dev->ConfigureFPGA(filename);

	switch (errorCode) {
	case okCFrontPanel::NoError:
		break;
	case okCFrontPanel::DeviceNotOpen:
		cerr << "FPGA configuration failed: Device not open." << endl;
		return(false);
	case okCFrontPanel::FileError:
		cerr << "FPGA configuration failed: Cannot find configuration file." << endl;
		return(false);
	case okCFrontPanel::InvalidBitstream:
		cerr << "FPGA configuration failed: Bitstream is not properly formatted." << endl;
		return(false);
	case okCFrontPanel::DoneNotHigh:
		cerr << "FPGA configuration failed: FPGA DONE signal did not assert after configuration." << endl;
		return(false);
	case okCFrontPanel::TransferError:
		cerr << "FPGA configuration failed: USB error occurred during download." << endl;
		return(false);
	case okCFrontPanel::CommunicationError:
		cerr << "FPGA configuration failed: Communication error with firmware." << endl;
		return(false);
	case okCFrontPanel::UnsupportedFeature:
		cerr << "FPGA configuration failed: Unsupported feature." << endl;
		return(false);
	default:
		cerr << "FPGA configuration failed: Unknown error." << endl;
		return(false);
	}

	// Check for Opal Kelly FrontPanel support in the FPGA configuration.
	if (dev->IsFrontPanelEnabled() == false) {
		cerr << "Opal Kelly FrontPanel support is not enabled in this FPGA configuration." << endl;
		delete dev;
		return(false);
	}

	int boardId, boardVersion;
	dev->UpdateWireOuts();
	boardId = dev->GetWireOutValue(WireOutBoardId);
	boardVersion = dev->GetWireOutValue(WireOutBoardVersion);

	if (boardId != RHYTHM_BOARD_ID) {
		cerr << "FPGA configuration does not support Rhythm.  Incorrect board ID: " << boardId << endl;
		return(false);
	}
	else {
        cout << "Rhythm Stim configuration file successfully loaded.  Rhythm Stim version number: " <<
			boardVersion << endl << endl;
	}

	return(true);
}

// Reads system clock frequency from Opal Kelly board (in MHz).  Should be 100 MHz for normal
// Rhythm operation.
double Rhs2000EvalBoard::getSystemClockFreq() const
{
	// Read back the CY22393 PLL configuation
	okCPLL22393 pll;
	dev->GetEepromPLL22393Configuration(pll);

	return pll.GetOutputFrequency(0);
}

// Initialize Rhythm FPGA to default starting values.
void Rhs2000EvalBoard::initialize()
{
	int i;

	resetBoard();
    enableAuxCommandsOnAllStreams();
    setGlobalSettlePolicy(false, false, false, false, false);
    setTtlOutMode(false, false, false, false, false, false, false, false);
	setSampleRate(SampleRate30000Hz);
    selectAuxCommandLength(AuxCmd1, 0, 0);
    selectAuxCommandLength(AuxCmd2, 0, 0);
    selectAuxCommandLength(AuxCmd3, 0, 0);
    selectAuxCommandLength(AuxCmd4, 0, 0);
    setContinuousRunMode(true);
    setStimCmdMode(false);
	setMaxTimeStep(4294967295);  // 4294967295 == (2^32 - 1)

	setCableLengthFeet(PortA, 3.0);  // assume 3 ft cables
	setCableLengthFeet(PortB, 3.0);
	setCableLengthFeet(PortC, 3.0);
	setCableLengthFeet(PortD, 3.0);

	setDspSettle(false);

    // Must first force all data streams off
    dev->SetWireInValue(WireInDataStreamEn, 0x0000);
    dev->UpdateWireIns();

	enableDataStream(0, true);        // start with only one data stream enabled
	for (i = 1; i < MAX_NUM_DATA_STREAMS; i++) {
		enableDataStream(i, false);
	}

    enableDcAmpConvert(true);
	setExtraStates(0);

    enableDac(0, false);
	enableDac(1, false);
	enableDac(2, false);
	enableDac(3, false);
	enableDac(4, false);
	enableDac(5, false);
	enableDac(6, false);
	enableDac(7, false);
	selectDacDataStream(0, 0);
	selectDacDataStream(1, 0);
	selectDacDataStream(2, 0);
	selectDacDataStream(3, 0);
	selectDacDataStream(4, 0);
	selectDacDataStream(5, 0);
	selectDacDataStream(6, 0);
	selectDacDataStream(7, 0);
	selectDacDataChannel(0, 0);
	selectDacDataChannel(1, 0);
	selectDacDataChannel(2, 0);
	selectDacDataChannel(3, 0);
	selectDacDataChannel(4, 0);
	selectDacDataChannel(5, 0);
	selectDacDataChannel(6, 0);
	selectDacDataChannel(7, 0);

	setDacManual(32768);    // midrange value = 0 V

    setDacGain(0);
    setAudioNoiseSuppress(0);

    setDacThreshold(0, 32768, true);
    setDacThreshold(1, 32768, true);
    setDacThreshold(2, 32768, true);
    setDacThreshold(3, 32768, true);
    setDacThreshold(4, 32768, true);
    setDacThreshold(5, 32768, true);
    setDacThreshold(6, 32768, true);
    setDacThreshold(7, 32768, true);

    enableDacReref(false);

    setAnalogInTriggerThreshold(1.65); // +1.65 V

    const int NEVER = 65535;
    int stream = 0;
    int channel = 0;

    for (stream = 0; stream < MAX_NUM_DATA_STREAMS; stream++) {
        for (channel = 0; channel < CHANNELS_PER_STREAM; channel++) {
            configureStimTrigger(stream, channel, 0, false, true, false);
            configureStimPulses(stream, channel, 1, Biphasic, true);
            programStimReg(stream, channel, EventAmpSettleOn, NEVER);
            programStimReg(stream, channel, EventStartStim, NEVER);
            programStimReg(stream, channel, EventStimPhase2, NEVER);
            programStimReg(stream, channel, EventStimPhase3, NEVER);
            programStimReg(stream, channel, EventEndStim, NEVER);
            programStimReg(stream, channel, EventRepeatStim, NEVER);
            programStimReg(stream, channel, EventAmpSettleOff, NEVER);
            programStimReg(stream, channel, EventChargeRecovOn, NEVER);
            programStimReg(stream, channel, EventChargeRecovOff, NEVER);
            programStimReg(stream, channel, EventAmpSettleOnRepeat, NEVER);
            programStimReg(stream, channel, EventAmpSettleOffRepeat, NEVER);
            programStimReg(stream, channel, EventEnd, 65534);
        }
    }

    // Configure analog output sequencers
    for (stream = 8; stream < 16; stream++) {
        configureStimTrigger(stream, 0, 0, false, true, false);
        configureStimPulses(stream, 0, 1, Monophasic, true);
        programStimReg(stream, 0, EventStartStim, 0);
        programStimReg(stream, 0, EventStimPhase2, NEVER);
        programStimReg(stream, 0, EventStimPhase3, NEVER);
        programStimReg(stream, 0, EventEndStim, 200);
        programStimReg(stream, 0, EventRepeatStim, NEVER);
        programStimReg(stream, 0, EventEnd, 240);
        programStimReg(stream, 0, DacBaseline, 32678);
        programStimReg(stream, 0, DacPositive, 32678 + 3200);
        programStimReg(stream, 0, DacNegative, 32678 - 3200);
    }

    // Configure digital output sequencers
    stream = 16;
    for (channel = 0; channel < 16; channel++) {
        configureStimTrigger(stream, channel, 0, false, true, false);
        configureStimPulses(stream, channel, 3, Biphasic, true);
        programStimReg(stream, channel, EventStartStim, NEVER);
        programStimReg(stream, channel, EventEndStim, NEVER);
        programStimReg(stream, channel, EventRepeatStim, NEVER);
        programStimReg(stream, channel, EventEnd, 65534);
    }

    setAmpSettleMode(false); // set amp_settle_mode (0 = amplifier low frequency cutoff select; 1 = amplifier fast settle)
    setChargeRecoveryMode(false); // set charge_recov_mode (0 = current-limited charge recovery drivers; 1 = charge recovery switch)
}

// Set the per-channel sampling rate of the RHS2116 chips connected to the FPGA.
bool Rhs2000EvalBoard::setSampleRate(AmplifierSampleRate newSampleRate)
{
    lock_guard<mutex> lockOk(okMutex);

	// Assuming a 100 MHz reference clock is provided to the FPGA, the programmable FPGA clock frequency
	// is given by:
	//
	//       FPGA internal clock frequency = 100 MHz * (M/D) / 2
	//
	// M and D are "multiply" and "divide" integers used in the FPGA's digital clock manager (DCM) phase-
	// locked loop (PLL) frequency synthesizer, and are subject to the following restrictions:
	//
	//                M must have a value in the range of 2 - 256
	//                D must have a value in the range of 1 - 256
	//                M/D must fall in the range of 0.05 - 3.33
	//
	// (See pages 85-86 of Xilinx document UG382 "Spartan-6 FPGA Clocking Resources" for more details.)
	//
	// This variable-frequency clock drives the state machine that controls all SPI communication
	// with the RHS2000 chips.  A complete SPI cycle (consisting of one CS pulse and 16 SCLK pulses)
	// takes 80 clock cycles.  The SCLK period is 4 clock cycles; the CS pulse is high for 14 clock
	// cycles between commands.
	//
	// Rhythm samples all 32 channels and then executes 3 "auxiliary" commands that can be used to read
	// and write from other registers on the chip, or to sample from the temperature sensor or auxiliary ADC
	// inputs, for example.  Therefore, a complete cycle that samples from each amplifier channel takes
	// 80 * (32 + 3) = 80 * 35 = 2800 clock cycles.
	//
	// So the per-channel sampling rate of each amplifier is 2800 times slower than the clock frequency.
	//
	// Based on these design choices, we can use the following values of M and D to generate the following
	// useful amplifier sampling rates for electrophsyiological applications:
	//
	//   M    D     clkout frequency    per-channel sample rate     per-channel sample period
	//  ---  ---    ----------------    -----------------------     -------------------------
	//    7  125          2.80 MHz               1.00 kS/s                 1000.0 usec = 1.0 msec
	//    7  100          3.50 MHz               1.25 kS/s                  800.0 usec
	//   21  250          4.20 MHz               1.50 kS/s                  666.7 usec
	//   14  125          5.60 MHz               2.00 kS/s                  500.0 usec
	//   35  250          7.00 MHz               2.50 kS/s                  400.0 usec
	//   21  125          8.40 MHz               3.00 kS/s                  333.3 usec
	//   14   75          9.33 MHz               3.33 kS/s                  300.0 usec
	//   28  125         11.20 MHz               4.00 kS/s                  250.0 usec
	//    7   25         14.00 MHz               5.00 kS/s                  200.0 usec
	//    7   20         17.50 MHz               6.25 kS/s                  160.0 usec
	//  112  250         22.40 MHz               8.00 kS/s                  125.0 usec
	//   14   25         28.00 MHz              10.00 kS/s                  100.0 usec
	//    7   10         35.00 MHz              12.50 kS/s                   80.0 usec
	//   21   25         42.00 MHz              15.00 kS/s                   66.7 usec
	//   28   25         56.00 MHz              20.00 kS/s                   50.0 usec
	//   35   25         70.00 MHz              25.00 kS/s                   40.0 usec
	//   42   25         84.00 MHz              30.00 kS/s                   33.3 usec
    //   56   25        112.00 MHz              40.00 kS/s (*)               25.0 usec
    //
    //  * The FPGA does not work reliably at a per-channel sample rate of 40 kS/s.
    //
	// To set a new clock frequency, assert new values for M and D (e.g., using okWireIn modules) and
	// pulse DCM_prog_trigger high (e.g., using an okTriggerIn module).  If this module is reset, it
	// reverts to a per-channel sampling rate of 30.0 kS/s.

	unsigned long M, D;

	switch (newSampleRate) {
	case SampleRate1000Hz:
		M = 7;
		D = 125;
		break;
	case SampleRate1250Hz:
		M = 7;
		D = 100;
		break;
	case SampleRate1500Hz:
		M = 21;
		D = 250;
		break;
	case SampleRate2000Hz:
		M = 14;
		D = 125;
		break;
	case SampleRate2500Hz:
		M = 35;
		D = 250;
		break;
	case SampleRate3000Hz:
		M = 21;
		D = 125;
		break;
	case SampleRate3333Hz:
		M = 14;
		D = 75;
		break;
	case SampleRate4000Hz:
		M = 28;
		D = 125;
		break;
	case SampleRate5000Hz:
		M = 7;
		D = 25;
		break;
	case SampleRate6250Hz:
		M = 7;
		D = 20;
		break;
	case SampleRate8000Hz:
		M = 112;
		D = 250;
		break;
	case SampleRate10000Hz:
		M = 14;
		D = 25;
		break;
	case SampleRate12500Hz:
		M = 7;
		D = 10;
		break;
	case SampleRate15000Hz:
		M = 21;
		D = 25;
		break;
	case SampleRate20000Hz:
		M = 28;
		D = 25;
		break;
	case SampleRate25000Hz:
		M = 35;
		D = 25;
		break;
	case SampleRate30000Hz:
		M = 42;
		D = 25;
		break;
    case SampleRate40000Hz: // Note: This sample rate does not work reliably in the FPGA
		M = 56;
		D = 25;
		break;
	default:
		return(false);
	}

	sampleRate = newSampleRate;

	// Wait for DcmProgDone = 1 before reprogramming clock synthesizer
	while (isDcmProgDone() == false) {}

	// Reprogram clock synthesizer
	dev->SetWireInValue(WireInDataFreqPll, (256 * M + D));
	dev->UpdateWireIns();
	dev->ActivateTriggerIn(TrigInDcmProg, 0);

	// Wait for DataClkLocked = 1 before allowing data acquisition to continue
	while (isDataClockLocked() == false) {}

	return(true);
}

// Returns the current per-channel sampling rate (in Hz) as a floating-point number.
double Rhs2000EvalBoard::getSampleRate() const
{
	switch (sampleRate) {
	case SampleRate1000Hz:
		return 1000.0;
		break;
	case SampleRate1250Hz:
		return 1250.0;
		break;
	case SampleRate1500Hz:
		return 1500.0;
		break;
	case SampleRate2000Hz:
		return 2000.0;
		break;
	case SampleRate2500Hz:
		return 2500.0;
		break;
	case SampleRate3000Hz:
		return 3000.0;
		break;
	case SampleRate3333Hz:
		return (10000.0 / 3.0);
		break;
	case SampleRate4000Hz:
		return 4000.0;
		break;
	case SampleRate5000Hz:
		return 5000.0;
		break;
	case SampleRate6250Hz:
		return 6250.0;
		break;
	case SampleRate8000Hz:
		return 8000.0;
		break;
	case SampleRate10000Hz:
		return 10000.0;
		break;
	case SampleRate12500Hz:
		return 12500.0;
		break;
	case SampleRate15000Hz:
		return 15000.0;
		break;
	case SampleRate20000Hz:
		return 20000.0;
		break;
	case SampleRate25000Hz:
		return 25000.0;
		break;
	case SampleRate30000Hz:
		return 30000.0;
		break;
    case SampleRate40000Hz:  // Note: This sample rate does not work reliably in the FPGA
		return 40000.0;
		break;
	default:
		return -1.0;
	}
}

Rhs2000EvalBoard::AmplifierSampleRate Rhs2000EvalBoard::getSampleRateEnum() const
{
	return sampleRate;
}

// Print a command list to the console in readable form.
void Rhs2000EvalBoard::printCommandList(const vector<unsigned int> &commandList) const
{
	unsigned int i;
	int channel, reg, data, uFlag, mFlag, dFlag, hFlag;
	unsigned int cmd;

	cout << endl;
	for (i = 0; i < commandList.size(); ++i) {
		cmd = commandList[i];
		channel = (cmd & 0x003f0000) >> 16;
		uFlag = (cmd & 0x20000000) >> 29;
		mFlag = (cmd & 0x10000000) >> 28;
		dFlag = (cmd & 0x08000000) >> 27;
		hFlag = (cmd & 0x04000000) >> 26;
		reg = (cmd & 0x00ff0000) >> 16;
		data = (cmd & 0x0000ffff);

		if ((cmd & 0xc0000000) == 0x00000000) {
			cout << "  command[" << i << "] = CONVERT(" << channel << "), UMDH=" << uFlag << mFlag << dFlag << hFlag << endl;
		}
		else if ((cmd & 0xc0000000) == 0xc0000000) {
			cout << "  command[" << i << "] = READ(" << reg << "), UM=" << uFlag << mFlag << endl;
		}
		else if ((cmd & 0xc0000000) == 0x80000000) {
			cout << "  command[" << i << "] = WRITE(" << reg << ",";
			cout << hex << uppercase << internal << setfill('0') << setw(4) << data << nouppercase << dec;
			cout << "), UM=" << uFlag << mFlag << endl;
		}
		else if (cmd == 0x55000000) {
			cout << "  command[" << i << "] = CALIBRATE" << endl;
		}
		else if (cmd == 0x6a000000) {
			cout << "  command[" << i << "] = CLEAR" << endl;
		}
		else {
			cout << "  command[" << i << "] = INVALID COMMAND: ";
			cout << hex << uppercase << internal << setfill('0') << setw(8) << cmd << nouppercase << dec;
			cout << endl;
		}
	}
	cout << endl;
}

// Specify a command sequence length (endIndex = 0-8191) and command loop index (0-8191) for all
// auxiliary command slots (AuxCmd1, AuxCmd2, AuxCmd3, and AuxCmd4).
void Rhs2000EvalBoard::selectAuxCommandLength(AuxCmdSlot auxCommandSlot, int loopIndex, int endIndex)
{
    lock_guard<mutex> lockOk(okMutex);
	if (loopIndex < 0 || loopIndex > 8191) {
		cerr << "Error in Rhs2000EvalBoard::selectAuxCommandLength: loopIndex out of range." << endl;
		return;
	}

	if (endIndex < 0 || endIndex > 8191) {
		cerr << "Error in Rhs2000EvalBoard::selectAuxCommandLength: endIndex out of range." << endl;
		return;
	}

    int auxCommandIndex = (int)auxCommandSlot;
    if (auxCommandIndex < 0 || auxCommandIndex > 3) {
        cerr << "Error in Rhs2000EvalBoard::selectAuxCommandLength: auxCommandSlot out of range." << endl;
    }

    dev->SetWireInValue(WireInMultiUse, loopIndex);
    dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInAuxCmdLength, auxCommandIndex + 4);
    dev->SetWireInValue(WireInMultiUse, endIndex);
    dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInAuxCmdLength, auxCommandIndex);
}

// Reset FPGA.  This clears all auxiliary command RAM banks, clears the USB FIFO, and resets the
// per-channel sampling rate to 30.0 kS/s/ch.
void Rhs2000EvalBoard::resetBoard()
{
    lock_guard<mutex> lockOk(okMutex);

	dev->SetWireInValue(WireInResetRun, 0x01, 0x01);
	dev->UpdateWireIns();
	dev->SetWireInValue(WireInResetRun, 0x00, 0x01);
	dev->UpdateWireIns();
}

// Low-level FPGA reset.  Call when closing application to make sure everything has stopped.
void Rhs2000EvalBoard::resetFpga()
{
    lock_guard<mutex> lockOk(okMutex);

    dev->ResetFPGA();
}

// Set the FPGA to run continuously once started (if continuousMode == true) or to run until
// maxTimeStep is reached (if continuousMode == false).
void Rhs2000EvalBoard::setContinuousRunMode(bool continuousMode)
{
    lock_guard<mutex> lockOk(okMutex);

    if (continuousMode) {
		dev->SetWireInValue(WireInResetRun, 0x02, 0x02);
	}
	else {
		dev->SetWireInValue(WireInResetRun, 0x00, 0x02);
	}
	dev->UpdateWireIns();
}

// Set maxTimeStep for cases where continuousMode == false.
void Rhs2000EvalBoard::setMaxTimeStep(unsigned int maxTimeStep)
{
    lock_guard<mutex> lockOk(okMutex);

    unsigned int maxTimeStepLsb, maxTimeStepMsb;

	maxTimeStepLsb = maxTimeStep & 0x0000ffff;
	maxTimeStepMsb = maxTimeStep & 0xffff0000;

	dev->SetWireInValue(WireInMaxTimeStepLsb, maxTimeStepLsb);
	dev->SetWireInValue(WireInMaxTimeStepMsb, maxTimeStepMsb >> 16);
	dev->UpdateWireIns();
}

// Initiate SPI data acquisition.
void Rhs2000EvalBoard::run()
{
    lock_guard<mutex> lockOk(okMutex);

    dev->ActivateTriggerIn(TrigInSpiStart, 0);
}

// Is the FPGA currently running?
bool Rhs2000EvalBoard::isRunning()
{
    lock_guard<mutex> lockOk(okMutex);
    int value;

	dev->UpdateWireOuts();
	value = dev->GetWireOutValue(WireOutSpiRunning);

    // update number of words in FIFO while we're at it
    lastNumWordsInFifo = (dev->GetWireOutValue(WireOutNumWordsMsb) << 16) + dev->GetWireOutValue(WireOutNumWordsLsb);
    numWordsHasBeenUpdated = true;

	if ((value & 0x01) == 0) {
		return false;
	}
	else {
		return true;
	}
}

// Returns the number of 16-bit words in the USB FIFO.  The user should never attempt to read
// more data than the FIFO currently contains, as it is not protected against underflow.
// (Private method.)
unsigned int Rhs2000EvalBoard::numWordsInFifo()
{
	dev->UpdateWireOuts();
    lastNumWordsInFifo = (dev->GetWireOutValue(WireOutNumWordsMsb) << 16) + dev->GetWireOutValue(WireOutNumWordsLsb);
    numWordsHasBeenUpdated = true;
    return lastNumWordsInFifo;
}

// Returns the number of 16-bit words in the USB FIFO.  The user should never attempt to read
// more data than the FIFO currently contains, as it is not protected against underflow.
// (Public, threadsafe method.)
unsigned int Rhs2000EvalBoard::getNumWordsInFifo()
{
    lock_guard<mutex> lockOk(okMutex);

    return numWordsInFifo();
}

// Returns the most recently mesaured number of 16-bit words in the USB FIFO.  Does not directly
// read this value from the USB port, and so may be out of date, but does not have to wait on
// other USB access to finish in order to execute.
unsigned int Rhs2000EvalBoard::getLastNumWordsInFifo()
{
    numWordsHasBeenUpdated = false;
    return lastNumWordsInFifo;
}

// Returns the most recently mesaured number of 16-bit words in the USB FIFO.  Does not directly
// read this value from the USB port, and so may be out of date, but does not have to wait on
// other USB access to finish in order to execute.  The boolean variable hasBeenUpdated indicates
// if this value has been updated since the last time this function was called.
unsigned int Rhs2000EvalBoard::getLastNumWordsInFifo(bool& hasBeenUpdated)
{
    hasBeenUpdated = numWordsHasBeenUpdated;
    numWordsHasBeenUpdated = false;
    return lastNumWordsInFifo;
}

// Returns the number of 16-bit words the USB SDRAM FIFO can hold.  The FIFO can actually hold a few
// thousand words more than the number returned by this method due to FPGA "mini-FIFOs" interfacing
// with the SDRAM, but this provides a conservative estimate of FIFO capacity.
unsigned int Rhs2000EvalBoard::fifoCapacityInWords()
{
	return FIFO_CAPACITY_WORDS;
}

// Set the delay for sampling the MISO line on a particular SPI port (PortA - PortD), in integer clock
// steps, where each clock step is 1/2800 of a per-channel sampling period.
// Note: Cable delay must be updated after sampleRate is changed, since cable delay calculations are
// based on the clock frequency!
void Rhs2000EvalBoard::setCableDelay(BoardPort port, int delay)
{
    lock_guard<mutex> lockOk(okMutex);
    int bitShift;

	if (delay < 0 || delay > 15) {
		cerr << "Warning in Rhs2000EvalBoard::setCableDelay: delay out of range: " << delay << endl;
	}

	if (delay < 0) delay = 0;
	if (delay > 15) delay = 15;

	switch (port) {
	case PortA:
		bitShift = 0;
		cableDelay[0] = delay;
		break;
	case PortB:
		bitShift = 4;
		cableDelay[1] = delay;
		break;
	case PortC:
		bitShift = 8;
		cableDelay[2] = delay;
		break;
	case PortD:
		bitShift = 12;
		cableDelay[3] = delay;
		break;
	default:
        cerr << "Error in Rhs2000EvalBoard::setCableDelay: unknown port." << endl;
	}

	dev->SetWireInValue(WireInMisoDelay, delay << bitShift, 0x000f << bitShift);
	dev->UpdateWireIns();
}

// Set the delay for sampling the MISO line on a particular SPI port (PortA - PortD) based on the length
// of the cable between the FPGA and the RHS2116 chip (in meters).
// Note: Cable delay must be updated after sampleRate is changed, since cable delay calculations are
// based on the clock frequency!
void Rhs2000EvalBoard::setCableLengthMeters(BoardPort port, double lengthInMeters)
{
	int delay;
	double tStep, cableVelocity, distance, timeDelay;
	const double speedOfLight = 299792458.0;  // units = meters per second
	const double xilinxLvdsOutputDelay = 1.9e-9;    // 1.9 ns Xilinx LVDS output pin delay
	const double xilinxLvdsInputDelay = 1.4e-9;     // 1.4 ns Xilinx LVDS input pin delay
	const double rhs2000Delay = 9.0e-9;             // 9.0 ns RHS2000 SCLK-to-MISO delay
	const double misoSettleTime = 6.7e-9;           // 6.7 ns delay after MISO changes, before we sample it

    tStep = 1.0 / (2800.0 * getSampleRate());  // data clock that samples MISO has a rate 35 x 80 = 2800x higher than the sampling rate
											   // cableVelocity = 0.67 * speedOfLight;  // propogation velocity on cable: version 1.3 and earlier
	cableVelocity = 0.555 * speedOfLight;  // propogation velocity on cable: version 1.4 improvement based on cable measurements
	distance = 2.0 * lengthInMeters;      // round trip distance data must travel on cable
	timeDelay = (distance / cableVelocity) + xilinxLvdsOutputDelay + rhs2000Delay + xilinxLvdsInputDelay + misoSettleTime;

	delay = (int)floor(((timeDelay / tStep) + 1.0) + 0.5);

	if (delay < 1) delay = 1;   // delay of zero is too short (due to I/O delays), even for zero-length cables

	setCableDelay(port, delay);
}

// Same function as above, but accepts lengths in feet instead of meters
void Rhs2000EvalBoard::setCableLengthFeet(BoardPort port, double lengthInFeet)
{
	setCableLengthMeters(port, 0.3048 * lengthInFeet);   // convert feet to meters
}

// Estimate cable length based on a particular delay used in setCableDelay.
// (Note: Depends on sample rate.)
double Rhs2000EvalBoard::estimateCableLengthMeters(int delay) const
{
	double tStep, cableVelocity, distance;
	const double speedOfLight = 299792458.0;  // units = meters per second
	const double xilinxLvdsOutputDelay = 1.9e-9;    // 1.9 ns Xilinx LVDS output pin delay
	const double xilinxLvdsInputDelay = 1.4e-9;     // 1.4 ns Xilinx LVDS input pin delay
	const double rhs2000Delay = 9.0e-9;             // 9.0 ns RHS2000 SCLK-to-MISO delay
	const double misoSettleTime = 6.7e-9;           // 6.7 ns delay after MISO changes, before we sample it

	tStep = 1.0 / (2800.0 * getSampleRate());  // data clock that samples MISO has a rate 35 x 80 = 2800x higher than the sampling rate
											   // cableVelocity = 0.67 * speedOfLight;  // propogation velocity on cable: version 1.3 and earlier
	cableVelocity = 0.555 * speedOfLight;  // propogation velocity on cable: version 1.4 improvement based on cable measurements

                                           // distance = cableVelocity * (delay * tStep - (xilinxLvdsOutputDelay + rhs2000Delay + xilinxLvdsInputDelay));  // version 1.3 and earlier
	distance = cableVelocity * ((((double)delay) - 1.0) * tStep - (xilinxLvdsOutputDelay + rhs2000Delay + xilinxLvdsInputDelay + misoSettleTime));  // version 1.4 improvement
	if (distance < 0.0) distance = 0.0;

	return (distance / 2.0);
}

// Same function as above, but returns length in feet instead of meters
double Rhs2000EvalBoard::estimateCableLengthFeet(int delay) const
{
	return 3.2808 * estimateCableLengthMeters(delay);
}

// Turn on or off DSP settle function in the FPGA.  (Only executes when CONVERT commands are sent.)
void Rhs2000EvalBoard::setDspSettle(bool enabled)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInResetRun, (enabled ? 0x04 : 0x00), 0x04);
	dev->UpdateWireIns();
}

// Enable or disable one of the eight available USB data streams (0-7).
void Rhs2000EvalBoard::enableDataStream(int stream, bool enabled)
{
    lock_guard<mutex> lockOk(okMutex);

    if (stream < 0 || stream >(MAX_NUM_DATA_STREAMS - 1)) {
        cerr << "Error in Rhs2000EvalBoard::enableDataStream: stream out of range." << endl;
		return;
	}

	if (enabled) {
		if (dataStreamEnabled[stream] == 0) {
			dev->SetWireInValue(WireInDataStreamEn, 0x0001 << stream, 0x0001 << stream);
			dev->UpdateWireIns();
			dataStreamEnabled[stream] = 1;
			numDataStreams++;
		}
	}
	else {
		if (dataStreamEnabled[stream] == 1) {
			dev->SetWireInValue(WireInDataStreamEn, 0x0000 << stream, 0x0001 << stream);
			dev->UpdateWireIns();
			dataStreamEnabled[stream] = 0;
			numDataStreams--;
		}
	}
}

// Returns the number of enabled data streams.
int Rhs2000EvalBoard::getNumEnabledDataStreams() const
{
	return numDataStreams;
}

// Read the 16 bits of the digital TTL input lines on the FPGA into an integer array.
void Rhs2000EvalBoard::getTtlIn(int ttlInArray[])
{
    lock_guard<mutex> lockOk(okMutex);
    int i, ttlIn;

	dev->UpdateWireOuts();
	ttlIn = dev->GetWireOutValue(WireOutTtlIn);

	for (i = 0; i < 16; ++i) {
		ttlInArray[i] = 0;
		if ((ttlIn & (1 << i)) > 0)
			ttlInArray[i] = 1;
	}
}

// Set manual value for DACs.
void Rhs2000EvalBoard::setDacManual(int value)
{
    lock_guard<mutex> lockOk(okMutex);
    if (value < 0 || value > 65535) {
		cerr << "Error in Rhs2000EvalBoard::setDacManual: value out of range." << endl;
		return;
	}

	dev->SetWireInValue(WireInDacManual, value);
	dev->UpdateWireIns();
}

// Set the eight red LEDs on the XEM6010 board according to integer array.
void Rhs2000EvalBoard::setLedDisplay(int ledArray[])
{
    lock_guard<mutex> lockOk(okMutex);
    int i, ledOut;

	ledOut = 0;
	for (i = 0; i < 8; ++i) {
		if (ledArray[i] > 0)
			ledOut += 1 << i;
	}
    dev->SetWireInValue(WireInLedDisplay, ledOut, 0x00ff);
	dev->UpdateWireIns();
}

// Set the eight red LEDs on the front panel SPI ports according to integer array.
void Rhs2000EvalBoard::setSpiLedDisplay(int ledArray[])
{
    lock_guard<mutex> lockOk(okMutex);
    int i, ledOut;

    ledOut = 0;
    for (i = 0; i < 8; ++i) {
        if (ledArray[i] > 0)
            ledOut += 1 << i;
    }
    dev->SetWireInValue(WireInLedDisplay, (ledOut << 8), 0xff00);
    dev->UpdateWireIns();
}

// Enable or disable AD5662 DAC channel (0-7)
void Rhs2000EvalBoard::enableDac(int dacChannel, bool enabled)
{
    lock_guard<mutex> lockOk(okMutex);
    if (dacChannel < 0 || dacChannel > 7) {
		cerr << "Error in Rhs2000EvalBoard::enableDac: dacChannel out of range." << endl;
		return;
	}

	switch (dacChannel) {
	case 0:
		dev->SetWireInValue(WireInDacSource1, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 1:
		dev->SetWireInValue(WireInDacSource2, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 2:
		dev->SetWireInValue(WireInDacSource3, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 3:
		dev->SetWireInValue(WireInDacSource4, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 4:
		dev->SetWireInValue(WireInDacSource5, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 5:
		dev->SetWireInValue(WireInDacSource6, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 6:
		dev->SetWireInValue(WireInDacSource7, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	case 7:
		dev->SetWireInValue(WireInDacSource8, (enabled ? 0x0200 : 0x0000), 0x0200);
		break;
	}
	dev->UpdateWireIns();
}

// Set the gain level of all eight DAC channels to 2^gain (gain = 0-7).
void Rhs2000EvalBoard::setDacGain(int gain)
{
    lock_guard<mutex> lockOk(okMutex);
    if (gain < 0 || gain > 7) {
		cerr << "Error in Rhs2000EvalBoard::setDacGain: gain out of range." << endl;
		return;
	}

	dev->SetWireInValue(WireInResetRun, gain << 13, 0xe000);
	dev->UpdateWireIns();
}

// Suppress the noise on DAC channels 0 and 1 (the audio channels) between
// +16*noiseSuppress and -16*noiseSuppress LSBs.  (noiseSuppress = 0-127).
void Rhs2000EvalBoard::setAudioNoiseSuppress(int noiseSuppress)
{
    lock_guard<mutex> lockOk(okMutex);

    if (noiseSuppress < 0 || noiseSuppress > 127) {
		cerr << "Error in Rhs2000EvalBoard::setAudioNoiseSuppress: noiseSuppress out of range." << endl;
		return;
	}

	dev->SetWireInValue(WireInResetRun, noiseSuppress << 6, 0x1fc0);
	dev->UpdateWireIns();
}

// Assign a particular data stream (0-7) to a DAC channel (0-7).  Setting stream
// to 8 selects DacManual value.
void Rhs2000EvalBoard::selectDacDataStream(int dacChannel, int stream)
{
    lock_guard<mutex> lockOk(okMutex);

    if (dacChannel < 0 || dacChannel > 7) {
		cerr << "Error in Rhs2000EvalBoard::selectDacDataStream: dacChannel out of range." << endl;
		return;
	}

    if (stream < 0 || stream > 8) {
		cerr << "Error in Rhs2000EvalBoard::selectDacDataStream: stream out of range." << endl;
		return;
	}

	switch (dacChannel) {
	case 0:
		dev->SetWireInValue(WireInDacSource1, stream << 5, 0x01e0);
		break;
	case 1:
		dev->SetWireInValue(WireInDacSource2, stream << 5, 0x01e0);
		break;
	case 2:
		dev->SetWireInValue(WireInDacSource3, stream << 5, 0x01e0);
		break;
	case 3:
		dev->SetWireInValue(WireInDacSource4, stream << 5, 0x01e0);
		break;
	case 4:
		dev->SetWireInValue(WireInDacSource5, stream << 5, 0x01e0);
		break;
	case 5:
		dev->SetWireInValue(WireInDacSource6, stream << 5, 0x01e0);
		break;
	case 6:
		dev->SetWireInValue(WireInDacSource7, stream << 5, 0x01e0);
		break;
	case 7:
		dev->SetWireInValue(WireInDacSource8, stream << 5, 0x01e0);
		break;
	}
	dev->UpdateWireIns();
}

// Assign a particular amplifier channel (0-31) to a DAC channel (0-7).
void Rhs2000EvalBoard::selectDacDataChannel(int dacChannel, int dataChannel)
{
    lock_guard<mutex> lockOk(okMutex);

    if (dacChannel < 0 || dacChannel > 7) {
		cerr << "Error in Rhs2000EvalBoard::selectDacDataChannel: dacChannel out of range." << endl;
		return;
	}

	if (dataChannel < 0 || dataChannel > 31) {
		cerr << "Error in Rhs2000EvalBoard::selectDacDataChannel: dataChannel out of range." << endl;
		return;
	}

	switch (dacChannel) {
	case 0:
		dev->SetWireInValue(WireInDacSource1, dataChannel << 0, 0x001f);
		break;
	case 1:
		dev->SetWireInValue(WireInDacSource2, dataChannel << 0, 0x001f);
		break;
	case 2:
		dev->SetWireInValue(WireInDacSource3, dataChannel << 0, 0x001f);
		break;
	case 3:
		dev->SetWireInValue(WireInDacSource4, dataChannel << 0, 0x001f);
		break;
	case 4:
		dev->SetWireInValue(WireInDacSource5, dataChannel << 0, 0x001f);
		break;
	case 5:
		dev->SetWireInValue(WireInDacSource6, dataChannel << 0, 0x001f);
		break;
	case 6:
		dev->SetWireInValue(WireInDacSource7, dataChannel << 0, 0x001f);
		break;
	case 7:
		dev->SetWireInValue(WireInDacSource8, dataChannel << 0, 0x001f);
		break;
	}
	dev->UpdateWireIns();
}

void Rhs2000EvalBoard::enableDcAmpConvert(bool enable)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInDcAmpConvert, (enable ? 1 : 0));
	dev->UpdateWireIns();
}

void Rhs2000EvalBoard::setExtraStates(unsigned int extraStates)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInExtraStates, extraStates);
	dev->UpdateWireIns();
}

// Enable optional FPGA-implemented digital high-pass filters associated with DAC outputs
// on USB interface board.. These one-pole filters can be used to record wideband neural data
// while viewing only spikes without LFPs on the DAC outputs, for example.  This is useful when
// using the low-latency FPGA thresholds to detect spikes and produce digital pulses on the TTL
// outputs, for example.
void Rhs2000EvalBoard::enableDacHighpassFilter(bool enable)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInMultiUse, enable ? 1 : 0);
	dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInDacHpf, 0);
}

// Set cutoff frequency (in Hz) for optional FPGA-implemented digital high-pass filters
// associated with DAC outputs on USB interface board.  These one-pole filters can be used
// to record wideband neural data while viewing only spikes without LFPs on the DAC outputs,
// for example.  This is useful when using the low-latency FPGA thresholds to detect spikes
// and produce digital pulses on the TTL outputs, for example.
void Rhs2000EvalBoard::setDacHighpassFilter(double cutoff)
{
    lock_guard<mutex> lockOk(okMutex);

    double b;
	int filterCoefficient;
	const double pi = 3.1415926535897;

	// Note that the filter coefficient is a function of the amplifier sample rate, so this
	// function should be called after the sample rate is changed.
	b = 1.0 - exp(-2.0 * pi * cutoff / getSampleRate());

	// In hardware, the filter coefficient is represented as a 16-bit number.
	filterCoefficient = (int)floor(65536.0 * b + 0.5);

	if (filterCoefficient < 1) {
		filterCoefficient = 1;
	}
	else if (filterCoefficient > 65535) {
		filterCoefficient = 65535;
	}

	dev->SetWireInValue(WireInMultiUse, filterCoefficient);
	dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInDacHpf, 1);
}

// Set thresholds for DAC channels; threshold output signals appear on TTL outputs 0-7.
// The parameter 'threshold' corresponds to the RHS2116 chip ADC output value, and must fall
// in the range of 0 to 65535, where the 'zero' level is 32768.
// If trigPolarity is true, voltages equaling or rising above the threshold produce a high TTL output.
// If trigPolarity is false, voltages equaling or falling below the threshold produce a high TTL output.
void Rhs2000EvalBoard::setDacThreshold(int dacChannel, int threshold, bool trigPolarity)
{
    lock_guard<mutex> lockOk(okMutex);

    if (dacChannel < 0 || dacChannel > 7) {
		cerr << "Error in Rhs2000EvalBoard::setDacThreshold: dacChannel out of range." << endl;
		return;
	}

	if (threshold < 0 || threshold > 65535) {
		cerr << "Error in Rhs2000EvalBoard::setDacThreshold: threshold out of range." << endl;
		return;
	}

	// Set threshold level.
	dev->SetWireInValue(WireInMultiUse, threshold);
    dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInDacThresh, dacChannel);

	// Set threshold polarity.
	dev->SetWireInValue(WireInMultiUse, (trigPolarity ? 1 : 0));
	dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInDacThresh, dacChannel + 8);
}

// Is variable-frequency clock DCM programming done?
bool Rhs2000EvalBoard::isDcmProgDone() const
{
	int value;

	dev->UpdateWireOuts();
	value = dev->GetWireOutValue(WireOutDataClkLocked);

	return ((value & 0x0002) > 1);
}

// Is variable-frequency clock PLL locked?
bool Rhs2000EvalBoard::isDataClockLocked() const
{
	int value;

	dev->UpdateWireOuts();
	value = dev->GetWireOutValue(WireOutDataClkLocked);

	return ((value & 0x0001) > 0);
}

// Flush all remaining data out of the FIFO.  (This function should only be called when SPI
// data acquisition has been stopped.)
void Rhs2000EvalBoard::flush()
{
    lock_guard<mutex> lockOk(okMutex);

    while (numWordsInFifo() >= USB_BUFFER_SIZE / 2) {
		dev->ReadFromPipeOut(PipeOutData, USB_BUFFER_SIZE, usbBuffer);
	}
	while (numWordsInFifo() > 0) {
		dev->ReadFromPipeOut(PipeOutData, 2 * numWordsInFifo(), usbBuffer);
	}
}

// Read data block from the USB interface, if one is available.  Returns true if data block
// was available.
bool Rhs2000EvalBoard::readDataBlock(Rhs2000DataBlock *dataBlock)
{
    lock_guard<mutex> lockOk(okMutex);

    unsigned int numBytesToRead;

	numBytesToRead = 2 * dataBlock->calculateDataBlockSizeInWords(numDataStreams);

	if (numBytesToRead > USB_BUFFER_SIZE) {
		cerr << "Error in Rhs2000EvalBoard::readDataBlock: USB buffer size exceeded.  " <<
			"Increase value of USB_BUFFER_SIZE." << endl;
		return false;
	}

	dev->ReadFromPipeOut(PipeOutData, numBytesToRead, usbBuffer);

	dataBlock->fillFromUsbBuffer(usbBuffer, 0, numDataStreams);

	return true;
}

// Reads a certain number of USB data blocks, if the specified number is available, and writes the raw bytes
// to a buffer.  Returns total number of bytes read.
long Rhs2000EvalBoard::readDataBlocksRaw(int numBlocks, unsigned char* buffer)
{
    lock_guard<mutex> lockOk(okMutex);

    unsigned int numWordsToRead = numBlocks * Rhs2000DataBlock::calculateDataBlockSizeInWords(numDataStreams);

    if (numWordsInFifo() < numWordsToRead)
        return 0;

    long result = dev->ReadFromPipeOut(PipeOutData, 2 * numWordsToRead, buffer);

    if (result == ok_Failed) {
        cerr << "CRITICAL (readDataBlockRaw): Failure on pipe read.  Check buffer size." << endl;
    } else if (result == ok_Timeout) {
        cerr << "CRITICAL (readDataBlockRaw): Timeout on pipe read.  Check buffer size." << endl;
    }

    return result;
}

// Reads a certain number of USB data blocks, if the specified number is available, and appends them
// to queue.  Returns true if data blocks were available.
bool Rhs2000EvalBoard::readDataBlocks(int numBlocks, queue<Rhs2000DataBlock> &dataQueue)
{
    lock_guard<mutex> lockOk(okMutex);

    unsigned int numWordsToRead, numBytesToRead;
	int i;
	Rhs2000DataBlock *dataBlock;

	numWordsToRead = numBlocks * dataBlock->calculateDataBlockSizeInWords(numDataStreams);

	if (numWordsInFifo() < numWordsToRead)
		return false;

	numBytesToRead = 2 * numWordsToRead;

	if (numBytesToRead > USB_BUFFER_SIZE) {
		cerr << "Error in Rhs2000EvalBoard::readDataBlocks: USB buffer size exceeded.  " <<
			"Increase value of USB_BUFFER_SIZE." << endl;
		return false;
	}

	dev->ReadFromPipeOut(PipeOutData, numBytesToRead, usbBuffer);

	dataBlock = new Rhs2000DataBlock(numDataStreams);
	for (i = 0; i < numBlocks; ++i) {
		dataBlock->fillFromUsbBuffer(usbBuffer, i, numDataStreams);
		dataQueue.push(*dataBlock);
	}
	delete dataBlock;

	return true;
}

// Writes the contents of a data block queue (dataQueue) to a binary output stream (saveOut).
// Returns the number of data blocks written.
int Rhs2000EvalBoard::queueToFile(queue<Rhs2000DataBlock> &dataQueue, ofstream &saveOut)
{
	int count = 0;

	while (!dataQueue.empty()) {
		dataQueue.front().write(saveOut, getNumEnabledDataStreams());
		dataQueue.pop();
		++count;
	}

	return count;
}

// Return name of Opal Kelly board based on model code.
string Rhs2000EvalBoard::opalKellyModelName(int model) const
{
	switch (model) {
	case OK_PRODUCT_XEM3001V1:
		return("XEM3001V1");
	case OK_PRODUCT_XEM3001V2:
		return("XEM3001V2");
	case OK_PRODUCT_XEM3010:
		return("XEM3010");
	case OK_PRODUCT_XEM3005:
		return("XEM3005");
	case OK_PRODUCT_XEM3001CL:
		return("XEM3001CL");
	case OK_PRODUCT_XEM3020:
		return("XEM3020");
	case OK_PRODUCT_XEM3050:
		return("XEM3050");
	case OK_PRODUCT_XEM9002:
		return("XEM9002");
	case OK_PRODUCT_XEM3001RB:
		return("XEM3001RB");
	case OK_PRODUCT_XEM5010:
		return("XEM5010");
	case OK_PRODUCT_XEM6110LX45:
		return("XEM6110LX45");
	case OK_PRODUCT_XEM6001:
		return("XEM6001");
	case OK_PRODUCT_XEM6010LX45:
		return("XEM6010LX45");
	case OK_PRODUCT_XEM6010LX150:
		return("XEM6010LX150");
	case OK_PRODUCT_XEM6110LX150:
		return("XEM6110LX150");
	case OK_PRODUCT_XEM6006LX9:
		return("XEM6006LX9");
	case OK_PRODUCT_XEM6006LX16:
		return("XEM6006LX16");
	case OK_PRODUCT_XEM6006LX25:
		return("XEM6006LX25");
	case OK_PRODUCT_XEM5010LX110:
		return("XEM5010LX110");
	case OK_PRODUCT_ZEM4310:
		return("ZEM4310");
	case OK_PRODUCT_XEM6310LX45:
		return("XEM6310LX45");
	case OK_PRODUCT_XEM6310LX150:
		return("XEM6310LX150");
	case OK_PRODUCT_XEM6110V2LX45:
		return("XEM6110V2LX45");
	case OK_PRODUCT_XEM6110V2LX150:
		return("XEM6110V2LX150");
	case OK_PRODUCT_XEM6002LX9:
		return("XEM6002LX9");
	case OK_PRODUCT_XEM6310MTLX45:
		return("XEM6310MTLX45");
	case OK_PRODUCT_XEM6320LX130T:
		return("XEM6320LX130T");
	default:
		return("UNKNOWN");
	}
}

// Return 4-bit "board mode" input.
int Rhs2000EvalBoard::getBoardMode()
{
    lock_guard<mutex> lockOk(okMutex);
    int mode;

	dev->UpdateWireOuts();
	mode = dev->GetWireOutValue(WireOutBoardMode);

	cout << "Board mode: " << mode << endl << endl;

	return mode;
}

// Return FPGA cable delay for selected SPI port.
int Rhs2000EvalBoard::getCableDelay(BoardPort port) const
{
	switch (port) {
	case PortA:
		return cableDelay[0];
	case PortB:
		return cableDelay[1];
	case PortC:
		return cableDelay[2];
	case PortD:
		return cableDelay[3];
	default:
		cerr << "Error in RHS2000EvalBoard::getCableDelay: unknown port." << endl;
		return -1;
	}
}

// Return FPGA cable delays for all SPI ports.
void Rhs2000EvalBoard::getCableDelay(vector<int> &delays) const
{
	if (delays.size() != 4) {
		delays.resize(4);
	}
	for (int i = 0; i < 4; ++i) {
		delays[i] = cableDelay[i];
	}
}

// Set all DACs to midline value (zero).  Must run SPI commands for this to take effect.
void Rhs2000EvalBoard::setAllDacsToZero()
{
    int i;

    setDacManual(32768);    // midrange value = 0 V
    for (i = 0; i < 8; i++) {
        selectDacDataStream(i, 8);
    }
}

// Run 128 dummy commands (i.e., reading from ROM registers)
void Rhs2000EvalBoard::runDummyCommands(Rhs2000Registers *chipRegisters)
{
    // Create command lists to be uploaded.
    int commandSequenceLength;
    vector<unsigned int> commandList;

    // Create a command list.
    commandSequenceLength = chipRegisters->createCommandListDummy(commandList, 128);
    uploadCommandList(commandList, AuxCmd1);
    selectAuxCommandLength(AuxCmd1, 0, commandSequenceLength - 1);

    setMaxTimeStep(commandSequenceLength);
    setContinuousRunMode(false);

    // Start SPI interface.
    run();

    // Wait for the 128-sample run to complete.
    while (isRunning()) { }

    flush();
}

// Create a command list to program chip registers, and execute it.
void Rhs2000EvalBoard::configureChip(Rhs2000Registers *chipRegisters)
{
    // Create command lists to be uploaded.
    int commandSequenceLength;
    vector<unsigned int> commandList;

    // Create a command list.
    commandSequenceLength = chipRegisters->createCommandListRegisterConfig(commandList, true);
    uploadCommandList(commandList, AuxCmd1);
    selectAuxCommandLength(AuxCmd1, 0, commandSequenceLength - 1);
    printCommandList(commandList); // optionally, print command list

    setMaxTimeStep(commandSequenceLength);
    setContinuousRunMode(false);

    // Start SPI interface.
    run();

    // Wait for the 128-sample run to complete.
    while (isRunning()) { }

    // Read the resulting single data block from the USB interface.
    // Rhs2000DataBlock *dataBlock = new Rhs2000DataBlock();
    // readDataBlock(dataBlock);

    // cout << "Number of 16-bit words in FIFO: " << numWordsInFifo() << endl;

    flush();
}

// Create a command list to program single chip register, and execute it.
void Rhs2000EvalBoard::configureRegister(Rhs2000Registers *chipRegisters, int reg)
{
    // Create command lists to be uploaded.
    int commandSequenceLength;
    vector<unsigned int> commandList;

    // Create a command list.
    commandSequenceLength = chipRegisters->createCommandListSingleRegisterConfig(commandList, reg);
    uploadCommandList(commandList, AuxCmd1);
    selectAuxCommandLength(AuxCmd1, 0, commandSequenceLength - 1);

    setMaxTimeStep(commandSequenceLength);
    setContinuousRunMode(false);

    // Start SPI interface.
    run();

    // Wait for the 128-sample run to complete.
    while (isRunning()) { }

    flush();
}

// Upload an auxiliary command list to a particular auxiliary command slot (AuxCmd1, AuxCmd2,
// AuxCmd3, or AuxCmd4) in the FPGA.
void Rhs2000EvalBoard::uploadCommandList(const vector<unsigned int> &commandList, AuxCmdSlot auxCommandSlot)
{
    lock_guard<mutex> lockOk(okMutex);
	unsigned int i;

	for (i = 0; i < commandList.size(); ++i) {
		commandBufferMsw[2 * i] = (unsigned char)((commandList[i] & 0x00ff0000) >> 16);
		commandBufferMsw[2 * i + 1] = (unsigned char)((commandList[i] & 0xff000000) >> 24);
		commandBufferLsw[2 * i] = (unsigned char)((commandList[i] & 0x000000ff) >> 0);
		commandBufferLsw[2 * i + 1] = (unsigned char)((commandList[i] & 0x0000ff00) >> 8);
	}

	switch (auxCommandSlot) {
	case AuxCmd1:
        dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd1Msw, 2 * commandList.size(), commandBufferMsw);
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd1Lsw, 2 * commandList.size(), commandBufferLsw);
		break;
	case AuxCmd2:
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd2Msw, 2 * commandList.size(), commandBufferMsw);
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd2Lsw, 2 * commandList.size(), commandBufferLsw);
		break;
	case AuxCmd3:
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd3Msw, 2 * commandList.size(), commandBufferMsw);
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd3Lsw, 2 * commandList.size(), commandBufferLsw);
		break;
	case AuxCmd4:
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd4Msw, 2 * commandList.size(), commandBufferMsw);
		dev->ActivateTriggerIn(TrigInRamAddrReset, 0);
		dev->WriteToPipeIn(PipeInAuxCmd4Lsw, 2 * commandList.size(), commandBufferLsw);
		break;
	}
	
}

// Selects an amplifier channel from a particular data stream to be subtracted from all DAC signals.
void Rhs2000EvalBoard::setDacRerefSource(int stream, int channel)
{
    lock_guard<mutex> lockOk(okMutex);

    if (stream < 0 || stream > (MAX_NUM_DATA_STREAMS - 1)) {
        cerr << "Error in Rhs2000EvalBoard::setDacRerefSource: stream out of range." << endl;
        return;
    }

    if (channel < 0 || channel > 15) {
        cerr << "Error in Rhs2000EvalBoard::setDacRerefSource: channel out of range." << endl;
        return;
    }

    dev->SetWireInValue(WireInDacReref, (stream << 5) + channel, 0x0000000ff);
    dev->UpdateWireIns();
}

// Enables DAC rereferencing, where a selected amplifier channel is subtracted from all DACs in real time.
void Rhs2000EvalBoard::enableDacReref(bool enabled)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInDacReref, (enabled ? 0x00000100 : 0x00000000), 0x00000100);
    dev->UpdateWireIns();
}

// Turn on or off automatic stimulation command mode in the FPGA.
void Rhs2000EvalBoard::setStimCmdMode(bool enabled)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInStimCmdMode, (enabled ? 0x01 : 0x00), 0x01);
    dev->UpdateWireIns();
}

// Set a particular stimulation control register.
void Rhs2000EvalBoard::programStimReg(int stream, int channel, StimRegister reg, int value)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInStimRegAddr, (stream << 8) + (channel << 4) + reg);
    dev->SetWireInValue(WireInStimRegWord, value);
    dev->UpdateWireIns();
    dev->ActivateTriggerIn(TrigInRamAddrReset, 1);
}

// Configure a particular stimulation trigger.
void Rhs2000EvalBoard::configureStimTrigger(int stream, int channel, int triggerSource, bool triggerEnabled, bool edgeTriggered, bool triggerOnLow)
{
    int value = (triggerEnabled ? (1 << 7) : 0) + (triggerOnLow ? (1 << 6) : 0) + (edgeTriggered ? (1 << 5) : 0) + triggerSource;
    programStimReg(stream, channel, TriggerParams, value);
}

// Configure the shape, polarity, and number of pulses for a particular stimulation control unit.
void Rhs2000EvalBoard::configureStimPulses(int stream, int channel, int numPulses, StimShape shape, bool negStimFirst)
{
    if (numPulses < 1) {
        cerr << "Error in Rhs2000EvalBoard::configureStimPulses: numPulses out of range." << endl;
        return;
    }

    int shapeInt = (int)shape;
    int value = (negStimFirst ? (1 << 10) : 0) + (shapeInt << 8) + (numPulses - 1);
    programStimReg(stream, channel, StimParams, value);
}

// Returns number of SPI ports (4 or 8) and if I/O expander board is present
int Rhs2000EvalBoard::readDigitalInManual(bool& expanderBoardDetected)
{
    lock_guard<mutex> lockOk(okMutex);
    int expanderBoardIdNumber;
    bool spiPortPresent[8];
    bool userId[3];
    bool serialId[4];
    bool digOutVoltageLevel;

    dev->UpdateWireOuts();
    expanderBoardDetected = (dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x04) != 0;
    expanderBoardIdNumber = ((dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x08) ? 1 : 0);

    dev->SetWireInValue(WireInSerialDigitalInCntl, 2);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);  // Load digital in shift registers on falling edge of serial_LOAD
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[7] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[6] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[5] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[4] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[3] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[2] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[1] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    spiPortPresent[0] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    digOutVoltageLevel = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    userId[2] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    userId[1] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    userId[0] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    serialId[3] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    serialId[2] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    serialId[1] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    serialId[0] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x01;

    int numPorts = 4;
    for (int i = 4; i < 8; i++) {
        if (spiPortPresent[i]) {
            numPorts = 8;
        }
    }

    // cout << "expanderBoardDetected: " << expanderBoardDetected << endl;
    // cout << "expanderBoardId: " << expanderBoardIdNumber << endl;
    // cout << "spiPortPresent: " << spiPortPresent[7] << spiPortPresent[6] << spiPortPresent[5] << spiPortPresent[4] << spiPortPresent[3] << spiPortPresent[2] << spiPortPresent[1] << spiPortPresent[0] << endl;
    // cout << "serialId: " << serialId[3] << serialId[2] << serialId[1] << serialId[0] << endl;
    // cout << "userId: " << userId[2] << userId[1] << userId[0] << endl;
    // cout << "digOutVoltageLevel: " << digOutVoltageLevel << endl;

    return numPorts;
}

void Rhs2000EvalBoard::readDigitalInExpManual()
{
    lock_guard<mutex> lockOk(okMutex);
    int ttlIn[16];

    dev->SetWireInValue(WireInSerialDigitalInCntl, 2);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);  // Load digital in shift registers on falling edge of serial_LOAD
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[15] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[14] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[13] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[12] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[11] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[10] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[9] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[8] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[7] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[6] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[5] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[4] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[3] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[2] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[1] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    dev->SetWireInValue(WireInSerialDigitalInCntl, 1);
    dev->UpdateWireIns();
    dev->SetWireInValue(WireInSerialDigitalInCntl, 0);
    dev->UpdateWireIns();

    dev->UpdateWireOuts();
    ttlIn[0] = dev->GetWireOutValue(WireOutSerialDigitalIn) & 0x02;

    // for (int i = 0; i < 16; i++) {
    //     cout << "TTL IN " << i + 1 << " = " << ttlIn[i]/2 << endl;
    // }
}

// Set the voltage threshold to be used for digital triggers on Analog In ports.
void Rhs2000EvalBoard::setAnalogInTriggerThreshold(double voltageThreshold)
{
    lock_guard<mutex> lockOk(okMutex);

    int value = (int) (32768 * (voltageThreshold / 10.24) + 32768);
    if (value < 0) {
        value = 0;
    } else if (value > 65535) {
        value = 65535;
    }

    dev->SetWireInValue(WireInAdcThreshold, value);
    dev->UpdateWireIns();
}

// Set state of manual stimulation trigger 0-7 (e.g., from keypresses).
void Rhs2000EvalBoard::setManualStimTrigger(int trigger, bool triggerOn)
{
    lock_guard<mutex> lockOk(okMutex);

    if (trigger < 0 || trigger > 7) {
        cerr << "Error in Rhs2000EvalBoard::setManualStimTrigger: trigger out of range." << endl;
        return;
    }

    dev->SetWireInValue(WireInManualTriggers, (triggerOn ? 1 : 0) << trigger, 1 << trigger);
    dev->UpdateWireIns();
}

// Enable auxiliary commands slots 0-3 on all data streams (0-7).  This disables automatic stimulation
// control on all data streams.
void Rhs2000EvalBoard::enableAuxCommandsOnAllStreams()
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInAuxEnable, 0x00ff, 0x00ff);
    dev->UpdateWireIns();
}

// Enable auxiliary commands slots 0-3 on one selected data stream, and disable auxiliary command slots on
// all other data streams.  This disables automatic stimulation control on the selected stream and enables
// automatic stimulation control on all other streams.
void Rhs2000EvalBoard::enableAuxCommandsOnOneStream(int stream)
{
    lock_guard<mutex> lockOk(okMutex);

    if (stream < 0 || stream >(MAX_NUM_DATA_STREAMS - 1)) {
        cerr << "Error in Rhs2000EvalBoard::enableAuxCommandsOnOneStream: stream out of range." << endl;
        return;
    }

    dev->SetWireInValue(WireInAuxEnable, 0x0001 << stream, 0x00ff);
    dev->UpdateWireIns();
}

// The first four boolean parameters determine if global settling should be applied to particular SPI ports A-D.
// If global settling is enabled, the amp settle function will be applied to ALL channels on a headstage when any one
// channel asserts amp settle.
// If the last boolean parameter is set true, global settling will be applied across all headstages: if any one channel
// asserts amp settle, then amp settle will be asserted on all channels, across all connected headstages.
void Rhs2000EvalBoard::setGlobalSettlePolicy(bool settleWholeHeadstageA, bool settleWholeHeadstageB, bool settleWholeHeadstageC,
                                             bool settleWholeHeadstageD, bool settleAllHeadstages)
{
    lock_guard<mutex> lockOk(okMutex);

    int value;

    value = (settleAllHeadstages ? 16 : 0) + (settleWholeHeadstageA ? 1 : 0) + (settleWholeHeadstageB ? 2 : 0) +
            (settleWholeHeadstageC ? 4 : 0) + (settleWholeHeadstageD ? 8 : 0);

    dev->SetWireInValue(WireInGlobalSettleSelect, value, 0x001f);
    dev->UpdateWireIns();
}

// Sets the function of Digital Out ports 1-8.
// true = Digital Out port controlled by DAC threshold-based spike detector
// false = Digital Out port controlled by digital sequencer
// Note: Digital Out ports 9-16 are always controlled by a digital sequencer.
void Rhs2000EvalBoard::setTtlOutMode(bool mode1, bool mode2, bool mode3, bool mode4, bool mode5, bool mode6, bool mode7, bool mode8)
{
    lock_guard<mutex> lockOk(okMutex);

    int value = 0;
    value += mode1 ? 1 : 0;
    value += mode2 ? 2 : 0;
    value += mode3 ? 4 : 0;
    value += mode4 ? 8 : 0;
    value += mode5 ? 16 : 0;
    value += mode6 ? 32 : 0;
    value += mode7 ? 64 : 0;
    value += mode8 ? 128 : 0;

    dev->SetWireInValue(WireInTtlOutMode, value, 0x000000ff);
    dev->UpdateWireIns();
}

// Select amp settle mode for all connected chips:
// useFastSettle false = amplifier low frequency cutoff select (recommended mode)
// useFastSettle true = amplifier fast settle (legacy mode from RHD2000 series chips)
void Rhs2000EvalBoard::setAmpSettleMode(bool useFastSettle)
{
    lock_guard<mutex> lockOk(okMutex);

    dev->SetWireInValue(WireInResetRun, (useFastSettle ? 0x08 : 0x00), 0x08); // set amp_settle_mode (0 = amplifier low frequency cutoff select; 1 = amplifier fast settle)
    dev->UpdateWireIns();
}

// Select charge recovery mode for all connected chips:
// useSwitch false = current-limited charge recovery drivers
// useSwitch true = charge recovery switch
void Rhs2000EvalBoard::setChargeRecoveryMode(bool useSwitch)
{
    dev->SetWireInValue(WireInResetRun, (useSwitch ? 0x10 : 0x00), 0x10); // set charge_recov_mode (0 = current-limited charge recovery drivers; 1 = charge recovery switch)
    dev->UpdateWireIns();
}

// Reset stimulation sequencer units.  This is typically called when data acquisition is stopped.
// It is possible that a stimulation sequencer could be in the middle of playing out a long
// pulse train (e.g., 100 stimulation pulses).  If this function is not called, the pulse train
// will resume after data acquisition is restarted.
void Rhs2000EvalBoard::resetSequencers()
{
    lock_guard<mutex> lockOk(okMutex);

    dev->ActivateTriggerIn(TrigInSpiStart, 1);
}
