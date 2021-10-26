//----------------------------------------------------------------------------------
// rhs2000registers.cpp
//
// Intan Technoloies RHS2000 Interface API
// Rhs2000Registers Class
// Version 1.011 (18 January 2018)
//
// Copyright (c) 2013-2018 Intan Technologies LLC
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
#include <math.h>
#include <vector>
#include <queue>
#include <limits>

#include "rhs2000registers.h"

using namespace std;

// This class creates and manages a data structure representing the internal RAM registers on
// an RHS2116 chip, and generates command lists to configure the chip and perform other functions.
// Changing the value of variables within an instance of this class does not directly affect an
// RHS2116 chip connected to the FPGA; rather, a command list must be generated from this class
// and then downloaded to the FPGA board using Rhs2000EvalBoard::uploadCommandList.

// Constructor.  Set RHS2116 register variables to default values.
Rhs2000Registers::Rhs2000Registers(double sampleRate, StimStepSize stimStep_)
{
    stimStep = stimStep_;

	int channel;

	ampPwr.resize(16);
	ampFastSettle.resize(16);
	ampFLSelect.resize(16);
	dcAmpPwr.resize(16);
	complianceMonitor.resize(16);
	stimOn.resize(16);
	stimPol.resize(16);
	chargeRecoverySwitch.resize(16);
	cLChargeRecoveryEn.resize(16);
	negCurrentMag.resize(16);
	negCurrentTrim.resize(16);
	posCurrentMag.resize(16);
	posCurrentTrim.resize(16);

	defineSampleRate(sampleRate);

	// Set default values for all register settings
								// adcBufferBias = 32;      // ADC reference buffer bias current (0 [highest current] - 63 [lowest current]);
								// This value should be set according to ADC sampling rate; set in defineSampleRate()

								// muxBias = 40;            // ADC input MUX bias current (0 [highest current] - 63 [lowest current]);
								// This value should be set according to ADC sampling rate; set in defineSampleRate()

	setDigOutHiZ(DigOut1);      // auxiliary digital output states
	setDigOutHiZ(DigOut2);
	setDigOutHiZ(DigOutOD);

	weakMiso = 1;               // weak MISO (0 = MISO line is HiZ when CS is inactive; 1 = MISO line is weakly
								// driven when CS is inactive)
	twosComp = 0;               // two's complement ADC results (0 = unsigned offset representation; 1 = signed
								// representation)
	absMode = 0;                // absolute value mode (0 = normal output; 1 = output passed through abs(x) function)
	enableDsp(true);            // DSP offset removal enable/disable
    setDspCutoffFreq(1.0);      // DSP offset removal HPF cutoff freqeuncy

	zcheckDacPower = 1;         // impedance testing DAC power-up (0 = power down; 1 = power up)
	zcheckLoad = 0;             // impedance testing dummy load (0 = normal operation; 1 = insert 60 pF to ground)
	setZcheckScale(ZcheckCs100fF);  // impedance testing scale factor (100 fF, 1.0 pF, or 10.0 pF)

	enableZcheck(false);        // impedance testing enable/disable

    setZcheckChannel(0);        // impedance testing amplifier select (0-15)

	setUpperBandwidth(7500.0); // set upper bandwidth of amplifiers
    setLowerBandwidth(1.0, 0);  // set lower bandwidth of amplifiers
    setLowerBandwidth(1000.0, 1); // set lower bandwidth of amplifiers for amplifier settle function

    setStimStepSize(stimStep);	// set stimulation currrent DAC step size
    setChargeRecoveryCurrentLimit(CurrentLimit10nA);		// set charge recovery current limit
    setChargeRecoveryTargetVoltage(0);                      // set charge recovery target voltage

	for (channel = 0; channel < 16; channel++) {
        setPosStimMagnitude(channel, 0, 0);				// set all stimulation magnitudes to zero
        setNegStimMagnitude(channel, 0, 0);
	}

	for (channel = 0; channel < 16; channel++) {
		ampFastSettle[channel] = 0;
		ampFLSelect[channel] = 0;
		stimOn[channel] = 0;
		stimPol[channel] = 0;
		chargeRecoverySwitch[channel] = 0;
		cLChargeRecoveryEn[channel] = 0;
	}

	powerUpAllAmps();           // turn on all amplifiers
	powerUpAllDCAmps();			// turn on all DC amplifiers
	setStimEnable(true);		// enable stimulation
}

// Define RHS2116 per-channel sampling rate so that certain sampling-rate-dependent registers are set correctly
// (This function does not change the sampling rate of the FPGA; for this, use Rhs2000EvalBoard::setSampleRate.)
void Rhs2000Registers::defineSampleRate(double newSampleRate)
{
	sampleRate = newSampleRate;

	if (sampleRate < 6001.0) {
		muxBias = 40;
		adcBufferBias = 32;
	}
	else if (sampleRate < 7001.0) {
		muxBias = 40;
		adcBufferBias = 16;
	}
	else if (sampleRate < 8751.0) {
		muxBias = 40;
		adcBufferBias = 8;
	}
	else if (sampleRate < 11001.0) {
		muxBias = 32;
		adcBufferBias = 8;
	}
	else if (sampleRate < 14001.0) {
		muxBias = 26;
		adcBufferBias = 8;
	}
	else if (sampleRate < 17501.0) {
		muxBias = 18;
		adcBufferBias = 4;
	}
	else if (sampleRate < 22001.0) {
		muxBias = 16;
		adcBufferBias = 3;
	}
	else  {
        muxBias = 5; // 7-->5, v1.011
        adcBufferBias = 3;
	}
}

// Drive auxiliary digital output low
void Rhs2000Registers::setDigOutLow(DigOut pin)
{
	switch (pin) {
	case(DigOut1):
		digOut1 = 0;
		digOut1HiZ = 0;
		break;
	case(DigOut2):
		digOut2 = 0;
		digOut2HiZ = 0;
		break;
	case (DigOutOD) :
		digOutOD = 1;
		break;
	}
}

// Drive auxiliary digital output high
void Rhs2000Registers::setDigOutHigh(DigOut pin)
{
	switch (pin) {
	case(DigOut1) :
		digOut1 = 1;
		digOut1HiZ = 0;
		break;
	case(DigOut2) :
		digOut2 = 1;
		digOut2HiZ = 0;
		break;
	case (DigOutOD) :
		digOutOD = 0;
		break;
	}
}

