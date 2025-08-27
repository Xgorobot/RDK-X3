#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <string>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QImage>
#include <QDir>
#include <QPainter>
#include <thread>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include "XGO.h"
#include "xgoscreen/XgoScreenManager.h"
#include <csignal>

namespace fs = std::filesystem;

class DogShowCombinedNode; // forward declaration
static DogShowCombinedNode* g_dog_node = nullptr; // global pointer for signal handler
static void handle_signal(int sig);

class DogShowCombinedNode : public rclcpp::Node {
public:
  DogShowCombinedNode() : Node("dog_show_combined_node") {
    port_ = this->declare_parameter<std::string>("port", "/dev/ttyS3");
    int baud = this->declare_parameter<int>("baud", 115200);
    version_ = this->declare_parameter<std::string>("version", "xgolite");
    bool verbose = this->declare_parameter<bool>("verbose", false);
    bool fast_startup = this->declare_parameter<bool>("fast_startup", false);

    RCLCPP_INFO(this->get_logger(), "xgo_motion starting, port=%s, baud=%d, version=%s", port_.c_str(), baud, version_.c_str());

    
    dummy_widget_ = std::make_unique<QWidget>();
    dummy_widget_->setFixedSize(320, 240);
    dummy_widget_->show();

    // 初始化 XGO 和 XgoScreenManager
    try {
      xgo_ = std::make_unique<XGO>(port_, baud, version_, verbose, fast_startup);
      xgo_->reset();
      xgo_->perform(1);
      RCLCPP_INFO(this->get_logger(), "Started perform mode 1.");
    } catch (const std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Failed to init XGO: %s", e.what());
    }

    // 初始化屏幕管理器
    screen_manager_ = std::make_unique<XgoScreenManager>(dummy_widget_.get());
    if (!screen_manager_->initialize(1, 0, 20000000, 5, 13, 29, 100, fast_startup)) {
      RCLCPP_ERROR(this->get_logger(), "Failed to initialize XgoScreenManager");
      return;
    }

    // 加载表情资源路径
    try {
      expression_path_ = ament_index_cpp::get_package_share_directory("xgo_motion") + "/expression";
      RCLCPP_INFO(this->get_logger(), "Expression path: %s", expression_path_.c_str());
    } catch (const std::exception& e) {
      RCLCPP_ERROR(this->get_logger(), "Failed to get package share directory: %s", e.what());
    }

    // 播放背景音乐
    play_music("Dream_48k_stereo.wav");

    // 定义表情序列
    expression_sequence_ = {
      {"sad", 14},
      {"naughty", 14},
      {"boring", 14},
      {"angry", 13},
      {"shame", 11},
      {"surprise", 15},
      {"happy", 12},
      {"sleepy", 19},
      {"seek", 12},
      {"lookaround", 12},
      {"love", 13},
      {"awkwardness", 11},
      {"eyes", 15},
      {"guffaw", 8},
      {"query", 7},
      {"Shakehead", 7},
      {"Stun", 8},
      {"wronged", 14}
    };

    current_expression_index_ = 0;
    current_frame_index_ = 0;
    
    // 开始表情播放定时器
    expression_timer_ = this->create_wall_timer(std::chrono::milliseconds(100), [this]() {
      play_expression_frame();
    });

    // 设置全局指针用于信号处理
    g_dog_node = this;
  }

  ~DogShowCombinedNode() override {
    stopMotion();
    // 停止音乐
    if (music_pid_ != -1) {
      system(("kill " + std::to_string(music_pid_)).c_str());
    }
    
    // 停止屏幕管理器
    if (screen_manager_) {
      screen_manager_->stop();
    }
    g_dog_node = nullptr;
  }

