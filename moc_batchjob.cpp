/****************************************************************************
** Meta object code from reading C++ file 'batchjob.h'
**
** Created: Tue Nov 26 15:15:49 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "batchjob.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'batchjob.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BatchJob[] = {

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
      22,    9,   10,    9, 0x0a,
      44,   40,    9,    9, 0x0a,
      80,   74,   66,    9, 0x0a,
     111,   74,    9,    9, 0x0a,
     137,   74,    9,    9, 0x0a,
     165,   74,    9,    9, 0x0a,
     191,   74,    9,    9, 0x0a,
     217,    9,    9,    9, 0x0a,
     225,    9,    9,    9, 0x0a,
     235,    9,    9,    9, 0x0a,
     248,    9,    9,    9, 0x08,
     281,    9,  258,    9, 0x08,
     304,  292,  258,    9, 0x08,
     351,  343,    9,    9, 0x08,
     397,  390,  386,    9, 0x08,
     414,    9,    9,    9, 0x08,
     427,    9,    9,    9, 0x08,
     445,    9,  440,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BatchJob[] = {
    "BatchJob\0\0std::string\0currentDateTime()\0"
    "msg\0printMsg(const char*)\0QString\0"
    "entry\0getNewFilename(QList<QString>)\0"
    "cropImage(QList<QString>)\0"
    "rotateImage(QList<QString>)\0"
    "evenImage(QList<QString>)\0"
    "copyImage(QList<QString>)\0start()\0"
    "quitApp()\0removeTemp()\0connect()\0"
    "QList<QList<QString> >\0getTable()\0"
    "table,count\0getJoblist(QList<QList<QString> >,int)\0"
    "joblist\0deleteJobs(QList<QList<QString> >)\0"
    "int\0params\0getType(QString)\0createLock()\0"
    "removeLock()\0bool\0isLocked()\0"
};

void BatchJob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BatchJob *_t = static_cast<BatchJob *>(_o);
        switch (_id) {
        case 0: { std::string _r = _t->currentDateTime();
            if (_a[0]) *reinterpret_cast< std::string*>(_a[0]) = _r; }  break;
        case 1: _t->printMsg((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 2: { QString _r = _t->getNewFilename((*reinterpret_cast< QList<QString>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: _t->cropImage((*reinterpret_cast< const QList<QString>(*)>(_a[1]))); break;
        case 4: _t->rotateImage((*reinterpret_cast< const QList<QString>(*)>(_a[1]))); break;
        case 5: _t->evenImage((*reinterpret_cast< const QList<QString>(*)>(_a[1]))); break;
        case 6: _t->copyImage((*reinterpret_cast< const QList<QString>(*)>(_a[1]))); break;
        case 7: _t->start(); break;
        case 8: _t->quitApp(); break;
        case 9: _t->removeTemp(); break;
        case 10: _t->connect(); break;
        case 11: { QList<QList<QString> > _r = _t->getTable();
            if (_a[0]) *reinterpret_cast< QList<QList<QString> >*>(_a[0]) = _r; }  break;
        case 12: { QList<QList<QString> > _r = _t->getJoblist((*reinterpret_cast< QList<QList<QString> >(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QList<QList<QString> >*>(_a[0]) = _r; }  break;
        case 13: _t->deleteJobs((*reinterpret_cast< QList<QList<QString> >(*)>(_a[1]))); break;
        case 14: { int _r = _t->getType((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: _t->createLock(); break;
        case 16: _t->removeLock(); break;
        case 17: { bool _r = _t->isLocked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BatchJob::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BatchJob::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BatchJob,
      qt_meta_data_BatchJob, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BatchJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BatchJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BatchJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BatchJob))
        return static_cast<void*>(const_cast< BatchJob*>(this));
    return QObject::qt_metacast(_clname);
}

int BatchJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
