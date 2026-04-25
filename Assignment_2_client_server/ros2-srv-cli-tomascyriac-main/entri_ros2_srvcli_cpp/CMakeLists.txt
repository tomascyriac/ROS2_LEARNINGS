cmake_minimum_required(VERSION 3.8)
project(entri_ros2_srvcli_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# TODO: Find the required packages 'rclcpp' and 'rosidl_default_generators'
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)


# TODO: Generate the custom interface srv/RobotTask.srv
#       Make sure to depend on 'rosidl_default_generators'
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/RobotTask.srv"
  DEPENDENCIES 
)

# TODO: Create the executable for 'service_node' from 'src/service_node.cpp'
# TODO: Add 'rclcpp' as a dependency for 'service_node'
# TODO: Call rosidl_target_interfaces for service_node to link the custom service target
add_executable(service_node src/service_node.cpp)
ament_target_dependencies(service_node rclcpp)
rosidl_target_interfaces(service_node ${PROJECT_NAME} "rosidl_typesupport_cpp")

# TODO: Create the executable for 'client_node' from 'src/client_node.cpp'
# TODO: Add 'rclcpp' as a dependency for 'client_node'
# TODO: Call rosidl_target_interfaces for client_node to link the custom service target
add_executable(client_node src/client_node.cpp)
ament_target_dependencies(client_node rclcpp)
rosidl_target_interfaces(client_node ${PROJECT_NAME} "rosidl_typesupport_cpp")


# Install targets
install(TARGETS
  # TODO: Add service_node and client_node here
  service_node
  client_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
