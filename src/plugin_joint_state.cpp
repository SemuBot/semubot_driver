#include "semubot_driver/plugin_joint_state.hpp"

namespace semubot
{

PluginJointState::PluginJointState(std::shared_ptr<rclcpp::Node> node)
    : node_(node)
{
    joint_state_pub_ = node_->create_publisher<sensor_msgs::msg::JointState>(
        "/motor_states", 10);

    RCLCPP_INFO(node_->get_logger(), "PluginJointState initialized");
}

void PluginJointState::on_state_received(const StatePacket &state)
{
    auto msg = sensor_msgs::msg::JointState();
    msg.header.stamp = node_->get_clock()->now();
    msg.name = {"motor1_joint", "motor2_joint", "motor3_joint"};

    msg.position = {
        static_cast<double>(state.deg[0]),
        static_cast<double>(state.deg[1]),
        static_cast<double>(state.deg[2])
    };
    msg.velocity = {
        static_cast<double>(state.vel[0]),
        static_cast<double>(state.vel[1]),
        static_cast<double>(state.vel[2])
    };
    msg.effort = {0.0, 0.0, 0.0};

    joint_state_pub_->publish(msg);
}

}  // namespace semubot