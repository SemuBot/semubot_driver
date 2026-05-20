# semubot_driver

> Low-level serial driver for communicating with the SemuBot STM32 wheelbase firmware.

![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## Overview

`semubot_driver` provides the host-side interface to the STM32 firmware over serial. It handles sending velocity commands, reading wheelbase state, and serves as the serial backend for `ros2_control`-based stacks when needed.

Use this package for:
- Direct serial testing of firmware variants
- Onboard-control variants where the STM32 handles mixing or PID internally
- Serial backend for `ros2ctrl-serial`

For `ros2_control`-based stacks, see [`semubot_ros_control`](https://github.com/SemuBot/semubot_ros_control).

---

## Serial Protocol

### Host → STM32

```
CMD:vx,vy,wz
```

| Field | Meaning |
|---|---|
| `vx` | Forward / backward |
| `vy` | Left / right (strafe) |
| `wz` | Rotation / yaw |

**Examples**

```
CMD:0.20,0.00,0.00    # forward
CMD:-0.20,0.00,0.00   # backward
CMD:0.00,0.20,0.00    # left
CMD:0.00,-0.20,0.00   # right
CMD:0.00,0.00,0.20    # rotate
CMD:0.00,0.00,0.00    # stop
```

### STM32 → Host

```
STATE:p1,p2,p3,v1,v2,v3
```

| Field | Meaning |
|---|---|
| `p1, p2, p3` | Wheel positions |
| `v1, v2, v3` | Wheel velocities |

**Motor order**

| Index | Joint |
|---|---|
| M1 | `omni_ball_1_joint` |
| M2 | `omni_ball_2_joint` |
| M3 | `omni_ball_3_joint` |

---

## Firmware Variants

### `onboard-serial`

Host sends `CMD:vx,vy,wz`. STM32 applies the onboard open-loop mixer and drives motors directly. No PID.

### `onboard-serial-pid`

Host sends `CMD:vx,vy,wz`. STM32 computes wheel velocity targets, reads encoders, runs onboard PID, and drives motors. PID runs entirely on the STM32.

### `ros2ctrl-serial`

The `semubot_ros_control` hardware interface sends low-level motor commands over serial. `semubot_driver` acts as the communication backend. PID runs on the ROS 2 side.

```
/cmd_vel
    → ros2_control controller
    → SemuBotHardwareInterface
    → semubot_driver
    → STM32 → PWM
```

---

## Usage

### Direct serial testing

Open a serial terminal to the STM32 and send commands manually:

```bash
# Forward
CMD:0.10,0.00,0.00

# Expected response
STATE:p1,p2,p3,v1,v2,v3

# Stop
CMD:0.00,0.00,0.00
```

### ros2_control mode

Run the driver node as the serial backend for the `ros2_control` stack:

```bash
ros2 run semubot_driver driver_node --ros-args \
  -p mode:=ros2_control \
  -p serial_port:=/dev/ttyACM0 \
  -p baud_rate:=115200
```

In this mode the driver forwards low-level motor commands from the hardware interface to the STM32 and returns encoder state. The STM32 does not perform mixing or PID — that is handled by the ROS 2 controller.

> Do not use `mode:=ros2_control` for onboard PID testing. In that case the STM32 expects `CMD:vx,vy,wz` directly.

---

## Related Packages

| Package | Description |
|---|---|
| [`semubot_ros_control`](https://github.com/SemuBot/semubot_ros_control) | `ros2_control` hardware interface and velocity controller |
| [`semubot_bringup`](https://github.com/SemuBot/semubot_bringup) | Top-level launch files |
| [`SemuBot-Firmware`](https://github.com/SemuBot/SemuBot-Firmware) | STM32 firmware (FreeRTOS, micro-ROS) |

---

## **License**

This project is licensed under the Apache 2.0 license - see the [LICENSE](LICENSE) file for more information.