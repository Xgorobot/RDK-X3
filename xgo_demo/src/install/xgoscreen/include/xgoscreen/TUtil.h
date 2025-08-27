#ifndef XGOSCREEN_TUTIL_H
#define XGOSCREEN_TUTIL_H

#include <QString>
#include <QDateTime>
#include <QDir>

class TUtil
{
public:
    static QString getTime();
    static QString getTimeMS();

    static QString getCurrentPath();
    static QString getApplicationPath();
};

#endif // XGOSCREEN_TUTIL_H