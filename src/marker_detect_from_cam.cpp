#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
    
    cv::Mat img;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_100); // predefined dictionary name
    cv::namedWindow("Out", 0);
    cv::VideoCapture cap; // vidoe는 터미널에서 ls -l /dev/video* 로 확인 가능, 귀찮으면 -1 (모든 비디오)
    cap.open(0);

    while(cap.grab()) {
        cap>>img;
        std::vector<std::vector<cv::Point2f>> corners;
        std::vector<int> ids;
        cv::aruco::detectMarkers(img, dict, corners, ids);

        /// 기본색으로 마커를 찾음, (색은 변경 못한다고 함)
        if(ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(img, corners, ids);
        }

        // 색 바꾸기 - detectMarkers()를 사용한 후 for loop로 색을 변경 가능 . Scalar()에서 색을 지정해준다 
        if(corners.size() > 0) {
            for(int i=0; i<(int)corners.size(); i++) {
                int x = corners[i][0].x + corners[i][2].x;
                int y = corners[i][0].y + corners[i][2].y;
                cv::putText(img, "id=" + std::to_string(ids[i]), cv::Point(x/2, y/2), 1, 1, cv::Scalar(0, 255, 255), 2, false); 
            }
        }

        cv::imshow("Out", img);
        char key = (char) cv::waitKey(1);
        if(key == 27) {  // ESC
            break;
        }
        
    }    

}