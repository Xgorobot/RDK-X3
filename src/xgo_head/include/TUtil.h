#ifndef TUTIL_H
#define TUTIL_H

#include <QString>
#include <QDateTime>
#include <QDir>
//#pragma execution_character_set("utf-8")

class TUtil
{
public:
    static QString getTime();
    static QString getTimeMS();

    static QString getCurrentPath();
    static QString getApplicationPath();

};
#endif // TUTIL_H
