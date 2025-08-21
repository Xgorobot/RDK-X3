#ifndef GPIOREADER_H
#define GPIOREADER_H

#include <QObject>
#include <QThread>

#include "TGpioUtil.h"
#include "TLog.h"

class GpioReader : public QThread
{
    Q_OBJECT

public:
    GpioReader(int gpioPort, int intervalMS = 100);
    ~GpioReader();
    void stopRead();

    void pauseRead();        // 暂停读取（新增）
    void resumeRead();       // 恢复读取（新增）

protected:
    void run();

private:
    int mGpioPort = 0;
    int mLastValue = 0; // low
    int mInterval = 100;
    bool mStop = false;
    bool mPaused = false;
    bool openGpio();
    bool closeGpio();

signals:
    void sigReceiveGpioData(int port, int lastValue, int newValue);
};


#endif // GPIOREADER_H
