/****************************************************************************
** Meta object code from reading C++ file 'stimfigure.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:/GitHub/closed-loop-neuroscience/Custom_architecture/Development/IntanStimRecordController/V4/source/stimfigure.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stimfigure.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StimFigure_t {
    QByteArrayData data[16];
    char stringdata0[341];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StimFigure_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StimFigure_t qt_meta_stringdata_StimFigure = {
    {
QT_MOC_LITERAL(0, 0, 10), // "StimFigure"
QT_MOC_LITERAL(1, 11, 21), // "updateEnableAmpSettle"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "enabled"
QT_MOC_LITERAL(4, 42, 23), // "updateMaintainAmpSettle"
QT_MOC_LITERAL(5, 66, 8), // "maintain"
QT_MOC_LITERAL(6, 75, 26), // "updateEnableChargeRecovery"
QT_MOC_LITERAL(7, 102, 28), // "highlightSecondPhaseDuration"
QT_MOC_LITERAL(8, 131, 9), // "highlight"
QT_MOC_LITERAL(9, 141, 24), // "highlightInterphaseDelay"
QT_MOC_LITERAL(10, 166, 28), // "highlightFirstPhaseAmplitude"
QT_MOC_LITERAL(11, 195, 29), // "highlightSecondPhaseAmplitude"
QT_MOC_LITERAL(12, 225, 25), // "highlightPreStimAmpSettle"
QT_MOC_LITERAL(13, 251, 26), // "highlightPostStimAmpSettle"
QT_MOC_LITERAL(14, 278, 30), // "highlightPostStimChargeRecovOn"
QT_MOC_LITERAL(15, 309, 31) // "highlightPostStimChargeRecovOff"

    },
    "StimFigure\0updateEnableAmpSettle\0\0"
    "enabled\0updateMaintainAmpSettle\0"
    "maintain\0updateEnableChargeRecovery\0"
    "highlightSecondPhaseDuration\0highlight\0"
    "highlightInterphaseDelay\0"
    "highlightFirstPhaseAmplitude\0"
    "highlightSecondPhaseAmplitude\0"
    "highlightPreStimAmpSettle\0"
    "highlightPostStimAmpSettle\0"
    "highlightPostStimChargeRecovOn\0"
    "highlightPostStimChargeRecovOff"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StimFigure[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a /* Public */,
       4,    1,   72,    2, 0x0a /* Public */,
       6,    1,   75,    2, 0x0a /* Public */,
       7,    1,   78,    2, 0x0a /* Public */,
       9,    1,   81,    2, 0x0a /* Public */,
      10,    1,   84,    2, 0x0a /* Public */,
      11,    1,   87,    2, 0x0a /* Public */,
      12,    1,   90,    2, 0x0a /* Public */,
      13,    1,   93,    2, 0x0a /* Public */,
      14,    1,   96,    2, 0x0a /* Public */,
      15,    1,   99,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void StimFigure::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StimFigure *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateEnableAmpSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->updateMaintainAmpSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updateEnableChargeRecovery((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->highlightSecondPhaseDuration((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->highlightInterphaseDelay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->highlightFirstPhaseAmplitude((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->highlightSecondPhaseAmplitude((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->highlightPreStimAmpSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->highlightPostStimAmpSettle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->highlightPostStimChargeRecovOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->highlightPostStimChargeRecovOff((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StimFigure::staticMetaObject = { {
    &AbstractFigure::staticMetaObject,
    qt_meta_stringdata_StimFigure.data,
    qt_meta_data_StimFigure,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StimFigure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StimFigure::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StimFigure.stringdata0))
        return static_cast<void*>(this);
    return AbstractFigure::qt_metacast(_clname);
}

int StimFigure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractFigure::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
