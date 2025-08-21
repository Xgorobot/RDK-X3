#include "SpiScreen2Inch.h"


uint8_t spi_tx_buffer[4096];
uint8_t spi_rx_buffer[4096];
SpiScreen2Inch::SpiScreen2Inch(int controllerIndex, int deviceIndex, int spiFrequency, int gpioCS, int gpioRest, int gpioDC)
{
  TLog::d("open spi");
  this->mSpiCtroller = controllerIndex;
  this->mSpiDevice = deviceIndex;
  this->mSpi = TSpiUtil::openSpi(this->mSpiCtroller, this->mSpiDevice);
  this->mFrequency = spiFrequency;
  TSpiUtil::setMaxSpeedHz(this->mSpi, this->mFrequency);
  TSpiUtil::setSpiMode(this->mSpi, 0);
  TSpiUtil::setBitsPerWord(this->mSpi, 8);

  TLog::d(QString("read spi speed:%1").arg(TSpiUtil::getMaxSpeedHz(this->mSpi)));
  TLog::d(QString("read spi mode:%1").arg(TSpiUtil::getSpiMode(this->mSpi)));
  TLog::d(QString("read spi bits per word:%1").arg(TSpiUtil::getBitsPerWord(this->mSpi)));

  // TLog::d("open CS");
  // this->mGpioCS = gpioCS;
  // const char* cs = QString::number(gpioCS).toStdString().c_str();
  // TGpioUtil::openGpio(cs);
  TLog::d("open reset");
  this->mGpioReset = gpioRest;
  const char* rest = QString::number(gpioRest).toStdString().c_str();
  TGpioUtil::openGpio(rest);
  TLog::d("open dc");
  this->mGpioDC = gpioDC;
  const char* dc = QString::number(gpioDC).toStdString().c_str();
  TGpioUtil::openGpio(dc);

  QThread::msleep(500); //wait for GPIO open.

  TLog::d("set gpio [reset] direction to out");
  TGpioUtil::setGpioDirection(rest, "out");
  TLog::d("set gpio [dc] direction to out");
  TGpioUtil::setGpioDirection(dc, "out");
  // TLog::d("set gpio [cs] direction to out");
  // TGpioUtil::setGpioDirection(cs, "out");



  // int result = TGpioUtil::writeGpioValue(QString::number(this->mGpioCS).toStdString().c_str(), "0");
  // TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioCS).arg("0").arg(result));

  this->screenInit();

}


SpiScreen2Inch::~SpiScreen2Inch()
{
  if(this->mSpi != -1)
  {
    TSpiUtil::closeSpi(this->mSpi);
  }

  if(this->mGpioDC != 0)
  {
    const char* dc = QString::number(this->mGpioDC).toStdString().c_str();
    TGpioUtil::writeGpioValue(dc, "0");
    TGpioUtil::closeGpio(dc);
    TLog::d(QString("close GPIO port %1.").arg(this->mGpioDC));
  }

  if(this->mGpioReset != 0)
  {
    const char* reset = QString::number(this->mGpioReset).toStdString().c_str();
    TGpioUtil::writeGpioValue(reset, "1");
    TGpioUtil::closeGpio(reset);
    TLog::d(QString("close GPIO port %1.").arg(this->mGpioReset));
  }
}


void SpiScreen2Inch::screenWhite()
{
  TLog::d("white screen start.");

  //white data
  // uint16_t value = color_To_565(0xFF, 0xFF, 0xFF);
  // TLog::i("0x" + QString::number(value, 16));
  // int size = 320 * 240;
  // uint16_t *p = (uint16_t *)this->mData;
  // for(int i=0; i<size; i++)
  // {
  //   *p = value;
  //   p++;
  // }

   for(int i=0; i<this->mDataLen; i++)
   {
     this->mData[i] = 0xFF;
   }




  //command before show screen
  this->spiCommand(0x36);
  this->spiData(0x70);
  this->setWindowOnScreen(0,0,320,240);
  int result = TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "1");
  TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioDC).arg("1").arg(result));


  //write screen data
  int stepNum = 40;
  int stepLen = 320*240*2/stepNum; //3840,   max 4096
  int tempP = 0;

  for(int i=0; i<stepNum; i++)
  {
    // TLog::d(QString("write %1 for %2").arg(i).arg(stepNum));
    TSpiUtil::spiWrite(this->mSpi, &(this->mData[tempP]), stepLen);
    tempP = tempP + stepLen;
  }
  TLog::d("white screen finished.");
}

