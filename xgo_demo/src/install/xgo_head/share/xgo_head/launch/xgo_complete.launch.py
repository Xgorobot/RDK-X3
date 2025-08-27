#!/usr/bin/env python3

import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    """
    Launch file for XGO system: start xgo_head (UI) and optional helpers.
    Keep feature nodes (e.g., xgo_motion) started by xgo_head via tmux as before,
    to avoid duplicate instances and keep the same UX.
    """
    
    cam_type_arg = DeclareLaunchArgument(
        'cam_type',
        default_value='mipi',
        description='Camera type (mipi/usb)'
    )

    # Optional: cleanup legacy tmux sessions once at startup
    tmux_cleanup = ExecuteProcess(
        cmd=['tmux', 'kill-server'],
        name='tmux_cleanup',
        output='screen'
    )

    # Battery monitoring node (optional, keep if your system has it)
    battery_node = Node(
        package='xgo_battery',
        executable='battery_publisher',
        name='battery_publisher',
        output='screen',
        parameters=[{
            'publish_frequency': 1.0,
        }]
    )

    # Main XGO head control node (UI)
    xgo_head_node = Node(
        package='xgo_head',
        executable='xgo_head',
        name='xgo_head_node',
        output='screen',
        parameters=[{
            'use_sim_time': False,
        }],
        # Set environment variables correctly
        env={'CAM_TYPE': LaunchConfiguration('cam_type')}
    )

    return LaunchDescription([
        cam_type_arg,
        tmux_cleanup,
        battery_node,
        xgo_head_node,
    ])