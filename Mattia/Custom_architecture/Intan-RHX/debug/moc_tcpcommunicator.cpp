/****************************************************************************
** Meta object code from reading C++ file 'tcpcommunicator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Engine/Processing/tcpcommunicator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpcommunicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TCPCommunicator_t {
    QByteArrayData data[14];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPCommunicator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPCommunicator_t qt_meta_stringdata_TCPCommunicator = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TCPCommunicator"
QT_MOC_LITERAL(1, 16, 13), // "newConnection"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 9), // "readyRead"
QT_MOC_LITERAL(4, 41, 13), // "statusChanged"
QT_MOC_LITERAL(5, 55, 20), // "attemptNewConnection"
QT_MOC_LITERAL(6, 76, 20), // "returnToDisconnected"
QT_MOC_LITERAL(7, 97, 19), // "establishConnection"
QT_MOC_LITERAL(8, 117, 13), // "TCPDataOutput"
QT_MOC_LITERAL(9, 131, 11), // "QByteArray*"
QT_MOC_LITERAL(10, 143, 5), // "array"
QT_MOC_LITERAL(11, 149, 3), // "len"
QT_MOC_LITERAL(12, 153, 17), // "emitNewConnection"
QT_MOC_LITERAL(13, 171, 13) // "emitReadyRead"

    },
    "TCPCommunicator\0newConnection\0\0readyRead\0"
    "statusChanged\0attemptNewConnection\0"
    "returnToDisconnected\0establishConnection\0"
    "TCPDataOutput\0QByteArray*\0array\0len\0"
    "emitNewConnection\0emitReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPCommunicator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   62,    2, 0x0a /* Public */,
       6,    0,   63,    2, 0x0a /* Public */,
       7,    0,   64,    2, 0x0a /* Public */,
       8,    2,   65,    2, 0x0a /* Public */,
      12,    0,   70,    2, 0x08 /* Private */,
      13,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::LongLong,   10,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TCPCommunicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TCPCommunicator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newConnection(); break;
        case 1: _t->readyRead(); break;
        case 2: _t->statusChanged(); break;
        case 3: _t->attemptNewConnection(); break;
        case 4: _t->returnToDisconnected(); break;
        case 5: _t->establishConnection(); break;
        case 6: _t->TCPDataOutput((*reinterpret_cast< QByteArray*(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 7: _t->emitNewConnection(); break;
        case 8: _t->emitReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TCPCommunicator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPCommunicator::newConnection)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TCPCommunicator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPCommunicator::readyRead)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TCPCommunicator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPCommunicator::statusChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TCPCommunicator::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TCPCommunicator.data,
    qt_meta_data_TCPCommunicator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TCPCommunicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPCommunicator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TCPCommunicator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TCPCommunicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TCPCommunicator::newConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TCPCommunicator::readyRead()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TCPCommunicator::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
