#include "XGO.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <thread>
#include <chrono>
#include <algorithm>

// 版本信息
static const std::string LIB_VERSION = "1.0.0";
static const std::string LAST_MODIFIED = "2024/12/23";

// 全局命令映射
std::map<std::string, std::vector<uint8_t>> XGOorder = {
    {"BATTERY", {0x01, 100}},
    {"PERFORM", {0x03, 0}},
    {"CALIBRATION", {0x04, 0}},
    {"UPGRADE", {0x05, 0}},
    {"SET_ORIGIN", {0x06, 1}},
    {"FIRMWARE_VERSION", {0x07}},
    {"GAIT_TYPE", {0x09, 0x00}},
    {"BT_NAME", {0x13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"UNLOAD_MOTOR", {0x20, 0}},
    {"LOAD_MOTOR", {0x20, 0}},
    {"VX", {0x30, 128}},
    {"VY", {0x31, 128}},
    {"VYAW", {0x32, 128}},
    {"TRANSLATION", {0x33, 0, 0, 0}},
    {"ATTITUDE", {0x36, 0, 0, 0}},
    {"PERIODIC_ROT", {0x39, 0, 0, 0}},
    {"MarkTime", {0x3C, 0}},
    {"MOVE_MODE", {0x3D, 0}},
    {"ACTION", {0x3E, 0}},
    {"MOVE_TO", {0x3F, 0, 0}},
    {"PERIODIC_TRAN", {0x80, 0, 0, 0}},
    {"MOTOR_ANGLE", {0x50, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}},
    {"MOTOR_SPEED", {0x5C, 1}},
    {"MOVE_TO_MID", {0x5F, 1}},
    {"LEG_POS", {0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"IMU", {0x61, 0}},
    {"ROLL", {0x62, 0}},
    {"PITCH", {0x63, 0}},
    {"TEACH_RECORD", {0x21, 0}},
    {"TEACH_PLAY", {0x22, 0}},
    {"TEACH_ARM_RECORD", {0x23, 0}},
    {"TEACH_ARM_PLAY", {0x24, 0}},
    {"YAW", {0x64, 0}},
    {"CLAW", {0x71, 0}},
    {"ARM_MODE", {0x72, 0}},
    {"ARM_X", {0x73, 0}},
    {"ARM_Z", {0x74, 0}},
    {"ARM_SPEED", {0x75, 0}},
    {"ARM_THETA", {0x76, 0}},
    {"ARM_R", {0x77, 0}},
    {"OUTPUT_ANALOG", {0x90, 0}},
    {"OUTPUT_DIGITAL", {0x91, 0}},
    {"LED_COLOR", {0x69, 0, 0, 0}},
    {"ANALOG_READ", {0x92, 0}},
    {"DIGITIAL_READ", {0x93, 0}},
    {"EX_MOTOR", {0xA1, 0x08, 0x00}}
};

// 全局参数映射
std::map<std::string, std::vector<float>> XGOparam;

XGO::XGO(const std::string& port, int baud, const std::string& version, bool verbose, bool fast_startup)
    : port(port), baud_rate(baud), verbose(verbose), version(version), mintime(0.65),
      rx_FLAG(0), rx_COUNT(0), rx_ADDR(0), rx_LEN(0), rx_TYPE(0), rx_data(50) {
    this->mFastStartup = fast_startup;
    
    if (!init_serial()) {
        std::cerr << "Failed to initialize serial port" << std::endl;
        return;
    }
    
    // Fast startup: 250ms -> 50ms
    std::this_thread::sleep_for(std::chrono::milliseconds(fast_startup ? 50 : 250));
    
    // 读取固件版本并设置参数
    std::string fw_version = read_firmware();
    if (!fw_version.empty() && fw_version[0] == 'M') {
        change_param("xgomini");
    } else if (!fw_version.empty() && fw_version[0] == 'L') {
        change_param("xgolite");
    } else {
        std::cerr << "ERROR! Can't read firmware version!" << std::endl;
        change_param("xgomini"); // 默认使用xgomini参数
    }
    
    reset();
    init_yaw = read_yaw();
    // Fast startup: 1s -> 200ms
    std::this_thread::sleep_for(std::chrono::milliseconds(fast_startup ? 200 : 1000));
}

XGO::~XGO() {
    close_serial();
}

bool XGO::init_serial() {
#ifdef _WIN32
    hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;
    }
    
    dcbSerialParams.BaudRate = baud_rate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;
    }
    
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        CloseHandle(hSerial);
        return false;
    }
    
    return true;
#else
    serial_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        return false;
    }
    
    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        close(serial_fd);
        return false;
    }
    
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        close(serial_fd);
        return false;
    }
    
    return true;
