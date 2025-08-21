#include "GpioReader.h"


GpioReader::GpioReader(int gpioPort, int intervalMS)
{
  this->mGpioPort = gpioPort;
  this->mInterval = intervalMS;
}

GpioReader::~GpioReader()
{
  this->stopRead();
  this->closeGpio();
}

bool GpioReader::openGpio()
{
  const char* port = QString::number(this->mGpioPort).toStdString().c_str();
  int result = TGpioUtil::openGpio(port);
  if(result != 0)
  {
    TLog::e(QString("can't open gpio port : %1").arg(this->mGpioPort));
    return false;
  }

  QThread::msleep(500); //wait for GPIO open.

  result = TGpioUtil::setGpioDirection(port, "in");
  if(result != 0)
  {
    TLog::e(QString("can't set gpio port to read mode: %1").arg(this->mGpioPort));
    return false;
  }

  TLog::i(QString("opened gpio port %1 to read mode.").arg(this->mGpioPort));
  return result;
}


bool GpioReader::closeGpio()
{
  const char* port = QString::number(this->mGpioPort).toStdString().c_str();
  int result = TGpioUtil::closeGpio(port);
  if(result != 0)
  {
    TLog::e(QString("can't close gpio port : %1").arg(this->mGpioPort));
    return false;
  }

  TLog::i(QString("closed gpio port %1 . ").arg(this->mGpioPort));
  return result;
}


void GpioReader::stopRead()
{
    this->mStop = true;
}

void GpioReader::pauseRead()
{
    mPaused = true;
}

void GpioReader::resumeRead()
{
    mPaused = false;
}


// void GpioReader::run()
// {
//     if (!openGpio()) {
//         TLog::e(QString("Failed to open GPIO %1").arg(mGpioPort));
//         return;
//     }

//     const char* port = QString::number(this->mGpioPort).toStdString().c_str();
//     int value = 0;

//     while (!mStop)
//     {

//         value = TGpioUtil::readGpioValue(port);

//         if (value != mLastValue) {
//             emit sigReceiveGpioData(mGpioPort, mLastValue, value);
//             mLastValue = value;
//         }

//         QThread::msleep(mInterval);
//     }
//     TLog::i(QString("GPIO %1 reader stopped.").arg(mGpioPort));

//     closeGpio();
// }
void GpioReader::run()
{
    this->openGpio();

    const char* port = QString::number(this->mGpioPort).toStdString().c_str();
    int value = 0;
    while(!this->mStop)
    {
      value = TGpioUtil::readGpioValue(port);
      // TLog::d(QString("read gpio port %1,  data: %2").arg(this->mGpioPort).arg(value));
        if (this->mPaused) {
            QThread::msleep(10); // 短暂休眠，避免空转
            continue;
        }

      if(value != this->mLastValue)
      {
        emit sigReceiveGpioData(this->mGpioPort, this->mLastValue, value);
        this->mLastValue = value;
      }
      QThread::msleep(this->mInterval);
    }

    TLog::i(QString("stop read gpio port: %1, GpioReader thread  stop.").arg(this->mGpioPort));
    closeGpio();

}
