#include "rclcomm.h"

rclcomm::rclcomm()  {
  node=rclcpp::Node::make_shared("xgo_head");
  //_publisher = node->create_publisher<std_msgs::msg::Int32>("ros2_qt_demo_publish", 10);
  //_subscription = node->create_subscription<std_msgs::msg::Int32>("ros2_qt_demo_publish", 10,std::bind(&rclcomm::recv_callback,this,std::placeholders::_1));
    rclcpp::QoS qos_profile(10);
    qos_profile.best_effort();
  TLog::d("before battery init.");
  subscriptionBattery = node->create_subscription<std_msgs::msg::Float32>("/xgo_battery", 10,std::bind(&rclcomm::recvBatteryCallback,this,std::placeholders::_1));
  subscriptionAudio = node->create_subscription<audio_msg::msg::SmartAudioData>("/audio_smart", 10,std::bind(&rclcomm::recvSmartAudioCallback,this,std::placeholders::_1));
  subscriptionCompressedImage = node->create_subscription<sensor_msgs::msg::CompressedImage>("/image", 10, std::bind(&rclcomm::recvCompressedImageCallback, this, std::placeholders::_1));
  subscriptionHandGesture = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_hand_gesture_detection", 10,  std::bind(&rclcomm::recvHandGesturCallback, this, std::placeholders::_1));
  subscriptionHandFollow = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_hand_lmk_detection",10,std::bind(&rclcomm::recvHandFollowCallback,this,std::placeholders::_1));
  subscriptionBody = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_mono2d_body_detection",10, std::bind(&rclcomm::recvBodyDecectionCallback,this, std::placeholders::_1));
  subscriptionYolo = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_dnn_detection",10,std::bind(&rclcomm::recvYoloDetectionCallback,this,std::placeholders::_1));
  subscriptionFace106 = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_face_landmarks_detection",10,std::bind(&rclcomm::recvFace106Callback,this,std::placeholders::_1));
  subscriptionFaceAge = node->create_subscription<ai_msgs::msg::PerceptionTargets>("/hobot_face_age_detection",10,std::bind(&rclcomm::recvFaceAgeCallback,this,std::placeholders::_1));
  
  TLog::d("after battery init.");
  this->start();
}
// rclcomm::~rclcomm()
// {
//   std::system("tmux kill-server");
// }
void rclcomm::setWidget(QWidget* widget, QImage* image)
{
  this->mWidget = widget;
  this->mImageBackground = image;
}

void rclcomm::run(){
    std_msgs::msg::Int32 pub_msg;
    pub_msg.data=0;
    rclcpp::WallRate loop_rate(33); //
    while (rclcpp::ok())
    {
        pub_msg.data++;
        // _publisher->publish(pub_msg);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    rclcpp::shutdown();
}

void rclcomm::recv_callback(const std_msgs::msg::Int32::SharedPtr msg){
    emitTopicData("receive ros2 message:"+QString::fromStdString(std::to_string(msg->data)));
}


void rclcomm::recvBatteryCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
  this->mBattery = msg->data;
  emitTopicData("receive battery:"+QString::fromStdString(std::to_string(msg->data)));

}

void rclcomm::recvSmartAudioCallback(const audio_msg::msg::SmartAudioData::SharedPtr msg)
{
  if(msg->frame_type.value == msg->frame_type.SMART_AUDIO_TYPE_CMD_WORD  )
  {
    QString cmd_word = QString::fromStdString(msg->cmd_word);
    TLog::d(QString("receive cmd:%1").arg(cmd_word));
  }
}

