#include "batchjob.h"

BatchJob::BatchJob(QObject *parent) : QObject(parent) {
    this->config[0] = "localhost";  // db-server hostname
    this->config[1] = "mainsql1";       // database name
    this->config[2] = "mainsql1";       // mysql user
    this->config[3] = "mysecretpass";   // mysql password

    /**this->config[0] = "localhost";  // db-server hostname
    this->config[1] = "test";       // database name
    this->config[2] = "root";       // mysql user
    this->config[3] = "hihaysod";   // mysql password**/
} //endconstructor

QTextStream BatchJob::qout(stdout, QIODevice::WriteOnly);

void BatchJob::quitApp() {
    removeLock();
    removeTemp();
    exit(0);
} //endfunction quitApp

void BatchJob::removeTemp() {
    QProcess delfiles;
    delfiles.start("rm -R ./temp");
    delfiles.waitForFinished();
} //endfunction removeTemp

const std::string BatchJob::currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X: ", &tstruct);

    return buf;
} //endfunction currentDateTime

void BatchJob::printMsg(const char* msg) {
    qout << currentDateTime().c_str() << msg << endl;
} //endfunction printMsg

void BatchJob::createLock() {
    QString outputFilename = "./lock.lck";
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);

    if (!outputFile.isOpen()) {
        printMsg("Unable to write lockfile. Exiting.");
        exit(0);
    } //endif

    QTextStream outStream(&outputFile);
    QString id = QString::number(QCoreApplication::applicationPid());
    outStream << id.toStdString().c_str() << endl;

    outputFile.close();
} //endfunction createLock

void BatchJob::removeLock() {
    QFile file("./lock.lck");
    if (file.exists()) {
        QFile::remove("./lock.lck");
    } //endif
} //endfunction removeLock

bool BatchJob::isLocked() {
    QFile file("./lock.lck");
    if (file.exists()) {
        return true;
    } else {
        return false;
    } //endif
} //endfunction isLocked

QString BatchJob::getNewFilename(QList<QString> entry) {
    bool isfirst = entry[3].contains("start{===}1", Qt::CaseSensitive);
    QString filenameNew;
    QStringList ql = entry[2].split("/");

    if (isfirst) {
        filenameNew = "./temp/" + entry[7] + "/" + ql[1] + entry[1] + "_conf.jpg";
    } else if (entry[2].contains("conf", Qt::CaseSensitive)){
        filenameNew = ql[1];
    } //endif

    return filenameNew;
} //endfunction getNewFilename

void BatchJob::cropImage(const QList<QString> &entry) {
    int x;
    int y;
    int w;
    int h;
    int cw;
    int ch;

    QStringList list = QString(entry[3]).split("{;;;}");

    QStringList xv = list[3].split("{===}");
    x = xv[1].toInt();

    QStringList yv = list[4].split("{===}");
    y = yv[1].toInt();

    QStringList hv = list[5].split("{===}");
    h = hv[1].toInt();

    QStringList wv = list[6].split("{===}");
    w = wv[1].toInt();

    QStringList cwv = list[7].split("{===}");
    cw = cwv[1].toInt();

    QStringList chv = list[8].split("{===}");
    ch = chv[1].toInt();

    QStringList l = entry[2].split("/");

    QString filename("./temp/" + entry[7] + "/" + l[1]);

    QImageReader reader(filename);
    QImage image = reader.read();
    int realW = w * (image.width() / cw);
    int realH = h * (image.height() / ch);
    int realX = x * (image.width() / cw);
    int realY = y * (image.height() / ch);

    QString filenameNew = getNewFilename(entry);

    Executor *exec = new Executor();
    exec->convertCrop(filename, filenameNew, realX, realY, realW, realH);
} //endfunction cropImage

