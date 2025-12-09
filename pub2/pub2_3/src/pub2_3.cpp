#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <memory>
#include <chrono>
#include <functional>
//#include <string>
using namespace std::chrono_literals;
void callback(rclcpp::Node::SharedPtr node, rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr mypub)
{
    geometry_msgs::msg::Twist msg;
    
    char input;
    RCLCPP_INFO(node->get_logger(), "키보드 입력 (f: 전진, b: 후진, l: 좌회전, r: 우회전, q: 종료)");
    
    std::cout << "입력: ";
    std::cin >> input;

    msg.linear.x = 0.0;
    msg.angular.z = 0.0;

    if (input == 'f') {          
        msg.linear.x = 2.0;
        msg.angular.z = 0.0;
        RCLCPP_INFO(node->get_logger(), "전진 중...");
    }
    else if (input == 'b') {     
        msg.linear.x = -2.0;
        msg.angular.z = 0.0;
        RCLCPP_INFO(node->get_logger(), "후진 중...");
    }
    else if (input == 'l') {     
        msg.angular.z = 2.0;
        msg.linear.x = 0.0;
        RCLCPP_INFO(node->get_logger(), "좌회전 중...");
    }
    else if (input == 'r') {     
        msg.angular.z = -2.0;
        msg.linear.x = 0.0;
        RCLCPP_INFO(node->get_logger(), "우회전 중...");
    }
    else if (input == 'q') {     
        RCLCPP_INFO(node->get_logger(), "프로그램 종료.");
        rclcpp::shutdown();
        return;
    }
    else {
        RCLCPP_WARN(node->get_logger(), "잘못된 입력입니다. (f/b/l/r/q 중 하나 입력)");
        return;
    }
    
    mypub->publish(msg);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("mynode2_3");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", qos_profile);

    std::function<void()> fn = std::bind(callback, node, pub);
    auto timer = node->create_wall_timer(100ms, fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}