void rclcomm::recvCompressedImageCallback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
  qint64 time = QDateTime::currentMSecsSinceEpoch();
  if(time - this->mLastImageTime < this->mImageIntervalMS)
  {
    TLog::d("interval not reach, ignor.");
    return;
  }
  else
  {
    this->mLastImageTime = time;
  }

  if(this->mBusyTempData)
  {
    TLog::w("busy parse jpg.");
    return;
  }

  // Prevent writing frames when in idle state (on main menu)
  if(running_function == -1)
  {
    TLog::d("running_function is -1 (idle), skip frame processing.");
    return;
  }

  this->mBusyTempData = true;

  QString format = QString::fromStdString(msg->format);
  int size = 0;
  size = msg->data.size();
  TLog::d(QString("receive a compressed Image, format is :%1,  size if :%2").arg(format).arg(size));
  if(size > this->mMaxSize)
  {
    TLog::e("The image is too big, return.");
    this->mBusyTempData = false;
    return;
  }

  for(int i=0; i<size; i++)
  {
    this->mTempData[i] = msg->data[i];
  }


  bool result = false;
  QImage tempImage;
  try
  {
    result = tempImage.loadFromData(this->mTempData, size, "JPG");
  }
  catch(...)
  {
    TLog::e("exception, parse jpg failed !");
  }


  if(result)
  {
    // TLog::d("parse jpeg succeed.");
    tempImage = tempImage.scaled(320, 240);
    QPainter painter(&tempImage);
    
  switch(running_function)
  {
    case 0:
          painter.setPen(QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
      this->mHandMutex.lock();
      TLog::d(QString("%1 hand to draw.").arg(this->mHandList.size()));
      if(QDateTime::currentMSecsSinceEpoch() - this->mLastHandTime > 400)
      {
        TLog::d("hand point too old  ,ignor. ");
      }
      else
      {
        for(int i=0; i<this->mHandList.size(); i++)
        {
          // TLog::d(QString("%1 hand has %2 points.").arg(this->mHandList.size()).arg(this->mHandList[i].size()));
          QQueue<QPointF> tempHand;
          //calc and draw points
          for(int j=0; j<this->mHandList[i].size(); j++)
          {
            QPointF p;
            p.setX(this->mHandList[i][j].x() == 0 ? 0 : this->mHandList[i][j].x() * this->mX / this->mPictureX);
            p.setY(this->mHandList[i][j].y() == 0 ? 0 : this->mHandList[i][j].y() * this->mY / this->mPictureY);
            painter.drawPoint(p);
            // TLog::d(QString("draw hand %1 point %2:[%3,%4] ").arg(i).arg(j).arg(p.x()).arg(p.y()));
            tempHand.append(p);
          }
          // draw finger line
          painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempHand[0], tempHand[1]);
          painter.drawLine(tempHand[1], tempHand[5]);
          painter.drawLine(tempHand[5], tempHand[9]);
          painter.drawLine(tempHand[9], tempHand[13]);
          painter.drawLine(tempHand[13], tempHand[17]);
          painter.drawLine(tempHand[17], tempHand[0]);

          painter.setPen(QPen(Qt::green,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempHand[1], tempHand[2]);
          painter.drawLine(tempHand[2], tempHand[3]);
          painter.drawLine(tempHand[3], tempHand[4]);



          painter.setPen(QPen(Qt::yellow,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempHand[5], tempHand[6]);
          painter.drawLine(tempHand[6], tempHand[7]);
          painter.drawLine(tempHand[7], tempHand[8]);


          painter.setPen(QPen(Qt::magenta,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempHand[9], tempHand[10]);
          painter.drawLine(tempHand[10], tempHand[11]);
          painter.drawLine(tempHand[11], tempHand[12]);


          painter.setPen(QPen(Qt::gray,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempHand[13], tempHand[14]);
          painter.drawLine(tempHand[14], tempHand[15]);
          painter.drawLine(tempHand[15], tempHand[16]);

          painter.setPen(Qt::black);
          painter.drawLine(tempHand[17], tempHand[18]);
          painter.drawLine(tempHand[18], tempHand[19]);
          painter.drawLine(tempHand[19], tempHand[20]);
        }
        // tempImage.save("/home/sunrise/temp.jpg");
      }

      this->mHandMutex.unlock();
      break;
    
      case 1:
      this->mYoloMutex.lock();
      painter.setPen(QPen(Qt::blue,4,Qt::SolidLine,Qt::RoundCap));
      if(QDateTime::currentMSecsSinceEpoch() - this->mLastYoloTime >400)
      {
        TLog::d("yoloPoint too old, ignor");  
      }
      else
      {
        for(int i=0; i<this->mYoloList.size(); i++)
        {
          QRect r;
          r.moveLeft(this->mYoloList[i].x() == 0 ? 0 : this->mYoloList[i].x() * this->mX / this->mPictureX);
          r.moveTop(this->mYoloList[i].y() == 0 ? 0 : this->mYoloList[i].y() * this->mY / this->mPictureY);
          r.setWidth(this->mYoloList[i].width() == 0 ? 0 : this->mYoloList[i].width() * this->mX / this->mPictureX);
          r.setHeight(this->mYoloList[i].height() == 0 ? 0 : this->mYoloList[i].height() * this->mY / this->mPictureY);
          painter.drawRect(r);
                    
          
          painter.save();
          if(this->mYoloList.size()>i)
          {
            // painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
            QPointF textPoint;
            textPoint.setX(this->mYoloList[i].x() == 0 ? 0 : this->mYoloList[i].x() * this->mX / this->mPictureX *(-1));
            textPoint.setY(this->mYoloList[i].y() == 0 ? 0 : this->mYoloList[i].y() * this->mY / this->mPictureY);
            // painter.translate(this->mPictureX, 0);
            painter.scale(-1, 1);
            painter.drawText(textPoint ,mYoloType[i]);      
            // painter.drawText(mYoloList[i].topLeft(),);      
          }
          
          painter.restore();


        }
      }
      this->mYoloMutex.unlock();
      break;

    case 2:
      this->mBodyMutex.lock();
      painter.setPen(QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
      // this->mHandMutex.lock();
      TLog::d(QString("%1 body to draw.").arg(this->mBodyList.size()));
      if(QDateTime::currentMSecsSinceEpoch() - this->mLastBodyTime >400)
      {
        TLog::d("bodyPoint too old, ignor");  
      }
      else
      {
        for(int i=0; i<this->mBodyList.size(); i++)
        {
          // TLog::d(QString("%1 hand has %2 points.").arg(this->mHandList.size()).arg(this->mHandList[i].size()));
          QQueue<QPointF> tempBody;
          //calc and draw points
          for(int j=0; j<this->mBodyList[i].size(); j++)
          {
            QPointF p;
            p.setX(this->mBodyList[i][j].x() == 0 ? 0 : this->mBodyList[i][j].x() * this->mX / this->mPictureX);
            p.setY(this->mBodyList[i][j].y() == 0 ? 0 : this->mBodyList[i][j].y() * this->mY / this->mPictureY);
            painter.drawPoint(p);
            // TLog::d(QString("draw hand %1 point %2:[%3,%4] ").arg(i).arg(j).arg(p.x()).arg(p.y()));
            tempBody.append(p);
          }
          // draw finger line
          painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
          painter.drawLine(tempBody[0], tempBody[1]);
          painter.drawLine(tempBody[2], tempBody[1]);
          painter.drawLine(tempBody[0], tempBody[2]);
          painter.drawLine(tempBody[2], tempBody[4]);
          painter.drawLine(tempBody[1], tempBody[3]);
          
        }
      }
      this->mBodyMutex.unlock();
      break;
    case 3:
      this->mFaceMutex.lock();
      painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap)); // Increased pen width from 3 to 5
      TLog::d(QString("%1 face to draw. mX=%2, mY=%3, mPictureX=%4, mPictureY=%5").arg(this->mFaceList.size()).arg(this->mX).arg(this->mY).arg(this->mPictureX).arg(this->mPictureY));
      if(QDateTime::currentMSecsSinceEpoch() - this->mLastFaceTime >3000)
      {
        TLog::d("facePoint too old, ignor");  
      }
      else
      {
        for(int i=0; i<this->mFaceList.size(); i++)
        {
          // TLog::d(QString("%1 hand has %2 points.").arg(this->mHandList.size()).arg(this->mHandList[i].size()));
          QQueue<QPointF> tempFace;
          //calc and draw points
          for(int j=0; j<this->mFaceList[i].size(); j++)
          {
            QPointF p;
            double ox = this->mFaceList[i][j].x();
            double oy = this->mFaceList[i][j].y();
            // if input is normalized (0..1), scale to source image size first
            if (ox > 0 && ox <= 1.5 && oy > 0 && oy <= 1.5) {
              ox *= this->mPictureX;
              oy *= this->mPictureY;
            }
            p.setX(ox == 0 ? 0 : ox * this->mX / this->mPictureX);
            p.setY(oy == 0 ? 0 : oy * this->mY / this->mPictureY);
            painter.drawPoint(p);
            // Add debug output for first 5 points
            if(j < 5) TLog::d(QString("draw face %1 point %2: orig[%3,%4] -> screen[%5,%6]").arg(i).arg(j).arg(this->mFaceList[i][j].x()).arg(this->mFaceList[i][j].y()).arg(p.x()).arg(p.y()));
            tempFace.append(p);
          }
          // draw finger line
          // painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
          // painter.drawLine(tempFace[0], tempFace[1]);
          // painter.drawLine(tempFace[2], tempFace[1]);
          // painter.drawLine(tempFace[0], tempFace[2]);
          // painter.drawLine(tempFace[2], tempFace[4]);
          // painter.drawLine(tempFace[1], tempFace[3]);
          
        }
      }
      this->mFaceMutex.unlock();
      break;
    case 5:
      this->mFaceAgeMutex.lock();
      painter.setPen(QPen(Qt::blue,3,Qt::SolidLine,Qt::RoundCap)); // Increased pen width from 2 to 3
      if(QDateTime::currentMSecsSinceEpoch() - this->mLastFaceAgeTime >3000)
      {
        TLog::d("FaceAge Rect too old, ignor");  
      }
      else
      {
        for(int i=0; i<this->mFaceAgeList.size(); i++)
        {
          // Extract and normalize if needed
          double ox = this->mFaceAgeList[i].x();
          double oy = this->mFaceAgeList[i].y();
          double ow = this->mFaceAgeList[i].width();
          double oh = this->mFaceAgeList[i].height();
          if (ow > 0 && ow <= 1.5 && oh > 0 && oh <= 1.5) {
            ox *= this->mPictureX; oy *= this->mPictureY; ow *= this->mPictureX; oh *= this->mPictureY;
          }

          QRect r;
          r.moveLeft(ox == 0 ? 0 : ox * this->mX / this->mPictureX);
          r.moveTop(oy == 0 ? 0 : oy * this->mY / this->mPictureY);
          r.setWidth(ow == 0 ? 0 : ow * this->mX / this->mPictureX);
          r.setHeight(oh == 0 ? 0 : oh * this->mY / this->mPictureY);
          TLog::d(QString("draw age rect[%1]: x=%2 y=%3 w=%4 h=%5").arg(i).arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height()));
          painter.drawRect(r);
        }

        // Draw age text at bottom-right corner, not following the box
        if (!this->mFaceAge.isEmpty()) {
          QString ageLabel = QString("age: %1").arg(this->mFaceAge.front());
          int margin = 12;
          painter.save();
          QFont f = painter.font();
          f.setPixelSize(28);
          painter.setFont(f);
          QPointF textPoint;
          // Pre-flip X so that after final image flip, text appears at bottom-right
          textPoint.setX(-1 * (this->mX - margin));
          textPoint.setY(this->mY - margin);
          painter.scale(-1, 1);
          TLog::d(QString("draw global age text '%1' at bottom-right (%2,%3)").arg(ageLabel).arg(textPoint.x()).arg(textPoint.y()));
          painter.drawText(textPoint, ageLabel);
          painter.restore();
        }
      }
      this->mFaceAgeMutex.unlock();
      break;
      
    case 6: {
      for (auto it = fingerTrails.begin(); it != fingerTrails.end(); ++it) 
      {
        const uint64_t& handId = it.key();           // 获取手掌 ID
        const auto& trail = it.value();              // 获取轨迹

        TLog::d("get point from list");

        // 设置画笔
        QPen pen;
        pen.setWidth(4);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        int colorIndex = handId % 6;
        QColor colors[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan};
        pen.setColor(colors[colorIndex]);
        painter.setPen(pen);

        TLog::d("set color complete");

        auto it_point = trail.begin();
        if (it_point == trail.end()) continue;

        // 当前点
        QPointF prev_pos = it_point->pos;
        ++it_point;

        double prevX = (prev_pos.x() != 0) ? prev_pos.x() * mX / mPictureX : 0;
        double prevY = (prev_pos.y() != 0) ? prev_pos.y() * mY / mPictureY : 0;

        for (; it_point != trail.end(); ++it_point) 
        {
          const QPointF& curr_pos = it_point->pos;

          double currX = (curr_pos.x() != 0) ? curr_pos.x() * mX / mPictureX : 0;
          double currY = (curr_pos.y() != 0) ? curr_pos.y() * mY / mPictureY : 0;

          // TLog::d("draw line from (%f,%f) to (%f,%f)", prevX, prevY, currX, currY);


          painter.drawLine(QPointF(prevX, prevY), QPointF(currX, currY));

          prevX = currX;
          prevY = currY;
        }
      }
      break;
    }
    case 7:
      
      break;

    default:
      TLog::d("this function have not been built , sorry about that");

  }    


  painter.end();  
  QTransform tImage;
  tImage.scale(-1,1);

  *this->mImageBackground = tempImage.transformed(tImage);
  }
  else
  {
    TLog::e("parse jpeg failed!");
  }
  this->mBusyTempData = false;
}



void rclcomm::recvHandGesturCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  TLog::d("receive a gesture. ");

  //clear last hand.
  this->mHandMutex.lock();
  // this->mHandList.clear();
  bool clearOld = false;

  auto targetList = msg->targets;
  int targetNum = targetList.size();
  // TLog::d(QString("target list size : %1").arg(targetNum));
  for(int i=0; i<targetNum; i++)
  {
    auto pointList = targetList[i].points;
    int pointListNum = pointList.size();
    // TLog::d(QString("target %1, point list size : %2").arg(i).arg(pointListNum));
    for(int j=0; j<pointListNum; j++)
    {
      QString pointType = QString::fromStdString(pointList[j].type);
      // TLog::d(QString("target %1, point %2 / %3,  type: %4").arg(i).arg(j).arg(pointListNum).arg(pointType));
      if(pointType == "hand_kps")
      {
        if(clearOld == false) //clear the old data on the first time.
        {
          this->mHandList.clear();
          clearOld = true;
          this->mLastHandTime = QDateTime::currentMSecsSinceEpoch();
        }

          auto handPoints = pointList[j].point;
          if(handPoints.size() == 21)
          {
              TLog::d(QString("hand points num  %1 , save.").arg(handPoints.size()));
             QQueue<QPointF> hand;
              for(int k=0; k<21; k++)
              {
                QPointF point;
                point.setX(handPoints[k].x);
                point.setY(handPoints[k].y);
                hand.append(point);
              }
              this->mHandList.append(hand);
          }
          else
          {
            TLog::e(QString("bad hand points num  %1 , ignor.").arg(handPoints.size()));
          }

      }
    }

  }




  this->mHandMutex.unlock();
  TLog::d(QString("total saved %1 hands.").arg(this->mHandList.size()));



}

