#include <chrono>
#include <cstdlib>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include"entri_ros2_srvcli_cpp/srv/robot_task.hpp"
// TODO: Include necessary headers for rclcpp and entri_ros2_srvcli_cpp/srv/robot_task.hpp

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  // TODO: Initialize rclcpp
  rclcpp::init(argc, argv);

  // TODO: Create a node named "task_trigger_client"
  // std::shared_ptr<rclcpp::Node> node = ...
  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("task_trigger_client");
  

  // TODO: Create a client hooked to the "allocate_robot_task" service
  // rclcpp::Client<entri_ros2_srvcli_cpp::srv::RobotTask>::SharedPtr client = ...
  rclcpp::Client<entri_ros2_srvcli_cpp::srv::RobotTask>::SharedPtr client = node->create_client<entri_ros2_srvcli_cpp::srv::RobotTask>("allocate_robot_task");  

  // TODO: Create a RobotTask request object
  // auto request = std::make_shared<...>();
  auto request = std::make_shared<entri_ros2_srvcli_cpp::srv::RobotTask::Request>();
  
  // TODO: Set request fields: task_id = 42, task_name = "Move_To_A"
  // request->task_id = 42;
  request->task_id = 42;
  // request->task_name = "Move_To_A";
  request->task_name = "Move_To_A";

  // TODO: Wait for the service to be available (use a loop with client->wait_for_service(1s))

  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }


  // TODO: Send the exact asynchronous request
  auto result = client->async_send_request(request);

      rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    if (executor.spin_until_future_complete(result, 5s)
        == rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = result.get();
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
            "Task allocation status: %s. Response: %s",
            response->success ? "True" : "False",
            response->task_response.c_str());
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
            "Failed to call service allocate_robot_task");
    }

    rclcpp::shutdown();
    return 0;
}
