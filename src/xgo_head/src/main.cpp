#include "mainwindow.h"
#include "TLog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TLog::init(TUtil::getApplicationPath() + "/log/log.txt");
    TLog::i("version 20250801_1611");

    MainWindow w;
    w.show();
    // TLog::i("version 20250415_1611");

    return a.exec();
}
