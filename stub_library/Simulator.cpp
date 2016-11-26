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
  RenderRobot(scene);

  cv::imshow("Simulator", scene);
}

void Simulator::RenderRobot(cv::Mat scene)
{
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
  AffineTransformVector(robot, mat);
  cv::polylines(scene, robot, true, cv::Scalar(0, 200, 200), 2);
  std::vector<cv::Vec2i> dist_sensor{
    cv::Vec2i(robot_size * -0.4, robot_size / 2.0),
    cv::Vec2i(robot_size * 0.4, robot_size / 2.0)
  };
  AffineTransformVector(dist_sensor, mat);
  cv::circle(scene, dist_sensor[0], 5.0, cv::Scalar(0, 0, 200));
  cv::circle(scene, dist_sensor[1], 5.0, cv::Scalar(0, 0, 200));
}

void Simulator::AffineTransformVector(std::vector<cv::Vec2i> &vec, cv::Mat mat)
{
  for (auto itr = vec.begin(); itr != vec.end(); ++itr) {
    auto v = cv::Vec3d((*itr)[0], (*itr)[1], 1.0);
    *itr = cv::Mat1d(mat * cv::Mat(v));
  }
}

};  // namespace stub
