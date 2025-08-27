#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <xgoscreen/TLog.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QImage img;
    // img.load(":/icon/images/foxy.jpg");
    // img.scaled(ui->label->width(),ui->label->height());
    // ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 初始化 ROS2
    int argc=0;
    char **argv=NULL;
    rclcpp::init(argc,argv);

    commNode = new rclcomm();
    this->mImageBackground = QImage(320, 240, QImage::Format_RGB32);
    this->mImageBackground.fill(Qt::black);
    //std::system("tmux kill-server");

    // Defer setWidget until screen manager is ready
    // commNode->setWidget(ui->centralwidget, &(this->mImageBackground));
    connect(commNode, SIGNAL(emitTopicData(QString)), this, SLOT(updateTopicInfo(QString)));
    TLog::d("++++++++++ ROS2 Node init finished.");
    int interval = 100; //ms
    gpioKeyLeftUp = new GpioReader(12, interval);
    gpioKeyLeftDown = new GpioReader(27, interval);
    gpioKeyRightUp = new GpioReader(30, interval);
    gpioKeyRightDown = new GpioReader(22, interval);
    connect(gpioKeyLeftUp, SIGNAL(sigReceiveGpioData(int, int, int)), this, SLOT(onGpioData(int, int, int)));
    connect(gpioKeyLeftDown, SIGNAL(sigReceiveGpioData(int, int, int)), this, SLOT(onGpioData(int, int, int)));
    connect(gpioKeyRightUp, SIGNAL(sigReceiveGpioData(int, int, int)), this, SLOT(onGpioData(int, int, int)));
    connect(gpioKeyRightDown, SIGNAL(sigReceiveGpioData(int, int, int)), this, SLOT(onGpioData(int, int, int)));
    gpioKeyLeftUp->start();
    gpioKeyLeftDown->start();
    gpioKeyRightUp->start();
    gpioKeyRightDown->start();

    ui->stackedWidget->setCurrentIndex(0);

    // 设置纯黑背景
    ui->centralwidget->setStyleSheet("#centralwidget{background-color: #000000;}");
    
    // 隐藏不需要显示的标签
    ui->label_2->hide();        // ROS2 humble
    ui->label_battery->hide();  // 电量显示
    ui->label_3->hide();        // XGO20250327
    
    TLog::d("++++++++++ gpioKey init finished.");

    // 使用屏幕管理器
    this->mScreenMgr = new XgoScreenManager(ui->centralwidget, this);
    this->mScreenMgr->setBackgroundColor(Qt::black);
    this->mScreenMgr->initialize(1, 0, 20000000, 5, 13, 29, 100);
    // Route camera frames directly into the screen manager's image buffer
    commNode->setWidget(ui->centralwidget, this->mScreenMgr->getBackgroundImagePointer());
     initLabels();
     TLog::d("++++++++++ 2 inch screen init finished.");


    // QList<QLabel*> labels = findChildren<QLabel*>(
    //   QRegularExpression("label_[0-2][0-2]")  // 匹配00-22的标签
    // );
    // const QString init_style = "background: white; border: 1px soild gray;";
    // for(QLabel* label : labels)
    // {
    //     label->setAutoFillBackground(true);
    //     label->setStyleSheet(init_style);
    // }


}
void MainWindow::updateTopicInfo(QString data){
    Q_UNUSED(data);
    // 不再显示电量文本
    // ui->label_battery->setText("battary:"+QString("%1").arg(commNode->get_battery()));

    // ui->label_txt->clear();
    // ui->label_txt->setText(data);
    
}
MainWindow::~MainWindow()
{
  if (this->mScreenMgr) {
    this->mScreenMgr->stop();
    delete this->mScreenMgr;
    this->mScreenMgr = nullptr;
  }

  gpioKeyLeftUp->stopRead();
  gpioKeyLeftDown->stopRead();
  gpioKeyRightUp->stopRead();
  gpioKeyRightDown->stopRead();

  delete ui;
}




QImage* MainWindow::windowToImg()
{

  return nullptr;

}


