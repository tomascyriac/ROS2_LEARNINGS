# ROS2_LEARNINGS

> **Course:**  Robotics and AI  
> **Mentor/Author:** (https://github.com/arjunskumar) 

---

## 📌 Table of Contents

1. [What is ROS2?](#1-what-is-ros2)
   - 1.1 [Why ROS2 over ROS1?](#why-ros2-over-ros1)
   - 1.2 [How ROS2 Nodes Communicate](#how-ros2-nodes-communicate)
   - 1.3 [ROS IDL and Message Generation](#ros-idl-and-message-generation)
2. [Required Software Setup](#2-required-software-setup)
   - 2.1 [Ubuntu 24.04 LTS](#21-ubuntu-2404-lts)
   - 2.2 [Visual Studio Code](#22-visual-studio-code)
   - 2.3 [Install ROS2 Jazzy](#23-install-ros2-jazzy)
   - 2.4 [GitHub Setup](#24-github-setup)
   - 2.5 [Terminal Tools](#25-terminal-tools)
   - 2.6 [Critical Environment Variables](#26-critical-environment-variables)
3. [ROS2 C++ Fundamentals](#3-ros2-c-fundamentals)
   - 3.1 [Running Built-in C++ Examples](#31-running-built-in-c-examples)
   - 3.2 [Create a Colcon Workspace](#32-create-a-colcon-workspace)
   - 3.3 [Create Your First C++ Package](#33-create-your-first-c-package)
   - 3.4 [Hello World in C++](#34-hello-world-in-c)
   - 3.5 [Beginner Introduction to CMake for ROS 2](#35-beginner-introduction-to-cmake-for-ros-2)
4. [Publishers and Subscribers](#4-publishers-and-subscribers)
   - 4.1 [Create a C++ Publisher](#41-create-a-c-publisher)
   - 4.2 [Create a C++ Subscriber](#42-create-a-c-subscriber)
   - 4.3 [Visualizing Pub-Sub with rqt_graph](#43-visualizing-pub-sub-with-rqt_graph)
   - 4.4 [QoS (Quality of Service)](#44-qos-quality-of-service)
5. [Launch Files](#5-launch-files)
   - 5.1 [Python-based Launch File for C++ Nodes](#51-python-based-launch-file-for-c-nodes)
   - 5.2 [Remapping Topics in Launch Files](#52-remapping-topics-in-launch-files)
   - 5.3 [Launch File Arguments and Conditions](#53-launch-file-arguments-and-conditions)
6. [Parameters in C++](#6-parameters-in-c)
   - 6.1 [Declare and Use Parameters](#61-declare-and-use-parameters)
   - 6.2 [Set Parameters from a Launch File](#62-set-parameters-from-a-launch-file)
   - 6.3 [Loading Explicit Parameters via YAML Config](#63-loading-explicit-parameters-via-yaml-config)
   - 6.4 [Dynamic Parameter Validation and Reloading](#64-dynamic-parameter-validation-and-reloading)
7. [Services in C++](#7-services-in-c)
   - 7.1 [Custom Service Interface](#71-custom-service-interface)
   - 7.2 [C++ Service Server](#72-c-service-server)
   - 7.3 [C++ Service Client](#73-c-service-client)
   - 7.4 [Service Server and Client Parallel Launch](#74-service-server-and-client-parallel-launch)
   - 7.5 [Unit Testing with `ament_add_gtest`](#75-unit-testing-with-ament_add_gtest)
8. [Custom Message Types](#8-custom-message-types)
   - 8.1 [Create Custom Message Logic node](#81-create-custom-message-logic-node)
   - 8.2 [Custom Application Launch](#82-custom-application-launch)
9. [Actions in C++](#9-actions-in-c)
   - 9.1 [Action Interface Definition](#91-action-interface-definition)
   - 9.2 [C++ Action Server](#92-c-action-server)
   - 9.3 [C++ Action Client](#93-c-action-client)
   - 9.4 [Lifecycle Nodes in Production Systems](#94-lifecycle-nodes-in-production-systems)
10. [rosbag2 — Data Recording and Replay](#10-rosbag2--data-recording-and-replay)
11. [TurtleSim Practice Session](#11-turtlesim-practice-session)
12. [CMakeLists.txt Reference](#12-cmakeliststxt-reference)
13. [Quick Reference Tables](#13-quick-reference-tables)
    - 13.1 [Useful Linux Commands](#131-useful-linux-commands)
    - 13.2 [Useful ROS2 CLI Commands](#132-useful-ros2-cli-commands)
    - 13.3 [rclcpp API Cheatsheet](#133-rclcpp-api-cheatsheet)
    - 13.4 [rosbag2 Quick Commands](#134-rosbag2-quick-commands)

---

## 1. What is ROS2?

**ROS** (Robot Operating System) is an open-source framework for building robotic applications. It is not a traditional operating system — it is a middleware layer that sits on top of Linux and provides:

- A **communication infrastructure** (topics, services, actions)
- A **build system** (`colcon` + `ament`)
- A rich **ecosystem of tools and libraries**

### Why ROS2 over ROS1?

| Feature | ROS1 | ROS2 |
|---|---|---|
| Middleware | Custom transport | DDS (Data Distribution Service) |
| Real-time support | No | Yes |
| Multi-robot systems | Limited |  Native support |
| Security | None |  DDS Security |
| Python version | Python 2 | Python 3 |
| C++ standard | C++11 | C++14/17 |
| Master node required |  (`roscore`) |  Not needed |
| Build System | `catkin_make` or `catkin build` | `colcon build` (using `ament`) |

#### Understanding the Build Systems
- **`catkin` (ROS 1)**: In ROS 1, workspaces were built using the `catkin` build system (often invoked via `catkin_make` or `catkin build`).
- **`colcon build` (ROS 2)**: Colcon is the universal command-line tool used in ROS 2 to build workspaces. It is language-agnostic and iterates over your packages to build them independently.
- **`ament` (ROS 2)**: While `colcon` is the tool that *runs* the build, `ament` is the underlying macro and packaging system (an evolution of `catkin`). When you see `ament_cmake` in a ROS 2 C++ package, it means the package config uses the Ament CMake macros to handle dependencies, library linking, and installation paths automatically!

>  **ROS1 Noetic** reached End of Life in May 2025. All new Entri Robotics and AI projects use **ROS2 Jazzy Jalisco**.

### How ROS2 Nodes Communicate

```
  [Publisher Node]  ──────►  /topic  ──────►  [Subscriber Node]
  [Client Node]     ◄──────►  /service  ◄──────►  [Server Node]
  [Action Client]   ◄──────►  /action   ◄──────►  [Action Server]
```

- **Topics**: Asynchronous, one-to-many. Publisher doesn't know who is listening.
- **Services**: Synchronous, request-response between two specific nodes.
- **Actions**: Long-running tasks with feedback. A basic custom action example is covered in Section 9.

### ROS IDL and Message Generation

ROS 2 interface files such as `.msg`, `.srv`, and `.action` are written using **ROS IDL**.

- **IDL** means **Interface Definition Language**
- it describes the structure of data, not the application logic
- ROS 2 uses these files to generate language-specific code automatically

Examples:

- `EntriRobotStatus.msg` defines a message structure
- `EntriCalc.srv` defines request and response structures
- `MoveDistance.action` defines goal, result, and feedback structures

You write:

```text
msg / srv / action files
```

ROS 2 generates:

```text
C++ headers + Python bindings + middleware type support
```

That is why this include works in C++:

```cpp
#include "entri_ros2_interfaces/msg/entri_robot_status.hpp"
```

#### How does a message convert internally?

At a beginner level, the flow looks like this:

```text
1. You define a message in ROS IDL
2. rosidl generates C++/Python message classes
3. Your node fills a C++ message object
4. rclcpp passes it to ROS 2 middleware layers
5. DDS serializes the data into bytes for transport
6. The subscriber side deserializes those bytes back into the same message type
7. Your callback receives the reconstructed message object
```

So when you write:

```cpp
std_msgs::msg::String msg;
msg.data = "hello";
publisher_->publish(msg);
```

ROS 2 is not sending a raw C++ object directly over the network. It uses generated type support plus DDS serialization underneath.

---

## 2. Required Software Setup

### 2.1 Ubuntu 24.04 LTS

ROS2 Jazzy Jalisco requires **Ubuntu 24.04 (Noble Numbat)**. You have several installation options:

| Option | Recommended? | Notes |
|---|---|---|
| **Native install** | Best | Full GPU support, best performance |
| **WSL2** (Windows 11) | Good | GUI works, minor 3D acceleration limitations |
| **VMware Fusion** |  OK | Free for personal use; 3D may be tricky |
| **Docker** | Advanced | Good for CI/CD, tricky for GUI/GPU |
| **Online (The Construct)** |  Not recommended | Poor GUI experience, no environment setup practice |

> **Entri Tip:** Choose **native install** or **WSL2** — you will learn valuable Linux environment skills that are essential for robotics engineering.

---

### 2.2 Visual Studio Code

Download from [code.visualstudio.com](https://code.visualstudio.com/).

**Recommended Extensions for ROS2 C++ Development:**

```
ext install ms-vscode.cpptools          # C/C++ IntelliSense
ext install ms-vscode.cmake-tools       # CMake support
ext install twxs.cmake                  # CMake syntax highlighting
ext install ms-iot.vscode-ros           # ROS extension
ext install yzhang.markdown-all-in-one  # For project documentation
ext install ms-vscode-remote.remote-wsl # If using WSL2
ext install ms-vscode-remote.remote-ssh # For physical robot access
```

To install all at once in a terminal:
```bash
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
code --install-extension ms-iot.vscode-ros
```

---

### 2.3 Install ROS2 Jazzy

Follow the [official installation guide](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html). Here is a condensed version:

```bash
# Step 1: Set locale
sudo apt update && sudo apt install locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

# Step 2: Add the ROS2 apt repository
sudo apt install software-properties-common
sudo add-apt-repository universe
sudo apt update && sudo apt install curl -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
  -o /usr/share/keyrings/ros-archive-keyring.gpg

echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
  http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | \
  sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Step 3: Install ROS2 Jazzy (full desktop)
sudo apt update
sudo apt upgrade
sudo apt install ros-jazzy-desktop

# Step 4: Source the environment
source /opt/ros/jazzy/setup.bash
```

**Add auto-sourcing to `.bashrc`:**

```bash
echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
echo "source ~/ros2_ws/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

> **GPG Key Issue Fix:** If you see a signature error:
> ```bash
> sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
>   -o /usr/share/keyrings/ros-archive-keyring.gpg
> ```

---

### 2.4 GitHub Setup

All Entri Robotics and AI projects are submitted via GitHub.

1. [Create a GitHub account](https://github.com/home)
2. [Sign up for GitHub Student Developer Pack](https://education.github.com/pack) (free pro tools!)
3. Configure git locally:
```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
```

**Recommended git client:** [GitKraken](https://www.gitkraken.com) (free with Student Pack) or use the CLI.

---

### 2.5 Terminal Tools

Install `terminator` for split-pane terminal support (you'll need many terminals simultaneously!):

```bash
sudo apt install terminator
```

- **Linux:** Use `terminator` (supports split windows)
---

### 2.6 Critical Environment Variables

Beginners frequently run into hidden networking and middleware issues. Understanding these two environment variables will save you massive debugging headaches:

- **`ROS_DOMAIN_ID`**: Isolates ROS 2 traffic on shared networks. By default, ROS 2 discovers every node on the same network. If multiple students are on the same Wi-Fi, your nodes will interact with their robots! Set a unique number (0-232) to isolate your fleet:
  ```bash
  export ROS_DOMAIN_ID=5
  ```
- **`RMW_IMPLEMENTATION`**: Defines which DDS middleware runs under the hood. ROS 2 installs `rmw_fastrtps_cpp` by default, but you might need `rmw_cyclonedds_cpp` for complex Wi-Fi setups.
  ```bash
  export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
  ```

## 3. ROS2 C++ Fundamentals

### 3.1 Running Built-in C++ Examples

ROS2 ships with built-in demo nodes. Let's test our installation:

**Terminal 1 — Start the C++ publisher (talker):**
```bash
ros2 run demo_nodes_cpp talker
```

Expected output:
```
[INFO] [talker]: Publishing: 'Hello World: 1'
[INFO] [talker]: Publishing: 'Hello World: 2'
[INFO] [talker]: Publishing: 'Hello World: 3'
```

**Terminal 2 — Start the C++ listener:**
```bash
ros2 run demo_nodes_cpp listener
```

Expected output:
```
[INFO] [listener]: I heard: [Hello World: 5]
[INFO] [listener]: I heard: [Hello World: 6]
```

**Useful CLI tools to inspect running nodes:**

```bash
ros2 node list                    # List all running nodes
ros2 node info /talker            # Detailed info about a node
ros2 topic list                   # List all active topics
ros2 topic info /chatter          # Info about a topic
ros2 topic echo /chatter          # Print messages from a topic
rqt_graph                         # Visualize node connections
rqt                               # Full graphical dashboard
```

---

### 3.2 Create a Colcon Workspace

A **colcon workspace** is the root directory where all your ROS2 packages live.

```bash
# Create the workspace directory
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws
```

**Workspace structure:**
```
ros2_ws/
├── src/               ← Your package source code lives here
├── build/             ← Generated by colcon build (don't edit)
├── install/           ← Installed binaries and headers
└── log/               ← Build logs
```

Build the (empty) workspace:
```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
```

> **Always run `colcon build` from the workspace root (`~/ros2_ws`), never from inside a package folder.**

### Workspace Building and Testing Essentials

Beyond simply running `colcon build`, there are critical commands for dependency management and advanced build flags:

- **Installing Missing Dependencies with `rosdep`**: Before building any new downloaded package, run `rosdep` to fetch missing underlying C++ system libraries (like PCL, Eigen, cv_bridge) required by the package:
  ```bash
  # Run this in the workspace root
  rosdep install -i --from-path src --rosdistro jazzy -y
  ```

- **`--symlink-install` (Python & Launch files)**: When you edit a Python node or an XML/Python launch file, you typically have to rebuild. Using `--symlink-install` links the source files to the install directory, meaning changes take effect immediately without needing to re-run `colcon build`.
  ```bash
  colcon build --symlink-install
  ```

- **Executing Tests**: ROS 2 emphasizes heavy test-driven development (`gtest`, `ament_lint`).
  ```bash
  colcon test               # Executes all testing profiles across your packages
  colcon test-result --all  # Summarizes precisely which unit tests passed/failed
  ```

- **Cleaning the Workspace**: Unlike ROS 1 which featured `catkin clean`, `colcon` does NOT have a native clean command! To perfectly clean a workspace, you must explicitly delete the autogenerated storage directories from your workspace root:
  ```bash
  rm -rf build/ install/ log/
  ```
  **Why do we need to delete these?** 
  - `build/`: Deletes stale or corrupted CMake caches securely.
  - `install/`: Drops all lingering compiled binaries. Without this, old versions of renamed nodes or interfaces might still execute!
  - `log/`: Wipes out old system compilation log warnings.

---

### 3.3 Create Your First C++ Package

Navigate into the `src` folder and create a new `ament_cmake` package:

```bash
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_cmake entri_ros2_basics_cpp
```

> **Entri Naming Convention:** All packages in this course follow the pattern `entri_ros2_<module>_cpp`.

After creation, your package looks like:
```
entri_ros2_basics_cpp/
├── CMakeLists.txt     ← Build configuration (most important file for C++)
├── include/
│   └── entri_ros2_basics_cpp/   ← Header files go here
├── package.xml        ← Package metadata and dependencies
└── src/               ← C++ source files go here
```

**Edit `package.xml`** to fill in course-specific metadata:

```xml
<?xml version="1.0"?>
<package format="3">
  <name>entri_ros2_basics_cpp</name>
  <version>1.0.0</version>
  <description>Entri Robotics and AI - ROS2 C++ Basics Package</description>

  <maintainer email="your@email.com">Your Name</maintainer>
  <license>Apache-2.0</license>

  <buildtool_depend>ament_cmake</buildtool_depend>
  <depend>rclcpp</depend>
  <depend>std_msgs</depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```

#### Understanding `package.xml` Dependency Types
While you'll frequently use the blanket `<depend>` tag, as a senior systems engineer, you should understand the targeted dependency variants used to reduce Docker build sizes and optimize processing:

- **`<depend>`**: A universal macro. It declares that the package needs the dependency for **building**, for **exporting**, and at **runtime level**. (Most common).
- **`<build_depend>`**: The dependency is ONLY needed at compile time. It handles header files or build tools but isn't strictly necessary dynamically during robot operation.
- **`<exec_depend>`**: The dependency is ONLY needed when running the compiled binary. Very common in Python packages, or when your package only interacts with another via CLI without compiling against its headers.

---

### 3.4 Hello World in C++

Create `src/hello_world.cpp`:

```cpp
#include "rclcpp/rclcpp.hpp" // Fundamental ROS 2 C++ API library

int main(int argc, char **argv)
{
    // Initialize the ROS 2 C++ environment.
    // This must be called before using any rclcpp features.
    rclcpp::init(argc, argv);

    // Create a node named "entri_hello_world".
    // make_shared creates a shared pointer managing our node, which is the ROS 2 convention.
    auto node = rclcpp::Node::make_shared("entri_hello_world");

    // Use the node's logger to print an INFO level message.
    // get_logger() grabs the logger specific to this node so output is pre-fixed with the node name.
    RCLCPP_INFO(node->get_logger(), "Hello from Entri Robotics and AI!");

    // Cleanly shutdown the rclcpp context once we're done.
    rclcpp::shutdown();
    
    return 0; // Return zero indicating a successful execution
}
```

**Register the executable in `CMakeLists.txt`:**

```cmake
cmake_minimum_required(VERSION 3.8)
project(entri_ros2_basics_cpp)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(hello_world src/hello_world.cpp)
ament_target_dependencies(hello_world rclcpp)

install(TARGETS
  hello_world
  DESTINATION lib/${PROJECT_NAME})

ament_package()
```

**Build and run:**

```bash
cd ~/ros2_ws
colcon build --packages-select entri_ros2_basics_cpp
source install/setup.bash
ros2 run entri_ros2_basics_cpp hello_world
```

Expected output:
```
[INFO] [entri_hello_world]: Hello from Entri Robotics and AI!
```

> **Tip:** Use `--packages-select <pkg_name>` with `colcon build` to build only one package instead of the entire workspace. This saves time!

---

### 3.5 Beginner Introduction to CMake for ROS 2

For ROS 2 C++ packages, `CMakeLists.txt` dictates exactly how your code is compiled. Understanding the basics will save hours of compilation debugging!

```cmake
# 1. Start by demanding minimum CMake capabilities and naming the package exactly as package.xml
cmake_minimum_required(VERSION 3.8)
project(entri_ros2_basics_cpp)

# 2. Instruct CMake to locate specific external ROS 2 dependencies that your code imports
find_package(ament_cmake REQUIRED)    # The core ROS 2 build system
find_package(rclcpp REQUIRED)         # The fundamental ROS2 real-time C++ API
find_package(std_msgs REQUIRED)       # Standard ROS message definitions (like String)

# 3. Generate the executable binary. Maps executable name -> C++ Source file
add_executable(entri_demo src/hello_world.cpp)

# 4. Explicitly link the executable to the ROS 2 packages/libraries it depends on!
# If you "include" something in C++, it must be linked here.
ament_target_dependencies(entri_demo rclcpp std_msgs)

# 5. Provide installation rules so `colcon` knows where to place your binary upon build.
# This makes `ros2 run entri_ros2_basics_cpp entri_demo` possible.
install(TARGETS entri_demo DESTINATION lib/${PROJECT_NAME})

# 6. Final requirement universally executed across ROS 2 Ament builds
ament_package()
```

#### Why do we use the `install()` target in CMake?
In ROS 2, `colcon build` takes your source code and compiles the binaries. However, ROS 2 needs a standardized place to find these executables when you execute a `ros2 run` command. 

- **The `install/lib` folder:** The `install()` target tells CMake to copy your compiled executable binaries into the workspace's `install/lib/<package_name>/` directory. Without this, ROS 2 won't know where your program is!
- **Production & Sharing:** When deploying to a real robot for production, we do **not** share or copy the `src` folder. We solely distribute the compressed `install` folder. The robot only needs the pre-compiled binaries to execute the software, which protects your proprietary source code and significantly saves disk space!

---

## 4. Publishers and Subscribers

### 4.1 Create a C++ Publisher

The publish-subscribe pattern is the backbone of ROS2 communication. A **publisher** sends data to a **topic**; one or many **subscribers** receive it asynchronously.

Create `src/publisher.cpp`:

```cpp
#include "rclcpp/rclcpp.hpp"        // Core ROS 2 functionality
#include "std_msgs/msg/string.hpp"  // String message type from std_msgs package

// Create a class inheriting from the rclcpp::Node base class
class EntriPublisher : public rclcpp::Node
{
public:
    /**
     * @brief Constructor for the EntriPublisher.
     * Initializes the node with the name "entri_cpp_publisher" and configures the publisher and timer.
     */
    EntriPublisher() : Node("entri_cpp_publisher"), count_(0)
    {
        // Obtain a publisher object for the topic "entri/topic".
        // The second parameter denotes to the queue size (QoS baseline) as 10.
        publisher_ = this->create_publisher<std_msgs::msg::String>("entri/topic", 10);

        // create_wall_timer triggers the given callback function periodically.
        // It's firing every 500 milliseconds (i.e. 2Hz).
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&EntriPublisher::publishMessage, this)
        );

        // Log to terminal that the publisher has successfully initialized
        RCLCPP_INFO(this->get_logger(), "Entri Publisher node has started!");
    }

private:
    /**
     * @brief The timer callback execution block causing message publication.
     */
    void publishMessage()
    {
        // Instantiate a string message structure
        auto msg = std_msgs::msg::String();
        
        // Form the string message to be dispatched
        msg.data = "Entri ROS2 Message #" + std::to_string(count_++);

        // Issue a terminal console log displaying what is being pushed to the topic
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
        
        // Execute the publish call transmitting the data
        publisher_->publish(msg);
    }

    // Class member attributes
    size_t count_;                                                    // Running counter indicating message iteration
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;   // A shared pointer to the created ROS 2 Publisher instance
    rclcpp::TimerBase::SharedPtr timer_;                              // Timekeeper responsible for invoking the callback
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EntriPublisher>());
    rclcpp::shutdown();
    return 0;
}
```

#### What does the `10` mean here?

That `10` is the QoS queue depth shorthand. The full beginner-friendly explanation of QoS, DDS, reliability, durability, and compatibility is covered in **Section 4.4**.

**Update `CMakeLists.txt`:**

```cmake
add_executable(entri_publisher src/publisher.cpp)
ament_target_dependencies(entri_publisher rclcpp std_msgs)

install(TARGETS
  hello_world
  entri_publisher
  DESTINATION lib/${PROJECT_NAME})
```

**Build and run:**

```bash
colcon build --packages-select entri_ros2_basics_cpp
ros2 run entri_ros2_basics_cpp entri_publisher
```

**Observe the topic in another terminal:**

```bash
ros2 topic echo /entri/topic
```

```
data: 'Entri ROS2 Message #3'
---
data: 'Entri ROS2 Message #4'
---
```

---

### 4.2 Create a C++ Subscriber

Create `src/subscriber.cpp`:

```cpp
#include "rclcpp/rclcpp.hpp"        // Included for overarching ROS 2 C++ capabilities
#include "std_msgs/msg/string.hpp"  // The same message type the publisher provides

// Creating the derived subscriber class from rclcpp::Node
class EntriSubscriber : public rclcpp::Node
{
public:
    /**
     * @brief Constructor for the EntriSubscriber node.
     */
    EntriSubscriber() : Node("entri_cpp_subscriber")
    {
        // Instantiating a subscription binding "entri/topic" with our message callback
        // The queue size is fixed at 10. `std::bind` binds the callback function to handle events
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "entri/topic",
            10,
            std::bind(&EntriSubscriber::messageCallback, this, std::placeholders::_1)
        );

        // Status update showing that spin will proceed effectively listening for outputs
        RCLCPP_INFO(this->get_logger(), "Entri Subscriber node has started!");
    }

private:
    /**
     * @brief Callback function that processes incoming std_msgs/String logic
     * @param msg Formally captured parameter carrying newly broadcast payload
     */
    void messageCallback(const std::shared_ptr<const std_msgs::msg::String> & msg)
    {
        RCLCPP_INFO(this->get_logger(), "I received: '%s'", msg->data.c_str());
    }

    // Shared pointer housing the subscription implementation
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EntriSubscriber>());
    rclcpp::shutdown();
    return 0;
}
```

**Callback signature note:** Prefer `const MsgType::SharedPtr & msg` or `std::shared_ptr<const MsgType>` for subscriber callbacks. Passing `SharedPtr` by value adds an extra reference-count operation on every message, and this signature tends to get copied into production code.

**Update `CMakeLists.txt`:**

```cmake
add_executable(entri_subscriber src/subscriber.cpp)
ament_target_dependencies(entri_subscriber rclcpp std_msgs)

install(TARGETS
  hello_world
  entri_publisher
  entri_subscriber
  DESTINATION lib/${PROJECT_NAME})
```

**Run publisher and subscriber together:**

```bash
# Terminal 1
ros2 run entri_ros2_basics_cpp entri_publisher

# Terminal 2
ros2 run entri_ros2_basics_cpp entri_subscriber
```

Subscriber output:
```
[INFO] [entri_cpp_subscriber]: I received: 'Entri ROS2 Message #0'
[INFO] [entri_cpp_subscriber]: I received: 'Entri ROS2 Message #1'
[INFO] [entri_cpp_subscriber]: I received: 'Entri ROS2 Message #2'
```

---

### 4.3 Visualizing Pub-Sub with rqt_graph

After starting both nodes, open `rqt_graph` to see the communication:

```bash
rqt_graph
```

You should see:
```
/entri_cpp_publisher  ──►  /entri/topic  ──►  /entri_cpp_subscriber
```

You can also use `rqt` for a full dashboard:
```bash
rqt
```

Go to `Plugins > Topics > Topic Monitor` to observe live message data.

---

### 4.4 QoS (Quality of Service)

Every time you see `10` passed to `create_publisher` or `create_subscription`, that is a **QoS history depth** shorthand. ROS 2 uses **QoS** settings, backed by **DDS**, to decide how messages are buffered and delivered between nodes.

#### DDS in Simple Words

ROS 2 sits on top of **DDS**: **Data Distribution Service**.

- ROS 2 gives you the APIs like publishers, subscribers, services, and actions
- DDS handles discovery, buffering, retries, and compatibility
- QoS is how we tell DDS what communication behavior we want

```text
Your C++ node code
      ↓
   rclcpp / ROS 2
      ↓
       DDS
      ↓
 network / other nodes
```

#### Core QoS Policies

| Policy | Options | Description |
|---|---|---|
| **History** | `KEEP_LAST(N)` / `KEEP_ALL` | How many recent messages to buffer |
| **Reliability** | `RELIABLE` / `BEST_EFFORT` | Guarantee delivery vs. fastest throughput |
| **Durability** | `VOLATILE` / `TRANSIENT_LOCAL` | Whether late subscribers receive saved data |

#### Beginner Intuition

- **Reliable**: "Make sure this arrives."
- **Best effort**: "Send it fast. If one sample is lost, keep going."
- **Volatile**: "Only future messages matter."
- **Transient local**: "Also give late subscribers the latest saved message."

#### Basic Examples

```cpp
// Good default for simple status messages
auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
publisher_ = this->create_publisher<std_msgs::msg::String>("status_text", qos);

// Good for fast sensor-like streams
auto sensor_qos = rclcpp::SensorDataQoS();
subscription_ = this->create_subscription<std_msgs::msg::String>(
    "fast_sensor_topic", sensor_qos,
    std::bind(&MyNode::callback, this, std::placeholders::_1));

// Good when late subscribers should receive the latest value immediately
auto latched_qos = rclcpp::QoS(rclcpp::KeepLast(1)).reliable().transient_local();
publisher_ = this->create_publisher<std_msgs::msg::String>("robot_mode", latched_qos);
```

#### Pub-Sub with DDS and QoS Example

The code below shows a very simple publisher and subscriber using an explicit QoS profile instead of the plain integer shorthand.

Publisher:

```cpp
auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile();
publisher_ = this->create_publisher<std_msgs::msg::String>("entri/topic", qos);
```

Subscriber:

```cpp
auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile();
subscription_ = this->create_subscription<std_msgs::msg::String>(
    "entri/topic",
    qos,
    std::bind(&MyNode::callback, this, std::placeholders::_1));
```

What DDS does here:

- discovers the publisher and subscriber automatically
- checks whether the QoS settings are compatible
- serializes the message data
- transports the bytes over the middleware layer
- reconstructs the message on the subscriber side

If you change only the subscriber to a mismatched profile, communication may stop:

```cpp
auto bad_qos = rclcpp::SensorDataQoS();  // BEST_EFFORT style preset
```

That is why beginner ROS 2 debugging often starts with checking both topic name and QoS compatibility.

#### End-to-End DDS Pub-Sub Example

This is the same idea, shown as a tiny publisher/subscriber pair using matching DDS-backed QoS settings.

Publisher node:

```cpp
auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile();
publisher_ = this->create_publisher<std_msgs::msg::String>("dds_demo_topic", qos);

auto msg = std_msgs::msg::String();
msg.data = "hello from DDS-backed ROS 2 pub-sub";
publisher_->publish(msg);
```

Subscriber node:

```cpp
auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile();
subscription_ = this->create_subscription<std_msgs::msg::String>(
    "dds_demo_topic",
    qos,
    [this](const std::shared_ptr<const std_msgs::msg::String> & msg) {
        RCLCPP_INFO(this->get_logger(), "Received: %s", msg->data.c_str());
    });
```

What happens underneath:

```text
Publisher creates ROS 2 message
        ↓
rclcpp uses generated type support
        ↓
DDS serializes the message
        ↓
DDS discovers matching subscriber
        ↓
subscriber DDS endpoint receives bytes
        ↓
ROS 2 reconstructs the message object
        ↓
callback runs in subscriber node
```

Useful commands while testing:

```bash
ros2 topic list
ros2 topic echo /dds_demo_topic
ros2 topic info /dds_demo_topic --verbose
rqt_graph
```

#### Compatibility Rule

Publisher and subscriber QoS must be compatible. If they are not, both nodes may appear to run normally but no messages flow.

```bash
ros2 topic info /entri/topic --verbose
```

Use `RELIABLE` for important commands and state updates, `BEST_EFFORT` for high-rate sensor streams, and `TRANSIENT_LOCAL` when late-joining nodes should still receive the latest value.

---

## 5. Launch Files

Instead of starting nodes one by one, **launch files** start multiple nodes simultaneously with a single command. In ROS2, launch files are written in **Python** (even for C++ packages).

### 5.1 Python-based Launch File for C++ Nodes

Create the launch directory:
```bash
mkdir -p ~/ros2_ws/src/entri_ros2_basics_cpp/launch
```

The current workspace does **not** include a separate `entri_pubsub.launch.py`. Instead, the package ships a remapping-based launch file named `launch/pub_sub_remap.launch.py`, which is explained in Section 5.2.

If you only want to understand the minimal structure of a Python launch file for two C++ nodes, the non-remapped version would look like this:

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    """
    Entri Robotics and AI — Launch both publisher and subscriber nodes.
    """

    publisher_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_publisher',
        name='entri_publisher',
        output='screen',
    )

    subscriber_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_subscriber',
        name='entri_subscriber',
        output='screen',
    )

    return LaunchDescription([
        publisher_node,
        subscriber_node,
    ])
```

**Register the launch directory in `CMakeLists.txt`:**

```cmake
# Install launch files
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch)
```

**Build the package:**

```bash
colcon build --packages-select entri_ros2_basics_cpp
```

To run the actual launch file present in this workspace, use:

```bash
ros2 launch entri_ros2_basics_cpp pub_sub_remap.launch.py
```

Both nodes will start in the same terminal with labeled output.

---

### 5.2 Remapping Topics in Launch Files

**Remapping** lets you redirect a node's topic to a different name without changing the source code. This is powerful for composing multi-robot systems.

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    """
    Entri Robotics and AI — Publish/Subscribe with Remappings Launch File
    
    This launch file runs the publisher and subscriber nodes simultaneously,
    but isolates their communication by remapping the topic "entri/topic"
    to "robot1/entri/topic". This pattern allows running multiple identical
    robots in the same domain without message interference.
    """

    # Define the publisher node with topic remappings
    publisher_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_publisher',
        name='robot1_publisher', # Renaming the node instance
        output='screen',         # Ensures logs appear in the terminal
        remappings=[
            ('entri/topic', 'robot1/entri/topic'), # Old Topic -> New Topic
        ]
    )

    # Define the subscriber node applying identical topic remappings
    subscriber_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_subscriber',
        name='robot1_subscriber',
        output='screen',
        remappings=[
            ('entri/topic', 'robot1/entri/topic'),
        ]
    )

    # Return the Description triggering execution of both nodes
    return LaunchDescription([
        publisher_node,
        subscriber_node
    ])
```

You can also remap from the CLI without a launch file:
```bash
ros2 run entri_ros2_basics_cpp entri_publisher \
  --ros-args --remap entri/topic:=robot1/entri/topic
```

---

### 5.3 Launch File Arguments and Conditions

Real ROS 2 launch files are often parameterized so the same file can behave differently in development, testing, and deployment. Three core tools appear often:

- `DeclareLaunchArgument`: defines a user-configurable input
- `LaunchConfiguration`: reads the argument value inside the launch description
- `IfCondition`: starts something only when a condition evaluates to true

Example:

```python
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    use_remap = LaunchConfiguration('use_remap')

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_remap',
            default_value='true',
            description='Whether to remap entri/topic into robot1/entri/topic'
        ),

        Node(
            package='entri_ros2_basics_cpp',
            executable='entri_publisher',
            name='entri_publisher',
            output='screen',
            remappings=[('entri/topic', 'robot1/entri/topic')],
            condition=IfCondition(use_remap),
        ),
    ])
```

After adding this pattern to a launch file, run it with a custom argument like this:

```bash
ros2 launch entri_ros2_basics_cpp pub_sub_remap.launch.py use_remap:=false
```

The current workspace launch files use fixed settings, but this pattern becomes essential once you start reusing the same launch file for simulation, hardware, and multi-robot setups.

---

## 6. Parameters in C++

**Parameters** allow you to configure a node's behavior at runtime without recompiling. Think of them as named, runtime-configurable variables.

### 6.1 Declare and Use Parameters

Create `src/param_node.cpp`:

```cpp
/**
 * @file param_node.cpp
 * @brief Demonstrates ROS 2 parameter declaration, retrieval, and dynamic usage.
 *
 * Parameters act as configurable settings for a node. In this example, the node
 * reads configuration elements such as 'robot_name' and 'publish_rate_hz' and
 * adjusts its timer dynamically.
 */

#include "rclcpp/rclcpp.hpp"

class EntriParamNode : public rclcpp::Node
{
public:
    /**
     * @brief Constructor for EntriParamNode.
     * Declares acceptable parameters, optionally defining default values,
     * and sets up a timer whose frequency is configured dynamically.
     */
    EntriParamNode() : Node("entri_param_node")
    {
        // Declare expected ROS 2 parameters and initialize default values.
        // It is necessary to declare a parameter before it can be used or overridden.
        this->declare_parameter<std::string>("robot_name", "EntriBot");
        this->declare_parameter<double>("publish_rate_hz", 2.0);
        this->declare_parameter<int>("max_messages", 100);

        // Fetch user-specified or statically configured values corresponding to parameter names.
        robot_name_     = this->get_parameter("robot_name").as_string();
        publish_rate_hz_ = this->get_parameter("publish_rate_hz").as_double();
        max_messages_   = this->get_parameter("max_messages").as_int();

        // Output the recognized parameters for debugging and system info.
        RCLCPP_INFO(this->get_logger(), "Robot Name: %s", robot_name_.c_str());
        RCLCPP_INFO(this->get_logger(), "Publish Rate: %.1f Hz", publish_rate_hz_);
        RCLCPP_INFO(this->get_logger(), "Max Messages: %d", max_messages_);

        // Calculate timer tick duration based on provided frequency configurations
        auto period_ms = std::chrono::milliseconds(
            static_cast<int>(1000.0 / publish_rate_hz_)
        );

        // Setup the recurring event function
        timer_ = this->create_wall_timer(
            period_ms,
            std::bind(&EntriParamNode::timerCallback, this)
        );
    }

private:
    /**
     * @brief A simple callback printing the param-configured robot name.
     */
    void timerCallback()
    {
        // Utilize the configurable parameter when reporting state
        RCLCPP_INFO(this->get_logger(), "[%s] Timer tick!", robot_name_.c_str());
    }

    std::string robot_name_;
    double publish_rate_hz_;
    int max_messages_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    // Begin ROS 2 execution
    rclcpp::init(argc, argv);
    
    // Manage ROS 2 spin loop logic block continuously
    rclcpp::spin(std::make_shared<EntriParamNode>());
    
    // Trigger controlled shutdown sequence to clean up resources
    rclcpp::shutdown();
    return 0;
}
```

**Run with parameter overrides:**

```bash
ros2 run entri_ros2_basics_cpp entri_param_node \
  --ros-args -p robot_name:=Robo1 -p publish_rate_hz:=5.0
```

**Inspect and set parameters live:**

```bash
ros2 param list /entri_param_node
ros2 param get /entri_param_node robot_name
ros2 param set /entri_param_node robot_name "NewBotName"
```

#### Built-in Topics: `/parameter_events` and `/rosout`

ROS 2 also publishes some useful built-in topics automatically:

- **`/parameter_events`**: shows parameter declarations, updates, and deletions
- **`/rosout`**: carries log messages produced by `RCLCPP_INFO`, `RCLCPP_WARN`, and `RCLCPP_ERROR`

These are excellent beginner debugging tools.

```bash
ros2 topic list
ros2 topic echo /parameter_events
ros2 topic echo /rosout
```

Typical use:

- change a parameter with `ros2 param set`
- watch `/parameter_events` to confirm the update happened
- watch `/rosout` to see the node's logs reacting to that change

---

### 6.2 Set Parameters from a Launch File

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    param_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_param_node',
        name='entri_param_node',
        parameters=[{
            'robot_name': 'EntriBot_Alpha',
            'publish_rate_hz': 10.0,
            'max_messages': 500,
        }]
    )

    return LaunchDescription([param_node])
```

You can also use a **YAML parameter file** (preferred for complex configurations):

Create `config/entri_params.yaml`:
```yaml
entri_param_node:
  ros__parameters:
    robot_name: "EntriBot_Prod"
    publish_rate_hz: 5.0
    max_messages: 1000
```

Load it in a launch file:
```python
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('entri_ros2_basics_cpp'),
        'config',
        'entri_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='entri_ros2_basics_cpp',
            executable='entri_param_node',
            parameters=[config]
        )
    ])
```

Install config in `CMakeLists.txt`:
```cmake
install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config)
```

---

### 6.3 Loading Explicit Parameters via YAML Config

An alternative to specifying every parameter via command-line logic is instructing a node to pull directly from YAML structurally.

```cpp
/**
 * @file param_node_yaml.cpp
 * @brief Node intended specifically to have its settings populated via YAML file.
 *
 * Uses basic parameters architecture but relies heavily on the environment
 * launch instructions directing configuration overrides natively.
 */

#include "rclcpp/rclcpp.hpp"

class YamlParamNode : public rclcpp::Node
{
public:
    YamlParamNode() : Node("yaml_param_node")
    {
        // Declare placeholders expecting substitution
        this->declare_parameter<std::string>("robot_name", "DefaultBot");
        this->declare_parameter<double>("publish_rate_hz", 1.0);
        this->declare_parameter<int>("max_messages", 10);

        // Fetch overrides pushed directly from configurations
        std::string robot_name = this->get_parameter("robot_name").as_string();
        double hz = this->get_parameter("publish_rate_hz").as_double();
        int max_msg = this->get_parameter("max_messages").as_int();

        RCLCPP_INFO(this->get_logger(), "YAML Parameters Loaded:");
        RCLCPP_INFO(this->get_logger(), " -> name: %s", robot_name.c_str());
        RCLCPP_INFO(this->get_logger(), " -> rate: %.1f", hz);
        RCLCPP_INFO(this->get_logger(), " -> limit: %d", max_msg);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    
    // We only perform initialization reporting for demonstration purposes
    auto node = std::make_shared<YamlParamNode>();
    rclcpp::spin(node);
    
    rclcpp::shutdown();
    return 0;
}
```

### 6.4 Dynamic Parameter Validation and Reloading

To ensure your code actively monitors changes initiated by `ros2 param set`, we implement a direct callback listening pipeline specifically engineered to hot-swap functionalities contextually.

```cpp
/**
 * @file param_node_dynamic.cpp
 * @brief Dynamic ROS 2 Parameter handling with configuration callbacks.
 *
 * It uses the 'OnSetParametersCallbackHandle' interface to immediately
 * detect and react to standard `ros2 param set` CLI commands dynamically 
 * adjusting behavior at runtime without needing a restart!
 */

#include "rclcpp/rclcpp.hpp"

class DynamicParamNode : public rclcpp::Node
{
public:
    DynamicParamNode() : Node("dynamic_param_node")
    {
        // 1. Declare parameters as usual
        this->declare_parameter<std::string>("robot_name", "EntriBot_Dynamic");
        this->declare_parameter<double>("publish_rate_hz", 2.0);

        // 2. Fetch initial values
        robot_name_ = this->get_parameter("robot_name").as_string();
        publish_rate_hz_ = this->get_parameter("publish_rate_hz").as_double();

        // 3. Register the callback handler exactly for runtime dynamic modifications
        // Using std::bind to attach the member function responding to changes
        param_callback_handle_ = this->add_on_set_parameters_callback(
            std::bind(&DynamicParamNode::parametersCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Started dynamic parameter node. Try running:");
        RCLCPP_INFO(this->get_logger(), "ros2 param set /dynamic_param_node robot_name NewName");

        // 4. Initialize wall timer based on initial parameters
        resetTimer();
    }

private:
    /**
     * @brief Method triggered whenever ANY parameter changes externally
     */
    rcl_interfaces::msg::SetParametersResult parametersCallback(
        const std::vector<rclcpp::Parameter> &parameters)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "success";

        // Iterate through requested parameter updates mapping them internally
        for (const auto &param : parameters) {
            if (param.get_name() == "robot_name") {
                robot_name_ = param.as_string();
                RCLCPP_INFO(this->get_logger(), "--> robot_name dramatically updated to: %s", robot_name_.c_str());
            } 
            else if (param.get_name() == "publish_rate_hz") {
                publish_rate_hz_ = param.as_double();
                RCLCPP_INFO(this->get_logger(), "--> publish_rate_hz updated to: %.1f", publish_rate_hz_);
                resetTimer();
            }
        }
        
        return result; 
    }

    void resetTimer()
    {
        const auto period_ms = std::chrono::milliseconds(
            static_cast<int>(1000.0 / publish_rate_hz_));

        auto new_timer = this->create_wall_timer(
            period_ms,
            std::bind(&DynamicParamNode::timerCallback, this));

        auto old_timer = std::move(timer_);
        timer_ = std::move(new_timer);

        if (old_timer) {
            old_timer->cancel();
            old_timer.reset();
        }
    }

    void timerCallback()
    {
        // Action demonstrating parameter utilization
        RCLCPP_INFO(this->get_logger(), "[%s] Dynamic Ping @ %.1f Hz", robot_name_.c_str(), publish_rate_hz_);
    }

    std::string robot_name_;
    double publish_rate_hz_;
    rclcpp::TimerBase::SharedPtr timer_;
    
    // Explicit handle saving connection states tracking external requests
    OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DynamicParamNode>());
    rclcpp::shutdown();
    return 0;
}
```

---

## 7. Services in C++

**Services** implement a synchronous request-response pattern. Unlike topics (fire and forget), a service client waits for the server to respond.

```
[Service Client]  ──request──►  [Service Server]
                  ◄─response──
```

### 7.1 Custom Service Interface

Create a separate **interfaces package** for custom messages and services:

```bash
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_cmake entri_ros2_interfaces
```

Create `srv/EntriCalc.srv`:

```bash
mkdir -p ~/ros2_ws/src/entri_ros2_interfaces/srv
```

```
# entri_ros2_interfaces/srv/EntriCalc.srv
# Request fields
float64 a
float64 b
string operation   # "add", "subtract", "multiply", "divide"
---
# Response fields
float64 result
bool success
string message
```

**Edit `CMakeLists.txt` for the interfaces package:**

```cmake
cmake_minimum_required(VERSION 3.8)
project(entri_ros2_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/EntriCalc.srv"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
```

**Edit `package.xml`:**

```xml
<buildtool_depend>ament_cmake</buildtool_depend>
<build_depend>rosidl_default_generators</build_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

#### What do these interface-package tags mean?

- **`rosidl_default_generators`**:
  Needed at **build time**.
  Use this when your package contains `.msg`, `.srv`, or `.action` files and ROS 2 must generate code from them.

- **`rosidl_default_runtime`**:
  Needed at **runtime**.
  Use this so the generated message/service/action type support is available after build and install.

- **`rosidl_interface_packages`**:
  This marks the package as an **interface package**.
  Use it when the package's main job is defining interfaces rather than running nodes.

Simple rule of thumb:

```text
I define .msg/.srv/.action files
  -> need rosidl_default_generators

I want the generated interfaces usable after install
  -> need rosidl_default_runtime

This package is mainly an interface-definition package
  -> mark it as rosidl_interface_packages
```

Why this split exists:

- generation happens during build
- runtime support is needed after generation
- package grouping helps ROS 2 tools understand the package role

Build the interfaces package first:
```bash
colcon build --packages-select entri_ros2_interfaces
source install/setup.bash
```

Verify the service was generated:
```bash
ros2 interface show entri_ros2_interfaces/srv/EntriCalc
```

---

### 7.2 C++ Service Server

Create `src/service_server.cpp` in `entri_ros2_basics_cpp`:

```cpp
/**
 * @file service_server.cpp
 * @brief Implements an RPC-style ROS 2 Service Server for the custom EntriCalc interface.
 *
 * This server node awaits calculation requests mapping to mathematical operations,
 * performs the necessary math reliably, handles division-by-zero, and responds.
 */

#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_interfaces/srv/entri_calc.hpp" // Header for custom srv definition

class EntriCalcServer : public rclcpp::Node
{
public:
    /**
     * @brief Node constructor. Prepares the service callback handler.
     */
    EntriCalcServer() : Node("entri_calc_server")
    {
        // Bind the service creation to the "entri_calc" service channel
        service_ = this->create_service<entri_ros2_interfaces::srv::EntriCalc>(
            "entri_calc",
            std::bind(&EntriCalcServer::handleRequest, this,
                      std::placeholders::_1, std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "Entri Calculator Service Server is ready.");
    }

private:
    /**
     * @brief Execution block for translating custom service requests.
     * @param request Shared pointer providing the a, b, and mathematical operation.
     * @param response Shared pointer enabling assignment of evaluated result variables.
     */
    void handleRequest(
        const std::shared_ptr<entri_ros2_interfaces::srv::EntriCalc::Request> request,
        std::shared_ptr<entri_ros2_interfaces::srv::EntriCalc::Response> response)
    {
        // Log incoming operations
        RCLCPP_INFO(this->get_logger(), "Received: %.2f %s %.2f",
            request->a, request->operation.c_str(), request->b);

        response->success = true; // Assume success initially

        // Calculate based on requested operation type string
        if (request->operation == "add") {
            response->result = request->a + request->b;
            response->message = "Addition successful";
        } else if (request->operation == "subtract") {
            response->result = request->a - request->b;
            response->message = "Subtraction successful";
        } else if (request->operation == "multiply") {
            response->result = request->a * request->b;
            response->message = "Multiplication successful";
        } else if (request->operation == "divide") {
            // Error handling: Catch mathematical anomalies like division by zero
            if (request->b == 0.0) {
                response->success = false;
                response->result = 0.0;
                response->message = "ERROR: Division by zero!";
            } else {
                response->result = request->a / request->b;
                response->message = "Division successful";
            }
        } else {
            // General error handling
            response->success = false;
            response->message = "Unknown operation: " + request->operation;
        }

        RCLCPP_INFO(this->get_logger(), "Result: %.2f | %s",
            response->result, response->message.c_str());
    }

    rclcpp::Service<entri_ros2_interfaces::srv::EntriCalc>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EntriCalcServer>());
    rclcpp::shutdown();
    return 0;
}
```

**Update `CMakeLists.txt`** to add the interface dependency:

```cmake
find_package(entri_ros2_interfaces REQUIRED)

add_executable(entri_service_server src/service_server.cpp)
ament_target_dependencies(entri_service_server rclcpp entri_ros2_interfaces)

# Add to install(TARGETS ...) block
```

**Update `package.xml`:**
```xml
<depend>entri_ros2_interfaces</depend>
```

---

### 7.3 C++ Service Client

Create `src/service_client.cpp`:

```cpp
/**
 * @file service_client.cpp
 * @brief An RPC-style ROS 2 Service Client designed to send a custom EntriCalc payload.
 *
 * This client constructs an initial calculation request, awaits server detection,
 * sends the request natively, and parses the received response using future handling.
 */

#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_interfaces/srv/entri_calc.hpp"
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class EntriCalcClient : public rclcpp::Node
{
public:
    /**
     * @brief Node constructor, creates client endpoint attached to "entri_calc".
     */
    EntriCalcClient() : Node("entri_calc_client")
    {
        client_ = this->create_client<entri_ros2_interfaces::srv::EntriCalc>("entri_calc");
    }

    /**
     * @brief Blocks logic until the service path connection is live.
     * @param timeout The maximum timeframe duration the client will wait.
     */
    bool waitForServer(std::chrono::seconds timeout = 5s)
    {
        // Poll connection availability iteratively up to standard timeout
        if (!client_->wait_for_service(timeout)) {
            RCLCPP_ERROR(this->get_logger(), "Service 'entri_calc' not available!");
            return false;
        }
        return true;
    }

    /**
     * @brief Asynchronously pushes variables and operations to remote server setup.
     * @return Formatted future capturing delayed network response dynamically.
     */
    std::shared_future<entri_ros2_interfaces::srv::EntriCalc::Response::SharedPtr>
    sendRequest(double a, double b, const std::string & operation)
    {
        // Package components functionally targeting specific request fields
        auto request = std::make_shared<entri_ros2_interfaces::srv::EntriCalc::Request>();
        request->a = a;
        request->b = b;
        request->operation = operation;

        RCLCPP_INFO(this->get_logger(), "Sending: %.2f %s %.2f", a, operation.c_str(), b);
        
        // Asynchronous client communication        // Call the service asynchronously!
        // We explicitly use .future.share() to avoid the deprecated implicit casting
        return client_->async_send_request(request).future.share();
    }

private:
    rclcpp::Client<entri_ros2_interfaces::srv::EntriCalc>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto client_node = std::make_shared<EntriCalcClient>();

    // Test server responsiveness before dispatching data frames
    if (!client_node->waitForServer()) {
        rclcpp::shutdown();
        return 1;
    }

    // Call mathematical division test query programmatically: 25.0 / 5.0
    auto future = client_node->sendRequest(25.0, 5.0, "divide");

    // Process asynchronous results halting sequential structure correctly
    if (rclcpp::spin_until_future_complete(client_node, future) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        // Retrieve internal results explicitly utilizing standard getter parameters
        auto response = future.get();
        RCLCPP_INFO(client_node->get_logger(), "Result: %.2f | Success: %s | %s",
            response->result,
            response->success ? "true" : "false",
            response->message.c_str());
    } else {
        RCLCPP_ERROR(client_node->get_logger(), "Service call failed!");
    }

    rclcpp::shutdown();
    return 0;
}
```

**Run the service:**

```bash
# Terminal 1 — Start the server
ros2 run entri_ros2_basics_cpp entri_service_server

# Terminal 2 — Run the client
ros2 run entri_ros2_basics_cpp entri_service_client
```

**Or test via CLI:**

```bash
ros2 service call /entri_calc entri_ros2_interfaces/srv/EntriCalc \
  "{a: 10.0, b: 3.0, operation: 'add'}"
```

---

### 7.4 Service Server and Client Parallel Launch

You can systematically generate a python script to concurrently spin up the endpoints using Launch functionality.

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    """
    Entri Robotics and AI — Service Server and Client Launch File
    
    This launch script demonstrates starting a Service Server and Client together.
    It provisions the server first to ensure the client has an endpoint to connect to.
    """

    # 1. Define the Math Operations Service Server Node
    service_server_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_service_server',
        name='entri_calc_server',
        output='screen'
    )

    # 2. Define the Math Operations Service Client Node
    # Unlike typical Python implementations, we start them concurrently
    # because the client's C++ code uses `wait_for_service()`, holding gracefully.
    service_client_node = Node(
        package='entri_ros2_basics_cpp',
        executable='entri_service_client',
        name='entri_calc_client',
        output='screen'
    )

    # Execute sequence
    return LaunchDescription([
        service_server_node,
        service_client_node
    ])
```

---

### 7.5 Unit Testing with `ament_add_gtest`

The service callback above mixes ROS transport code with business logic. In production, we usually extract the math into a small pure function so we can test it with `colcon test` without spinning a node.

Create `include/entri_ros2_basics_cpp/calc_logic.hpp`:

```cpp
#pragma once

#include <string>

namespace entri_ros2_basics_cpp
{
struct CalcResult
{
    double result;
    bool success;
    std::string message;
};

inline CalcResult perform_operation(double a, double b, const std::string & operation)
{
    if (operation == "add") {
        return {a + b, true, "Addition successful"};
    }
    if (operation == "subtract") {
        return {a - b, true, "Subtraction successful"};
    }
    if (operation == "multiply") {
        return {a * b, true, "Multiplication successful"};
    }
    if (operation == "divide") {
        if (b == 0.0) {
            return {0.0, false, "ERROR: Division by zero!"};
        }
        return {a / b, true, "Division successful"};
    }
    return {0.0, false, "Unknown operation: " + operation};
}
}  // namespace entri_ros2_basics_cpp
```

Now the service callback can simply call `perform_operation(...)` and copy the fields into the ROS response.

Create `test/test_calc_logic.cpp`:

```cpp
#include <gtest/gtest.h>
#include "entri_ros2_basics_cpp/calc_logic.hpp"

TEST(EntriCalcLogic, DivideSuccess)
{
    const auto result = entri_ros2_basics_cpp::perform_operation(25.0, 5.0, "divide");
    EXPECT_TRUE(result.success);
    EXPECT_DOUBLE_EQ(result.result, 5.0);
}

TEST(EntriCalcLogic, DivideByZeroFails)
{
    const auto result = entri_ros2_basics_cpp::perform_operation(25.0, 0.0, "divide");
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.message, "ERROR: Division by zero!");
}
```

Add the test target to `CMakeLists.txt`:

```cmake
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_calc_logic test/test_calc_logic.cpp)
  target_include_directories(test_calc_logic PUBLIC include)
endif()
```

Add the matching test dependency to `package.xml`:

```xml
<test_depend>ament_cmake_gtest</test_depend>
```

Run it from the workspace root:

```bash
colcon test --packages-select entri_ros2_basics_cpp
colcon test-result --all
```

This is the same workflow you will see in real ROS 2 packages: keep the ROS-facing node thin, keep the logic testable, and let `ament_add_gtest` verify the behavior quickly.

---

## 8. Custom Message Types

Similar to custom services, you can define custom **message** types in the interfaces package.

Create `msg/EntriRobotStatus.msg`:

```bash
mkdir -p ~/ros2_ws/src/entri_ros2_interfaces/msg
```

```
# entri_ros2_interfaces/msg/EntriRobotStatus.msg
std_msgs/Header header
string robot_id
float32 battery_percentage
float32 cpu_usage
bool is_operational
geometry_msgs/Pose current_pose
```

**Naming note:**

- `EntriRobotStatus` is the custom ROS 2 message **type name**.
- `entri_robot_status` is the **topic name** used by the publisher.
- `entri_robot_status.hpp` is the generated C++ header name.

In ROS 2, interface type names use `PascalCase`, while topic names usually use lowercase with underscores.

Update `CMakeLists.txt` in the interfaces package:

```cmake
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/EntriRobotStatus.msg"
  "srv/EntriCalc.srv"
  DEPENDENCIES std_msgs geometry_msgs
)
```

Add `package.xml` dependencies:
```xml
<depend>std_msgs</depend>
<depend>geometry_msgs</depend>
```

Use the custom message in a publisher:
### 8.1 Create Custom Message Logic node

```cpp
/**
 * @file custom_msg_srv_node.cpp
 * @brief Explores integrating custom messages natively into operational nodes.
 *
 * This node instantiates a custom publisher that publishes an 
 * `EntriRobotStatus` custom message at exactly 1Hz utilizing diverse datatypes.
 */

#include "rclcpp/rclcpp.hpp"
// Import compiled header from CMake dependency generation targeting custom package!
#include "entri_ros2_interfaces/msg/entri_robot_status.hpp" 

class CustomMessagingNode : public rclcpp::Node
{
public:
    CustomMessagingNode() : Node("custom_message_demonstration") // Register node logic footprint
    {
        // 1. Standup publisher connected to newly forged interface schemas
        publisher_ = this->create_publisher<entri_ros2_interfaces::msg::EntriRobotStatus>(
            "entri_robot_status", 10);

        // 2. Drive repeated timer executions outputting structures
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1000), // 1 Hz fixed rate
            std::bind(&CustomMessagingNode::publishCustomData, this)
        );

        RCLCPP_INFO(this->get_logger(), "Custom messaging publisher spinning!");
    }

private:
    void publishCustomData()
    {
        // Create an explicit datastructure adhering completely to your EntriRobotStatus.msg
        auto status_msg = entri_ros2_interfaces::msg::EntriRobotStatus();
        
        // Populate standard primitive mappings directly translating .msg syntax
        status_msg.header.stamp = this->get_clock()->now();
        status_msg.header.frame_id = "base_link";
        
        status_msg.robot_id = "entri_alpha_bot";
        status_msg.battery_percentage = 85.3f;
        status_msg.cpu_usage = 23.4f;
        status_msg.is_operational = true;
        
        // Pushing geometry component mapping (x,y,z spatial awareness geometry_msgs)
        status_msg.current_pose.position.x = 10.5;
        status_msg.current_pose.position.y = -5.0;
        status_msg.current_pose.position.z = 0.0;
        
        // Calculate quaternion (assuming fixed 0 rotation baseline)
        status_msg.current_pose.orientation.w = 1.0; 

        // Announce locally on terminal visually inspecting transmission
        RCLCPP_INFO(this->get_logger(), "Sending custom diagnostics for: %s | Batt: %.1f%%",
                    status_msg.robot_id.c_str(), status_msg.battery_percentage);

        // Submit the complex message across the standard communication framework safely
        publisher_->publish(status_msg);
    }

    rclcpp::Publisher<entri_ros2_interfaces::msg::EntriRobotStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CustomMessagingNode>());
    rclcpp::shutdown();
    return 0;
}
```

### 8.2 Custom Application Launch

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    """
    Entri Robotics and AI — Custom Message Display Launch File
    """

    custom_msg_node = Node(
        package='entri_ros2_basics_cpp',
        executable='custom_msg_srv_node',
        name='custom_msg_demonstrator',
        output='screen'
    )

    return LaunchDescription([
        custom_msg_node
    ])
```

---

## 9. Actions in C++

**Actions** are used for long-running tasks that need three things at once:

- A goal request from the client
- Periodic feedback while the task is running
- A final result when the task finishes

Typical robotics examples include navigation, docking, arm motion, and waypoint following.

### 9.1 Action Interface Definition

Create an action definition file:

```bash
mkdir -p ~/ros2_ws/src/entri_ros2_interfaces/action
```

```action
# entri_ros2_interfaces/action/MoveDistance.action
# Goal
float32 target_distance
float32 speed
---
# Result
bool success
float32 final_distance
string message
---
# Feedback
float32 current_distance
float32 percent_complete
```

Update the interfaces package `CMakeLists.txt`:

```cmake
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/EntriRobotStatus.msg"
  "srv/EntriCalc.srv"
  "action/MoveDistance.action"
  DEPENDENCIES std_msgs geometry_msgs
)
```

Build and inspect it:

```bash
colcon build --packages-select entri_ros2_interfaces
source install/setup.bash
ros2 interface show entri_ros2_interfaces/action/MoveDistance
```

### 9.2 C++ Action Server

Below is a compact example of a custom action server. This is an example for the notes; the current workspace does not include these files yet.

```cpp
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "entri_ros2_interfaces/action/move_distance.hpp"
#include <chrono>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

class MoveDistanceServer : public rclcpp::Node
{
public:
    using MoveDistance = entri_ros2_interfaces::action::MoveDistance;
    using GoalHandleMoveDistance = rclcpp_action::ServerGoalHandle<MoveDistance>;

    MoveDistanceServer() : Node("move_distance_server")
    {
        action_server_ = rclcpp_action::create_server<MoveDistance>(
            this,
            "move_distance",
            std::bind(&MoveDistanceServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&MoveDistanceServer::handle_cancel, this, std::placeholders::_1),
            std::bind(&MoveDistanceServer::handle_accepted, this, std::placeholders::_1)
        );
    }

private:
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &,
        std::shared_ptr<const MoveDistance::Goal> goal)
    {
        if (goal->target_distance <= 0.0f || goal->speed <= 0.0f) {
            RCLCPP_WARN(this->get_logger(), "Rejecting invalid goal");
            return rclcpp_action::GoalResponse::REJECT;
        }
        RCLCPP_INFO(this->get_logger(), "Accepted goal: %.2f m at %.2f m/s",
            goal->target_distance, goal->speed);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "Cancel request received");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        // NOTE: detach() is used here only to keep this notes example short.
        // In production, store the worker thread and join it in the destructor,
        // or use a callback group with a MultiThreadedExecutor so `this`
        // cannot disappear while execute() is still running.
        std::thread{std::bind(&MoveDistanceServer::execute, this, goal_handle)}.detach();
    }

    void execute(const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        const auto goal = goal_handle->get_goal();
        auto feedback = std::make_shared<MoveDistance::Feedback>();
        auto result = std::make_shared<MoveDistance::Result>();

        const int steps = 10;
        for (int i = 1; i <= steps; ++i) {
            if (goal_handle->is_canceling()) {
                result->success = false;
                result->final_distance = feedback->current_distance;
                result->message = "Goal canceled";
                goal_handle->canceled(result);
                return;
            }

            feedback->current_distance = goal->target_distance * static_cast<float>(i) / steps;
            feedback->percent_complete = 100.0f * static_cast<float>(i) / steps;
            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "Progress: %.1f%%", feedback->percent_complete);
            std::this_thread::sleep_for(500ms);
        }

        result->success = true;
        result->final_distance = goal->target_distance;
        result->message = "Target distance reached";
        goal_handle->succeed(result);
    }

    rclcpp_action::Server<MoveDistance>::SharedPtr action_server_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveDistanceServer>());
    rclcpp::shutdown();
    return 0;
}
```

### 9.3 C++ Action Client

```cpp
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "entri_ros2_interfaces/action/move_distance.hpp"
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class MoveDistanceClient : public rclcpp::Node
{
public:
    using MoveDistance = entri_ros2_interfaces::action::MoveDistance;
    using GoalHandleMoveDistance = rclcpp_action::ClientGoalHandle<MoveDistance>;

    MoveDistanceClient() : Node("move_distance_client")
    {
        client_ = rclcpp_action::create_client<MoveDistance>(this, "move_distance");
    }

    void send_goal(float target_distance, float speed)
    {
        if (!client_->wait_for_action_server(5s)) {
            RCLCPP_ERROR(this->get_logger(), "MoveDistance action server not available.");
            rclcpp::shutdown();
            return;
        }

        MoveDistance::Goal goal_msg;
        goal_msg.target_distance = target_distance;
        goal_msg.speed = speed;

        auto options = rclcpp_action::Client<MoveDistance>::SendGoalOptions();
        options.goal_response_callback =
            [this](GoalHandleMoveDistance::SharedPtr goal_handle) {
                if (!goal_handle) {
                    RCLCPP_ERROR(this->get_logger(), "Goal was rejected by the server.");
                } else {
                    RCLCPP_INFO(this->get_logger(), "Goal accepted by the server.");
                }
            };

        options.feedback_callback =
            [this](GoalHandleMoveDistance::SharedPtr,
                   const std::shared_ptr<const MoveDistance::Feedback> feedback) {
                RCLCPP_INFO(this->get_logger(), "Distance: %.2f m | Progress: %.1f%%",
                    feedback->current_distance,
                    feedback->percent_complete);
            };

        options.result_callback =
            [this](const GoalHandleMoveDistance::WrappedResult & result) {
                switch (result.code) {
                    case rclcpp_action::ResultCode::SUCCEEDED:
                        RCLCPP_INFO(this->get_logger(), "Action succeeded: %.2f m | %s",
                            result.result->final_distance,
                            result.result->message.c_str());
                        break;
                    case rclcpp_action::ResultCode::ABORTED:
                        RCLCPP_ERROR(this->get_logger(), "Action aborted.");
                        break;
                    case rclcpp_action::ResultCode::CANCELED:
                        RCLCPP_WARN(this->get_logger(), "Action canceled.");
                        break;
                    default:
                        RCLCPP_ERROR(this->get_logger(), "Unknown action result code.");
                        break;
                }
                rclcpp::shutdown();
            };

        client_->async_send_goal(goal_msg, options);
    }

private:
    rclcpp_action::Client<MoveDistance>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveDistanceClient>();
    node->send_goal(2.5f, 0.5f);
    rclcpp::spin(node);
    return 0;
}
```

Build requirements for action-enabled C++ nodes:

```cmake
find_package(rclcpp_action REQUIRED)
find_package(entri_ros2_interfaces REQUIRED)

add_executable(move_distance_server src/move_distance_server.cpp)
ament_target_dependencies(move_distance_server rclcpp rclcpp_action entri_ros2_interfaces)

add_executable(move_distance_client src/move_distance_client.cpp)
ament_target_dependencies(move_distance_client rclcpp rclcpp_action entri_ros2_interfaces)
```

Useful CLI commands:

```bash
ros2 action list
ros2 action info /move_distance
ros2 action send_goal /move_distance entri_ros2_interfaces/action/MoveDistance \
  "{target_distance: 3.0, speed: 0.6}"
```

### 9.4 Lifecycle Nodes in Production Systems

`rclcpp_lifecycle::LifecycleNode` is common in production robotics stacks, especially Nav2 and many larger bringup pipelines. Instead of jumping straight from construction to active behavior, a lifecycle node moves through managed states such as `unconfigured`, `inactive`, and `active`.

#### What Is a Lifecycle Node?

A lifecycle node is a ROS 2 node with an explicit state machine. Instead of "construct the node and immediately do everything," the node exposes controlled transition points such as:

- `on_configure()` for loading parameters, allocating memory, opening drivers, or creating publishers/subscribers
- `on_activate()` for actually starting outputs that affect the rest of the system
- `on_deactivate()` for stopping those outputs cleanly
- `on_cleanup()` for releasing resources and returning to a safe pre-configured state
- `on_shutdown()` for final shutdown handling

This gives the rest of the system a predictable contract: the node exists, but it is not necessarily ready or active yet.

#### Why Do Production Systems Use Lifecycle Nodes?

On a real robot, "node process is running" is not the same thing as "this part of the robot is safe and ready."

Examples:

- a camera driver may need to open the device and validate calibration before publishing images
- a motor controller should not send commands until the robot is fully initialized
- a localization or navigation stack may need maps, transforms, and parameters loaded before activation

Lifecycle nodes solve this by separating phases:

- create the node
- configure resources
- activate behavior
- deactivate safely if something upstream fails
- clean up and recover without restarting the whole process

That control model is a big reason Nav2 relies so heavily on lifecycle-managed nodes.

#### How Does the Lifecycle Flow Work?

At a high level, the lifecycle is usually:

```text
unconfigured
   -> configure
inactive
   -> activate
active
   -> deactivate
inactive
   -> cleanup
unconfigured
```

Typical interpretation:

- `unconfigured`: the node exists, but hardware/resources are not ready
- `inactive`: configuration succeeded, but publishers/outputs are intentionally not active
- `active`: the node is fully participating in the robot system

This is especially useful during bringup because a launch system or supervisor can decide exactly when each subsystem is allowed to become active.

#### Why Does This Feel Different from ROS 1?

In ROS 1, most nodes behaved like plain processes:

- start the node
- initialize everything in the constructor or `main()`
- begin publishing/subscribing immediately
- rely on launch ordering, sleeps, or ad-hoc readiness checks

That worked for many systems, but it often blurred together:

- process lifetime
- hardware readiness
- communication readiness
- safety readiness

ROS 2 lifecycle nodes make those stages explicit. This is one of the conceptual shifts from ROS 1 to ROS 2: more emphasis on managed startup, deterministic state transitions, and production-grade orchestration rather than "launch everything and hope each node is ready in time."

Why teams use it:

- initialize hardware and parameters in `on_configure()`
- start publishers, timers, or motion outputs only in `on_activate()`
- stop safely in `on_deactivate()` and `on_cleanup()`
- make launch systems responsible for explicit state transitions

Minimal example:

```cpp
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "std_msgs/msg/string.hpp"

class ManagedPublisher : public rclcpp_lifecycle::LifecycleNode
{
public:
    using CallbackReturn =
        rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

    ManagedPublisher() : rclcpp_lifecycle::LifecycleNode("managed_publisher")
    {
    }

    CallbackReturn on_configure(const rclcpp_lifecycle::State &)
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("managed_topic", 10);
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn on_activate(const rclcpp_lifecycle::State &)
    {
        publisher_->on_activate();
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &)
    {
        publisher_->on_deactivate();
        return CallbackReturn::SUCCESS;
    }

private:
    rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr publisher_;
};
```

Typical CLI flow:

```bash
ros2 lifecycle get /managed_publisher
ros2 lifecycle set /managed_publisher configure
ros2 lifecycle set /managed_publisher activate
```

In practice, lifecycle nodes are often driven by a launch file, lifecycle manager, or higher-level bringup system rather than by manually typing these commands each time.

If students later open Nav2 or MoveIt2 bringup code, this pattern is one of the first major differences they will notice compared with beginner `rclcpp::Node` examples.

---

## 10. rosbag2 — Data Recording and Replay

`rosbag2` records ROS 2 topic traffic so you can replay real robot behavior later. This is extremely useful for debugging sensors, validating subscriber logic, and reproducing bugs.

```bash
# Record all currently active topics
ros2 bag record -a

# Record only a few topics
ros2 bag record /entri/topic /entri_robot_status

# Save to a named bag directory
ros2 bag record -o diagnostics_run /entri_robot_status

# Show metadata about the recording
ros2 bag info diagnostics_run

# Replay the captured messages
ros2 bag play diagnostics_run
```

Common workflow:

1. Start your publishers on the robot or in simulation.
2. Record the important topics with `ros2 bag record`.
3. Stop the system after the issue appears.
4. Replay the data offline and test your subscriber or algorithm repeatedly.

---

## 11. TurtleSim Practice Session

TurtleSim is a great sandbox for practicing topics, services, and parameters.

**Install if needed:**
```bash
sudo apt install ros-jazzy-turtlesim
```

**Start turtlesim:**
```bash
# Terminal 1
ros2 run turtlesim turtlesim_node

# Terminal 2
ros2 run turtlesim turtle_teleop_key

# Terminal 3
rqt
```

**Explore topics:**
```bash
ros2 topic list
ros2 topic info /turtle1/cmd_vel
ros2 topic echo /turtle1/pose
```

**Explore and use services:**
```bash
ros2 service list
ros2 service call /turtle1/set_pen turtlesim/srv/SetPen \
  "{r: 255, g: 0, b: 0, width: 3, 'off': 0}"
ros2 service call /clear std_srvs/srv/Empty
```

**Explore and change parameters:**
```bash
ros2 param list /turtlesim
ros2 param get /turtlesim background_r
ros2 param set /turtlesim background_r 100
ros2 param set /turtlesim background_g 200
```

> **Entri Exercise:** Write a C++ node that subscribes to `/turtle1/pose` and publishes movement commands to `/turtle1/cmd_vel` to make the turtle move in a circle. Hint: use `geometry_msgs/msg/Twist`.

---

## 12. CMakeLists.txt Reference

Here is a complete, annotated `CMakeLists.txt` for an Entri Robotics and AI C++ package:

```cmake
cmake_minimum_required(VERSION 3.8)
project(entri_ros2_basics_cpp)

# Use C++17 standard
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(entri_ros2_interfaces REQUIRED)


add_executable(hello_world src/hello_world.cpp)
ament_target_dependencies(hello_world rclcpp)

add_executable(entri_publisher src/publisher.cpp)
ament_target_dependencies(entri_publisher rclcpp std_msgs)

add_executable(entri_subscriber src/subscriber.cpp)
ament_target_dependencies(entri_subscriber rclcpp std_msgs)

add_executable(entri_param_node src/param_node.cpp)
ament_target_dependencies(entri_param_node rclcpp)

add_executable(entri_service_server src/service_server.cpp)
ament_target_dependencies(entri_service_server rclcpp entri_ros2_interfaces)

add_executable(entri_service_client src/service_client.cpp)
ament_target_dependencies(entri_service_client rclcpp entri_ros2_interfaces)

add_executable(param_node_dynamic src/param_node_dynamic.cpp)
ament_target_dependencies(param_node_dynamic rclcpp)

add_executable(param_node_yaml src/param_node_yaml.cpp)
ament_target_dependencies(param_node_yaml rclcpp)

add_executable(custom_msg_srv_node src/custom_msg_srv_node.cpp)
ament_target_dependencies(custom_msg_srv_node rclcpp entri_ros2_interfaces)

add_executable(move_distance_server src/move_distance_server.cpp)
ament_target_dependencies(move_distance_server rclcpp rclcpp_action entri_ros2_interfaces)

add_executable(move_distance_client src/move_distance_client.cpp)
ament_target_dependencies(move_distance_client rclcpp rclcpp_action entri_ros2_interfaces)

install(TARGETS
  hello_world
  entri_publisher
  entri_subscriber
  entri_param_node
  entri_service_server
  entri_service_client
  param_node_dynamic
  param_node_yaml
  custom_msg_srv_node
  move_distance_server
  move_distance_client
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

ament_package()
```

---

## 13. Quick Reference Tables

### 13.1 Useful Linux Commands

| Command | Description |
|---|---|
| `sudo apt update` | Update local package index from repositories |
| `sudo apt upgrade` | Install updated package versions |
| `sudo apt install <pkg>` | Install a package |
| `ls -la` | List directory contents with details |
| `cd ~/ros2_ws` | Change to ROS2 workspace directory |
| `mkdir -p <path>` | Create directories recursively |
| `rm -rf <path>` | Remove files/directories (use with caution!) |
| `touch <filename>` | Create a new empty file |
| `chmod +x <file>` | Make a file executable |
| `tree -L 3` | Show directory tree (3 levels deep) |
| `nano <file>` | Edit a file in terminal |
| `grep -r "pattern" .` | Search for text in files |
| `which ros2` | Find the location of an executable |
| `echo $ROS_DISTRO` | Print current ROS2 distribution |

---

### 13.2 Useful ROS2 CLI Commands

| Command | Description |
|---|---|
| `ros2 node list` | List all active nodes |
| `ros2 node info /node_name` | Show publishers, subscribers, services of a node |
| `ros2 topic list` | List all active topics |
| `ros2 topic echo /topic` | Print messages from a topic in the terminal |
| `ros2 topic info /topic` | Show type and connection count of a topic |
| `ros2 topic pub /topic <type> <data>` | Manually publish a message to a topic |
| `ros2 topic hz /topic` | Measure publish frequency of a topic |
| `ros2 service list` | List all active services |
| `ros2 service call /srv <type> <data>` | Call a service manually |
| `ros2 param list /node` | List parameters of a node |
| `ros2 param get /node <param>` | Get a parameter value |
| `ros2 param set /node <param> <val>` | Set a parameter value at runtime |
| `ros2 interface show <type>` | Show the definition of a message/service |
| `ros2 pkg create` | Create a new ROS2 package |
| `ros2 run <pkg> <node>` | Run a node from a package |
| `ros2 launch <pkg> <file.launch.py>` | Run a launch file |
| `ros2 bag record -a` | Record all active topics into a rosbag2 dataset |
| `ros2 bag record /topic1 /topic2` | Record only selected topics |
| `ros2 bag info <bag_dir>` | Inspect metadata about a recorded bag |
| `ros2 bag play <bag_dir>` | Replay recorded topic data |
| `colcon build` | Build entire workspace |
| `colcon build --packages-select <pkg>` | Build a single package |
| `source install/setup.bash` | Source the workspace environment |
| `ros2 doctor` | **(Diagnostic)** Validates your system, OS, and network for ROS 2 compatibility |
| `ros2 wtf` | An exact alias to `ros2 doctor`, used playfully when your nodes mysteriously fail! |
| `rqt` | Open the ROS2 graphical tool dashboard |
| `rqt_graph` | Visualize node/topic connections |

---

### 13.3 rclcpp API Cheatsheet

| Operation | C++ rclcpp Code |
|---|---|
| Initialize ROS2 | `rclcpp::init(argc, argv);` |
| Create a node | `auto node = std::make_shared<MyNode>();` |
| Log info | `RCLCPP_INFO(this->get_logger(), "msg %s", var);` |
| Log warning | `RCLCPP_WARN(this->get_logger(), "warn");` |
| Log error | `RCLCPP_ERROR(this->get_logger(), "error");` |
| Create publisher | `this->create_publisher<MsgType>("topic", qos);` |
| Publish message | `publisher_->publish(msg);` |
| Create subscription | `this->create_subscription<MsgType>("topic", qos, callback);` |
| Preferred sub callback | `void callback(const MsgType::SharedPtr & msg);` or `void callback(std::shared_ptr<const MsgType> msg);` |
| Create timer | `this->create_wall_timer(period, callback);` |
| Declare parameter | `this->declare_parameter<Type>("name", default);` |
| Get parameter | `this->get_parameter("name").as_string();` |
| Create service server | `this->create_service<SrvType>("name", callback);` |
| Create service client | `this->create_client<SrvType>("name");` |
| Spin infinite loop | `rclcpp::spin(node);` |
| Spin once (step) | `rclcpp::spin_once(node);` (Executes only one callback cycle, used heavily in custom loops) |
| Spin until future | `rclcpp::spin_until_future_complete(node, future);` (Pauses thread sequentially until service client future resolves) |
| Shutdown | `rclcpp::shutdown();` |

---

### 13.4 rosbag2 Quick Commands

`rosbag2` is ROS 2's built-in tool for recording and replaying topic traffic. It is one of the most useful debugging tools in robotics because it lets you reproduce system behavior without the robot actively running.

```bash
# Record everything currently publishing
ros2 bag record -a

# Record only one topic
ros2 bag record /entri/topic

# Save to a custom folder name
ros2 bag record -o pubsub_debug /entri/topic

# Inspect what is inside a bag
ros2 bag info pubsub_debug

# Replay the captured data
ros2 bag play pubsub_debug
```

Common uses:

- Capture sensor streams during a bug
- Replay data offline while testing a subscriber or algorithm
- Compare behavior before and after a code change

---

## Final Workspace Structure

After completing all modules, your workspace should look like this:

```
ros2_ws/
└── src/
    ├── entri_ros2_interfaces/
    │   ├── CMakeLists.txt
    │   ├── package.xml
    │   ├── action/
    │   │   └── MoveDistance.action
    │   ├── msg/
    │   │   └── EntriRobotStatus.msg
    │   └── srv/
    │       └── EntriCalc.srv
    │
    └── entri_ros2_basics_cpp/
        ├── CMakeLists.txt
        ├── package.xml
        ├── config/
        │   └── entri_params.yaml
        ├── launch/
        │   ├── calc_service.launch.py
        │   ├── custom_msg_srv.launch.py
        │   ├── entri_params.launch.py
        │   ├── move_distance_action.launch.py
        │   └── pub_sub_remap.launch.py
        └── src/
            ├── hello_world.cpp
            ├── move_distance_server.cpp
            ├── move_distance_client.cpp
            ├── publisher.cpp
            ├── subscriber.cpp
            ├── param_node.cpp
            ├── param_node_yaml.cpp
            ├── param_node_dynamic.cpp
            ├── service_server.cpp
            ├── service_client.cpp
            └── custom_msg_srv_node.cpp
```

---


*Prepared for **Entri Robotics and AI** course — ROS2 C++ Track*  
*Distribution: ROS2 Jazzy Jalisco | Ubuntu 24.04 LTS*
