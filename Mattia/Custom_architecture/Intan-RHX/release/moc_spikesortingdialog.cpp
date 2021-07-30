/****************************************************************************
** Meta object code from reading C++ file 'spikesortingdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Dialogs/spikesortingdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spikesortingdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SpikeSortingDialog_t {
    QByteArrayData data[22];
    char stringdata0[357];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpikeSortingDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpikeSortingDialog_t qt_meta_stringdata_SpikeSortingDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SpikeSortingDialog"
QT_MOC_LITERAL(1, 19, 15), // "updateFromState"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 19), // "setVoltageThreshold"
QT_MOC_LITERAL(4, 56, 9), // "threshold"
QT_MOC_LITERAL(5, 66, 15), // "setVoltageScale"
QT_MOC_LITERAL(6, 82, 5), // "index"
QT_MOC_LITERAL(7, 88, 12), // "setTimeScale"
QT_MOC_LITERAL(8, 101, 21), // "setNumSpikesDisplayed"
QT_MOC_LITERAL(9, 123, 26), // "loadSpikeSortingParameters"
QT_MOC_LITERAL(10, 150, 26), // "saveSpikeSortingParameters"
QT_MOC_LITERAL(11, 177, 10), // "clearScope"
QT_MOC_LITERAL(12, 188, 12), // "takeSnapshot"
QT_MOC_LITERAL(13, 201, 13), // "clearSnapshot"
QT_MOC_LITERAL(14, 215, 20), // "changeCurrentChannel"
QT_MOC_LITERAL(15, 236, 17), // "nativeChannelName"
QT_MOC_LITERAL(16, 254, 10), // "toggleLock"
QT_MOC_LITERAL(17, 265, 13), // "setToSelected"
QT_MOC_LITERAL(18, 279, 24), // "toggleSuppressionEnabled"
QT_MOC_LITERAL(19, 304, 7), // "enabled"
QT_MOC_LITERAL(20, 312, 20), // "toggleArtifactsShown"
QT_MOC_LITERAL(21, 333, 23) // "setSuppressionThreshold"

    },
    "SpikeSortingDialog\0updateFromState\0\0"
    "setVoltageThreshold\0threshold\0"
    "setVoltageScale\0index\0setTimeScale\0"
    "setNumSpikesDisplayed\0loadSpikeSortingParameters\0"
    "saveSpikeSortingParameters\0clearScope\0"
    "takeSnapshot\0clearSnapshot\0"
    "changeCurrentChannel\0nativeChannelName\0"
    "toggleLock\0setToSelected\0"
    "toggleSuppressionEnabled\0enabled\0"
    "toggleArtifactsShown\0setSuppressionThreshold"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpikeSortingDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    1,   95,    2, 0x08 /* Private */,
       5,    1,   98,    2, 0x08 /* Private */,
       7,    1,  101,    2, 0x08 /* Private */,
       8,    1,  104,    2, 0x08 /* Private */,
       9,    0,  107,    2, 0x08 /* Private */,
      10,    0,  108,    2, 0x08 /* Private */,
      11,    0,  109,    2, 0x08 /* Private */,
      12,    0,  110,    2, 0x08 /* Private */,
      13,    0,  111,    2, 0x08 /* Private */,
      14,    1,  112,    2, 0x08 /* Private */,
      16,    0,  115,    2, 0x08 /* Private */,
      17,    0,  116,    2, 0x08 /* Private */,
      18,    1,  117,    2, 0x08 /* Private */,
      20,    1,  120,    2, 0x08 /* Private */,
      21,    0,  123,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,

       0        // eod
};

void SpikeSortingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SpikeSortingDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->setVoltageThreshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setVoltageScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setTimeScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setNumSpikesDisplayed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->loadSpikeSortingParameters(); break;
        case 6: _t->saveSpikeSortingParameters(); break;
        case 7: _t->clearScope(); break;
        case 8: _t->takeSnapshot(); break;
        case 9: _t->clearSnapshot(); break;
        case 10: _t->changeCurrentChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->toggleLock(); break;
        case 12: _t->setToSelected(); break;
        case 13: _t->toggleSuppressionEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->toggleArtifactsShown((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setSuppressionThreshold(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SpikeSortingDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SpikeSortingDialog.data,
    qt_meta_data_SpikeSortingDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SpikeSortingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpikeSortingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SpikeSortingDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SpikeSortingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
