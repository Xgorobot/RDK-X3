//这里仅是简单demo，用于演示。  机器狗下位机的完整功能，请参考xgo官方的串口协议：https://www.yuque.com/luwudynamics/cn/acdzm2yqrkml35m7
//修改时间：2024年8月26日
//作者：唐代林
//


#include "xgo_body/xgo_body.hpp"

//一些通用的方法，转化数据类型。
float Saturation(float data,float min_limit,float max_limit){
    if(data<min_limit)
        return min_limit;
    else if(data>max_limit)
        return max_limit;
    else
        return data;
}

float uint82float(uint8_t data,float min_limit,float max_limit){
    return (float)data/255.0*(max_limit-min_limit)+min_limit;
}

uint8_t float2uint8(float data, float min_limit,float max_limit){
    return (uint8_t)((data-min_limit)/(max_limit-min_limit)*255);
}

float bytes2float(uint8_t* databytes){
    return *(float*)(databytes);
}



//XGO_BODY类
XGO_BODY::XGO_BODY() : Node("xgo_body")
{
    RCLCPP_INFO(rclcpp::get_logger("XgoRos"), "Initializing xgo node...");
    initParam();
    initCOM();
    initSubcriber();
    initPublisher();
}


//ROS1版本的旧代码，用于获取参数。 分别是最大速度、各关节角度的限制。 
bool XGO_BODY::initParam(){
        return true;
        /*
    if(nh_.get_parameter("vx_max",vx_max) &&
       nh_.get_parameter("vy_max",vy_max) &&
       nh_.get_parameter("vyaw_max",vyaw_max) &&
       nh_.get_parameter("period_limit",period_limit) &&
       nh_.get_parameter("joint_limit",joint_limit_xml) &&
       nh_.get_parameter("arm_limit",arm_limit_xml) &&
       nh_.get_parameter("body_limit",body_limit_xml) &&
       nh_.get_parameter("enable_joint_gui",enable_joint_gui))
       {
            joint_limit.resize(joint_limit_xml.size());
            for(int i=0;i<joint_limit_xml.size();++i){
                for(int j=0;j<joint_limit_xml[i].size();++j){
                    joint_limit[i].push_back(static_cast<int>(joint_limit_xml[i][j]));
                }
            }

            arm_limit.resize(arm_limit_xml.size());
            for(int i=0;i<arm_limit_xml.size();++i){
                for(int j=0;j<arm_limit_xml[i].size();++j){
                    arm_limit[i].push_back(static_cast<int>(arm_limit_xml[i][j]));
                }
            }

            body_limit.resize(body_limit_xml.size());
            for(int i=0;i<body_limit_xml.size();++i){
                for(int j=0;j<body_limit_xml[i].size();++j){
                    body_limit[i].push_back(static_cast<int>(body_limit_xml[i][j]));
                }
            }
            return true;
       }else{
            RCLCPP_ERROR(rclcpp::get_logger("XgoRos"), "Initializing param failed!");
            return false;
       }
*/

}



//启动下位机的COM口。在国产地平线x3 modal板上， 这个Port为"/dev/ttyS3"。如用在其它板子上，请联系硬件开发人员，获取串口名称。 
bool XGO_BODY::initCOM(){
        /*
    if(nh_.get_parameter("com_port",com_port_str)){
        std::cout<<"Connecting to "<<com_port_str<<std::endl;
    }
    */
    std::cout << "xgo serial device: " << com_port_str << std::endl ;
    std::strcpy(com_port,com_port_str.c_str());
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    ser.setPort(com_port_str);
    ser.setBaudrate(115200);
    ser.setTimeout(to);
    ser.open();
    if(ser.isOpen())
    {
            std::cout << "open successful." << std::endl;
    }
    else
    {
            std::cout << "open failed." << std::endl;
    }
    sendOrder(0x08,0x01);
    std::cout << "xgo serial OK" << std::endl;
    return true;
}