// Set auxiliary digital output to high-impedance (HiZ) state
void Rhs2000Registers::setDigOutHiZ(DigOut pin)
{
	switch (pin) {
	case(DigOut1) :
		digOut1 = 0;
		digOut1HiZ = 1;
		break;
	case(DigOut2) :
		digOut2 = 0;
		digOut2HiZ = 1;
		break;
	case (DigOutOD) :
		digOutOD = 0;
		break;
	}
}

// Enable or disable DSP offset removal filter
void Rhs2000Registers::enableDsp(bool enabled)
{
	dspEn = (enabled ? 1 : 0);
}

// Set the DSP offset removal filter cutoff frequency as closely to the requested
// newDspCutoffFreq (in Hz) as possible; returns the actual cutoff frequency (in Hz).
double Rhs2000Registers::setDspCutoffFreq(double newDspCutoffFreq)
{
	int n;
	double x, fCutoff[16], logNewDspCutoffFreq, logFCutoff[16], minLogDiff;
	const double Pi = 2 * acos(0.0);

	fCutoff[0] = 0.0;   // We will not be using fCutoff[0], but we initialize it to be safe

	logNewDspCutoffFreq = log10(newDspCutoffFreq);

	// Generate table of all possible DSP cutoff frequencies
	for (n = 1; n < 16; ++n) {
		x = pow(2.0, (double)n);
		fCutoff[n] = sampleRate * log(x / (x - 1.0)) / (2 * Pi);
		logFCutoff[n] = log10(fCutoff[n]);
		// cout << "  fCutoff[" << n << "] = " << fCutoff[n] << " Hz" << endl;
	}

	// Now find the closest value to the requested cutoff frequency (on a logarithmic scale)
	if (newDspCutoffFreq > fCutoff[1]) {
		dspCutoffFreq = 1;
	}
	else if (newDspCutoffFreq < fCutoff[15]) {
		dspCutoffFreq = 15;
	}
	else {
		minLogDiff = 10000000.0;
		for (n = 1; n < 16; ++n) {
            if (fabs(logNewDspCutoffFreq - logFCutoff[n]) < minLogDiff) {
                minLogDiff = fabs(logNewDspCutoffFreq - logFCutoff[n]);
				dspCutoffFreq = n;
			}
		}
	}

	return fCutoff[dspCutoffFreq];
}

// Returns the current value of the DSP offset removal cutoff frequency (in Hz).
double Rhs2000Registers::getDspCutoffFreq() const
{
	double x;
	const double Pi = 2 * acos(0.0);

	x = pow(2.0, (double)dspCutoffFreq);

	return sampleRate * log(x / (x - 1.0)) / (2 * Pi);
}

// Enable or disable impedance checking mode
void Rhs2000Registers::enableZcheck(bool enabled)
{
	zcheckEn = (enabled ? 1 : 0);
}

// Power up or down impedance checking DAC
void Rhs2000Registers::setZcheckDacPower(bool enabled)
{
	zcheckDacPower = (enabled ? 1 : 0);
}

// Select the series capacitor used to convert the voltage waveform generated by the on-chip
// DAC into an AC current waveform that stimulates a selected electrode for impedance testing
// (ZcheckCs100fF, ZcheckCs1pF, or Zcheck10pF).
void Rhs2000Registers::setZcheckScale(ZcheckCs scale)
{
	switch (scale) {
	case ZcheckCs100fF:
		zcheckScale = 0x00;     // Cs = 0.1 pF
		break;
	case ZcheckCs1pF:
		zcheckScale = 0x01;     // Cs = 1.0 pF
		break;
	case ZcheckCs10pF:
		zcheckScale = 0x03;     // Cs = 10.0 pF
		break;
	}
}

// Select the amplifier channel (0-15) for impedance testing.
int Rhs2000Registers::setZcheckChannel(int channel)
{
	if (channel < 0 || channel > 63) {
		return -1;
	}
	else {
		zcheckSelect = channel;
		return zcheckSelect;
	}
}

// Power up or down selected amplifier on chip
void Rhs2000Registers::setAmpPowered(int channel, bool powered)
{
	if (channel >= 0 && channel < 16) {
		ampPwr[channel] = (powered ? 1 : 0);
	}
}

// Power up all amplifiers on chip
void Rhs2000Registers::powerUpAllAmps()
{
	for (int channel = 0; channel < 16; ++channel) {
		ampPwr[channel] = 1;
	}
}

// Power down all amplifiers on chip
void Rhs2000Registers::powerDownAllAmps()
{
	for (int channel = 0; channel < 16; ++channel) {
		ampPwr[channel] = 0;
	}
}

// Power up or down selected DC amplifier on chip
void Rhs2000Registers::setDCAmpPowered(int channel, bool powered)
{
	if (channel >= 0 && channel < 16) {
		dcAmpPwr[channel] = (powered ? 1 : 0);
	}
}

// Power up all DC amplifiers on chip
void Rhs2000Registers::powerUpAllDCAmps()
{
	for (int channel = 0; channel < 16; ++channel) {
		dcAmpPwr[channel] = 1;
	}
}

// Power down all DC amplifiers on chip
void Rhs2000Registers::powerDownAllDCAmps()
{
	for (int channel = 0; channel < 16; ++channel) {
		dcAmpPwr[channel] = 0;
	}
}

// Enable or disable stimulation globally
void Rhs2000Registers::setStimEnable(bool enable)
{
	if (enable) {
		stimEnableA = 0xaaaa;
		stimEnableB = 0x00ff;
	}
	else {
		stimEnableA = 0x0000;
		stimEnableB = 0x0000;
	}
}

