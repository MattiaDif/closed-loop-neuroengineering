/****************************************************************************
** Meta object code from reading C++ file 'triggerrecorddialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../triggerrecorddialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'triggerrecorddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TriggerRecordDialog_t {
    QByteArrayData data[8];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TriggerRecordDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TriggerRecordDialog_t qt_meta_stringdata_TriggerRecordDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "TriggerRecordDialog"
QT_MOC_LITERAL(1, 20, 15), // "setDigitalInput"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "index"
QT_MOC_LITERAL(4, 43, 18), // "setTriggerPolarity"
QT_MOC_LITERAL(5, 62, 19), // "recordBufferSeconds"
QT_MOC_LITERAL(6, 82, 5), // "value"
QT_MOC_LITERAL(7, 88, 18) // "postTriggerSeconds"

    },
    "TriggerRecordDialog\0setDigitalInput\0"
    "\0index\0setTriggerPolarity\0recordBufferSeconds\0"
    "value\0postTriggerSeconds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TriggerRecordDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       5,    1,   40,    2, 0x08 /* Private */,
       7,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void TriggerRecordDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TriggerRecordDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDigitalInput((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setTriggerPolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->recordBufferSeconds((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->postTriggerSeconds((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TriggerRecordDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_TriggerRecordDialog.data,
    qt_meta_data_TriggerRecordDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TriggerRecordDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TriggerRecordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TriggerRecordDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TriggerRecordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
