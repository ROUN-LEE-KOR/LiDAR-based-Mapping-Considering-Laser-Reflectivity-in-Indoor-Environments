#!/usr/bin/env python3

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node

def generate_launch_description():

    share_dir = get_package_share_directory('scan_filter')
    rviz_config_file = os.path.join(share_dir, 'launch', 'rviz2.rviz')

    return LaunchDescription([
        Node(
            package='scan_filter',
            executable='scan_filter_node',
            name='scan_filter_node',
            output='screen'
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config_file],
            output='screen'
        )
    ])
