/****************************************************************************
** Meta object code from reading C++ file 'digoutdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../digoutdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'digoutdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DigOutDialog_t {
    QByteArrayData data[16];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DigOutDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DigOutDialog_t qt_meta_stringdata_DigOutDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DigOutDialog"
QT_MOC_LITERAL(1, 13, 27), // "highlightFirstPhaseDuration"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 9), // "highlight"
QT_MOC_LITERAL(4, 52, 25), // "highlightPostTriggerDelay"
QT_MOC_LITERAL(5, 78, 25), // "highlightPulseTrainPeriod"
QT_MOC_LITERAL(6, 104, 25), // "highlightRefractoryPeriod"
QT_MOC_LITERAL(7, 130, 6), // "accept"
QT_MOC_LITERAL(8, 137, 18), // "notifyFocusChanged"
QT_MOC_LITERAL(9, 156, 8), // "QWidget*"
QT_MOC_LITERAL(10, 165, 9), // "lostFocus"
QT_MOC_LITERAL(11, 175, 11), // "gainedFocus"
QT_MOC_LITERAL(12, 187, 13), // "enableWidgets"
QT_MOC_LITERAL(13, 201, 15), // "roundTimeInputs"
QT_MOC_LITERAL(14, 217, 28), // "calculatePulseTrainFrequency"
QT_MOC_LITERAL(15, 246, 25) // "constrainPulseTrainPeriod"

    },
    "DigOutDialog\0highlightFirstPhaseDuration\0"
    "\0highlight\0highlightPostTriggerDelay\0"
    "highlightPulseTrainPeriod\0"
    "highlightRefractoryPeriod\0accept\0"
    "notifyFocusChanged\0QWidget*\0lostFocus\0"
    "gainedFocus\0enableWidgets\0roundTimeInputs\0"
    "calculatePulseTrainFrequency\0"
    "constrainPulseTrainPeriod"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DigOutDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       5,    1,   70,    2, 0x06 /* Public */,
       6,    1,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   76,    2, 0x0a /* Public */,
       8,    2,   77,    2, 0x0a /* Public */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,
      14,    0,   84,    2, 0x08 /* Private */,
      15,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DigOutDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DigOutDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->highlightFirstPhaseDuration((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->highlightPostTriggerDelay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->highlightPulseTrainPeriod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->highlightRefractoryPeriod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->accept(); break;
        case 5: _t->notifyFocusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 6: _t->enableWidgets(); break;
        case 7: _t->roundTimeInputs(); break;
        case 8: _t->calculatePulseTrainFrequency(); break;
        case 9: _t->constrainPulseTrainPeriod(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DigOutDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DigOutDialog::highlightFirstPhaseDuration)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DigOutDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DigOutDialog::highlightPostTriggerDelay)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DigOutDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DigOutDialog::highlightPulseTrainPeriod)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DigOutDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DigOutDialog::highlightRefractoryPeriod)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DigOutDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_DigOutDialog.data,
    qt_meta_data_DigOutDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DigOutDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DigOutDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DigOutDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DigOutDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void DigOutDialog::highlightFirstPhaseDuration(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DigOutDialog::highlightPostTriggerDelay(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DigOutDialog::highlightPulseTrainPeriod(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DigOutDialog::highlightRefractoryPeriod(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
