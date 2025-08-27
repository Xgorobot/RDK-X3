#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <audio_msg/msg/smart_audio_data.hpp>

#include "XGO.h"
#include <xgoscreen/XgoScreenManager.h>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QThread>
#include <QPainter>
#include <QFont>
#include <QRect>
#include <QTimer>

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <cmath>
#include <atomic>
#include <thread>
#include <algorithm>
using audio_msg::msg::SmartAudioData;

class VoiceControlNode : public rclcpp::Node {
public:
  VoiceControlNode(QWidget* widget, XgoScreenManager* screen_manager, const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("xgo_voice_control", options), widget_(widget), screen_manager_(screen_manager) {
    // Declare parameters
    port_ = this->declare_parameter<std::string>("port", "/dev/ttyS3");
    baud_ = this->declare_parameter<int>("baud", 115200);
    version_ = this->declare_parameter<std::string>("version", "xgolite");
    verbose_ = this->declare_parameter<bool>("verbose", false);
    fast_startup_ = this->declare_parameter<bool>("fast_startup", false);

    RCLCPP_INFO(this->get_logger(), "xgo_voice_control starting, port=%s, baud=%d, version=%s, fast_startup=%s",
                port_.c_str(), baud_, version_.c_str(), fast_startup_ ? "true" : "false");

    // Init XGO
    try {
      xgo_ = std::make_unique<XGO>(port_, baud_, version_, verbose_, fast_startup_);
    } catch (const std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Failed to open XGO on %s: %s", port_.c_str(), e.what());
    }

    // Init screen (Qt)
    if (screen_manager_ && widget_) {
      if (!screen_manager_->isRunning()) {
        screen_manager_->initialize(1, 0, 20000000, 5, 13, 29, 100, fast_startup_);
      }
      screen_manager_->setBackgroundColor(Qt::black);
    }
    reminder_ = u8"向前走|向后退|向左移动|向右移动|正转|反转";

    if (screen_manager_) {
      QTimer::singleShot(0, screen_manager_, [this]() {
        this->drawText("等待语音指令…");
      });
    }

    // Subscribe to /audio_smart
    smart_sub_ = this->create_subscription<SmartAudioData>(
      "/audio_smart", 10,
      std::bind(&VoiceControlNode::onSmartAudio, this, std::placeholders::_1));

    // Timer to periodically refresh UI (optional)
    timer_ = this->create_wall_timer(std::chrono::milliseconds(100), [this]() {
      // Reserved for future UI refresh tasks
    });


    rclcpp::on_shutdown([this]() {
      shutting_down_.store(true, std::memory_order_relaxed);
      if (xgo_) {
        try { xgo_->stop(); } catch (...) {}
        try { xgo_->reset(); } catch (...) {}
      }
    });
  }


  ~VoiceControlNode() {
    if (xgo_) {
      try { xgo_->stop(); } catch (...) {}
      try { xgo_->reset(); } catch (...) {}
    }
  }


  void emergencyStop() { 
    shutting_down_.store(true, std::memory_order_relaxed);
    stopAndReset(); 
  }

private:
  void drawText(const std::string &text) {
    if (!screen_manager_) return;
    QImage* bg = screen_manager_->getBackgroundImagePointer();
    if (!bg) return;
    if (bg->size() != QSize(320, 240)) {
      *bg = QImage(320, 240, QImage::Format_RGB32);
    }
    bg->fill(Qt::black);

    QPainter painter(bg);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QFont font;
    font.setPointSize(16);
    painter.setFont(font);
    painter.setPen(Qt::white);
    QRect textRect(0, 40, 320, 120);
    painter.drawText(textRect, Qt::AlignCenter, QString::fromUtf8(text.c_str()));

    QFont smallFont;
    smallFont.setPointSize(12);
    painter.setFont(smallFont);
    painter.setPen(Qt::yellow);
    QRect reminderRect(0, 180, 320, 40);
    painter.drawText(reminderRect, Qt::AlignCenter, QString::fromUtf8(reminder_.c_str()));

    painter.end();
    bg->setPixelColor(0, 0, QColor(1, 1, 1));
  }

  void onSmartAudio(const SmartAudioData::SharedPtr msg) {
    // Only process cmd word frames
    if (msg->frame_type.value != msg->frame_type.SMART_AUDIO_TYPE_CMD_WORD) {
      return;
    }
    if (shutting_down_.load(std::memory_order_relaxed)) return;

    const std::string cmd = msg->cmd_word;

    // Update screen with recognized command on Qt GUI thread
    if (screen_manager_) {
      QTimer::singleShot(0, screen_manager_, [this, cmd]() {
        this->drawText(cmd);
      });
    }

    auto launchAsync = [this](auto fn) {
      if (action_in_progress_.load(std::memory_order_relaxed)) {
        return;
      }
      action_in_progress_.store(true, std::memory_order_relaxed);
      std::thread([this, fn]() mutable {
        try {
          if (!shutting_down_.load(std::memory_order_relaxed)) {
            fn();
          }
        } catch (const std::exception &e) {
          RCLCPP_ERROR(this->get_logger(), "Action failed: %s", e.what());
        }
        action_in_progress_.store(false, std::memory_order_relaxed);
      }).detach();
    };

    // Map and execute
    if (cmd == u8"向前走") {
      launchAsync([this]() { moveForward(20); });
    } else if (cmd == u8"向后退") {
      launchAsync([this]() { moveBackward(20); });
    } else if (cmd == u8"向左移动") {
      launchAsync([this]() { moveLeft(20); });
    } else if (cmd == u8"向右移动") {
      launchAsync([this]() { moveRight(20); });
    } else if (cmd == u8"正转") {
      launchAsync([this]() { turnRight(360.0f); });
    } else if (cmd == u8"反转") {
      launchAsync([this]() { turnLeft(360.0f); });
    } else if (cmd == u8"退出" || cmd == u8"退出指令") {
      emergencyStop();
      QTimer::singleShot(0, screen_manager_, [](){ QCoreApplication::quit(); });
      rclcpp::shutdown();
    } else {
      RCLCPP_INFO(this->get_logger(), "Unrecognized cmd: %s", cmd.c_str());
    }
  }

  void moveForward(float centimeters) {
    if (xgo_ && !shutting_down_.load(std::memory_order_relaxed)) xgo_->move_x_by(centimeters);
  }

  void moveBackward(float centimeters) {
    moveForward(-centimeters);
  }

  void moveLeft(float centimeters) {
    if (xgo_ && !shutting_down_.load(std::memory_order_relaxed)) xgo_->move_y_by(centimeters);
  }

  void moveRight(float centimeters) {
    moveLeft(-centimeters);
  }

  void turnLeft(float degrees) {
    if (xgo_ && !shutting_down_.load(std::memory_order_relaxed)) xgo_->turn_by(degrees, 0.5f);
  }

  void turnRight(float degrees) {
    turnLeft(-degrees);
  }

  void stopAndReset() {
    if (xgo_) {
      try {
        xgo_->stop();
      } catch (...) {}
      try {
        xgo_->reset();
      } catch (...) {}
    }
    if (screen_manager_) {
      QTimer::singleShot(0, screen_manager_, [this]() {
        this->drawText("已停止并复位");
      });
    }
  }

private:
  std::unique_ptr<XGO> xgo_;
  QWidget* widget_ {nullptr};
  XgoScreenManager* screen_manager_ {nullptr};

  rclcpp::Subscription<SmartAudioData>::SharedPtr smart_sub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::string port_;
  int baud_;
  std::string version_;
  bool verbose_;
  bool fast_startup_;

  std::string reminder_;

  std::atomic<bool> shutting_down_{false};
  std::atomic<bool> action_in_progress_{false};
};

int main(int argc, char ** argv) {
  // Create Qt application and a dummy widget for screen manager
  int qt_argc = 0; char** qt_argv = nullptr;
  QApplication app(qt_argc, qt_argv);
  QWidget dummyWidget; // not shown, used for grab and palette
  dummyWidget.setFixedSize(320, 240);
  dummyWidget.show();

  XgoScreenManager screenMgr(&dummyWidget);

  // Start ROS 2
  rclcpp::init(argc, argv);
  auto node = std::make_shared<VoiceControlNode>(&dummyWidget, &screenMgr);

  QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [node]() {
    node->emergencyStop();
    rclcpp::shutdown();
  });

  // Run ROS spinning in a separate thread so that Qt event loop can run for ScreenThread timers
  std::thread ros_thread([&]() {
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    exec.spin();
    exec.remove_node(node);
    rclcpp::shutdown();
  });

  int ret = app.exec();

  node->emergencyStop();
  rclcpp::shutdown();

  if (ros_thread.joinable()) ros_thread.join();
  screenMgr.stop();
  return ret;
}