// Returns the value of a selected RAM register (0-250) on the RHS2116 chip, based
// on the current register variables in the class instance.
int Rhs2000Registers::getRegisterValue(int reg)
{
	int regout;
	const int zcheckDac = 128;  // midrange

	switch (reg) {
	case 0:
		regout = (adcBufferBias << 6) + muxBias;
		break;
	case 1:
		regout = (digOutOD << 12) + (digOut2 << 11) + (digOut2HiZ << 10) + (digOut1 << 9) +
			(digOut1HiZ << 8) + (weakMiso << 7) + (twosComp << 6) + (absMode << 5) +
			(dspEn << 4) + dspCutoffFreq;
		break;
	case 2:
		regout = (zcheckSelect << 8) + (zcheckDacPower << 6) + (zcheckLoad << 5) +
			(zcheckScale << 3) + zcheckEn;
		break;
	case 3:
		regout = zcheckDac;
		break;
	case 4:
		regout = (rH1Dac2 << 6) + rH1Dac1;
		break;
	case 5:
		regout = (rH2Dac2 << 6) + rH2Dac1;
		break;
	case 6:
		regout = (rLDac3A << 13) + (rLDac2A << 7) + rLDac1A;
		break;
	case 7:
		regout = (rLDac3B << 13) + (rLDac2B << 7) + rLDac1B;
		break;
	case 8:
		regout = vectorToWord(ampPwr);
		break;
	case 10:
		regout = vectorToWord(ampFastSettle);
		break;
	case 12:
		regout = vectorToWord(ampFLSelect);
		break;
	case 32:
		regout = stimEnableA;
		break;
	case 33:
		regout = stimEnableB;
		break;
	case 34:
		regout = (stimStepSel3 << 13) + (stimStepSel2 << 7) + stimStepSel1;
		break;
	case 35:
		regout = (stimPBias << 4) + stimNBias;
		break;
	case 36:
		regout = chargeRecoveryDac;
		break;
	case 37:
		regout = (chargeRecoveryCurrentLimitSel3 << 13) + (chargeRecoveryCurrentLimitSel2 << 7) +
			chargeRecoveryCurrentLimitSel1;
		break;
	case 38:
		regout = vectorToWord(dcAmpPwr);
		break;
	// Register 40 is read only
	case 42:
		regout = vectorToWord(stimOn);
		break;
	case 44:
		regout = vectorToWord(stimPol);
		break;
	case 46:
		regout = vectorToWord(chargeRecoverySwitch);
		break;
	case 48:
		regout = vectorToWord(cLChargeRecoveryEn);
		break;
	// Register 50 is read only
	case 64:
		regout = (negCurrentTrim[0] << 8) + negCurrentMag[0];
		break;
	case 65:
		regout = (negCurrentTrim[1] << 8) + negCurrentMag[1];
		break;
	case 66:
		regout = (negCurrentTrim[2] << 8) + negCurrentMag[2];
		break;
	case 67:
		regout = (negCurrentTrim[3] << 8) + negCurrentMag[3];
		break;
	case 68:
		regout = (negCurrentTrim[4] << 8) + negCurrentMag[4];
		break;
	case 69:
		regout = (negCurrentTrim[5] << 8) + negCurrentMag[5];
		break;
	case 70:
		regout = (negCurrentTrim[6] << 8) + negCurrentMag[6];
		break;
	case 71:
		regout = (negCurrentTrim[7] << 8) + negCurrentMag[7];
		break;
	case 72:
		regout = (negCurrentTrim[8] << 8) + negCurrentMag[8];
		break;
	case 73:
		regout = (negCurrentTrim[9] << 8) + negCurrentMag[9];
		break;
	case 74:
		regout = (negCurrentTrim[10] << 8) + negCurrentMag[10];
		break;
	case 75:
		regout = (negCurrentTrim[11] << 8) + negCurrentMag[11];
		break;
	case 76:
		regout = (negCurrentTrim[12] << 8) + negCurrentMag[12];
		break;
	case 77:
		regout = (negCurrentTrim[13] << 8) + negCurrentMag[13];
		break;
	case 78:
		regout = (negCurrentTrim[14] << 8) + negCurrentMag[14];
		break;
	case 79:
		regout = (negCurrentTrim[15] << 8) + negCurrentMag[15];
		break;
	case 96:
		regout = (posCurrentTrim[0] << 8) + posCurrentMag[0];
		break;
	case 97:
		regout = (posCurrentTrim[1] << 8) + posCurrentMag[1];
		break;
	case 98:
		regout = (posCurrentTrim[2] << 8) + posCurrentMag[2];
		break;
	case 99:
		regout = (posCurrentTrim[3] << 8) + posCurrentMag[3];
		break;
	case 100:
		regout = (posCurrentTrim[4] << 8) + posCurrentMag[4];
		break;
	case 101:
		regout = (posCurrentTrim[5] << 8) + posCurrentMag[5];
		break;
	case 102:
		regout = (posCurrentTrim[6] << 8) + posCurrentMag[6];
		break;
	case 103:
		regout = (posCurrentTrim[7] << 8) + posCurrentMag[7];
		break;
	case 104:
		regout = (posCurrentTrim[8] << 8) + posCurrentMag[8];
		break;
	case 105:
		regout = (posCurrentTrim[9] << 8) + posCurrentMag[9];
		break;
	case 106:
		regout = (posCurrentTrim[10] << 8) + posCurrentMag[10];
		break;
	case 107:
		regout = (posCurrentTrim[11] << 8) + posCurrentMag[11];
		break;
	case 108:
		regout = (posCurrentTrim[12] << 8) + posCurrentMag[12];
		break;
	case 109:
		regout = (posCurrentTrim[13] << 8) + posCurrentMag[13];
		break;
	case 110:
		regout = (posCurrentTrim[14] << 8) + posCurrentMag[14];
		break;
	case 111:
		regout = (posCurrentTrim[15] << 8) + posCurrentMag[15];
		break;
	default:
		regout = -1;
	}
	return regout;
}

// Convert a 16-bit vector to a 16-bit word
int Rhs2000Registers::vectorToWord(vector<int> &v)
{
	int word = 0;

	for (int i = 0; i < 16; i++) {
		if (v[i]) {
			word += (1 << i);
		}
	}

	return word;
}

// Returns the value of the RH1 resistor (in ohms) corresponding to a particular upper
// bandwidth value (in Hz).
double Rhs2000Registers::rH1FromUpperBandwidth(double upperBandwidth) const
{
	double log10f = log10(upperBandwidth);

	return 0.9730 * pow(10.0, (8.0968 - 1.1892 * log10f + 0.04767 * log10f * log10f));
}

// Returns the value of the RH2 resistor (in ohms) corresponding to a particular upper
// bandwidth value (in Hz).
double Rhs2000Registers::rH2FromUpperBandwidth(double upperBandwidth) const
{
	double log10f = log10(upperBandwidth);

	return 1.0191 * pow(10.0, (8.1009 - 1.0821 * log10f + 0.03383 * log10f * log10f));
}

// Returns the value of the RL resistor (in ohms) corresponding to a particular lower
// bandwidth value (in Hz).
double Rhs2000Registers::rLFromLowerBandwidth(double lowerBandwidth) const
{
	double log10f = log10(lowerBandwidth);

	if (lowerBandwidth < 4.0) {
		return 1.0061 * pow(10.0, (4.9391 - 1.2088 * log10f + 0.5698 * log10f * log10f +
			0.1442 * log10f * log10f * log10f));
	}
	else {
		return 1.0061 * pow(10.0, (4.7351 - 0.5916 * log10f + 0.08482 * log10f * log10f));
	}
}

// Returns the amplifier upper bandwidth (in Hz) corresponding to a particular value
// of the resistor RH1 (in ohms).
double Rhs2000Registers::upperBandwidthFromRH1(double rH1) const
{
	double a, b, c;

	a = 0.04767;
	b = -1.1892;
	c = 8.0968 - log10(rH1 / 0.9730);

	return pow(10.0, ((-b - sqrt(b * b - 4 * a * c)) / (2 * a)));
}

// Returns the amplifier upper bandwidth (in Hz) corresponding to a particular value
// of the resistor RH2 (in ohms).
double Rhs2000Registers::upperBandwidthFromRH2(double rH2) const
{
	double a, b, c;

	a = 0.03383;
	b = -1.0821;
	c = 8.1009 - log10(rH2 / 1.0191);

	return pow(10.0, ((-b - sqrt(b * b - 4 * a * c)) / (2 * a)));
}

// Returns the amplifier lower bandwidth (in Hz) corresponding to a particular value
// of the resistor RL (in ohms).
double Rhs2000Registers::lowerBandwidthFromRL(double rL) const
{
	double a, b, c;

	// Quadratic fit below is invalid for values of RL less than 5.1 kOhm
	if (rL < 5100.0) {
		rL = 5100.0;
	}

	if (rL < 30000.0) {
		a = 0.08482;
		b = -0.5916;
		c = 4.7351 - log10(rL / 1.0061);
	}
	else {
		a = 0.3303;
		b = -1.2100;
		c = 4.9873 - log10(rL / 1.0061);
	}

	return pow(10.0, ((-b - sqrt(b * b - 4 * a * c)) / (2 * a)));
}

// Sets the on-chip RH1 and RH2 DAC values appropriately to set a particular amplifier
// upper bandwidth (in Hz).  Returns an estimate of the actual upper bandwidth achieved.
double Rhs2000Registers::setUpperBandwidth(double upperBandwidth)
{
	const double RH1Base = 2200.0;
	const double RH1Dac1Unit = 600.0;
	const double RH1Dac2Unit = 29400.0;
	const int RH1Dac1Steps = 63;
	const int RH1Dac2Steps = 31;

	const double RH2Base = 8700.0;
	const double RH2Dac1Unit = 763.0;
	const double RH2Dac2Unit = 38400.0;
	const int RH2Dac1Steps = 63;
	const int RH2Dac2Steps = 31;

	double actualUpperBandwidth;
	double rH1Target, rH2Target;
	double rH1Actual, rH2Actual;
	int i;

	// Upper bandwidths higher than 30 kHz don't work well with the RHS2000 amplifiers
	if (upperBandwidth > 30000.0) {
		upperBandwidth = 30000.0;
	}

	rH1Target = rH1FromUpperBandwidth(upperBandwidth);

	rH1Dac1 = 0;
	rH1Dac2 = 0;
	rH1Actual = RH1Base;

	for (i = 0; i < RH1Dac2Steps; ++i) {
		if (rH1Actual < rH1Target - (RH1Dac2Unit - RH1Dac1Unit / 2)) {
			rH1Actual += RH1Dac2Unit;
			++rH1Dac2;
		}
	}

	for (i = 0; i < RH1Dac1Steps; ++i) {
		if (rH1Actual < rH1Target - (RH1Dac1Unit / 2)) {
			rH1Actual += RH1Dac1Unit;
			++rH1Dac1;
		}
	}

	rH2Target = rH2FromUpperBandwidth(upperBandwidth);

	rH2Dac1 = 0;
	rH2Dac2 = 0;
	rH2Actual = RH2Base;

	for (i = 0; i < RH2Dac2Steps; ++i) {
		if (rH2Actual < rH2Target - (RH2Dac2Unit - RH2Dac1Unit / 2)) {
			rH2Actual += RH2Dac2Unit;
			++rH2Dac2;
		}
	}

	for (i = 0; i < RH2Dac1Steps; ++i) {
		if (rH2Actual < rH2Target - (RH2Dac1Unit / 2)) {
			rH2Actual += RH2Dac1Unit;
			++rH2Dac1;
		}
	}

	double actualUpperBandwidth1, actualUpperBandwidth2;

	actualUpperBandwidth1 = upperBandwidthFromRH1(rH1Actual);
	actualUpperBandwidth2 = upperBandwidthFromRH2(rH2Actual);

	// Upper bandwidth estimates calculated from actual RH1 value and acutal RH2 value
	// should be very close; we will take their geometric mean to get a single
	// number.
	actualUpperBandwidth = sqrt(actualUpperBandwidth1 * actualUpperBandwidth2);

	/*
	cout << endl;
	cout << "Rhs2000Registers::setUpperBandwidth" << endl;
	cout << fixed << setprecision(1);

	cout << "RH1 DAC2 = " << rH1Dac2 << ", DAC1 = " << rH1Dac1 << endl;
	cout << "RH1 target: " << rH1Target << " Ohms" << endl;
	cout << "RH1 actual: " << rH1Actual << " Ohms" << endl;

	cout << "RH2 DAC2 = " << rH2Dac2 << ", DAC1 = " << rH2Dac1 << endl;
	cout << "RH2 target: " << rH2Target << " Ohms" << endl;
	cout << "RH2 actual: " << rH2Actual << " Ohms" << endl;

	cout << "Upper bandwidth target: " << upperBandwidth << " Hz" << endl;
	cout << "Upper bandwidth actual: " << actualUpperBandwidth << " Hz" << endl;

	cout << endl;
	cout << setprecision(6);
	cout.unsetf(ios::floatfield);
	*/

	return actualUpperBandwidth;
}

