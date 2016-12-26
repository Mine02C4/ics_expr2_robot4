#include "calibration.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

bool Calibration::DetectChessboardCorners(cv::Mat& image, cv::Size board_size, std::vector<cv::Point2f>& image_corners)
{
  cv::Mat gray_image;
  bool found = findChessboardCorners(image, board_size, image_corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
  cv::cvtColor(image, gray_image, CV_BGR2GRAY);
  cornerSubPix(gray_image, image_corners, cv::Size(9, 9), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.1));
  return found;
}
