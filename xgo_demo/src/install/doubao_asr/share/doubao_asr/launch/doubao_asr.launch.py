from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='doubao_asr',
            executable='doubao_asr_node',
            name='doubao_asr_node',
            output='screen',
            parameters=[{
                'arecord_device': 'hw:0,1',
                'arecord_rate': 48000,
                'arecord_channels': 2,
                'arecord_duration': 5,
                'audio_file': 'test.wav',
                'enable_wake': True
            }]
        )
    ])