#endif
}

void XGO::close_serial() {
#ifdef _WIN32
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
#else
    if (serial_fd >= 0) {
        close(serial_fd);
    }
#endif
}

void XGO::change_param(const std::string& version) {
    if (version == "xgomini") {
        XGOparam["TRANSLATION_LIMIT"] = {35, 19.5, 75, 120}; // X Y Z_min Z_max
        XGOparam["ATTITUDE_LIMIT"] = {20, 22, 16}; // Roll Pitch Yaw
        XGOparam["LEG_LIMIT"] = {35, 18, 75, 115}; // X Y Z_min Z_max
        XGOparam["VX_LIMIT"] = {25};
        XGOparam["VY_LIMIT"] = {18};
        XGOparam["VYAW_LIMIT"] = {100};
        XGOparam["MARK_TIME_LIMIT"] = {10, 35};
        XGOparam["PERIOD_LIMIT"] = {1.5, 8};
        XGOparam["ARM_LIMIT"] = {-80, 155, -95, 155, 70, 270, 80, 140};
    } else if (version == "xgolite") {
        XGOparam["TRANSLATION_LIMIT"] = {25, 18, 60, 110};
        XGOparam["ATTITUDE_LIMIT"] = {20, 10, 12};
        XGOparam["LEG_LIMIT"] = {25, 18, 60, 110};
        XGOparam["VX_LIMIT"] = {25};
        XGOparam["VY_LIMIT"] = {18};
        XGOparam["VYAW_LIMIT"] = {100};
        XGOparam["MARK_TIME_LIMIT"] = {10, 25};
        XGOparam["PERIOD_LIMIT"] = {1.5, 8};
        XGOparam["ARM_LIMIT"] = {-80, 155, -95, 155, 70, 270, 80, 140};
    }
}

uint8_t XGO::conver2u8(float data, float limit, uint8_t min_value) {
    float max_value = 255.0f;
    if (data >= limit) {
        return static_cast<uint8_t>(max_value);
    } else if (data <= -limit) {
        return min_value;
    } else {
        return static_cast<uint8_t>(255.0f / (2 * limit) * (data + limit));
    }
}

uint8_t XGO::conver2u8(float data, const std::vector<float>& limit, uint8_t min_value) {
    float max_value = 255.0f;
    if (data >= limit[1]) {
        return static_cast<uint8_t>(max_value);
    } else if (data <= limit[0]) {
        return min_value;
    } else {
        return static_cast<uint8_t>(255.0f / (limit[1] - limit[0]) * (data - limit[0]));
    }
}

float XGO::conver2float(uint8_t data, float limit) {
    return (data - 128.0f) / 255.0f * limit;
}

float XGO::conver2float(uint8_t data, const std::vector<float>& limit) {
    return data / 255.0f * (limit[1] - limit[0]) + limit[0];
}

float XGO::byte2float(const std::vector<uint8_t>& rawdata) {
    if (rawdata.size() < 4) return 0.0f;
    
    uint32_t temp = (static_cast<uint32_t>(rawdata[3]) << 24) |
                    (static_cast<uint32_t>(rawdata[2]) << 16) |
                    (static_cast<uint32_t>(rawdata[1]) << 8) |
                    static_cast<uint32_t>(rawdata[0]);
    
    return *reinterpret_cast<float*>(&temp);
}

int16_t XGO::byte2short(const std::vector<uint8_t>& rawdata) {
    if (rawdata.size() < 2) return 0;
    
    return static_cast<int16_t>((rawdata[0] << 8) | rawdata[1]);
}

int XGO::search(char data, const std::vector<char>& list) {
    for (size_t i = 0; i < list.size(); ++i) {
        if (data == list[i]) {
            return static_cast<int>(i + 1);
        }
    }
    return -1;
}

void XGO::send_command(const std::string& key, int index, int len) {
    if (XGOorder.find(key) == XGOorder.end()) {
        std::cerr << "Unknown command: " << key << std::endl;
        return;
    }
    
    uint8_t mode = 0x00;
    uint8_t order = XGOorder[key][0] + index - 1;
    std::vector<uint8_t> value;
    uint8_t value_sum = 0;
    
    for (int i = 0; i < len; ++i) {
        if (index + i < static_cast<int>(XGOorder[key].size())) {
            value.push_back(XGOorder[key][index + i]);
            value_sum += XGOorder[key][index + i];
        }
    }
    
    uint8_t sum_data = ((len + 0x08) + mode + order + value_sum) % 256;
    sum_data = 255 - sum_data;
    
    std::vector<uint8_t> tx = {0x55, 0x00, static_cast<uint8_t>(len + 0x08), mode, order};
    tx.insert(tx.end(), value.begin(), value.end());
    tx.push_back(sum_data);
    tx.push_back(0x00);
    tx.push_back(0xAA);
    
#ifdef _WIN32
    DWORD bytes_written;
    WriteFile(hSerial, tx.data(), tx.size(), &bytes_written, NULL);
#else
    write(serial_fd, tx.data(), tx.size());
#endif
    
    if (verbose) {
        std::cout << "tx_data: ";
        for (uint8_t byte : tx) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }
}

