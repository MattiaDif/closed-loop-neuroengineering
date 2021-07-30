/****************************************************************************
** Meta object code from reading C++ file 'controlwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Windows/controlwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControlWindow_t {
    QByteArrayData data[102];
    char stringdata0[1633];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlWindow_t qt_meta_stringdata_ControlWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ControlWindow"
QT_MOC_LITERAL(1, 14, 14), // "sendSetCommand"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "parameter"
QT_MOC_LITERAL(4, 40, 5), // "value"
QT_MOC_LITERAL(5, 46, 14), // "sendGetCommand"
QT_MOC_LITERAL(6, 61, 18), // "sendExecuteCommand"
QT_MOC_LITERAL(7, 80, 6), // "action"
QT_MOC_LITERAL(8, 87, 31), // "sendExecuteCommandWithParameter"
QT_MOC_LITERAL(9, 119, 15), // "sendNoteCommand"
QT_MOC_LITERAL(10, 135, 10), // "noteString"
QT_MOC_LITERAL(11, 146, 12), // "setTimeLabel"
QT_MOC_LITERAL(12, 159, 4), // "text"
QT_MOC_LITERAL(13, 164, 12), // "setStatusBar"
QT_MOC_LITERAL(14, 177, 25), // "setStatusBarReadyPlayback"
QT_MOC_LITERAL(15, 203, 11), // "jumpToStart"
QT_MOC_LITERAL(16, 215, 14), // "jumpToPosition"
QT_MOC_LITERAL(17, 230, 6), // "target"
QT_MOC_LITERAL(18, 237, 12), // "jumpRelative"
QT_MOC_LITERAL(19, 250, 13), // "jumpInSeconds"
QT_MOC_LITERAL(20, 264, 22), // "setDataFileReaderSpeed"
QT_MOC_LITERAL(21, 287, 13), // "playbackSpeed"
QT_MOC_LITERAL(22, 301, 15), // "updateFromState"
QT_MOC_LITERAL(23, 317, 25), // "updateForChangeHeadstages"
QT_MOC_LITERAL(24, 343, 15), // "updateTimeLabel"
QT_MOC_LITERAL(25, 359, 20), // "updateTopStatusLabel"
QT_MOC_LITERAL(26, 380, 15), // "updateStatusBar"
QT_MOC_LITERAL(27, 396, 17), // "queueErrorMessage"
QT_MOC_LITERAL(28, 414, 12), // "errorMessage"
QT_MOC_LITERAL(29, 427, 22), // "stopAndReportAnyErrors"
QT_MOC_LITERAL(30, 450, 5), // "group"
QT_MOC_LITERAL(31, 456, 7), // "ungroup"
QT_MOC_LITERAL(32, 464, 13), // "renameChannel"
QT_MOC_LITERAL(33, 478, 12), // "setReference"
QT_MOC_LITERAL(34, 491, 24), // "updateHardwareFifoStatus"
QT_MOC_LITERAL(35, 516, 11), // "percentFull"
QT_MOC_LITERAL(36, 528, 17), // "updateMainCpuLoad"
QT_MOC_LITERAL(37, 546, 7), // "percent"
QT_MOC_LITERAL(38, 554, 16), // "hideControlPanel"
QT_MOC_LITERAL(39, 571, 16), // "showControlPanel"
QT_MOC_LITERAL(40, 588, 8), // "probeMap"
QT_MOC_LITERAL(41, 597, 11), // "performance"
QT_MOC_LITERAL(42, 609, 4), // "psth"
QT_MOC_LITERAL(43, 614, 11), // "spectrogram"
QT_MOC_LITERAL(44, 626, 12), // "spikeSorting"
QT_MOC_LITERAL(45, 639, 3), // "isi"
QT_MOC_LITERAL(46, 643, 21), // "keyboardShortcutsHelp"
QT_MOC_LITERAL(47, 665, 17), // "enableLoggingSlot"
QT_MOC_LITERAL(48, 683, 6), // "enable"
QT_MOC_LITERAL(49, 690, 16), // "openIntanWebsite"
QT_MOC_LITERAL(50, 707, 5), // "about"
QT_MOC_LITERAL(51, 713, 22), // "chooseFileFormatDialog"
QT_MOC_LITERAL(52, 736, 22), // "selectBaseFilenameSlot"
QT_MOC_LITERAL(53, 759, 17), // "runControllerSlot"
QT_MOC_LITERAL(54, 777, 20), // "recordControllerSlot"
QT_MOC_LITERAL(55, 798, 29), // "triggeredRecordControllerSlot"
QT_MOC_LITERAL(56, 828, 18), // "stopControllerSlot"
QT_MOC_LITERAL(57, 847, 10), // "rewindSlot"
QT_MOC_LITERAL(58, 858, 15), // "fastForwardSlot"
QT_MOC_LITERAL(59, 874, 16), // "fastPlaybackSlot"
QT_MOC_LITERAL(60, 891, 15), // "jumpToStartSlot"
QT_MOC_LITERAL(61, 907, 16), // "jumpBack1SecSlot"
QT_MOC_LITERAL(62, 924, 17), // "jumpBack10SecSlot"
QT_MOC_LITERAL(63, 942, 18), // "jumpToPositionSlot"
QT_MOC_LITERAL(64, 961, 16), // "loadSettingsSlot"
QT_MOC_LITERAL(65, 978, 19), // "defaultSettingsSlot"
QT_MOC_LITERAL(66, 998, 16), // "saveSettingsSlot"
QT_MOC_LITERAL(67, 1015, 20), // "loadStimSettingsSlot"
QT_MOC_LITERAL(68, 1036, 20), // "saveStimSettingsSlot"
QT_MOC_LITERAL(69, 1057, 8), // "undoSlot"
QT_MOC_LITERAL(70, 1066, 8), // "redoSlot"
QT_MOC_LITERAL(71, 1075, 22), // "copyStimParametersSlot"
QT_MOC_LITERAL(72, 1098, 23), // "pasteStimParametersSlot"
QT_MOC_LITERAL(73, 1122, 21), // "ampSettleSettingsSlot"
QT_MOC_LITERAL(74, 1144, 26), // "chargeRecoverySettingsSlot"
QT_MOC_LITERAL(75, 1171, 12), // "zoomInTScale"
QT_MOC_LITERAL(76, 1184, 13), // "zoomOutTScale"
QT_MOC_LITERAL(77, 1198, 15), // "toggleRollSweep"
QT_MOC_LITERAL(78, 1214, 12), // "zoomInYScale"
QT_MOC_LITERAL(79, 1227, 13), // "zoomOutYScale"
QT_MOC_LITERAL(80, 1241, 18), // "setLabelCustomName"
QT_MOC_LITERAL(81, 1260, 18), // "setLabelNativeName"
QT_MOC_LITERAL(82, 1279, 26), // "setLabelImpedanceMagnitude"
QT_MOC_LITERAL(83, 1306, 22), // "setLabelImpedancePhase"
QT_MOC_LITERAL(84, 1329, 17), // "setLabelReference"
QT_MOC_LITERAL(85, 1347, 14), // "cycleLabelText"
QT_MOC_LITERAL(86, 1362, 21), // "changeBackgroundColor"
QT_MOC_LITERAL(87, 1384, 16), // "setShowAuxInputs"
QT_MOC_LITERAL(88, 1401, 21), // "setShowSupplyVoltages"
QT_MOC_LITERAL(89, 1423, 18), // "setSpikeThresholds"
QT_MOC_LITERAL(90, 1442, 17), // "autoColorChannels"
QT_MOC_LITERAL(91, 1460, 17), // "autoGroupChannels"
QT_MOC_LITERAL(92, 1478, 18), // "ungroupAllChannels"
QT_MOC_LITERAL(93, 1497, 20), // "restoreOrderChannels"
QT_MOC_LITERAL(94, 1518, 19), // "alphabetizeChannels"
QT_MOC_LITERAL(95, 1538, 18), // "changeUsedXPUIndex"
QT_MOC_LITERAL(96, 1557, 5), // "index"
QT_MOC_LITERAL(97, 1563, 17), // "changeAudioVolume"
QT_MOC_LITERAL(98, 1581, 6), // "volume"
QT_MOC_LITERAL(99, 1588, 20), // "changeAudioThreshold"
QT_MOC_LITERAL(100, 1609, 9), // "threshold"
QT_MOC_LITERAL(101, 1619, 13) // "remoteControl"

    },
    "ControlWindow\0sendSetCommand\0\0parameter\0"
    "value\0sendGetCommand\0sendExecuteCommand\0"
    "action\0sendExecuteCommandWithParameter\0"
    "sendNoteCommand\0noteString\0setTimeLabel\0"
    "text\0setStatusBar\0setStatusBarReadyPlayback\0"
    "jumpToStart\0jumpToPosition\0target\0"
    "jumpRelative\0jumpInSeconds\0"
    "setDataFileReaderSpeed\0playbackSpeed\0"
    "updateFromState\0updateForChangeHeadstages\0"
    "updateTimeLabel\0updateTopStatusLabel\0"
    "updateStatusBar\0queueErrorMessage\0"
    "errorMessage\0stopAndReportAnyErrors\0"
    "group\0ungroup\0renameChannel\0setReference\0"
    "updateHardwareFifoStatus\0percentFull\0"
    "updateMainCpuLoad\0percent\0hideControlPanel\0"
    "showControlPanel\0probeMap\0performance\0"
    "psth\0spectrogram\0spikeSorting\0isi\0"
    "keyboardShortcutsHelp\0enableLoggingSlot\0"
    "enable\0openIntanWebsite\0about\0"
    "chooseFileFormatDialog\0selectBaseFilenameSlot\0"
    "runControllerSlot\0recordControllerSlot\0"
    "triggeredRecordControllerSlot\0"
    "stopControllerSlot\0rewindSlot\0"
    "fastForwardSlot\0fastPlaybackSlot\0"
    "jumpToStartSlot\0jumpBack1SecSlot\0"
    "jumpBack10SecSlot\0jumpToPositionSlot\0"
    "loadSettingsSlot\0defaultSettingsSlot\0"
    "saveSettingsSlot\0loadStimSettingsSlot\0"
    "saveStimSettingsSlot\0undoSlot\0redoSlot\0"
    "copyStimParametersSlot\0pasteStimParametersSlot\0"
    "ampSettleSettingsSlot\0chargeRecoverySettingsSlot\0"
    "zoomInTScale\0zoomOutTScale\0toggleRollSweep\0"
    "zoomInYScale\0zoomOutYScale\0"
    "setLabelCustomName\0setLabelNativeName\0"
    "setLabelImpedanceMagnitude\0"
    "setLabelImpedancePhase\0setLabelReference\0"
    "cycleLabelText\0changeBackgroundColor\0"
    "setShowAuxInputs\0setShowSupplyVoltages\0"
    "setSpikeThresholds\0autoColorChannels\0"
    "autoGroupChannels\0ungroupAllChannels\0"
    "restoreOrderChannels\0alphabetizeChannels\0"
    "changeUsedXPUIndex\0index\0changeAudioVolume\0"
    "volume\0changeAudioThreshold\0threshold\0"
    "remoteControl"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      85,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  439,    2, 0x06 /* Public */,
       5,    1,  444,    2, 0x06 /* Public */,
       6,    1,  447,    2, 0x06 /* Public */,
       8,    2,  450,    2, 0x06 /* Public */,
       9,    1,  455,    2, 0x06 /* Public */,
      11,    1,  458,    2, 0x06 /* Public */,
      13,    1,  461,    2, 0x06 /* Public */,
      14,    0,  464,    2, 0x06 /* Public */,
      15,    0,  465,    2, 0x06 /* Public */,
      16,    1,  466,    2, 0x06 /* Public */,
      18,    1,  469,    2, 0x06 /* Public */,
      20,    1,  472,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    0,  475,    2, 0x0a /* Public */,
      23,    0,  476,    2, 0x0a /* Public */,
      24,    1,  477,    2, 0x0a /* Public */,
      25,    1,  480,    2, 0x0a /* Public */,
      26,    1,  483,    2, 0x0a /* Public */,
      27,    1,  486,    2, 0x0a /* Public */,
      29,    0,  489,    2, 0x0a /* Public */,
      30,    0,  490,    2, 0x0a /* Public */,
      31,    0,  491,    2, 0x0a /* Public */,
      32,    0,  492,    2, 0x0a /* Public */,
      33,    0,  493,    2, 0x0a /* Public */,
      34,    1,  494,    2, 0x0a /* Public */,
      36,    1,  497,    2, 0x0a /* Public */,
      38,    0,  500,    2, 0x0a /* Public */,
      39,    0,  501,    2, 0x08 /* Private */,
      40,    0,  502,    2, 0x08 /* Private */,
      41,    0,  503,    2, 0x08 /* Private */,
      42,    0,  504,    2, 0x08 /* Private */,
      43,    0,  505,    2, 0x08 /* Private */,
      44,    0,  506,    2, 0x08 /* Private */,
      45,    0,  507,    2, 0x08 /* Private */,
      46,    0,  508,    2, 0x08 /* Private */,
      47,    1,  509,    2, 0x08 /* Private */,
      49,    0,  512,    2, 0x08 /* Private */,
      50,    0,  513,    2, 0x08 /* Private */,
      51,    0,  514,    2, 0x08 /* Private */,
      52,    0,  515,    2, 0x08 /* Private */,
      53,    0,  516,    2, 0x08 /* Private */,
      54,    0,  517,    2, 0x08 /* Private */,
      55,    0,  518,    2, 0x08 /* Private */,
      56,    0,  519,    2, 0x08 /* Private */,
      57,    0,  520,    2, 0x08 /* Private */,
      58,    0,  521,    2, 0x08 /* Private */,
      59,    0,  522,    2, 0x08 /* Private */,
      60,    0,  523,    2, 0x08 /* Private */,
      61,    0,  524,    2, 0x08 /* Private */,
      62,    0,  525,    2, 0x08 /* Private */,
      63,    0,  526,    2, 0x08 /* Private */,
      64,    0,  527,    2, 0x08 /* Private */,
      65,    0,  528,    2, 0x08 /* Private */,
      66,    0,  529,    2, 0x08 /* Private */,
      67,    0,  530,    2, 0x08 /* Private */,
      68,    0,  531,    2, 0x08 /* Private */,
      69,    0,  532,    2, 0x08 /* Private */,
      70,    0,  533,    2, 0x08 /* Private */,
      71,    0,  534,    2, 0x08 /* Private */,
      72,    0,  535,    2, 0x08 /* Private */,
      73,    0,  536,    2, 0x08 /* Private */,
      74,    0,  537,    2, 0x08 /* Private */,
      75,    0,  538,    2, 0x08 /* Private */,
      76,    0,  539,    2, 0x08 /* Private */,
      77,    0,  540,    2, 0x08 /* Private */,
      78,    0,  541,    2, 0x08 /* Private */,
      79,    0,  542,    2, 0x08 /* Private */,
      80,    0,  543,    2, 0x08 /* Private */,
      81,    0,  544,    2, 0x08 /* Private */,
      82,    0,  545,    2, 0x08 /* Private */,
      83,    0,  546,    2, 0x08 /* Private */,
      84,    0,  547,    2, 0x08 /* Private */,
      85,    0,  548,    2, 0x08 /* Private */,
      86,    0,  549,    2, 0x08 /* Private */,
      87,    0,  550,    2, 0x08 /* Private */,
      88,    0,  551,    2, 0x08 /* Private */,
      89,    0,  552,    2, 0x08 /* Private */,
      90,    0,  553,    2, 0x08 /* Private */,
      91,    0,  554,    2, 0x08 /* Private */,
      92,    0,  555,    2, 0x08 /* Private */,
      93,    0,  556,    2, 0x08 /* Private */,
      94,    0,  557,    2, 0x08 /* Private */,
      95,    1,  558,    2, 0x08 /* Private */,
      97,    1,  561,    2, 0x08 /* Private */,
      99,    1,  564,    2, 0x08 /* Private */,
     101,    0,  567,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    3,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Double,   19,
    QMetaType::Void, QMetaType::Double,   21,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   35,
    QMetaType::Void, QMetaType::Double,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   48,
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
    QMetaType::Void, QMetaType::Int,   96,
    QMetaType::Void, QMetaType::Int,   98,
    QMetaType::Void, QMetaType::Int,  100,
    QMetaType::Void,

       0        // eod
};

void ControlWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSetCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->sendGetCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendExecuteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sendExecuteCommandWithParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->sendNoteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setTimeLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->setStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setStatusBarReadyPlayback(); break;
        case 8: _t->jumpToStart(); break;
        case 9: _t->jumpToPosition((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->jumpRelative((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->setDataFileReaderSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->updateFromState(); break;
        case 13: _t->updateForChangeHeadstages(); break;
        case 14: _t->updateTimeLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->updateTopStatusLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->updateStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->queueErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->stopAndReportAnyErrors(); break;
        case 19: _t->group(); break;
        case 20: _t->ungroup(); break;
        case 21: _t->renameChannel(); break;
        case 22: _t->setReference(); break;
        case 23: _t->updateHardwareFifoStatus((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 24: _t->updateMainCpuLoad((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 25: _t->hideControlPanel(); break;
        case 26: _t->showControlPanel(); break;
        case 27: _t->probeMap(); break;
        case 28: _t->performance(); break;
        case 29: _t->psth(); break;
        case 30: _t->spectrogram(); break;
        case 31: _t->spikeSorting(); break;
        case 32: _t->isi(); break;
        case 33: _t->keyboardShortcutsHelp(); break;
        case 34: _t->enableLoggingSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->openIntanWebsite(); break;
        case 36: _t->about(); break;
        case 37: _t->chooseFileFormatDialog(); break;
        case 38: _t->selectBaseFilenameSlot(); break;
        case 39: _t->runControllerSlot(); break;
        case 40: _t->recordControllerSlot(); break;
        case 41: _t->triggeredRecordControllerSlot(); break;
        case 42: _t->stopControllerSlot(); break;
        case 43: _t->rewindSlot(); break;
        case 44: _t->fastForwardSlot(); break;
        case 45: _t->fastPlaybackSlot(); break;
        case 46: _t->jumpToStartSlot(); break;
        case 47: _t->jumpBack1SecSlot(); break;
        case 48: _t->jumpBack10SecSlot(); break;
        case 49: _t->jumpToPositionSlot(); break;
        case 50: _t->loadSettingsSlot(); break;
        case 51: _t->defaultSettingsSlot(); break;
        case 52: _t->saveSettingsSlot(); break;
        case 53: _t->loadStimSettingsSlot(); break;
        case 54: _t->saveStimSettingsSlot(); break;
        case 55: _t->undoSlot(); break;
        case 56: _t->redoSlot(); break;
        case 57: _t->copyStimParametersSlot(); break;
        case 58: _t->pasteStimParametersSlot(); break;
        case 59: _t->ampSettleSettingsSlot(); break;
        case 60: _t->chargeRecoverySettingsSlot(); break;
        case 61: _t->zoomInTScale(); break;
        case 62: _t->zoomOutTScale(); break;
        case 63: _t->toggleRollSweep(); break;
        case 64: _t->zoomInYScale(); break;
        case 65: _t->zoomOutYScale(); break;
        case 66: _t->setLabelCustomName(); break;
        case 67: _t->setLabelNativeName(); break;
        case 68: _t->setLabelImpedanceMagnitude(); break;
        case 69: _t->setLabelImpedancePhase(); break;
        case 70: _t->setLabelReference(); break;
        case 71: _t->cycleLabelText(); break;
        case 72: _t->changeBackgroundColor(); break;
        case 73: _t->setShowAuxInputs(); break;
        case 74: _t->setShowSupplyVoltages(); break;
        case 75: _t->setSpikeThresholds(); break;
        case 76: _t->autoColorChannels(); break;
        case 77: _t->autoGroupChannels(); break;
        case 78: _t->ungroupAllChannels(); break;
        case 79: _t->restoreOrderChannels(); break;
        case 80: _t->alphabetizeChannels(); break;
        case 81: _t->changeUsedXPUIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 82: _t->changeAudioVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 83: _t->changeAudioThreshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 84: _t->remoteControl(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControlWindow::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::sendSetCommand)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::sendGetCommand)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::sendExecuteCommand)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::sendExecuteCommandWithParameter)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::sendNoteCommand)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::setTimeLabel)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::setStatusBar)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::setStatusBarReadyPlayback)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::jumpToStart)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::jumpToPosition)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::jumpRelative)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ControlWindow::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlWindow::setDataFileReaderSpeed)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControlWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_ControlWindow.data,
    qt_meta_data_ControlWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControlWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ControlWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 85)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 85;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 85)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 85;
    }
    return _id;
}

// SIGNAL 0
void ControlWindow::sendSetCommand(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlWindow::sendGetCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControlWindow::sendExecuteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControlWindow::sendExecuteCommandWithParameter(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ControlWindow::sendNoteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ControlWindow::setTimeLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ControlWindow::setStatusBar(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ControlWindow::setStatusBarReadyPlayback()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ControlWindow::jumpToStart()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ControlWindow::jumpToPosition(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ControlWindow::jumpRelative(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ControlWindow::setDataFileReaderSpeed(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