// Sets the on-chip RL DAC values appropriately to set a particular amplifier
// lower bandwidth (in Hz).  If select == 1, fL_A is set; if select == 0, fL_B
// is set.  Returns an estimate of the actual lower bandwidth achieved.
double Rhs2000Registers::setLowerBandwidth(double lowerBandwidth, int select)
{
	const double RLBase = 3500.0;
	const double RLDac1Unit = 175.0;
	const double RLDac2Unit = 12700.0;
	const double RLDac3Unit = 3000000.0;
	const int RLDac1Steps = 127;
	const int RLDac2Steps = 63;

	double actualLowerBandwidth;
	double rLTarget;
	double rLActual;
	int i, rLDac1, rLDac2, rLDac3;

	// Lower bandwidths higher than 1.5 kHz don't work well with the Rhs2000 amplifiers
	if (lowerBandwidth > 1500.0) {
		lowerBandwidth = 1500.0;
	}

	rLTarget = rLFromLowerBandwidth(lowerBandwidth);

	rLDac1 = 0;
	rLDac2 = 0;
	rLDac3 = 0;
	rLActual = RLBase;

	if (lowerBandwidth < 0.15) {
		rLActual += RLDac3Unit;
		++rLDac3;
	}

	for (i = 0; i < RLDac2Steps; ++i) {
		if (rLActual < rLTarget - (RLDac2Unit - RLDac1Unit / 2)) {
			rLActual += RLDac2Unit;
			++rLDac2;
		}
	}

	for (i = 0; i < RLDac1Steps; ++i) {
		if (rLActual < rLTarget - (RLDac1Unit / 2)) {
			rLActual += RLDac1Unit;
			++rLDac1;
		}
	}

	actualLowerBandwidth = lowerBandwidthFromRL(rLActual);

	if (select) {
		rLDac1A = rLDac1;
		rLDac2A = rLDac2;
		rLDac3A = rLDac3;
	}
	else {
		rLDac1B = rLDac1;
		rLDac2B = rLDac2;
		rLDac3B = rLDac3;
	}

    /*
	cout << endl;
	cout << fixed << setprecision(1);
	cout << "Rhs2000Registers::setLowerBandwidth" << endl;

	cout << "RL DAC3 = " << rLDac3 << ", DAC2 = " << rLDac2 << ", DAC1 = " << rLDac1 << endl;
	cout << "RL target: " << rLTarget << " Ohms" << endl;
	cout << "RL actual: " << rLActual << " Ohms" << endl;

	cout << setprecision(3);

	cout << "Lower bandwidth target: " << lowerBandwidth << " Hz" << endl;
	cout << "Lower bandwidth actual: " << actualLowerBandwidth << " Hz" << endl;

	cout << endl;
	cout << setprecision(6);
	cout.unsetf(ios::floatfield);
    */

	return actualLowerBandwidth;
}

// Set the stimulation current DAC step size
void Rhs2000Registers::setStimStepSize(StimStepSize step)
{
	switch (step) {
	case StimStepSizeMin:
		stimStepSel1 = 127;
		stimStepSel2 = 63;
		stimStepSel3 = 3;
		stimPBias = 6;
		stimNBias = 6;
		break;
	case StimStepSize10nA:
		stimStepSel1 = 64;
		stimStepSel2 = 19;
		stimStepSel3 = 3;
		stimPBias = 6;
		stimNBias = 6;
		break;
	case StimStepSize20nA:
		stimStepSel1 = 40;
		stimStepSel2 = 40;
		stimStepSel3 = 1;
		stimPBias = 7;
		stimNBias = 7;
		break;
	case StimStepSize50nA:
		stimStepSel1 = 64;
		stimStepSel2 = 40;
		stimStepSel3 = 0;
		stimPBias = 7;
		stimNBias = 7;
		break;
	case StimStepSize100nA:
		stimStepSel1 = 30;
		stimStepSel2 = 20;
		stimStepSel3 = 0;
		stimPBias = 7;
		stimNBias = 7;
		break;
	case StimStepSize200nA:
		stimStepSel1 = 25;
		stimStepSel2 = 10;
		stimStepSel3 = 0;
		stimPBias = 8;
		stimNBias = 8;
		break;
	case StimStepSize500nA:
		stimStepSel1 = 101;
		stimStepSel2 = 3;
		stimStepSel3 = 0;
		stimPBias = 9;
		stimNBias = 9;
		break;
	case StimStepSize1uA:
		stimStepSel1 = 98;
		stimStepSel2 = 1;
		stimStepSel3 = 0;
		stimPBias = 10;
		stimNBias = 10;
		break;
	case StimStepSize2uA:
		stimStepSel1 = 94;
		stimStepSel2 = 0;
		stimStepSel3 = 0;
		stimPBias = 11;
		stimNBias = 11;
		break;
	case StimStepSize5uA:
		stimStepSel1 = 38;
		stimStepSel2 = 0;
		stimStepSel3 = 0;
		stimPBias = 14;
		stimNBias = 14;
		break;
	case StimStepSize10uA:
		stimStepSel1 = 15;
		stimStepSel2 = 0;
		stimStepSel3 = 0;
		stimPBias = 15;
		stimNBias = 15;
		break;
	case StimStepSizeMax:
		stimStepSel1 = 0;
		stimStepSel2 = 0;
		stimStepSel3 = 0;
		stimPBias = 15;
		stimNBias = 15;
		break;
	}
}

// Convert stimulation step size enum to floating point current value (in amps).
double Rhs2000Registers::stimStepSizeToDouble(StimStepSize step)
{
    switch (step) {
    case StimStepSize10nA:
        return 10.0e-9;
    case StimStepSize20nA:
        return 20.0e-9;
    case StimStepSize50nA:
        return 50.0e-9;
    case StimStepSize100nA:
        return 100.0e-9;
    case StimStepSize200nA:
        return 200.0e-9;
    case StimStepSize500nA:
        return 500.0e-9;
    case StimStepSize1uA:
        return 1.0e-6;
    case StimStepSize2uA:
        return 2.0e-6;
    case StimStepSize5uA:
        return 5.0e-6;
    case StimStepSize10uA:
        return 10.0e-6;
    case StimStepSizeMin:
    case StimStepSizeMax:
    default:
        return std::numeric_limits<double>::quiet_NaN();
    }
}

