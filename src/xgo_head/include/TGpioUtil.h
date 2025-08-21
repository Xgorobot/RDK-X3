#ifndef TGPIOUTIL_H
#define TGPIOUTIL_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define ON "1"
#define OFF "0"
#define DIRECTION_OUT "out"
#define DIRECTION_IN "in"

using namespace std;

class TGpioUtil
{
public:
    static int openGpio(const char *port);
    static int closeGpio(const char *port);
    static int setGpioDirection(const char *port, const char *direction);
    static int writeGpioValue(const char *port, const char *value);
    static int readGpioValue(const char *port);
};

#endif // TGPIOUTIL_H















