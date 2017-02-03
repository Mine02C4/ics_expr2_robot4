#ifndef ROBOT_CORE_CVISION_H_
#define ROBOT_CORE_CVISION_H_

#include <mutex>
#include <string>

#include <opencv2/highgui/highgui.hpp>

class CVision
{
public:
  static CVision& getInstance(void) {
    static CVision singleton;
    return singleton;
  }
  int Init();
  //void Finalize();
  bool ReadFrame();
  bool DetectBlueBox(int &area, int& cx, int& cy);
  void FetchFrame();
private:
  const int camera_width_ = 320;
  const int camera_height_ = 240;
  const int pointer_detection_size_ = 80;
  const cv::Point pointer_offset_ = cv::Point(0, 40);
  CVision();
  bool frame_updated_;
  cv::Mat frame_;
  cv::Mat last_frame_;
  std::mutex frame_mtx_;
  cv::VideoCapture cap_;
  void DetectTargetBlue(cv::Mat &hsv, cv::Mat &mask);
  bool DetectPointer(cv::Mat &rgb, cv::Mat &hsv, cv::Point &p, std::string &opt);
};

#endif  // ROBOT_CORE_CVISION_H_
