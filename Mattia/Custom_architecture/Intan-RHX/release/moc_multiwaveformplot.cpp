/****************************************************************************
** Meta object code from reading C++ file 'multiwaveformplot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/multiwaveformplot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiwaveformplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MultiWaveformPlot_t {
    QByteArrayData data[9];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiWaveformPlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiWaveformPlot_t qt_meta_stringdata_MultiWaveformPlot = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MultiWaveformPlot"
QT_MOC_LITERAL(1, 18, 15), // "updateFromState"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 24), // "openWaveformSelectDialog"
QT_MOC_LITERAL(4, 60, 17), // "addPinnedWaveform"
QT_MOC_LITERAL(5, 78, 8), // "waveName"
QT_MOC_LITERAL(6, 87, 15), // "increaseSpacing"
QT_MOC_LITERAL(7, 103, 15), // "decreaseSpacing"
QT_MOC_LITERAL(8, 119, 10) // "enableSlot"

    },
    "MultiWaveformPlot\0updateFromState\0\0"
    "openWaveformSelectDialog\0addPinnedWaveform\0"
    "waveName\0increaseSpacing\0decreaseSpacing\0"
    "enableSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiWaveformPlot[] = {

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
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    1,   46,    2, 0x0a /* Public */,
       6,    0,   49,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultiWaveformPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MultiWaveformPlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->openWaveformSelectDialog(); break;
        case 2: _t->addPinnedWaveform((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->increaseSpacing(); break;
        case 4: _t->decreaseSpacing(); break;
        case 5: _t->enableSlot(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MultiWaveformPlot::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_MultiWaveformPlot.data,
    qt_meta_data_MultiWaveformPlot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MultiWaveformPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiWaveformPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultiWaveformPlot.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MultiWaveformPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
