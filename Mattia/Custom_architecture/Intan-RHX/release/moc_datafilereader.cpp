/****************************************************************************
** Meta object code from reading C++ file 'datafilereader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Engine/Processing/DataFileReaders/datafilereader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datafilereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataFileReader_t {
    QByteArrayData data[19];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataFileReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataFileReader_t qt_meta_stringdata_DataFileReader = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DataFileReader"
QT_MOC_LITERAL(1, 15, 19), // "setPosStimAmplitude"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 6), // "stream"
QT_MOC_LITERAL(4, 43, 7), // "channel"
QT_MOC_LITERAL(5, 51, 9), // "amplitude"
QT_MOC_LITERAL(6, 61, 19), // "setNegStimAmplitude"
QT_MOC_LITERAL(7, 81, 12), // "setStatusBar"
QT_MOC_LITERAL(8, 94, 12), // "setTimeLabel"
QT_MOC_LITERAL(9, 107, 14), // "sendSetCommand"
QT_MOC_LITERAL(10, 122, 11), // "jumpToStart"
QT_MOC_LITERAL(11, 134, 14), // "jumpToPosition"
QT_MOC_LITERAL(12, 149, 10), // "targetTime"
QT_MOC_LITERAL(13, 160, 12), // "jumpRelative"
QT_MOC_LITERAL(14, 173, 13), // "jumpInSeconds"
QT_MOC_LITERAL(15, 187, 17), // "setStatusBarReady"
QT_MOC_LITERAL(16, 205, 15), // "setStatusBarEOF"
QT_MOC_LITERAL(17, 221, 16), // "setPlaybackSpeed"
QT_MOC_LITERAL(18, 238, 14) // "playbackSpeed_"

    },
    "DataFileReader\0setPosStimAmplitude\0\0"
    "stream\0channel\0amplitude\0setNegStimAmplitude\0"
    "setStatusBar\0setTimeLabel\0sendSetCommand\0"
    "jumpToStart\0jumpToPosition\0targetTime\0"
    "jumpRelative\0jumpInSeconds\0setStatusBarReady\0"
    "setStatusBarEOF\0setPlaybackSpeed\0"
    "playbackSpeed_"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataFileReader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   69,    2, 0x06 /* Public */,
       6,    3,   76,    2, 0x06 /* Public */,
       7,    1,   83,    2, 0x06 /* Public */,
       8,    1,   86,    2, 0x06 /* Public */,
       9,    2,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   94,    2, 0x0a /* Public */,
      11,    1,   95,    2, 0x0a /* Public */,
      13,    1,   98,    2, 0x0a /* Public */,
      15,    0,  101,    2, 0x0a /* Public */,
      16,    0,  102,    2, 0x0a /* Public */,
      17,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   18,

       0        // eod
};

void DataFileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataFileReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setPosStimAmplitude((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setNegStimAmplitude((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->setStatusBar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setTimeLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sendSetCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->jumpToStart(); break;
        case 6: _t->jumpToPosition((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->jumpRelative((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setStatusBarReady(); break;
        case 9: _t->setStatusBarEOF(); break;
        case 10: _t->setPlaybackSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataFileReader::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataFileReader::setPosStimAmplitude)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataFileReader::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataFileReader::setNegStimAmplitude)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataFileReader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataFileReader::setStatusBar)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataFileReader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataFileReader::setTimeLabel)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataFileReader::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataFileReader::sendSetCommand)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataFileReader::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DataFileReader.data,
    qt_meta_data_DataFileReader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataFileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataFileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataFileReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataFileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DataFileReader::setPosStimAmplitude(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataFileReader::setNegStimAmplitude(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataFileReader::setStatusBar(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataFileReader::setTimeLabel(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataFileReader::sendSetCommand(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