void MainWindow::onGpioData(int port, int oldValue, int newValue)
{
  TLog::d(QString("gpio port %1,  oldValue:%2,  newValue:%3").arg(port).arg(oldValue).arg(newValue));
  QString key = "";
  QString stat = "";
  if(port == 12)
  {
    key = "左上键";
    if(newValue == 0)
    {
      // changeBackgroundImg();
      // ui->label
      change_focus(false);
      TLog::d(QString("LEFTUP"));

      // ui->label->clear();
      // ui->label->setText("label"+QString("%1").arg(m_currentRow)+QString("%1").arg(m_currentCol));

    }
  }
  else if(port == 27)
  {
    key = "左下键";
    if(newValue == 0)
    {
      ui->label->setText("退出");
      ui->stackedWidget->setCurrentIndex(0);
      m_labels[currentlabel]->stop_function();

      commNode->change_running(-1);

      if(m_labels[currentlabel]->is_hide_window && m_labels[currentlabel]->is_running_function == true)
      {
        QThread::msleep(600);
        if (this->mScreenMgr) {
          this->mScreenMgr->pauseRefresh(false);
        }
        if (this->mScreenMgr) {
          // 退出功能界面后，重新初始化屏幕
          this->mScreenMgr->initialize(1, 0, 20000000, 5, 13, 29, 100);
          commNode->setWidget(ui->centralwidget, this->mScreenMgr->getBackgroundImagePointer());
        }

      }
      m_labels[currentlabel]->is_running_function = false;


      if (this->mScreenMgr) {
        this->mScreenMgr->setBackgroundColor(Qt::black);
       // Also immediately clear the current background frame to black to avoid ghost image
       QImage* bg = this->mScreenMgr->getBackgroundImagePointer();
       if (bg) {
         bg->fill(Qt::black);
       }
      }
      ui->centralwidget->setStyleSheet("#centralwidget{background-color: #000000;}");


      updateSelection();

    }
  }
  else if(port == 30)
  {
    key = "右上键";
    if(newValue == 0)
    {
      // xgoSpeak();
      TLog::d(QString("RIGHTUP"));

      change_focus(true);
      // ui->label->clear();
      // ui->label->setText("label"+QString("%1").arg(m_currentRow)+QString("%1").arg(m_currentCol));
    }
  }
  else if(port == 22)
  {
    key = "右下键";
    if(newValue == 0)
    {
      ui->label->setText("进入");
      ui->stackedWidget->setCurrentIndex(2);
      m_labels[currentlabel]->function();
      if(m_labels[currentlabel]->is_hide_window) 
      {
        if (this->mScreenMgr) {
          this->mScreenMgr->pauseRefresh(true);
        }
        if (this->mScreenMgr) {

          this->mScreenMgr->stop();
        }

      }
      ui->label_info->setText(m_labels[currentlabel]->getinfo());
      commNode->change_running(currentlabel);
      //To do: jump in function
      //m_labels[m_currentRow][m_currentCol]->function()
    }
    
  }
  // ui->centralwidget->update();


  // if(newValue == 0)
  // {
  //   stat = "  按下";
  //   ui->label->clear();
  //   ui->label->setText(key + stat);


  // }
  // else
  // {
  //   stat = "  恢复";
  //   ui->label->clear();
  //   ui->label->setText(key + stat);
  // }

}



void MainWindow::changeBackgroundImg()
{
  this->mImgIndex ++;
  if(this->mImgIndex >= this->mImgMax)
    this->mImgIndex = 0;


  ui->centralwidget->setStyleSheet("#centralwidget{background-color: #000000;}");
  if (this->mScreenMgr) {
    this->mScreenMgr->setBackgroundColor(Qt::black);
   // Clear buffered frame too
   QImage* bg = this->mScreenMgr->getBackgroundImagePointer();
   if (bg) {
     bg->fill(Qt::black);
   }
  }
}


void MainWindow::xgoSpeak()
{

  this->mXgoSpeaker.stop();


  if(this->mXgoAudioIndex >= this->mXgoAudioList.length())
  {
    this->mXgoAudioIndex = 0;
  }


  // QString file = QString("qrc:/audio/audio/%1").arg(this->mXgoAudioList[this->mXgoAudioIndex]);
  QString file = QString("file:///home/sunrise/xgo_demo/src/xgo_head/resource/audio/%1").arg(this->mXgoAudioList[this->mXgoAudioIndex]);

  TLog::d(QString("play the %1 th audio, file[%2].").arg(this->mXgoAudioIndex).arg(file));
  if (QFile::exists(QUrl(file).toLocalFile())) {
    // player.setMedia(QUrl(path));
  } else {
    qDebug() << "文件不存在";
    // return -1;
  }
  this->mXgoSpeaker.playAudio(QUrl(file));

  this->mXgoAudioIndex ++;

}

