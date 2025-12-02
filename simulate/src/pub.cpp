#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"
#include <memory>
#include <chrono>
#include <string>


std::string video_path = "/home/linux/ros2_ws/simulation/5_lt_cw_100rpm_out.mp4"; 

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("simul_pub");
    
    
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
    auto mypub = node->create_publisher<sensor_msgs::msg::CompressedImage>("image/compressed", qos_profile);
    
    sensor_msgs::msg::CompressedImage::SharedPtr msg;
    
   
    cv::VideoCapture cap(video_path);

    if (!cap.isOpened()) {
        RCLCPP_ERROR(node->get_logger(), "Could not open video file: %s", video_path.c_str());
        rclcpp::shutdown();
        return -1;
    }


    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0.0) {
        fps = 30.0; 
    }
    RCLCPP_INFO(node->get_logger(), "Video FPS: %.2f", fps);

    
    rclcpp::WallRate loop_rate(fps);

    cv::Mat frame;
    std_msgs::msg::Header hdr;

    while(rclcpp::ok())
    {
        cap >> frame;
        
        // 영상이 끝나면 종료 (무한 반복하려면 주석 참고)
        if (frame.empty()) { 
            RCLCPP_INFO(node->get_logger(), "End of video.");
            break;
            
            // 만약 무한 반복하고 싶다면 break 대신 아래 코드 사용
            // cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            // continue;
        }

       
        hdr.stamp = node->now();
        hdr.frame_id = "camera_frame"; 

      
        msg = cv_bridge::CvImage(hdr, "bgr8", frame).toCompressedImageMsg();
        
        mypub->publish(*msg);
        
        
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}