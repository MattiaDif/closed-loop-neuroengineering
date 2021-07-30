/****************************************************************************
** Meta object code from reading C++ file 'filterdisplayselector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/filterdisplayselector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filterdisplayselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FilterDisplaySelector_t {
    QByteArrayData data[14];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FilterDisplaySelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FilterDisplaySelector_t qt_meta_stringdata_FilterDisplaySelector = {
    {
QT_MOC_LITERAL(0, 0, 21), // "FilterDisplaySelector"
QT_MOC_LITERAL(1, 22, 15), // "updateFromState"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 12), // "enableOrder1"
QT_MOC_LITERAL(4, 52, 7), // "checked"
QT_MOC_LITERAL(5, 60, 12), // "enableOrder2"
QT_MOC_LITERAL(6, 73, 12), // "enableOrder3"
QT_MOC_LITERAL(7, 86, 12), // "enableOrder4"
QT_MOC_LITERAL(8, 99, 18), // "filterOrderChanged"
QT_MOC_LITERAL(9, 118, 16), // "arrangeByChanged"
QT_MOC_LITERAL(10, 135, 5), // "index"
QT_MOC_LITERAL(11, 141, 23), // "displayLabelTextChanged"
QT_MOC_LITERAL(12, 165, 16), // "changeLabelWidth"
QT_MOC_LITERAL(13, 182, 20) // "showDisabledChannels"

    },
    "FilterDisplaySelector\0updateFromState\0"
    "\0enableOrder1\0checked\0enableOrder2\0"
    "enableOrder3\0enableOrder4\0filterOrderChanged\0"
    "arrangeByChanged\0index\0displayLabelTextChanged\0"
    "changeLabelWidth\0showDisabledChannels"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilterDisplaySelector[] = {

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
       1,    0,   64,    2, 0x0a /* Public */,
       3,    1,   65,    2, 0x08 /* Private */,
       5,    1,   68,    2, 0x08 /* Private */,
       6,    1,   71,    2, 0x08 /* Private */,
       7,    1,   74,    2, 0x08 /* Private */,
       8,    0,   77,    2, 0x08 /* Private */,
       9,    1,   78,    2, 0x08 /* Private */,
      11,    1,   81,    2, 0x08 /* Private */,
      12,    1,   84,    2, 0x08 /* Private */,
      13,    1,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void FilterDisplaySelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FilterDisplaySelector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateFromState(); break;
        case 1: _t->enableOrder1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->enableOrder2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->enableOrder3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->enableOrder4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->filterOrderChanged(); break;
        case 6: _t->arrangeByChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->displayLabelTextChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->changeLabelWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->showDisabledChannels((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FilterDisplaySelector::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FilterDisplaySelector.data,
    qt_meta_data_FilterDisplaySelector,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FilterDisplaySelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FilterDisplaySelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FilterDisplaySelector.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FilterDisplaySelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
