/****************************************************************************
** Meta object code from reading C++ file 'pageview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Widgets/pageview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pageview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PageView_t {
    QByteArrayData data[24];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PageView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PageView_t qt_meta_stringdata_PageView = {
    {
QT_MOC_LITERAL(0, 0, 8), // "PageView"
QT_MOC_LITERAL(1, 9, 10), // "mouseMoved"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 1), // "x"
QT_MOC_LITERAL(4, 23, 1), // "y"
QT_MOC_LITERAL(5, 25, 12), // "mousePresent"
QT_MOC_LITERAL(6, 38, 15), // "hoveredSiteName"
QT_MOC_LITERAL(7, 54, 9), // "mouseExit"
QT_MOC_LITERAL(8, 64, 10), // "mouseEnter"
QT_MOC_LITERAL(9, 75, 14), // "resizeComplete"
QT_MOC_LITERAL(10, 90, 15), // "siteHighlighted"
QT_MOC_LITERAL(11, 106, 10), // "nativeName"
QT_MOC_LITERAL(12, 117, 9), // "highlight"
QT_MOC_LITERAL(13, 127, 16), // "deselectAllSites"
QT_MOC_LITERAL(14, 144, 7), // "bestFit"
QT_MOC_LITERAL(15, 152, 8), // "scrollUp"
QT_MOC_LITERAL(16, 161, 10), // "scrollDown"
QT_MOC_LITERAL(17, 172, 10), // "scrollLeft"
QT_MOC_LITERAL(18, 183, 11), // "scrollRight"
QT_MOC_LITERAL(19, 195, 6), // "zoomIn"
QT_MOC_LITERAL(20, 202, 7), // "zoomOut"
QT_MOC_LITERAL(21, 210, 10), // "changeMode"
QT_MOC_LITERAL(22, 221, 9), // "MouseMode"
QT_MOC_LITERAL(23, 231, 4) // "mode"

    },
    "PageView\0mouseMoved\0\0x\0y\0mousePresent\0"
    "hoveredSiteName\0mouseExit\0mouseEnter\0"
    "resizeComplete\0siteHighlighted\0"
    "nativeName\0highlight\0deselectAllSites\0"
    "bestFit\0scrollUp\0scrollDown\0scrollLeft\0"
    "scrollRight\0zoomIn\0zoomOut\0changeMode\0"
    "MouseMode\0mode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PageView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   84,    2, 0x06 /* Public */,
       7,    0,   93,    2, 0x06 /* Public */,
       8,    0,   94,    2, 0x06 /* Public */,
       9,    0,   95,    2, 0x06 /* Public */,
      10,    2,   96,    2, 0x06 /* Public */,
      13,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  102,    2, 0x0a /* Public */,
      15,    0,  103,    2, 0x0a /* Public */,
      16,    0,  104,    2, 0x0a /* Public */,
      17,    0,  105,    2, 0x0a /* Public */,
      18,    0,  106,    2, 0x0a /* Public */,
      19,    0,  107,    2, 0x0a /* Public */,
      20,    0,  108,    2, 0x0a /* Public */,
      21,    1,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Bool, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   11,   12,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,

       0        // eod
};

void PageView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PageView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mouseMoved((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->mouseExit(); break;
        case 2: _t->mouseEnter(); break;
        case 3: _t->resizeComplete(); break;
        case 4: _t->siteHighlighted((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->deselectAllSites(); break;
        case 6: _t->bestFit(); break;
        case 7: _t->scrollUp(); break;
        case 8: _t->scrollDown(); break;
        case 9: _t->scrollLeft(); break;
        case 10: _t->scrollRight(); break;
        case 11: _t->zoomIn(); break;
        case 12: _t->zoomOut(); break;
        case 13: _t->changeMode((*reinterpret_cast< MouseMode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PageView::*)(float , float , bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::mouseMoved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PageView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::mouseExit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PageView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::mouseEnter)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PageView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::resizeComplete)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PageView::*)(QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::siteHighlighted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (PageView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageView::deselectAllSites)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PageView::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_PageView.data,
    qt_meta_data_PageView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PageView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PageView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PageView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PageView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void PageView::mouseMoved(float _t1, float _t2, bool _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PageView::mouseExit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PageView::mouseEnter()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PageView::resizeComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PageView::siteHighlighted(QString _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void PageView::deselectAllSites()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
