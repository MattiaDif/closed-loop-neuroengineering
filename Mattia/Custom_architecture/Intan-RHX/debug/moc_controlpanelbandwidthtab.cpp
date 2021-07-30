/****************************************************************************
** Meta object code from reading C++ file 'controlpanelbandwidthtab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/controlpanelbandwidthtab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanelbandwidthtab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControlPanelBandwidthTab_t {
    QByteArrayData data[17];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlPanelBandwidthTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlPanelBandwidthTab_t qt_meta_stringdata_ControlPanelBandwidthTab = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ControlPanelBandwidthTab"
QT_MOC_LITERAL(1, 25, 21), // "simpleBandwidthDialog"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 23), // "advancedBandwidthDialog"
QT_MOC_LITERAL(4, 72, 15), // "viewFiltersSlot"
QT_MOC_LITERAL(5, 88, 17), // "changeNotchFilter"
QT_MOC_LITERAL(6, 106, 11), // "filterIndex"
QT_MOC_LITERAL(7, 118, 13), // "changeLowType"
QT_MOC_LITERAL(8, 132, 7), // "lowType"
QT_MOC_LITERAL(9, 140, 14), // "changeHighType"
QT_MOC_LITERAL(10, 155, 8), // "highType"
QT_MOC_LITERAL(11, 164, 14), // "changeLowOrder"
QT_MOC_LITERAL(12, 179, 8), // "lowOrder"
QT_MOC_LITERAL(13, 188, 15), // "changeHighOrder"
QT_MOC_LITERAL(14, 204, 9), // "highOrder"
QT_MOC_LITERAL(15, 214, 15), // "changeLowCutoff"
QT_MOC_LITERAL(16, 230, 16) // "changeHighCutoff"

    },
    "ControlPanelBandwidthTab\0simpleBandwidthDialog\0"
    "\0advancedBandwidthDialog\0viewFiltersSlot\0"
    "changeNotchFilter\0filterIndex\0"
    "changeLowType\0lowType\0changeHighType\0"
    "highType\0changeLowOrder\0lowOrder\0"
    "changeHighOrder\0highOrder\0changeLowCutoff\0"
    "changeHighCutoff"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlPanelBandwidthTab[] = {

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
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    1,   67,    2, 0x08 /* Private */,
       7,    1,   70,    2, 0x08 /* Private */,
       9,    1,   73,    2, 0x08 /* Private */,
      11,    1,   76,    2, 0x08 /* Private */,
      13,    1,   79,    2, 0x08 /* Private */,
      15,    0,   82,    2, 0x08 /* Private */,
      16,    0,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ControlPanelBandwidthTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanelBandwidthTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->simpleBandwidthDialog(); break;
        case 1: _t->advancedBandwidthDialog(); break;
        case 2: _t->viewFiltersSlot(); break;
        case 3: _t->changeNotchFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->changeLowType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->changeHighType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->changeLowOrder((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->changeHighOrder((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->changeLowCutoff(); break;
        case 9: _t->changeHighCutoff(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControlPanelBandwidthTab::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ControlPanelBandwidthTab.data,
    qt_meta_data_ControlPanelBandwidthTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControlPanelBandwidthTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelBandwidthTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelBandwidthTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanelBandwidthTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
