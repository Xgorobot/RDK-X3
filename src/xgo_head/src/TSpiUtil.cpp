#include "TSpiUtil.h"

uint8_t tx_buffer[4096];
uint8_t rx_buffer[4096];

int TSpiUtil::openSpi(int controllerIndex, int deviceIndex)
{
  QString device = QString("/dev/spidev%1.%2").arg(controllerIndex).arg(deviceIndex);
  int fd = open(device.toStdString().c_str()  , O_RDWR | O_SYNC);
  if (fd < 0)
  {
    TLog::e(QString("can not open spi device: %1").arg(device));
  }
  else
  {
    TLog::i(QString("open spi device: %1,  get fd: %2.").arg(device).arg(fd));
  }
  return fd;
}


bool TSpiUtil::closeSpi(int spi)
{
  close(spi);

  return true;
}


bool TSpiUtil::setSpiMode(int spi, int mode)
{
  uint32_t modeValue = mode;
  int result = ioctl(spi,SPI_IOC_WR_MODE32,&modeValue);
  if(result == -1)
  {
    TLog::e(QString("failed to set SPI_IOC_WR_MODE32 of %1 to %2").arg(spi).arg(mode));
    return false;
  }
  else
  {
    TLog::i(QString("set SPI_IOC_WR_MODE32 of %1 to %2").arg(spi).arg(mode));
  }

  return true;
}


int TSpiUtil::getSpiMode(int spi)
{
  uint32_t mode = -1;
  int result = ioctl(spi,SPI_IOC_RD_MODE32,&mode);
  if(result == -1)
  {
    TLog::e(QString("failed to read SPI_IOC_RD_MODE32 of %1").arg(spi));
    return -1;
  }
  else
  {
    TLog::i(QString("read SPI_IOC_RD_MODE32 of %1 to %2").arg(spi).arg(mode));
  }

  return mode;
}


bool TSpiUtil::setBitsPerWord(int spi, int bits)
{
  uint8_t bits8 = bits;
  int result = ioctl(spi, SPI_IOC_WR_BITS_PER_WORD, &bits8);
  if(result == -1)
  {
    TLog::e(QString("failed to set SPI_IOC_WR_BITS_PER_WORD of %1 to %2").arg(spi).arg(bits8));
    return false;
  }
  else
  {
    TLog::i(QString("set SPI_IOC_WR_BITS_PER_WORD of %1 to %2").arg(spi).arg(bits8));
    return true;
  }
}


int TSpiUtil::getBitsPerWord(int spi)
{
  uint8_t bits = 0;
  int result = ioctl(spi, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (result == -1)
  {
    TLog::e(QString("failed to read SPI_IOC_RD_BITS_PER_WORD of %1").arg(spi));
    return -1;
  }
  else
  {
    TLog::i(QString("read SPI_IOC_RD_BITS_PER_WORD of %1 to %2").arg(spi).arg(bits));
    return bits;
  }
}


bool TSpiUtil::setBitOrder(int spi, int order)
{
  int result = ioctl(spi, SPI_IOC_WR_LSB_FIRST, &order);
  if(result == -1)
  {
    TLog::e(QString("failed to set SPI_IOC_WR_LSB_FIRST of %1 to %2").arg(spi).arg(order));
    return false;
  }
  else
  {
    TLog::i(QString("set SPI_IOC_WR_LSB_FIRST of %1 to %2").arg(spi).arg(order));
    return true;
  }
}


int TSpiUtil::getBitOrder(int spi)
{
  int order = 0;
  int result = ioctl(spi, SPI_IOC_RD_LSB_FIRST, &order);
  if (result == -1)
  {
    TLog::e(QString("failed to read SPI_IOC_RD_LSB_FIRST of %1").arg(spi));
    return -1;
  }
  else
  {
    TLog::i(QString("read SPI_IOC_RD_LSB_FIRST of %1 to %2").arg(spi).arg(order));
    return order;
  }
}


bool TSpiUtil::setMaxSpeedHz(int spi, int hz)
{
  uint32_t hzValue = hz;
  int result = ioctl(spi, SPI_IOC_WR_MAX_SPEED_HZ, &hzValue);
  if(result == -1)
  {
    TLog::e(QString("failed to set SPI_IOC_WR_MAX_SPEED_HZ of %1 to %2").arg(spi).arg(hz));
    return false;
  }
  else
  {
    TLog::i(QString("set SPI_IOC_WR_MAX_SPEED_HZ of %1 to %2").arg(spi).arg(hz));
    return true;
  }
}


int TSpiUtil::getMaxSpeedHz(int spi)
{
  uint32_t hz = 0;
  int result = ioctl(spi, SPI_IOC_RD_MAX_SPEED_HZ, &hz);
  if (result == -1)
  {
    TLog::e(QString("failed to read SPI_IOC_RD_MAX_SPEED_HZ of %1").arg(spi));
    return -1;
  }
  else
  {
    TLog::i(QString("read SPI_IOC_RD_MAX_SPEED_HZ of %1 to %2").arg(spi).arg(hz));
    return hz;
  }
}


bool TSpiUtil::spiWrite(int spi, uint8_t const *data, size_t dataLen)
{
  int size = write(spi, (const void *)data, dataLen);
  if(size == (int)dataLen)
  {
    // TLog::d(QString("success, spi %1 write %2 bytes").arg(spi).arg(dataLen));
    return true;
  }
  else
  {
    TLog::e(QString("fail, spi %1 need write %2 bytes, write %3 bytes").arg(spi).arg(dataLen).arg(size));
    return false;
  }

  // struct spi_ioc_transfer tr;
  // tr.tx_buf = (unsigned long)data;
  // tr.rx_buf = (unsigned long)rx_buffer;
  // tr.len = (uint)dataLen;
  // tr.delay_usecs = 0;

  // int ret = ioctl(spi, SPI_IOC_MESSAGE(1), &tr);
  // if(ret >= 1)
  // {
  //   TLog::d(QString("spi %1 write %2 bytes succefully,  return %3").arg(spi).arg(dataLen).arg(ret));
  //   return true;
  // }
  // else
  // {
  //   TLog::e(QString("spi %1 need write %2 bytes, but fail,  return %3").arg(spi).arg(dataLen).arg(ret));
  //   return false;
  // }




}

int TSpiUtil::spiRead(int spi, const char *temp, int maxLen)
{
  int size = read(spi, (void *)temp, maxLen);
  return size;
}




void TSpiUtil::spiTransfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len)
{
  int ret;
  // int out_fd;
  // struct spi_ioc_transfer tr = {
  //     .tx_buf = (unsigned long)tx,
  //     .rx_buf = (unsigned long)rx,
  //     .len = (uint)len,
  // };

  struct spi_ioc_transfer tr;
      tr.tx_buf = (unsigned long)tx;
      tr.rx_buf = (unsigned long)rx;
      tr.len = (uint)len;


  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
  {
    TLog::e("can't send spi message");
  }
  TLog::d(QString("spi transfer return value is %1\r\n").arg(ret));
}