// Set positive stimulation magnitude (0 to 255, in DAC steps) and trim (-128 to +127)
// for a channel (0-15).
int Rhs2000Registers::setPosStimMagnitude(int channel, int magnitude, int trim)
{
	if (channel < 0 || channel > 15) {
		cerr << "Error in Rhs2000Registers::setPosStimMagnitude: channel out of range." << endl;
		return -1;
	}
	if (magnitude < 0 || magnitude > 255) {
		cerr << "Error in Rhs2000Registers::setPosStimMagnitude: magnitude out of range." << endl;
		return -1;
	}
	if (trim < -128 || trim > 127) {
		cerr << "Error in Rhs2000Registers::setPosStimMagnitude: trim out of range." << endl;
		return -1;
	}
	posCurrentMag[channel] = magnitude;
	posCurrentTrim[channel] = trim + 128;
	return 0;
}

// Set negative stimulation magnitude (0 to 255, in DAC steps) and trim (-128 to +127)
// for a channel (0-15).
int Rhs2000Registers::setNegStimMagnitude(int channel, int magnitude, int trim)
{
	if (channel < 0 || channel > 15) {
		cerr << "Error in Rhs2000Registers::setNegStimMagnitude: channel out of range." << endl;
		return -1;
	}
	if (magnitude < 0 || magnitude > 255) {
		cerr << "Error in Rhs2000Registers::setNegStimMagnitude: magnitude out of range." << endl;
		return -1;
	}
	if (trim < -128 || trim > 127) {
		cerr << "Error in Rhs2000Registers::setNegStimMagnitude: trim out of range." << endl;
		return -1;
	}
	negCurrentMag[channel] = magnitude;
	negCurrentTrim[channel] = trim + 128;
	return 0;
}

// Set charge recovery current limit.
void Rhs2000Registers::setChargeRecoveryCurrentLimit(ChargeRecoveryCurrentLimit limit)
{
	switch (limit) {
	case CurrentLimitMin:
		chargeRecoveryCurrentLimitSel1 = 127;
		chargeRecoveryCurrentLimitSel2 = 63;
		chargeRecoveryCurrentLimitSel3 = 3;
		break;
	case CurrentLimit1nA:
		chargeRecoveryCurrentLimitSel1 = 0;
		chargeRecoveryCurrentLimitSel2 = 30;
		chargeRecoveryCurrentLimitSel3 = 2;
		break;
	case CurrentLimit2nA:
		chargeRecoveryCurrentLimitSel1 = 0;
		chargeRecoveryCurrentLimitSel2 = 15;
		chargeRecoveryCurrentLimitSel3 = 1;
		break;
	case CurrentLimit5nA:
		chargeRecoveryCurrentLimitSel1 = 0;
		chargeRecoveryCurrentLimitSel2 = 31;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit10nA:
		chargeRecoveryCurrentLimitSel1 = 50;
		chargeRecoveryCurrentLimitSel2 = 15;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit20nA:
		chargeRecoveryCurrentLimitSel1 = 78;
		chargeRecoveryCurrentLimitSel2 = 7;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit50nA:
		chargeRecoveryCurrentLimitSel1 = 22;
		chargeRecoveryCurrentLimitSel2 = 3;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit100nA:
		chargeRecoveryCurrentLimitSel1 = 56;
		chargeRecoveryCurrentLimitSel2 = 1;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit200nA:
		chargeRecoveryCurrentLimitSel1 = 71;
		chargeRecoveryCurrentLimitSel2 = 0;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit500nA:
		chargeRecoveryCurrentLimitSel1 = 26;
		chargeRecoveryCurrentLimitSel2 = 0;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimit1uA:
		chargeRecoveryCurrentLimitSel1 = 9;
		chargeRecoveryCurrentLimitSel2 = 0;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	case CurrentLimitMax:
		chargeRecoveryCurrentLimitSel1 = 0;
		chargeRecoveryCurrentLimitSel2 = 0;
		chargeRecoveryCurrentLimitSel3 = 0;
		break;
	}
}

// Convert current limit enum to floating point current value (in amps).
double Rhs2000Registers::chargeRecoveryCurrentLimitToDouble(ChargeRecoveryCurrentLimit limit)
{
    switch (limit) {
    case CurrentLimit1nA:
        return 1.0e-9;
    case CurrentLimit2nA:
        return 2.0e-9;
    case CurrentLimit5nA:
        return 5.0e-9;
    case CurrentLimit10nA:
        return 10.0e-9;
    case CurrentLimit20nA:
        return 20.0e-9;
    case CurrentLimit50nA:
        return 50.0e-9;
    case CurrentLimit100nA:
        return 100.0e-9;
    case CurrentLimit200nA:
        return 200.0e-9;
    case CurrentLimit500nA:
        return 500.0e-9;
    case CurrentLimit1uA:
        return 1.0e-6;
    case CurrentLimitMin:
    case CurrentLimitMax:
    default:
        return std::numeric_limits<double>::quiet_NaN();
    }
}

// Set the target voltage for current-limited charge recovery.  The parameter
// vTarget should specify a voltage in the range of -1.225 to +1.215 (units = volts).
// Returns actual value of target voltage.
double Rhs2000Registers::setChargeRecoveryTargetVoltage(double vTarget)
{
	double dacStep = 1.225 / 128.0;
	int value;

	value = ((int)floor(vTarget / dacStep + 0.5)) + 128;
	if (value < 0) value = 0;
	if (value > 255) value = 255;

	chargeRecoveryDac = value;
	return dacStep * (value - 128);
}

// Return a 32-bit MOSI command (CALIBRATE or CLEAR)
unsigned int Rhs2000Registers::createRhs2000Command(Rhs2000CommandType commandType)
{
	switch (commandType) {
	case Rhs2000CommandCalibrate:
		return 0x55000000;   // 01010101 00000000 00000000 00000000
		break;
	case Rhs2000CommandCalClear:
		return 0x6a000000;   // 01101010 00000000 00000000 00000000
		break;
	case Rhs2000CommandComplianceReset:
		return 0xd0ff0000;   // 11010000 11111111 00000000 00000000 (Read from Register 255 with M flag set)
		break;
	default:
		cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
			"Only 'Calibrate', 'Clear Calibration', or 'Compliance Monitor Reset' commands take zero arguments." << endl;
		return 0xffffffff;
	}
}

