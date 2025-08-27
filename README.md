# XGO机器人控制系统

一个基于ROS2的XGO四足机器人综合控制系统，集成了运动控制、视觉识别、语音交互、手势控制等多种功能模块。

## 目录

- [项目简介](#项目简介)
- [文件结构](#文件结构)
- [安装说明](#安装说明)
- [使用方法](#使用方法)
- [更新日志](#更新日志)
- [许可证](#许可证)

## 项目简介

本项目是一个完整的XGO四足机器人控制系统，基于ROS2 Humble开发。系统提供了丰富的功能模块，包括：

## 文件结构

```
/RDK-X3
├── config/                     # 全局配置文件目录
│   ├── audio_config.json       # 音频配置
│   ├── *.hbm                   # AI模型文件
│   ├── *workconfig.json        # 各种AI模型工作配置
│   ├── *.list                  # 类别标签文件
│   └── *.jpg                   # 测试图片
├── serial/                     # 串口通信库
│   └── serial/
│       ├── CMakeLists.txt
│       ├── package.xml
│       ├── src/                # 源代码
│       ├── include/            # 头文件
│       └── README.md
└── xgo_demo/                   # XGO机器人主要功能包
    ├── config/                 # 配置文件（与全局config相同）
    ├── src/                    # ROS2功能包源码
    │   ├── xgo_device/         # 设备控制包
    │   ├── xgo_motion/         # 运动控制包
    │   ├── xgo_face_control/   # 人脸控制包
    │   ├── xgo_gesture_control/# 手势控制包
    │   ├── xgo_head/           # 头部控制包
    │   ├── xgo_voice_control/  # 语音控制包
    │   ├── xgo_body_tracker/   # 身体跟踪包
    │   ├── xgolib/             # XGO核心库
    │   └── xgoscreen/          # 屏幕显示包
    ├── build/                  # 编译输出目录
    ├── install/                # 安装目录
    ├── log/                    # 日志文件
    └── *.workspace             # 工作空间配置
```


## 安装说明


### 1. 环境准备

```bash
# 安装ROS2 Humble
sudo apt update
sudo apt install ros-humble-desktop

# 安装依赖
sudo apt install cmake build-essential
sudo apt install qtbase5-dev
sudo apt install python3-colcon-common-extensions
```

### 2. 克隆项目

```bash
git clone https://github.com/Xgorobot/RDK-X3
cd RDK-X3
```

### 3. 编译串口库

```bash
cd serial/serial
mkdir build && cd build
cmake ..
make
sudo make install  # 可选，也可以直接与ROS2包一起编译
```

### 4. 编译ROS2包

```bash
cd xgo_demo
source /opt/tros/humble/setup.bash
source /opt/ros/humble/setup.bash
colcon build
```

### 5. 设置环境

```bash
source install/setup.bash
```

## 使用方法

### 启动ROS2节点
例如：
```bash
# 启动设备管理节点
ros2 run xgo_device xgo_device_node

# 启动运动控制
ros2 run xgo_motion xgo_motion_node
```


### 使用Launch文件
例如：
```bash
# 启动完整系统（如果有launch文件）
ros2 launch xgo_your_project xgo_your_project.launch.py
```

## 更新日志

### v1.0.0 (2025-08-27)
- 初始版本发布
- 初始功能9个功能



## 许可证


### 报告问题
如果发现bug或有功能建议，请在GitHub Issues中报告。


**注意**: 本项目仍在积极开发中，API可能会发生变化。建议在生产环境使用前进行充分测试。

如有问题或建议，欢迎提交Issue或Pull Request！