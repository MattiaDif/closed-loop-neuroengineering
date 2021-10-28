/****************************************************************************
** Meta object code from reading C++ file 'anoutfigure.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../anoutfigure.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'anoutfigure.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnOutFigure_t {
    QByteArrayData data[10];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnOutFigure_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnOutFigure_t qt_meta_stringdata_AnOutFigure = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AnOutFigure"
QT_MOC_LITERAL(1, 12, 27), // "updateMonophasicAndPositive"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 10), // "logicValue"
QT_MOC_LITERAL(4, 52, 28), // "highlightSecondPhaseDuration"
QT_MOC_LITERAL(5, 81, 9), // "highlight"
QT_MOC_LITERAL(6, 91, 24), // "highlightInterphaseDelay"
QT_MOC_LITERAL(7, 116, 28), // "highlightFirstPhaseAmplitude"
QT_MOC_LITERAL(8, 145, 29), // "highlightSecondPhaseAmplitude"
QT_MOC_LITERAL(9, 175, 24) // "highlightBaselineVoltage"

    },
    "AnOutFigure\0updateMonophasicAndPositive\0"
    "\0logicValue\0highlightSecondPhaseDuration\0"
    "highlight\0highlightInterphaseDelay\0"
    "highlightFirstPhaseAmplitude\0"
    "highlightSecondPhaseAmplitude\0"
    "highlightBaselineVoltage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnOutFigure[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       6,    1,   50,    2, 0x0a /* Public */,
       7,    1,   53,    2, 0x0a /* Public */,
       8,    1,   56,    2, 0x0a /* Public */,
       9,    1,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void AnOutFigure::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AnOutFigure *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateMonophasicAndPositive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->highlightSecondPhaseDuration((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->highlightInterphaseDelay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->highlightFirstPhaseAmplitude((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->highlightSecondPhaseAmplitude((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->highlightBaselineVoltage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AnOutFigure::staticMetaObject = { {
    &AbstractFigure::staticMetaObject,
    qt_meta_stringdata_AnOutFigure.data,
    qt_meta_data_AnOutFigure,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AnOutFigure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnOutFigure::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnOutFigure.stringdata0))
        return static_cast<void*>(this);
    return AbstractFigure::qt_metacast(_clname);
}

int AnOutFigure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractFigure::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
