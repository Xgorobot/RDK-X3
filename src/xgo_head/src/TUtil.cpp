#include <TUtil.h>
#include <QApplication>

QString TUtil::getTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

QString TUtil::getTimeMS()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss_zzz");
}


QString TUtil::getCurrentPath()
{
    return QDir::currentPath();
}


QString TUtil::getApplicationPath()
{
    return QApplication::applicationDirPath();
}
