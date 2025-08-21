#ifndef SPISCREEN2INCH_H
#define SPISCREEN2INCH_H

#include <QObject>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QPixmap>
#include <QWidget>


#include "TLog.h"
#include "TGpioUtil.h"
#include "TSpiUtil.h"


//for xgo 2 inch screen , ST7789VW .
class SpiScreen2Inch : public QObject
{
  Q_OBJECT
public:
  SpiScreen2Inch(int controllerIndex, int deviceIndex, int spiFrequency, int gpioCS, int gpioRest, int gpioDC);
  ~SpiScreen2Inch();
  void showImg(QImage* img);

private:
  int mSpi = -1;
  int mSpiCtroller = -1;
  int mSpiDevice = -1;
  int mX = 320;
  int mY = 240;
  int mFrequency = 0;

  int mDataLen = 320 *240 *2;
  uint8_t mData[240*320*2]; //565, 16bit per pix.

  int mGpioCS = 0;
  int mGpioReset = 0;
  int mGpioDC = 0;

  uint16_t color_To_565(uint8_t r, uint8_t g, uint8_t b);

  void spiCommand(uint8_t oxValue);
  void spiData(uint8_t oxValue);
  void screenInit();
  void screenReset();
  void screenWhite();
  void setWindowOnScreen(int startX, int startY, int width, int height);

  bool mScreenBusy = false;




};


class ScreenThread : public QThread
{
    Q_OBJECT
public:
  ScreenThread(QWidget* widget,  QImage* imageBackground,  SpiScreen2Inch* screen, int intervalMS);
  ~ScreenThread();
  void pauseScreen(bool needPause);

public slots:
  void refreshScreen();

protected:
  void run() override;

private:
  QWidget* mWidget = nullptr;
  QImage* mImageBackground = nullptr;
  SpiScreen2Inch* mScreen = nullptr;
  int mInterval = 100; //ms.
  int mPause = false;
  QTimer* mTimer = nullptr;


};

#endif // SPISCREEN2INCH_H
