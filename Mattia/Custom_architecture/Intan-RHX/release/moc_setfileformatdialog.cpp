/****************************************************************************
** Meta object code from reading C++ file 'setfileformatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Dialogs/setfileformatdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setfileformatdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetFileFormatDialog_t {
    QByteArrayData data[7];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetFileFormatDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetFileFormatDialog_t qt_meta_stringdata_SetFileFormatDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SetFileFormatDialog"
QT_MOC_LITERAL(1, 20, 19), // "updateOldFileFormat"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 23), // "updateLowpassSampleRate"
QT_MOC_LITERAL(4, 65, 17), // "updateSaveLowpass"
QT_MOC_LITERAL(5, 83, 16), // "updateSaveSpikes"
QT_MOC_LITERAL(6, 100, 19) // "updateSaveSnapshots"

    },
    "SetFileFormatDialog\0updateOldFileFormat\0"
    "\0updateLowpassSampleRate\0updateSaveLowpass\0"
    "updateSaveSpikes\0updateSaveSnapshots"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetFileFormatDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetFileFormatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetFileFormatDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateOldFileFormat(); break;
        case 1: _t->updateLowpassSampleRate(); break;
        case 2: _t->updateSaveLowpass(); break;
        case 3: _t->updateSaveSpikes(); break;
        case 4: _t->updateSaveSnapshots(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SetFileFormatDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SetFileFormatDialog.data,
    qt_meta_data_SetFileFormatDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetFileFormatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetFileFormatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetFileFormatDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SetFileFormatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