void XGO::read_command(uint8_t addr, int read_len) {
    // 清空接收缓冲区
#ifdef _WIN32
    PurgeComm(hSerial, PURGE_RXCLEAR);
#else
    tcflush(serial_fd, TCIFLUSH);
#endif
    
    uint8_t mode = 0x02;
    uint8_t sum_data = (0x09 + mode + addr + read_len) % 256;
    sum_data = 255 - sum_data;
    
    std::vector<uint8_t> tx = {0x55, 0x00, 0x09, mode, addr, static_cast<uint8_t>(read_len), sum_data, 0x00, 0xAA};
    
#ifdef _WIN32
    DWORD bytes_written;
    WriteFile(hSerial, tx.data(), tx.size(), &bytes_written, NULL);
#else
    write(serial_fd, tx.data(), tx.size());
#endif
    
    if (verbose) {
        std::cout << "tx_data: ";
        for (uint8_t byte : tx) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }
}

bool XGO::unpack_data(float timeout) {
    auto start_time = std::chrono::steady_clock::now();
    std::vector<uint8_t> rx_msg;
    
    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        
        if (elapsed >= timeout * 1000) {
            break;
        }
        
        uint8_t buffer[256];
        int bytes_read = 0;
        
#ifdef _WIN32
        DWORD dwBytesRead;
        if (ReadFile(hSerial, buffer, sizeof(buffer), &dwBytesRead, NULL)) {
            bytes_read = dwBytesRead;
        }
#else
        bytes_read = read(serial_fd, buffer, sizeof(buffer));
        if (bytes_read < 0) bytes_read = 0;
#endif
        
        if (bytes_read > 0) {
            for (int i = 0; i < bytes_read; ++i) {
                uint8_t num = buffer[i];
                rx_msg.push_back(num);
                
                if (rx_FLAG == 0) {
                    if (num == 0x55) {
                        rx_FLAG = 1;
                    } else {
                        rx_FLAG = 0;
                    }
                } else if (rx_FLAG == 1) {
                    if (num == 0x00) {
                        rx_FLAG = 2;
                    } else {
                        rx_FLAG = 0;
                    }
                } else if (rx_FLAG == 2) {
                    rx_LEN = num;
                    rx_FLAG = 3;
                } else if (rx_FLAG == 3) {
                    rx_TYPE = num;
                    rx_FLAG = 4;
                } else if (rx_FLAG == 4) {
                    rx_ADDR = num;
                    rx_FLAG = 5;
                    rx_COUNT = 0;
                } else if (rx_FLAG == 5) {
                    if (rx_COUNT == (rx_LEN - 9)) {
                        rx_data[rx_COUNT] = num;
                        rx_FLAG = 6;
                    } else if (rx_COUNT < rx_LEN - 9) {
                        rx_data[rx_COUNT] = num;
                        rx_COUNT++;
                    }
                } else if (rx_FLAG == 6) {
                    uint8_t rx_CHECK = 0;
                    for (int j = 0; j < (rx_LEN - 8); ++j) {
                        rx_CHECK += rx_data[j];
                    }
                    rx_CHECK = 255 - (rx_LEN + rx_TYPE + rx_ADDR + rx_CHECK) % 256;
                    if (num == rx_CHECK) {
                        rx_FLAG = 7;
                    } else {
                        rx_FLAG = 0;
                        rx_COUNT = 0;
                        rx_ADDR = 0;
                        rx_LEN = 0;
                    }
                } else if (rx_FLAG == 7) {
                    if (num == 0x00) {
                        rx_FLAG = 8;
                    } else {
                        rx_FLAG = 0;
                        rx_COUNT = 0;
                        rx_ADDR = 0;
                        rx_LEN = 0;
                    }
                } else if (rx_FLAG == 8) {
                    if (num == 0xAA) {
                        rx_FLAG = 0;
                        if (verbose) {
                            std::cout << "rx_data: ";
                            for (uint8_t byte : rx_msg) {
                                std::cout << static_cast<int>(byte) << " ";
                            }
                            std::cout << std::endl;
                        }
                        return true;
                    } else {
                        rx_FLAG = 0;
                        rx_COUNT = 0;
                        rx_ADDR = 0;
                        rx_LEN = 0;
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    return false;
}

// 基本运动控制实现
void XGO::stop() {
    move_x(0);
    move_y(0);
    mark_time(0);
    turn(0);
}

void XGO::move(char direction, float step) {
    if (direction == 'x' || direction == 'X') {
        move_x(step);
    } else if (direction == 'y' || direction == 'Y') {
        move_y(step);
    } else {
        std::cerr << "ERROR! Invalid direction!" << std::endl;
    }
}

void XGO::move_x(float step, float runtime) {
    XGOorder["VX"][1] = conver2u8(step, XGOparam["VX_LIMIT"][0]);
    send_command("VX");
    if (runtime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
        XGOorder["VX"][1] = conver2u8(0, XGOparam["VX_LIMIT"][0]);
        send_command("VX");
    }
}

void XGO::move_y(float step, float runtime) {
    XGOorder["VY"][1] = conver2u8(step, XGOparam["VY_LIMIT"][0]);
    send_command("VY");
    if (runtime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
        XGOorder["VY"][1] = conver2u8(0, XGOparam["VY_LIMIT"][0]);
        send_command("VY");
    }
}

void XGO::turn(float step, float runtime) {
    XGOorder["VYAW"][1] = conver2u8(step, XGOparam["VYAW_LIMIT"][0]);
    send_command("VYAW");
    if (runtime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
        XGOorder["VYAW"][1] = conver2u8(0, XGOparam["VYAW_LIMIT"][0]);
        send_command("VYAW");
    }
}

void XGO::forward(float step) {
    move_x(std::abs(step));
}

void XGO::back(float step) {
    move_x(-std::abs(step));
}

void XGO::left(float step) {
    move_y(std::abs(step));
}

void XGO::right(float step) {
    move_y(-std::abs(step));
}

void XGO::turnleft(float step) {
    turn(std::abs(step));
}

void XGO::turnright(float step) {
    turn(-std::abs(step));
}

void XGO::move_x_by(float distance, float vx, float k, float mintime) {
    float runtime = k * std::abs(distance) + mintime;
    move_x(std::copysign(vx, distance));
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
    move_x(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void XGO::move_y_by(float distance, float vy, float k, float mintime) {
    float runtime = k * std::abs(distance) + mintime;
    move_y(std::copysign(vy, distance));
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
    move_y(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void XGO::turn_by(float theta, float mintime, float vyaw, float k) {
    float runtime = std::abs(theta) * k + mintime;
    turn(std::copysign(vyaw, theta));
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(runtime * 1000)));
    turn(0);
}

void XGO::turn_to(float theta, float vyaw, float emax) {
    float cur_yaw = read_yaw();
    float des_yaw = init_yaw + theta;
    while (std::abs(des_yaw - cur_yaw) >= emax) {
        turn(std::copysign(vyaw, des_yaw - cur_yaw));
        cur_yaw = read_yaw();
    }
    turn(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// 姿态控制实现
void XGO::internal_translation(char direction, float data) {
    std::vector<char> dirs = {'x', 'y', 'z'};
    int index = search(direction, dirs);
    if (index == -1) {
        std::cerr << "ERROR! Direction must be 'x', 'y' or 'z'" << std::endl;
        return;
    }
    
    if (index <= 2) {
        XGOorder["TRANSLATION"][index] = conver2u8(data, XGOparam["TRANSLATION_LIMIT"][index - 1]);
    } else {
        std::vector<float> z_limit = {XGOparam["TRANSLATION_LIMIT"][2], XGOparam["TRANSLATION_LIMIT"][3]};
        XGOorder["TRANSLATION"][index] = conver2u8(data, z_limit);
    }
    send_command("TRANSLATION", index);
}

void XGO::translation(char direction, float data) {
    internal_translation(direction, data);
}

void XGO::translation(const std::vector<char>& direction, const std::vector<float>& data) {
    if (direction.size() != data.size()) {
        std::cerr << "ERROR! The length of direction and data don't match!" << std::endl;
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        internal_translation(direction[i], data[i]);
    }
}

void XGO::internal_attitude(char direction, float data) {
    std::vector<char> dirs = {'r', 'p', 'y'};
    int index = search(direction, dirs);
    if (index == -1) {
        std::cerr << "ERROR! Direction must be 'r', 'p' or 'y'" << std::endl;
        return;
    }
    
    XGOorder["ATTITUDE"][index] = conver2u8(data, XGOparam["ATTITUDE_LIMIT"][index - 1]);
    send_command("ATTITUDE", index);
}

void XGO::attitude(char direction, float data) {
    internal_attitude(direction, data);
}

void XGO::attitude(const std::vector<char>& direction, const std::vector<float>& data) {
    if (direction.size() != data.size()) {
        std::cerr << "ERROR! The length of direction and data don't match!" << std::endl;
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        internal_attitude(direction[i], data[i]);
    }
}

// 动作控制实现
void XGO::action(int action_id, float wait_seconds) {
    if (action_id <= 0 || action_id > 255) {
        std::cerr << "ERROR! Illegal Action ID!" << std::endl;
        return;
    }
    XGOorder["ACTION"][1] = action_id;
    send_command("ACTION");
    
    if (wait_seconds > 0) {
        // 根据用户指定的秒数等待
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(wait_seconds * 1000)));
    }
}

void XGO::reset() {
    action(255);
    std::this_thread::sleep_for(std::chrono::milliseconds(mFastStartup ? 150 : 1000));
}

// 腿部控制实现
void XGO::leg(int leg_id, const std::vector<float>& data) {
    if (leg_id < 1 || leg_id > 4) {
        std::cerr << "Error! Illegal Index!" << std::endl;
        return;
    }
    if (data.size() != 3) {
        std::cerr << "Error! Illegal Value!" << std::endl;
        return;
    }
    
    std::vector<uint8_t> value(3);
    for (int i = 0; i < 3; ++i) {
        if (i < 2) {
            value[i] = conver2u8(data[i], XGOparam["LEG_LIMIT"][i]);
        } else {
            std::vector<float> z_limit = {XGOparam["LEG_LIMIT"][2], XGOparam["LEG_LIMIT"][3]};
            value[i] = conver2u8(data[i], z_limit);
        }
    }
    
    for (int i = 0; i < 3; ++i) {
        int index = 3 * (leg_id - 1) + i + 1;
        XGOorder["LEG_POS"][index] = value[i];
        send_command("LEG_POS", index);
    }
}

void XGO::internal_motor(int index, float data) {
    // 简化的电机控制，实际实现需要根据电机类型设置不同的限制
    if (index < 13) {
        XGOorder["MOTOR_ANGLE"][index] = conver2u8(data, 90.0f); // 简化的角度限制
    } else if (index == 13) {
        claw(conver2u8(data, 255.0f));
        return;
    } else {
        XGOorder["MOTOR_ANGLE"][index] = conver2u8(data, 90.0f);
    }
    send_command("MOTOR_ANGLE", index);
}

void XGO::motor(int motor_id, float data) {
    std::vector<int> MOTOR_ID = {11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42, 43, 51, 52, 53};
    
    auto it = std::find(MOTOR_ID.begin(), MOTOR_ID.end(), motor_id);
    if (it == MOTOR_ID.end()) {
        std::cerr << "Error! Illegal Index!" << std::endl;
        return;
    }
    
    int index = std::distance(MOTOR_ID.begin(), it) + 1;
    internal_motor(index, data);
}

void XGO::motor(const std::vector<int>& motor_id, const std::vector<float>& data) {
    if (motor_id.size() != data.size()) {
        std::cerr << "Error! Length Mismatching!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < motor_id.size(); ++i) {
        motor(motor_id[i], data[i]);
    }
}

// 电机控制实现
void XGO::unload_motor(int leg_id) {
    if (leg_id < 1 || leg_id > 5) {
        std::cerr << "ERROR! leg_id must be 1, 2, 3, 4 or 5" << std::endl;
        return;
    }
    XGOorder["UNLOAD_MOTOR"][1] = 0x10 + leg_id;
    send_command("UNLOAD_MOTOR");
}

void XGO::unload_allmotor() {
    XGOorder["UNLOAD_MOTOR"][1] = 0x01;
    send_command("UNLOAD_MOTOR");
}

void XGO::load_motor(int leg_id) {
    if (leg_id < 1 || leg_id > 5) {
        std::cerr << "ERROR! leg_id must be 1, 2, 3, 4 or 5" << std::endl;
        return;
    }
    XGOorder["LOAD_MOTOR"][1] = 0x20 + leg_id;
    send_command("LOAD_MOTOR");
}

void XGO::load_allmotor() {
    XGOorder["LOAD_MOTOR"][1] = 0x00;
    send_command("LOAD_MOTOR");
}

void XGO::motor_speed(int speed) {
    if (speed < 0 || speed > 255) {
        std::cerr << "ERROR! Illegal Value! The speed parameter needs to be between 0 and 255!" << std::endl;
        return;
    }
    if (speed == 0) speed = 1;
    XGOorder["MOTOR_SPEED"][1] = speed;
    send_command("MOTOR_SPEED");
}

// 周期性运动实现
void XGO::internal_periodic_rot(char direction, float period) {
    std::vector<char> dirs = {'r', 'p', 'y'};
    int index = search(direction, dirs);
    if (index == -1) {
        std::cerr << "ERROR! Direction must be 'r', 'p' or 'y'" << std::endl;
        return;
    }
    
    if (period == 0) {
        XGOorder["PERIODIC_ROT"][index] = 0;
    } else {
        XGOorder["PERIODIC_ROT"][index] = conver2u8(period, XGOparam["PERIOD_LIMIT"], 1);
    }
    send_command("PERIODIC_ROT", index);
}

void XGO::periodic_rot(char direction, float period) {
    internal_periodic_rot(direction, period);
}

void XGO::periodic_rot(const std::vector<char>& direction, const std::vector<float>& period) {
    if (direction.size() != period.size()) {
        std::cerr << "ERROR! The length of direction and data don't match!" << std::endl;
        return;
    }
    for (size_t i = 0; i < period.size(); ++i) {
        internal_periodic_rot(direction[i], period[i]);
    }
}

void XGO::internal_periodic_tran(char direction, float period) {
    std::vector<char> dirs = {'x', 'y', 'z'};
    int index = search(direction, dirs);
    if (index == -1) {
        std::cerr << "ERROR! Direction must be 'x', 'y' or 'z'" << std::endl;
        return;
    }
    
    if (period == 0) {
        XGOorder["PERIODIC_TRAN"][index] = 0;
    } else {
        XGOorder["PERIODIC_TRAN"][index] = conver2u8(period, XGOparam["PERIOD_LIMIT"], 1);
    }
    send_command("PERIODIC_TRAN", index);
}

void XGO::periodic_tran(char direction, float period) {
    internal_periodic_tran(direction, period);
}

void XGO::periodic_tran(const std::vector<char>& direction, const std::vector<float>& period) {
    if (direction.size() != period.size()) {
        std::cerr << "ERROR! The length of direction and data don't match!" << std::endl;
        return;
    }
    for (size_t i = 0; i < period.size(); ++i) {
        internal_periodic_tran(direction[i], period[i]);
    }
}

// 踏步控制实现
void XGO::mark_time(float data) {
    if (data == 0) {
        XGOorder["MarkTime"][1] = 0;
    } else {
        XGOorder["MarkTime"][1] = conver2u8(data, XGOparam["MARK_TIME_LIMIT"], 1);
    }
    send_command("MarkTime");
}

void XGO::pace(const std::string& mode) {
    uint8_t value;
    if (mode == "normal") {
        value = 0x00;
    } else if (mode == "slow") {
        value = 0x01;
    } else if (mode == "high") {
        value = 0x02;
    } else {
        std::cerr << "ERROR! Illegal Value!" << std::endl;
        return;
    }
    XGOorder["MOVE_MODE"][1] = value;
    send_command("MOVE_MODE");
}

void XGO::gait_type(const std::string& mode) {
    uint8_t value;
    if (mode == "trot") {
        value = 0x00;
    } else if (mode == "walk") {
        value = 0x01;
    } else if (mode == "high_walk") {
        value = 0x02;
    } else if (mode == "slow_trot") {
        value = 0x03;
    } else {
        std::cerr << "ERROR! Illegal Value!" << std::endl;
        return;
    }
    XGOorder["GAIT_TYPE"][1] = value;
    send_command("GAIT_TYPE");
}

// 系统控制实现
void XGO::imu(int mode) {
    if (mode != 0 && mode != 1) {
        std::cerr << "ERROR! Illegal Value!" << std::endl;
        return;
    }
    XGOorder["IMU"][1] = mode;
    send_command("IMU");
}

void XGO::perform(int mode) {
    if (mode != 0 && mode != 1) {
        std::cerr << "ERROR! Illegal Value!" << std::endl;
        return;
    }
    XGOorder["PERFORM"][1] = mode;
    send_command("PERFORM");
}

void XGO::calibration(const std::string& state) {
    if (state == "start" || state == "1") {
        XGOorder["CALIBRATION"][1] = 1;
    } else if (state == "end" || state == "0") {
        XGOorder["CALIBRATION"][1] = 0;
    } else {
        std::cerr << "ERROR!" << std::endl;
        return;
    }
    send_command("CALIBRATION");
}

void XGO::set_origin() {
    XGOorder["SET_ORIGIN"][1] = 1;
    send_command("SET_ORIGIN");
}

// 机械臂控制实现
void XGO::arm(float arm_x, float arm_z) {
    std::vector<float> x_limit = {XGOparam["ARM_LIMIT"][0], XGOparam["ARM_LIMIT"][1]};
    std::vector<float> z_limit = {XGOparam["ARM_LIMIT"][2], XGOparam["ARM_LIMIT"][3]};
    
    uint8_t arm_x_u8 = conver2u8(arm_x, x_limit);
    uint8_t arm_z_u8 = conver2u8(arm_z, z_limit);
    
    XGOorder["ARM_X"][1] = arm_x_u8;
    XGOorder["ARM_Z"][1] = arm_z_u8;
    send_command("ARM_X");
    send_command("ARM_Z");
}

void XGO::arm_polar(float arm_theta, float arm_r) {
    std::vector<float> theta_limit = {XGOparam["ARM_LIMIT"][4], XGOparam["ARM_LIMIT"][5]};
    std::vector<float> r_limit = {XGOparam["ARM_LIMIT"][6], XGOparam["ARM_LIMIT"][7]};
    
    uint8_t arm_theta_u8 = conver2u8(arm_theta, theta_limit);
    uint8_t arm_r_u8 = conver2u8(arm_r, r_limit);
    
    XGOorder["ARM_THETA"][1] = arm_theta_u8;
    XGOorder["ARM_R"][1] = arm_r_u8;
    send_command("ARM_THETA");
    send_command("ARM_R");
}

void XGO::arm_mode(int mode) {
    if (mode != 0x01 && mode != 0x00) {
        std::cerr << "Error! Illegal Value!" << std::endl;
        return;
    }
    XGOorder["ARM_MODE"][1] = mode;
    send_command("ARM_MODE");
}

void XGO::claw(float pos) {
    std::vector<float> limit = {0, 255};
    uint8_t claw_pos = conver2u8(pos, limit);
    XGOorder["CLAW"][1] = claw_pos;
    send_command("CLAW");
}

void XGO::arm_speed(int speed) {
    if (speed < 0 || speed > 255) {
        std::cerr << "ERROR! Illegal Value! The speed parameter needs to be between 0 and 255!" << std::endl;
        return;
    }
    if (speed == 0) speed = 1;
    XGOorder["ARM_SPEED"][1] = speed;
    send_command("ARM_SPEED");
}

void XGO::moveToMid() {
    send_command("MOVE_TO_MID");
}

// 示教功能实现
void XGO::teach(const std::string& mode, int pos_id) {
    if (mode == "play") {
        XGOorder["TEACH_PLAY"][1] = pos_id;
        send_command("TEACH_PLAY");
    } else if (mode == "record") {
        XGOorder["TEACH_RECORD"][1] = pos_id;
        send_command("TEACH_RECORD");
    }
}

void XGO::teach_arm(const std::string& mode, int pos_id) {
    if (mode == "play") {
        XGOorder["TEACH_ARM_PLAY"][1] = pos_id;
        send_command("TEACH_ARM_PLAY");
    } else if (mode == "record") {
        XGOorder["TEACH_ARM_RECORD"][1] = pos_id;
        send_command("TEACH_ARM_RECORD");
    }
}

// 数据读取实现
std::vector<float> XGO::read_motor() {
    read_command(XGOorder["MOTOR_ANGLE"][0], 15);
    std::vector<float> angle;
    if (unpack_data()) {
        for (int i = 0; i <= rx_COUNT; ++i) {
            // 简化的角度转换，实际实现需要根据电机类型进行不同的转换
            angle.push_back(conver2float(rx_data[i], 90.0f));
        }
    }
    return angle;
}

int XGO::read_battery() {
    read_command(XGOorder["BATTERY"][0], 1);
    int battery = 0;
    if (unpack_data()) {
        battery = static_cast<int>(rx_data[0]);
    }
    return battery;
}

std::string XGO::read_firmware() {
    read_command(XGOorder["FIRMWARE_VERSION"][0], 10);
    std::string firmware_version = "Null";
    if (unpack_data()) {
        std::string temp(reinterpret_cast<char*>(rx_data.data()), 10);
        // 移除空字符
        temp.erase(std::find(temp.begin(), temp.end(), '\0'), temp.end());
        firmware_version = temp;
    }
    return firmware_version;
}

float XGO::read_roll() {
    read_command(XGOorder["ROLL"][0], 4);
    float roll = 0;
    if (unpack_data()) {
        std::vector<uint8_t> data(rx_data.begin(), rx_data.begin() + 4);
        roll = byte2float(data);
    }
    return std::round(roll * 100.0f) / 100.0f;
}

float XGO::read_pitch() {
    read_command(XGOorder["PITCH"][0], 4);
    float pitch = 0;
    if (unpack_data()) {
        std::vector<uint8_t> data(rx_data.begin(), rx_data.begin() + 4);
        pitch = byte2float(data);
    }
    return std::round(pitch * 100.0f) / 100.0f;
}

float XGO::read_yaw() {
    read_command(XGOorder["YAW"][0], 4);
    float yaw = 0;
    if (unpack_data()) {
        std::vector<uint8_t> data(rx_data.begin(), rx_data.begin() + 4);
        yaw = byte2float(data);
    }
    return std::round(yaw * 100.0f) / 100.0f;
}

int XGO::read_analog() {
    read_command(XGOorder["ANALOG_READ"][0], 1);
    int data = 0;
    if (unpack_data()) {
        data = static_cast<int>(rx_data[0]);
    }
    return data;
}

int XGO::read_digital() {
    read_command(XGOorder["DIGITIAL_READ"][0], 1);
    int data = 0;
    if (unpack_data()) {
        data = static_cast<int>(rx_data[0]);
    }
    return data;
}

int XGO::read_rotate_state() {
    read_command(XGOorder["MOVE_TO"][0], 1);
    int state = 0;
    if (unpack_data()) {
        state = static_cast<int>(rx_data[0]);
    }
    return state;
}

std::vector<float> XGO::read_imu() {
    read_command(0x65, 24);
    std::vector<float> result;
    if (unpack_data()) {
        if (!version.empty() && version[0] == 'R') {
            result = unpack_imu_r();
        } else {
            result = unpack_imu();
        }
    }
    return result;
}

int XGO::read_imu_int16(const std::string& direction) {
    if (direction == "roll") {
        read_command(0x66, 2);
    } else if (direction == "pitch") {
        read_command(0x67, 2);
    } else if (direction == "yaw") {
        read_command(0x68, 2);
    } else {
        return 0;
    }
    
    int result = 0;
    if (unpack_data()) {
        std::vector<uint8_t> data(rx_data.begin(), rx_data.begin() + 2);
        result = byte2short(data);
    }
    return result;
}

std::vector<float> XGO::unpack_imu() {
    std::vector<float> result;
    for (int i = 0; i < 9; ++i) {
        if (i < 6) {
            uint16_t temp = (rx_data[2 * i + 1] << 8) | rx_data[2 * i];
            int16_t signed_temp = static_cast<int16_t>(temp);
            if (i < 3) {
                result.push_back(signed_temp / 16384.0f * 9.8f);
            } else {
                result.push_back(signed_temp / 16.4f);
            }
        } else {
            std::vector<uint8_t> data = {rx_data[4 * i - 12], rx_data[4 * i - 11], rx_data[4 * i - 10], rx_data[4 * i - 9]};
            float temp = byte2float(data);
            result.push_back(temp / 180.0f * 3.14159f);
        }
    }
    return result;
}

std::vector<float> XGO::unpack_imu_r() {
    std::vector<float> result;
    for (int i = 0; i < 6; ++i) {
        std::vector<uint8_t> data = {rx_data[4 * i], rx_data[4 * i + 1], rx_data[4 * i + 2], rx_data[4 * i + 3]};
        result.push_back(byte2float(data));
    }
    return result;
}

// 其他功能实现
void XGO::bt_rename(const std::string& name) {
    if (name.length() > 10) {
        std::cerr << "ERROR! The length of the input string cannot be longer than 10!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < name.length(); ++i) {
        XGOorder["BT_NAME"][i + 1] = static_cast<uint8_t>(name[i]);
    }
    send_command("BT_NAME", 1, static_cast<int>(name.length()));
}

void XGO::move_to(int data, bool wait, float overtime) {
    uint16_t packed_data = static_cast<uint16_t>(data);
    XGOorder["MOVE_TO"][1] = (packed_data >> 8) & 0xFF;
    XGOorder["MOVE_TO"][2] = packed_data & 0xFF;
    send_command("MOVE_TO", 1, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if (wait) {
        auto start_time = std::chrono::steady_clock::now();
        while (true) {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            
            if (elapsed >= overtime) {
                break;
            }
            
            int flag = read_rotate_state();
            if (flag) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void XGO::output_analog(int data) {
    XGOorder["OUTPUT_ANALOG"][1] = data;
    send_command("OUTPUT_ANALOG");
}

void XGO::output_digital(int data) {
    XGOorder["OUTPUT_DIGITAL"][1] = data;
    send_command("OUTPUT_DIGITAL");
}

void XGO::extern_motor(int position) {
    uint16_t pos = static_cast<uint16_t>(position);
    XGOorder["EX_MOTOR"][1] = (pos >> 8) & 0xFF;
    XGOorder["EX_MOTOR"][2] = pos & 0xFF;
    send_command("EX_MOTOR", 1, 2);
}

void XGO::set_move_mintime(float mintime) {
    this->mintime = mintime;
}

std::string XGO::read_lib_version() {
    return LIB_VERSION;
}