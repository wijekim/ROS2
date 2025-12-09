/*#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>
using std::placeholders::_1;
  
void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    cv::imshow("wsl",frame);
    cv::waitKey(1);
    RCLCPP_INFO(node->get_logger(), "Received Image : %s,%d,%d", msg->format.c_str(),frame.rows,frame.cols);
}
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camsub_wsl11");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
    //auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort(); //UDP
    std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, _1);
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed11",qos_profile,fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
*/



#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>

using std::placeholders::_1;
  
void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    // 1. 수신된 압축 이미지 데이터를 OpenCV Mat으로 디코딩하여 원본 컬러 영상(frame) 생성
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);

    // 원본 영상의 유효성 확인
    if (frame.empty()) {
        RCLCPP_ERROR(node->get_logger(), "Failed to decode image.");
        return;
    }

    static cv::VideoWriter writer;
    static bool is_writer_initialized = false;

    if (!is_writer_initialized) {
        // 파일명, 코덱(MJPG), FPS(30), 해상도, 컬러여부 설정
        // wsl 환경 등에서는 'MJPG' 코덱과 .avi 확장자가 가장 호환성이 좋습니다.
        std::string filename = "output_video.avi";
        int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
        double fps = 20.0; 
        
        writer.open(filename, codec, fps, frame.size(), true);

        if (writer.isOpened()) {
            RCLCPP_INFO(node->get_logger(), "Video recording started: %s", filename.c_str());
            is_writer_initialized = true;
        } else {
            RCLCPP_ERROR(node->get_logger(), "Failed to open video writer.");
        }
    }

    // 초기화가 성공했다면 현재 프레임을 파일에 씀
    if (is_writer_initialized) {
        writer.write(frame);
    }

    // 2. 원본 영상을 그레이스케일로 변환
    //cv::Mat gray_frame;
    //cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

    // 3. 그레이스케일 영상을 이진 영상으로 변환 
    //cv::Mat binary_frame;
    // 임계값 100을 기준으로, 100보다 크면 255(흰색), 작거나 같으면 0(검은색)으로 설정
    //cv::threshold(gray_frame, binary_frame, 100, 255, cv::THRESH_BINARY);

    // 4. 변환된 영상만 출력 
    //cv::imshow("Grayscale Image", gray_frame);
    //cv::imshow("Binary Image", binary_frame);
    cv::imshow("real image",frame);
    cv::waitKey(1); // 영상 디스플레이를 위한 대기

    
    RCLCPP_INFO(node->get_logger(), "Received Image - Format: %s, Size: %d x %d. Displaying Gray & Binary.", 
                msg->format.c_str(), frame.rows, frame.cols);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camsub_wsl11");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
    
    std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, _1);
    
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed11", qos_profile, fn);
    
    RCLCPP_INFO(node->get_logger(), "Starting CompressedImage subscriber...");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}