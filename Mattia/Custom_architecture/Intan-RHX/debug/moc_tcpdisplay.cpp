/****************************************************************************
** Meta object code from reading C++ file 'tcpdisplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/tcpdisplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpdisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TCPDisplay_t {
    QByteArrayData data[41];
    char stringdata0[736];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPDisplay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPDisplay_t qt_meta_stringdata_TCPDisplay = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TCPDisplay"
QT_MOC_LITERAL(1, 11, 14), // "sendSetCommand"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "parameter"
QT_MOC_LITERAL(4, 37, 5), // "value"
QT_MOC_LITERAL(5, 43, 14), // "sendGetCommand"
QT_MOC_LITERAL(6, 58, 18), // "sendExecuteCommand"
QT_MOC_LITERAL(7, 77, 6), // "action"
QT_MOC_LITERAL(8, 84, 31), // "sendExecuteCommandWithParameter"
QT_MOC_LITERAL(9, 116, 15), // "sendNoteCommand"
QT_MOC_LITERAL(10, 132, 4), // "note"
QT_MOC_LITERAL(11, 137, 27), // "establishWaveformConnection"
QT_MOC_LITERAL(12, 165, 24), // "establishSpikeConnection"
QT_MOC_LITERAL(13, 190, 20), // "updateCommandWidgets"
QT_MOC_LITERAL(14, 211, 24), // "waveformOutputHostEdited"
QT_MOC_LITERAL(15, 236, 25), // "waveformOutputPortChanged"
QT_MOC_LITERAL(16, 262, 21), // "spikeOutputHostEdited"
QT_MOC_LITERAL(17, 284, 22), // "spikeOutputPortChanged"
QT_MOC_LITERAL(18, 307, 13), // "clearCommands"
QT_MOC_LITERAL(19, 321, 11), // "clearErrors"
QT_MOC_LITERAL(20, 333, 21), // "selectPresentChannels"
QT_MOC_LITERAL(21, 355, 22), // "selectChannelsToStream"
QT_MOC_LITERAL(22, 378, 11), // "addChannels"
QT_MOC_LITERAL(23, 390, 14), // "removeChannels"
QT_MOC_LITERAL(24, 405, 14), // "addAllChannels"
QT_MOC_LITERAL(25, 420, 17), // "removeAllChannels"
QT_MOC_LITERAL(26, 438, 23), // "updateDataOutputWidgets"
QT_MOC_LITERAL(27, 462, 27), // "processNewCommandConnection"
QT_MOC_LITERAL(28, 490, 34), // "processNewWaveformOutputConne..."
QT_MOC_LITERAL(29, 525, 31), // "processNewSpikeOutputConnection"
QT_MOC_LITERAL(30, 557, 15), // "commandsConnect"
QT_MOC_LITERAL(31, 573, 21), // "waveformOutputConnect"
QT_MOC_LITERAL(32, 595, 18), // "spikeOutputConnect"
QT_MOC_LITERAL(33, 614, 18), // "commandsDisconnect"
QT_MOC_LITERAL(34, 633, 24), // "waveformOutputDisconnect"
QT_MOC_LITERAL(35, 658, 21), // "spikeOutputDisconnect"
QT_MOC_LITERAL(36, 680, 17), // "readClientCommand"
QT_MOC_LITERAL(37, 698, 9), // "TCPReturn"
QT_MOC_LITERAL(38, 708, 6), // "result"
QT_MOC_LITERAL(39, 715, 8), // "TCPError"
QT_MOC_LITERAL(40, 724, 11) // "errorString"

    },
    "TCPDisplay\0sendSetCommand\0\0parameter\0"
    "value\0sendGetCommand\0sendExecuteCommand\0"
    "action\0sendExecuteCommandWithParameter\0"
    "sendNoteCommand\0note\0establishWaveformConnection\0"
    "establishSpikeConnection\0updateCommandWidgets\0"
    "waveformOutputHostEdited\0"
    "waveformOutputPortChanged\0"
    "spikeOutputHostEdited\0spikeOutputPortChanged\0"
    "clearCommands\0clearErrors\0"
    "selectPresentChannels\0selectChannelsToStream\0"
    "addChannels\0removeChannels\0addAllChannels\0"
    "removeAllChannels\0updateDataOutputWidgets\0"
    "processNewCommandConnection\0"
    "processNewWaveformOutputConnection\0"
    "processNewSpikeOutputConnection\0"
    "commandsConnect\0waveformOutputConnect\0"
    "spikeOutputConnect\0commandsDisconnect\0"
    "waveformOutputDisconnect\0spikeOutputDisconnect\0"
    "readClientCommand\0TCPReturn\0result\0"
    "TCPError\0errorString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPDisplay[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  179,    2, 0x06 /* Public */,
       5,    1,  184,    2, 0x06 /* Public */,
       6,    1,  187,    2, 0x06 /* Public */,
       8,    2,  190,    2, 0x06 /* Public */,
       9,    1,  195,    2, 0x06 /* Public */,
      11,    0,  198,    2, 0x06 /* Public */,
      12,    0,  199,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  200,    2, 0x08 /* Private */,
      14,    0,  201,    2, 0x08 /* Private */,
      15,    0,  202,    2, 0x08 /* Private */,
      16,    0,  203,    2, 0x08 /* Private */,
      17,    0,  204,    2, 0x08 /* Private */,
      18,    0,  205,    2, 0x08 /* Private */,
      19,    0,  206,    2, 0x08 /* Private */,
      20,    0,  207,    2, 0x08 /* Private */,
      21,    0,  208,    2, 0x08 /* Private */,
      22,    0,  209,    2, 0x08 /* Private */,
      23,    0,  210,    2, 0x08 /* Private */,
      24,    0,  211,    2, 0x08 /* Private */,
      25,    0,  212,    2, 0x08 /* Private */,
      26,    0,  213,    2, 0x08 /* Private */,
      27,    0,  214,    2, 0x0a /* Public */,
      28,    0,  215,    2, 0x0a /* Public */,
      29,    0,  216,    2, 0x0a /* Public */,
      30,    0,  217,    2, 0x0a /* Public */,
      31,    0,  218,    2, 0x0a /* Public */,
      32,    0,  219,    2, 0x0a /* Public */,
      33,    0,  220,    2, 0x0a /* Public */,
      34,    0,  221,    2, 0x0a /* Public */,
      35,    0,  222,    2, 0x0a /* Public */,
      36,    0,  223,    2, 0x0a /* Public */,
      37,    1,  224,    2, 0x0a /* Public */,
      39,    1,  227,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    3,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
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
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   40,

       0        // eod
};

void TCPDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TCPDisplay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSetCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->sendGetCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendExecuteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sendExecuteCommandWithParameter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->sendNoteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->establishWaveformConnection(); break;
        case 6: _t->establishSpikeConnection(); break;
        case 7: _t->updateCommandWidgets(); break;
        case 8: _t->waveformOutputHostEdited(); break;
        case 9: _t->waveformOutputPortChanged(); break;
        case 10: _t->spikeOutputHostEdited(); break;
        case 11: _t->spikeOutputPortChanged(); break;
        case 12: _t->clearCommands(); break;
        case 13: _t->clearErrors(); break;
        case 14: _t->selectPresentChannels(); break;
        case 15: _t->selectChannelsToStream(); break;
        case 16: _t->addChannels(); break;
        case 17: _t->removeChannels(); break;
        case 18: _t->addAllChannels(); break;
        case 19: _t->removeAllChannels(); break;
        case 20: _t->updateDataOutputWidgets(); break;
        case 21: _t->processNewCommandConnection(); break;
        case 22: _t->processNewWaveformOutputConnection(); break;
        case 23: _t->processNewSpikeOutputConnection(); break;
        case 24: _t->commandsConnect(); break;
        case 25: _t->waveformOutputConnect(); break;
        case 26: _t->spikeOutputConnect(); break;
        case 27: _t->commandsDisconnect(); break;
        case 28: _t->waveformOutputDisconnect(); break;
        case 29: _t->spikeOutputDisconnect(); break;
        case 30: _t->readClientCommand(); break;
        case 31: _t->TCPReturn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->TCPError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TCPDisplay::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::sendSetCommand)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::sendGetCommand)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::sendExecuteCommand)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::sendExecuteCommandWithParameter)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::sendNoteCommand)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::establishWaveformConnection)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TCPDisplay::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPDisplay::establishSpikeConnection)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TCPDisplay::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TCPDisplay.data,
    qt_meta_data_TCPDisplay,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TCPDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TCPDisplay.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TCPDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void TCPDisplay::sendSetCommand(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCPDisplay::sendGetCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TCPDisplay::sendExecuteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TCPDisplay::sendExecuteCommandWithParameter(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TCPDisplay::sendNoteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TCPDisplay::establishWaveformConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TCPDisplay::establishSpikeConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
