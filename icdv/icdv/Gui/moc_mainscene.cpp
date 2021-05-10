/****************************************************************************
** Meta object code from reading C++ file 'mainscene.h'
**
** Created: Sat Apr 23 09:52:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainScene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   10,   11,   10, 0x0a,
      27,   10,   11,   10, 0x0a,
      34,   10,   11,   10, 0x0a,
      44,   10,   11,   10, 0x0a,
      54,   10,   10,   10, 0x0a,
      63,   10,   10,   10, 0x0a,
      73,   10,   10,   10, 0x0a,
     105,   96,   11,   10, 0x0a,
     126,  119,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainScene[] = {
    "MainScene\0\0bool\0LoadDump()\0Save()\0"
    "Authors()\0Version()\0ZoomIn()\0ZoomOut()\0"
    "SetLayoutIteratrions()\0iconSize\0"
    "Resize(QSize)\0resize\0resizeEvent(QResizeEvent*)\0"
};

const QMetaObject MainScene::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainScene,
      qt_meta_data_MainScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainScene))
        return static_cast<void*>(const_cast< MainScene*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = LoadDump();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = Save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = Authors();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = Version();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: ZoomIn(); break;
        case 5: ZoomOut(); break;
        case 6: SetLayoutIteratrions(); break;
        case 7: { bool _r = Resize((*reinterpret_cast< const QSize(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
