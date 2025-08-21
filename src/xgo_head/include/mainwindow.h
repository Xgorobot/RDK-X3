#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define LABEL_COUNT 9
#include <QMainWindow>
#include <QPalette>
#include <QImage>
#include "rclcomm.h"
#include <iostream>
#include "TLog.h"
#include <QRegularExpression>  
#include "MatrixLabel.h"
#include <vector>
#include "GpioReader.h"
#include "SpiScreen2Inch.h"
#include "XgoSpeaker.h"
#include "QUrl"
#include <QStackedWidget>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initLabels();
    void updateSelection();
private:
    Ui::MainWindow *ui;
    // Ui::page2 *ui_page2;
    rclcomm *commNode;
    GpioReader* gpioKeyLeftUp = nullptr;
    GpioReader* gpioKeyLeftDown = nullptr;
    GpioReader* gpioKeyRightUp = nullptr;
    GpioReader* gpioKeyRightDown = nullptr;

    int mImgIndex = 1;
    int mImgMax = 5;
    SpiScreen2Inch* mScreen = nullptr;
    ScreenThread* mScreenThread = nullptr;
    QImage mImageBackground;
    int column = 0;
    int line = 0;
    XgoSpeaker mXgoSpeaker;
    int mXgoAudioIndex = 0;
    QStringList mXgoAudioList = {"bark2w.wav", "angry2w.wav", "tired2w.wav"};
    QPalette init_Palette;
    int currentlabel = 0;
    int page_current = 0;
    MatrixLabel* m_labels[9];  // 标签矩阵
public slots:
    void updateTopicInfo(QString);
    void onGpioData(int port , int oldValue,  int newValue);
    void changeBackgroundImg();
    void xgoSpeak();
    void change_focus(bool direction);
    QImage* windowToImg();
};
#endif // MAINWINDOW_H
