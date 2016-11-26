#include "Simulator.h"

#include <opencv2/highgui/highgui.hpp>

namespace stub {

Simulator::Simulator() :
  playground_(InitPlayground())
{
}

Rectangle& Simulator::InitPlayground()
{
  return Rectangle(1000, 700);
}

void Simulator::Render()
{
  std::lock_guard<std::mutex> lock(mtx);
  cv::Mat scene(cv::Size(1100, 800), CV_8UC3, cv::Scalar(255, 255, 255));
  playground_.Render(scene);
  auto itr = vobjects_.begin();
  while (itr != vobjects_.end()) {
    itr->get()->Render(scene);
  }
  cv::imshow("Simulator", scene);
}

};  // namespace stub
