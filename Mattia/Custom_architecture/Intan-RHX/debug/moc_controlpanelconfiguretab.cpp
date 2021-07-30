/****************************************************************************
** Meta object code from reading C++ file 'controlpanelconfiguretab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/controlpanelconfiguretab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanelconfiguretab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ControlPanelConfigureTab_t {
    QByteArrayData data[16];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlPanelConfigureTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlPanelConfigureTab_t qt_meta_stringdata_ControlPanelConfigureTab = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ControlPanelConfigureTab"
QT_MOC_LITERAL(1, 25, 18), // "sendExecuteCommand"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 15), // "sendNoteCommand"
QT_MOC_LITERAL(4, 61, 11), // "rescanPorts"
QT_MOC_LITERAL(5, 73, 23), // "manualCableDelayControl"
QT_MOC_LITERAL(6, 97, 19), // "configDigOutControl"
QT_MOC_LITERAL(7, 117, 16), // "enableFastSettle"
QT_MOC_LITERAL(8, 134, 6), // "enable"
QT_MOC_LITERAL(9, 141, 24), // "enableExternalFastSettle"
QT_MOC_LITERAL(10, 166, 28), // "setExternalFastSettleChannel"
QT_MOC_LITERAL(11, 195, 7), // "channel"
QT_MOC_LITERAL(12, 203, 11), // "addLiveNote"
QT_MOC_LITERAL(13, 215, 19), // "displayLastLiveNote"
QT_MOC_LITERAL(14, 235, 4), // "note"
QT_MOC_LITERAL(15, 240, 8) // "setNotes"

    },
    "ControlPanelConfigureTab\0sendExecuteCommand\0"
    "\0sendNoteCommand\0rescanPorts\0"
    "manualCableDelayControl\0configDigOutControl\0"
    "enableFastSettle\0enable\0"
    "enableExternalFastSettle\0"
    "setExternalFastSettleChannel\0channel\0"
    "addLiveNote\0displayLastLiveNote\0note\0"
    "setNotes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlPanelConfigureTab[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       3,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   75,    2, 0x08 /* Private */,
       5,    0,   76,    2, 0x08 /* Private */,
       6,    0,   77,    2, 0x08 /* Private */,
       7,    1,   78,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      10,    1,   84,    2, 0x08 /* Private */,
      12,    0,   87,    2, 0x08 /* Private */,
      13,    1,   88,    2, 0x08 /* Private */,
      15,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

       0        // eod
};

void ControlPanelConfigureTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanelConfigureTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendExecuteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sendNoteCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->rescanPorts(); break;
        case 3: _t->manualCableDelayControl(); break;
        case 4: _t->configDigOutControl(); break;
        case 5: _t->enableFastSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->enableExternalFastSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setExternalFastSettleChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->addLiveNote(); break;
        case 9: _t->displayLastLiveNote((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setNotes(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControlPanelConfigureTab::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlPanelConfigureTab::sendExecuteCommand)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControlPanelConfigureTab::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ControlPanelConfigureTab::sendNoteCommand)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ControlPanelConfigureTab::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ControlPanelConfigureTab.data,
    qt_meta_data_ControlPanelConfigureTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ControlPanelConfigureTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelConfigureTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelConfigureTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanelConfigureTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ControlPanelConfigureTab::sendExecuteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlPanelConfigureTab::sendNoteCommand(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
