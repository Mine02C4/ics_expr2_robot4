#ifndef OPENCV_TEST_STEREO_H_
#define OPENCV_TEST_STEREO_H_

#include <opencv2/core/core.hpp>

class Stereo
{
public:
  static int CalcDisparity(cv::Mat& left_img, cv::Mat& right_img, cv::Mat& disparity);
  static void RenderDisparity(cv::Mat& disparity, int numberOfDisparities);
private:
};

#endif  // OPENCV_TEST_STEREO_H_
