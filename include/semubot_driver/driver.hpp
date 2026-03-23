#pragma once

#include <rclcpp/rclcpp.hpp>
#include "semubot_driver/hardware.hpp"
#include "semubot_driver/plugin_motors.hpp"
#include "semubot_driver/plugin_joint_state.hpp"

namespace semubot
{

class Driver : public rclcpp::Node
{
public:
    Driver();
    void initialize();
    ~Driver();

private:
    void update();

    std::shared_ptr<Hardware> hw_;
    std::shared_ptr<PluginMotors> motors_;
    std::shared_ptr<PluginJointState> joint_state_;

    rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace semubot