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
    msg.name = {"omni_ball_1_joint",
        "omni_ball_2_joint",
        "omni_ball_3_joint"};

    msg.position = {
        static_cast<double>(state.deg[0]), // Motor 1
        static_cast<double>(state.deg[1]), // Motor 2
        static_cast<double>(state.deg[2])  // Motor 3
    };
    msg.velocity = {
        static_cast<double>(state.vel[0]), // M1
        static_cast<double>(state.vel[1]), // M2
        static_cast<double>(state.vel[2])  // M3
    };
    msg.effort = {0.0, 0.0, 0.0};

    joint_state_pub_->publish(msg);
}

}  // namespace semubot