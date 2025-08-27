#ifndef XGOSCREEN_XGOSCREENMANAGER_H
#define XGOSCREEN_XGOSCREENMANAGER_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <xgoscreen/SpiScreen2Inch.h>

/**
 * @brief 简化的屏幕管理器，提供文字和图片显示的高级API
 * 
 * 这个类封装了SpiScreen2Inch和ScreenThread的复杂性，
 * 为xgo_head提供简单易用的屏幕显示接口
 */
class XgoScreenManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param widget 用于抓屏的QWidget
     * @param parent QObject父对象
     */
    explicit XgoScreenManager(QWidget* widget, QObject *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~XgoScreenManager();

    /**
     * @brief 初始化屏幕
     * @param spiController SPI控制器索引，默认1
     * @param spiDevice SPI设备索引，默认0  
     * @param spiFrequency SPI频率，默认20MHz
     * @param gpioCS CS引脚，默认5
     * @param gpioReset 复位引脚，默认13
     * @param gpioDC DC引脚，默认29
     * @param refreshInterval 刷新间隔（毫秒），默认100ms
     * @return 初始化是否成功
     */
    bool initialize(int spiController = 1, int spiDevice = 0, int spiFrequency = 20000000,
                   int gpioCS = 5, int gpioReset = 13, int gpioDC = 29, int refreshInterval = 100, bool fast_startup = false);

    /**
     * @brief 设置背景图片
     * @param image 背景图片
     */
    void setBackgroundImage(const QImage& image);

    /**
     * @brief 设置为纯色背景
     * @param color 背景颜色，默认黑色
     */
    void setBackgroundColor(const QColor& color = Qt::black);

    /**
     * @brief 暂停或恢复屏幕刷新
     * @param pause true为暂停，false为恢复
     */
    void pauseRefresh(bool pause);

    /**
     * @brief 停止屏幕刷新并清理资源
     */
    void stop();

    /**
     * @brief 重新启动屏幕
     */
    void restart();

    /**
     * @brief 检查屏幕是否正在运行
     * @return true如果正在运行
     */
    bool isRunning() const;

    /**
     * @brief 获取当前背景图片
     * @return 当前背景图片的拷贝
     */
    QImage getBackgroundImage() const;

    /**
     * @brief 获取内部背景图像指针（供生产者直接写入）
     * @return 指向内部QImage的指针
     */
    QImage* getBackgroundImagePointer() { return &mBackgroundImage; }

signals:
    /**
     * @brief 屏幕状态改变信号
     * @param running 是否正在运行
     */
    void screenStatusChanged(bool running);

private slots:
    /**
     * @brief 内部刷新槽函数
     */
    void onRefresh();

private:
    QWidget* mWidget;                    ///< 用于抓屏的Widget
    SpiScreen2Inch* mScreen;            ///< SPI屏幕对象
    ScreenThread* mScreenThread;        ///< 屏幕刷新线程
    QImage mBackgroundImage;            ///< 背景图片
    bool mInitialized;                  ///< 是否已初始化
    bool mRunning;                      ///< 是否正在运行
    
    // SPI参数
    int mSpiController;
    int mSpiDevice;
    int mSpiFrequency;
    int mGpioCS;
    int mGpioReset;
    int mGpioDC;
    int mRefreshInterval;

    /**
     * @brief 清理资源
     */
    void cleanup();
};

#endif // XGOSCREEN_XGOSCREENMANAGER_H