// Return a 32-bit MOSI command (CONVERT or READ)
unsigned int Rhs2000Registers::createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1)
{
	switch (commandType) {
	case Rhs2000CommandConvert:
		if (arg1 > 15) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Channel number out of range." << endl;
			return 0xffffffff;
		}
		return 0x00000000 + (arg1 << 16);  // 00umdh00 00cccccc 00000000 00000000; if the command is 'Convert',
									       // arg1 is the channel number
		break;
	case Rhs2000CommandRegRead:
		if (arg1 > 255) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Register address out of range." << endl;
			return 0xffffffff;
		}
		return 0xc0000000 + (arg1 << 16);  // 11um0000 rrrrrrrr 00000000 00000000; if the command is 'Register Read',
									       // arg1 is the register address
		break;
	default:
		cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
			"Only 'Convert' and 'Register Read' commands take one argument." << endl;
		return 0xffffffff;
	}
}

// Return a 32-bit MOSI command (WRITE)
unsigned int Rhs2000Registers::createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1, unsigned int arg2)
{
	switch (commandType) {
	case Rhs2000CommandRegWrite:
		if (arg1 > 255) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Register address out of range." << endl;
			return 0xffffffff;
		}
		if (arg2 > 65535) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Register data out of range." << endl;
			return 0xffffffff;
		}
		return 0x80000000 + (arg1 << 16) + arg2; // 10um0000 rrrrrrrr dddddddd dddddddd; if the command is 'Register Write',
										 	     // arg1 is the register address and arg2 is the data
		break;
	case Rhs2000CommandConvert:
		if (arg1 > 15) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Channel number out of range." << endl;
			return 0xffffffff;
		}
		return 0x00000000 + (arg2 << 26) + (arg1 << 16);  // 00umdh00 00cccccc 00000000 00000000; if the command is 'Convert',
														  // arg1 is the channel number and arg2 is the H flag
		break;
	default:
		cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
			"Only 'Register Write' and 'Convert' commands take two arguments." << endl;
		return 0xffffffff;
	}
}

// Return a 32-bit MOSI command (WRITE)
unsigned int Rhs2000Registers::createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1, unsigned int arg2, unsigned int uFlag, unsigned int mFlag)
{
	switch (commandType) {
	case Rhs2000CommandRegWrite:
		if (arg1 > 255) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Register address out of range." << endl;
			return 0xffffffff;
		}
		if (arg2 > 65535) {
			cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
				"Register data out of range." << endl;
			return 0xffffffff;
		}
		return 0x80000000 + (uFlag << 29) + (mFlag << 28) + (arg1 << 16) + arg2; // 10um0000 rrrrrrrr dddddddd dddddddd; if the command is 'Register Write',
												 // arg1 is the register address and arg2 is the data
		break;
	default:
		cerr << "Error in Rhs2000Registers::createRhs2000Command: " <<
			"Only 'Register Write' commands take four arguments." << endl;
		return 0xffffffff;
	}
}

// Create a list of 128 commands to program most RAM registers on an RHS2116 chip, read those values
// back to confirm programming, and read ROM registers.
// If updateStimParams == true, update stimulation amplitudes and other charge-recovery parameters.
// Returns the length of the command list.
int Rhs2000Registers::createCommandListRegisterConfig(vector<unsigned int> &commandList, bool updateStimParams)
{
	commandList.clear();    // if command list already exists, erase it and start a new one

							// Start with a few dummy commands in case chip is still powering up
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

	// Program 53 RAM registers
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 0, getRegisterValue(0)));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 1, getRegisterValue(1)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 2, getRegisterValue(2)));
	// Don't program Register 3 (Impedance Check DAC) here; create DAC waveform in another command stream
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 4, getRegisterValue(4)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 5, getRegisterValue(5)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 6, getRegisterValue(6)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 7, getRegisterValue(7)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 8, getRegisterValue(8)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 10, getRegisterValue(10)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 12, getRegisterValue(12)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 32, getRegisterValue(32)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 33, getRegisterValue(33)));
    if (updateStimParams) {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 34, getRegisterValue(34)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 35, getRegisterValue(35)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 36, getRegisterValue(36)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 37, getRegisterValue(37)));
    } else {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    }
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 38, getRegisterValue(38)));
	// Register 40 (Compliance Monitor) is read only; clear it here:
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 40, 0, 0, 1));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 42, getRegisterValue(42)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 44, getRegisterValue(44)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 46, getRegisterValue(46)));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 48, getRegisterValue(48)));
	// Register 50 (Fault Current Detect) is read only

    if (updateStimParams) {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 64, getRegisterValue(64)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 65, getRegisterValue(65)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 66, getRegisterValue(66)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 67, getRegisterValue(67)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 68, getRegisterValue(68)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 69, getRegisterValue(69)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 70, getRegisterValue(70)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 71, getRegisterValue(71)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 72, getRegisterValue(72)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 73, getRegisterValue(73)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 74, getRegisterValue(74)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 75, getRegisterValue(75)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 76, getRegisterValue(76)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 77, getRegisterValue(77)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 78, getRegisterValue(78)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 79, getRegisterValue(79)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 96, getRegisterValue(96)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 97, getRegisterValue(97)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 98, getRegisterValue(98)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 99, getRegisterValue(99)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 100, getRegisterValue(100)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 101, getRegisterValue(101)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 102, getRegisterValue(102)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 103, getRegisterValue(103)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 104, getRegisterValue(104)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 105, getRegisterValue(105)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 106, getRegisterValue(106)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 107, getRegisterValue(107)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 108, getRegisterValue(108)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 109, getRegisterValue(109)));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 110, getRegisterValue(110)));
        // Update all triggered registers with last WRITE command:
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 111, getRegisterValue(111), 1, 0));
    } else {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    }

	// Read 5 ROM registers
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 254));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 253));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 252));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 251));

	// Read back 56 RAM registers to confirm programming
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 0));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 1));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 2));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 3));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 4));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 5));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 6));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 7));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 8));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 10));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 12));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 32));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 33));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 34));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 35));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 36));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 37));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 38));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 40));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 42));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 44));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 46));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 48));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 50));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 64));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 65));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 66));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 67));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 68));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 69));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 70));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 71));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 72));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 73));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 74));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 75));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 76));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 77));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 78));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 79));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 96));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 97));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 98));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 99));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 100));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 101));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 102));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 103));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 104));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 105));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 106));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 107));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 108));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 109));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 110));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 111));

    // ADC calibration should not be performed on RHS2116; rather, CLEAR command should be sent.
    commandList.push_back(createRhs2000Command(Rhs2000CommandCalClear));

	// End with dummy commands
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

	return commandList.size();
}

