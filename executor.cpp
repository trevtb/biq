#include "executor.h"

Executor::Executor(QObject *parent) : QObject(parent) {
} //endconstructor

QString Executor::getColorSpace(QString filename) {
    QString cmd = "identify";
    QStringList arguments;
    arguments << "-verbose" << filename;

    QProcess proc;
    proc.start(cmd, arguments);
    proc.waitForFinished(-1);

    QString p_stdout = proc.readAllStandardOutput();
    QStringList sList = p_stdout.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);

    QString retVal;
    for (int i=0; i<sList.size(); i++) {
        if (sList[i].contains("Colorspace")) {
            QString tempVal = sList[i].simplified();
            tempVal = tempVal.replace(" ", "");
            QStringList tl = tempVal.split(":");
            retVal = tl[1];
        } //endif
    } //endfor

    return retVal;
} //endfunction getColorSpace

QString Executor::getColorProfile(QString colorspace) {
    QString iccname = "none";
    if (QString::compare(colorspace, "CMYK", Qt::CaseInsensitive) == 0) {
        iccname = "ISOcoated_v2_300_eci.icc";
    } else if (QString::compare(colorspace, "sRGB", Qt::CaseInsensitive) == 0 || QString::compare(colorspace, "RGB", Qt::CaseInsensitive) == 0) {
        iccname = "eciRGB_v2.icc";
    } //endif

    return iccname;
} //endfunction getColorProfile

void Executor::convertCopy(QString filename, QString profile, QString destination, QString width, QString height, int mirror, int targetdpi) {
    width = width;
    height = height;
    QString cmd = "convert";

    // INTERPOLATION
    //QStringList argsdensity;
    //argsdensity << filename << "-strip" << "-density" << QString::number(targetdpi) << destination;
    //QProcess densityproc;
    //densityproc.start(cmd, argsdensity);
    //densityproc.waitForFinished(-1);

    QStringList dpi = getDPI(filename);

    double dw = width.toDouble() * 10.0;
    double dh = height.toDouble() * 10.0;

    dw = mm2PX(dw, dpi[0].toDouble());
    dh = mm2PX(dh, dpi[1].toDouble());

    QString size = QString::number(int(dw + 0.5)) + "x" + QString::number(int(dh + 0.5)) + "!";

    QString profpath = "./profiles/" + profile;
    QString proftargetpath = "./profiles/target_profile.icc";
    QStringList arguments;

    //MIT RESIZE
    arguments << filename << "-resize" << size <<"-profile" << profpath << "-profile" << proftargetpath << destination;
    // OHNE RESIZE
    //arguments << destination <<"-profile" << profpath << "-profile" << proftargetpath << finaldest;
    QProcess proc;
    proc.start(cmd, arguments);
    proc.waitForFinished(-1);

    if (mirror == 1) {
        int bx = mm2PX(30.0, dpi[0].toDouble());
        int by = mm2PX(30.0, dpi[1].toDouble());

        QString sizearg = QString::number(int(dw + 0.5)+(2*bx)) + "x" + QString::number(int(dh + 0.5)+(2*by))+"-"+QString::number(bx)+"-"+QString::number(by);
        QProcess proc2;
        QStringList argsnew;
        argsnew << destination << "-set" << "option:distort:viewport" << sizearg << "-virtual-pixel" << "Mirror" << "-filter" << "point" << "-distort" << "SRT" << "0" << destination;

        proc2.start(cmd, argsnew);
        proc2.waitForFinished(-1);
    } //endif
} //endfunction convertCopy

void Executor::convertCrop(QString filename, QString destination, int x, int y, int w, int h) {
    QString cmd = "convert";
    QString geo = QString::number(w) + "x" + QString::number(h);
    if (x < 0) {
        geo += QString::number(x);
    } else {
        geo += "+" + QString::number(x);
    } //endif

    if (y < 0) {
        geo += QString::number(y);
    } else {
        geo += "+" + QString::number(y);
    } //endif

    QStringList arguments;
    arguments << filename << "-crop" << geo << destination;

    QProcess proc;
    proc.start(cmd, arguments);
    proc.waitForFinished(-1);
} //endfunction convertCrop

void Executor::convertRotate(QString filename, QString destination, int angle) {
    QString cmd = "convert";
    QStringList arguments;
    arguments << filename << "-rotate" << QString::number(angle) << destination;

    QProcess proc;
    proc.start(cmd, arguments);
    proc.waitForFinished(-1);
} //endfunction convertRotate

void Executor::toFormat(QString source, QString destination) {
    QString cmd = "convert";
    QStringList arguments;
    arguments << source << destination;

    QProcess proc;
    proc.start(cmd, arguments);
    proc.waitForFinished(-1);
} //endfunction toFormat

double Executor::mm2PX(double mm, double dpi) {
    double ctinch = 25.4;
    double retVal = 0.0;
    retVal = (dpi * mm) / ctinch;

    return retVal;
} //endfunction mm2PX

QStringList Executor::getDPI(QString filename) {
    QString cmdf = "identify";
    QStringList argumentsf;
    argumentsf << "-format" << "\"%x x %y\"" << filename;

    QProcess procf;
    procf.start(cmdf, argumentsf);
    procf.waitForFinished(-1);

    QString p_stdout = procf.readAllStandardOutput();
    QStringList sList = p_stdout.split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);

    QString dpivalx;
    QString dpivaly;
    for (int i=0; i<sList.size(); i++) {
        if (sList[i].contains("PixelsPerInch")) {
            QString tempVal = sList[i].simplified();
            QStringList tl = tempVal.split(" ");
            dpivalx = tl[0];
            dpivalx.remove("\"");
            dpivaly = tl[3];
        } //endif
    } //endfor

    QStringList retVal;
    retVal << dpivalx << dpivaly;

    return retVal;
} //endfunction getDPI
