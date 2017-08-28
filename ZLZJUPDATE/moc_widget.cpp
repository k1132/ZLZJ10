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
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      32,    7,    7,    7, 0x08,
      58,    7,    7,    7, 0x08,
      84,    7,    7,    7, 0x08,
     110,    7,    7,    7, 0x08,
     136,    7,    7,    7, 0x08,
     162,    7,    7,    7, 0x08,
     174,    7,    7,    7, 0x08,
     184,    7,    7,    7, 0x08,
     194,    7,    7,    7, 0x08,
     206,    7,    7,    7, 0x08,
     220,    7,    7,    7, 0x08,
     252,  246,    7,    7, 0x08,
     279,    7,    7,    7, 0x08,
     305,    7,    7,    7, 0x08,
     337,  332,    7,    7, 0x08,
     380,    7,    7,    7, 0x08,
     407,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0on_pushButton_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_8_clicked()\0xiaoshiji()\0"
    "shanhua()\0Licheng()\0slotmouse()\0"
    "gzmslottest()\0on_pushButton_3_clicked()\0"
    "index\0on_comboBox_activated(int)\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_10_clicked()\0date\0"
    "on_dateTimeEdit_dateTimeChanged(QDateTime)\0"
    "on_pushButton_11_clicked()\0"
    "on_pushButton_12_clicked()\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_5_clicked(); break;
        case 2: _t->on_pushButton_6_clicked(); break;
        case 3: _t->on_pushButton_7_clicked(); break;
        case 4: _t->on_pushButton_2_clicked(); break;
        case 5: _t->on_pushButton_8_clicked(); break;
        case 6: _t->xiaoshiji(); break;
        case 7: _t->shanhua(); break;
        case 8: _t->Licheng(); break;
        case 9: _t->slotmouse(); break;
        case 10: _t->gzmslottest(); break;
        case 11: _t->on_pushButton_3_clicked(); break;
        case 12: _t->on_comboBox_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_pushButton_9_clicked(); break;
        case 14: _t->on_pushButton_10_clicked(); break;
        case 15: _t->on_dateTimeEdit_dateTimeChanged((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        case 16: _t->on_pushButton_11_clicked(); break;
        case 17: _t->on_pushButton_12_clicked(); break;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
