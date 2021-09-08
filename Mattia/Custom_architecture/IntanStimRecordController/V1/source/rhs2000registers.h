//----------------------------------------------------------------------------------
// rhs2000registers.h
//
// Intan Technoloies RHS2000 Interface API
// Rhs2000Registers Class Header File
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

#ifndef RHS2000REGISTERS_H
#define RHS2000REGISTERS_H

using namespace std;

class Rhs2000Registers
{

public:
    enum StimStepSize {
        StimStepSizeMin = 0,
        StimStepSize10nA = 1,
        StimStepSize20nA = 2,
        StimStepSize50nA = 3,
        StimStepSize100nA = 4,
        StimStepSize200nA = 5,
        StimStepSize500nA = 6,
        StimStepSize1uA = 7,
        StimStepSize2uA = 8,
        StimStepSize5uA = 9,
        StimStepSize10uA = 10,
        StimStepSizeMax = 11
    };

    Rhs2000Registers(double sampleRate, StimStepSize stimStep_);

	void defineSampleRate(double newSampleRate);

	enum DigOut {
		DigOut1,
		DigOut2,
		DigOutOD
	};

	void setDigOutLow(DigOut pin);
	void setDigOutHigh(DigOut pin);
	void setDigOutHiZ(DigOut pin);

	void enableDsp(bool enabled);
	double setDspCutoffFreq(double newDspCutoffFreq);
	double getDspCutoffFreq() const;

	void enableZcheck(bool enabled);
	void setZcheckDacPower(bool enabled);

	enum ZcheckCs {
		ZcheckCs100fF,
		ZcheckCs1pF,
		ZcheckCs10pF
	};

	enum ZcheckPolarity {
		ZcheckPositiveInput,
		ZcheckNegativeInput
	};

	void setZcheckScale(ZcheckCs scale);
	int setZcheckChannel(int channel);

	void setAmpPowered(int channel, bool powered);
	void powerUpAllAmps();
	void powerDownAllAmps();
	void setDCAmpPowered(int channel, bool powered);
	void powerUpAllDCAmps();
	void powerDownAllDCAmps();

	void setStimEnable(bool enable);

	int getRegisterValue(int reg);

	double setUpperBandwidth(double upperBandwidth);
	double setLowerBandwidth(double lowerBandwidth, int select);

	void setStimStepSize(StimStepSize step);
    static double stimStepSizeToDouble(StimStepSize step);
	int setPosStimMagnitude(int channel, int magnitude, int trim);
	int setNegStimMagnitude(int channel, int magnitude, int trim);

	enum ChargeRecoveryCurrentLimit {
        CurrentLimitMin = 0,
        CurrentLimit1nA,
        CurrentLimit2nA,
        CurrentLimit5nA,
        CurrentLimit10nA,
        CurrentLimit20nA,
        CurrentLimit50nA,
        CurrentLimit100nA,
        CurrentLimit200nA,
        CurrentLimit500nA,
        CurrentLimit1uA,
        CurrentLimitMax
	};

	void setChargeRecoveryCurrentLimit(ChargeRecoveryCurrentLimit limit);
    static double chargeRecoveryCurrentLimitToDouble(ChargeRecoveryCurrentLimit limit);
	double setChargeRecoveryTargetVoltage(double vTarget);

    int createCommandListRegisterConfig(vector<unsigned int> &commandList, bool updateStimParams);
    int createCommandListRegisterRead(vector<unsigned int> &commandList);
	int createCommandListZcheckDac(vector<unsigned int> &commandList, double frequency, double amplitude);
	int createCommandListDummy(vector <unsigned int> &commandList, int n);
	int createCommandListDummy(vector <unsigned int> &commandList, int n, unsigned int cmd);
	int createCommandListSingleRegisterConfig(vector<unsigned int> &commandList, int reg);
    int createCommandListSetStimMagnitudes(vector<unsigned int> &commandList, int channel,
                                           int posMag, int posTrim, int negMag, int negTrim);
    int createCommandListConfigChargeRecovery(vector<unsigned int> &commandList, ChargeRecoveryCurrentLimit currentLimit, double targetVoltage);

