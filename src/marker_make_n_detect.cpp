#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
    
    cv::Mat img;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250); // predefined dictionary name
    //마커만들기 drawMarker
    // cv::aruco::drawMarker(dict, 21, 400, img, 1); /// 두 번째는 아이디, 세번째 픽셀, 마지막 argument는 보더의 두께
    // 저장
    // cv::imwrite("marker.png", img);
    // 실행해보려면 아래는 주석처리하고(drawDetectedMarkers() 까지) 위에 주석 풀기


    // 이미지일기 -- 만약 아래코드에서 imshow에서 마커가 발견안되면 마커의 안쪽의 갯수를 세어야함, 예를 들어 6x6이면(마지막검은색제외) 위의
    // getPredefinedDictionary의 enum을 수정해준다 
    img = cv::imread("/home/sgtubunamr/catkin_ws/src/aruco_test/board_img/board.png"); 
    
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    cv::aruco::detectMarkers(img, dict, corners, ids);

    if(ids.size() > 0) {
        cv::aruco::drawDetectedMarkers(img, corners, ids);
    }
    

    cv::namedWindow("Out", 0);
    cv::imshow("Out", img);
    cv::waitKey(0);

}