#ifndef STUB_LIBRARY_SIMULATOR_H_
#define STUB_LIBRARY_SIMULATOR_H_

#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <utility>

#include <opencv2/core/core.hpp>

#include "VirtualObject.h"

namespace stub {

struct Sensor {
  cv::Vec2i position;
  double direction;
};

class Simulator
{
public:
  static Simulator& getInstance(void) {
    static Simulator singleton;
    return singleton;
  }
  double direction_ = 30.0; // in degree
  double x = 100.0, y = 100.0;
  std::mutex mtx_;
  double GetDistance(int sensor_id);
private:
  Rectangle playground_;
  std::list<std::shared_ptr<VirtualObject>> vobjects_;

  std::vector<Sensor> sensors_ = {
    { cv::Vec2i(16, 16), 180 }, // LEFT_FRONT
    { cv::Vec2i(-16, 16), 180 }, // RIGHT_FRONT
  };
  Simulator();
  void Render();
  void RenderRobot(cv::Mat scene);
  static void AffineTransformVector(std::vector<cv::Vec2i> &vec, cv::Mat mat);
  static double Cross2d(cv::Vec2d v1, cv::Vec2d v2);
  static bool CrossingDetection(Line l1, Line l2);
  cv::Mat GetTransformMatrix();
};

};  // namespace stub

#endif  // STUB_LIBRARY_SIMULATOR_H_
