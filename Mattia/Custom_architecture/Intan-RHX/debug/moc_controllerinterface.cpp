/****************************************************************************
** Meta object code from reading C++ file 'controllerinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Engine/Processing/controllerinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controllerinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControllerInterface_t {
    QByteArrayData data[22];
    char stringdata0[336];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControllerInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControllerInterface_t qt_meta_stringdata_ControllerInterface = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ControllerInterface"
QT_MOC_LITERAL(1, 20, 12), // "setTimeLabel"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "text"
QT_MOC_LITERAL(4, 39, 17), // "setTopStatusLabel"
QT_MOC_LITERAL(5, 57, 11), // "haveStopped"
QT_MOC_LITERAL(6, 69, 21), // "setHardwareFifoStatus"
QT_MOC_LITERAL(7, 91, 11), // "percentFull"
QT_MOC_LITERAL(8, 103, 14), // "cpuLoadPercent"
QT_MOC_LITERAL(9, 118, 7), // "percent"
QT_MOC_LITERAL(10, 126, 15), // "TCPErrorMessage"
QT_MOC_LITERAL(11, 142, 12), // "errorMessage"
QT_MOC_LITERAL(12, 155, 15), // "updateFromState"
QT_MOC_LITERAL(13, 171, 25), // "updateCurrentAudioChannel"
QT_MOC_LITERAL(14, 197, 4), // "name"
QT_MOC_LITERAL(15, 202, 19), // "manualStimTriggerOn"
QT_MOC_LITERAL(16, 222, 7), // "keyName"
QT_MOC_LITERAL(17, 230, 20), // "manualStimTriggerOff"
QT_MOC_LITERAL(18, 251, 22), // "manualStimTriggerPulse"
QT_MOC_LITERAL(19, 274, 18), // "updateHardwareFifo"
QT_MOC_LITERAL(20, 293, 30), // "updateWaveformProcessorCpuLoad"
QT_MOC_LITERAL(21, 324, 11) // "percentLoad"

    },
    "ControllerInterface\0setTimeLabel\0\0"
    "text\0setTopStatusLabel\0haveStopped\0"
    "setHardwareFifoStatus\0percentFull\0"
    "cpuLoadPercent\0percent\0TCPErrorMessage\0"
    "errorMessage\0updateFromState\0"
    "updateCurrentAudioChannel\0name\0"
    "manualStimTriggerOn\0keyName\0"
    "manualStimTriggerOff\0manualStimTriggerPulse\0"
    "updateHardwareFifo\0updateWaveformProcessorCpuLoad\0"
    "percentLoad"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControllerInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    0,   85,    2, 0x06 /* Public */,
       6,    1,   86,    2, 0x06 /* Public */,
       8,    1,   89,    2, 0x06 /* Public */,
      10,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   95,    2, 0x0a /* Public */,
      13,    1,   96,    2, 0x0a /* Public */,
      15,    1,   99,    2, 0x0a /* Public */,
      17,    1,  102,    2, 0x0a /* Public */,
      18,    1,  105,    2, 0x0a /* Public */,
      19,    1,  108,    2, 0x08 /* Private */,
      20,    1,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,   21,

       0        // eod
};

void ControllerInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControllerInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setTimeLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setTopStatusLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->haveStopped(); break;
        case 3: _t->setHardwareFifoStatus((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->cpuLoadPercent((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->TCPErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updateFromState(); break;
        case 7: _t->updateCurrentAudioChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->manualStimTriggerOn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->manualStimTriggerOff((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->manualStimTriggerPulse((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->updateHardwareFifo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->updateWaveformProcessorCpuLoad((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControllerInterface::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::setTimeLabel)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControllerInterface::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::setTopStatusLabel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ControllerInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::haveStopped)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ControllerInterface::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::setHardwareFifoStatus)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ControllerInterface::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::cpuLoadPercent)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ControllerInterface::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControllerInterface::TCPErrorMessage)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControllerInterface::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ControllerInterface.data,
    qt_meta_data_ControllerInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControllerInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControllerInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ControllerInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ControllerInterface::setTimeLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControllerInterface::setTopStatusLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControllerInterface::haveStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ControllerInterface::setHardwareFifoStatus(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ControllerInterface::cpuLoadPercent(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ControllerInterface::TCPErrorMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
