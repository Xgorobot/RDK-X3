#include <xgoscreen/SpiScreen2Inch.h>
#include <QColor>

uint8_t spi_tx_buffer[4096];
uint8_t spi_rx_buffer[4096];
SpiScreen2Inch::SpiScreen2Inch(int controllerIndex, int deviceIndex, int spiFrequency, int gpioCS, int gpioRest, int gpioDC, bool fast_startup)
{
  this->mFastStartup = fast_startup;
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

  TLog::d("open reset");
  this->mGpioReset = gpioRest;
  std::string restStr = QString::number(gpioRest).toStdString();
  const char* rest = restStr.c_str();
  TGpioUtil::openGpio(rest);
  TLog::d("open dc");
  this->mGpioDC = gpioDC;
  std::string dcStr = QString::number(gpioDC).toStdString();
  const char* dc = dcStr.c_str();
  TGpioUtil::openGpio(dc);

  QThread::msleep(this->mFastStartup ? 50 : 500);

  TLog::d("set gpio [reset] direction to out");
  TGpioUtil::setGpioDirection(rest, "out");
  TLog::d("set gpio [dc] direction to out");
  TGpioUtil::setGpioDirection(dc, "out");

  this->screenInit();
}

SpiScreen2Inch::~SpiScreen2Inch()
{
  if(this->mSpi != -1)
  {
    TSpiUtil::closeSpi(this->mSpi);
  }

  // Do not unexport GPIO here to avoid interfering with other processes sharing the same pins
  if(this->mGpioDC != 0)
  {
    std::string dcStr = QString::number(this->mGpioDC).toStdString();
    const char* dc = dcStr.c_str();
    TGpioUtil::writeGpioValue(dc, "0");
    // TGpioUtil::closeGpio(dc); // removed
    TLog::d(QString("keep GPIO port %1 exported.").arg(this->mGpioDC));
  }

  if(this->mGpioReset != 0)
  {
    std::string resetStr = QString::number(this->mGpioReset).toStdString();
    const char* reset = resetStr.c_str();
    TGpioUtil::writeGpioValue(reset, "1");
    // TGpioUtil::closeGpio(reset); // removed
    TLog::d(QString("keep GPIO port %1 exported.").arg(this->mGpioReset));
  }
}

void SpiScreen2Inch::screenWhite()
{
  TLog::d("white screen start.");
  for(int i=0; i<this->mDataLen; i++)
  {
    this->mData[i] = 0xFF;
  }

  this->spiCommand(0x36);
  this->spiData(0x70);
  this->setWindowOnScreen(0,0,320,240);
  int result = TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "1");
  Q_UNUSED(result);

  int stepNum = 40;
  int stepLen = 320*240*2/stepNum;
  int tempP = 0;

  for(int i=0; i<stepNum; i++)
  {
    TSpiUtil::spiWrite(this->mSpi, &(this->mData[tempP]), stepLen);
    tempP = tempP + stepLen;
  }
  TLog::d("white screen finished.");
}

void SpiScreen2Inch::showImg(QImage *img)
{
  if(img == nullptr)
    return;

  if(this->mScreenBusy)
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
  Q_UNUSED(result);

  int stepNum = 40;
  int stepLen = 320*240*2/stepNum;
  int tempP = 0;
  uint8_t tempData[3840];
  for(int i=0; i<stepNum; i++)
  {
    for(int j=0; j<3840/2; j++)
    {
      tempData[2*j] = this->mData[tempP + 2*j + 1];
      tempData[2*j + 1] = this->mData[tempP + 2*j];
    }
    TSpiUtil::spiWrite(this->mSpi, tempData, stepLen);
    tempP = tempP + stepLen;
  }

  this->mScreenBusy = false;
}

uint16_t SpiScreen2Inch::color_To_565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r& 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

void SpiScreen2Inch::spiCommand(uint8_t oxValue)
{
  int resultGpio = TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "0");
  Q_UNUSED(resultGpio);

  spi_tx_buffer[0] = oxValue;

  bool resultSpi = TSpiUtil::spiWrite(this->mSpi, (const uint8_t*)spi_tx_buffer, 1);
  Q_UNUSED(resultSpi);
}

void SpiScreen2Inch::spiData(uint8_t oxValue)
{
  TGpioUtil::writeGpioValue(QString::number(this->mGpioDC).toStdString().c_str(), "1");
  bool result = TSpiUtil::spiWrite(this->mSpi, (const uint8_t*)&oxValue, 1);
  Q_UNUSED(result);
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

  // 默认黑屏
  QImage blackImg(320, 240, QImage::Format_RGB32);
  blackImg.fill(Qt::black);
  this->showImg(&blackImg);
}

void SpiScreen2Inch::screenReset()
{
  TLog::d("spi screen reset start.");
  std::string resetStr = QString::number(this->mGpioReset).toStdString();
  const char* reset = resetStr.c_str();
  int result = TGpioUtil::writeGpioValue(reset, "1");
  Q_UNUSED(result);
  QThread::msleep(this->mFastStartup ? 5 : 20);
  result = TGpioUtil::writeGpioValue(reset, "0");
  Q_UNUSED(result);
  QThread::msleep(this->mFastStartup ? 5 : 20);
  result = TGpioUtil::writeGpioValue(reset, "1");
  Q_UNUSED(result);
  QThread::msleep(this->mFastStartup ? 5 : 20);
  TLog::d("spi screen reset finished.");
}

void SpiScreen2Inch::setWindowOnScreen(int startX, int startY, int width, int height)
{
  quint16 tempInt;
  char tempChar;

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
  // 安全停止线程和定时器
  if (this->mTimer) {
    // 在线程内部请求停止定时器
    QMetaObject::invokeMethod(this->mTimer, "stop", Qt::QueuedConnection);
    // 等待线程结束
    this->quit();
    this->wait(3000); 

    delete this->mTimer;
    this->mTimer = nullptr;
  }
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
  if(this->mWidget == nullptr || this->mImageBackground == nullptr)
    return;
  if(this->mScreen == nullptr)
    return;
  if(this->mPause)
    return;

  try
  {
    this->mWidget->setAutoFillBackground(true);
    QPalette palette = this->mWidget->palette();
    bool useBlack = false;
    if (this->mImageBackground->width() == 320 && this->mImageBackground->height() == 240) {
      useBlack = true;
      for (int y : {0, 120, 239}) {
        for (int x : {0, 160, 319}) {
          QColor c(this->mImageBackground->pixel(x, y));
          if (c.red() != 0 || c.green() != 0 || c.blue() != 0) { useBlack = false; break; }
        }
        if (!useBlack) break;
      }
    }

    if (useBlack) {
      this->mWidget->setStyleSheet("background-color: #000000;");
      palette.setBrush(QPalette::Window, QBrush(Qt::black));
    } else {
      this->mWidget->setStyleSheet("");
      QPixmap pix = QPixmap::fromImage(this->mImageBackground->scaled(320, 240));
      palette.setBrush(QPalette::Window, QBrush(pix));
    }
    this->mWidget->setPalette(palette);
  }
  catch(...)
  {
    TLog::e("set background faile!");
  }

  QPixmap * pix = nullptr;
  try
  {
    pix = new QPixmap(this->mWidget->grab().copy());
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
  }
}