/****************************************************************************
** Meta object code from reading C++ file 'commandparser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Engine/Processing/commandparser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandparser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CommandParser_t {
    QByteArrayData data[27];
    char stringdata0[417];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandParser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandParser_t qt_meta_stringdata_CommandParser = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CommandParser"
QT_MOC_LITERAL(1, 14, 18), // "updateGUIFromState"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 15), // "TCPReturnSignal"
QT_MOC_LITERAL(4, 50, 6), // "result"
QT_MOC_LITERAL(5, 57, 14), // "TCPErrorSignal"
QT_MOC_LITERAL(6, 72, 5), // "error"
QT_MOC_LITERAL(7, 78, 17), // "sEndOfLineiveNote"
QT_MOC_LITERAL(8, 96, 4), // "note"
QT_MOC_LITERAL(9, 101, 13), // "stimTriggerOn"
QT_MOC_LITERAL(10, 115, 7), // "keyName"
QT_MOC_LITERAL(11, 123, 14), // "stimTriggerOff"
QT_MOC_LITERAL(12, 138, 16), // "stimTriggerPulse"
QT_MOC_LITERAL(13, 155, 28), // "connectTCPWaveformDataOutput"
QT_MOC_LITERAL(14, 184, 25), // "connectTCPSpikeDataOutput"
QT_MOC_LITERAL(15, 210, 31), // "disconnectTCPWaveformDataOutput"
QT_MOC_LITERAL(16, 242, 28), // "disconnectTCPSpikeDataOutput"
QT_MOC_LITERAL(17, 271, 14), // "setCommandSlot"
QT_MOC_LITERAL(18, 286, 9), // "parameter"
QT_MOC_LITERAL(19, 296, 5), // "value"
QT_MOC_LITERAL(20, 302, 14), // "getCommandSlot"
QT_MOC_LITERAL(21, 317, 18), // "executeCommandSlot"
QT_MOC_LITERAL(22, 336, 6), // "action"
QT_MOC_LITERAL(23, 343, 31), // "executeCommandWithParameterSlot"
QT_MOC_LITERAL(24, 375, 15), // "noteCommandSlot"
QT_MOC_LITERAL(25, 391, 12), // "TCPErrorSlot"
QT_MOC_LITERAL(26, 404, 12) // "errorMessage"

    },
    "CommandParser\0updateGUIFromState\0\0"
    "TCPReturnSignal\0result\0TCPErrorSignal\0"
    "error\0sEndOfLineiveNote\0note\0stimTriggerOn\0"
    "keyName\0stimTriggerOff\0stimTriggerPulse\0"
    "connectTCPWaveformDataOutput\0"
    "connectTCPSpikeDataOutput\0"
    "disconnectTCPWaveformDataOutput\0"
    "disconnectTCPSpikeDataOutput\0"
    "setCommandSlot\0parameter\0value\0"
    "getCommandSlot\0executeCommandSlot\0"
    "action\0executeCommandWithParameterSlot\0"
    "noteCommandSlot\0TCPErrorSlot\0errorMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandParser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    1,  100,    2, 0x06 /* Public */,
       5,    1,  103,    2, 0x06 /* Public */,
       7,    1,  106,    2, 0x06 /* Public */,
       9,    1,  109,    2, 0x06 /* Public */,
      11,    1,  112,    2, 0x06 /* Public */,
      12,    1,  115,    2, 0x06 /* Public */,
      13,    0,  118,    2, 0x06 /* Public */,
      14,    0,  119,    2, 0x06 /* Public */,
      15,    0,  120,    2, 0x06 /* Public */,
      16,    0,  121,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    2,  122,    2, 0x0a /* Public */,
      20,    1,  127,    2, 0x0a /* Public */,
      21,    1,  130,    2, 0x0a /* Public */,
      23,    2,  133,    2, 0x0a /* Public */,
      24,    1,  138,    2, 0x0a /* Public */,
      25,    1,  141,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   18,   19,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   22,   18,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   26,

       0        // eod
};

void CommandParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CommandParser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateGUIFromState(); break;
        case 1: _t->TCPReturnSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->TCPErrorSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sEndOfLineiveNote((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->stimTriggerOn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->stimTriggerOff((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->stimTriggerPulse((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->connectTCPWaveformDataOutput(); break;
        case 8: _t->connectTCPSpikeDataOutput(); break;
        case 9: _t->disconnectTCPWaveformDataOutput(); break;
        case 10: _t->disconnectTCPSpikeDataOutput(); break;
        case 11: _t->setCommandSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 12: _t->getCommandSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->executeCommandSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->executeCommandWithParameterSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 15: _t->noteCommandSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->TCPErrorSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CommandParser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::updateGUIFromState)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::TCPReturnSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::TCPErrorSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::sEndOfLineiveNote)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::stimTriggerOn)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::stimTriggerOff)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::stimTriggerPulse)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::connectTCPWaveformDataOutput)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::connectTCPSpikeDataOutput)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::disconnectTCPWaveformDataOutput)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (CommandParser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandParser::disconnectTCPSpikeDataOutput)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CommandParser::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CommandParser.data,
    qt_meta_data_CommandParser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CommandParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandParser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommandParser.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommandParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void CommandParser::updateGUIFromState()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CommandParser::TCPReturnSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommandParser::TCPErrorSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommandParser::sEndOfLineiveNote(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CommandParser::stimTriggerOn(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CommandParser::stimTriggerOff(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CommandParser::stimTriggerPulse(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CommandParser::connectTCPWaveformDataOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CommandParser::connectTCPSpikeDataOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CommandParser::disconnectTCPWaveformDataOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void CommandParser::disconnectTCPSpikeDataOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
