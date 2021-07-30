/****************************************************************************
** Meta object code from reading C++ file 'psthdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Dialogs/psthdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'psthdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PSTHDialog_t {
    QByteArrayData data[18];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PSTHDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PSTHDialog_t qt_meta_stringdata_PSTHDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PSTHDialog"
QT_MOC_LITERAL(1, 11, 15), // "updateFromState"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 20), // "changeCurrentChannel"
QT_MOC_LITERAL(4, 49, 17), // "nativeChannelName"
QT_MOC_LITERAL(5, 67, 10), // "toggleLock"
QT_MOC_LITERAL(6, 78, 13), // "setToSelected"
QT_MOC_LITERAL(7, 92, 17), // "setPreTriggerSpan"
QT_MOC_LITERAL(8, 110, 5), // "index"
QT_MOC_LITERAL(9, 116, 18), // "setPostTriggerSpan"
QT_MOC_LITERAL(10, 135, 10), // "setBinSize"
QT_MOC_LITERAL(11, 146, 15), // "setMaxNumTrials"
QT_MOC_LITERAL(12, 162, 14), // "clearLastTrial"
QT_MOC_LITERAL(13, 177, 14), // "clearAllTrials"
QT_MOC_LITERAL(14, 192, 17), // "setDigitalTrigger"
QT_MOC_LITERAL(15, 210, 18), // "setTriggerPolarity"
QT_MOC_LITERAL(16, 229, 10), // "configSave"
QT_MOC_LITERAL(17, 240, 8) // "saveData"

    },
    "PSTHDialog\0updateFromState\0\0"
    "changeCurrentChannel\0nativeChannelName\0"
    "toggleLock\0setToSelected\0setPreTriggerSpan\0"
    "index\0setPostTriggerSpan\0setBinSize\0"
    "setMaxNumTrials\0clearLastTrial\0"
    "clearAllTrials\0setDigitalTrigger\0"
    "setTriggerPolarity\0configSave\0saveData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PSTHDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    1,   85,    2, 0x08 /* Private */,
       5,    0,   88,    2, 0x08 /* Private */,
       6,    0,   89,    2, 0x08 /* Private */,
       7,    1,   90,    2, 0x08 /* Private */,
       9,    1,   93,    2, 0x08 /* Private */,
      10,    1,   96,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      13,    0,  103,    2, 0x08 /* Private */,
      14,    1,  104,    2, 0x08 /* Private */,
      15,    1,  107,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PSTHDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PSTHDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->changeCurrentChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->toggleLock(); break;
        case 3: _t->setToSelected(); break;
        case 4: _t->setPreTriggerSpan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setPostTriggerSpan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setBinSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setMaxNumTrials((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->clearLastTrial(); break;
        case 9: _t->clearAllTrials(); break;
        case 10: _t->setDigitalTrigger((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setTriggerPolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->configSave(); break;
        case 13: _t->saveData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PSTHDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_PSTHDialog.data,
    qt_meta_data_PSTHDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PSTHDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PSTHDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PSTHDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PSTHDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
struct qt_meta_stringdata_PSTHSaveConfigDialog_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PSTHSaveConfigDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PSTHSaveConfigDialog_t qt_meta_stringdata_PSTHSaveConfigDialog = {
    {
QT_MOC_LITERAL(0, 0, 20) // "PSTHSaveConfigDialog"

    },
    "PSTHSaveConfigDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PSTHSaveConfigDialog[] = {

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

void PSTHSaveConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject PSTHSaveConfigDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_PSTHSaveConfigDialog.data,
    qt_meta_data_PSTHSaveConfigDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PSTHSaveConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PSTHSaveConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PSTHSaveConfigDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PSTHSaveConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
