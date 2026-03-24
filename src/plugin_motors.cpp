#include "semubot_driver/plugin_motors.hpp"

namespace semubot
{

PluginMotors::PluginMotors(std::shared_ptr<Hardware> hw,
                           std::shared_ptr<rclcpp::Node> node)
    : hw_(hw), node_(node)
{
    mode_ = node_->declare_parameter<std::string>("mode", "teleop");

    if (mode_ == "ros2_control") {
        cmd_ros2ctrl_sub_ = node_->create_subscription<std_msgs::msg::Float32MultiArray>(
            "/hardware_interface/velocity_cmd", 10,
            std::bind(&PluginMotors::cmd_ros2control_callback, this, std::placeholders::_1));
        RCLCPP_INFO(node_->get_logger(), "PluginMotors initialized in ros2_control mode");
    } else {
        cmd_vel_sub_ = node_->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel", 10,
            std::bind(&PluginMotors::cmd_vel_callback, this, std::placeholders::_1));
        RCLCPP_INFO(node_->get_logger(), "PluginMotors initialized in teleop mode");
    }
}

void PluginMotors::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    hw_->send_cmd(
        static_cast<float>(msg->linear.x),
        static_cast<float>(msg->linear.y),
        static_cast<float>(msg->angular.z));
}

void PluginMotors::cmd_ros2control_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
{
    RCLCPP_INFO(node_->get_logger(), "CMD received: %.2f %.2f %.2f",
        msg->data[0], msg->data[1], msg->data[2]);
    if (msg->data.size() < 3) return;
    hw_->send_cmd_motors(
        msg->data[0],
        msg->data[1],
        msg->data[2]);
}

}  // namespace semubot