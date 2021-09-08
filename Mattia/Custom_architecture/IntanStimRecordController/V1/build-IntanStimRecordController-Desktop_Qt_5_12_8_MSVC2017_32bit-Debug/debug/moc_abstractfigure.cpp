/****************************************************************************
** Meta object code from reading C++ file 'abstractfigure.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/abstractfigure.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'abstractfigure.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AbstractFigure_t {
    QByteArrayData data[14];
    char stringdata0[241];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AbstractFigure_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AbstractFigure_t qt_meta_stringdata_AbstractFigure = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AbstractFigure"
QT_MOC_LITERAL(1, 15, 15), // "updateStimShape"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 9), // "stimShape"
QT_MOC_LITERAL(4, 42, 18), // "updateStimPolarity"
QT_MOC_LITERAL(5, 61, 12), // "stimPolarity"
QT_MOC_LITERAL(6, 74, 18), // "updatePulseOrTrain"
QT_MOC_LITERAL(7, 93, 12), // "pulseOrTrain"
QT_MOC_LITERAL(8, 106, 25), // "highlightPostTriggerDelay"
QT_MOC_LITERAL(9, 132, 9), // "highlight"
QT_MOC_LITERAL(10, 142, 25), // "highlightPulseTrainPeriod"
QT_MOC_LITERAL(11, 168, 25), // "highlightRefractoryPeriod"
QT_MOC_LITERAL(12, 194, 18), // "highlightStimTrace"
QT_MOC_LITERAL(13, 213, 27) // "highlightFirstPhaseDuration"

    },
    "AbstractFigure\0updateStimShape\0\0"
    "stimShape\0updateStimPolarity\0stimPolarity\0"
    "updatePulseOrTrain\0pulseOrTrain\0"
    "highlightPostTriggerDelay\0highlight\0"
    "highlightPulseTrainPeriod\0"
    "highlightRefractoryPeriod\0highlightStimTrace\0"
    "highlightFirstPhaseDuration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AbstractFigure[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    1,   57,    2, 0x0a /* Public */,
       6,    1,   60,    2, 0x0a /* Public */,
       8,    1,   63,    2, 0x0a /* Public */,
      10,    1,   66,    2, 0x0a /* Public */,
      11,    1,   69,    2, 0x0a /* Public */,
      12,    1,   72,    2, 0x0a /* Public */,
      13,    1,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void AbstractFigure::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AbstractFigure *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateStimShape((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateStimPolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updatePulseOrTrain((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->highlightPostTriggerDelay((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->highlightPulseTrainPeriod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->highlightRefractoryPeriod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->highlightStimTrace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->highlightFirstPhaseDuration((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AbstractFigure::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_AbstractFigure.data,
    qt_meta_data_AbstractFigure,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AbstractFigure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AbstractFigure::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AbstractFigure.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AbstractFigure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
