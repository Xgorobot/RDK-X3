#include "TGpioUtil.h"

int TGpioUtil::openGpio(const char *port)
{
    int fd = -1;
    const char *path = "/sys/class/gpio/export";

    if((fd = open(path, O_WRONLY)) == -1) {
        perror("Failed to open gpio! ");
        return -1;
    }

    write(fd, port, sizeof(port));
    close(fd);

    return 0;
}

int TGpioUtil::closeGpio(const char *port)
{
    int fd = -1;
    const char *path = "/sys/class/gpio/unexport";

    if((fd = open(path, O_WRONLY)) == -1) {
        perror("Failed to close gpio! ");
        return -1;
    }

    write(fd, port, sizeof(port));
    close(fd);

    return 0;
}

int TGpioUtil::setGpioDirection(const char *port, const char *direction)
{
    int fd = -1;
    char path[40] = {0};
    sprintf(path, "/sys/class/gpio/gpio%s/direction", port);

    if((fd = open(path, O_WRONLY)) == -1) {
        perror("Failed to set GPIO dirention! ");
        return -1;
    }

    write(fd, direction, sizeof(direction));
    close(fd);

    return 0;
}

int TGpioUtil::writeGpioValue(const char *port, const char *value)
{
    int fd = -1;
    char path[40] = {0};
    sprintf(path, "/sys/class/gpio/gpio%s/value", port);

    if((fd = open(path, O_WRONLY)) == -1) {
        perror("Failed to set GPIO value! ");
        return -1;
    }

    write(fd, value, sizeof(value));
    close(fd);

    return 0;
}

int TGpioUtil::readGpioValue(const char *port)
{
    int fd = -1;
    int value = 0;
    char path[40] = {0};
    sprintf(path, "/sys/class/gpio/gpio%s/value", port);

    if((fd = open(path, O_RDONLY)) == -1) {
        perror("Failed to set GPIO value! ");
        return -1;
    }

    char tmp[2] = {0};
    read(fd, tmp, 1);
    close(fd);

    value = atoi(tmp);

    return value;
}



