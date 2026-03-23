#include "semubot_driver/plugin_motors.hpp"

namespace semubot
{

PluginMotors::PluginMotors(std::shared_ptr<Hardware> hw,
                           std::shared_ptr<rclcpp::Node> node)
    : hw_(hw), node_(node)
{
    cmd_vel_sub_ = node_->create_subscription<geometry_msgs::msg::Twist>(
        "/cmd_vel", 10,
        std::bind(&PluginMotors::cmd_vel_callback, this, std::placeholders::_1));

    RCLCPP_INFO(node_->get_logger(), "PluginMotors initialized");
}

void PluginMotors::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    hw_->send_cmd(
        static_cast<float>(msg->linear.x),
        static_cast<float>(msg->linear.y),
        static_cast<float>(msg->angular.z));
}

}  // namespace semubot