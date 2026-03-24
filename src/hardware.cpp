#include "semubot_driver/hardware.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

namespace semubot
{

Hardware::Hardware(std::shared_ptr<rclcpp::Node> node) : node_(node)
{
    port_      = node_->declare_parameter<std::string>("serial_port", "/dev/ttyACM0");
    baud_rate_ = node_->declare_parameter<int>("baud_rate", 115200);
}

Hardware::~Hardware()
{
    disconnect();
}

bool Hardware::connect()
{
    fd_ = open(port_.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd_ < 0) {
        RCLCPP_ERROR(node_->get_logger(), "Failed to open %s", port_.c_str());
        return false;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    tcgetattr(fd_, &tty);

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    tty.c_cflag = CS8 | CREAD | CLOCAL;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    tcflush(fd_, TCIFLUSH);
    tcsetattr(fd_, TCSANOW, &tty);

    RCLCPP_INFO(node_->get_logger(), "Opened %s at 115200", port_.c_str());
    return true;
}

void Hardware::disconnect()
{
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}

bool Hardware::is_connected()
{
    return fd_ >= 0;
}

void Hardware::send_cmd(float vx, float vy, float omega)
{
    if (fd_ < 0) return;
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "CMD:%.4f,%.4f,%.4f\n", vx, vy, omega);
    write(fd_, buf, len);
}

bool Hardware::get_state(StatePacket &state)
{
    if (fd_ < 0) return false;

    char c;
    while (read(fd_, &c, 1) == 1) {
        if (c == '\n') {
            if (line_buf_.rfind("STATE:", 0) == 0) {
                int parsed = sscanf(line_buf_.c_str() + 6,
                    "%f,%f,%f,%f,%f,%f",
                    &state.deg[0], &state.deg[1], &state.deg[2],
                    &state.vel[0], &state.vel[1], &state.vel[2]);
                line_buf_.clear();
                return parsed == 6;
            }
            line_buf_.clear();
        } else {
            line_buf_ += c;
        }
    }
    return false;
}


void Hardware::send_cmd_motors(float v1, float v2, float v3)
{
    if (fd_ < 0) {
        RCLCPP_WARN(node_->get_logger(), "Serial not open!");
        return;
    }
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "CMD:%.4f,%.4f,%.4f\n", v1, v2, v3);
    int written = write(fd_, buf, len);
    RCLCPP_INFO(node_->get_logger(), "Sent: %s (%d bytes)", buf, written);
}


}  // namespace semubot