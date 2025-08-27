#include <rclcpp/rclcpp.hpp>
#include <ai_msgs/msg/perception_targets.hpp>
#include <XGO.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <functional>
#include <atomic>

using ai_msgs::msg::PerceptionTargets;

struct PID {
  double kp{0.0}, ki{0.0}, kd{0.0};
  double integ{0.0};
  double prev{0.0};
  double clamp(double v, double lo, double hi) { return std::max(lo, std::min(hi, v)); }
  double step(double err, double dt, double limit_abs) {
    integ += err * dt;
    double deriv = (err - prev) / std::max(dt, 1e-6);
    prev = err;
    double out = kp * err + ki * integ + kd * deriv;
    return clamp(out, -limit_abs, limit_abs);
  }
  void reset(){integ=0.0; prev=0.0;}
};

class XgoBodyTracker : public rclcpp::Node {
public:
  XgoBodyTracker() : Node("xgo_body_tracker") {
    // Parameters
    //前后表现较好，左右较差
    declare_parameter<std::string>("port", "/dev/ttyS3");
    declare_parameter<int>("baud", 115200);
    declare_parameter<std::string>("version", "xgolite");
    declare_parameter<bool>("verbose", false);
    declare_parameter<bool>("fast_startup", false);
    declare_parameter<std::string>("topic", "/hobot_mono2d_body_detection");
    declare_parameter<int>("img_w", 960);
    declare_parameter<int>("img_h", 544);
    declare_parameter<double>("kp_x", 1.0);  // 比例增益：控制左右调整的敏感度，1.2时虽然敏感度高，但是会导致水平移动偏移严重
    declare_parameter<double>("kp_y", 1.2);  // 比例增益：控制前后调整的敏感度
    declare_parameter<double>("ki_x", 0.08); // 积分增益：消除左右的稳态误差，太大会引起数据震荡，太小则误差太大
    declare_parameter<double>("ki_y", 0.08); // 积分增益：消除前后的稳态误差
    declare_parameter<double>("kd_x", 0.04); // 微分增益：提供阻尼，大了可减少超调，但对噪音敏感，小了则增加超调量
    declare_parameter<double>("kd_y", 0.04); // 垂直微分增益
    declare_parameter<double>("limit_x_cm", 3.0);  // 水平输出限制
    declare_parameter<double>("limit_y_cm", 4.0);  // 垂直输出限制
    declare_parameter<double>("step_min_cm", 0.8);  // 微调的最小步幅
    declare_parameter<int>("lost_timeout_ms", 800);
    declare_parameter<double>("band_low", 0.47);
    declare_parameter<double>("band_high", 0.53);
    declare_parameter<double>("center_deadband_x", 0.02); // 水平中心死区
    declare_parameter<double>("y_hysteresis", 0.05);  // 垂直滞后，主要是减少误差用的
    declare_parameter<double>("ema_alpha", 0.3);  // EMA平滑系数，使数据更加平滑
    declare_parameter<double>("rate_limit_cm", 2.5);  // 变化率限制，增大可以有更快的响应变化
    declare_parameter<double>("target_height_ratio", 0.7);  // 目标身体高度比例
    declare_parameter<double>("roi_weight", 0.8);  // ROI权重，这个是给body块和目标点检测两种方法的比重关系，但是主要使用的body块，只有在没有检测到body块时才用目标点检测
    declare_parameter<double>("height_tolerance", 0.1);   // 高度比例容忍度，因为后续是按照检测到vody块的高度比例进行前进后退命令的，这里给个误差值，别一直进行调整
    declare_parameter<bool>("debug_output", true);  

    std::string port = get_parameter("port").as_string();
    int baud = get_parameter("baud").as_int();
    std::string version = get_parameter("version").as_string();
    bool verbose = get_parameter("verbose").as_bool();
    bool fast = get_parameter("fast_startup").as_bool();

    try {
      xgo_ = std::make_unique<XGO>(port, baud, version, verbose, fast);
      xgo_->reset();
    } catch (const std::exception &e) {
      RCLCPP_ERROR(get_logger(), "Failed to init XGO: %s", e.what());
    }

    auto topic = get_parameter("topic").as_string();
    sub_ = create_subscription<PerceptionTargets>(
      topic, 10, std::bind(&XgoBodyTracker::onBody, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&XgoBodyTracker::controlLoop, this));

    last_seen_ = now();
    
    // 注册关闭时的紧急停止回调（按下B键触发应用退出时会调用）
    rclcpp::on_shutdown([this]() {
      shutting_down_.store(true, std::memory_order_relaxed);
      if (xgo_) {
        try { xgo_->stop(); } catch (...) {}
        try { xgo_->reset(); } catch (...) {}
      }
    });
  }

