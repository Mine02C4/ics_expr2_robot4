#ifndef OPENCV_TEST_CALIBRATION_H_
#define OPENCV_TEST_CALIBRATION_H_

#include <vector>
#include <opencv2/core/core.hpp>

class Calibration
{
public:
  static bool DetectChessboardCorners(cv::Mat& image, cv::Size board_size, std::vector<cv::Point2f>& image_corners);
private:
};

#endif  // OPENCV_TEST_CALIBRATION_H_
