#ifndef __XGO_BODY_H
#define __XGO_BODY_H
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include <serial/serial.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <geometry_msgs/msg/twist.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include "std_msgs/msg/float32.hpp"
#include <sensor_msgs/msg/joint_state.hpp>

#include "xgo_body/msg/joint_angle.hpp"
#include "xgo_body/msg/body_pose.hpp"
#include "xgo_body/msg/leg_pose.hpp"
#include "xgo_body/msg/arm_pose.hpp"

using namespace std::chrono_literals;

typedef struct{
    uint8_t addr;
    uint8_t length;
    uint8_t message[40];
}OrderPacket;

class XGO_BODY  : public rclcpp::Node
{
    public:
        XGO_BODY();
        void readState();
    private:
        //rclcpp::Node* nh_;
        //bool enable_joint_gui;
//        XmlRpc::XmlRpcValue joint_limit_xml;
//        XmlRpc::XmlRpcValue body_limit_xml;
//        XmlRpc::XmlRpcValue arm_limit_xml;
        std::string com_port_str = "/dev/ttyS3";  //for x3 modual. 
        char com_port[30];
        uint8_t rxFlag=0;
        uint8_t rxBuffer[256];
        uint8_t rxData[256];
        uint8_t rxPtr=0;

        //ros::Subscriber sub_cmd_vel_;
        //ros::Subscriber sub_body_pose_;
        //ros::Subscriber sub_arm_pose_;
        //ros::Subscriber sub_joint_angle_;
        //ros::Subscriber sub_leg_pose_;

        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_cmd_vel;
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr subscription_xgo_action;

	rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_battery;

        serial::Serial ser;
        //ros::Publisher pub_joint_;

        float vx=0;
        float vy=0;
        float vyaw=0;
        float battery;
        float body_pose[6]={0,0,108,0,0,0};
        float joint_angle[15];
        float leg_pose[12]={0,0,108,0,0,108,0,0,108,0,0,108};
        float arm_pose[3] = {85,85,3};
        float imu_angle[3];
        float imu_acc[3];
        float vx_max = 0.3;
        float vy_max = 0.2;
        float vyaw_max = 1.2;
        std::vector<float> period_limit;
        std::vector<std::vector<int>> joint_limit;
        std::vector<std::vector<int>> body_limit;
        std::vector<std::vector<int>> arm_limit;
	rclcpp::TimerBase::SharedPtr timer_battery;
	rclcpp::TimerBase::SharedPtr timer_serial_read;
	void timer_callback_battery();
	void timer_callback_serial_read();

        void sendSpeed();
        void sendOrder(uint8_t addr,uint8_t data);
        void action(uint8_t action_id);
        void reset();
        void sendMotorAngle();
        void updateState();
        void pubJointState();
        void sendBodyPose();
        void sendLegPose();
        void sendArmPose();
        bool initParam();
        bool initCOM();
        void initSubcriber();
        void initPublisher();
        void sendOrder(OrderPacket packet);
        void cmdvelCallback(const geometry_msgs::msg::Twist& msg);
        void xgoActionCallback(const std_msgs::msg::UInt8& msg);
        //void legposeCallback(const xgo_ros::LegPose& msg);
        //void bodyposeCallback(const xgo_ros::BodyPose& msg);
        //void jointangleCallback(const xgo_ros::JointAngle& msg);
       // void armposeCallback(const xgo_ros::ArmPose& msg);
};

float Saturation(float data,float min_limit,float max_limit);
uint8_t float2uint8(float data, float min_limit,float max_limit);
float uint82float(uint8_t data,float min_limit,float max_limit);

#endif

