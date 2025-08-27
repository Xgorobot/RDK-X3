from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='xgo_voice_control',
            executable='voice_control_node',
            name='xgo_voice_control',
            parameters=[{
                'port': '/dev/ttyS3',
                'baud': 115200,
                'version': 'xgolite',
                'verbose': False,
                'fast_startup': True,
            }],
            output='screen'
        )
    ])