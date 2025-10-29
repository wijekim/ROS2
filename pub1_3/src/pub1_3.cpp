#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>("cmd_vel_publisher");
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    geometry_msgs::msg::Twist msg;
    char input;

    RCLCPP_INFO(node->get_logger(), "키보드 입력 (f: 전진, b: 후진, l: 좌회전, r: 우회전, q: 종료)");

    while (rclcpp::ok())
    {
        std::cout << "입력: ";
        std::cin >> input;

        // 기본 속도 초기화
        msg.linear.x = 0.0;
        msg.angular.z = 0.0;

        if (input == 'f') {          // forward
            msg.linear.x = 2.0;
            RCLCPP_INFO(node->get_logger(), "전진 중...");
        }
        else if (input == 'b') {     // backward
            msg.linear.x = -2.0;
            RCLCPP_INFO(node->get_logger(), "후진 중...");
        }
        else if (input == 'l') {     // turn left
            msg.angular.z = 2.0;
            RCLCPP_INFO(node->get_logger(), "좌회전 중...");
        }
        else if (input == 'r') {     // turn right
            msg.angular.z = -2.0;
            RCLCPP_INFO(node->get_logger(), "우회전 중...");
        }
        else if (input == 'q') {     // quit
            RCLCPP_INFO(node->get_logger(), "프로그램 종료.");
            break;
        }
        else {
            RCLCPP_WARN(node->get_logger(), "잘못된 입력입니다. (f/b/l/r/q 중 하나 입력)");
            continue;
        }

        publisher->publish(msg);
    }

    rclcpp::shutdown();
    return 0;
}
