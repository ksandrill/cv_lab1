/****************************************************************************
** Meta object code from reading C++ file 'HsvSlider.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../HsvSlider.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HsvSlider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HsvSlider_t {
    const uint offsetsAndSize[28];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HsvSlider_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HsvSlider_t qt_meta_stringdata_HsvSlider = {
    {
QT_MOC_LITERAL(0, 9), // "HsvSlider"
QT_MOC_LITERAL(10, 10), // "hueChanged"
QT_MOC_LITERAL(21, 0), // ""
QT_MOC_LITERAL(22, 3), // "hue"
QT_MOC_LITERAL(26, 17), // "saturationChanged"
QT_MOC_LITERAL(44, 10), // "saturation"
QT_MOC_LITERAL(55, 12), // "valueChanged"
QT_MOC_LITERAL(68, 5), // "value"
QT_MOC_LITERAL(74, 6), // "setHue"
QT_MOC_LITERAL(81, 13), // "setSaturation"
QT_MOC_LITERAL(95, 8), // "setValue"
QT_MOC_LITERAL(104, 8), // "getImage"
QT_MOC_LITERAL(113, 8), // "setImage"
QT_MOC_LITERAL(122, 5) // "image"

    },
    "HsvSlider\0hueChanged\0\0hue\0saturationChanged\0"
    "saturation\0valueChanged\0value\0setHue\0"
    "setSaturation\0setValue\0getImage\0"
    "setImage\0image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HsvSlider[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    0 /* Public */,
       4,    1,   65,    2, 0x06,    2 /* Public */,
       6,    1,   68,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,   71,    2, 0x0a,    6 /* Public */,
       9,    1,   74,    2, 0x0a,    8 /* Public */,
      10,    1,   77,    2, 0x0a,   10 /* Public */,
      11,    0,   80,    2, 0x0a,   12 /* Public */,
      12,    1,   81,    2, 0x0a,   13 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,   13,

       0        // eod
};

void HsvSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HsvSlider *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->hueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->saturationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setHue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setSaturation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->getImage(); break;
        case 7: _t->setImage((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HsvSlider::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HsvSlider::hueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HsvSlider::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HsvSlider::saturationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HsvSlider::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HsvSlider::valueChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject HsvSlider::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_HsvSlider.offsetsAndSize,
    qt_meta_data_HsvSlider,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HsvSlider_t
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>


>,
    nullptr
} };


const QMetaObject *HsvSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HsvSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HsvSlider.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HsvSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void HsvSlider::hueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HsvSlider::saturationChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HsvSlider::valueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
