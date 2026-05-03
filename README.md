# semubot_driver

Low-level driver package for communicating with the Semubot STM32 wheelbase firmware.

This package is used for direct serial-based testing and for onboard-control variants where the STM32 receives robot velocity commands and handles motor control internally.

## Purpose

`semubot_driver` provides the host-side interface for the STM32 firmware.

Typical responsibilities:

    send velocity commands to STM32
    read wheelbase state from STM32
    support direct serial testing
    provide tools for open-loop and PID firmware experiments

## Command convention

Robot velocity command:

    vx = forward/backward
    vy = left/right
    wz = rotation/yaw

For serial firmware variants, commands are sent as:

    CMD:vx,vy,wz

Example:

    CMD:0.20,0.00,0.00

This means:

    vx = 0.20
    vy = 0.00
    wz = 0.00

## Serial protocol

### Command from host to STM32

    CMD:vx,vy,wz

Example commands:

    CMD:0.20,0.00,0.00
    CMD:-0.20,0.00,0.00
    CMD:0.00,0.20,0.00
    CMD:0.00,-0.20,0.00
    CMD:0.00,0.00,0.20

### State from STM32 to host

    STATE:p1,p2,p3,v1,v2,v3

Meaning:

    p1, p2, p3 = wheel positions
    v1, v2, v3 = wheel velocities

Expected motor order:

    1 = M1 = omni_ball_1_joint
    2 = M2 = omni_ball_2_joint
    3 = M3 = omni_ball_3_joint

## Supported firmware variants

### onboard-serial-openloop

The host sends:

    CMD:vx,vy,wz

The STM32 does:

    parse command
    apply onboard open-loop mixer
    generate PWM duty
    drive motors

PID is not used in this variant.

### onboard-serial-pid

The host sends:

    CMD:vx,vy,wz

The STM32 does:

    parse command
    compute wheel velocity targets
    read encoders
    run onboard PID
    generate PWM duty
    drive motors

PID is done on the STM32.

### ros2ctrl-serial

In the ros2_control serial variant, the host-side ros2_control hardware interface sends low-level motor commands to the STM32.

In that architecture:

    /cmd_vel
    -> ros2_control controller
    -> serial hardware interface
    -> STM32
    -> PWM duty

PID may be done on the ROS side depending on the controller implementation.


## Direct serial testing

Example using a serial terminal:

    CMD:0.10,0.00,0.00

Expected response format:

    STATE:p1,p2,p3,v1,v2,v3

Stop command:

    CMD:0.00,0.00,0.00

## Direction tests

Forward:

    CMD:0.20,0.00,0.00

Backward:

    CMD:-0.20,0.00,0.00

Left:

    CMD:0.00,0.20,0.00

Right:

    CMD:0.00,-0.20,0.00

Rotate:

    CMD:0.00,0.00,0.20

## Relationship to semubot_ros_control

`semubot_driver` is for low-level serial communication and direct firmware testing.

`semubot_ros_control` is for ros2_control-based control using a hardware interface and controller.

Use `semubot_driver` when testing:

    onboard-serial-openloop
    onboard-serial-pid
    direct serial firmware behavior

Use `semubot_ros_control` when testing:

    ros2ctrl-serial
    ros2ctrl-microros
    ROS-side PID



## ros2_control mode

`semubot_driver` can be run in `ros2_control` mode:

    ros2 run semubot_driver driver_node --ros-args -p mode:=ros2_control

With serial settings:

    ros2 run semubot_driver driver_node --ros-args -p mode:=ros2_control -p serial_port:=/dev/ttyACM0 -p baud_rate:=115200

In this mode, the driver is used as the serial communication backend for the ros2_control stack.

The command path is:

    /cmd_vel
    -> ros2_control controller
    -> semubot_ros_control hardware interface
    -> semubot_driver
    -> STM32

This is different from onboard serial control.

In onboard serial control, the driver or host sends robot velocity commands directly to the STM32:

    CMD:vx,vy,wz

and the STM32 performs the motion mixing or PID onboard.

In `ros2_control` mode, the ROS 2 controller handles the higher-level control. The driver’s job is mainly to communicate with the STM32, forward commands, and read state feedback.

Use `mode:=ros2_control` when testing:

    ros2ctrl-serial
    ROS-side PID
    ros2_control hardware interface integration

Do not use `mode:=ros2_control` for standalone onboard PID testing, where the STM32 expects direct robot velocity commands.