void BatchJob::rotateImage(const QList<QString> &entry) {
    QStringList list = entry[3].split("{;;;}");
    QStringList aval = list[3].split("{===}");
    int angle = aval[1].toInt();

    QStringList l = entry[2].split("/");
    QString filename = "./temp/" + entry[7] + "/" + l[1];

    QString filenameNew = getNewFilename(entry);

    Executor *exec = new Executor();
    exec->convertRotate(filename, filenameNew, angle);
} //endfunction rotateImage

void BatchJob::evenImage(const QList<QString> &entry) {
    QStringList l = QString(entry[2]).split("/");
    QString filename = "./temp/" + entry[7] + "/" + l[1];
    QImageReader reader(filename);
    QImage image = reader.read();
    QStringList list = entry[3].split("{;;;}");
    QStringList aval = list[3].split("{===}");

    double angle = aval[1].toInt();
    if (angle > 90) {
        angle = 360 - angle;
    } //endif
    angle = (double)(90 - angle) / (double)180 * M_PI;
    double tanres = tan(angle);

    double b;
    if (image.width() > image.height()) {
        b = image.width() / tanres;
    } else {
        b = image.height() / tanres;
    } //endif

    int newWidth = (image.width() - b);
    int newHeight = (image.height() - b);

    QString filenameNew = getNewFilename(entry);

    Executor *exec = new Executor();
    exec->convertCrop(filename, filenameNew, b, b, newWidth, newHeight);
} //endfunction evenImage

void BatchJob::copyImage(const QList<QString> &entry) {
    QStringList l = QString(entry[3]).split("{;;;}");

    QStringList fnl = l[0].split("{===}");
    QStringList wl = l[1].split("{===}");
    QStringList hl = l[2].split("{===}");

    QStringList srcf = QString(entry[2]).split("/");

    QString destfname("./" + fnl[1]);
    QString srcfname("./temp/" + entry[7] + "/" + srcf[1]);

    Executor *exec = new Executor();
    QString cspace = exec->getColorSpace(srcfname);

    QString iccp = exec->getColorProfile(cspace);

    int mirror = entry[4].toInt();
    int dpi = entry[5].toInt();
    exec->convertCopy(srcfname, iccp, destfname, wl[1], hl[1], mirror, dpi);
} //endfunction copyImage

void BatchJob::dummyAction(const QList<QString> &entry) {
    QStringList l = QString(entry[2]).split("/");
    QString srcfname = "./temp/" + entry[7] + "/" + l[1];
    QString destfname = getNewFilename(entry);

    Executor *exec = new Executor();
    exec->toFormat(srcfname, destfname);
} //endfunction dummyAction

void executeJoblist(const QList<QList<QString> > &list) {
    BatchJob bj;

    QDir().mkdir("./temp/");
    QDir().mkdir(QString("./temp/" + list[0][7]) );
    QStringList cl = QString(list[0][2]).split("/");

    QFile::copy(QString("./" + cl[1]), QString("./temp/" + list[0][7] + "/" + cl[1]) );

    QList<QString> row;

    foreach (row, list) {
        if (row[6].toInt() == 0) {
            bj.cropImage(row);
            QString msg("Executing database row with ID #" + row[0] + ": cropping image...");
            BatchJob::qout << bj.currentDateTime().c_str() << msg.toStdString().c_str() << endl;
        } else if (row[6].toInt() == 1) {
            bj.rotateImage(row);
            QString msg("Executing database row with ID #" + row[0] + ": rotating image...");
            BatchJob::qout << bj.currentDateTime().c_str() << msg.toStdString().c_str() << endl;
        } else if (row[6].toInt() == 2) {
            bj.evenImage(row);
            QString msg("Executing database row with ID #" + row[0] + ": evening image...");
            BatchJob::qout << bj.currentDateTime().c_str() << msg.toStdString().c_str() << endl;
        } else if (row[6].toInt() == 3) {
            bj.copyImage(row);
            QString msg("Executing database row with ID #" + row[0] + ": copying image...");
            BatchJob::qout << bj.currentDateTime().c_str() << msg.toStdString().c_str() << endl;
        } else if (row[6].toInt() == 4) {
            bj.dummyAction(row);
            QString msg("Executing database row with ID #" + row[0] + ": moving image for color conversion...");
            BatchJob::qout << bj.currentDateTime().c_str() << msg.toStdString().c_str() << endl;
        } //endif
    } //endforeach
} //endmethod executeJobs

