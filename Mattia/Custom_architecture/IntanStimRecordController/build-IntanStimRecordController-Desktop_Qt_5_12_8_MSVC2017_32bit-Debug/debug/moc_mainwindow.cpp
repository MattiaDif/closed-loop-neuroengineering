/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[103];
    char stringdata0[1717];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 12), // "focusChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "QWidget*"
QT_MOC_LITERAL(4, 34, 9), // "lostFocus"
QT_MOC_LITERAL(5, 44, 11), // "gainedFocus"
QT_MOC_LITERAL(6, 56, 18), // "notifyFocusChanged"
QT_MOC_LITERAL(7, 75, 18), // "copyStimParameters"
QT_MOC_LITERAL(8, 94, 19), // "pasteStimParameters"
QT_MOC_LITERAL(9, 114, 5), // "about"
QT_MOC_LITERAL(10, 120, 21), // "keyboardShortcutsHelp"
QT_MOC_LITERAL(11, 142, 15), // "chipFiltersHelp"
QT_MOC_LITERAL(12, 158, 15), // "comparatorsHelp"
QT_MOC_LITERAL(13, 174, 8), // "dacsHelp"
QT_MOC_LITERAL(14, 183, 18), // "highpassFilterHelp"
QT_MOC_LITERAL(15, 202, 15), // "notchFilterHelp"
QT_MOC_LITERAL(16, 218, 14), // "fastSettleHelp"
QT_MOC_LITERAL(17, 233, 14), // "ioExpanderHelp"
QT_MOC_LITERAL(18, 248, 16), // "openIntanWebsite"
QT_MOC_LITERAL(19, 265, 17), // "runInterfaceBoard"
QT_MOC_LITERAL(20, 283, 20), // "recordInterfaceBoard"
QT_MOC_LITERAL(21, 304, 27), // "triggerRecordInterfaceBoard"
QT_MOC_LITERAL(22, 332, 18), // "stopInterfaceBoard"
QT_MOC_LITERAL(23, 351, 22), // "selectBaseFilenameSlot"
QT_MOC_LITERAL(24, 374, 15), // "changeNumFrames"
QT_MOC_LITERAL(25, 390, 5), // "index"
QT_MOC_LITERAL(26, 396, 12), // "changeYScale"
QT_MOC_LITERAL(27, 409, 17), // "changeYScaleDcAmp"
QT_MOC_LITERAL(28, 427, 15), // "changeYScaleAdc"
QT_MOC_LITERAL(29, 443, 13), // "changeAmpType"
QT_MOC_LITERAL(30, 457, 12), // "changeTScale"
QT_MOC_LITERAL(31, 470, 16), // "changeSampleRate"
QT_MOC_LITERAL(32, 487, 15), // "sampleRateIndex"
QT_MOC_LITERAL(33, 503, 16), // "updateStimParams"
QT_MOC_LITERAL(34, 520, 17), // "changeNotchFilter"
QT_MOC_LITERAL(35, 538, 16), // "notchFilterIndex"
QT_MOC_LITERAL(36, 555, 20), // "enableHighpassFilter"
QT_MOC_LITERAL(37, 576, 6), // "enable"
QT_MOC_LITERAL(38, 583, 29), // "highpassFilterLineEditChanged"
QT_MOC_LITERAL(39, 613, 32), // "highpassFilterLineEditChanged_CA"
QT_MOC_LITERAL(40, 646, 15), // "changeBandwidth"
QT_MOC_LITERAL(41, 662, 24), // "changeImpedanceFrequency"
QT_MOC_LITERAL(42, 687, 10), // "changePort"
QT_MOC_LITERAL(43, 698, 4), // "port"
QT_MOC_LITERAL(44, 703, 13), // "changeDacGain"
QT_MOC_LITERAL(45, 717, 22), // "changeDacNoiseSuppress"
QT_MOC_LITERAL(46, 740, 9), // "dacEnable"
QT_MOC_LITERAL(47, 750, 13), // "dacSetChannel"
QT_MOC_LITERAL(48, 764, 11), // "dacSelected"
QT_MOC_LITERAL(49, 776, 10), // "dacChannel"
QT_MOC_LITERAL(50, 787, 13), // "renameChannel"
QT_MOC_LITERAL(51, 801, 20), // "sortChannelsByNumber"
QT_MOC_LITERAL(52, 822, 18), // "sortChannelsByName"
QT_MOC_LITERAL(53, 841, 27), // "restoreOriginalChannelOrder"
QT_MOC_LITERAL(54, 869, 19), // "alphabetizeChannels"
QT_MOC_LITERAL(55, 889, 19), // "toggleChannelEnable"
QT_MOC_LITERAL(56, 909, 17), // "enableAllChannels"
QT_MOC_LITERAL(57, 927, 18), // "disableAllChannels"
QT_MOC_LITERAL(58, 946, 10), // "spikeScope"
QT_MOC_LITERAL(59, 957, 18), // "newSelectedChannel"
QT_MOC_LITERAL(60, 976, 14), // "SignalChannel*"
QT_MOC_LITERAL(61, 991, 10), // "newChannel"
QT_MOC_LITERAL(62, 1002, 9), // "scanPorts"
QT_MOC_LITERAL(63, 1012, 12), // "loadSettings"
QT_MOC_LITERAL(64, 1025, 12), // "saveSettings"
QT_MOC_LITERAL(65, 1038, 16), // "loadStimSettings"
QT_MOC_LITERAL(66, 1055, 16), // "saveStimSettings"
QT_MOC_LITERAL(67, 1072, 14), // "showImpedances"
QT_MOC_LITERAL(68, 1087, 7), // "enabled"
QT_MOC_LITERAL(69, 1095, 14), // "saveImpedances"
QT_MOC_LITERAL(70, 1110, 23), // "runImpedanceMeasurement"
QT_MOC_LITERAL(71, 1134, 23), // "manualCableDelayControl"
QT_MOC_LITERAL(72, 1158, 14), // "plotPointsMode"
QT_MOC_LITERAL(73, 1173, 19), // "setSaveFormatDialog"
QT_MOC_LITERAL(74, 1193, 16), // "setDacThreshold1"
QT_MOC_LITERAL(75, 1210, 9), // "threshold"
QT_MOC_LITERAL(76, 1220, 16), // "setDacThreshold2"
QT_MOC_LITERAL(77, 1237, 16), // "setDacThreshold3"
QT_MOC_LITERAL(78, 1254, 16), // "setDacThreshold4"
QT_MOC_LITERAL(79, 1271, 16), // "setDacThreshold5"
QT_MOC_LITERAL(80, 1288, 16), // "setDacThreshold6"
QT_MOC_LITERAL(81, 1305, 16), // "setDacThreshold7"
QT_MOC_LITERAL(82, 1322, 16), // "setDacThreshold8"
QT_MOC_LITERAL(83, 1339, 18), // "dacThresholdEnable"
QT_MOC_LITERAL(84, 1358, 27), // "referenceSetSelectedChannel"
QT_MOC_LITERAL(85, 1386, 20), // "referenceSetHardware"
QT_MOC_LITERAL(86, 1407, 13), // "referenceHelp"
QT_MOC_LITERAL(87, 1421, 9), // "stimParam"
QT_MOC_LITERAL(88, 1431, 31), // "setDigitalOutSequenceParameters"
QT_MOC_LITERAL(89, 1463, 17), // "Rhs2000EvalBoard*"
QT_MOC_LITERAL(90, 1481, 9), // "evalBoard"
QT_MOC_LITERAL(91, 1491, 11), // "timestep_us"
QT_MOC_LITERAL(92, 1503, 7), // "channel"
QT_MOC_LITERAL(93, 1511, 15), // "StimParameters*"
QT_MOC_LITERAL(94, 1527, 10), // "parameters"
QT_MOC_LITERAL(95, 1538, 30), // "setAnalogOutSequenceParameters"
QT_MOC_LITERAL(96, 1569, 25), // "setStimSequenceParameters"
QT_MOC_LITERAL(97, 1595, 14), // "currentstep_uA"
QT_MOC_LITERAL(98, 1610, 6), // "stream"
QT_MOC_LITERAL(99, 1617, 17), // "ampSettleSettings"
QT_MOC_LITERAL(100, 1635, 22), // "chargeRecoverySettings"
QT_MOC_LITERAL(101, 1658, 26), // "custom_architecture_Enable"
QT_MOC_LITERAL(102, 1685, 31) // "custom_architecture_trig_Enable"

    },
    "MainWindow\0focusChanged\0\0QWidget*\0"
    "lostFocus\0gainedFocus\0notifyFocusChanged\0"
    "copyStimParameters\0pasteStimParameters\0"
    "about\0keyboardShortcutsHelp\0chipFiltersHelp\0"
    "comparatorsHelp\0dacsHelp\0highpassFilterHelp\0"
    "notchFilterHelp\0fastSettleHelp\0"
    "ioExpanderHelp\0openIntanWebsite\0"
    "runInterfaceBoard\0recordInterfaceBoard\0"
    "triggerRecordInterfaceBoard\0"
    "stopInterfaceBoard\0selectBaseFilenameSlot\0"
    "changeNumFrames\0index\0changeYScale\0"
    "changeYScaleDcAmp\0changeYScaleAdc\0"
    "changeAmpType\0changeTScale\0changeSampleRate\0"
    "sampleRateIndex\0updateStimParams\0"
    "changeNotchFilter\0notchFilterIndex\0"
    "enableHighpassFilter\0enable\0"
    "highpassFilterLineEditChanged\0"
    "highpassFilterLineEditChanged_CA\0"
    "changeBandwidth\0changeImpedanceFrequency\0"
    "changePort\0port\0changeDacGain\0"
    "changeDacNoiseSuppress\0dacEnable\0"
    "dacSetChannel\0dacSelected\0dacChannel\0"
    "renameChannel\0sortChannelsByNumber\0"
    "sortChannelsByName\0restoreOriginalChannelOrder\0"
    "alphabetizeChannels\0toggleChannelEnable\0"
    "enableAllChannels\0disableAllChannels\0"
    "spikeScope\0newSelectedChannel\0"
    "SignalChannel*\0newChannel\0scanPorts\0"
    "loadSettings\0saveSettings\0loadStimSettings\0"
    "saveStimSettings\0showImpedances\0enabled\0"
    "saveImpedances\0runImpedanceMeasurement\0"
    "manualCableDelayControl\0plotPointsMode\0"
    "setSaveFormatDialog\0setDacThreshold1\0"
    "threshold\0setDacThreshold2\0setDacThreshold3\0"
    "setDacThreshold4\0setDacThreshold5\0"
    "setDacThreshold6\0setDacThreshold7\0"
    "setDacThreshold8\0dacThresholdEnable\0"
    "referenceSetSelectedChannel\0"
    "referenceSetHardware\0referenceHelp\0"
    "stimParam\0setDigitalOutSequenceParameters\0"
    "Rhs2000EvalBoard*\0evalBoard\0timestep_us\0"
    "channel\0StimParameters*\0parameters\0"
    "setAnalogOutSequenceParameters\0"
    "setStimSequenceParameters\0currentstep_uA\0"
    "stream\0ampSettleSettings\0"
    "chargeRecoverySettings\0"
    "custom_architecture_Enable\0"
    "custom_architecture_trig_Enable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      79,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  409,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,  414,    2, 0x0a /* Public */,
       7,    0,  419,    2, 0x0a /* Public */,
       8,    0,  420,    2, 0x0a /* Public */,
       9,    0,  421,    2, 0x08 /* Private */,
      10,    0,  422,    2, 0x08 /* Private */,
      11,    0,  423,    2, 0x08 /* Private */,
      12,    0,  424,    2, 0x08 /* Private */,
      13,    0,  425,    2, 0x08 /* Private */,
      14,    0,  426,    2, 0x08 /* Private */,
      15,    0,  427,    2, 0x08 /* Private */,
      16,    0,  428,    2, 0x08 /* Private */,
      17,    0,  429,    2, 0x08 /* Private */,
      18,    0,  430,    2, 0x08 /* Private */,
      19,    0,  431,    2, 0x08 /* Private */,
      20,    0,  432,    2, 0x08 /* Private */,
      21,    0,  433,    2, 0x08 /* Private */,
      22,    0,  434,    2, 0x08 /* Private */,
      23,    0,  435,    2, 0x08 /* Private */,
      24,    1,  436,    2, 0x08 /* Private */,
      26,    1,  439,    2, 0x08 /* Private */,
      27,    1,  442,    2, 0x08 /* Private */,
      28,    1,  445,    2, 0x08 /* Private */,
      29,    1,  448,    2, 0x08 /* Private */,
      30,    1,  451,    2, 0x08 /* Private */,
      31,    2,  454,    2, 0x08 /* Private */,
      34,    1,  459,    2, 0x08 /* Private */,
      36,    1,  462,    2, 0x08 /* Private */,
      38,    0,  465,    2, 0x08 /* Private */,
      39,    0,  466,    2, 0x08 /* Private */,
      40,    0,  467,    2, 0x08 /* Private */,
      41,    0,  468,    2, 0x08 /* Private */,
      42,    1,  469,    2, 0x08 /* Private */,
      44,    1,  472,    2, 0x08 /* Private */,
      45,    1,  475,    2, 0x08 /* Private */,
      46,    1,  478,    2, 0x08 /* Private */,
      47,    0,  481,    2, 0x08 /* Private */,
      48,    1,  482,    2, 0x08 /* Private */,
      50,    0,  485,    2, 0x08 /* Private */,
      51,    0,  486,    2, 0x08 /* Private */,
      52,    0,  487,    2, 0x08 /* Private */,
      53,    0,  488,    2, 0x08 /* Private */,
      54,    0,  489,    2, 0x08 /* Private */,
      55,    0,  490,    2, 0x08 /* Private */,
      56,    0,  491,    2, 0x08 /* Private */,
      57,    0,  492,    2, 0x08 /* Private */,
      58,    0,  493,    2, 0x08 /* Private */,
      59,    1,  494,    2, 0x08 /* Private */,
      62,    0,  497,    2, 0x08 /* Private */,
      63,    0,  498,    2, 0x08 /* Private */,
      64,    0,  499,    2, 0x08 /* Private */,
      65,    0,  500,    2, 0x08 /* Private */,
      66,    0,  501,    2, 0x08 /* Private */,
      67,    1,  502,    2, 0x08 /* Private */,
      69,    0,  505,    2, 0x08 /* Private */,
      70,    0,  506,    2, 0x08 /* Private */,
      71,    0,  507,    2, 0x08 /* Private */,
      72,    1,  508,    2, 0x08 /* Private */,
      73,    0,  511,    2, 0x08 /* Private */,
      74,    1,  512,    2, 0x08 /* Private */,
      76,    1,  515,    2, 0x08 /* Private */,
      77,    1,  518,    2, 0x08 /* Private */,
      78,    1,  521,    2, 0x08 /* Private */,
      79,    1,  524,    2, 0x08 /* Private */,
      80,    1,  527,    2, 0x08 /* Private */,
      81,    1,  530,    2, 0x08 /* Private */,
      82,    1,  533,    2, 0x08 /* Private */,
      83,    0,  536,    2, 0x08 /* Private */,
      84,    0,  537,    2, 0x08 /* Private */,
      85,    0,  538,    2, 0x08 /* Private */,
      86,    0,  539,    2, 0x08 /* Private */,
      87,    0,  540,    2, 0x08 /* Private */,
      88,    4,  541,    2, 0x08 /* Private */,
      95,    4,  550,    2, 0x08 /* Private */,
      96,    6,  559,    2, 0x08 /* Private */,
      99,    0,  572,    2, 0x08 /* Private */,
     100,    0,  573,    2, 0x08 /* Private */,
     101,    0,  574,    2, 0x08 /* Private */,
     102,    0,  575,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   32,   33,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Bool,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Bool,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 60,   61,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   68,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   68,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 89, QMetaType::Double, QMetaType::Int, 0x80000000 | 93,   90,   91,   92,   94,
    QMetaType::Void, 0x80000000 | 89, QMetaType::Double, QMetaType::Int, 0x80000000 | 93,   90,   91,   92,   94,
    QMetaType::Void, 0x80000000 | 89, QMetaType::Double, QMetaType::Double, QMetaType::Int, QMetaType::Int, 0x80000000 | 93,   90,   91,   97,   98,   92,   94,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->focusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 1: _t->notifyFocusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 2: _t->copyStimParameters(); break;
        case 3: _t->pasteStimParameters(); break;
        case 4: _t->about(); break;
        case 5: _t->keyboardShortcutsHelp(); break;
        case 6: _t->chipFiltersHelp(); break;
        case 7: _t->comparatorsHelp(); break;
        case 8: _t->dacsHelp(); break;
        case 9: _t->highpassFilterHelp(); break;
        case 10: _t->notchFilterHelp(); break;
        case 11: _t->fastSettleHelp(); break;
        case 12: _t->ioExpanderHelp(); break;
        case 13: _t->openIntanWebsite(); break;
        case 14: _t->runInterfaceBoard(); break;
        case 15: _t->recordInterfaceBoard(); break;
        case 16: _t->triggerRecordInterfaceBoard(); break;
        case 17: _t->stopInterfaceBoard(); break;
        case 18: _t->selectBaseFilenameSlot(); break;
        case 19: _t->changeNumFrames((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->changeYScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->changeYScaleDcAmp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->changeYScaleAdc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->changeAmpType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->changeTScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->changeSampleRate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 26: _t->changeNotchFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->enableHighpassFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->highpassFilterLineEditChanged(); break;
        case 29: _t->highpassFilterLineEditChanged_CA(); break;
        case 30: _t->changeBandwidth(); break;
        case 31: _t->changeImpedanceFrequency(); break;
        case 32: _t->changePort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->changeDacGain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->changeDacNoiseSuppress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->dacEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->dacSetChannel(); break;
        case 37: _t->dacSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->renameChannel(); break;
        case 39: _t->sortChannelsByNumber(); break;
        case 40: _t->sortChannelsByName(); break;
        case 41: _t->restoreOriginalChannelOrder(); break;
        case 42: _t->alphabetizeChannels(); break;
        case 43: _t->toggleChannelEnable(); break;
        case 44: _t->enableAllChannels(); break;
        case 45: _t->disableAllChannels(); break;
        case 46: _t->spikeScope(); break;
        case 47: _t->newSelectedChannel((*reinterpret_cast< SignalChannel*(*)>(_a[1]))); break;
        case 48: _t->scanPorts(); break;
        case 49: _t->loadSettings(); break;
        case 50: _t->saveSettings(); break;
        case 51: _t->loadStimSettings(); break;
        case 52: _t->saveStimSettings(); break;
        case 53: _t->showImpedances((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->saveImpedances(); break;
        case 55: _t->runImpedanceMeasurement(); break;
        case 56: _t->manualCableDelayControl(); break;
        case 57: _t->plotPointsMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 58: _t->setSaveFormatDialog(); break;
        case 59: _t->setDacThreshold1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->setDacThreshold2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->setDacThreshold3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 62: _t->setDacThreshold4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 63: _t->setDacThreshold5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 64: _t->setDacThreshold6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 65: _t->setDacThreshold7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 66: _t->setDacThreshold8((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 67: _t->dacThresholdEnable(); break;
        case 68: _t->referenceSetSelectedChannel(); break;
        case 69: _t->referenceSetHardware(); break;
        case 70: _t->referenceHelp(); break;
        case 71: _t->stimParam(); break;
        case 72: _t->setDigitalOutSequenceParameters((*reinterpret_cast< Rhs2000EvalBoard*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< StimParameters*(*)>(_a[4]))); break;
        case 73: _t->setAnalogOutSequenceParameters((*reinterpret_cast< Rhs2000EvalBoard*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< StimParameters*(*)>(_a[4]))); break;
        case 74: _t->setStimSequenceParameters((*reinterpret_cast< Rhs2000EvalBoard*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< StimParameters*(*)>(_a[6]))); break;
        case 75: _t->ampSettleSettings(); break;
        case 76: _t->chargeRecoverySettings(); break;
        case 77: _t->custom_architecture_Enable(); break;
        case 78: _t->custom_architecture_trig_Enable(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QWidget * , QWidget * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::focusChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 79)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 79;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 79)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 79;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::focusChanged(QWidget * _t1, QWidget * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE