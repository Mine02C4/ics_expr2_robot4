#ifndef OPENCV_TEST_STEREO_H_
#define OPENCV_TEST_STEREO_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
/*
class StereoCamera
{
public:
  void Init();
  void Display();
  StereoCamera();
private:
  cv::VideoCapture right_cap_;
  cv::VideoCapture left_cap_;
  cv::Mat right_frame_, left_frame_;
  cv::Mat cameraMatrix[2], distCoeffs[2];
  cv::Mat R, T, E, F;
  cv::Mat R1, R2, P1, P2, Q;
  cv::Rect validRoi[2];
};
*/
#endif  // OPENCV_TEST_STEREO_H_
