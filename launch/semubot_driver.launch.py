from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    parameters_file = PathJoinSubstitution([
        FindPackageShare('semubot_driver'), 'config', 'parameters.yaml'
    ])

    joy_config_file = PathJoinSubstitution([
        FindPackageShare('semubot_driver'), 'config', 'joy_config.yaml'
    ])

    return LaunchDescription([
        DeclareLaunchArgument(
            'serial_port',
            default_value='/dev/ttyACM0',
            description='Serial port for STM32 CDC'
        ),
        DeclareLaunchArgument(
            'baud_rate',
            default_value='115200',
            description='Baud rate'
        ),

        # Serial driver node
        Node(
            package='semubot_driver',
            executable='driver_node',
            name='driver',
            parameters=[
                parameters_file,
                {
                    'serial_port': LaunchConfiguration('serial_port'),
                    'baud_rate':   LaunchConfiguration('baud_rate'),
                }
            ],
            output='screen',
        ),
    ])