void BatchJob::start() {
    if (isLocked()) {
        printMsg("Lockfile detected, exiting...");
        exit(0);
    } else {
        createLock();
    } //endif

    this->db = QSqlDatabase::addDatabase("QMYSQL");

    int cores = QThread::idealThreadCount();
    bool multicore = true;
    if (cores < 2) {
        multicore = false;
        cores = 1;
    } else {
        QThreadPool::globalInstance()->setMaxThreadCount(cores);
    } //endif

    printMsg("############# BATCH IMAGE QUEUE (BIQ) v0.1 #############");
    printMsg("Starting weapons and defence systems...");
    if (multicore) {
        printMsg(QString("Multiple CPU cores detected: " + QString::number(cores)).toStdString().c_str());
        printMsg("We are going to utilize all of them for image rendering.");
    } else {
        printMsg("Single core CPU, running in small penis mode.");
    } //endif

    connect();

    printMsg("Retrieving Data...");
    QList<QList<QString> > table = getTable();

    if (table.isEmpty()) {
        printMsg("The queue was empty, exiting.");
        quitApp();
    } //endif
    printMsg("Generating joblists...\n");
    QList<QList<QList<QString> > > cpuq;
    int count = 0;
    while (!table.isEmpty()) {
        // BEGINNE VERARBEITUNG
        QList<QList<QString> > joblist = getJoblist(table, count);
        int isdel = QString::compare(joblist[0][0], QString("delete"), Qt::CaseSensitive);
        if (isdel == 0) {
            int start = joblist[0][1].toInt();
            int end = joblist[0][2].toInt();
            QList<QList<QString> > dellist;
            for (int i=start; i<=end; i++) {
                dellist.append(QList<QString>(table[i]));
            } //endfor
            printMsg("Found incomplete job: not generating joblist and deleting from database...\n");
            deleteJobs(dellist);
        } else {
            printMsg(QString("########################### JOBLIST, ID: " + QString::number(count) + " ############################").toStdString().c_str());
            printMsg("# ACTIONS: 0=crop, 1=rotate, 2=even, 3=copy, 4=colorconversion ########");
            printMsg("#######################################################################");
            QList<QString> row;
            for (int i=0; i<joblist.size(); i++) {
                row = joblist[i];
                printMsg(QString("ID:\t\t" + row[0]).toStdString().c_str());
                printMsg(QString("Action:\t" + row[6]).toStdString().c_str());
                if (i != joblist.size()-1) {
                    printMsg("----------------------------------------------------");
                } //endif
            } //endforeach
            printMsg("####################################################\n");

            if (multicore) {
                cpuq.append(joblist);
            } else {
                printMsg("Executing joblist...\n");
                executeJoblist(joblist);
            } //endif

            deleteJobs(joblist);
            count++;
        } //endif

        table = getTable();
        // ENDE DER VERARBEITUNG
    } //endwhile

    if (multicore) {
        QString msg("We have generated " + QString::number(cpuq.size()) + " joblists.");
        printMsg(msg.toStdString().c_str());
        printMsg("Injecting joblists into process scheduler and starting CPU workers...\n");
        printMsg("LAUNCHING WEAPON OF MASS COMPUTATION:");
        QFutureWatcher<void> mywatch;
        QFuture<void> proc = QtConcurrent::map(cpuq, &executeJoblist);
        mywatch.setFuture(proc);
        mywatch.waitForFinished();
    } //endif

    printMsg("##### Execution finished. We are done! #####");
    printMsg("Deleting temporary files... ");
    printMsg("Exiting...");

    if (this->db.isOpen()) {
        this->db.close();
    } //endif

    quitApp();
} //endfunction start

