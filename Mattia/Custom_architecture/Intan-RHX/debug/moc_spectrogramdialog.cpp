/****************************************************************************
** Meta object code from reading C++ file 'spectrogramdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Dialogs/spectrogramdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrogramdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SpectrogramDialog_t {
    QByteArrayData data[24];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpectrogramDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpectrogramDialog_t qt_meta_stringdata_SpectrogramDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SpectrogramDialog"
QT_MOC_LITERAL(1, 18, 15), // "updateFromState"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 20), // "changeCurrentChannel"
QT_MOC_LITERAL(4, 56, 17), // "nativeChannelName"
QT_MOC_LITERAL(5, 74, 10), // "toggleLock"
QT_MOC_LITERAL(6, 85, 13), // "setToSelected"
QT_MOC_LITERAL(7, 99, 17), // "changeDisplayMode"
QT_MOC_LITERAL(8, 117, 5), // "index"
QT_MOC_LITERAL(9, 123, 15), // "setNumFftPoints"
QT_MOC_LITERAL(10, 139, 7), // "setFMin"
QT_MOC_LITERAL(11, 147, 4), // "fMin"
QT_MOC_LITERAL(12, 152, 7), // "setFMax"
QT_MOC_LITERAL(13, 160, 4), // "fMax"
QT_MOC_LITERAL(14, 165, 10), // "setFMarker"
QT_MOC_LITERAL(15, 176, 7), // "fMarker"
QT_MOC_LITERAL(16, 184, 17), // "toggleShowFMarker"
QT_MOC_LITERAL(17, 202, 7), // "enabled"
QT_MOC_LITERAL(18, 210, 15), // "setNumHarmonics"
QT_MOC_LITERAL(19, 226, 3), // "num"
QT_MOC_LITERAL(20, 230, 12), // "setTimeScale"
QT_MOC_LITERAL(21, 243, 17), // "setDigitalDisplay"
QT_MOC_LITERAL(22, 261, 10), // "configSave"
QT_MOC_LITERAL(23, 272, 8) // "saveData"

    },
    "SpectrogramDialog\0updateFromState\0\0"
    "changeCurrentChannel\0nativeChannelName\0"
    "toggleLock\0setToSelected\0changeDisplayMode\0"
    "index\0setNumFftPoints\0setFMin\0fMin\0"
    "setFMax\0fMax\0setFMarker\0fMarker\0"
    "toggleShowFMarker\0enabled\0setNumHarmonics\0"
    "num\0setTimeScale\0setDigitalDisplay\0"
    "configSave\0saveData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpectrogramDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    1,   95,    2, 0x08 /* Private */,
       9,    1,   98,    2, 0x08 /* Private */,
      10,    1,  101,    2, 0x08 /* Private */,
      12,    1,  104,    2, 0x08 /* Private */,
      14,    1,  107,    2, 0x08 /* Private */,
      16,    1,  110,    2, 0x08 /* Private */,
      18,    1,  113,    2, 0x08 /* Private */,
      20,    1,  116,    2, 0x08 /* Private */,
      21,    1,  119,    2, 0x08 /* Private */,
      22,    0,  122,    2, 0x08 /* Private */,
      23,    0,  123,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SpectrogramDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SpectrogramDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->changeCurrentChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->toggleLock(); break;
        case 3: _t->setToSelected(); break;
        case 4: _t->changeDisplayMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setNumFftPoints((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setFMin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setFMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setFMarker((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->toggleShowFMarker((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->setNumHarmonics((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setTimeScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setDigitalDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->configSave(); break;
        case 14: _t->saveData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SpectrogramDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SpectrogramDialog.data,
    qt_meta_data_SpectrogramDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SpectrogramDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpectrogramDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrogramDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SpectrogramDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
struct qt_meta_stringdata_SpectrogramSaveConfigDialog_t {
    QByteArrayData data[1];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpectrogramSaveConfigDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpectrogramSaveConfigDialog_t qt_meta_stringdata_SpectrogramSaveConfigDialog = {
    {
QT_MOC_LITERAL(0, 0, 27) // "SpectrogramSaveConfigDialog"

    },
    "SpectrogramSaveConfigDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpectrogramSaveConfigDialog[] = {

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

void SpectrogramSaveConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SpectrogramSaveConfigDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SpectrogramSaveConfigDialog.data,
    qt_meta_data_SpectrogramSaveConfigDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SpectrogramSaveConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpectrogramSaveConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrogramSaveConfigDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SpectrogramSaveConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
