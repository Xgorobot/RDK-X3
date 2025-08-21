#ifndef RCLCOMM_H
#define RCLCOMM_H
#include <QObject>
#include <QThread>
#include <QImage>
#include <QWidget>
#include <QPalette>
#include <QPainter>
#include <QDateTime>
#include <QQueue>
#include <QMutex>
#include <QPointF>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "rclcpp/qos.hpp"
#include <QImage>
#include <QDateTime>
#include <QMutexLocker>
#include <opencv2/opencv.hpp>
#include <cmath>     

#include "hbm_img_msgs/msg/hbm_msg1080_p.hpp"
#include "audio_msg/msg/smart_audio_data.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "ai_msgs/msg/perception_targets.hpp"


#include "TLog.h"

struct TrackedPoint {//for hand draw
    QPointF pos;
    qint64 timestamp;  
};


class rclcomm : public QThread
{
    Q_OBJECT
public:
    rclcomm();
    void setWidget(QWidget* widget, QImage* image);
    void publish_topic(int count);
    void recv_callback(const std_msgs::msg::Int32::SharedPtr msg);

    void recvBatteryCallback(const std_msgs::msg::Float32::SharedPtr msg);
    // void rehbmImageCallback(const hbm_img_msgs::msg::HbmMsg1080P::SharedPtr msg);
    void recvSmartAudioCallback(const audio_msg::msg::SmartAudioData::SharedPtr msg);
    void recvCompressedImageCallback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    void recvHandGesturCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    void recvBodyDecectionCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    void recvYoloDetectionCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    void recvFaceAgeCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    void recvFace106Callback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    void recvHandFollowCallback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);
    float get_battery();
    void change_running(int runing);
    std::optional<QPointF> extractIndexFingerTip(ai_msgs::msg::Target target);
    void cleanupOldTracks(qint64 now);


protected:
    void run();

private:
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr _publisher;
    rclcpp::Subscription<audio_msg::msg::SmartAudioData>::SharedPtr subscriptionAudio;


    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscriptionBattery;

    rclcpp::Subscription<audio_msg::msg::SmartAudioData>::SharedPtr subscriptionAudioCommand;
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscriptionCompressedImage;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionHandGesture;
    // rclcpp::Subscription<hbm_img_msgs::msg::HbmMsg1080P>::SharedPtr subscriptionHbmImage;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionBody;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionYolo;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionFace106;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionFaceAge;
    rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr subscriptionHandFollow;
    std::shared_ptr<rclcpp::Node> node;

    int mMaxSize = 1024 * 1024 * 1024;
    uchar mTempData[1024 * 1024 * 1024];
    bool  mBusyTempData = false;
    float mBattery = 0;
    QWidget* mWidget = nullptr;
    QImage* mImageBackground = nullptr;
    int mX = 320;
    int mY = 240;
    int mPictureX = 960;
    int mPictureY = 544; // image size;

    qint64 mLastImageTime = 0;
    int mImageIntervalMS = 100; //100ms one pic.


    QQueue<QQueue<QPointF>> mHandList;
    QQueue<QQueue<QPointF>> mBodyList;
    QQueue<QQueue<QPointF>> mFaceList;
    QQueue<QString> mYoloType;
    QQueue<QRect> mYoloList;
    QQueue<QString> mFaceAge;
    QQueue<QRect> mFaceAgeList;
    QHash<uint64, QQueue<TrackedPoint>> fingerTrails;
    int mHandPointNum = 21;
    int mBodyPointNum = 18;
    int mFacePointNum = 106; // todo:count

    QMutex mHandMutex;
    QMutex mFollowMutex;
    QMutex mBodyMutex;
    QMutex mYoloMutex;
    QMutex mFaceMutex;
    QMutex mFaceAgeMutex;

    qint64 mLastHandTime = 0;
    qint64 mLastBodyTime = 0;
    qint64 mLastYoloTime = 0;
    qint64 mLastFaceTime = 0,mLastFaceAgeTime = 0;

    int running_function = -1;

signals:
    void emitTopicData(QString);
};


#endif // RCLCOMM_H
