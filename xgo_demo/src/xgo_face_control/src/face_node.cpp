#include <rclcpp/rclcpp.hpp>
#include <ai_msgs/msg/perception_targets.hpp>
#include <XGO.h>
#include <chrono>
#include <memory>
#include <vector>
#include <string>

using std::placeholders::_1;
using ai_msgs::msg::PerceptionTargets;

class XgoFaceNode : public rclcpp::Node {
public:
  XgoFaceNode() : Node("xgo_face_node") {
    declare_parameter<std::string>("port", "/dev/ttyS3");
    declare_parameter<int>("baud", 115200);
    declare_parameter<std::string>("version", "xgolite");
    declare_parameter<bool>("verbose", false);
    declare_parameter<bool>("fast_startup", false);
    declare_parameter<double>("kp_yaw", 0.25);    // yaw比例系数
    declare_parameter<double>("kp_pitch", 0.18);  // pitch比例系数
    declare_parameter<double>("limit_yaw", 18.0); // yaw限制
    declare_parameter<double>("limit_pitch", 20.0); // pitch限制
    declare_parameter<std::string>("topic", "/hobot_face_landmarks_detection");
    declare_parameter<int>("img_w", 960);
    declare_parameter<int>("img_h", 544);

    std::string port = get_parameter("port").as_string();
    int baud = get_parameter("baud").as_int();
    std::string version = get_parameter("version").as_string();
    bool verbose = get_parameter("verbose").as_bool();
    bool fast = get_parameter("fast_startup").as_bool();

    xgo_ = std::make_unique<XGO>(port, baud, version, verbose, fast);
    xgo_->reset();

    auto topic = get_parameter("topic").as_string();
    sub_ = create_subscription<PerceptionTargets>(
      topic, 10,
      std::bind(&XgoFaceNode::onFace, this, _1));

    RCLCPP_INFO(get_logger(), "XGO Face Node started, listening %s", topic.c_str());
  }

  ~XgoFaceNode() override {
    try {
      xgo_->reset();
    } catch (...) {}
  }

private:
  void onFace(const PerceptionTargets::SharedPtr msg) {
    double kp_yaw = get_parameter("kp_yaw").as_double();
    double kp_pitch = get_parameter("kp_pitch").as_double();
    double limit_yaw = get_parameter("limit_yaw").as_double();
    double limit_pitch = get_parameter("limit_pitch").as_double();
    int img_w = get_parameter("img_w").as_int();
    int img_h = get_parameter("img_h").as_int();


    float cx = -1.f, cy = -1.f; 
    for (auto &t : msg->targets) {
      for (auto &p : t.points) {
        if (p.type == "face_kps") {
          const auto &pts = p.point;
          if (pts.size() == 106) {
            double sumx = 0.0, sumy = 0.0;
            for (const auto &pt : pts) {
              sumx += pt.x;
              sumy += pt.y;
            }
            cx = static_cast<float>(sumx / 106.0);
            cy = static_cast<float>(sumy / 106.0);
            break;
          }
        }
      }
      if (cx >= 0 && cy >= 0) break;
    }

    if (cx < 0 || cy < 0) return; 

    // 若坐标为像素值，则按图像宽高归一化
    double nx = cx;
    double ny = cy;
    if (nx > 1.0 || ny > 1.0) {
      if (img_w > 0 && img_h > 0) {
        nx = cx / static_cast<double>(img_w);
        ny = cy / static_cast<double>(img_h);
      }
    }

    // 将关键点质心转换为偏移（以中点0.5为基准）
    // yaw: 左正右负；pitch: 下正上负
    double offset_x = (0.5 - nx);  // 左为正
    double offset_y = (ny - 0.5);  // 下为正

    double yaw = kp_yaw * offset_x * 500.0;    // 放大使幅度变大，反应更明显
    double pitch = kp_pitch * offset_y * 500.0;

    // 限幅
    if (yaw > limit_yaw) yaw = limit_yaw;
    if (yaw < -limit_yaw) yaw = -limit_yaw;
    if (pitch > limit_pitch) pitch = limit_pitch;
    if (pitch < -limit_pitch) pitch = -limit_pitch;

    try {
      std::vector<char> dirs = {'y','p'};
      std::vector<float> vals = {static_cast<float>(yaw), static_cast<float>(pitch)};
      xgo_->attitude(dirs, vals);
    } catch (const std::exception &e) {
      RCLCPP_WARN(get_logger(), "attitude error: %s", e.what());
    }
  }

  std::unique_ptr<XGO> xgo_;
  rclcpp::Subscription<PerceptionTargets>::SharedPtr sub_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XgoFaceNode>());
  rclcpp::shutdown();
  return 0;
}