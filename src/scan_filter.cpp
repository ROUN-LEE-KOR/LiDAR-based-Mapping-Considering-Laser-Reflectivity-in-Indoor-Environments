#include "scan_filter.hpp"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

double dzero=0.0;
double d = 0/dzero;
string frame_id = "base_link";

ScanFilter::ScanFilter() 
: Node("scan_filter"), qos(rclcpp::QoS(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_default))), 
qos_lidar(rclcpp::QoS(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_default)))
{  
    qos.keep_last(10).best_effort().durability_volatile();
    qos_lidar.keep_last(5).best_effort().durability_volatile();
    initializeClass();
}

ScanFilter::~ScanFilter()
{

}

void ScanFilter::initializeClass()
{
    callbackGroupLiDAR = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    rclcpp::SubscriptionOptions options;
    options.callback_group = callbackGroupLiDAR;

    scan_sub = this->create_subscription<sensor_msgs::msg::LaserScan>("/scans", qos_lidar, 
        std::bind(&ScanFilter::ScanMsgCallback, this, std::placeholders::_1), options);

    scan_filter_pub = this->create_publisher<sensor_msgs::msg::LaserScan>("/filtered_scan", qos);

    memset(&parameter_scan, 0.0, sizeof(parameter_scan));
    RCLCPP_INFO(this->get_logger(), "ScanFilter Node Initialized");
}

void ScanFilter::ScanMsgCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    if(msg->ranges.size() != 0)
    {
        parameter_scan.ranges.clear();
        parameter_scan.intensities.clear();
        // cout << msg->ranges.size() << "subsize" << endl;
        for(unsigned int i=0; i < msg->ranges.size(); ++i)
        {
            //parameter_scan.time = msg.header.stamp;
            parameter_scan.ranges.push_back(msg->ranges[i]);
            parameter_scan.intensities.push_back(msg->intensities[i]);
        }
    }
}

void ScanFilter::filtering()
{
    //cout << parameter_scan.intensities.size() << "sizes" << endl;
    //if(parameter_scan.ranges.size() != 0 && parameter_scan.intensities.size() !=0)
    //{
        for(int j=3; j<parameter_scan.ranges.size(); ++j)
        {
            int criteria = isinf(parameter_scan.ranges[j]);
            //cout << "zzzz" << endl;
            if(criteria == 0)
            {
                if(parameter_scan.intensities[j] < 21 && parameter_scan.intensities[j] != 500) 
                {
                    parameter_scan.intensities[j-2] = 500;
                    parameter_scan.intensities[j-1] = 500;
                    parameter_scan.intensities[j] = 500;
                    parameter_scan.intensities[j+1] = 500;
                    parameter_scan.intensities[j+2] = 500;
                    parameter_scan.ranges[j-2] = d;
                    parameter_scan.ranges[j-1] = d;
                    parameter_scan.ranges[j] = d;
                    parameter_scan.ranges[j+1] = d;
                    parameter_scan.ranges[j+2] = d;
                }
            }
            // else
            // {
            //     parameter_scan.ranges_filtered.push_back(parameter_scan.ranges[j]);
            //     parameter_scan.intensities_filtered.push_back(parameter_scan.intensities[j]);
            // }
        //memset(&parameter_scan, 0.0, sizeof(parameter_scan));
        }
    //}

}

void ScanFilter::ParameterParser()
{
    // if(parameter_scan.ranges.size() != 0 && parameter_scan.intensities.size() !=0)
    // {
        auto scan = std::make_shared<sensor_msgs::msg::LaserScan>();
        scan->ranges = parameter_scan.ranges;
        scan->intensities = parameter_scan.intensities;
        scan->range_min = 0.0;
        scan->range_max = 200.0;
        scan->time_increment = 0.0;
        scan->angle_increment = 0.006;
        scan->angle_max = M_PI;
        scan->angle_min = -M_PI;
        scan->header.stamp = this->now();
        scan->header.frame_id = frame_id;
        // cout << parameter_scan.ranges.size() << "total size0" << endl;
        // cout << scan->ranges.size() << "total size" << endl;
        scan_filter_pub->publish(*scan);
        parameter_scan.ranges.clear();
        parameter_scan.intensities.clear();
        // scan->ranges.clear();
        // scan->intensities.clear();
    // }
}

void ScanFilter::run()
{ // control freq.: 20Hz
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "\033[1;32m----> Scan filter Started.\033[0m");
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(true);
    rclcpp::executors::SingleThreadedExecutor exec;

    while(rclcpp::ok()) 
    {
        rclcpp::Rate loop_rate(10);
        filtering();
        ParameterParser();
        exec.spin_once();
        loop_rate.sleep();
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ScanFilter>();
    node->run();
    rclcpp::shutdown();
    return 0;
}
