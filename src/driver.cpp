#include "semubot_driver/driver.hpp"

namespace semubot
{

Driver::Driver() : Node("driver_node")
{
}

void Driver::initialize()
{
    auto node_ptr = shared_from_this();

    hw_ = std::make_shared<Hardware>(node_ptr);

    if (!hw_->connect()) {
        RCLCPP_ERROR(get_logger(), "Failed to connect to hardware");
        throw std::runtime_error("Hardware connection failed");
    }

    motors_      = std::make_shared<PluginMotors>(hw_, node_ptr);
    joint_state_ = std::make_shared<PluginJointState>(node_ptr);

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(20),
        std::bind(&Driver::update, this));

    RCLCPP_INFO(get_logger(), "SemuBot driver initialized");
}

void Driver::update()
{
    StatePacket state;
    if (hw_->get_state(state)) {
        joint_state_->on_state_received(state);
    }
}

Driver::~Driver()
{
    hw_->disconnect();
}

}  // namespace semubot