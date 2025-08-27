from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='xgo_body_tracker',
            executable='body_tracker_node',
            name='xgo_body_tracker',
            output='screen',
            parameters=[
                {'port': '/dev/ttyS3'},
                {'baud': 115200},
                {'version': 'xgolite'},
                {'verbose': False},
                {'fast_startup': False},
                {'topic': '/hobot_mono2d_body_detection'},
                {'img_w': 960},
                {'img_h': 544},
                {'kp_x': 0.4},   # 左右平移控制比例
                {'kp_y': 0.35},  # 前后控制比例（基于脚部深度代理，使用脚间距/高度比）
                {'ki_x': 0.0},
                {'ki_y': 0.0},
                {'kd_x': 0.0},
                {'kd_y': 0.0},
                {'limit_x_cm': 6.0},
                {'limit_y_cm': 6.0},
                {'step_min_cm': 1.5},
                {'lost_timeout_ms': 800},
            ]
        )
    ])