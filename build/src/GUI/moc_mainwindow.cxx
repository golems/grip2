/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Feb 5 17:32:21 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/GUI/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      19,   11,   11,   11, 0x08,
      31,   11,   11,   11, 0x08,
      43,   11,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      58,   11,   11,   11, 0x08,
      66,   11,   11,   11, 0x08,
      72,   11,   11,   11, 0x08,
      79,   11,   11,   11, 0x08,
      97,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     135,   11,   11,   11, 0x08,
     160,   11,   11,   11, 0x08,
     168,   11,   11,   11, 0x08,
     176,   11,   11,   11, 0x08,
     190,   11,   11,   11, 0x08,
     204,   11,   11,   11, 0x08,
     217,   11,   11,   11, 0x08,
     230,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0load()\0quickLoad()\0"
    "saveScene()\0close()\0exit()\0front()\0"
    "top()\0side()\0startSimulation()\0"
    "stopSimulation()\0simulateSingleStep()\0"
    "renderDuringSimulation()\0white()\0"
    "black()\0resetCamera()\0xga1024x768()\0"
    "vga640x480()\0hd1280x720()\0about()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->load(); break;
        case 1: _t->quickLoad(); break;
        case 2: _t->saveScene(); break;
        case 3: _t->close(); break;
        case 4: _t->exit(); break;
        case 5: _t->front(); break;
        case 6: _t->top(); break;
        case 7: _t->side(); break;
        case 8: _t->startSimulation(); break;
        case 9: _t->stopSimulation(); break;
        case 10: _t->simulateSingleStep(); break;
        case 11: _t->renderDuringSimulation(); break;
        case 12: _t->white(); break;
        case 13: _t->black(); break;
        case 14: _t->resetCamera(); break;
        case 15: _t->xga1024x768(); break;
        case 16: _t->vga640x480(); break;
        case 17: _t->hd1280x720(); break;
        case 18: _t->about(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
