#include <rclcpp/rclcpp.hpp>
#include <rclcpp/qos.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include <thread>
#include <signal.h>
#include <atomic>
#include <chrono>
#include <QTimer>
#include <QCoreApplication>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <xgoscreen/XgoScreenManager.h>

class LidarDisplayNode : public rclcpp::Node
{
public:
    LidarDisplayNode() : Node("lidar_display_node")
    {

        auto qos = rclcpp::QoS(10)
            .reliability(rclcpp::ReliabilityPolicy::BestEffort)//这个要和话题对应上
            .durability(rclcpp::DurabilityPolicy::Volatile);
        
        // 订阅scan话题,scan话题是雷达扫描数据
        scan_subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", qos,
            std::bind(&LidarDisplayNode::scan_callback, this, std::placeholders::_1));
        
        RCLCPP_INFO(this->get_logger(), "Configured QoS: BestEffort reliability, Volatile durability");
        
        // 创建发布器，用于调试信息
        debug_publisher_ = this->create_publisher<std_msgs::msg::String>("lidar_debug", 10);
        
        // 初始化屏幕显示参数
        screen_width_ = 320;   
        screen_height_ = 240;  
        center_x_ = screen_width_ / 2;
        center_y_ = screen_height_ / 2;
        max_display_range_ = 5.0;  //最大显示米数

        initializeScreen();
        
        RCLCPP_INFO(this->get_logger(), "LiDAR Display Node initialized");
        RCLCPP_INFO(this->get_logger(), "Waiting for scan data on topic: /scan");
    }
    
    ~LidarDisplayNode()
    {
        RCLCPP_INFO(this->get_logger(), "Destroying LidarDisplayNode...");
        cleanup();
    }
    
    void cleanup() {
        if (screen_manager_) {
            try {
                screen_manager_->stop();
                delete screen_manager_;
                screen_manager_ = nullptr;
            } catch (const std::exception& e) {
                RCLCPP_WARN(this->get_logger(), "Error during screen cleanup: %s", e.what());
            }
        }
        if (widget_) {
            delete widget_;
            widget_ = nullptr;
        }
        RCLCPP_INFO(this->get_logger(), "Node cleanup completed");
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received scan data with %zu points", msg->ranges.size());
        
        process_scan_data(msg);
        
        visualize_on_screen(msg);
        
        publish_debug_info(msg);
    }
    
    void process_scan_data(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {

        display_points_.clear();
        
        float angle = msg->angle_min;
        
        for (size_t i = 0; i < msg->ranges.size(); ++i)
        {
            float range = msg->ranges[i];
            

            if (range >= msg->range_min && range <= msg->range_max && 
                range <= max_display_range_ && std::isfinite(range))
            {
                // 将极坐标转换为笛卡尔坐标
                float x = range * cos(angle);
                float y = range * sin(angle);
                
                int screen_x = static_cast<int>(center_x_ + (x / max_display_range_) * (screen_width_ / 2 - 10));
                int screen_y = static_cast<int>(center_y_ - (y / max_display_range_) * (screen_height_ / 2 - 10));
                

                if (screen_x >= 0 && screen_x < screen_width_ && 
                    screen_y >= 0 && screen_y < screen_height_)
                {
                    display_points_.push_back({screen_x, screen_y, range});
                }
            }
            
            angle += msg->angle_increment;
        }
        
        RCLCPP_DEBUG(this->get_logger(), "Processed %zu valid points for display", display_points_.size());
    }
    
    void visualize_on_screen(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        if (!screen_manager_ || !screen_manager_->isRunning()) {
            RCLCPP_WARN(this->get_logger(), "Screen manager not initialized or not running");
            return;
        }
        

        QImage* background = screen_manager_->getBackgroundImagePointer();
        if (!background) {
            RCLCPP_ERROR(this->get_logger(), "Failed to get background image pointer");
            return;
        }
        

        QPainter painter(background);
        painter.setRenderHint(QPainter::Antialiasing, true);
        

        painter.fillRect(0, 0, screen_width_, screen_height_, Qt::black);
        

        painter.setPen(QPen(Qt::gray, 1));
        for (int i = 1; i <= 5; ++i) {
            int radius = (i * (std::min(screen_width_, screen_height_) / 2 - 20)) / 5;
            painter.drawEllipse(center_x_ - radius, center_y_ - radius, radius * 2, radius * 2);
        }
        

        painter.setPen(QPen(Qt::red, 3));
        painter.drawEllipse(center_x_ - 3, center_y_ - 3, 6, 6);
        

        for (const auto& point : display_points_) {
            QColor color = getDistanceQColor(point.distance);
            painter.setPen(QPen(color, 2));
            painter.drawPoint(point.x, point.y);
        }
        

        painter.setPen(QPen(Qt::darkGray, 1));
        painter.drawLine(center_x_, 0, center_x_, screen_height_); 
        painter.drawLine(0, center_y_, screen_width_, center_y_);  
        

        painter.setPen(QPen(Qt::white, 1));
        painter.setFont(QFont("Arial", 10));
        QString info_text = QString("Points: %1 Range: %2m")
                           .arg(display_points_.size())
                           .arg(max_display_range_, 0, 'f', 1);
        painter.drawText(10, 20, info_text);
        
        painter.end();
        
        RCLCPP_DEBUG(this->get_logger(), "Displayed %zu points on screen", display_points_.size());
    }
    
    void publish_debug_info(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        auto debug_msg = std_msgs::msg::String();
        

        float min_range = *std::min_element(msg->ranges.begin(), msg->ranges.end());
        float max_range = *std::max_element(msg->ranges.begin(), msg->ranges.end());
        

        size_t valid_points = 0;
        for (float range : msg->ranges)
        {
            if (range >= msg->range_min && range <= msg->range_max && std::isfinite(range))
            {
                valid_points++;
            }
        }
        
        char debug_info[200];
        snprintf(debug_info, sizeof(debug_info),
                "Scan: %zu total, %zu valid, min: %.2fm, max: %.2fm, display: %zu points",
                msg->ranges.size(), valid_points, min_range, max_range, display_points_.size());
        
        debug_msg.data = debug_info;
        debug_publisher_->publish(debug_msg);
    }
    
    QColor getDistanceQColor(float distance)
    {

        float normalized = std::min(distance / max_display_range_, 1.0f);
        
        if (normalized < 0.3f) {
            return Qt::red;    // 红色 - 近距离
        } else if (normalized < 0.7f) {
            return Qt::yellow; // 黄色 - 中距离
        } else {
            return Qt::green;  // 绿色 - 远距离
        }
    }
    
    void initializeScreen()
    {
        try {

            widget_ = new QWidget();
            widget_->resize(screen_width_, screen_height_);

            screen_manager_ = new XgoScreenManager(widget_);
            

            bool success = screen_manager_->initialize(
                1,          // spiController
                0,          // spiDevice
                20000000,   // spiFrequency (20MHz)
                5,          // gpioCS
                13,         // gpioReset
                29,         // gpioDC
                100,        // refreshInterval (100ms)
                false       // fast_startup
            );
            
            if (success) {

                screen_manager_->setBackgroundColor(Qt::black);
                RCLCPP_INFO(this->get_logger(), "Screen manager initialized successfully");
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to initialize screen manager");
            }
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Exception during screen initialization: %s", e.what());
        }
    }
    
    
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr debug_publisher_;
    
    
    int screen_width_;
    int screen_height_;
    int center_x_;
    int center_y_;
    float max_display_range_;
    

    QWidget* widget_;
    XgoScreenManager* screen_manager_;
    

    struct DisplayPoint
    {
        int x, y;
        float distance;
    };
    
    std::vector<DisplayPoint> display_points_;
};


