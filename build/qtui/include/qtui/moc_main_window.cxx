/****************************************************************************
** Meta object code from reading C++ file 'main_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/qtui/include/qtui/main_window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qtui__MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   44,   44,   44, 0x0a,
      45,   77,   44,   44, 0x0a,
      83,   44,   44,   44, 0x0a,
     114,   44,   44,   44, 0x0a,
     147,  193,   44,   44, 0x0a,
     199,   44,   44,   44, 0x0a,
     230,   44,   44,   44, 0x0a,
     264,   44,   44,   44, 0x0a,
     298,   44,   44,   44, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qtui__MainWindow[] = {
    "qtui::MainWindow\0on_actionAbout_triggered()\0"
    "\0on_button_connect_clicked(bool)\0check\0"
    "on_button_connectRos_clicked()\0"
    "on_button_manualActute_clicked()\0"
    "on_checkbox_use_environment_stateChanged(int)\0"
    "state\0on_button_writeStart_clicked()\0"
    "on_button_radioButton_1_clicked()\0"
    "on_button_radioButton_2_clicked()\0"
    "updateLoggingView()\0"
};

void qtui::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_actionAbout_triggered(); break;
        case 1: _t->on_button_connect_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_button_connectRos_clicked(); break;
        case 3: _t->on_button_manualActute_clicked(); break;
        case 4: _t->on_checkbox_use_environment_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_button_writeStart_clicked(); break;
        case 6: _t->on_button_radioButton_1_clicked(); break;
        case 7: _t->on_button_radioButton_2_clicked(); break;
        case 8: _t->updateLoggingView(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qtui::MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qtui::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qtui__MainWindow,
      qt_meta_data_qtui__MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qtui::MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qtui::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qtui::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qtui__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qtui::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
