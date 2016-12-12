#define _USE_MATH_DEFINES
#include "Simulator.hpp"

#include <cmath>  

#include <iterator>
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
  RenderSensorVis(scene);
  cv::imshow("Simulator", scene);
}

void Simulator::RenderRobot(cv::Mat scene)
{
  const double robot_size = 40.0;
  auto mat = GetTransformMatrix();
  std::vector<cv::Vec2i> robot = {
    cv::Vec2i(robot_size / -2.0, robot_size / -2.0),
    cv::Vec2i(robot_size / 2.0, robot_size / -2.0),
    cv::Vec2i(robot_size / 2.0, robot_size / 2.0),
    cv::Vec2i(robot_size / -2.0, robot_size / 2.0)
  };
  AffineTransformVector(robot, mat);
  cv::polylines(scene, robot, true, cv::Scalar(0, 200, 200), 2);
  std::vector<cv::Vec2i> dist_sensor;
  std::transform(sensors_.begin(), sensors_.end(), std::back_inserter(dist_sensor),
    [](Sensor s) -> cv::Vec2i {
    return s.position;
  });
  AffineTransformVector(dist_sensor, mat);
  cv::circle(scene, dist_sensor[0], 5.0, cv::Scalar(0, 0, 200));
  cv::circle(scene, dist_sensor[1], 5.0, cv::Scalar(0, 0, 200));
}

void Simulator::RenderSensorVis(cv::Mat scene)
{
  for (auto lp : vsensors_) {
    if (lp != 0) {
      cv::line(scene, lp->first, lp->second, cv::Scalar(0, 255, 0));
    }
  }
}

void Simulator::AffineTransformVector(std::vector<cv::Vec2i> &vec, cv::Mat mat)
{
  for (auto itr = vec.begin(); itr != vec.end(); ++itr) {
    auto v = cv::Vec3d((*itr)[0], (*itr)[1], 1.0);
    *itr = cv::Mat1d(mat * cv::Mat(v));
  }
}

double Simulator::Cross2d(cv::Vec2d v1, cv::Vec2d v2)
{
  return v1[0] * v2[1] - v1[1] * v2[0];
}

bool Simulator::CrossingDetection(Line l1, Line l2)
{
  return
    (Cross2d(l1.second - l1.first, l2.first - l1.first) * Cross2d(l1.second - l1.first, l2.second - l1.first) < 0) &&
    (Cross2d(l2.second - l2.first, l1.first - l2.first) * Cross2d(l2.second - l2.first, l1.second - l2.first) < 0);
}

cv::Mat Simulator::GetTransformMatrix()
{
  auto mat = cv::getRotationMatrix2D(cv::Point(), direction_, 1.0);
  mat.at<double>(0, 2) += x;
  mat.at<double>(1, 2) += y;
  return mat;
}

Line Simulator::GetSensorDetectionLine(int sensor_id)
{
  auto v = cv::Vec3d(sensors_[sensor_id].position[0], sensors_[sensor_id].position[1], 1.0);
  cv::Vec2d spos = cv::Mat1d(GetTransformMatrix() * cv::Mat(v));
  auto direction = (sensors_[sensor_id].direction + direction_) * M_PI / 180.0;
  auto dpos = spos + (cv::Vec2d(sin(direction), cos(direction)) * sensor_max_range);
  return Line(spos, dpos);
}

double Simulator::GetDistance(int sensor_id)
{
  std::lock_guard<std::mutex> lock(mtx_);
  double dist = INFINITY;  
  Line dline = GetSensorDetectionLine(sensor_id);
  vsensors_[sensor_id].reset(new Line(dline));
  auto edges = playground_.GetEdges();
  for (auto edge : edges) {
    if (CrossingDetection(dline, edge)) {
      auto e = edge.second - edge.first;
      double b1 = abs(Cross2d(e, dline.first - edge.first));
      double b2 = abs(Cross2d(e, dline.second - edge.first));
      double t = b1 / (b1 + b2);
      double d = cv::norm(dline.second - dline.first) * t;
      dist = std::min(dist, d);
    }
  }
  return dist == INFINITY ? -1.0 : dist;
}



};  // namespace stub
