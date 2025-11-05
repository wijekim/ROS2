#include "rclcpp/rclcpp.hpp"        
#include "std_msgs/msg/int32.hpp"   
#include "rclcpp/time_source.hpp"
#include <memory>                   
#include <chrono>                   
#include <functional>               

using namespace std::chrono_literals;  


void callback(rclcpp::Node::SharedPtr node, rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr mypub)
{
    static int cnt =0;
    auto message =std_msgs::msg::Int32();
    message.data=cnt;
    RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data); 

    mypub->publish(message);
    cnt++;   
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);   
    auto node = std::make_shared<rclcpp::Node>("node_pub1_2");    
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));       
    auto mypub = node->create_publisher<std_msgs::msg::Int32 >("topic_pub1_2",qos_profile);
    
    
    std::function<void()>fn=std::bind(callback,node,mypub);
    auto timer=node->create_wall_timer(50ms,fn);  
    rclcpp::spin(node);  
    rclcpp::shutdown();  
    return 0;
}