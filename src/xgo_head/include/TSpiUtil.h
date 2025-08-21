#ifndef TSPIUTIL_H
#define TSPIUTIL_H


#include <linux/spi/spidev.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "TLog.h"


// for linux only.
class TSpiUtil
{
  /*
   * for /dev/spidev2.0, controllerIndex is 2, deviceIndex is 0.
   * if open , return int value stands for spi,  bigger than 0 .
  */
public:
  static int openSpi(int controllerIndex, int deviceIndex);
  static bool closeSpi(int spi);

  static bool  setSpiMode(int spi, int mode);
  static int getSpiMode(int spi);

  static bool setBitsPerWord(int spi, int bits);
  static int getBitsPerWord(int spi);

  static bool setBitOrder(int spi , int order); // order : MSB_FIRST or LSB_FIRST
  static int getBitOrder(int spi);

  static bool setMaxSpeedHz(int spi, int hz);
  static int getMaxSpeedHz(int spi);

  static bool spiWrite(int spi, uint8_t const *data, size_t dataLen);
  static int spiRead(int spi, const char* temp, int maxLen);

  static void spiTransfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len);




};

#endif // TSPIUTIL_H

// useful web url:
// https://blog.csdn.net/Bella1990/article/details/134688193
// https://blog.csdn.net/engineer0/article/details/121153009
