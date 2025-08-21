#include <TLog.h>
#include <QtCore>
#include <QMutex>

//#pragma execution_character_set("utf-8")

QMutex* TLog::sMutex = new  QMutex();
QFile* TLog::sFilePointer = nullptr;
long TLog::sMaxSize = 10 * 1024 * 1024;
int TLog::sLevel = 0;

void TLog::init(QString fileNamePath, long maxSize)
{
    sMaxSize = maxSize;
    TLog::setLevel(TLog::level_debug);

    qInfo() << "日志文件[" << fileNamePath << "]";
    sFilePointer = new QFile(fileNamePath);
    if(sFilePointer->exists() && sFilePointer->size() > maxSize)
    {
        sFilePointer->remove();
    }

    //创建父目录
    try
    {
        QFileInfo info(fileNamePath);
        QDir dir = info.dir();
        dir.makeAbsolute();
        dir.mkpath(dir.path());
    } catch(...) {}


    if(!sFilePointer->open(QFile::Append))
    {
       qCritical() << "can't open file " << sFilePointer;
       sFilePointer = nullptr;
    }
    else
    {
        QString level = "";
        switch (sLevel)
        {
            case 0:
            case 1:
                level = "debug";
                break;
            case 2:
                level = "info";
                break;
            case 3:
                level = "warning";
                break;
            case 4:
                level = "error";
                break;
            case 5:
                level = "important";
                break;
            default:
                level = "unknow";
        }
        TLog::ii("打开日志文件[" + fileNamePath + "], 级别为" + QString::number(sLevel) + ":" + level + ", 超过" + QString::number(sMaxSize) + "字节将重建文件。");
    }
}


void TLog::setLevel(int level)
{
    sLevel = level;
}


void TLog::d(QString txt)
{
    if(sLevel > level_debug)
        return;

    QString log = "debug\t" + TUtil::getTimeMS() + "\t" + txt;
    qDebug().noquote() << log;
    writeFile(log);
}

void TLog::i(QString txt)
{
    if(sLevel > level_info)
        return;

    QString log = "info\t" + TUtil::getTimeMS() + "\t" + txt;
    qInfo().noquote() << log;
    writeFile(log);
}

void TLog::w(QString txt)
{
    if(sLevel > level_warning)
        return;

    QString log = "warning\t" + TUtil::getTimeMS() + "\t" + txt;
    qWarning().noquote() << log;
    writeFile(log);
}

void TLog::e(QString txt)
{
    if(sLevel > level_error)
        return;

    QString log = "error\t" + TUtil::getTimeMS() + "\t" + txt;
    qCritical().noquote() << log;
    writeFile(log);
}


void TLog::ii(QString txt)
{
    if(sLevel > level_important)
        return;

    QString log = "important\t" + TUtil::getTimeMS() + "\t" + txt;
    qCritical().noquote() << log;
    writeFile(log);
}


void TLog::writeFile(QString log)
{
    try
    {
        if(sFilePointer != nullptr)
        {
            sMutex->lock();
            sFilePointer->write(log.toUtf8());
            sFilePointer->write("\r\n");
            sFilePointer->flush();
            sMutex->unlock();
        }
    } catch (...) {}
}
