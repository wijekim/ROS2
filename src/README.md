## 실행 결과

### IN LINE
https://youtu.be/B8ycoOGWdFM?si=pRl8ejabE-qS8xeA

### OUT LINE
https://youtu.be/0hAaTeLVuoU?si=jX0tq5Nj7QiMV_2t

## Object.cpp
### 설명
<pre><code>//이진화된 이미지에서 라인 후보들을 찾고, 가장 유력한 라인을 선택
void findObjects(cv::Mat& binary, cv::Point& tmp_pt, cv::Mat& stats, cv::Mat& centroids) {

    cv::Mat labels;
    //stats: 각 x,y,w,h와 면적(area) 정보가 담김
    //centroids: 무게중심(x, y) 좌표
    int cnt = cv::connectedComponentsWithStats(binary, labels, stats, centroids);

    // 1채널(Gray) -> 3채널(BGR) 변환 (컬러로 박스를 그리기 위함)
    cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR);

    int min_index = -1; // 찾은 객체의 번호 저장
    int min_dist = binary.cols; // 초기 거리값은 크게 설정 /최소 거리 저장용

    for (int i = 1; i < cnt; i++) {
        int area = stats.at<int>(i, 4); // 면적


        // 면적이 100픽셀 이하인 작은 점들은 무시
        if (area > 100) { // 노이즈 제거 (면적 100 이하 무시)
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));
            
            // 거리 계산
            int dist = cv::norm(cv::Point(x, y) - tmp_pt);    

            // 가장 가깝고 150 픽셀 이내인 객체 선정
            if (dist < min_dist && dist <= 150) {   
                min_dist = dist;
                min_index = i;
            }
        }
    }

    // 객체를 찾았으면 위치 갱신
    if (min_index != -1 && min_dist <= 150) { 
        //// 라인을 찾았으면, tmp_pt를 현재 찾은 라인의 위치로 갱신
        tmp_pt = cv::Point(cvRound(centroids.at<double>(min_index, 0)), cvRound(centroids.at<double>(min_index, 1)));    
    }
    else {
        // 못 찾았으면 마지막 위치에 점 찍기 /이전 위치(tmp_pt)에 빨간 점만 찍음
        cv::circle(binary, cv::Point(tmp_pt.x, tmp_pt.y), 5, cv::Scalar(0, 0, 255), -1);
    }
}
</code></pre> 

* findObjects()함수

현재 검출된 라인들과 이전 프레임에서의 메인이되는 라인의 무게 중심의 차이를 이용
거리는 150 이하인 경우로 설정하였는데 100일 경우 회전시에 급격한 변화가 생기면서 라인을 놓치는 경우가 생기고 150인 경우에 다른 라인을 메인으로 잡아버리는 경우가 생기기 때문에 거리는 150 미만인 경우로 잡음 



<pre><code>{void drawObjects(cv::Mat& stats, cv::Mat& centroids, cv::Point& tmp_pt, cv::Mat& binary) {
    for (int i = 1; i < stats.rows; i++) {
        int area = stats.at<int>(i, 4);
        if (area > 100) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));

            // 현재 추적 중인 객체는 빨간색, 나머지는 파란색
            if (x == tmp_pt.x && y == tmp_pt.y) { 
                cv::rectangle(binary, cv::Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), cv::Scalar(0, 0, 255));
                cv::circle(binary, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);
            }
            else {
                cv::rectangle(binary, cv::Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), cv::Scalar(255, 0, 0));
                cv::circle(binary, cv::Point(x, y), 5, cv::Scalar(255, 0, 0), -1);
            }
        }
    }
}}</code></pre>  

* drawObjects()
기준점을 표시해주는 함수
tmp_pt : 메인라인의 무게중심 좌표가 저장
메인라인에는 빨간색 바운딩박스와 점을 찍고, 나머지 라인에는 파란색 바운딩박스와 점을 찍음.


<pre><code>{int Errorobject(cv::Mat& binary, cv::Point& tmp_pt) {
    return ((binary.cols / 2) - tmp_pt.x);
}}</code></pre>

* Errorobject() 함수
#### error = (로봇 중앙 x좌표) - (메인라인 중앙 x좌표)
(-)인 경우 : 라인이 오른쪽에 , (+)인 경우 : 라인이 왼쪽에 있음


## sub.cpp
### 전처리
<pre><code>{cv::Rect roi_rect(0, 270, 640, 90);
    cv::Mat roi = frame(roi_rect);

    cv::Mat gray;
    cv::cvtColor(roi, gray, cv::COLOR_BGR2GRAY);

    //결과 = 입력 + (목표평균 - 현재평균)
    cv::Scalar avg_pixel = cv::mean(gray);
    //목표 밝기 설정
    double target_mean = 100.0;

    gray = gray+(target_mean-avg_pixel[0]);

    cv::Mat binary;
    cv::threshold(gray, binary, 150, 255, cv::THRESH_BINARY);}</code></pre>

### 영상 1장 처리하는데 걸리는 시간

pre><code>#include <chrono> //선언 
 auto start_time = steady_clock::now(); //시작 시간 저장
 //측정할 코드
 auto end_time = steady_clock::now(); // 종료시간 저장
 float totalTime = duration<float, std::milli>(end_time - start_time).count(); // 밀리초 단위/시간차 저장</code></pre>

    
