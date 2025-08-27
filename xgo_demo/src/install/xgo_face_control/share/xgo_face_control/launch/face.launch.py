from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
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