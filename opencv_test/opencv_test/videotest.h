#ifndef OPENCV_TEST_VIDEOTEST_H_
#define OPENCV_TEST_VIDEOTEST_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "recognition.h"

class VideoTest {
public:
  VideoTest();
  ~VideoTest();
  int Init();
  void ReadFrame();
  void TrackFeatures();
  void DetectionByColor();
private:
  cv::Mat gray_img_;
  cv::VideoCapture cap_;
  cv::Mat frame_, disp_features_;
  FeatureDescription prev_fd_;
  cv::Mat target_img_;
  FeatureDescription target_fd_;
};

#endif  // OPENCV_TEST_VIDEOTEST_H_
