#pragma once

#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "std_msgs/msg/int16_multi_array.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

using namespace std;

const int num_reading = 1046; // for velodyne vlp 16

struct parameter
{
    //time time;
    vector<float> ranges;
    vector<float> intensities;
    vector<float> ranges_filtered;
    vector<float> intensities_filtered;
};

class ScanFilter : public rclcpp::Node
{

public:
  ScanFilter();
  ~ScanFilter();

public:
    struct parameter parameter_scan;
    void filtering();
    void ParameterParser();
    void ScanMsgCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_filter_pub;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub;
    rclcpp::CallbackGroup::SharedPtr callbackGroupLiDAR;
    void initializeClass();
    void run();

    rclcpp::QoS qos;
    rclcpp::QoS qos_lidar;
};

// rmw_qos_profile_t qos_profile{
//     RMW_QOS_POLICY_HISTORY_KEEP_LAST,
//     1,
//     RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
//     RMW_QOS_POLICY_DURABILITY_VOLATILE,
//     RMW_QOS_DEADLINE_DEFAULT,
//     RMW_QOS_LIFESPAN_DEFAULT,
//     RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
//     RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
//     false
// };

// auto qos = rclcpp::QoS(
//     rclcpp::QoSInitialization(
//         qos_profile.history,
//         qos_profile.depth
//     ),
//     qos_profile);


// rmw_qos_profile_t qos_profile_lidar{
//     RMW_QOS_POLICY_HISTORY_KEEP_LAST,
//     5,
//     RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
//     RMW_QOS_POLICY_DURABILITY_VOLATILE,
//     RMW_QOS_DEADLINE_DEFAULT,
//     RMW_QOS_LIFESPAN_DEFAULT,
//     RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
//     RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
//     false
// };

// auto qos_lidar = rclcpp::QoS(
//     rclcpp::QoSInitialization(
//         qos_profile_lidar.history,
//         qos_profile_lidar.depth
//     ),
//     qos_profile_lidar);