void BatchJob::connect() {
    printMsg("Connecting to database...");

    db.setHostName(config[0]);
    db.setDatabaseName(config[1]);
    db.setUserName(config[2]);
    db.setPassword(config[3]);

    bool ok = db.open();

    if (!ok) {
        printMsg("ERROR: Couldn't connect to database, exiting.");
        quitApp();
    } //endif
} //endfunction connect

QList<QList<QString> > BatchJob::getTable() {
    QList<QList<QString> > tab;

    QSqlQuery query(QString("SELECT * FROM imagequeue"));

    if ( !query.isActive() ) {
        printMsg("ERROR: Could not execute the query.");
    } else {
            while (query.next()) {
                QList<QString> row;
                row.append(query.value(0).toString());
                row.append(query.value(1).toString());
                row.append(query.value(2).toString());
                row.append(query.value(3).toString());
                row.append(query.value(4).toString());
                row.append(query.value(5).toString());
                tab.append(row);
            } //endwhile
        } //endif

    return tab;
} //endfunction getTable

QList<QList<QString> > BatchJob::getJoblist(QList<QList<QString> > table, int count) {
    QList<QList<QString> > joblist;
    bool isvalid = true;
    int start = 0;
    int end = -1;

    if (!(table.size() > 1)) {
        isvalid = false;
        end = 0;
    } else {
        if (table[1][3].contains("start{===}0", Qt::CaseSensitive)) {
            for (int i=1; i<table.size(); i++) {
                if (table[i][3].contains("start{===}1")) {
                    isvalid = false;
                    end = i-1;
                    break;
                } else if (table[i][3].contains("finish{===}1")) {
                    end = i;
                    break;
                } //endif
            } //endfor
            if (end == -1) {
                isvalid = false;
                end = 0;
            } //endif
        } else {
            if (!table[1][3].contains("finish{===}1", Qt::CaseSensitive)) {
                isvalid = false;
                end = 0;
            } else {
                end = 1;
            } //endif
        } //endif
    } //endif

    // Wir haben eine gültige Jobliste
    if (isvalid) {
        QList<QString> row;
        int type = -1;
        for (int i=start; i<=end; i++) {
            row = table[i];
            if (row[3].contains("finish")) {
                row.append("3");
            } else {
                type = getType(row[3]);
                if (type == 0) {
                    row.append("0");
                } else if (type == 1) {
                    row.append("1");
                } else if (type == 4) {
                    row.append("4");
                } else {
                    row.append("2");
                } //endif
           } //endif
           row.append(QString::number(count));

           joblist.append(row);
        } //endfor
    // Wir haben keine gültige Jobliste
    } else {
        QList<QString> li;
        li.append("delete");
        li.append(QString::number(start));
        li.append(QString::number(end));
        joblist.append(li);
    } //endif

    return joblist;
} //endfunction getJoblist

int BatchJob::getType(QString params) {
    int retVal = -1;

    bool hasrotate = params.contains("angle", Qt::CaseSensitive);
    bool haseven = params.contains("begradigen", Qt::CaseSensitive);
    bool hasdummy = params.contains("dummy", Qt::CaseSensitive);

    if (hasrotate && haseven) {
        retVal = 2;
    } else if (hasrotate && !haseven) {
        retVal = 1;
    } else if (!hasrotate && !haseven && !hasdummy){
        retVal = 0;
    } else if (hasdummy) {
        retVal = 4;
    } //endif

    return retVal;
} //endfunction getType

void BatchJob::deleteJobs(QList<QList<QString> > joblist) {
    QString sql = "DELETE FROM imagequeue WHERE id IN (";

    for (int i=0; i<joblist.size(); i++) {
        sql += joblist[i][0];
        if (i != (joblist.size() - 1)) {
            sql += ",";
        } //endif
    } //endfor

    sql += ")";

    QSqlQuery query;
    query.exec(sql);
} //endfunction deleteJobs
