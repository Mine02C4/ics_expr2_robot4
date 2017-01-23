#ifndef OPENCV_TEST_STEREO_H_
#define OPENCV_TEST_STEREO_H_

#include <opencv2/core/core.hpp>

class Stereo
{
public:
  static int CalcDisparity(cv::Mat& left_img, cv::Mat& right_img, cv::Mat& disparity);
  static void RenderDisparity(cv::Mat& disparity, int numberOfDisparities);
  void RenderGLWindow();
  void SetData(cv::Mat& img3d);
  Stereo();
private:
  int gl_window_;
  cv::Mat img3d_;
};

#endif  // OPENCV_TEST_STEREO_H_
