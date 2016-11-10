#ifndef OPENCV_TEST_VIDEOTEST_H_
#define OPENCV_TEST_VIDEOTEST_H_

#include <opencv2/highgui/highgui.hpp>

class VideoTest {
public:
  VideoTest();
  ~VideoTest();
  int Init();
  void ReadFrame();
  void TrackFeatures();
private:
  cv::Mat gray_img_;
  cv::VideoCapture cap_;
  cv::Mat frame_, disp_features_;
};

#endif  // OPENCV_TEST_VIDEOTEST_H_
