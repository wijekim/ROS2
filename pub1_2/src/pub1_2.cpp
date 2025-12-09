#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("node_pub1_2");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    // 퍼블리셔 생성
    auto mypub = node->create_publisher<geometry_msgs::msg::Vector3>("topic_pub1_2", qos_profile);

    rclcpp::WallRate loop_rate(1.0); // 1Hz

    geometry_msgs::msg::Vector3 msg;

    std::cout << "x, y, z 값을 입력하세요 : ";
    std::cin >> msg.x >> msg.y >> msg.z;
    while (rclcpp::ok())
    {
        

        RCLCPP_INFO(node->get_logger(), "Publish: x=%.2f, y=%.2f, z=%.2f",msg.x, msg.y, msg.z);

        mypub->publish(msg);
        // rclcpp::spin_some(node); // 필요 시 활성화
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