//订阅命令。 目前只用到通用的“/cmd_vel”， 和我自定义的“/xgo_action”, 即"动作指令". 
void XGO_BODY::initSubcriber(){
    //sub_cmd_vel_ = nh_.subscribe("/cmd_vel",10, &XGO_BODY::cmdvelCallback, this);
    //sub_body_pose_ = nh_.subscribe("/body_pose",10, &XGO_BODY::bodyposeCallback, this);
    //sub_arm_pose_ = nh_.subscribe("/arm_pose",10, &XGO_BODY::armposeCallback, this);
    //sub_joint_angle_ = nh_.subscribe("/joint_angle",10, &XGO_BODY::jointangleCallback, this);
    //sub_leg_pose_ = nh_.subscribe("/leg_pose",10, &XGO_BODY::legposeCallback, this);

    subscription_cmd_vel = this->create_subscription<geometry_msgs::msg::Twist>( "/cmd_vel", 10, std::bind(&XGO_BODY::cmdvelCallback, this, std::placeholders::_1));
    subscription_xgo_action = this->create_subscription<std_msgs::msg::UInt8>( "/xgo_action", 10, std::bind(&XGO_BODY::xgoActionCallback, this, std::placeholders::_1));

    publisher_battery = this->create_publisher<std_msgs::msg::Float32>("/xgo_battery", 10);
    timer_battery = this->create_wall_timer(3s, std::bind(&XGO_BODY::timer_callback_battery, this));
    timer_serial_read = this->create_wall_timer(100ms, std::bind(&XGO_BODY::timer_callback_serial_read, this));
}

void XGO_BODY::initPublisher(){
    //if(!enable_joint_gui){
    //   pub_joint_ = nh_.advertise<sensor_msgs::msg::JointState>("/joint_states",10);
   // }
}

void XGO_BODY::readState(){
    int readLen = ser.available();
    //std::cout << "xgo serial data left: " << readLen << " bits" << std::endl;
    //
    if(readLen > 255)
    {
	std::cout << "xgo serial data left: " << readLen << " bits, will be dropped." << std::endl;
    // int rubishSize = 1024000;
	uint8_t rubish[1024000];
	ser.read(rubish, readLen);
	return;
    }

    uint8_t tempPtr = 0;
    if(readLen>0){
        //std::cout << "before, read from xgo: " << readLen << " bits" << std::endl;
        readLen = ser.read(rxBuffer,readLen);
        //std::cout << "after, read from xgo: " << readLen << " bits" << std::endl;
        while(tempPtr!=readLen){
            switch(rxFlag){
                case 0:
                    if(rxBuffer[tempPtr]==0x55)
                        rxFlag = 1;
                    break;
                case 1:
                    if(rxBuffer[tempPtr]==0x00)
                        rxFlag = 2;
                    else
                        rxFlag = 0;
                    break;
                case 2:
                    rxData[rxPtr++] = rxBuffer[tempPtr];
                    if(rxPtr==40){
                        rxFlag = 3;
                    }
                    break;
                case 3:
                    if(rxBuffer[tempPtr]==0x00)
                        rxFlag = 4;
                    else
                        rxFlag = 0;
                    break;
                case 4:
                    if(rxBuffer[tempPtr]==0xAA)
                        updateState();
                    rxFlag = 0;
                    rxPtr = 0;
                    break;
                default:
                    rxFlag = 0;
                    rxPtr = 0;
                    break;
            }
            tempPtr++;
        }
    }
}

void XGO_BODY::timer_callback_serial_read()
{
	readState();
}



void XGO_BODY::timer_callback_battery()
{
	battery = rxData[0];
        std::cout << "battery: " << battery << std::endl;
	auto message = std_msgs::msg::Float32();
	message.data = battery; 
	this->publisher_battery->publish(message);	
}

void XGO_BODY::updateState(){
    //battery = rxData[0];
    //std::cout << "battery: " << battery << std::endl;


    /*
    for(int i=0;i<15;i++){
        if(i<12){
            joint_angle[i] = uint82float(rxData[1+i],joint_limit[i%3][0],joint_limit[i%3][1])/57.3;
        }else{
            joint_angle[i] = uint82float(rxData[1+i],joint_limit[i-9][0],joint_limit[i-9][1])/57.3;
        }
    }
    imu_angle[0] = bytes2float(rxData+16);
    imu_angle[1] = bytes2float(rxData+20);
    imu_angle[2] = bytes2float(rxData+24);
    imu_acc[0] = bytes2float(rxData+28);
    imu_acc[1] = bytes2float(rxData+32);
    imu_acc[2] = bytes2float(rxData+36);
    pubJointState();
    */
}

void XGO_BODY::cmdvelCallback(const geometry_msgs::msg::Twist& msg){
     std::cout << "/cmd_vel received!" << std::endl;
     vx = Saturation(msg.linear.x,-vx_max,vx_max);
     vy = Saturation(msg.linear.y,-vy_max,vy_max);
     vyaw = Saturation(msg.angular.z,-vyaw_max,vyaw_max);
     sendSpeed();
}


void XGO_BODY::xgoActionCallback(const std_msgs::msg::UInt8& msg){
     std::cout << "/xgo_action received! " << static_cast<unsigned>(msg.data) << std::endl;
     sendOrder(0x3E, msg.data);
}