std::atomic<bool> g_shutdown_requested{false};
std::atomic<int> g_signal_count{0};
std::shared_ptr<LidarDisplayNode> g_node = nullptr;
QApplication* g_app = nullptr;


void signalHandler(int signum)
{
    int count = ++g_signal_count;
    RCLCPP_INFO(rclcpp::get_logger("signal_handler"), "Received signal %d (count: %d), shutting down...", signum, count);
    
    g_shutdown_requested = true;
    

    if (count >= 3) {
        RCLCPP_WARN(rclcpp::get_logger("signal_handler"), "Force exit after %d signals", count);
        std::exit(1);
    }
    
 
    if (g_node) {
        g_node.reset();
    }
    

    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
    

    if (g_app) {
        g_app->quit();
        QCoreApplication::processEvents();
    }
}

int main(int argc, char * argv[])
{
    
    rclcpp::init(argc, argv);
    
  
    QApplication app(argc, argv);
    g_app = &app;
    
    
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    g_node = std::make_shared<LidarDisplayNode>();
    
    RCLCPP_INFO(g_node->get_logger(), "Starting LiDAR Display Node...");
    

    QTimer* exit_timer = new QTimer(&app);
    QObject::connect(exit_timer, &QTimer::timeout, [&app]() {
        if (g_shutdown_requested) {
            RCLCPP_INFO(rclcpp::get_logger("lidar_display"), "Exit timer triggered, quitting application...");
            app.quit();
        }
    });
    exit_timer->start(100); 
    
    try {
    
        std::thread ros_thread([&]() {
            while (rclcpp::ok() && !g_shutdown_requested) {
                try {
                    rclcpp::spin_some(g_node);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                } catch (const std::exception& e) {
                    RCLCPP_ERROR(rclcpp::get_logger("lidar_display"), "ROS thread error: %s", e.what());
                    break;
                }
            }
            RCLCPP_INFO(rclcpp::get_logger("lidar_display"), "ROS thread exiting...");
        });
        
    
        RCLCPP_INFO(rclcpp::get_logger("lidar_display"), "Starting Qt event loop...");
        int result = app.exec();
        RCLCPP_INFO(rclcpp::get_logger("lidar_display"), "Qt event loop ended");
        
   
        g_shutdown_requested = true;
        
        auto start_time = std::chrono::steady_clock::now();
        const auto timeout = std::chrono::seconds(5);
        
        if (ros_thread.joinable()) {
       
            while (ros_thread.joinable()) {
                auto elapsed = std::chrono::steady_clock::now() - start_time;
                if (elapsed > timeout) {
                    RCLCPP_WARN(rclcpp::get_logger("lidar_display"), "ROS thread join timeout, detaching...");
                    ros_thread.detach();
                    break;
                }
                
            
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                if (!ros_thread.joinable()) {
                    break;
                }
            }
            
            if (ros_thread.joinable()) {
                ros_thread.join();
            }
        }
        
    
        if (g_node) {
            g_node->cleanup();
            g_node.reset();
        }
        
        if (rclcpp::ok()) {
            rclcpp::shutdown();
        }
        
        RCLCPP_INFO(rclcpp::get_logger("lidar_display"), "Program exited successfully");
        return result;
    } catch (const std::exception& e) {
        RCLCPP_ERROR(rclcpp::get_logger("lidar_display"), "Exception in main: %s", e.what());
        g_shutdown_requested = true;
        
   
        if (g_node) {
            g_node->cleanup();
            g_node.reset();
        }
        
        if (rclcpp::ok()) {
            rclcpp::shutdown();
        }
        
        return -1;
    }
}