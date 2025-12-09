#include "rclcpp/rclcpp.hpp"        
#include "std_msgs/msg/int32.hpp"   
#include <memory>                   
#include <chrono>                   
#include <functional>               

using namespace std::chrono_literals;  

// callback 함수
void mysub_callback(rclcpp::Node::SharedPtr node, const std_msgs::msg::Int32::SharedPtr msg)
{
    RCLCPP_INFO(node->get_logger(), "Received: %d", msg->data);  // 수신된 메시지 출력
}

// main 함수
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);   
    auto node = std::make_shared<rclcpp::Node>("node_sub1_2");  // 노드 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));      
    
    std::function<void(const std_msgs::msg::Int32::SharedPtr)> fn = std::bind(mysub_callback, node, std::placeholders::_1);
    auto mysub = node->create_subscription<std_msgs::msg::Int32>("topic_pub1_2", qos_profile, fn);

    rclcpp::spin(node);    
    rclcpp::shutdown();   
    return 0;
}