void SpiScreen2Inch::showImg(QImage *img)
{
  // TLog::d("showImg start");
  if(img == nullptr)
    return;

  if(this->mScreenBusy) //for multi thread.
    return;

  this->mScreenBusy = true;

  QImage tempImg;
  if(img->width() != this->mX || img->height() != this->mY)
  {
    tempImg = img->scaled(this->mX, this->mY);
  }
  else
  {
    tempImg = img->copy();
  }


  tempImg = tempImg.convertToFormat(QImage::Format_BGR888);
  uchar* source = tempImg.bits();
  uint16_t * target = (uint16_t*)this->mData;
  int size = this->mX * this->mY;
  for(int i=0; i<size; i++)
  {
    uint16_t value565 = this->color_To_565(source[i*3+2], source[i*3+1], source[i*3]);
    target[i] = value565;
  }

  this->spiCommand(0x36);
  this->spiData(0x70);
  this->setWindowOnScreen(0,0,320,240);

  int result = TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "1");
  // TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioDC).arg("1").arg(result));

  int stepNum = 40;
  int stepLen = 320*240*2/stepNum; //3840,   max 4096
  int tempP = 0;
  uint8_t tempData[3840];
  for(int i=0; i<stepNum; i++)
  {
    // TLog::d(QString("write %1 for %2").arg(i).arg(stepNum));

    for(int j=0; j<3840/2; j++)
    {
      tempData[2*j] = this->mData[tempP + 2*j + 1];
      tempData[2*j + 1] = this->mData[tempP + 2*j];
    }
    TSpiUtil::spiWrite(this->mSpi, tempData, stepLen);
    tempP = tempP + stepLen;

    // TSpiUtil::spiTransfer(this->mSpi, temp, spi_rx_buffer, stepLen);
    // QThread::msleep(100);
  }

  // QImage img565(this->mData, 320, 240, 640, QImage::Format_RGB16);
  // img565.save("/home/sunrise/img565.jpg");
  // TLog::d("showImg finished");

  this->mScreenBusy = false;

}


uint16_t SpiScreen2Inch::color_To_565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r& 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

void SpiScreen2Inch::spiCommand(uint8_t oxValue)
{

  int resultGpio = TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "0");
  // TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioDC).arg("0").arg(resultGpio));

  spi_tx_buffer[0] = oxValue;

  bool resultSpi = TSpiUtil::spiWrite(this->mSpi, (const uint8_t*)spi_tx_buffer, 1);

  // TLog::d(QString("spiCommand, write 0X%1, %2")
  //         .arg(QString::number(oxValue, 16))
  //         .arg(resultSpi));

}


void SpiScreen2Inch::spiData(uint8_t oxValue)
{

  TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "1");
  bool result = TSpiUtil::spiWrite(this->mSpi, (const uint8_t*)&oxValue, 1);

  // TLog::d(QString("spiData, write 0X%1, %2").arg(QString::number(oxValue, 16)).arg(result));
}

void SpiScreen2Inch::screenInit()
{
  TLog::d("spi screen init start.");
  this->screenReset();

   this->spiCommand(0x36);
   this->spiData(0x00);

   this->spiCommand(0x3A);
   this->spiData(0x05);

   this->spiCommand(0x21);

   this->spiCommand(0x2A);
   this->spiData(0x00);
   this->spiData(0x00);
   this->spiData(0x01);
   this->spiData(0x3F);

   this->spiCommand(0x2B);
   this->spiData(0x00);
   this->spiData(0x00);
   this->spiData(0x00);
   this->spiData(0xEF);

   this->spiCommand(0xB2);
   this->spiData(0x0C);
   this->spiData(0x0C);
   this->spiData(0x00);
   this->spiData(0x33);
   this->spiData(0x33);

   this->spiCommand(0xB7);
   this->spiData(0x35);

   this->spiCommand(0xBB);
   this->spiData(0x19);

   this->spiCommand(0xC0);
   this->spiData(0x2C);

   this->spiCommand(0xC2);
   this->spiData(0x01);

   this->spiCommand(0xC3);
   this->spiData(0x12);

   this->spiCommand(0xC4);
   this->spiData(0x20);

   this->spiCommand(0xC6);
   this->spiData(0x0F);

   this->spiCommand(0xD0);
   this->spiData(0xA4);
   this->spiData(0xA1);

   this->spiCommand(0xE0);
   this->spiData(0xD0);
   this->spiData(0x04);
   this->spiData(0x0D);
   this->spiData(0x11);
   this->spiData(0x13);
   this->spiData(0x2B);
   this->spiData(0x3F);
   this->spiData(0x54);
   this->spiData(0x4C);
   this->spiData(0x18);
   this->spiData(0x0D);
   this->spiData(0x0B);
   this->spiData(0x1F);
   this->spiData(0x23);


   this->spiCommand(0xE1);
   this->spiData(0xD0);
   this->spiData(0x04);
   this->spiData(0x0C);
   this->spiData(0x11);
   this->spiData(0x13);
   this->spiData(0x2C);
   this->spiData(0x3F);
   this->spiData(0x44);
   this->spiData(0x51);
   this->spiData(0x2F);
   this->spiData(0x1F);
   this->spiData(0x1F);
   this->spiData(0x20);
   this->spiData(0x23);


   this->spiCommand(0x21);

   this->spiCommand(0x11);

   this->spiCommand(0x29);
  TLog::d("spi screen init finished.");

  this->screenWhite();
}



