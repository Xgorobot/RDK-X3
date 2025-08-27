#include <xgoscreen/XgoScreenManager.h>
#include <xgoscreen/TLog.h>
#include <QPalette>
#include <QBrush>

XgoScreenManager::XgoScreenManager(QWidget* widget, QObject *parent)
    : QObject(parent), mWidget(widget), mScreen(nullptr), mScreenThread(nullptr), mInitialized(false), mRunning(false),
      mSpiController(1), mSpiDevice(0), mSpiFrequency(20000000), mGpioCS(5), mGpioReset(13), mGpioDC(29), mRefreshInterval(100)
{
    mBackgroundImage = QImage(320, 240, QImage::Format_RGB32);
    mBackgroundImage.fill(Qt::black);
}

XgoScreenManager::~XgoScreenManager()
{
    stop();
}

bool XgoScreenManager::initialize(int spiController, int spiDevice, int spiFrequency,
                                  int gpioCS, int gpioReset, int gpioDC, int refreshInterval, bool fast_startup)
{
    if (!mWidget) {
        TLog::e("XgoScreenManager initialize failed: widget is null.");
        return false;
    }

    // 覆盖参数
    mSpiController = spiController;
    mSpiDevice = spiDevice;
    mSpiFrequency = spiFrequency;
    mGpioCS = gpioCS;
    mGpioReset = gpioReset;
    mGpioDC = gpioDC;
    mRefreshInterval = refreshInterval;

    // 初始化屏幕
    mScreen = new SpiScreen2Inch(mSpiController, mSpiDevice, mSpiFrequency, mGpioCS, mGpioReset, mGpioDC, fast_startup);
    mScreenThread = new ScreenThread(mWidget, &mBackgroundImage, mScreen, mRefreshInterval);
    mScreenThread->start();

    mInitialized = true;
    mRunning = true;
    emit screenStatusChanged(true);
    return true;
}

void XgoScreenManager::setBackgroundImage(const QImage& image)
{
    mBackgroundImage = image;
}

void XgoScreenManager::setBackgroundColor(const QColor& color)
{
    mBackgroundImage = QImage(320, 240, QImage::Format_RGB32);
    mBackgroundImage.fill(color);
}

void XgoScreenManager::pauseRefresh(bool pause)
{
    if (mScreenThread) mScreenThread->pauseScreen(pause);
}

void XgoScreenManager::stop()
{
    if (!mInitialized) return;

    if (mScreenThread) {
        mScreenThread->pauseScreen(true);
        QThread::msleep(200);
        delete mScreenThread;
        mScreenThread = nullptr;
    }

    if (mScreen) {
        delete mScreen;
        mScreen = nullptr;
    }

    mInitialized = false;
    mRunning = false;
    emit screenStatusChanged(false);
}

void XgoScreenManager::restart()
{
    stop();
    initialize(mSpiController, mSpiDevice, mSpiFrequency, mGpioCS, mGpioReset, mGpioDC, mRefreshInterval, false);
}

bool XgoScreenManager::isRunning() const
{
    return mRunning;
}

QImage XgoScreenManager::getBackgroundImage() const
{
    return mBackgroundImage.copy();
}

void XgoScreenManager::onRefresh()
{
    // 保留接口，后续可在内部做定制化刷新控制
}

void XgoScreenManager::cleanup()
{
    stop();
}