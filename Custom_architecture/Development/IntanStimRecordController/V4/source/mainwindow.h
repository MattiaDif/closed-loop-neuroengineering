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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>
#include "rhs2000datablock.h"
#include "rhs2000evalboard.h"
#include "rhs2000registers.h"
#include "globalconstants.h"
#include "stimparameters.h"

class QAction;
class QPushButton;
class QToolButton;
class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QComboBox;
class QSlider;
class QLineEdit;
class QLabel;
class QFile;
class WavePlot;
class SignalProcessor;
class Rhs2000EvalBoard;
class SignalSources;
class SignalGroup;
class SignalChannel;
class SpikeScopeDialog;
class KeyboardShortcutDialog;
class HelpDialogChipFilters;
class HelpDialogComparators;
class HelpDialogDacs;
class HelpDialogHighpassFilter;
class HelpDialogNotchFilter;
class HelpDialogFastSettle;
class HelpDialogReference;
class HelpDialogIOExpander;
class WaitForTriggerDialog;
class UsbDataThread;
class DataStreamFifo;
class StimParamDialog;
class DigOutDialog;
class AnOutDialog;
class AmpSettleDialog;
class ChargeRecoveryDialog;

using namespace std;

struct ReferenceSource {
    int stream;
    int channel;
    bool softwareMode;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int sampleRateIndex_, int stimStepIndex);
    ~MainWindow();

    void setNumWaveformsComboBox(int index);

    QComboBox *yScaleComboBox;
    QComboBox *yScaleDcAmpComboBox;
    QComboBox *yScaleAdcComboBox;
    QComboBox *tScaleComboBox;
    QComboBox *ampComboBox;

    QVector<int> yScaleList;
    QVector<int> yScaleDcAmpList;
    QVector<int> yScaleAdcList;
    QVector<int> tScaleList;
    QVector<QVector<bool> > channelVisible;

    int getEvalBoardMode();
    bool isRecording();
    ReferenceSource getReferenceSource();
    bool markerMode();
    bool resetXOnMarker();
    int markerChannel();
    bool showV0Axis();
    void setManualStimTrigger(int trigger, bool triggerOn);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void focusChanged(QWidget *lostFocus, QWidget *gainedFocus);

public slots:
    void notifyFocusChanged(QWidget *lostFocus, QWidget *gainedFocus);
    void copyStimParameters();
    void pasteStimParameters();