  ~XgoBodyTracker() override {
    try { if (xgo_) { xgo_->stop(); xgo_->reset(); } } catch (...) {}
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
  struct Kps2D { float x{0}, y{0}; };
  struct ROI { float x{0}, y{0}, w{0}, h{0}; float confidence{0}; };

  // 提取body块的边界框数据
  bool extractBodyROI(const PerceptionTargets &msg, ROI &roi) {
    for (const auto &t : msg.targets) {
      for (const auto &r : t.rois) {
        if (r.type == "body") {
          roi.x = static_cast<float>(r.rect.x_offset);
          roi.y = static_cast<float>(r.rect.y_offset);
          roi.w = static_cast<float>(r.rect.width);
          roi.h = static_cast<float>(r.rect.height);
          roi.confidence = static_cast<float>(r.confidence);
          return true;
        }
      }
    }
    return false;
  }

 
  bool extractLowerBody(const PerceptionTargets &msg, std::vector<Kps2D> &feet, std::vector<Kps2D> &knees) {
    float img_w = static_cast<float>(get_parameter("img_w").as_int());
    float img_h = static_cast<float>(get_parameter("img_h").as_int());


    for (const auto &t : msg.targets) {
      for (const auto &ps : t.points) {
        if (ps.type == "body_kps") {
          const auto &pts = ps.point;
          if (pts.size() < 12) continue; 
          std::vector<Kps2D> all;
          all.reserve(pts.size());
          for (const auto &p : pts) {
            float px = static_cast<float>(p.x);
            float py = static_cast<float>(p.y);
            if (px > 0 && px <= 1.5f && py > 0 && py <= 1.5f) {
              px *= img_w; py *= img_h;
            }
            all.push_back({px, py});
          }
          std::sort(all.begin(), all.end(), [](const Kps2D&a, const Kps2D&b){return a.y > b.y;});
          // 选取最靠下的4个点作为脚/脚踝候选，再往上4个作为膝盖候选
          size_t n = all.size();
          size_t nFeet = std::min<size_t>(2, n);
          size_t nKnee = std::min<size_t>(4, n) - nFeet;
          feet.clear(); knees.clear();
          for (size_t i = 0; i < nFeet; ++i) feet.push_back(all[i]);
          for (size_t i = nFeet; i < nFeet + nKnee; ++i) knees.push_back(all[i]);
          if (!feet.empty()) return true;
        }
      }
    }
    return false;
  }

  // body_kps 中提取两个最靠近底部（y最大）的点，作为左右脚
  bool extractBodyTwoFeet(const PerceptionTargets &msg, Kps2D &foot1, Kps2D &foot2) {
    float img_w = static_cast<float>(get_parameter("img_w").as_int());
    float img_h = static_cast<float>(get_parameter("img_h").as_int());

    // bool found = false; // removed unused variable
    std::vector<Kps2D> all;
    for (const auto &t : msg.targets) {
      for (const auto &ps : t.points) {
        if (ps.type == "body_kps") {
          for (const auto &p : ps.point) {
            float px = static_cast<float>(p.x);
            float py = static_cast<float>(p.y);
            if (px > 0 && px <= 1.5f && py > 0 && py <= 1.5f) { px *= img_w; py *= img_h; }
            all.push_back({px, py});
          }
        }
      }
    }
    if (all.size() < 2) return false;
    std::sort(all.begin(), all.end(), [](const Kps2D &a, const Kps2D &b){ return a.y > b.y; });
    foot1 = all[0];
    foot2 = all[1];
    return true;
  }

  bool extractBodyFoot(const PerceptionTargets &msg, Kps2D &foot) {
    float img_w = static_cast<float>(get_parameter("img_w").as_int());
    float img_h = static_cast<float>(get_parameter("img_h").as_int());
    bool got = false;
    Kps2D best{};
    for (const auto &t : msg.targets) {
      for (const auto &ps : t.points) {
        if (ps.type == "body_kps") {
          for (const auto &p : ps.point) {
            float px = static_cast<float>(p.x);
            float py = static_cast<float>(p.y);
            if (px > 0 && px <= 1.5f && py > 0 && py <= 1.5f) { px *= img_w; py *= img_h; }
            if (!got || py > best.y) { best = {px, py}; got = true; }
          }
        }
      }
    }
    if (got) { foot = best; return true; }
    return false;
  }

  void onBody(const PerceptionTargets::SharedPtr msg) {
    if (shutting_down_.load(std::memory_order_relaxed)) return;
    // 混合跟踪：同时提取ROI和关键点信息
    ROI body_roi;
    bool have_roi = extractBodyROI(*msg, body_roi);
    
    Kps2D f1, f2;
    bool have_two_feet = extractBodyTwoFeet(*msg, f1, f2);
    
    if (have_roi || have_two_feet) {
      last_seen_ = now();
      have_roi_ = have_roi;
      
      if (have_roi) {
        latest_roi_ = body_roi;
        // EMA平滑ROI中心
        Kps2D roi_center{body_roi.x + body_roi.w * 0.5f, body_roi.y + body_roi.h * 0.5f};
        double alpha = get_parameter("ema_alpha").as_double();
        if (!have_smoothed_roi_) { roi_smoothed_ = roi_center; have_smoothed_roi_ = true; }
        else {
          roi_smoothed_.x = static_cast<float>(alpha * roi_center.x + (1.0 - alpha) * roi_smoothed_.x);
          roi_smoothed_.y = static_cast<float>(alpha * roi_center.y + (1.0 - alpha) * roi_smoothed_.y);
        }
      }
      
      if (have_two_feet) {
        using_midpoint_ = true;
        Kps2D mid{ static_cast<float>((f1.x + f2.x) * 0.5f), static_cast<float>((f1.y + f2.y) * 0.5f) };
        latest_mid_ = mid;
        // EMA 平滑中心点，降低抖动
        double alpha = get_parameter("ema_alpha").as_double();
        if (!have_smoothed_) { foot_smoothed_ = mid; have_smoothed_ = true; }
        else {
          foot_smoothed_.x = static_cast<float>(alpha * mid.x + (1.0 - alpha) * foot_smoothed_.x);
          foot_smoothed_.y = static_cast<float>(alpha * mid.y + (1.0 - alpha) * foot_smoothed_.y);
        }
      } else {
        // 缺脚时：退化为单脚点控制
        Kps2D foot;
        if (extractBodyFoot(*msg, foot)) {
          using_midpoint_ = false;
          latest_foot_ = foot;
          double alpha = get_parameter("ema_alpha").as_double();
          if (!have_smoothed_) { foot_smoothed_ = foot; have_smoothed_ = true; }
          else {
            foot_smoothed_.x = static_cast<float>(alpha * foot.x + (1.0 - alpha) * foot_smoothed_.x);
            foot_smoothed_.y = static_cast<float>(alpha * foot.y + (1.0 - alpha) * foot_smoothed_.y);
          }
        }
      }
      have_target_ = true;
    }
  }

  void controlLoop() {
    if (shutting_down_.load(std::memory_order_relaxed)) return;
    if (!xgo_) return;

    auto now_t = now();
    int lost_timeout_ms = get_parameter("lost_timeout_ms").as_int();
    auto dt_ns = (now_t - last_seen_).nanoseconds();
    if (dt_ns > static_cast<int64_t>(lost_timeout_ms) * 1000000LL) {
      pid_x_.reset(); pid_y_.reset();
      have_target_ = false;
      have_smoothed_ = false;
      have_smoothed_roi_ = false;
      out_x_prev_ = 0.0; out_y_prev_ = 0.0;
      return;
    }
    if (!have_target_) return;

    double img_w = static_cast<double>(get_parameter("img_w").as_int());
    double img_h = static_cast<double>(get_parameter("img_h").as_int());
    double cx = img_w * 0.5;

    double err_x = 0.0, err_y = 0.0;
    
    if (have_roi_ && have_smoothed_roi_) {
      // 水平控制：ROI中心对准图像中心
      double roi_center_x = static_cast<double>(roi_smoothed_.x);
      double roi_dx_norm = (roi_center_x - cx) / img_w;  
        double center_db = get_parameter("center_deadband_x").as_double();
        err_x = (std::abs(roi_dx_norm) < center_db) ? 0.0 : -roi_dx_norm;  // 人在右侧时为负值（右移），左侧时为正值（左移）
      
      // 垂直控制：基于body块高度进行距离控制
      double current_height = static_cast<double>(latest_roi_.h);
      double current_height_ratio = current_height / img_h;
      double target_height_ratio = get_parameter("target_height_ratio").as_double();
      double height_tolerance = get_parameter("height_tolerance").as_double();
      
      if (current_height_ratio < target_height_ratio - height_tolerance) {
        err_y = (target_height_ratio - current_height_ratio) * 5.0;
      } else if (current_height_ratio > target_height_ratio + height_tolerance) {
        err_y = (target_height_ratio - current_height_ratio) * 5.0;
      } else {
        err_y = 0.0;  
      }
    } else if (have_smoothed_) {

      double foot_dx_norm = (static_cast<double>(foot_smoothed_.x) - cx) / img_w;
      double center_db = get_parameter("center_deadband_x").as_double();
      err_x = (std::abs(foot_dx_norm) < center_db) ? 0.0 : -foot_dx_norm;  

      double y_norm = static_cast<double>(foot_smoothed_.y) / img_h;
      err_y = (0.5 - y_norm) * 2.0;
    }
    

    bool debug = get_parameter("debug_output").as_bool();
    if (debug && (err_x != 0.0 || err_y != 0.0)) {
      RCLCPP_INFO(get_logger(), "Control: err_x=%.3f, err_y=%.3f, have_roi=%d", 
                  err_x, err_y, have_roi_ ? 1 : 0);
      if (have_roi_) {
        double current_height_ratio = static_cast<double>(latest_roi_.h) / img_h;
        RCLCPP_INFO(get_logger(), "ROI: center_x=%.1f, height_ratio=%.3f, target=%.3f", 
                    roi_smoothed_.x, current_height_ratio, get_parameter("target_height_ratio").as_double());
      }
    }

    // PID控制
    double kp_x = get_parameter("kp_x").as_double();
    double ki_x = get_parameter("ki_x").as_double();
    double kd_x = get_parameter("kd_x").as_double();
    double kp_y = get_parameter("kp_y").as_double();
    double ki_y = get_parameter("ki_y").as_double();
    double kd_y = get_parameter("kd_y").as_double();
    double limit_x_cm = get_parameter("limit_x_cm").as_double();
    double limit_y_cm = get_parameter("limit_y_cm").as_double();
    double step_min = get_parameter("step_min_cm").as_double();

    pid_x_.kp = kp_x; pid_x_.ki = ki_x; pid_x_.kd = kd_x;
    pid_y_.kp = kp_y; pid_y_.ki = ki_y; pid_y_.kd = kd_y;

    double dt = 0.1;
    double out_x_cm = pid_x_.step(err_x * 15.0, dt, limit_x_cm);  
    double out_y_cm = pid_y_.step(err_y * 10.0, dt, limit_y_cm);

    // 限速
    double rate_limit = get_parameter("rate_limit_cm").as_double();
    double dx = out_x_cm - out_x_prev_;
    if (std::abs(dx) > rate_limit) out_x_cm = out_x_prev_ + (dx > 0 ? rate_limit : -rate_limit);
    double dy = out_y_cm - out_y_prev_;
    if (std::abs(dy) > rate_limit) out_y_cm = out_y_prev_ + (dy > 0 ? rate_limit : -rate_limit);

    if (std::abs(out_x_cm) < step_min) out_x_cm = 0.0;
    if (std::abs(out_y_cm) < step_min) out_y_cm = 0.0;

    try {
      static int control_cycle = 0;
      static int last_control_mode = 0; // 0=无控制, 1=水平, 2=垂直, 3=混合
      static int mode_stable_count = 0; 
      static int target_control_mode = 0; 
      control_cycle++;
      
      bool should_move_forward_back = (out_y_cm != 0.0);
      bool should_move_left_right = (out_x_cm != 0.0);
      
      double x_threshold = 0.8; 
      double y_threshold = 1.0; 
      
      bool priority_horizontal = (std::abs(out_x_cm) > x_threshold);
      bool priority_vertical = (std::abs(out_y_cm) > y_threshold);
      
 
      int desired_control_mode = 0;
      if (priority_horizontal && priority_vertical) {
        desired_control_mode = 3; // 混合模式
      } else if (priority_horizontal) {
        desired_control_mode = 1; // 水平优先
      } else if (priority_vertical) {
        desired_control_mode = 2; // 垂直优先
      }
      
      const int MODE_SWITCH_DELAY = 3; // 需要连续3个周期确认才切换模式
      
      if (desired_control_mode == target_control_mode) {
        mode_stable_count++;
      } else {
        target_control_mode = desired_control_mode;
        mode_stable_count = 1;
      }
      
      // 模式稳定足够长时间后才真正切换
      int current_control_mode = last_control_mode;
      if (mode_stable_count >= MODE_SWITCH_DELAY) {
        current_control_mode = target_control_mode;
      }
      
      if (current_control_mode != last_control_mode) {
        if (current_control_mode == 1 || (last_control_mode == 2 && current_control_mode != 2)) {
          pid_y_.reset();
          if (debug) {
            RCLCPP_INFO(get_logger(), "PID Reset: Vertical PID reset due to mode change %d->%d (stable_count=%d)", last_control_mode, current_control_mode, mode_stable_count);
          }
        }
        if (current_control_mode == 2 || (last_control_mode == 1 && current_control_mode != 1)) {
          pid_x_.reset();
          if (debug) {
            RCLCPP_INFO(get_logger(), "PID Reset: Horizontal PID reset due to mode change %d->%d (stable_count=%d)", last_control_mode, current_control_mode, mode_stable_count);
          }
        }
        last_control_mode = current_control_mode;
        mode_stable_count = 0; 
      }
      
      if (debug && (err_x != 0.0 || err_y != 0.0)) {
        RCLCPP_INFO(get_logger(), "Mode: desired=%d, target=%d, current=%d, stable_count=%d, priority_h=%s, priority_v=%s", 
                   desired_control_mode, target_control_mode, current_control_mode, mode_stable_count,
                   priority_horizontal ? "true" : "false", priority_vertical ? "true" : "false");
      }
      
      // 权重衰减控制策略：在优先模式时降低非优先方向的影响
      double weight_x = 1.0;  // 水平控制权重
      double weight_y = 1.0;  // 垂直控制权重
      //下面就是各个模式下的水平控制和垂直控制的权重参数
      if (priority_horizontal && priority_vertical) {
        
        if (control_cycle % 4 < 2) {
          weight_x = 1.0;
          weight_y = 0.1;  
          if (debug) {
            RCLCPP_INFO(get_logger(), "Mixed mode: LEFT-RIGHT priority, weight_x=%.1f, weight_y=%.1f", weight_x, weight_y);
          }
        } else {

          weight_x = 0.1;  
          weight_y = 1.0;
          if (debug) {
            RCLCPP_INFO(get_logger(), "Mixed mode: FORWARD-BACK priority, weight_x=%.1f, weight_y=%.1f", weight_x, weight_y);
          }
        }
      } else if (priority_horizontal) {
        weight_x = 1.0;
        weight_y = 0.05;  
        if (debug) {
          RCLCPP_INFO(get_logger(), "Horizontal priority: weight_x=%.1f, weight_y=%.1f", weight_x, weight_y);
        }
      } else if (priority_vertical) {

        weight_x = 0.15;
        weight_y = 1.0;
        if (debug) {
          RCLCPP_INFO(get_logger(), "Vertical priority: weight_x=%.1f, weight_y=%.1f", weight_x, weight_y);
        }
      }
      

      double final_out_x = out_x_cm * weight_x;
      double final_out_y = out_y_cm * weight_y;
      
      if (should_move_left_right || priority_horizontal) {
        xgo_->move_y_by(static_cast<float>(final_out_x));
        if (debug && final_out_x != 0.0) {
          RCLCPP_INFO(get_logger(), "LEFT-RIGHT: out_x=%.2f, final=%.2f (weight=%.1f)", out_x_cm, final_out_x, weight_x);
        }
      }
      if (should_move_forward_back || priority_vertical) {
        xgo_->move_x_by(static_cast<float>(final_out_y));
        if (debug && final_out_y != 0.0) {
          RCLCPP_INFO(get_logger(), "FORWARD-BACK: out_y=%.2f, final=%.2f (weight=%.1f)", out_y_cm, final_out_y, weight_y);
        }
      }
      
      out_x_prev_ = out_x_cm;
      out_y_prev_ = out_y_cm;
    } catch (const std::exception &e) {
      RCLCPP_WARN(get_logger(), "xgo move error: %s", e.what());
    }
  }

  // Data
  std::unique_ptr<XGO> xgo_;
  rclcpp::Subscription<PerceptionTargets>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;

  PID pid_x_, pid_y_;
  rclcpp::Time last_seen_;
  bool have_target_{false};
  Kps2D latest_foot_{};
  Kps2D latest_mid_{}; 
  Kps2D foot_smoothed_{}; bool have_smoothed_{false};
  bool using_midpoint_{false}; 

  bool have_roi_{false};
  ROI latest_roi_{};
  Kps2D roi_smoothed_{}; bool have_smoothed_roi_{false};
  
  double out_x_prev_{0.0}, out_y_prev_{0.0};
  
  std::atomic<bool> shutting_down_{false};
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<XgoBodyTracker>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}