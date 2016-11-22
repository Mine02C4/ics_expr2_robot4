#ifndef OPENCV_TEST_RECOGNITION_H_
#define OPENCV_TEST_RECOGNITION_H_

#include <vector>
#include <tuple>

#include <opencv2/features2d/features2d.hpp>

typedef std::tuple<std::vector<cv::KeyPoint>, cv::Mat> FeatureDescription;

class Recognition {
public:
  static void DescribeFeatures(cv::Mat &image, FeatureDescription &fd);
  static void MatchKeyPoints(
    FeatureDescription &fd1,
    FeatureDescription &fd2,
    std::vector<cv::DMatch> &good_matches,
    std::vector<cv::Point2f> &match_point1,
    std::vector<cv::Point2f> &match_point2
  );
private:
};

#endif  // OPENCV_TEST_RECOGNITION_H_
