#pragma once

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include "semubot_driver/hardware.hpp"

namespace semubot
{

class PluginMotors
{
public:
    PluginMotors(std::shared_ptr<Hardware> hw,
                 std::shared_ptr<rclcpp::Node> node);

private:
    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
    void cmd_ros2control_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg);

    std::shared_ptr<Hardware> hw_;
    std::shared_ptr<rclcpp::Node> node_;

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr cmd_ros2ctrl_sub_;

    std::string mode_;
};

}  // namespace semubot