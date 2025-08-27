#include <rclcpp/rclcpp.hpp>
#include <ai_msgs/msg/perception_targets.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <thread>
#include "XGO.h"

using namespace std::chrono_literals;

class XgoGestureNode : public rclcpp::Node {
public:
  XgoGestureNode() : Node("xgo_gesture_node") {
    port_ = this->declare_parameter<std::string>("port", "/dev/ttyS3");
    int baud = this->declare_parameter<int>("baud", 115200);
    version_ = this->declare_parameter<std::string>("version", "xgolite");
    verbose_ = this->declare_parameter<bool>("verbose", false);
    fast_startup_ = this->declare_parameter<bool>("fast_startup", false);

    RCLCPP_INFO(this->get_logger(), "Gesture node starting, port=%s, baud=%d, version=%s", port_.c_str(), baud, version_.c_str());
    try {
      xgo_ = std::make_unique<XGO>(port_, baud, version_, verbose_, fast_startup_);
      xgo_->reset();
    } catch (const std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Failed to init XGO: %s", e.what());
    }

    gesture_map_ = {
      // gesture_id -> pair(action_id, wait_seconds)
      {2, {12, 5.0f}},  // ThumbUp -> 坐下 -> 动作12 -> 5s
      {3, {21, 8.0f}},  // Victory -> 俯卧撑 -> 动作21 -> 8s
      {4, {22, 8.0f}},  // Mute -> 张望 -> 动作22 -> 8s
      {5, {19, 10.0f}}, // Palm -> 握手 -> 动作19 -> 10s
      {11, {14, 10.0f}},// Okay -> 伸懒腰 -> 动作14 -> 10s
      {14, {23, 6.0f}}, // Awesome -> 跳舞 -> 动作23 -> 6s
    };

    sub_ = this->create_subscription<ai_msgs::msg::PerceptionTargets>(
      "/hobot_hand_gesture_detection", 10,
      std::bind(&XgoGestureNode::onGesture, this, std::placeholders::_1)
    );

    // 防抖：同一手势触发后冷却时间
    cooldown_ = this->declare_parameter<double>("cooldown", 2.0);

    // 注册关闭时的紧急停止回调（按下B键触发应用退出时会调用）
    rclcpp::on_shutdown([this]() {
      shutting_down_.store(true, std::memory_order_relaxed);
      if (xgo_) {
        try { xgo_->stop(); } catch (...) {}
        try { xgo_->reset(); } catch (...) {}
      }
    });
  }

  ~XgoGestureNode() override {
    // 确保程序退出时（例如按下B键导致应用退出）也会停止并复位
    if (xgo_) {
      try { xgo_->stop(); } catch (...) {}
      try { xgo_->reset(); } catch (...) {}
    }
  }

  // 外部可调用的紧急停止（如需要从其它模块显式触发）
  void emergencyStop() {
    shutting_down_.store(true, std::memory_order_relaxed);
    if (xgo_) {
      try { xgo_->stop(); } catch (...) {}
      try { xgo_->reset(); } catch (...) {}
    }
  }

private:
  void onGesture(const ai_msgs::msg::PerceptionTargets::SharedPtr msg) {
    if (shutting_down_.load(std::memory_order_relaxed)) return;

    const auto & targets = msg->targets;
    if (targets.empty()) return;

    for (const auto & t : targets) {
      // attributes里通常包含分类结果，参考face age读取方式: attributes[j].value
      for (const auto & attr : t.attributes) {
        int gesture_id = static_cast<int>(attr.value);
        auto it = gesture_map_.find(gesture_id);
        if (it == gesture_map_.end()) continue;

        // 防抖：按手势ID节流
        auto now = this->now();
        auto last_it = last_trigger_time_.find(gesture_id);
        if (last_it != last_trigger_time_.end()) {
          double dt = (now - last_it->second).seconds();
          if (dt < cooldown_) {
            continue;
          }
        }
        last_trigger_time_[gesture_id] = now;

        if (shutting_down_.load(std::memory_order_relaxed)) return;
        if (action_in_progress_.load(std::memory_order_relaxed)) {
          // 正在执行上一个动作，忽略新的触发，避免叠加
          return;
        }
        action_in_progress_.store(true, std::memory_order_relaxed);

        int action_id = it->second.first;
        float wait_s = it->second.second;

        RCLCPP_INFO(this->get_logger(), "Gesture %d -> action %d (%.1fs)", gesture_id, action_id, wait_s);

        // 将动作放到后台线程，避免阻塞回调，从而能在shutdown时立刻stop/reset
        std::thread([this, action_id, wait_s]() {
          try {
            if (!shutting_down_.load(std::memory_order_relaxed) && xgo_) {
              xgo_->action(action_id, wait_s);
            }
          } catch (const std::exception &e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to run action: %s", e.what());
          }
          action_in_progress_.store(false, std::memory_order_relaxed);
        }).detach();

        // 一个消息中可能有多个属性，这里触发一个就退出，避免重复
        return;
      }
    }
  }

  std::string port_;
  std::string version_;
  bool verbose_;
  bool fast_startup_;
  std::unique_ptr<XGO> xgo_;

  rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr sub_;
  std::unordered_map<int, std::pair<int,float>> gesture_map_;
  std::unordered_map<int, rclcpp::Time> last_trigger_time_;
  double cooldown_ {2.0};

  std::atomic<bool> shutting_down_{false};
  std::atomic<bool> action_in_progress_{false};
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<XgoGestureNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}