void rclcomm::recvHandFollowCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  TLog::d("receive a hand");
  this->mFollowMutex.lock();
  // auto targetList = msg->targets;
  // auto targetNum = targetList.size();
  qint64 now = QDateTime::currentMSecsSinceEpoch();
  for (auto& target : msg->targets) {
    if (target.type != "person") continue;
            // TLog::d("get");
            // 提取食指指尖
    std::optional<QPointF> indexTip = extractIndexFingerTip(target);
    if (!indexTip) continue;
    uint64 trackId = target.track_id;
    TrackedPoint p;
    p.pos = *indexTip;
    p.timestamp = now;
    this->fingerTrails[trackId].enqueue(p);
            // TLog::d("add point");
    auto& trail = this->fingerTrails[trackId];
    while (!trail.isEmpty()) 
    {
      if (now - trail.head().timestamp > 2000) 
      {
        TLog::d("delete point");
        trail.dequeue();
      } 
      else 
      {
        break;
      }
    }
  }

  cleanupOldTracks(now);
  this->mFollowMutex.unlock();
}

std::optional<QPointF> rclcomm::extractIndexFingerTip(ai_msgs::msg::Target target) {
  auto pointList = target.points;
  int pointListNum = pointList.size();
  for (int i = 0;i<pointListNum; i++) {
    auto handPoint = pointList[i].point;
    if (pointList[i].type == "hand_kps") {
      // TLog::d(QString("pointlist:%1").arg(handPoint.size()));

      if (handPoint.size() == 21) {

        QPointF pt ; 
        pt.setX(handPoint[8].x);
        pt.setY(handPoint[8].y);


        return pt;
      }
     
    }
  }
  return std::nullopt;
}

