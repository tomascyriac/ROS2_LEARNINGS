#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp" // Include the header for rclcpp to use ROS2 functionalities
// Include the header for the RobotStatus message type defined in the entri_ros2_pubsub_cpp package
#include "entri_ros2_pubsub_cpp/msg/robot_status.hpp" 

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), battery_level_(100.0)
    {
      // TODO: Create a publisher for entri_ros2_pubsub_cpp::msg::RobotStatus on topic "robot_status"
      // publisher_ = ...

      publisher_ = this->create_publisher<entri_ros2_pubsub_cpp::msg::RobotStatus>("robot_status", 12);


      // TODO: Create a wall timer that triggers the timer_callback every 500ms
      // timer_ = ...
      timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      // TODO: Create a RobotStatus message
      // auto message = ...

      auto message = entri_ros2_pubsub_cpp::msg::RobotStatus();

      // TODO: Set message fields:
      // message.robot_id = 12
      // message.battery_level = battery_level_
      // message.status_message = "Operating"

      message.robot_id = 12;
      message.battery_level = battery_level_;
      message.status_message = "Operating";

      // TODO: Print the message being published using RCLCPP_INFO
      // RCLCPP_INFO(...);

      if (battery_level_ <= 0.0f) {
        battery_level_ = 100.0; // Reset battery level for continuous simulation
        message.status_message = "Restoring Battery";
      }

      RCLCPP_INFO(this->get_logger(), "Publishing: Robot %d is at %.1f%% battery. Status: %s", 
                  message.robot_id, message.battery_level, message.status_message.c_str());

      // TODO: Publish the message
      // publisher_->publish(...);
      publisher_->publish(message);

      battery_level_ -= 0.5; // Simulate battery drain
    }

    // TODO: Declare the timer_ and publisher_ member variables
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<entri_ros2_pubsub_cpp::msg::RobotStatus>::SharedPtr publisher_;
    float battery_level_;
  };

int main(int argc, char * argv[])
{
  // TODO: Initialize rclcpp
  rclcpp::init(argc, argv);
  
  // TODO: spin the node to keep it running
  // rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::spin(std::make_shared<MinimalPublisher>());

  // TODO: shutdown rclcpp
  rclcpp::shutdown();
  return 0;
}
