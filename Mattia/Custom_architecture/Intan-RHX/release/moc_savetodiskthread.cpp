/****************************************************************************
** Meta object code from reading C++ file 'savetodiskthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Engine/Threads/savetodiskthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'savetodiskthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SaveToDiskThread_t {
    QByteArrayData data[13];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SaveToDiskThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SaveToDiskThread_t qt_meta_stringdata_SaveToDiskThread = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SaveToDiskThread"
QT_MOC_LITERAL(1, 17, 12), // "setStatusBar"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "setTimeLabel"
QT_MOC_LITERAL(4, 44, 14), // "sendSetCommand"
QT_MOC_LITERAL(5, 59, 5), // "error"
QT_MOC_LITERAL(6, 65, 12), // "saveLiveNote"
QT_MOC_LITERAL(7, 78, 4), // "note"
QT_MOC_LITERAL(8, 83, 19), // "setPosStimAmplitude"
QT_MOC_LITERAL(9, 103, 6), // "stream"
QT_MOC_LITERAL(10, 110, 7), // "channel"
QT_MOC_LITERAL(11, 118, 9), // "amplitude"
QT_MOC_LITERAL(12, 128, 19) // "setNegStimAmplitude"

    },
    "SaveToDiskThread\0setStatusBar\0\0"
    "setTimeLabel\0sendSetCommand\0error\0"
    "saveLiveNote\0note\0setPosStimAmplitude\0"
    "stream\0channel\0amplitude\0setNegStimAmplitude"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SaveToDiskThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    1,   52,    2, 0x06 /* Public */,
       4,    2,   55,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   63,    2, 0x0a /* Public */,
       8,    3,   66,    2, 0x0a /* Public */,
      12,    3,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,

       0        // eod
};

void SaveToDiskThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SaveToDiskThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setTimeLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendSetCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->saveLiveNote((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->setPosStimAmplitude((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->setNegStimAmplitude((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SaveToDiskThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SaveToDiskThread::setStatusBar)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SaveToDiskThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SaveToDiskThread::setTimeLabel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SaveToDiskThread::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SaveToDiskThread::sendSetCommand)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SaveToDiskThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SaveToDiskThread::error)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SaveToDiskThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_SaveToDiskThread.data,
    qt_meta_data_SaveToDiskThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SaveToDiskThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SaveToDiskThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SaveToDiskThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SaveToDiskThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SaveToDiskThread::setStatusBar(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SaveToDiskThread::setTimeLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SaveToDiskThread::sendSetCommand(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SaveToDiskThread::error(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
