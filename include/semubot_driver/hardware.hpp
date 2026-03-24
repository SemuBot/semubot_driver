#pragma once

#include <rclcpp/rclcpp.hpp>
#include <string>
#include <termios.h>

namespace semubot
{

struct StatePacket {
    float deg[3];
    float vel[3];
};

class Hardware
{
public:
    explicit Hardware(std::shared_ptr<rclcpp::Node> node);
    ~Hardware();

    bool connect();
    void disconnect();
    bool is_connected();
    void send_cmd(float vx, float vy, float omega);
    void send_cmd_motors(float v1, float v2, float v3);
    bool get_state(StatePacket &state);

private:
    std::shared_ptr<rclcpp::Node> node_;
    std::string port_;
    int baud_rate_;
    int fd_ = -1;

    std::string line_buf_;
};

}  // namespace semubot