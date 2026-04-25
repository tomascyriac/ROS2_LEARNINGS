[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Z3mLYNF8)
# Assignment: ROS2 C++ Custom Service & Client

**Course:** ROS2 Fundamentals  
**Build System:** ament_cmake

## Objective
The goal of this assignment is to verify your understanding of:
1. Creating a custom ROS2 service format (`.srv`).
2. Integrating custom services within a ROS2 package.
3. Building custom services using `rosidl` packages.
4. Implementing Server and Client nodes that communicate using this custom service type.

## Problem Statement
You are building a Task Allocation system for a smart factory robot. 
You must complete the provided skeleton code within the `entri_ros2_srvcli_cpp` package to:
1. Define a `RobotTask` custom service.
2. Configure CMake/Package XML correctly to compile the service.
3. Write a Service node (`service_node`) that allocates tasks.
4. Write a Client node (`client_node`) that triggers the task allocation and logs the result.

### Requirements

1. **Custom Service Definition (`srv/RobotTask.srv`)**:
    - **Request**: Include `int32 task_id` and `string task_name`.
    - **Response**: Include `bool success` and `string task_response`.

2. **Service Module (`src/service_node.cpp`)**:
    - Complete the skeleton to offer the service named `"allocate_robot_task"`.
    - Use your new `RobotTask` interface.
    - Inside the callback structure provided: 
      - If `task_id` is an *even* number, respond with `success = true` and `task_response = "Task <task_name> allocated successfully."`
      - If `task_id` is an *odd* number, respond with `success = false` and `task_response = "Task <task_name> rejected."`
    - Tip: A C++ namespace for custom services looks like `entri_ros2_srvcli_cpp::srv::RobotTask`.

3. **Client Module (`src/client_node.cpp`)**:
    - Complete the skeleton to call the `"allocate_robot_task"` service.
    - Initialize the request with `task_id = 42` and `task_name = "Move_To_A"`.
    - Handle waiting for the service to become available before sending.
    - Log the response using exactly this layout:
      `Task allocation status: True. Response: Task Move_To_A allocated successfully.`

4. **Package Metadata (`package.xml`)**:
    - Fill out the empty dependencies (`rclcpp`, `rosidl_default_generators`, etc).
    - Note: Generating services in ROS2 requires specific build tool dependencies.

5. **Build Configuration (`CMakeLists.txt`)**:
    - Complete the `rosidl_generate_interfaces` block.
    - Add executables for `service_node` and `client_node`.
    - Note: After adding an executable, you *must* append dependency targets using `ament_target_dependencies` AND `rosidl_target_interfaces` to get your node to "know" about your custom service.

## How to Test Locally
Before pushing to GitHub, ensure your code compiles and runs successfully on your local ROS2 workspace.

```bash
# 1. Build the package
colcon build --packages-select entri_ros2_srvcli_cpp

# 2. Source the setup file
source install/setup.bash

# 3. Open Terminal 1 and Run the Service Node
ros2 run entri_ros2_srvcli_cpp service_node

# 4. Open Terminal 2 and Run the Client Node
source install/setup.bash
ros2 run entri_ros2_srvcli_cpp client_node
```

### Expected Output

**Terminal 1 (Service Output):**
```shell
[INFO] [1710000000.123] [rclcpp]: Incoming request
task_id: 42
task_name: Move_To_A
```

**Terminal 2 (Client Output during connection):**
```shell
[INFO] [1710000000.125] [rclcpp]: Task allocation status: True. Response: Task Move_To_A allocated successfully.
```

The automated GitHub grader will verify if you correctly compiled the workspace and if the `client_node` logs exactly match the expected success response string.