#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "semubot_driver/hardware.hpp"

namespace semubot
{

class PluginJointState
{
public:
    explicit PluginJointState(std::shared_ptr<rclcpp::Node> node);
    void on_state_received(const StatePacket &state);

private:
    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;
};

}  // namespace semubot