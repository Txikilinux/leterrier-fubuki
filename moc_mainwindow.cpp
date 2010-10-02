/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sat Oct 2 16:15:22 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      36,   11,   11,   11, 0x08,
      57,   11,   11,   11, 0x08,
      92,   11,   11,   11, 0x08,
     121,   11,   11,   11, 0x08,
     151,  145,   11,   11, 0x08,
     179,  145,   11,   11, 0x08,
     208,   11,   11,   11, 0x08,
     233,   11,   11,   11, 0x08,
     255,   11,   11,   11, 0x08,
     277,   11,   11,   11, 0x08,
     299,   11,   11,   11, 0x08,
     321,   11,   11,   11, 0x08,
     343,   11,   11,   11, 0x08,
     365,   11,   11,   11, 0x08,
     387,   11,   11,   11, 0x08,
     409,   11,   11,   11, 0x08,
     433,  431,   11,   11, 0x08,
     447,   11,   11,   11, 0x08,
     469,   11,   11,   11, 0x08,
     491,   11,   11,   11, 0x08,
     513,   11,   11,   11, 0x08,
     535,   11,   11,   11, 0x08,
     557,   11,   11,   11, 0x08,
     579,   11,   11,   11, 0x08,
     601,   11,   11,   11, 0x08,
     623,   11,   11,   11, 0x08,
     645,  431,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_btnNouveau_clicked()\0"
    "on_btnAide_clicked()\0"
    "on_actionAide_en_local_triggered()\0"
    "on_action_propos_triggered()\0"
    "on_btnCorrige_clicked()\0index\0"
    "on_cBoxSuite_activated(int)\0"
    "on_cBoxNiveau_activated(int)\0"
    "on_btnVerifier_clicked()\0on_btnCase0_clicked()\0"
    "on_btnCase1_clicked()\0on_btnCase2_clicked()\0"
    "on_btnCase3_clicked()\0on_btnCase4_clicked()\0"
    "on_btnCase5_clicked()\0on_btnCase6_clicked()\0"
    "on_btnCase7_clicked()\0on_btnCase8_clicked()\0"
    "i\0_btnCase(int)\0on_btnNbre0_clicked()\0"
    "on_btnNbre1_clicked()\0on_btnNbre2_clicked()\0"
    "on_btnNbre3_clicked()\0on_btnNbre4_clicked()\0"
    "on_btnNbre5_clicked()\0on_btnNbre6_clicked()\0"
    "on_btnNbre7_clicked()\0on_btnNbre8_clicked()\0"
    "_btnNbre(int)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
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
        switch (_id) {
        case 0: on_btnNouveau_clicked(); break;
        case 1: on_btnAide_clicked(); break;
        case 2: on_actionAide_en_local_triggered(); break;
        case 3: on_action_propos_triggered(); break;
        case 4: on_btnCorrige_clicked(); break;
        case 5: on_cBoxSuite_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_cBoxNiveau_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: on_btnVerifier_clicked(); break;
        case 8: on_btnCase0_clicked(); break;
        case 9: on_btnCase1_clicked(); break;
        case 10: on_btnCase2_clicked(); break;
        case 11: on_btnCase3_clicked(); break;
        case 12: on_btnCase4_clicked(); break;
        case 13: on_btnCase5_clicked(); break;
        case 14: on_btnCase6_clicked(); break;
        case 15: on_btnCase7_clicked(); break;
        case 16: on_btnCase8_clicked(); break;
        case 17: _btnCase((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: on_btnNbre0_clicked(); break;
        case 19: on_btnNbre1_clicked(); break;
        case 20: on_btnNbre2_clicked(); break;
        case 21: on_btnNbre3_clicked(); break;
        case 22: on_btnNbre4_clicked(); break;
        case 23: on_btnNbre5_clicked(); break;
        case 24: on_btnNbre6_clicked(); break;
        case 25: on_btnNbre7_clicked(); break;
        case 26: on_btnNbre8_clicked(); break;
        case 27: _btnNbre((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
