#ifndef ROBOT_CORE_CVISION_H_
#define ROBOT_CORE_CVISION_H_

#include <mutex>

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
  CVision();
  bool frame_updated_;
  cv::Mat frame_;
  cv::Mat last_frame_;
  std::mutex frame_mtx_;
  cv::VideoCapture cap_;
  void DetectTargetBlue(cv::Mat & hsv, cv::Mat & mask);
};

#endif  // ROBOT_CORE_CVISION_H_
