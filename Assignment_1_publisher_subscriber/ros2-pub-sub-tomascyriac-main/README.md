[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/4jgMCaSu)
# Assignment: ROS2 C++ Custom Pub/Sub Messages

**Course:** ROS2 Fundamentals  
**Build System:** ament_cmake

## Objective
The goal of this assignment is to verify your understanding of:
1. Creating a custom ROS2 message format (`.msg`).
2. Integrating custom messages within a ROS2 package.
3. Building custom messages using `rosidl` packages.
4. Implementing Publisher and Subscriber nodes that communicate using this custom message type.

## Problem Statement
You are tasked to build the core communication backbone for a smart factory robot monitor. 
You must complete the provided skeleton code within the `entri_ros2_pubsub_cpp` package to:
1. Define a `RobotStatus` custom message.
2. Ensure CMake/Package XML configurations are set to compile the message.
3. Write a publisher that continually updates the status.
4. Write a subscriber that parses the status and logs it formatted cleanly.

### Requirements

1. **Custom Message Definition (`msg/RobotStatus.msg`)**:
    - Include `int32 robot_id`.
    - Include `float32 battery_level`.
    - Include `string status_message`.

2. **Publisher Module (`src/publisher_node.cpp`)**:
    - Complete the skeleton to publish on topic `"robot_status"`.
    - Use your new `RobotStatus` interface.
    - Set the `robot_id` to `12`, the `status_message` to `"Operating"`, and decay the battery continuously (skeleton provides logic, you just attach variables).
    - Tip: A C++ namespace for custom msgs looks like `entri_ros2_pubsub_cpp::msg::RobotStatus`.

3. **Subscriber Module (`src/subscriber_node.cpp`)**:
    - Complete the skeleton to subscribe to `"robot_status"`.
    - Implement the callback structure so it executes when messages arrive.
    - Inside the callback, use `RCLCPP_INFO` to print the exact required format: 
      `Robot X is at Y% battery. Status: Z`
      (*Example:* `Robot 12 is at 99.5% battery. Status: Operating`)

4. **Package Metadata (`package.xml`)**:
    - Fill out the empty dependencies (`rclcpp`, `rosidl_default_generators`, etc).
    - Note: Generating messages in ROS2 requires specific build and execution tool dependencies.

5. **Build Configuration (`CMakeLists.txt`)**:
    - Complete `rosidl_generate_interfaces` block.
    - Add executables for `publisher_node` and `subscriber_node`.
    - Note: After adding an executable, you *must* append dependency targets using `ament_target_dependencies` AND `rosidl_target_interfaces` to get your node to "know" about your custom message.

## How to Test Locally
Before pushing to GitHub, ensure your code compiles and runs successfully on your local ROS2 workspace.

```bash
# 1. Build the package
colcon build --packages-select entri_ros2_pubsub_cpp

# 2. Source the setup file
source install/setup.bash

# 3. Open Terminal 1 and Run Subscriber
ros2 run entri_ros2_pubsub_cpp subscriber_node

# 4. Open Terminal 2 and Run Publisher
source install/setup.bash
ros2 run entri_ros2_pubsub_cpp publisher_node
```

### Expected Output

**Terminal 2 (Publisher):**
```shell
[INFO] [1710000000.123] [minimal_publisher]: Publishing status for Robot 12
...
```

**Terminal 1 (Subscriber):**
```shell
[INFO] [1710000000.125] [minimal_subscriber]: Robot 12 is at 100.0% battery. Status: Operating
[INFO] [1710000000.625] [minimal_subscriber]: Robot 12 is at 99.5% battery. Status: Operating
```

The automated GitHub grader will verify if the student correctly compiled the workspace and if the subscriber terminal correctly outputs logs matching `"Robot [ID] is at [BATT]% battery"`.