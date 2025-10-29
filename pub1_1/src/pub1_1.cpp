#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <memory>
#include <chrono>
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("node_pub1");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    auto mypub = node->create_publisher<std_msgs::msg::Int32>("topic_pub1",
    qos_profile );
    std_msgs::msg::Int32 message;
    message.data = 0;
    rclcpp::WallRate loop_rate(1.0);
    while(rclcpp::ok())
    {
    RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data);
    mypub->publish(message);
    message.data++;
    //rclcpp::spin_some(node);
    loop_rate.sleep(); //반복주파수에서 남은 시간 만큼 sleep
    }
    rclcpp::shutdown();
    return 0;
}