// Read all registers from chip without changing any values.
int Rhs2000Registers::createCommandListRegisterRead(vector<unsigned int> &commandList)
{
    commandList.clear();    // if command list already exists, erase it and start a new one

                            // Start with a few dummy commands in case chip is still powering up
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

    for (int i = 0; i < 54; i++) {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    }

    // Read 5 ROM registers
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 254));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 253));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 252));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 251));

    // Read back 56 RAM registers to confirm programming
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 0));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 1));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 2));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 3));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 4));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 5));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 6));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 7));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 8));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 10));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 12));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 32));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 33));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 34));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 35));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 36));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 37));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 38));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 40));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 42));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 44));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 46));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 48));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 50));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 64));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 65));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 66));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 67));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 68));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 69));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 70));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 71));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 72));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 73));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 74));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 75));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 76));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 77));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 78));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 79));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 96));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 97));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 98));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 99));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 100));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 101));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 102));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 103));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 104));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 105));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 106));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 107));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 108));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 109));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 110));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 111));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

    // End with dummy commands
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

    return commandList.size();
}

// Create a list of up to 8192 commands to generate a sine wave of particular frequency (in Hz) and
// amplitude (in DAC steps, 0-128) using the on-chip impedance testing voltage DAC.  If frequency is set to zero,
// a DC baseline waveform is created.
// Returns the length of the command list.
int Rhs2000Registers::createCommandListZcheckDac(vector<unsigned int> &commandList, double frequency, double amplitude)
{
	int i, period, value;
	double t;
	const double Pi = 2 * acos(0.0);

	commandList.clear();    // if command list already exists, erase it and start a new one

	if (amplitude < 0.0 || amplitude > 128.0) {
		cerr << "Error in Rhs2000Registers::createCommandListZcheckDac: Amplitude out of range." << endl;
		return -1;
	}
	if (frequency < 0.0) {
		cerr << "Error in Rhs2000Registers::createCommandListZcheckDac: Negative frequency not allowed." << endl;
		return -1;
	}
	else if (frequency > sampleRate / 4.0) {
		cerr << "Error in Rhs2000Registers::createCommandListZcheckDac: " <<
			"Frequency too high relative to sampling rate." << endl;
		return -1;
	}
	if (frequency == 0.0) {
		for (i = 0; i < MaxCommandLength; ++i) {
            commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 3, 128));
		}
	}
	else {
		period = (int)floor(sampleRate / frequency + 0.5);
		if (period > MaxCommandLength) {
			cerr << "Error in Rhs2000Registers::createCommandListZcheckDac: Frequency too low." << endl;
			return -1;
		}
		else {
			t = 0.0;
			for (i = 0; i < period; ++i) {
				value = (int)floor(amplitude * sin(2 * Pi * frequency * t) + 128.0 + 0.5);
				if (value < 0) {
					value = 0;
				}
				else if (value > 255) {
					value = 255;
				}
				commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 3, value));
				t += 1.0 / sampleRate;
			}
		}
	}

	return commandList.size();
}

// Create a list of dummy commands for the RHS2116 chip.
// Returns the length of the command list (which should be n).
int Rhs2000Registers::createCommandListDummy(vector <unsigned int> &commandList, int n)
{
	commandList.clear();

	for (int i = 0; i < n; i++) {
		commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	}

	return commandList.size();
}

// Create a list of dummy commands for the RHS2116 chip with a specific command.
// Returns the length of the command list (which should be n).
int Rhs2000Registers::createCommandListDummy(vector <unsigned int> &commandList, int n, unsigned int cmd)
{
	commandList.clear();

	for (int i = 0; i < n; i++) {
		commandList.push_back(cmd);
	}

	return commandList.size();
}

// Create a list of commands for the RHS2116 chip to update a single RAM register.
// Returns the length of the command list (which should be 128).
int Rhs2000Registers::createCommandListSingleRegisterConfig(vector<unsigned int> &commandList, int reg)
{
	commandList.clear();    // If command list already exists, erase it and start a new one.

							// Start with two dummy commands.
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

	// Now, configure RAM register.
	commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, reg, getRegisterValue(reg)));

    // More dummy commands to make 128 total commands
    while (commandList.size() < 128) {
		commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
	}

	return commandList.size();
}

// Set positive and negative stimulation magnitude and trim parameters for a single channel.
// Returns the length of the command list (which should be 128).
int Rhs2000Registers::createCommandListSetStimMagnitudes(vector<unsigned int> &commandList, int channel,
                                                         int posMag, int posTrim, int negMag, int negTrim)
{
    commandList.clear();    // If command list already exists, erase it and start a new one.

                            // Start with two dummy commands.
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

    setPosStimMagnitude(channel, posMag, posTrim);
    setNegStimMagnitude(channel, negMag, negTrim);

    // Now, configure RAM registers.
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 96 + channel, getRegisterValue(96 + channel), 1, 0)); // positive register; update
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 64 + channel, getRegisterValue(64 + channel), 1, 0)); // negative register; update

    // More dummy commands to make 128 total commands
    while (commandList.size() < 128) {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    }

    return commandList.size();
}

// Set charge recovery current limit and target voltage (Registers 36 and 37).
// Returns the length of the command list (which should be 128).
int Rhs2000Registers::createCommandListConfigChargeRecovery(vector<unsigned int> &commandList,
                                                            ChargeRecoveryCurrentLimit currentLimit, double targetVoltage)
{
    commandList.clear();    // If command list already exists, erase it and start a new one.

                            // Start with two dummy commands.
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));

    setChargeRecoveryCurrentLimit(currentLimit);		// set charge recovery current limit
    setChargeRecoveryTargetVoltage(targetVoltage);      // set charge recovery target voltage

    // Now, configure RAM registers.
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 36, getRegisterValue(36), 0, 0));
    commandList.push_back(createRhs2000Command(Rhs2000CommandRegWrite, 37, getRegisterValue(37), 0, 0));

    // More dummy commands to make 128 total commands
    while (commandList.size() < 128) {
        commandList.push_back(createRhs2000Command(Rhs2000CommandRegRead, 255));
    }

    return commandList.size();
}
