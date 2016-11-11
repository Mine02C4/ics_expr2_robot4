#ifndef OPENCV_TEST_VIDEOTEST_H_
#define OPENCV_TEST_VIDEOTEST_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

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
  std::vector<cv::KeyPoint> prev_keypts_;
  cv::Mat prev_desc_;
};

#endif  // OPENCV_TEST_VIDEOTEST_H_