void rclcomm::cleanupOldTracks(qint64 now) {
  QList<uint64> toRemove;
  for (auto it = fingerTrails.begin(); it != fingerTrails.end(); ++it) {
    if (it.value().isEmpty()) continue;
    if (now - it.value().back().timestamp > 5000) { 
      toRemove.append(it.key());
    }
  }
  for (uint64 id : toRemove) {
    fingerTrails.remove(id);
  }
}


void rclcomm::recvBodyDecectionCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  TLog::d("receive a body");

  this->mBodyMutex.lock();
  bool clearOld = false;
  auto targetList = msg->targets;
  int targetNum = targetList.size();

  // auto targetList = msg->targets;
  // int targetNum = targetList.size();
  // TLog::d(QString("target list size : %1").arg(targetNum));
  for(int i=0; i<targetNum; i++)
  {
    auto pointList = targetList[i].points;
    int pointListNum = pointList.size();
    // TLog::d(QString("target %1, point list size : %2").arg(i).arg(pointListNum));
    for(int j=0; j<pointListNum; j++)
    {
      QString pointType = QString::fromStdString(pointList[j].type);
      // TLog::d(QString("target %1, point %2 / %3,  type: %4").arg(i).arg(j).arg(pointListNum).arg(pointType));
      if(pointType == "body_kps")
      {
        if(clearOld == false) //clear the old data on the first time.
        {
          this->mBodyList.clear();
          clearOld = true;
          this->mLastBodyTime = QDateTime::currentMSecsSinceEpoch();
        }

          auto bodyPoints = pointList[j].point;
          if(bodyPoints.size() == 19)
          {
              TLog::d(QString("body points num  %1 , save.").arg(bodyPoints.size()));
             QQueue<QPointF> body;
              for(int k=0; k<19; k++)
              {
                QPointF point;
                point.setX(bodyPoints[k].x);
                point.setY(bodyPoints[k].y);
                body.append(point);
              }

              this->mBodyList.append(body);
          }
          else
          {
            TLog::e(QString("bad body points num  %1 , ignor.").arg(bodyPoints.size()));
          }

      }
    }

  }
  this->mBodyMutex.unlock();

    
}