  void stopMotion() {
    if (xgo_) {
      try {
        xgo_->perform(0);
        RCLCPP_INFO(this->get_logger(), "Stopped perform mode.");
      } catch (...) {}
    }
  }

private:
  void play_music(const std::string& music_file) {
    std::string music_path = ament_index_cpp::get_package_share_directory("xgo_motion") + "/music/" + music_file;
    if (fs::exists(music_path)) {
        std::string escaped_path = "\"" + music_path + "\"";
        std::string command = "aplay -D hw:0,0 " + escaped_path;
        pid_t pid = fork();
        if (pid == 0) {
            execl("/bin/sh", "sh", "-c", command.c_str(), (char *) NULL);
            _exit(127);
        } else if (pid > 0) {
            music_pid_ = pid;
        }
        RCLCPP_INFO(this->get_logger(), "Started music: %s", music_file.c_str());
    } else {
        RCLCPP_WARN(this->get_logger(), "Music file not found: %s", music_path.c_str());
    }
  }

  void play_expression_frame() {
    if (expression_sequence_.empty()) return;
    
    auto& [expression_name, frame_count] = expression_sequence_[current_expression_index_];
    
    std::string img_path = expression_path_ + "/" + expression_name + "/" + std::to_string(current_frame_index_ + 1) + ".png";
    
    if (fs::exists(img_path)) {
    
      QImage src(QString::fromStdString(img_path));
      if (!src.isNull()) {
   
        QImage canvas(320, 240, QImage::Format_RGB32);
        canvas.fill(Qt::black);

        QSize targetSize = src.size();
        targetSize.scale(320, 240, Qt::KeepAspectRatio);
        QImage scaled = src.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        int x = (320 - scaled.width()) / 2;
        int y = (240 - scaled.height()) / 2;
        QPainter p(&canvas);
        p.drawImage(x, y, scaled);
        p.end();
        
        // 设置(0,0)为近黑非零，避免被误判为纯黑
        canvas.setPixelColor(0, 0, QColor(1, 1, 1));
        
        screen_manager_->setBackgroundImage(canvas);
        RCLCPP_DEBUG(this->get_logger(), "Displaying %s frame %d", expression_name.c_str(), current_frame_index_ + 1);
      } else {
        RCLCPP_WARN(this->get_logger(), "Failed to load image: %s", img_path.c_str());
      }
    } else {
      RCLCPP_WARN(this->get_logger(), "Image not found: %s", img_path.c_str());
    }
    
    // 切换到下一帧
    current_frame_index_++;
    if (current_frame_index_ >= frame_count) {
      current_frame_index_ = 0;
      current_expression_index_ = (current_expression_index_ + 1) % expression_sequence_.size();
      RCLCPP_INFO(this->get_logger(), "Switched to expression: %s", expression_sequence_[current_expression_index_].first.c_str());
    }
  }

  std::string port_;
  std::string version_;
  std::string expression_path_;
  std::unique_ptr<XGO> xgo_;
  std::unique_ptr<QApplication> app_;
  std::unique_ptr<QWidget> dummy_widget_;
  std::unique_ptr<XgoScreenManager> screen_manager_;
  rclcpp::TimerBase::SharedPtr expression_timer_;
  
  std::vector<std::pair<std::string, int>> expression_sequence_;
  size_t current_expression_index_;
  int current_frame_index_;
  
  pid_t music_pid_ = -1;
};

static void handle_signal(int sig) {
  (void)sig;
  if (g_dog_node) {
    g_dog_node->stopMotion();
  }
  // Ensure ROS2 spinning thread stops
  rclcpp::shutdown();
  // Exit Qt event loop to allow main to proceed and clean up
  QCoreApplication::quit();
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  

  QApplication app(argc, argv);
  auto node = std::make_shared<DogShowCombinedNode>();
  g_dog_node = node.get();

  std::signal(SIGINT, handle_signal);
  std::signal(SIGTERM, handle_signal);
  std::signal(SIGHUP, handle_signal);
  
  // 单独的线程运行 ROS2 spin
  std::thread ros_thread([&]() {
    rclcpp::spin(node);
  });
  

  app.exec();
  
  ros_thread.join();
  rclcpp::shutdown();
  return 0;
}