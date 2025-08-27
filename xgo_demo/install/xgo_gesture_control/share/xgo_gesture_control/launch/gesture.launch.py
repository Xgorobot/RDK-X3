from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='xgo_gesture_control',
            executable='gesture_node',
            name='xgo_gesture_node',
            output='screen',
            parameters=[{
                'port': '/dev/ttyS3',
                'baud': 115200,
                'version': 'xgolite',
                'verbose': False,
                'fast_startup': False,
                'cooldown': 2.0,
            }]
        )
    ])