/****************************************************************************
** Meta object code from reading C++ file 'smartspinbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/smartspinbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smartspinbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmartSpinBox_t {
    QByteArrayData data[11];
    char stringdata0[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmartSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmartSpinBox_t qt_meta_stringdata_SmartSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SmartSpinBox"
QT_MOC_LITERAL(1, 13, 15), // "editingFinished"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12), // "valueChanged"
QT_MOC_LITERAL(4, 43, 16), // "trueValueChanged"
QT_MOC_LITERAL(5, 60, 8), // "setValue"
QT_MOC_LITERAL(6, 69, 3), // "val"
QT_MOC_LITERAL(7, 73, 9), // "loadValue"
QT_MOC_LITERAL(8, 83, 10), // "roundValue"
QT_MOC_LITERAL(9, 94, 10), // "scaleUnits"
QT_MOC_LITERAL(10, 105, 20) // "sendSignalValueMicro"

    },
    "SmartSpinBox\0editingFinished\0\0"
    "valueChanged\0trueValueChanged\0setValue\0"
    "val\0loadValue\0roundValue\0scaleUnits\0"
    "sendSignalValueMicro"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmartSpinBox[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       4,    1,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   61,    2, 0x0a /* Public */,
       7,    1,   64,    2, 0x0a /* Public */,
       8,    0,   67,    2, 0x0a /* Public */,
       9,    1,   68,    2, 0x09 /* Protected */,
      10,    1,   71,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,

       0        // eod
};

void SmartSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SmartSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->editingFinished(); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->trueValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->loadValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->roundValue(); break;
        case 6: _t->scaleUnits((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->sendSignalValueMicro((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SmartSpinBox::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmartSpinBox::editingFinished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SmartSpinBox::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmartSpinBox::valueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SmartSpinBox::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmartSpinBox::trueValueChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SmartSpinBox::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SmartSpinBox.data,
    qt_meta_data_SmartSpinBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SmartSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmartSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmartSpinBox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmartSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void SmartSpinBox::editingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SmartSpinBox::valueChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SmartSpinBox::trueValueChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_CurrentSpinBox_t {
    QByteArrayData data[7];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurrentSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurrentSpinBox_t qt_meta_stringdata_CurrentSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CurrentSpinBox"
QT_MOC_LITERAL(1, 15, 9), // "loadValue"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "val"
QT_MOC_LITERAL(4, 30, 10), // "roundValue"
QT_MOC_LITERAL(5, 41, 10), // "scaleUnits"
QT_MOC_LITERAL(6, 52, 20) // "sendSignalValueMicro"

    },
    "CurrentSpinBox\0loadValue\0\0val\0roundValue\0"
    "scaleUnits\0sendSignalValueMicro"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurrentSpinBox[] = {

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
       1,    1,   34,    2, 0x0a /* Public */,
       4,    0,   37,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x08 /* Private */,
       6,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void CurrentSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CurrentSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->roundValue(); break;
        case 2: _t->scaleUnits((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->sendSignalValueMicro((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CurrentSpinBox::staticMetaObject = { {
    &SmartSpinBox::staticMetaObject,
    qt_meta_stringdata_CurrentSpinBox.data,
    qt_meta_data_CurrentSpinBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CurrentSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurrentSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurrentSpinBox.stringdata0))
        return static_cast<void*>(this);
    return SmartSpinBox::qt_metacast(_clname);
}

int CurrentSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SmartSpinBox::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_TimeSpinBox_t {
    QByteArrayData data[7];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TimeSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TimeSpinBox_t qt_meta_stringdata_TimeSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TimeSpinBox"
QT_MOC_LITERAL(1, 12, 9), // "loadValue"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "val"
QT_MOC_LITERAL(4, 27, 10), // "roundValue"
QT_MOC_LITERAL(5, 38, 10), // "scaleUnits"
QT_MOC_LITERAL(6, 49, 20) // "sendSignalValueMicro"

    },
    "TimeSpinBox\0loadValue\0\0val\0roundValue\0"
    "scaleUnits\0sendSignalValueMicro"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TimeSpinBox[] = {

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
       1,    1,   34,    2, 0x0a /* Public */,
       4,    0,   37,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x08 /* Private */,
       6,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void TimeSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TimeSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->roundValue(); break;
        case 2: _t->scaleUnits((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->sendSignalValueMicro((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TimeSpinBox::staticMetaObject = { {
    &SmartSpinBox::staticMetaObject,
    qt_meta_stringdata_TimeSpinBox.data,
    qt_meta_data_TimeSpinBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TimeSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TimeSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TimeSpinBox.stringdata0))
        return static_cast<void*>(this);
    return SmartSpinBox::qt_metacast(_clname);
}

int TimeSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SmartSpinBox::qt_metacall(_c, _id, _a);
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
