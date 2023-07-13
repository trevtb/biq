#ifndef BATCHJOB_H
#define BATCHJOB_H

#include "executor.h"
#include <QCoreApplication>
#include <QObject>
#include <QTextStream>
#include <iostream>
#include <QSqlDatabase>
#include <QList>
#include <QStringList>
#include <QImage>
#include <QImageReader>
#include <qmath.h>
#include <QSqlQuery>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QThread>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <tiffio.h>
#include <QProcess>

class BatchJob : public QObject {
    Q_OBJECT
public:
    explicit BatchJob(QObject *parent = 0);

private:
    QSqlDatabase db;

public:
    const char* config[];
    static QTextStream qout;

public slots:
    const std::string currentDateTime();
    void printMsg(const char* msg);
    QString getNewFilename(QList<QString> entry);
    void cropImage(const QList<QString> &entry);
    void rotateImage(const QList<QString> &entry);
    void evenImage(const QList<QString> &entry);
    void copyImage(const QList<QString> &entry);
    void dummyAction(const QList<QString> &entry);
    void start();
    void quitApp();
    void removeTemp();

private slots:
    void connect();
    QList<QList<QString> > getTable();
    QList<QList<QString> > getJoblist(QList<QList<QString> > table, int count);
    void deleteJobs(QList<QList<QString> > joblist);
    int getType(QString params);
    void createLock();
    void removeLock();
    bool isLocked();
};

#endif // BATCHJOB_H
