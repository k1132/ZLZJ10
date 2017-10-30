/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,    8,    7,    7, 0x08,
      93,    7,    7,    7, 0x08,
     106,    7,    7,    7, 0x08,
     118,    7,    7,    7, 0x08,
     128,    7,    7,    7, 0x08,
     138,    7,    7,    7, 0x08,
     152,    7,    7,    7, 0x08,
     168,    7,    7,    7, 0x08,
     182,    7,    7,    7, 0x08,
     197,    7,    7,    7, 0x08,
     213,  208,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0current,previous\0"
    "on_listWidget_currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "timeoutfun()\0xiaoshiji()\0Licheng()\0"
    "shanhua()\0gzmslottest()\0gzmTihuanslot()\0"
    "NoChangeFun()\0OneSecondOff()\0myscroll()\0"
    "date\0on_dateTimeEdit_dateTimeChanged(QDateTime)\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->on_listWidget_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->timeoutfun(); break;
        case 2: _t->xiaoshiji(); break;
        case 3: _t->Licheng(); break;
        case 4: _t->shanhua(); break;
        case 5: _t->gzmslottest(); break;
        case 6: _t->gzmTihuanslot(); break;
        case 7: _t->NoChangeFun(); break;
        case 8: _t->OneSecondOff(); break;
        case 9: _t->myscroll(); break;
        case 10: _t->on_dateTimeEdit_dateTimeChanged((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget,
      qt_meta_data_Widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Widget))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
