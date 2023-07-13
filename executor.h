#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QObject>
#include <QProcess>
#include <batchjob.h>

class Executor : public QObject {
    Q_OBJECT
public:
    explicit Executor(QObject *parent = 0);

signals:

public slots:
    QString getColorSpace(QString);
    QString getColorProfile(QString);
    void convertCopy(QString, QString, QString, QString, QString, int, int);
    void convertCrop(QString, QString, int, int, int, int);
    void convertRotate(QString, QString, int);
    void toFormat(QString, QString);
    double mm2PX(double, double);
    QStringList getDPI(QString);
};

#endif // EXECUTOR_H
