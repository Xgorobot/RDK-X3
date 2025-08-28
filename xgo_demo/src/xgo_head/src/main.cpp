#include "mainwindow.h"
#include <xgoscreen/TLog.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TLog::init(TUtil::getApplicationPath() + "/log/log.txt");

    MainWindow w;
    w.show();

    return a.exec();
}
