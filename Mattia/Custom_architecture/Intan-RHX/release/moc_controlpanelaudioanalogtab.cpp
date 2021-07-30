/****************************************************************************
** Meta object code from reading C++ file 'controlpanelaudioanalogtab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/controlpanelaudioanalogtab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanelaudioanalogtab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControlPanelAudioAnalogTab_t {
    QByteArrayData data[22];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlPanelAudioAnalogTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlPanelAudioAnalogTab_t qt_meta_stringdata_ControlPanelAudioAnalogTab = {
    {
QT_MOC_LITERAL(0, 0, 26), // "ControlPanelAudioAnalogTab"
QT_MOC_LITERAL(1, 27, 17), // "changeAudioFilter"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 11), // "filterIndex"
QT_MOC_LITERAL(4, 58, 11), // "enableAudio"
QT_MOC_LITERAL(5, 70, 7), // "enabled"
QT_MOC_LITERAL(6, 78, 12), // "changeVolume"
QT_MOC_LITERAL(7, 91, 6), // "volume"
QT_MOC_LITERAL(8, 98, 15), // "changeThreshold"
QT_MOC_LITERAL(9, 114, 9), // "threshold"
QT_MOC_LITERAL(10, 124, 13), // "changeDacGain"
QT_MOC_LITERAL(11, 138, 5), // "index"
QT_MOC_LITERAL(12, 144, 22), // "changeDacNoiseSuppress"
QT_MOC_LITERAL(13, 167, 19), // "openDacConfigDialog"
QT_MOC_LITERAL(14, 187, 18), // "dac1LockToSelected"
QT_MOC_LITERAL(15, 206, 6), // "enable"
QT_MOC_LITERAL(16, 213, 23), // "enableDacHighpassFilter"
QT_MOC_LITERAL(17, 237, 29), // "setDacHighpassFilterFrequency"
QT_MOC_LITERAL(18, 267, 14), // "dac1SetChannel"
QT_MOC_LITERAL(19, 282, 14), // "dac2SetChannel"
QT_MOC_LITERAL(20, 297, 11), // "dac1Disable"
QT_MOC_LITERAL(21, 309, 11) // "dac2Disable"

    },
    "ControlPanelAudioAnalogTab\0changeAudioFilter\0"
    "\0filterIndex\0enableAudio\0enabled\0"
    "changeVolume\0volume\0changeThreshold\0"
    "threshold\0changeDacGain\0index\0"
    "changeDacNoiseSuppress\0openDacConfigDialog\0"
    "dac1LockToSelected\0enable\0"
    "enableDacHighpassFilter\0"
    "setDacHighpassFilterFrequency\0"
    "dac1SetChannel\0dac2SetChannel\0dac1Disable\0"
    "dac2Disable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlPanelAudioAnalogTab[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x08 /* Private */,
       4,    1,   87,    2, 0x08 /* Private */,
       6,    1,   90,    2, 0x08 /* Private */,
       8,    1,   93,    2, 0x08 /* Private */,
      10,    1,   96,    2, 0x08 /* Private */,
      12,    1,   99,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    1,  103,    2, 0x08 /* Private */,
      16,    1,  106,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,
      18,    0,  110,    2, 0x08 /* Private */,
      19,    0,  111,    2, 0x08 /* Private */,
      20,    0,  112,    2, 0x08 /* Private */,
      21,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ControlPanelAudioAnalogTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanelAudioAnalogTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeAudioFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->enableAudio((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->changeVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->changeThreshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->changeDacGain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->changeDacNoiseSuppress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->openDacConfigDialog(); break;
        case 7: _t->dac1LockToSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->enableDacHighpassFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setDacHighpassFilterFrequency(); break;
        case 10: _t->dac1SetChannel(); break;
        case 11: _t->dac2SetChannel(); break;
        case 12: _t->dac1Disable(); break;
        case 13: _t->dac2Disable(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControlPanelAudioAnalogTab::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ControlPanelAudioAnalogTab.data,
    qt_meta_data_ControlPanelAudioAnalogTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControlPanelAudioAnalogTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelAudioAnalogTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelAudioAnalogTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanelAudioAnalogTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
