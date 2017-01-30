#ifndef ROBOT_CORE_CVISION_H_
#define ROBOT_CORE_CVISION_H_

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
  
private:
  CVision();
  cv::Mat frame_;
  cv::VideoCapture cap_;
  void DetectTargetBlue(cv::Mat & hsv, cv::Mat & mask);
};

#endif  // ROBOT_CORE_CVISION_H_
