#include "batchjob.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    BatchJob bj;
    bj.start();
} //endfunction main
