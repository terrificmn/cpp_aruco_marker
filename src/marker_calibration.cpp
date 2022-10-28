#include <ros/ros.h>
#include <ros/package.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

//// checker_board, field_size, frame_size는 상황에 맞춰서 변경

int main(int argc, char ** argv) {

    std::vector<cv::String> file_names;
    // need to edit here
    std::string pkg_path = ros::package::getPath("cpp_aruco_marker"); //패키지명 넣어준다
    
    std::string path = pkg_path + "/images/opencv_frame_*.png";
    cv::glob(path, file_names, false);
    // cv::Size pattern_size(25 - 1, 18 - 1);
    /// 안쪽의 깍두기 갯수 (가로, 세로) 전체 갯수에서 안쪽이므로 -1 해준다
    cv::Size pattern_size(10 - 1, 7 - 1);  // 9, 6
    std::vector<std::vector<cv::Point2f>> q(file_names.size());  // image points

    std::vector<std::vector<cv::Point3f>> Q;
    // 1. Generate checkerboard (world) coordinates Q.

    // The board has 25 x 18, fields with a size of 15x15mm
    // 보드 사이즈에 따라서 바꾸기
    // int checker_board[2] = {24, 18};
    // int field_size = 15;
    /// checker_board에는 실제 깍두기 갯수를 넣어준다
    int checker_board[2] = {10, 7};
    int field_size = 23;

    // defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for(int i=1; i<checker_board[1]; i++) {
        for(int j=1; j<checker_board[0]; j++) {
            objp.push_back(cv::Point3f(j * field_size, i * field_size, 0));
        }
    }

    std::vector<cv::Point2f> img_point;
    // Detect feature points

    std::size_t i = 0; // for count
    // 모든 파일을 loop 해서 corner 찾기
    for (auto const &f : file_names) {
        std::cout << std::string(f) << std::endl;

        // 2. Read in the image an call cv::findChessboardCorners()
        cv::Mat img = cv::imread(file_names[i]);
        cv::Mat gray;

        // convert
        cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
        bool pattern_found = cv::findChessboardCorners(gray, pattern_size, q[i], cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK); // default value parameters

        // 3. Use cv::cornerSubPix() to refine the found corner detections
        if(pattern_found) {
            cv::cornerSubPix(gray, q[i], cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
            // size(11,11) 를 포함해서 파라미터들은 opencv의 기본 파라미터 적용
            Q.push_back(objp);

        }

        // 4. Display
        cv::drawChessboardCorners(img, pattern_size, q[i], pattern_found);
        cv::imshow("chessboard detection", img);
        cv::waitKey(0);  /// 0 이면 ESC 를 누르면 다음 진행
        i++;
    }

    cv::Matx33f K(cv::Matx33f::eye()); // intrinsic camera matrix  // calibrateCamera() output으로 사용
    cv::Vec<float, 5> k(0, 0, 0, 0, 0); // distortion coefficients  // calibrateCamera() output으로 사용
    std::vector<cv::Mat> rvecs, tvecs;  /// rotation vectors  /// translation vectors
    std::vector<double> std_intrinsics, std_extrinsics, per_view_errors;
    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 + cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT; // it's stardard value

    // cv::Size frame_size(1440, 1080);  // image size
    // cv::Size frame_size(1280, 720);  // image size for cam
    cv::Size frame_size(1920, 1080);  // image size for robot

    ROS_INFO("Calibration...");

    // 5. Call "float error = cv::calibrateCamear()" with the input coordinates and output parameters as declared above..

    float error = cv::calibrateCamera(Q, q, frame_size, K, k, rvecs, tvecs, flags);
    // K, k 는 추후 사용하게 됨 (setup)

    // result, copy and use it later
    ROS_INFO("calibration result:");
    std::cout << "Peprojection error = " << error << "\nK =\n"
    << K << "\nk=\n" << k << std::endl;

    // Precompute lens correction interpolation
    cv::Mat mapX, mapY;
    cv::initUndistortRectifyMap(K, k, cv::Matx33f::eye(), K, frame_size, CV_32FC1, mapX, mapY);

    // show lens corrected images
    for(auto const &f : file_names) {
        std::cout << std::string(f) << std::endl;

        cv::Mat img = cv::imread(f, cv::IMREAD_COLOR);
        cv::Mat imgUndistored;

        // 6. Remap the image using the precomputed interpolation maps
        cv::remap(img, imgUndistored, mapX, mapY, cv::INTER_LINEAR);

        cv::imshow("undistorted image", imgUndistored);
        cv::waitKey(0);

    }

    return 0;

}