/*
void XGO_BODY::jointangleCallback(const xgo_ros::JointAngle& msg){
    for(int i=0;i<15;i++){
        joint_angle[i] = msg.angle[i];
        sendMotorAngle();
    }
}

void XGO_BODY::bodyposeCallback(const xgo_ros::BodyPose& msg){
    for(int i=0;i<6;i++){
        body_pose[i] = msg.pose[i];
        sendBodyPose();
    }
}

void XGO_BODY::legposeCallback(const xgo_ros::LegPose& msg){
    for(int i=0;i<12;i++){
        leg_pose[i] = msg.pose[i];
        sendLegPose();
    }
}

void XGO_BODY::armposeCallback(const xgo_ros::ArmPose& msg){
    for(int i=0;i<3;i++){
        arm_pose[i] = msg.pose[i];
        sendArmPose();
    }
}
*/

void XGO_BODY::sendOrder(uint8_t addr,uint8_t data){
    OrderPacket packet;
    packet.addr = addr;
    packet.length = 1;
    packet.message[0] = data;
    sendOrder(packet);
}

void XGO_BODY::sendOrder(OrderPacket packet){
    uint8_t data[50];
    data[0]=0x55;data[1]=0x00;
    data[2]=packet.length+8;
    data[3]=0x01;
    data[4]=packet.addr;
    data[5+packet.length] = data[2] + data[3] +data[4];
    for(int i=0;i<packet.length;i++){
        data[5+i] = packet.message[i];
        data[5+packet.length] += data[5+i];
    }
    data[5+packet.length] = ~data[5+packet.length];
    data[6+packet.length] = 0x00;
    data[7+packet.length] = 0xAA;
    ser.write(data,8+packet.length);
}

void XGO_BODY::action(uint8_t action_id){
    sendOrder(0x3E,action_id);
}

void XGO_BODY::reset(){
    action(0xFF);
}

void XGO_BODY::sendBodyPose(){
    for(int i=0;i<6;i++){
        sendOrder(0x33+i,float2uint8(body_pose[i],body_limit[i][0],body_limit[i][1]));
    }
}

void XGO_BODY::sendLegPose(){
    OrderPacket packet;
    packet.addr = 0x40;
    packet.length = 12;
    for(int i=0;i<12;i++){
        packet.message[i] = float2uint8(leg_pose[i],body_limit[i%3][0],body_limit[i%3][1]);
    }
    sendOrder(packet);
}

void XGO_BODY::sendArmPose(){
    sendOrder(0x71,float2uint8(arm_pose[0],body_limit[0][0],body_limit[0][1]));
    sendOrder(0x73,float2uint8(arm_pose[1],body_limit[1][0],body_limit[1][1]));
    sendOrder(0x74,float2uint8(arm_pose[2],body_limit[2][0],body_limit[2][1]));
}

void XGO_BODY::sendMotorAngle(){
    OrderPacket packet;
    packet.addr = 0x50;
    packet.length = 15;
    for(int i=0;i<15;i++){
        if(i<12)
            packet.message[i] = float2uint8(joint_angle[i],joint_limit[i%3][0],joint_limit[i%3][1]);
        else
            packet.message[i] = float2uint8(joint_angle[i],joint_limit[i-9][0],joint_limit[i-9][1]);
    }
    sendOrder(packet);
}

void XGO_BODY::sendSpeed(){
    sendOrder(0x30,float2uint8(vx,-vx_max,vx_max));
    sendOrder(0x31,float2uint8(vy,-vy_max,vy_max));
    sendOrder(0x32,float2uint8(vyaw,-vyaw_max,vyaw_max));
}

void XGO_BODY::pubJointState(){
/*
 * if(!enable_joint_gui){
        sensor_msgs::msg::JointState jointState;
        jointState.header.stamp = rclcpp::Time::now();
        jointState.name={"13_Joint", "12_Joint", "11_Joint", "23_Joint", "22_Joint", "21_Joint", "33_Joint", "32_Joint", "31_Joint", "43_Joint", "42_Joint", "41_Joint", "53_Joint", "52_Joint", "51_Joint", "50_Joint", "500_Joint"};
        jointState.position={joint_angle[2],joint_angle[1],joint_angle[0],joint_angle[5],joint_angle[4],joint_angle[3],joint_angle[8],joint_angle[7],joint_angle[6],joint_angle[11],joint_angle[10],joint_angle[9],-joint_angle[14],joint_angle[13],joint_angle[12],0.0,0.0};
        pub_joint_.publish(jointState);
    }
    */
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XGO_BODY>());
  rclcpp::shutdown();

  return 0;
}

