#include <ros/ros.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

///////////////////////////////////////////// test 해야함
///////////////////////////////////////////// test 해야함

int main(int argc, char ** argv) {

    cv::Mat cameraMatrix, distCoeffs;
    readCameraParameters(filename, cameraMatrix, distCoeffs);

    cv::FileStorage fs("calibration_params.yml", cv::FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;    

    //yaml 파일은 이런 형식으로 되어 있음
    // %YAML:1.0
    // ---
    // calibration_time: "Tue 09 Apr 2019 04:02:02 PM EDT"
    // image_width: 640
    // image_height: 480
    // flags: 0
    // camera_matrix: !!opencv-matrix
    //  rows: 3
    //  cols: 3
    //  dt: d
    //  data: [ 5.2036144200036904e+02, 0., 3.0255876920712751e+02, 0.,
    //     5.1834337983691967e+02, 2.4278187493920279e+02, 0., 0., 1. ]
    // distortion_coefficients: !!opencv-matrix
    //  rows: 1
    //  cols: 5
    //  dt: d
    //  data: [ 1.0303738535136008e-01, -2.7013831868935040e-01,
    //     -2.5505644429536711e-03, -6.9928818760003142e-03,
    //     1.6547444868453295e-01 ]
    // avg_reprojection_error: 1.2025225757564226e+00

    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
    //markerConers는 detectMarkers() 함수를 상요하면 리턴되는 vector
    //0.05 두번째 팔미터는 마커의 사이즈(미터), pose의 translation vectors 도 같은 단위
    //cameraMatrix, distCoeffs 는 칼리브레이션으로 나온 각각 rotation vector, translation vector
    //markercorners()에 사용하게 됨
    
    // 이미지로 확인
    inputImage.copyTo(outputImage);
    for (int i=0; i < rvecs.size(); i++) {
        auto rvec = rvces[i];
        auto tvec = tvecs[i];
        cv::drawFrameAxes(outputImage, cameraMatrix, distCoeffs, rvec, tvec, 0.1);
    }
     // outputImage는 마커가 그려질 이미지(출력)
     // 마지막 파라미터는 asix의 length tvec과의 같은 단위 (meters)


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