/****************************************************************************
** Meta object code from reading C++ file 'isidialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Dialogs/isidialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'isidialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ISIDialog_t {
    QByteArrayData data[14];
    char stringdata0[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISIDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISIDialog_t qt_meta_stringdata_ISIDialog = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ISIDialog"
QT_MOC_LITERAL(1, 10, 15), // "updateFromState"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 20), // "changeCurrentChannel"
QT_MOC_LITERAL(4, 48, 17), // "nativeChannelName"
QT_MOC_LITERAL(5, 66, 10), // "toggleLock"
QT_MOC_LITERAL(6, 77, 13), // "setToSelected"
QT_MOC_LITERAL(7, 91, 11), // "setTimeSpan"
QT_MOC_LITERAL(8, 103, 5), // "index"
QT_MOC_LITERAL(9, 109, 10), // "setBinSize"
QT_MOC_LITERAL(10, 120, 15), // "changeYAxisMode"
QT_MOC_LITERAL(11, 136, 8), // "clearISI"
QT_MOC_LITERAL(12, 145, 10), // "configSave"
QT_MOC_LITERAL(13, 156, 8) // "saveData"

    },
    "ISIDialog\0updateFromState\0\0"
    "changeCurrentChannel\0nativeChannelName\0"
    "toggleLock\0setToSelected\0setTimeSpan\0"
    "index\0setBinSize\0changeYAxisMode\0"
    "clearISI\0configSave\0saveData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISIDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    1,   65,    2, 0x08 /* Private */,
       5,    0,   68,    2, 0x08 /* Private */,
       6,    0,   69,    2, 0x08 /* Private */,
       7,    1,   70,    2, 0x08 /* Private */,
       9,    1,   73,    2, 0x08 /* Private */,
      10,    1,   76,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    0,   80,    2, 0x08 /* Private */,
      13,    0,   81,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ISIDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ISIDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->changeCurrentChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->toggleLock(); break;
        case 3: _t->setToSelected(); break;
        case 4: _t->setTimeSpan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setBinSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->changeYAxisMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->clearISI(); break;
        case 8: _t->configSave(); break;
        case 9: _t->saveData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ISIDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ISIDialog.data,
    qt_meta_data_ISIDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ISIDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISIDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ISIDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ISIDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
struct qt_meta_stringdata_ISISaveConfigDialog_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISISaveConfigDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISISaveConfigDialog_t qt_meta_stringdata_ISISaveConfigDialog = {
    {
QT_MOC_LITERAL(0, 0, 19) // "ISISaveConfigDialog"

    },
    "ISISaveConfigDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISISaveConfigDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ISISaveConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ISISaveConfigDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ISISaveConfigDialog.data,
    qt_meta_data_ISISaveConfigDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ISISaveConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISISaveConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ISISaveConfigDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ISISaveConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
