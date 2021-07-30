/****************************************************************************
** Meta object code from reading C++ file 'controlpaneltriggertab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/controlpaneltriggertab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpaneltriggertab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControlPanelTriggerTab_t {
    QByteArrayData data[8];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlPanelTriggerTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlPanelTriggerTab_t qt_meta_stringdata_ControlPanelTriggerTab = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ControlPanelTriggerTab"
QT_MOC_LITERAL(1, 23, 13), // "enableTrigger"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 6), // "enable"
QT_MOC_LITERAL(4, 45, 16), // "setTriggerSource"
QT_MOC_LITERAL(5, 62, 5), // "index"
QT_MOC_LITERAL(6, 68, 18), // "setTriggerPolarity"
QT_MOC_LITERAL(7, 87, 18) // "setTriggerPosition"

    },
    "ControlPanelTriggerTab\0enableTrigger\0"
    "\0enable\0setTriggerSource\0index\0"
    "setTriggerPolarity\0setTriggerPosition"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlPanelTriggerTab[] = {

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
       6,    1,   40,    2, 0x08 /* Private */,
       7,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void ControlPanelTriggerTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanelTriggerTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enableTrigger((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setTriggerSource((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setTriggerPolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setTriggerPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControlPanelTriggerTab::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ControlPanelTriggerTab.data,
    qt_meta_data_ControlPanelTriggerTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControlPanelTriggerTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelTriggerTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelTriggerTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanelTriggerTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
