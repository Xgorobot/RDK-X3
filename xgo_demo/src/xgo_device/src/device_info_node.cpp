#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/string.hpp>
#include <memory>
#include <string>
#include "XGO.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QFont>
#include <QSize>
#include <thread>
#include <xgoscreen/XgoScreenManager.h>
#include <QNetworkInterface>

class DeviceInfoNode : public rclcpp::Node {
public:
  DeviceInfoNode(QWidget* widget, XgoScreenManager* screenMgr) : Node("device_info_node"), widget_(widget), screenMgr_(screenMgr) {
    // Declare parameters
    port_ = this->declare_parameter<std::string>("port", "/dev/ttyS3");
    int baud = this->declare_parameter<int>("baud", 115200);
    version_ = this->declare_parameter<std::string>("version", "xgolite");
    bool verbose = this->declare_parameter<bool>("verbose", false);
    bool fast_startup = this->declare_parameter<bool>("fast_startup", false);
    double period = this->declare_parameter<double>("period", 3.0);

    RCLCPP_INFO(this->get_logger(), "xgo_device starting, port=%s, baud=%d, version=%s", port_.c_str(), baud, version_.c_str());

    try {
      xgo_ = std::make_unique<XGO>(port_, baud, version_, verbose, fast_startup);
    } catch (const std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "Failed to open XGO on %s: %s", port_.c_str(), e.what());
    }

    battery_pub_ = this->create_publisher<std_msgs::msg::Float32>("/xgo_battery", 10);
    version_pub_ = this->create_publisher<std_msgs::msg::String>("/xgo_version", 10);

    // ensure screenMgr is initialized
    if (screenMgr && widget_) {
      if (!screenMgr_->isRunning()) {
        screenMgr_->initialize(1, 0, 20000000, 5, 13, 29, 100, fast_startup);
      }
    }

    timer_ = this->create_wall_timer(std::chrono::duration<double>(period), [this]() {
      // Gather info
      int batteryPct = -1;
      std::string fw;
      if (xgo_) {
        try {
          batteryPct = xgo_->read_battery();
        } catch (const std::exception &e) {
          RCLCPP_WARN(this->get_logger(), "read_battery failed: %s", e.what());
        }
        try {
          fw = xgo_->read_firmware();
        } catch (const std::exception &e) {
          RCLCPP_WARN(this->get_logger(), "read_firmware failed: %s", e.what());
        }
      }

      // Publish
      if (batteryPct >= 0) {
        auto battery_msg = std_msgs::msg::Float32();
        battery_msg.data = static_cast<float>(batteryPct);
        battery_pub_->publish(battery_msg);
      }
      if (!fw.empty()) {
        auto version_msg = std_msgs::msg::String();
        version_msg.data = fw;
        version_pub_->publish(version_msg);
      }

      // Compose display strings in Chinese
      QString title = QString::fromUtf8("设备信息");
      QString fwLine = QString::fromUtf8("固件型号：%1").arg(QString::fromStdString(fw));
      QString batLine = QString::fromUtf8("电量：%1%").arg(batteryPct >= 0 ? QString::number(batteryPct) : QString("--"));

      // Get IPv4 address
      QString ip = getPreferredIPv4();
      bool isLoopback = (ip == "127.0.0.1" || ip == "::1" || ip.startsWith("127."));
      QString ipLine = isLoopback
        ? QString::fromUtf8("IP：%1（未联网）").arg(ip)
        : QString::fromUtf8("IP：%1").arg(ip);

      // Render to screen directly
      if (screenMgr_) {
        QImage* bg = screenMgr_->getBackgroundImagePointer();
        if (bg) {
          if (bg->size() != QSize(320,240)) {
            *bg = QImage(320,240,QImage::Format_RGB32);
          }
          bg->fill(Qt::black);
          QPainter p(bg);
          p.setRenderHint(QPainter::Antialiasing);
          // Prefer Microsoft YaHei if present for better Chinese rendering; fallback to default
          QFont fontTitle("Microsoft YaHei", 22, QFont::DemiBold);
          QFont fontBody("Microsoft YaHei", 18, QFont::Normal);
          p.setPen(Qt::white);
          // Title
          p.setFont(fontTitle);
          p.drawText(QRect(0, 10, 320, 36), Qt::AlignHCenter | Qt::AlignVCenter, title);
          // Body lines
          p.setFont(fontBody);
          int y = 80;
          int lineH = 36;
          p.drawText(20, y, fwLine); y += lineH;
          p.drawText(20, y, batLine); y += lineH;
          p.drawText(20, y, ipLine); y += lineH;
          p.end();
          // 确保不是纯黑，纯黑会被QT界面刷屏识别为背景
          bg->setPixelColor(0, 0, QColor(1, 1, 1));
        }
      }
    });
  }

private:
  // 获取首选 IPv4 地址：优先非回环、非虚拟接口；无则返回127.0.0.1
  QString getPreferredIPv4() const {
    const auto ifs = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : ifs) {
      if (!(iface.flags() & QNetworkInterface::IsUp)) continue;
      if (!(iface.flags() & QNetworkInterface::IsRunning)) continue;
      if (iface.flags() & QNetworkInterface::IsLoopBack) continue;
      if (iface.humanReadableName().contains("docker", Qt::CaseInsensitive)) continue;
      if (iface.humanReadableName().contains("veth", Qt::CaseInsensitive)) continue;
      const auto addrs = iface.addressEntries();
      for (const QNetworkAddressEntry &ae : addrs) {
        const auto ip = ae.ip();
        if (ip.protocol() == QAbstractSocket::IPv4Protocol) {
          return ip.toString();
        }
      }
    }
    // fallback to loopback
    return QString("127.0.0.1");
  }

  std::string port_;
  std::string version_;
  std::unique_ptr<XGO> xgo_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr version_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  QWidget* widget_ {nullptr};
  XgoScreenManager* screenMgr_ {nullptr};
};

int main(int argc, char **argv) {
  // Create Qt application for screen manager
  int qt_argc = 0; char** qt_argv = nullptr;
  QApplication app(qt_argc, qt_argv);
  QWidget dummyWidget; // not shown, used for grab and palette
  dummyWidget.setFixedSize(320, 240);
  dummyWidget.show();

  // Create screen manager and initialize
  XgoScreenManager screenMgr(&dummyWidget);
  // 屏幕初始化放在 DeviceInfoNode 构造函数中按参数（包括 fast_startup）执行
  
  // Start ROS2
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DeviceInfoNode>(&dummyWidget, &screenMgr);

  // Run ROS spinning in a separate thread so that Qt event loop can run for ScreenThread timers
  std::thread ros_thread([&](){ rclcpp::spin(node); rclcpp::shutdown(); });

  int ret = app.exec();

  // Cleanup
  if (ros_thread.joinable()) ros_thread.join();
  screenMgr.stop();
  return ret;
}