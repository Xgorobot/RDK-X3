#!/usr/bin/env python3

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # 声明启动参数
    declare_scan_topic_arg = DeclareLaunchArgument(
        'scan_topic',
        default_value='scan',
        description='Name of the laser scan topic to subscribe to'
    )
    
    declare_debug_arg = DeclareLaunchArgument(
        'debug',
        default_value='false',
        description='Enable debug output'
    )
    
    # 获取启动参数
    scan_topic = LaunchConfiguration('scan_topic')
    debug = LaunchConfiguration('debug')
    
    # 创建lidar_display_node节点
    lidar_display_node = Node(
        package='xgo_lidar_display',
        executable='lidar_display_node',
        name='lidar_display_node',
        output='screen',
        parameters=[
            {'use_sim_time': False},
            {'debug': debug}
        ],
        remappings=[
            ('scan', scan_topic)
        ]
    )
    
    return LaunchDescription([
        declare_scan_topic_arg,
        declare_debug_arg,
        lidar_display_node
    ])