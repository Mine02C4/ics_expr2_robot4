#ifndef OPENCV_TEST_RECOGNITION_H_
#define OPENCV_TEST_RECOGNITION_H_

#include <vector>

#include <opencv2/features2d/features2d.hpp>

class Recognition {
public:
  static void MatchKeyPoints(
    std::vector<cv::KeyPoint>&,
    cv::Mat&,
    std::vector<cv::KeyPoint>&,
    cv::Mat&,
    std::vector<cv::Point2f>&,
    std::vector<cv::Point2f>&
  );
private:
};

#endif  // OPENCV_TEST_RECOGNITION_H_