void rclcomm::recvYoloDetectionCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  this->mYoloMutex.lock();
  bool clearOld = false;
  auto targetList = msg->targets;
  int targetNum = targetList.size();
   for(int i=0; i<targetNum; i++)
  {
    if(clearOld == false) //clear the old data on the first time.
      {
        this->mYoloList.clear();
        this->mYoloType.clear();
        clearOld = true;
        TLog::d("clear old YOLO data");
        this->mLastYoloTime = QDateTime::currentMSecsSinceEpoch();
      }
    auto yoloRoi = targetList[i].rois;
    int roiNum = yoloRoi.size();
    // TLog::d(QString("rois number : %1").arg(roiNum));
    for(int j = 0; j < roiNum; j++)
    {
      auto yoloConfidence = yoloRoi[j].confidence;
      if(yoloConfidence >= 0.6 )
      {
        auto yoloRect =  yoloRoi[j].rect;
        QRect rect(yoloRect.x_offset,yoloRect.y_offset,yoloRect.width,yoloRect.height);
        QString text = QString::fromStdString(targetList[i].type);
        mYoloList.append(rect);
        mYoloType.append(text);
      }
      
    }
    

  }
  this->mYoloMutex.unlock();
}

void rclcomm::recvFace106Callback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  TLog::d("receive a face");

  this->mFaceMutex.lock();
  bool clearOld = false;
  auto targetList = msg->targets;
  int targetNum = targetList.size();

  // auto targetList = msg->targets;
  // int targetNum = targetList.size();
  // TLog::d(QString("target list size : %1").arg(targetNum));
  for(int i=0; i<targetNum; i++)
  {
    auto pointList = targetList[i].points;
    int pointListNum = pointList.size();
    // TLog::d(QString("target %1, point list size : %2").arg(i).arg(pointListNum));
    for(int j=0; j<pointListNum; j++)
    {
      QString pointType = QString::fromStdString(pointList[j].type);
      // TLog::d(QString("target %1, point %2 / %3,  type: %4").arg(i).arg(j).arg(pointListNum).arg(pointType));
      if(pointType == "face_kps")
      {
        if(clearOld == false) //clear the old data on the first time.
        {
          this->mFaceList.clear();
          clearOld = true;
          this->mLastFaceTime = QDateTime::currentMSecsSinceEpoch();
        }

          auto facePoints = pointList[j].point;
          if(facePoints.size() == 106)
          {
              TLog::d(QString("Face points num  %1 , save.").arg(facePoints.size()));
             QQueue<QPointF> face;
              for(int k=0; k< 106; k++)
              {
                QPointF point;
                point.setX(facePoints[k].x);
                point.setY(facePoints[k].y);
                face.append(point);
              }
              this->mFaceList.append(face);
          }
          else
          {
            TLog::e(QString("bad Face points num  %1 , ignor.").arg(facePoints.size()));
          }

      }
    }

  }
  this->mFaceMutex.unlock();
}


