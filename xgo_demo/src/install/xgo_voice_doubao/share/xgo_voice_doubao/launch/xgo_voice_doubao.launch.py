from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='xgo_voice_doubao',
            executable='voice_node',
            output='screen'
        )
    ])