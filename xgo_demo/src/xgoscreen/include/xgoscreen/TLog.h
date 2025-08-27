#ifndef XGOSCREEN_TLOG_H
#define XGOSCREEN_TLOG_H

#include <QMutex>
#include <QFile>
#include <xgoscreen/TUtil.h>

class TLog
{
public:
    const static int level_debug = 1;
    const static int level_info = 2;
    const static int level_warning = 3;
    const static int level_error = 4;
    const static int level_important = 5;

    static void init(QString fileNamePath, long maxSize = 10 * 1024 * 1024);
    static void setLevel(int level = 0);

    static void d(QString txt);
    static void i(QString txt);
    static void w(QString txt);
    static void e(QString txt);
    static void ii(QString txt);

private:
    static QFile* sFilePointer;
    static long sMaxSize;
    static int sLevel;

    static QMutex* sMutex;
    static void writeFile(QString txt);
};

#endif // XGOSCREEN_TLOG_H