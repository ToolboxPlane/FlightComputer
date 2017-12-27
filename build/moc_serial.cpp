/****************************************************************************
** Meta object code from reading C++ file 'serial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FlightComputerSerial/serial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Serial_t {
    QByteArrayData data[9];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Serial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Serial_t qt_meta_stringdata_Serial = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Serial"
QT_MOC_LITERAL(1, 7, 4), // "test"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 3), // "arg"
QT_MOC_LITERAL(4, 17, 10), // "getChannel"
QT_MOC_LITERAL(5, 28, 7), // "channel"
QT_MOC_LITERAL(6, 36, 10), // "setChannel"
QT_MOC_LITERAL(7, 47, 3), // "val"
QT_MOC_LITERAL(8, 51, 4) // "read"

    },
    "Serial\0test\0\0arg\0getChannel\0channel\0"
    "setChannel\0val\0read"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Serial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x4a /* Public | isScriptable */,
       4,    1,   37,    2, 0x4a /* Public | isScriptable */,
       6,    2,   40,    2, 0x4a /* Public | isScriptable */,
       8,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::QString, QMetaType::QString,    3,
    QMetaType::Int, QMetaType::Int,    5,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int,    5,    7,
    QMetaType::Void,

       0        // eod
};

void Serial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Serial *_t = static_cast<Serial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QString _r = _t->test((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->getChannel((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->setChannel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->read(); break;
        default: ;
        }
    }
}

const QMetaObject Serial::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Serial.data,
      qt_meta_data_Serial,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Serial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Serial::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Serial.stringdata0))
        return static_cast<void*>(const_cast< Serial*>(this));
    return QObject::qt_metacast(_clname);
}

int Serial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
