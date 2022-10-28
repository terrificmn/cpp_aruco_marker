#include <ros/ros.h>
#include <ros/package.h>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

///////////////////////////////////////////// test 해야함
///////////////////////////////////////////// test 해야함

int main(int argc, char ** argv) {

    cv::Mat camera_matrix, dist_coeffs, input_image, output_image, image_copy;
    std::string pkg_path = ros::package::getPath("cpp_aruco_marker"); //패키지명 넣어준다

    cv::FileStorage fs(pkg_path + "/camera_params/calibration_params.yml", cv::FileStorage::READ);
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> dist_coeffs;    

    std::cout << "loaded camera_matrix: "<< camera_matrix << std::endl;
    std::cout << "loaded distortion_coefficients: "<< dist_coeffs << std::endl;

    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250); 
    std::vector<std::vector<cv::Point2f>> marker_corners;
    std::vector<int> ids;
    
    input_image = cv::imread("/home/sgtubunamr/catkin_ws/src/cpp_aruco_marker/board_img/board.png"); 
    cv::aruco::detectMarkers(input_image, dictionary, marker_corners, ids);
    input_image.copyTo(image_copy);

    if (ids.size() > 0) {
        cv::aruco::drawDetectedMarkers(input_image, marker_corners, ids);
        cv::aruco::estimatePoseSingleMarkers(marker_corners, 0.05, camera_matrix, dist_coeffs, rvecs, tvecs);
    }
    //markerConers는 detectMarkers() 함수를 상요하면 리턴되는 vector
    //0.05 두번째 팔미터는 마커의 사이즈(미터), pose의 translation vectors 도 같은 단위
    //cameraMatrix, distCoeffs 는 칼리브레이션으로 나온 각각 rotation vector, translation vector
    //markercorners()에 사용하게 됨
    cv::aruco::drawAxis(image_copy, camera_matrix, dist_coeffs, rvecs[0], tvecs[0], 0.1);
    cv::imshow("Pose estimation", image_copy);

    char key = (char)cv::waitKey(0);
        if (key == 27) 
            return 0;

    

    // // 이미지로 확인
    // input_image.copyTo(output_image);
    // for (int i=0; i < rvecs.size(); i++) {
    //     auto rvec = rvecs[i];
    //     auto tvec = tvecs[i];
    //     cv::drawFrameAexs(output_image, camera_matrix, dist_coeffs, rvec, tvec, 0.1);
    // }
    //  // outputImage는 마커가 그려질 이미지(출력)
    //  // 마지막 파라미터는 asix의 length tvec과의 같은 단위 (meters)

    //// 동영상
    // cv::VideoCapture inputVideo;
    // inputVideo.open(0);

    // cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);

    // while(inputVideo.grab()) {
    //     cv::Mat image, imageCopyOutput;
    //     inputVideo.retrieve(image);
    //     image.copyTo(imageCopyOutput);

    //     std::vector<int> ids;
    //     std::vector<std::vector<cv::Point2f>> corners;
    //     cv::aruco::detectMarkers(image, dictionary, corners, ids);

    //     // 하나의 marker라도 발견되면 실행
    //     if(ids.size() > 0) {
    //         cv::aruco::drawDetecedMarkers(imageCopyOutput, corners, ids);

    //         std::vector<cv::Vec3d> rvecs, tvecs;
    //         cv::aruco::estimatePoseSignleMarkers(markerCorners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
            

    //         // draw asix
    //         for(int i=0; i < ids.size(); i++) {
    //             cv::drawFrameAxes(imageCopyOutput, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
    //         }

            
    //     }
    // }

}