void SpiScreen2Inch::screenReset()
{
  TLog::d("spi screen reset start.");
  const char* reset = QString::number(this->mGpioReset).toStdString().c_str();
  int result = TGpioUtil::writeGpioValue(reset, "1");
  TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioReset).arg("1").arg(result));
  QThread::msleep(20);
  result = TGpioUtil::writeGpioValue(reset, "0");
  TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioReset).arg("0").arg(result));
  QThread::msleep(20);
  result = TGpioUtil::writeGpioValue(reset, "1");
  TLog::d(QString("GPIO port %1 set to %2, result %3").arg(this->mGpioReset).arg("1").arg(result));
  QThread::msleep(20);
  TLog::d("spi screen reset finished.");
}


void SpiScreen2Inch::setWindowOnScreen(int startX, int startY, int width, int height)
{
  // TLog::d("set windows start.");
  quint16 tempInt;
  char tempChar;

  //x
  this->spiCommand(0x2A);

  tempInt = startX;
  tempChar = tempInt >> 8;
  this->spiData(tempChar);
  tempChar = tempInt & 0xFF;
  this->spiData(tempChar);

  tempInt = width;
  tempChar = tempInt >> 8;
  this->spiData(tempChar);
  tempChar = (tempInt-1) & 0xFF;
  this->spiData(tempChar);

  //y
  this->spiCommand(0x2B);
  tempInt = startY;
  tempChar = tempInt >> 8;
  this->spiData(tempChar);
  tempChar = tempInt & 0xFF;
  this->spiData(tempChar);

  tempInt = height;
  tempChar = tempInt >> 8;
  this->spiData(tempChar);
  tempChar = (tempInt-1) & 0xFF;
  this->spiData(tempChar);

  this->spiCommand(0x2C);
  // TLog::d("set windows finished.");
}








ScreenThread::ScreenThread(QWidget *widget, QImage* imageBackground, SpiScreen2Inch *screen, int intervalMS)
{
  this->mWidget = widget;
  this->mImageBackground = imageBackground;
  this->mScreen = screen;
  this->mInterval = intervalMS;
}


ScreenThread::~ScreenThread()
{
  this->mTimer->stop();
}


void ScreenThread::pauseScreen(bool needPause)
{
  this->mPause = needPause;
}


void ScreenThread::run()
{
  TLog::i("ScreenThread start.");
  this->mTimer = new QTimer();
  this->mTimer->setInterval(this->mInterval);
  connect(this->mTimer, SIGNAL(timeout()), this, SLOT(refreshScreen()));
  this->mTimer->start();

  this->exec();
  TLog::i("ScreenThread going down.");
}



void ScreenThread::refreshScreen()
{
  // TLog::d("refresh screen.");

  if(this->mWidget == nullptr || this->mImageBackground == nullptr)
    return;

  if(this->mScreen == nullptr)
    return;

  if(this->mPause)
  {
    return;
  }
  //set background;
  try
  {
    this->mWidget->setAutoFillBackground(true);
    QPalette palette = this->mWidget->palette();
    // palette.setColor(QPalette::Window, Qt::red);
    // palette.setBrush(this->mWidget->backgroundRole(), Qt::black);
    QPixmap pix = QPixmap::fromImage(this->mImageBackground->scaled(320, 240));
    // pix.save("/home/sunrise/pix.jpg");
    palette.setBrush(QPalette::Window, QBrush(pix));

    this->mWidget->setPalette(palette);
    // TLog::d("set background over.");
  }
  catch(...)
  {
    TLog::e("set background faile!");
  }

  //grab img from widget and show.
  QPixmap * pix = nullptr;
  try
  {
    pix = new QPixmap(this->mWidget->grab().copy());
    // TLog::d("grab widget to pix over!");
  }
  catch(...)
  {
    TLog::e("grab widget to pix failed!");
    pix = nullptr;
  }

  if(pix != nullptr)
  {
    try
    {
      QImage img = pix->toImage();
      this->mScreen->showImg(&img);
    }
    catch(...)
    {
      TLog::e("show on 2inch screen failed!");
    }
    delete pix;
    pix = nullptr;
    // TLog::d("show on 2inch screen over!");
  }
  // TLog::d("refresh screen finished.");

  // this->pauseScreen(true);//test for once.
}

