private slots:
    void about();
    void keyboardShortcutsHelp();
    void chipFiltersHelp();
    void comparatorsHelp();
    void dacsHelp();
    void highpassFilterHelp();
    void notchFilterHelp();
    void fastSettleHelp();
    void ioExpanderHelp();
    void openIntanWebsite();
    void runInterfaceBoard();
    void recordInterfaceBoard();
    void triggerRecordInterfaceBoard();
    void stopInterfaceBoard();
    void selectBaseFilenameSlot();
    void changeNumFrames(int index);
    void changeYScale(int index);
    void changeYScaleDcAmp(int index);
    void changeYScaleAdc(int index);
    void changeAmpType(int index);
    void changeTScale(int index);
    void changeSampleRate(int sampleRateIndex, bool updateStimParams);
    void changeNotchFilter(int notchFilterIndex);
    void enableHighpassFilter(bool enable);
    void highpassFilterLineEditChanged();
    void hardThresholdLineEditChanged_CA(); // update GUI MD 03-09-2021
    void refractoryLineEditChanged_CA(); // update GUI MD 07-09-2021
    void changeBandwidth();
    void changeImpedanceFrequency();
    void changePort(int port);
    void changeDacGain(int index);
    void changeDacNoiseSuppress(int index);
    void dacEnable(bool enable);
    void dacSetChannel();
    void dacSelected(int dacChannel);
    void renameChannel();
    void sortChannelsByNumber();
    void sortChannelsByName();
    void restoreOriginalChannelOrder();
    void alphabetizeChannels();
    void toggleChannelEnable();
    void enableAllChannels();
    void disableAllChannels();
    void spikeScope();
    void newSelectedChannel(SignalChannel* newChannel);
    void scanPorts();
    void loadSettings();
    void saveSettings();
    void loadStimSettings();
    void saveStimSettings();

    void loadConfigFile_CA();   //update GUI MD 13-09-2021

    void showImpedances(bool enabled);
    void saveImpedances();
    void runImpedanceMeasurement();
    void manualCableDelayControl();
    void plotPointsMode(bool enabled);
    void setSaveFormatDialog();
    void setDacThreshold1(int threshold);
    void setDacThreshold2(int threshold);
    void setDacThreshold3(int threshold);
    void setDacThreshold4(int threshold);
    void setDacThreshold5(int threshold);
    void setDacThreshold6(int threshold);
    void setDacThreshold7(int threshold);
    void setDacThreshold8(int threshold);
    void dacThresholdEnable();
    void referenceSetSelectedChannel();
    void referenceSetHardware();
    void referenceHelp();
    void stimParam();
    void setDigitalOutSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, int channel, StimParameters *parameters);
    void setAnalogOutSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, int channel, StimParameters *parameters);
    void setStimSequenceParameters(Rhs2000EvalBoard *evalBoard, double timestep_us, double currentstep_uA, int stream, int channel, StimParameters *parameters);
    void ampSettleSettings();
    void chargeRecoverySettings();
    void custom_architecture_Enable();      // update GUI - SB - 05-04-2019
    void custom_architecture_trig_Enable(); // update GUI - SB - 05-04-2019

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createLayout();

    int openInterfaceBoard(bool &expanderBoardDetected);
    void initializeInterfaceBoard();
    void findConnectedAmplifiers();
    int deviceId(Rhs2000DataBlock *dataBlock, int stream);

    void selectBaseFilename(SaveFormat format);
    void updateImpedanceFrequency();
    void setDacGainLabel(int gain);
    void setDacNoiseSuppressLabel(int noiseSuppress);
    void setDacChannelLabel(int dacChannel, QString channel, QString name);

    void writeSaveFileHeader(QDataStream &outStream, QDataStream &infoStream, SaveFormat format);
    void factorOutParallelCapacitance(double &trueMagnitude, double &impedancePhase,
                                      double frequency, double parasiticCapacitance);
    void empiricalResistanceCorrection(double &impedanceMagnitude, double &impedancePhase,
                                       double boardSampleRate);

    void setStatusBarReady();
    void setStatusBarRunning();
    void setStatusBarRecording(double bytesPerMinute, double totalElapsedRecordTimeSeconds);
    void setStatusBarWaitForTrigger();

    void setSaveFormat(SaveFormat format);
    void startNewSaveFile(SaveFormat format);
    void closeSaveFile(SaveFormat format);

    void setHighpassFilterCutoff(double cutoff);

    void setHardThreshold_CA(double th);    ////update GUI MD 07-09-2021
    void setRefractory_CA(double refractory); // update GUI MD 07-09-2021
    //void setPipeIn_CA(double th, double refractory);    //update GUI MD 09-09-2021

    void referenceSetChannel();

    void setChargeRecoveryParameters(bool mode,
                                     Rhs2000Registers::ChargeRecoveryCurrentLimit currentLimit,
                                     double targetVoltage);

    QString sampleRateText(int index);

    int sampleRateIndex;
    Rhs2000Registers::StimStepSize stimStep;
    Rhs2000Registers::ChargeRecoveryCurrentLimit chargeRecoveryCurrentLimit;
    double chargeRecoveryTargetVoltage;

    int ttlOut[16];
    int evalBoardMode;
    int numSpiPorts;
    bool expanderBoardConnected;

    bool running;
    bool recording;
    bool triggerSet;
    bool triggered;

    bool saveTtlOut;
    bool saveDcAmps;
    bool validFilename;
    bool synthMode;
    bool stimParamsHaveChanged;

    QString saveBaseFileName;
    QString saveFileName;
    QFile *saveFile;
    QDataStream *saveStream;

    QString infoFileName;
    QFile *infoFile;
    QDataStream *infoStream;

    SaveFormat saveFormat;
    int newSaveFilePeriodMinutes;

    unsigned int numUsbBlocksToRead;

    Rhs2000EvalBoard *evalBoard;
    SignalSources *signalSources;

    double cableLengthPortA;  // in meters
    double cableLengthPortB;  // in meters
    double cableLengthPortC;  // in meters
    double cableLengthPortD;  // in meters

    double desiredDspCutoffFreq;
    double actualDspCutoffFreq;
    double desiredUpperBandwidth;
    double actualUpperBandwidth;
    double desiredLowerBandwidth;
    double desiredLowerSettleBandwidth;
    double actualLowerBandwidth;
    double actualLowerSettleBandwidth;
    bool dspEnabled;
    double notchFilterFrequency;
    double notchFilterBandwidth;
    bool notchFilterEnabled;
    double highpassFilterFrequency;


    double hardThreshold_CA; // update GUI MD 03-09-2021
    double refractory_CA;   // update GUI MD 07-09-2021
    //double pipe_in[2];


    bool highpassFilterEnabled;
    double desiredImpedanceFreq;
    double actualImpedanceFreq;
    bool impedanceFreqValid;
    bool useFastSettle;
    bool headstageGlobalSettle;
    bool chargeRecoveryMode;

    int recordTriggerChannel;
    int recordTriggerPolarity;
    int recordTriggerBuffer;
    int postTriggerTime;
    bool saveTriggerChannel;

    ReferenceSource referenceSource;

    QVector<bool> manualDelayEnabled;
    QVector<int> manualDelay;

    double boardSampleRate;

    QVector<double> sampleRateList;

    QVector<SignalChannel*> dacSelectedChannel;
    QVector<bool> dacEnabled;
    QVector<int> chipId;

    queue<Rhs2000DataBlock> dataQueue;

    WavePlot *wavePlot;
    SignalProcessor *signalProcessor;

    UsbDataThread *usbDataThread;
    DataStreamFifo *usbStreamFifo;
    unsigned char* usbReadBuffer;

    SpikeScopeDialog *spikeScopeDialog;
    StimParamDialog *stimParamDialog;
    DigOutDialog *digOutDialog;
    AnOutDialog *anOutDialog;
    AmpSettleDialog *ampSettleDialog;
    ChargeRecoveryDialog *chargeRecoveryDialog;

    KeyboardShortcutDialog *keyboardShortcutDialog;
    HelpDialogChipFilters *helpDialogChipFilters;
    HelpDialogComparators *helpDialogComparators;
    HelpDialogDacs *helpDialogDacs;
    HelpDialogHighpassFilter *helpDialogHighpassFilter;
    HelpDialogNotchFilter *helpDialogNotchFilter;
    HelpDialogFastSettle *helpDialogFastSettle;
    HelpDialogReference *helpDialogReference;
    HelpDialogIOExpander *helpDialogIOExpander;

    QAction *loadSettingsAction;
    QAction *saveSettingsAction;
    QAction *loadStimSettingsAction;
    QAction *saveStimSettingsAction;
    QAction *exitAction;
    QAction *originalOrderAction;
    QAction *alphaOrderAction;
    QAction *aboutAction;
    QAction *intanWebsiteAction;
    QAction *keyboardHelpAction;
    QAction *renameChannelAction;
    QAction *toggleChannelEnableAction;
    QAction *enableAllChannelsAction;
    QAction *disableAllChannelsAction;
    QAction *copyStimParametersAction;
    QAction *pasteStimParametersAction;
    QAction *ampSettleSettingsAction;
    QAction *chargeRecoverySettingsAction;
    QAction *loadConfigFileAction_CA; //update GUI MD 13-09-2021

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *stimMenu;
    QMenu *channelMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QPushButton *runButton;
    QPushButton *recordButton;
    QPushButton *triggerButton;
    QPushButton *stopButton;
    QPushButton *baseFilenameButton;
    QPushButton *renameChannelButton;
    QPushButton *enableChannelButton;
    QPushButton *enableAllButton;
    QPushButton *disableAllButton;
    QPushButton *spikeScopeButton;
    QPushButton *changeBandwidthButton;
    QPushButton *impedanceFreqSelectButton;
    QPushButton *runImpedanceTestButton;
    QPushButton *dacSetButton;
    QPushButton *scanButton;
    QPushButton *saveImpedancesButton;
    QPushButton *setSaveFormatButton;
    QPushButton *setCableDelayButton;
    QPushButton *refSelectButton;
    QPushButton *refHardwareRefButton;
    QPushButton *stimParamButton;

    QToolButton *helpDialogChipFiltersButton;
    QToolButton *helpDialogComparatorsButton;
    QToolButton *helpDialogDacsButton;
    QToolButton *helpDialogHighpassFilterButton;
    QToolButton *helpDialogNotchFilterButton;
    QToolButton *helpDialogSettleButton;
    QToolButton *helpDialogRefButton;
    QToolButton *helpDialogIOExpanderButton;

    QCheckBox *dacEnableCheckBox;
    QCheckBox *dacLockToSelectedBox;
    QCheckBox *showImpedanceCheckBox;
    QCheckBox *plotPointsCheckBox;
    QCheckBox *highpassFilterCheckBox;
    QCheckBox *v0AxisLineCheckBox;
    QCheckBox *displayMarkerCheckBox;
    QCheckBox *displayTriggerCheckBox;
    QCheckBox *dac1ThresholdEnableCheckBox;
    QCheckBox *dac2ThresholdEnableCheckBox;
    QCheckBox *dac3ThresholdEnableCheckBox;
    QCheckBox *dac4ThresholdEnableCheckBox;
    QCheckBox *dac5ThresholdEnableCheckBox;
    QCheckBox *dac6ThresholdEnableCheckBox;
    QCheckBox *dac7ThresholdEnableCheckBox;
    QCheckBox *dac8ThresholdEnableCheckBox;
    QCheckBox *cst_en_CheckBox;     // update GUI - SB - 05-04-2019
    QCheckBox *cst_trig_CheckBox;   // update GUI - SB - 05-04-2019

    QRadioButton *displayPortAButton;
    QRadioButton *displayPortBButton;
    QRadioButton *displayPortCButton;
    QRadioButton *displayPortDButton;
    QRadioButton *displayAdcButton;
    QRadioButton *displayDacButton;
    QRadioButton *displayDigInButton;
    QRadioButton *displayDigOutButton;

    QButtonGroup *dacButtonGroup;
    QRadioButton *dacButton1;
    QRadioButton *dacButton2;
    QRadioButton *dacButton3;
    QRadioButton *dacButton4;
    QRadioButton *dacButton5;
    QRadioButton *dacButton6;
    QRadioButton *dacButton7;
    QRadioButton *dacButton8;

    QComboBox *numFramesComboBox;
    QComboBox *sampleRateComboBox;
    QComboBox *notchFilterComboBox;
    QComboBox *chargeRecoveryModeComboBox;

    QSpinBox *dac1ThresholdSpinBox;
    QSpinBox *dac2ThresholdSpinBox;
    QSpinBox *dac3ThresholdSpinBox;
    QSpinBox *dac4ThresholdSpinBox;
    QSpinBox *dac5ThresholdSpinBox;
    QSpinBox *dac6ThresholdSpinBox;
    QSpinBox *dac7ThresholdSpinBox;
    QSpinBox *dac8ThresholdSpinBox;
    QSpinBox *displayMarkerSpinBox;

    QSlider *dacGainSlider;
    QSlider *dacNoiseSuppressSlider;

    QLineEdit *saveFilenameLineEdit;
    QLineEdit *highpassFilterLineEdit;
    QLineEdit *hardThresholdLineEdit_CA;    //update GUI MD 03-09-2021
    QLineEdit *refractoryLineEdit_CA;    //update GUI MD 07-09-2021
    QLineEdit *note1LineEdit;
    QLineEdit *note2LineEdit;
    QLineEdit *note3LineEdit;

    QLabel *voltageScaleLabel;
    QLabel *ampTypeLabel;
    QLabel *statusBarLabel;
    QLabel *fifoLagLabel;
    QLabel *fifoFullLabel;
    QLabel *bufferFullLabel;
    QLabel *cpuWarningLabel;
    QLabel *dspCutoffFreqLabel;
    QLabel *upperBandwidthLabel;
    QLabel *lowerBandwidthLabel;
    QLabel *lowerSettleBandwidthLabel;
    QLabel *desiredImpedanceFreqLabel;
    QLabel *actualImpedanceFreqLabel;
    QLabel *dacGainLabel;
    QLabel *dacNoiseSuppressLabel;
    QLabel *refTypeLabel;
    QLabel *refChannelLabel;

    StimParameters copiedStimParameters;
};


#endif // MAINWINDOW_H