	enum Rhs2000CommandType {
		Rhs2000CommandConvert,
		Rhs2000CommandCalibrate,
		Rhs2000CommandCalClear,
		Rhs2000CommandRegWrite,
		Rhs2000CommandRegRead,
		Rhs2000CommandComplianceReset
	};

	unsigned int createRhs2000Command(Rhs2000CommandType commandType);
	unsigned int createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1);
	unsigned int createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1, unsigned int arg2);
	unsigned int createRhs2000Command(Rhs2000CommandType commandType, unsigned int arg1, unsigned int arg2, unsigned int uFlag, unsigned int mFlag);

private:
	double sampleRate;
    StimStepSize stimStep;

	// RHS2000 Register 0 variables
	int adcBufferBias;
	int muxBias;

	// RHS2000 Register 1 variables
	int digOutOD;
	int digOut1;
	int digOut1HiZ;
	int digOut2;
	int digOut2HiZ;
	int weakMiso;
	int twosComp;
	int absMode;
	int dspEn;
	int dspCutoffFreq;

	// RHS2000 Register 2 variables
	int zcheckSelect;
	int zcheckDacPower;
	int zcheckLoad;
	int zcheckScale;
	int zcheckEn;

	// RHS2000 Register 3 variables
    // int zcheckDac;     // handle Zcheck DAC waveform elsewhere

	// RHS2000 Register 4-7 variables
	int rH1Dac1;
	int rH1Dac2;
	int rH2Dac1;
	int rH2Dac2;
	int rLDac1A;
	int rLDac2A;
	int rLDac3A;
	int rLDac1B;
	int rLDac2B;
	int rLDac3B;

	// RHS2000 Register 8 variables
	vector<int> ampPwr;

	// RHS2000 Register 10 variables
	vector<int> ampFastSettle;

	// RHS2000 Register 12 variables
	vector<int> ampFLSelect;

	// RHS2000 Register 32 variables
	int stimEnableA;

	// RHS2000 Register 33 variables
	int stimEnableB;

	// RHS2000 Register 34 variables
	int stimStepSel1;
	int stimStepSel2;
	int stimStepSel3;

	// RHS2000 Register 35 variables
	int stimNBias;
	int stimPBias;

	// RHS2000 Register 36 variables
	int chargeRecoveryDac;

	// RHS2000 Register 37 variables
	int chargeRecoveryCurrentLimitSel1;
	int chargeRecoveryCurrentLimitSel2;
	int chargeRecoveryCurrentLimitSel3;

	// RHS2000 Register 38 variables
	vector<int> dcAmpPwr;

	// RHS2000 Register 40 variables
	vector<int> complianceMonitor;

	// RHS2000 Register 42 variables
	vector<int> stimOn;

	// RHS2000 Register 44 variables
	vector<int> stimPol;

	// RHS2000 Register 46 variables
	vector<int> chargeRecoverySwitch;

	// RHS2000 Register 48 variables
	vector<int> cLChargeRecoveryEn;

	// RHS2000 Register 50 variables
	int faultCurrentDetect;

	// RHS2000 Register 64-79 variables
	vector<int> negCurrentMag;
	vector<int> negCurrentTrim;

	// RHS2000 Register 96-111 variables
	vector<int> posCurrentMag;
	vector<int> posCurrentTrim;

	double rH1FromUpperBandwidth(double upperBandwidth) const;
	double rH2FromUpperBandwidth(double upperBandwidth) const;
	double rLFromLowerBandwidth(double lowerBandwidth) const;
	double upperBandwidthFromRH1(double rH1) const;
	double upperBandwidthFromRH2(double rH2) const;
	double lowerBandwidthFromRL(double rL) const;

	int vectorToWord(vector<int> &v);

	static const int MaxCommandLength = 8192; // size of on-FPGA auxiliary command RAM banks

};

#endif // RHS2000REGISTERS_H
