from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
import os
from ament_index_python import get_package_share_directory

def generate_launch_description():
    return LaunchDescription([
        # 启动人脸关键点检测
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                get_package_share_directory('face_landmarks_detection'),
                '/launch/body_det_face_landmarks_det.launch.py'
            ])
        ),
        
        # 启动人脸跟踪控制
        Node(
            package='xgo_face_control',
            executable='face_node',
            name='xgo_face_node',
            output='screen',
            parameters=[{
                'port': '/dev/ttyS3',
                'baud': 115200,
                'version': 'xgolite',
                'verbose': False,
                'fast_startup': False,
                'kp_yaw': 0.11,
                'kp_pitch': 0.07,
                'limit_yaw': 10.0,
                'limit_pitch': 12.0,
            }]
        )
    ])