//navLabels
void MainWindow::initLabels()
{
    // 初始化标签矩阵
    m_labels[0] = qobject_cast<MatrixLabel*>(ui->label_00);
    m_labels[0]->setinfo("手势识别");
    m_labels[0]->setCommand("hml_session","export CAM_TYPE=mipi && ros2 launch hand_gesture_detection hand_gesture_detection.launch.py & ros2 launch xgo_gesture_control gesture.launch.py");
    m_labels[0]->setPixmap(QPixmap(":/icon/images/手势识别.png"));
    m_labels[0]->setText("");

    m_labels[1] = qobject_cast<MatrixLabel*>(ui->label_01);
    m_labels[1]->setinfo("目标检测");
    m_labels[1]->setCommand("yolo_detect","export CAM_TYPE=mipi && ros2 launch dnn_node_example dnn_node_example.launch.py dnn_example_config_file:=config/yolov2workconfig.json dnn_example_image_width:=960 dnn_example_image_height:=544");
    m_labels[1]->setPixmap(QPixmap(":/icon/images/目标追踪.png"));
    m_labels[1]->setText("");

    m_labels[2] = qobject_cast<MatrixLabel*>(ui->label_02);
    m_labels[2]->setinfo("人体识别");
    m_labels[2]->setCommand("body_session","export CAM_TYPE=mipi && ros2 launch mono2d_body_detection mono2d_body_detection.launch.py & ros2 launch xgo_body_tracker body_tracker.launch.py");
    m_labels[2]->setPixmap(QPixmap(":/icon/images/人体识别.png"));
    m_labels[2]->setText("");

    m_labels[3] = qobject_cast<MatrixLabel*>(ui->label_03);
    m_labels[3]->setinfo("人脸检测");
    m_labels[3]->setCommand("face_detect_session","export CAM_TYPE=mipi &&  ros2 launch xgo_face_control face_with_detection.launch.py");
    m_labels[3]->setPixmap(QPixmap(":/icon/images/人脸检测.png"));
    m_labels[3]->setText("");

    m_labels[4] = qobject_cast<MatrixLabel*>(ui->label_04);
    m_labels[4]->setinfo("关于设备");
    m_labels[4]->setCommand("device_info_session","ros2 run xgo_device device_info_node ");
    m_labels[4]->is_hide_window = true;
    m_labels[4]->setPixmap(QPixmap(":/icon/images/关于设备.png"));
    m_labels[4]->setText("");

    m_labels[5] = qobject_cast<MatrixLabel*>(ui->label_05);
    m_labels[5]->setinfo("年龄识别");
    m_labels[5]->setCommand("face_age_session","export CAM_TYPE=mipi && ros2 launch face_age_detection body_det_face_age_det.launch.py");
    m_labels[5]->setPixmap(QPixmap(":/icon/images/年龄识别.png"));
    m_labels[5]->setText("");

    m_labels[6] = qobject_cast<MatrixLabel*>(ui->label_06);
    m_labels[6]->setinfo("隔空作画");
    m_labels[6]->setCommand("hand_Trail","export CAM_TYPE=mipi && ros2 launch hand_lmk_detection hand_lmk_detection.launch.py");
    m_labels[6]->setPixmap(QPixmap(":/icon/images/隔空作画.png"));
    m_labels[6]->setText("");

    m_labels[7] = qobject_cast<MatrixLabel*>(ui->label_07);
    m_labels[7]->setinfo("语音识别");
    m_labels[7]->setCommand("Audio_detection","ros2 launch hobot_audio hobot_audio.launch.py & ros2 launch xgo_voice_control voice_control.launch.py");
    m_labels[7]->is_hide_window = true;
    m_labels[7]->setPixmap(QPixmap(":/icon/images/语音识别.png"));
    m_labels[7]->setText("");

    m_labels[8] = qobject_cast<MatrixLabel*>(ui->label_08);
    m_labels[8]->setinfo("表演模式");
    m_labels[8]->setCommand("dog_show_session","ros2 run xgo_motion dog_show_combined ");
    m_labels[8]->is_hide_window = true;
    m_labels[8]->setPixmap(QPixmap(":/icon/images/表演模式.png"));
    m_labels[8]->setText("");

    updateSelection();
}

void MainWindow::change_focus(bool direction)
{
  if(direction == true)currentlabel++;
  if(direction == false)currentlabel--;
  if(currentlabel>=LABEL_COUNT) {
    currentlabel = 0;
    // ui->stackedWidget->setCurrentIndex(1);
    // stackedWidget->setCurrentIndex(1); 
    TLog::d(QString("change page"));
  }
  if(currentlabel<0) {
    currentlabel = 8;
    // ui->stackedWidget->setCurrentIndex(0);
    // stackedWidget->setCurrentIndex(0); // 切换到页面 2
    TLog::d(QString("change page"));

  }

  updateSelection();
}

void MainWindow::updateSelection()
{
    // 清除所有高亮
    for(int i=0; i<9; ++i){
      if(m_labels[i]) m_labels[i]->setHighlight(false);  
    }

    // 设置当前高亮
    if(MatrixLabel* current_label = m_labels[currentlabel]){
        current_label->setHighlight(true);
        ui->label->setText(QString(current_label->getinfo()));
    }
}
