/****************************************************************************
** Meta object code from reading C++ file 'netvisu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../netvisu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'netvisu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetVisu_t {
    QByteArrayData data[17];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetVisu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetVisu_t qt_meta_stringdata_NetVisu = {
    {
QT_MOC_LITERAL(0, 0, 7), // "NetVisu"
QT_MOC_LITERAL(1, 8, 11), // "increaseOne"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 2), // "ms"
QT_MOC_LITERAL(4, 24, 13), // "callMeFromQml"
QT_MOC_LITERAL(5, 38, 10), // "getHiddenX"
QT_MOC_LITERAL(6, 49, 10), // "getHiddenY"
QT_MOC_LITERAL(7, 60, 9), // "getInputs"
QT_MOC_LITERAL(8, 70, 10), // "getOutputs"
QT_MOC_LITERAL(9, 81, 14), // "getStartNeuron"
QT_MOC_LITERAL(10, 96, 11), // "vector<int>"
QT_MOC_LITERAL(11, 108, 12), // "getEndNeuron"
QT_MOC_LITERAL(12, 121, 12), // "getNeuronTyp"
QT_MOC_LITERAL(13, 134, 19), // "getConnectionWeight"
QT_MOC_LITERAL(14, 154, 13), // "vector<qreal>"
QT_MOC_LITERAL(15, 168, 18), // "getNeuronValueVect"
QT_MOC_LITERAL(16, 187, 12) // "getActivFunc"

    },
    "NetVisu\0increaseOne\0\0ms\0callMeFromQml\0"
    "getHiddenX\0getHiddenY\0getInputs\0"
    "getOutputs\0getStartNeuron\0vector<int>\0"
    "getEndNeuron\0getNeuronTyp\0getConnectionWeight\0"
    "vector<qreal>\0getNeuronValueVect\0"
    "getActivFunc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetVisu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   77,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x0a /* Public */,
       6,    0,   79,    2, 0x0a /* Public */,
       7,    0,   80,    2, 0x0a /* Public */,
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,
      15,    0,   86,    2, 0x0a /* Public */,
      16,    0,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    0x80000000 | 10,
    0x80000000 | 10,
    0x80000000 | 10,
    0x80000000 | 14,
    0x80000000 | 14,
    QMetaType::Int,

       0        // eod
};

void NetVisu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetVisu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->increaseOne((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->callMeFromQml(); break;
        case 2: { int _r = _t->getHiddenX();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->getHiddenY();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { int _r = _t->getInputs();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->getOutputs();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { vector<int> _r = _t->getStartNeuron();
            if (_a[0]) *reinterpret_cast< vector<int>*>(_a[0]) = std::move(_r); }  break;
        case 7: { vector<int> _r = _t->getEndNeuron();
            if (_a[0]) *reinterpret_cast< vector<int>*>(_a[0]) = std::move(_r); }  break;
        case 8: { vector<int> _r = _t->getNeuronTyp();
            if (_a[0]) *reinterpret_cast< vector<int>*>(_a[0]) = std::move(_r); }  break;
        case 9: { vector<qreal> _r = _t->getConnectionWeight();
            if (_a[0]) *reinterpret_cast< vector<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 10: { vector<qreal> _r = _t->getNeuronValueVect();
            if (_a[0]) *reinterpret_cast< vector<qreal>*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->getActivFunc();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetVisu::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetVisu::increaseOne)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetVisu::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_NetVisu.data,
    qt_meta_data_NetVisu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetVisu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetVisu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetVisu.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetVisu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void NetVisu::increaseOne(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
