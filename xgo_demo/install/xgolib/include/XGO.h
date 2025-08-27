#ifndef XGO_H
#define XGO_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

class XGO {
public:
    // 构造函数和析构函数
    XGO(const std::string& port, int baud = 115200, const std::string& version = "xgomini", bool verbose = false, bool fast_startup = false);
    ~XGO();

    // 基本运动控制
    void stop();
    void move(char direction, float step);
    void move_x(float step, float runtime = 0);
    void move_y(float step, float runtime = 0);
    void turn(float step, float runtime = 0);
    void forward(float step);
    void back(float step);
    void left(float step);
    void right(float step);
    void turnleft(float step);
    void turnright(float step);

    // 精确移动控制
    void move_x_by(float distance, float vx = 18, float k = 0.035, float mintime = 0.55);
    void move_y_by(float distance, float vy = 18, float k = 0.0373, float mintime = 0.5);
    void turn_by(float theta, float mintime, float vyaw = 16, float k = 0.08);
    void turn_to(float theta, float vyaw = 60, float emax = 10);

    // 姿态控制
    void translation(char direction, float data);
    void translation(const std::vector<char>& direction, const std::vector<float>& data);
    void attitude(char direction, float data);
    void attitude(const std::vector<char>& direction, const std::vector<float>& data);

    // 动作控制
    void action(int action_id, float wait_seconds = 3.0);
    void reset();

    // 腿部控制
    void leg(int leg_id, const std::vector<float>& data);
    void motor(int motor_id, float data);
    void motor(const std::vector<int>& motor_id, const std::vector<float>& data);

    // 电机控制
    void unload_motor(int leg_id);
    void unload_allmotor();
    void load_motor(int leg_id);
    void load_allmotor();
    void motor_speed(int speed);

    // 周期性运动
    void periodic_rot(char direction, float period);
    void periodic_rot(const std::vector<char>& direction, const std::vector<float>& period);
    void periodic_tran(char direction, float period);
    void periodic_tran(const std::vector<char>& direction, const std::vector<float>& period);

    // 踏步控制
    void mark_time(float data);
    void pace(const std::string& mode);
    void gait_type(const std::string& mode);

    // 系统控制
    void imu(int mode);
    void perform(int mode);
    void calibration(const std::string& state);
    void set_origin();

    // 机械臂控制
    void arm(float arm_x, float arm_z);
    void arm_polar(float arm_theta, float arm_r);
    void arm_mode(int mode);
    void claw(float pos);
    void arm_speed(int speed);
    void moveToMid();

    // 示教功能
    void teach(const std::string& mode, int pos_id);
    void teach_arm(const std::string& mode, int pos_id);

    // 数据读取
    std::vector<float> read_motor();
    int read_battery();
    std::string read_firmware();
    float read_roll();
    float read_pitch();
    float read_yaw();
    int read_analog();
    int read_digital();
    int read_rotate_state();
    std::vector<float> read_imu();
    int read_imu_int16(const std::string& direction);

    // 其他功能
    void bt_rename(const std::string& name);
    void move_to(int data, bool wait = true, float overtime = 15.0);
    void output_analog(int data);
    void output_digital(int data);
    void extern_motor(int position);
    void set_move_mintime(float mintime);
    std::string read_lib_version();

private:
    // 串口相关
#ifdef _WIN32
    HANDLE hSerial;
#else
    int serial_fd;
#endif
    std::string port;
    int baud_rate;
    bool verbose;
    std::string version;
    float mintime;
    float init_yaw;
    bool mFastStartup;  // 快速启动标志

    // 通信协议相关
    int rx_FLAG;
    int rx_COUNT;
    int rx_ADDR;
    int rx_LEN;
    int rx_TYPE;
    std::vector<uint8_t> rx_data;

    // 内部方法
    bool init_serial();
    void close_serial();
    void send_command(const std::string& key, int index = 1, int len = 1);
    void read_command(uint8_t addr, int read_len = 1);
    bool unpack_data(float timeout = 1.0);
    void change_baud(int baud);
    
    // 数据转换
    uint8_t conver2u8(float data, float limit, uint8_t min_value = 0);
    uint8_t conver2u8(float data, const std::vector<float>& limit, uint8_t min_value = 0);
    float conver2float(uint8_t data, float limit);
    float conver2float(uint8_t data, const std::vector<float>& limit);
    float byte2float(const std::vector<uint8_t>& rawdata);
    int16_t byte2short(const std::vector<uint8_t>& rawdata);
    
    // 参数查找
    int search(char data, const std::vector<char>& list);
    
    // 参数设置
    void change_param(const std::string& version);
    
    // 内部姿态和平移控制
    void internal_translation(char direction, float data);
    void internal_attitude(char direction, float data);
    void internal_periodic_rot(char direction, float period);
    void internal_periodic_tran(char direction, float period);
    void internal_motor(int index, float data);
    
    // IMU数据解包
    std::vector<float> unpack_imu();
    std::vector<float> unpack_imu_r();
};

// 全局命令映射和参数
extern std::map<std::string, std::vector<uint8_t>> XGOorder;
extern std::map<std::string, std::vector<float>> XGOparam;

#endif // XGO_H