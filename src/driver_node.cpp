#include <rclcpp/rclcpp.hpp>
#include "semubot_driver/driver.hpp"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto driver_node = std::make_shared<semubot::Driver>();
    driver_node->initialize();
    rclcpp::spin(driver_node);
    rclcpp::shutdown();
    return 0;
}