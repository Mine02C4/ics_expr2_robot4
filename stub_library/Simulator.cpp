#include "Simulator.h"

#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stub {

Simulator::Simulator() :
  playground_(Rectangle(1000, 700))
{
  auto th = std::thread([this] {
    while (true) {
      if (cv::waitKey(10) == 27) { // ESC
        break;
      }
      Render();
    }
  });
  th.detach();
}

void Simulator::Render()
{
  std::lock_guard<std::mutex> lock(mtx_);
  cv::Mat scene(cv::Size(1000, 800), CV_8UC3, cv::Scalar(255, 255, 255));
  playground_.Render(scene);
  for (auto itr = vobjects_.begin(); itr != vobjects_.end(); ++itr) {
    itr->get()->Render(scene);
  }
  const double robot_size = 40.0;
  auto mat = cv::getRotationMatrix2D(cv::Point(), direction_, 1.0);
  mat.at<double>(0, 2) += x;
  mat.at<double>(1, 2) += y;
  std::vector<cv::Vec2i> robot = {
    cv::Vec2i(robot_size / -2.0, robot_size / -2.0),
    cv::Vec2i(robot_size / 2.0, robot_size / -2.0),
    cv::Vec2i(robot_size / 2.0, robot_size / 2.0),
    cv::Vec2i(robot_size / -2.0, robot_size / 2.0)
  };

  for (auto itr = robot.begin(); itr != robot.end(); ++itr) {
    auto vec = cv::Vec3d((*itr)[0], (*itr)[1], 1.0);
    *itr = cv::Mat1d(mat * cv::Mat(vec));
  }
  cv::polylines(scene, robot, true, cv::Scalar(0, 200, 200), 2);
  cv::imshow("Simulator", scene);
}

};  // namespace stub
