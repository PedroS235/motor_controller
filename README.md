# Motor Controller for Differential Drive Robot

## Description
This Motor Controller project is designed to work seamlessly with a differential drive robot [Perceptron Bot](https://github.com/PedroS235/perceptron_bot). It offers a comprehensive suite of features, including a motor driver implementation, a serial communication protocol (soon to be developed), a timer API for simplified timer creation, and a PID controller. Additionally, the timer and PID controller functionalities are designed to become external libraries for wider use.

The motor driver component assumes the use of an **L298N driver** to facilitate communication between the Arduino and the actual motors. It can operate in both open and closed-loop modes, with encoder feedback required for closed-loop control. An implementation of an Encoder class is provided for this purpose.

The Motor Controller manages two instances of the motor driver, calculating the robot's pose based on the distances traveled by the wheels. It also accepts velocity commands, specifically linear x and angular z velocities.

In the near future, a serial communication protocol will be developed, facilitating integration with the Robot Operating System (ROS). This protocol will enable a ROS node to send serial flags, such as "o 0.5 0.5" (flag left_vel right_vel), and perform other functions like retrieving odometry data.

This entire repo has been developed on YouTube, for the series [Building and Autonomous Robot From Scratch](https://youtube.com/playlist?list=PLk6lkttcBmn6jJXv34IQDu1FgJ8daLgI6&si=cC8ekMfqWyN_AXRU)

## Table of Contents
- [Motor Controller for Differential Drive Robot](#motor-controller-for-differential-drive-robot)
  - [Description](#description)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [Usage](#usage)
  - [Configuration](#configuration)
  - [Examples](#examples)
  - [Doxygen Documentation](#doxygen-documentation)
  - [Contributing](#contributing)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Getting Started
To get started with this Motor Controller project, follow these steps:

### Prerequisites
Before you begin, ensure you have the following:
- **Hardware**: Arduino, L298N driver, differential drive robot
- **Software**: PlatformIO

### Installation
1. Clone this repository to your local machine.
2. Configure your hardware connections as specified in the configuratin.hpp file, or change the configuration to match your connections.

## Usage
At this moment in time, you can use the main.cpp as an example how how to use the motor controller. Essentially, you need to call the method `run`
on every main loop iteration. To command the robot, you can then use `set_cmd(cmd_vel)` to make your robot/motors move.

In case you upload the current code to a board, the robot should be going on a circle.

## Configuration
You can configure various aspects of the Motor Controller by editing the `configuration.hpp` file located in the `./include` directory. This file allows you to specify pin configurations and PID controller settings, among other things.


## Doxygen Documentation
The project includes Doxygen documentation, which can be found in the `./doxygen/doxygen_generated/html` directory. To view the documentation, you can start a Python server as follows:

```bash
$ cd doxygen/generated_doxygen/html && python -m http.server
```
Then, open your web browser and navigate to http://localhost:8000 to access the Doxygen documentation.

# Contributing

Contributions are welcome. Help us make this Motor Controller even better!

# License

This project is licensed under the MIT License. Feel free to use, modify, and distribute it in accordance with the license terms.