void rclcomm::recvFaceAgeCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg)
{
  this->mFaceAgeMutex.lock();
  bool clearOld = false;
  auto targetList = msg->targets;
  int targetNum = targetList.size();
   for(int i=0; i<targetNum; i++)
  {
    if(clearOld == false) //clear the old data on the first time.
      {
        this->mFaceAgeList.clear();
        this->mFaceAge.clear();
        clearOld = true;
        TLog::d("clear old face data");
        this->mLastFaceAgeTime = QDateTime::currentMSecsSinceEpoch();
      }
    auto faceRoi = targetList[i].rois;
    auto faceAge = targetList[i].attributes;

    int roiNum = faceRoi.size();
    int ageNum = faceAge.size();

    TLog::d(QString("rois number : %1,face number:%2").arg(roiNum).arg(ageNum));
    for(int j = 0; j < roiNum; j++)
    {
      // auto faceConfidence = faceRoi[j].confidence;
      auto faceRect =  faceRoi[j].rect;

      QRect rect(faceRect.x_offset,faceRect.y_offset,faceRect.width,faceRect.height);
      mFaceAgeList.append(rect);
    }
    
    for(int j = 0; j<ageNum;j++)
    {
      auto age = faceAge[j].value;
      QString text = QString::number(age);
      TLog::d(QString("get age:%1").arg(age));
      mFaceAge.append(text);
    }
  }
  this->mFaceAgeMutex.unlock();
}

void rclcomm::change_running(int runing)
{
  running_function = runing;
 
  if (this->mImageBackground) {
    // 先清黑
    this->mImageBackground->fill(Qt::black);
    if (running_function != -1) {
    
      this->mImageBackground->setPixel(0, 0, qRgb(1, 0, 0));
    }
  }
}
float rclcomm::get_battery()
{
  return mBattery;
}